/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		: 	aud_mute_func.c
	モジュール名	: 	Audioモジュール
	機能概略		: 	ミュート制御処理
-------------------------------------------------------------------------------
修正履歴
	番号		日付		担当者		関数名			修正内容
	【000】		12/07/13	leejh						新規作成
 	【001】		12/09/25	leejh		Aud_adjust_mute_info(変更)
										Aud_init_mute_func(変更)
********************************************************************************/
#define		_AUD_MUTE_FUNC_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#include	"../main/MA_inc.h"
#include	"../power/PW_inc.h"

#include	"aud_def.h"
#include	"aud_main.h"
#include 	"aud_sys.h"
#include	"aud_ext.h"
#include	"aud_mute_func.h"
#include	"aud_dev_func.h"
#include	"aud_beep_func.h"

/*---------------------------------------------------------------------------*
 *	内部定数定義														 	 *
 *---------------------------------------------------------------------------*/
#define CAUD_MUTE_SEQ_START		0x00		/* Mute処理シーケンス開始		 */
#define CAUD_MUTE_SEQ_OFF		0xff		/* Mute処理シーケンス終了		 */
#define CAUD_MUTE_INVALID		0xff		/* 無効データ					 */

#define CAUD_MUTE_SEQ_50MS		((50/5) + 1)	/* 50ms MUTEシーケンスタイマ */
#define	CAUD_MUTE_SEQ_250MS		((250/5) + 1)	/* 250ms MUTEシーケンスタイマ*/

#define	CAUD_POWER_BATT_MUTE	0x00000001
#define	CAUD_POWER_MUTE			0x00000002
#define	CAUD_MODE_CHANG_MUTE	0x00000004
#define	CAUD_AUDIO_MUTE			0x00000008
#define	CAUD_TUNER_MODULE_MUTE	0x00010000
#define	CAUD_AUX_MODULE_MUTE	0x00100000
#define	CAUD_BAND_CHANG_MUTE	0x01000000

#define	CAUD_MUTE_MODE_MODULE_MASK	0x0000FFFFU	/* モードを持たないモジュールのmute要求マスク	*/

/*---------------------------------------------------------------------------*
 *	内部構造体定義														 	 *
 *---------------------------------------------------------------------------*/
typedef struct {
	BYTE			seq_id;		/* mute処理シーケンス番号						 */
	BYTE			reserved;	/* padding										 */
	LONG			mute_req;	/* mute要求		0: mute off要求	1: mute on要求	 */
} TAUD_MUTE_INFO;

/*---------------------------------------------------------------------------*
 *	内部変数定義														 	 *
 *---------------------------------------------------------------------------*/
static TAUD_MUTE_INFO	aud_mute_info;			/* mute制御制御情報			*/
static TAUD_AUDIO_INFO	*aud_aud_info;

/*---------------------------------------------------------------------------*
 *	マクロ定義															 	 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *	プロトタイプ定義													 	 *
 *---------------------------------------------------------------------------*/
static LONG Aud_get_mute_req_id( BYTE );
static LONG Aud_get_curt_mode_mute( void );

static void Aud_drive_sys_mute( BYTE );
static void Aud_drive_data_mute( BYTE );

static void Aud_muteon_seq1( void );
static void Aud_muteon_seq2( void );
static void Aud_muteon_seqend( void );
static void Aud_muteON_main( void );

static void Aud_muteoff_seq1( void );
static void Aud_muteoff_seq2( void );
static void Aud_muteoff_seqend( void );
static void Aud_muteOFF_main( void );

static BOOL Aud_is_mute_on_req( LONG );
static BOOL Aud_is_mute_off_req( void );
static BOOL Aud_is_mute_req( void );
static BOOL Aud_is_mute_seq( void );

/******************************************************************************
	関数名		：Aud_get_mute_state_list
	引数		：
	戻り値		：
	機能概略	：mute状態リスト取得
------------------------------------------------------------------------------
	修正履歴
 * 
	 番号		日付		担当者		修正内容
******************************************************************************/
TAUD_MUTE_FLAG *Aud_get_mute_state_list(void)
{
	return(&aud_mute_state);
}

/******************************************************************************
	関数名		：Aud_is_on_mute
	引数		：なし
	戻り値		：BOOL	:	TRUE  => mute on中
				：			FALSE => mute off中
	機能概略	：現在がmute on中であるかを通知する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		06/08/28	ISHIHARA	新規作成
******************************************************************************/
BOOL Aud_is_on_mute( void )
{
	/* mute onが完了しているか? */
	if ((aud_mute_state.curt_mute == ON) &&
		(aud_mute_info.seq_id     == CAUD_MUTE_SEQ_OFF))
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

/******************************************************************************
	関数名		：Aud_mute_status
	引数		：なし
	戻り値		：BYTE	CAUD_MUTE_ON_END	：MUTE ON処理完了
						CAUD_MUTE_OFF_END	：MUTE OFF処理完了
						CAUD_MUTE_ON_BUSY	：MUTE ON処理中
						CAUD_MUTE_OFF_BUSY	：MUTE OFF処理中
	機能概略	：mute状態確認処理
	説明		：muteの各状態を取得
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		09/06/04	lirongjun	新規作成
******************************************************************************/
BYTE Aud_mute_status( void )
{
	BYTE status;
	
	if(( aud_mute_state.curt_mute == ON ) &&
		( aud_mute_info.seq_id == CAUD_MUTE_SEQ_OFF ))
	{
		status = CAUD_MUTE_ON_END;
	}
	else if(( aud_mute_state.curt_mute == OFF ) &&
		( aud_mute_info.seq_id == CAUD_MUTE_SEQ_OFF ))
	{
		status = CAUD_MUTE_OFF_END;
	}
	else if(( aud_mute_state.curt_mute == OFF ) &&
		( aud_mute_info.seq_id != CAUD_MUTE_SEQ_OFF ))
	{
		status = CAUD_MUTE_ON_BUSY;
	}
	else
	{
		status = CAUD_MUTE_OFF_BUSY;
	}
	return( status );
}

/******************************************************************************
	関数名		：Aud_get_mute_req_id
	引数		：BYTE	mute_id		:	モジュールmute id
	戻り値		：LONG	module_id	:	mute flag
	機能概略	：モジュールから受け取ったmute idをmute flagに変換する関数
				　※各モジュールから毎回コールされるので、ROMテーブルに
					しない事!（処理負荷大）
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static LONG Aud_get_mute_req_id( BYTE mute_id )
{
	LONG module_id;

	/* 戻り値を初期化する */
	module_id = (LONG) 0;

	switch ( mute_id )
	{
		case CAUD_MUTE_PWR_DT_MUTE:
			module_id = CAUD_POWER_MUTE;
			break;

		case CAUD_MUTE_MAIN_DT_MUTE:
			module_id = CAUD_MODE_CHANG_MUTE;
			break;

		case CAUD_MUTE_AUD_NR_MUTE:
			module_id = CAUD_AUDIO_MUTE;
			break;

		case CAUD_MUTE_TUN_NR_MUTE:
			module_id = CAUD_TUNER_MODULE_MUTE;
			break;

		case CAUD_MUTE_AUX_NR_MUTE:
			module_id = CAUD_AUX_MODULE_MUTE;
			break;

		case CAUD_MUTE_BANDCHG_NR_MUTE:
			module_id = CAUD_BAND_CHANG_MUTE;
			break;
	
		default:
			module_id = (LONG) 0;
			break;
	}
	return ( module_id );
}

/******************************************************************************
	関数名		：Aud_get_curt_mode_mute
	引数		：なし
	戻り値		：LONG	act_mute_id	:	カレントモードのmute flag
	機能概略	：カレントモードのmute flagを通知する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static LONG Aud_get_curt_mode_mute( void )
{
	LONG act_mute_id;

	act_mute_id = (LONG) 0;

	switch ( Ma_current_mode())
	{
		case CMA_TUNER:
			act_mute_id = (LONG) ( CAUD_TUNER_MODULE_MUTE | CAUD_BAND_CHANG_MUTE );
			break;
			
		case CMA_AUX:
			act_mute_id = (LONG) CAUD_AUX_MODULE_MUTE;
			break;
			
		case CMA_ISR:
			act_mute_id = (LONG) CAUD_TUNER_MODULE_MUTE;
			break;
			
		default:
			break;
	}
	return ( act_mute_id );
}

/******************************************************************************
	関数名		：Aud_drive_sys_mute
	引数		：BYTE	on_off		ON	：SYS MUTE ON
									OFF	：SYS MUTE OFF
	機能概略	：SYS MUTEを制御する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_drive_sys_mute( BYTE on_off )
{	
	if (( on_off == ON ) && ( aud_mute_state.sys_mute == OFF ))
	{
		/* sys mute on	*/
		MIO_OUT_pSYS_MUTE( LOW );
		
		aud_mute_state.sys_mute = ON;
	}
	else if (( on_off == OFF ) && ( aud_mute_state.sys_mute == ON ))
	{
		/* sys mute off	*/
		MIO_OUT_pSYS_MUTE( HIGH );
		
		aud_mute_state.sys_mute = OFF;
	}
	else	/* Exception */
	{
		;
	}
}

/******************************************************************************
	関数名		：Aud_drive_data_mute
	引数		：BYTE	on_off		ON	：DATA MUTE ON
									OFF	：DATA MUTE OFF
	戻り値		：なし
	機能概略	：DATA MUTEを制御する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_drive_data_mute( BYTE on_off )
{
	BYTE snd_flg;
	
	snd_flg = ON;			/* デフォルトは送信許可 */
	
	if (( on_off == ON ) && ( aud_mute_state.data_mute == OFF ))
	{
		/* data mute on */
		aud_mute_state.data_mute = ON;
	}
	else if (( on_off == OFF ) && ( aud_mute_state.data_mute == ON ))
	{
		/* data mute off */	
		aud_mute_state.data_mute = OFF;
	}
	else	/* Exception */
	{
		snd_flg = OFF;		/* 送信禁止	*/
	}
	
	if ( snd_flg  == ON )	/* Flag Onは送信要求 */
	{
		Aud_ctrl_ASP( CAUD_ASP_VOL );
	}
}

/******************************************************************************
	関数名		：Aud_adjust_mute_info
	引数		：BYTE	module_id	:	mute要求モジュールID
				：BYTE	on_off		:	Mute ON/OFF
	戻り値		：なし
	機能概略	：Mute要求を受付ける
				  当該関数ではmute on/off要求のみを受け付ける。
				  要求が受付可能かはmuteメイン処理で判別する。
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_adjust_mute_info( BYTE mute_id, BYTE on_off )
{	
	/* batt off mute要求(電源) */
	if ( mute_id == CAUD_MUTE_PWR_DA_MUTE )
	{
		if ( on_off == ON )
		{
			/* batt off mute on		*/
			Aud_drive_sys_mute( ON );

			/* mute on設定 */
			aud_mute_info.mute_req |= CAUD_POWER_BATT_MUTE;
		}
		else
		{
			/* batt off mute off								 */
			/* 	※ここではフラグのみクリアしmute処理メインで	 */
			/* 	  他のモジュールがmute off処理をスタートさせる	 */
			aud_mute_info.mute_req &= ~CAUD_POWER_BATT_MUTE;
		}
	}
	/* その他モジュールからのmute要求 */
	else
	{
		if ( on_off == ON )
		{
			/* mute onフラグを設定する */
			aud_mute_info.mute_req |= (LONG) Aud_get_mute_req_id( mute_id );
		}
		else
		{
			/* mute onフラグをクリアする */
			aud_mute_info.mute_req &= (LONG) ~Aud_get_mute_req_id( mute_id );
		}
	}
}

/******************************************************************************
	関数名		：Aud_muteon_seq1
				  Aud_muteon_seq2
				  Aud_muteon_seqend
	引数		：なし
	戻り値		：なし
	機能概略	：MUTE ONシーケンス処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_muteon_seq1( void )
{
	/* sys mute on */
	Aud_drive_sys_mute( ON );
	
	/* SEQ ID更新 */
	aud_mute_info.seq_id++;

	/* インターバルタイマ50ms設定 */
	Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_PMEUT_TIM, CAUD_MUTE_SEQ_50MS );
}

static void Aud_muteon_seq2( void )
{
	/* インターバルタイマタイムアウト */
	if ( Aud_get_timer(CAUD_CNT_05MS, CAUD_C05MS_PMEUT_TIM ) == 0)
	{
		/* Data Mute On */
		Aud_drive_data_mute ( ON );
	
		/* SEQ ID更新 */
		aud_mute_info.seq_id++;
		
		/* インターバルタイマ設定 */
		Aud_set_timer(CAUD_CNT_05MS, CAUD_C05MS_PMEUT_TIM, 0 );
	}
}

static void Aud_muteon_seqend( void )
{
	/* インターバルタイマタイムアウト */
	if ( Aud_get_timer( CAUD_CNT_05MS, CAUD_C05MS_PMEUT_TIM ) == 0 )
	{
		/* MUTE OFFシーケンス処理終了 */
		aud_mute_info.seq_id     = CAUD_MUTE_SEQ_OFF;
		aud_mute_state.onoff_seq = OFF;
		
		aud_mute_state.curt_mute = ON;
	}
}

/******************************************************************************
	関数名		：Aud_muteON_main
	引数		：なし
	戻り値		：なし
	機能概略	：MUTE ONメイン処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_muteON_main( void )
{
	switch ( aud_mute_info.seq_id )
	{
		case 0:
			Aud_muteon_seq1();
			break;

		case 1:
			Aud_muteon_seq2();
			break;

		case 2:
			Aud_muteon_seqend();
			break;

		default:
			break;
	}
}

/******************************************************************************
	関数名		：Aud_muteoff_seq1
				  Aud_muteoff_seq2
				  Aud_muteoff_seqend
	引数		：なし
	戻り値		：なし
	機能概略	：MUTE OFFシーケンス処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_muteoff_seq1( void )
{
	/* Data Mute Off */
	Aud_drive_data_mute( OFF );

	/* SEQ ID更新 */
	aud_mute_info.seq_id++;

	/* インターバルタイマ設定 */
	Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_PMEUT_TIM, CAUD_MUTE_SEQ_250MS );
}

static void Aud_muteoff_seq2( void )
{
	if ( Aud_get_timer( CAUD_CNT_05MS, CAUD_C05MS_PMEUT_TIM ) == 0 )
	{
		/* Sys Mute Off */
		Aud_drive_sys_mute( OFF );

		/* SEQ ID更新 */
		aud_mute_info.seq_id++;

		/* インターバルタイマ設定 */
		Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_PMEUT_TIM, 0 );
	}
}

static void Aud_muteoff_seqend( void )
{
	if (Aud_get_timer(CAUD_CNT_05MS, CAUD_C05MS_PMEUT_TIM) == 0)
	{		
		/* MUTE OFFシーケンス処理終了 */
		aud_mute_info.seq_id     = CAUD_MUTE_SEQ_OFF;
		aud_mute_state.onoff_seq = OFF;

		/* mute off確定 */
		aud_mute_state.curt_mute = OFF;	
		
		/* インターバルタイマ設定(MUTE ENDして50ms後にBEEPを流すため) */
		Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_AMUTE_END_TIM, CAUD_MUTE_SEQ_50MS );
	}
}
/******************************************************************************
	関数名		：Aud_muteOFF_main
	引数		：なし
	戻り値		：なし
	機能概略	：MUTE OFFメイン処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_muteOFF_main( void )
{
	switch ( aud_mute_info.seq_id )
	{
		case 0:
			Aud_muteoff_seq1();
			break;

		case 1:
			Aud_muteoff_seq2();
			break;

		case 2:
			Aud_muteoff_seqend();
			break;

		default:
			break;
	}
}

/******************************************************************************
	関数名		：Aud_is_mute_on_req
	引数		：なし
	戻り値		：BOOL	:	TRUE  => mute on処理可
				：			FALSE => mute on処理不可
	機能概略	：mute on要求が受付可能かを確認する関数
				  ※受付可能条件
					mute on処理シーケンス中でない
					mecha mute on要求で,bluetooth mode中でない
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static BOOL Aud_is_mute_on_req( LONG mute_info )
{
	BOOL state;

	state = FALSE;

	/* mute onシーケンス処理中はmute on起動しない */
	if (( aud_mute_info.seq_id != CAUD_MUTE_SEQ_OFF ) &&
		( aud_mute_state.onoff_seq == ON ))
	{	
		state = FALSE;
	}
	else
	{
		/* 電源系のmute on要求 */
		if (( mute_info & (LONG)( CAUD_POWER_BATT_MUTE + CAUD_POWER_MUTE )) == (LONG) ON )
		{
			/* mute on処理シーケンス開始設定 */
			aud_mute_info.seq_id = CAUD_MUTE_SEQ_START;
			aud_mute_state.onoff_seq = ON;
			state = TRUE;
		}
		/* AUDIO系のmute on要求 */
		else if (( mute_info & (LONG )~CAUD_AUDIO_MUTE ) == (LONG) OFF )
		{
			/* mute on処理シーケンス開始設定 */
			aud_mute_info.seq_id     = CAUD_MUTE_SEQ_START;
			aud_mute_state.onoff_seq = ON;
			state                    = TRUE;
		}
		/* その他 */
		else
		{
			/* mute on処理シーケンス開始設定 */
			aud_mute_info.seq_id     = CAUD_MUTE_SEQ_START;
			aud_mute_state.onoff_seq = ON;
			state                    = TRUE;
		}
	}
	return ( state );
}

/******************************************************************************
	関数名		：Aud_is_mute_off_req
	引数		：なし
	戻り値		：BOOL	:	TRUE  => mute off処理可
				：			FALSE => mute off処理不可
	機能概略	：mute off要求が受付可能かを確認する関数
				  ※受付可能条件
					Power on状態中
					mute off処理シーケンス中ではない
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
	【001】		12/10/19	leejh		BEEP(MUTE制御有り)中では制御しない分岐追加 
******************************************************************************/
static BOOL Aud_is_mute_off_req( void )
{
	BOOL	state;

	/* 戻り値を初期化 */
	state = FALSE;

	/* Power on状態中 */
	if ( Pw_Power_status_get() == ON ) 
	{
		/* mute offシーケンス処理中はmute off起動しない */
		if ((aud_mute_info.seq_id     != CAUD_MUTE_SEQ_OFF) &&
			(aud_mute_state.onoff_seq == OFF))
		{
			state = FALSE;
		}
		/* BEEP(MUTE制御有り)中では制御しない */
		else if (Aud_is_beep_mute() == TRUE )
		{
			state = FALSE;
		}
		else
		{
			/* mute off処理シーケンス開始設定 */
			aud_mute_info.seq_id     = CAUD_MUTE_SEQ_START;
			aud_mute_state.onoff_seq = OFF;
			state                    = TRUE;
		}
	}
	return( state );
}

/******************************************************************************
	関数名		：Aud_is_mute_seq
	引数		：なし
	戻り値		：BOOL	:	TRUE  => mute要求有効
				：			FALSE => mute要求無効
	機能概略	：mute要求チェック処理
				  カレントモードからのmute要求及びモードを持たないモジュール
				  (power, main, etc)からのmute要求をチェックする。
				  ※カレントモード以外からのmute要求は無視する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static BOOL Aud_is_mute_req( void )
{
	BOOL	state;
	LONG	mute_state;

	/* 戻り値を初期化する */
	state = FALSE;

	/* カレントモードからのmute要求を抽出する */
	mute_state  = ( aud_mute_info.mute_req & Aud_get_curt_mode_mute());

	/* モードを持たないモジュールからのmute要求を抽出する */
	mute_state |= ( aud_mute_info.mute_req & CAUD_MUTE_MODE_MODULE_MASK );
	
	/* 現在mute on状態 */
	if ( aud_mute_state.curt_mute == ON )
	{
		/* 	カレントモード以外のモジュールを除く				 */
		/* 	全てのモジュールがmute off要求(mute on要求がない)	 */
		if ( mute_state == (LONG) OFF )
		{
			/* mute off要求が受付可能かをチェックする */
			state = Aud_is_mute_off_req();
		}
	}
	
	/* 現在mute off状態 */
	else
	{
		/* mute on要求あり */
		if ( mute_state != (LONG) OFF )
		{
			/* mute on要求が受付可能かをチェックする */
			state = Aud_is_mute_on_req( mute_state );
		}
	}
	return( state );
}

/******************************************************************************
	関数名		：Aud_is_mute_seq
	引数		：なし
	戻り値		：BOOL	:	TRUE  => mute処理あり
				：			FALSE => mute処理なし
	機能概略	：mute処理の必要があるかを確認する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		06/03/21	ISHIHARA	新規作成
******************************************************************************/
static BOOL Aud_is_mute_seq( void )
{
	BOOL	state;

	state = FALSE;

	/* mute要求チェック */
	state = Aud_is_mute_req();

	/* mute処理シーケンスチェック */
	if (aud_mute_info.seq_id != CAUD_MUTE_SEQ_OFF )
	{
		state = TRUE;
	}
	return( state );
}

/******************************************************************************
	関数名		：Aud_mute_main
	引数		：なし
	戻り値		：なし
	機能概略	：muteメイン処理
	説明		：audioから毎回コールされる関数。
				：mute処理の必要があるかを確認し、必要がなければ処理を
				：スキップする
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		06/03/21	ISHIHARA	新規作成
******************************************************************************/
void Aud_mute_main(void)
{
	/* mute処理の必要があるかを確認する */
	if (Aud_is_mute_seq() == TRUE)
	{
		if (aud_mute_state.onoff_seq == ON)
		{
			/* mute on処理 */
			Aud_muteON_main();
		}
		else
		{
			/* mute off処理 */
			Aud_muteOFF_main();
		}
	}
}

/******************************************************************************
	関数名		：Aud_init_mute_func
	引数		：なし
	戻り値		：なし
	機能概略	：muteブロックの初期化関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_init_mute_func(void)
{
	/* Sysmute On */
	aud_mute_state.sys_mute = ON;
#if 1	/* C12 */
	MIO_OUT_pSYS_MUTE( LOW );
#endif
	/* mute制御制御情報初期化	 */
	aud_mute_info.seq_id     = CAUD_MUTE_SEQ_OFF;
	aud_mute_state.onoff_seq = OFF;
	aud_mute_state.curt_mute = ON;
}

#undef _AUD_MUTE_FUNC_C_

