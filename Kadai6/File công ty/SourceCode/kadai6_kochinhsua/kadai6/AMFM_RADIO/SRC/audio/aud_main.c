/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		: 	aud_main.c
	モジュール名	: 	Audioモジュール
	機能概略		: 	メイン処理
-------------------------------------------------------------------------------
修正履歴
	番号		日付		担当者		関数名			修正内容
	【000】		12/07/13	leejh						新規作成
	【001】		12/09/25	leejh		Aud_msg_maincnt_off_req(変更)
										Aud_msg_power_off(変更)
 										Aud_get_power_mode(変更)
										Aud_init_evol_func(変更)
										Aud_secret_loud_off(追加)
										Aud_test_edit_data(追加)
										Aud_test_edit_vol(追加)
										Aud_test_edit_tone(追加)
										Aud_test_edit_sp(追加)
										Aud_test_edit_loud(追加)
										Aud_test_edit_mute(追加)
										Aud_test_edit_beep(追加)
	【002】		12/11/07	leejh		Aud_msg_maincnt_off(変更)
										Aud_msg_power_off(変更)
********************************************************************************/
#define		_AUD_MAIN_C_

#include	<string.h>
#include	"../model.h"

#include	"../comm/common.h"
#include	"../main/MA_inc.h"
#include	"../tuner/TU_inc.h"
#include	"../power/PW_inc.h"
#include	"../io/IO_inc.h"
#include	"../submode/SBM_inc.h"
#include	"../testmode/TEST_inc.h"

#include	"aud_ext.h"
#include	"aud_sys.h"
#include	"aud_def.h"
#include	"aud_main.h"
#include	"aud_mute_func.h"
#include	"aud_dev_func.h"
#include	"aud_beep_func.h"

#define		_TEST_NON_MUTE_		0

/*---------------------------------------------------------------------------*
 *	内部定数定義														 	 *
 *---------------------------------------------------------------------------*/
#define	AUD_MODE_MAX	( CMA_MODE_MAX + 1 )	/* モード MAX	*/

#define CAUD_EVOL_SEQ_START	0x01	/* Mute処理シーケンス開始	*/
#define CAUD_EVOL_SEQ_OFF	0xFF	/* Mute処理シーケンス終了	*/

/* MAIN MODE -> AUDIOサウンドソース変換テーブル */
const TAUD_MODE_CONV_DEF aud_mid_tbl[] =
{
	/* MainModuleID	, AudioModuleID								*/
	{ CMA_TUNER		, CAUD_MODE_TUNER_AM	},	/* TUNER MODE	*/
	{ CMA_AUX		, CAUD_MODE_AUX1		},	/* AUX1 MODE	*/
	{ CMA_ISR		, CAUD_MODE_TUNER_AM	}	/* ISR MODE		*/
};

/*---------------------------------------------------------------------------*
 *	構造体定義															 	 *
 *---------------------------------------------------------------------------*/
/* サウンドソース毎のAUDIO情報管理リスト */
typedef struct {
	BYTE		volume[AUD_MODE_MAX];	/* VOLステップ管理リスト			*/
} TAUD_AUDIO_STEP_LIST;

/* イベント情報管理 */
typedef struct {
	BYTE		mcnt_on		:1;	/* 0: main cnt onイベント	0: OFF 1: ON	*/
	BYTE		adjust		:1;	/* 1: audio調整				0: OFF 1: ON	*/
	BYTE		band_chg	:1;	/* 2: TUBER BAND切換え		0: OFF 1: ON	*/
	BYTE		mode_chg	:1;	/* 3: モードチェンジEVENT	0: OFF 1: ON	*/
	BYTE		reserved	:4;	/* reserved									*/
} TAUD_EVENT;

/* シーケンス情報管理 */
typedef struct {
	TAUD_EVENT	event;				/* EVOLイベントフラグ		 */
	BYTE		seq_id;				/* 処理シーケンスカウンタ	 */
	BYTE		maincnt_seq_id;		/* MainCnt_On初期値送信シーケンスカウンタ */
} TAUD_SEQ_MODE;

/*---------------------------------------------------------------------------*
 *	変数定義															 	 *
 *---------------------------------------------------------------------------*/
static TAUD_AUDIO_INFO		Aud_audio_info;						/* AUDIO情報					 */
static TAUD_AUDIO_STEP_LIST	Aud_saved_list;						/* サウンドソース毎のAUDIO情報	 */
static BYTE					Aud_01ms_timer[CAUD_C01MS_MAX];		/* 1msタイマカウンタ			 */
static BYTE					Aud_05ms_timer[CAUD_C05MS_MAX];		/* 5msタイマカウンタ			 */
static BYTE					Aud_10ms_timer[CAUD_C10MS_MAX];		/* 10msタイマカウンタ			 */
static BYTE					Aud_25ms_timer[CAUD_C25MS_MAX];		/* 25msタイマカウンタ			 */
static BYTE					Aud_1sec_timer[CAUD_C1SEC_MAX];		/* 1secタイマカウンタ			 */
static TAUD_TIMER_INFO		Aud_01ms_info;						/* 1msタイマ情報				 */
static TAUD_TIMER_INFO		Aud_05ms_info;						/* 5msタイマ情報				 */
static TAUD_TIMER_INFO		Aud_10ms_info;						/* 10msタイマ情報				 */
static TAUD_TIMER_INFO		Aud_25ms_info;						/* 25msタイマ情報				 */
static TAUD_TIMER_INFO		Aud_1sec_info;						/* 1secタイマ情報				 */
static TAUD_SEQ_MODE		aud_mode_evol;						/* EVOL処理シーケンス監視情報	 */

/*---------------------------------------------------------------------------*
 *	プロトタイプ定義													 	 *
 *---------------------------------------------------------------------------*/
static BYTE Aud_mode_MainToAudio( BYTE );
static void Aud_update_src_info( void );
static void Aud_save_audio_info( void );
static void Aud_read_audio_info( void );
static void Aud_event_maincnt_on( void );
static void Aud_event_mode_change( void );
static void Aud_event_band_change( void );
static void Aud_event_ajust_audio( void );

static void Aud_chk_BAND_change( void );
static void Aud_chk_audio_event( void );

static void Aud_audio_event( void );
static void Aud_chk_audio_mute( void );

static void Aud_init_volume( void );
static void Aud_init_bass( void );
static void Aud_init_treble( void );
static void Aud_init_balance( void );
static void Aud_init_fader( void );

static void Aud_init_audio_list( void );
static void Aud_init_evol_func( void );

#if _TEST_MODE_
static void Aud_test_edit_vol( BYTE, BYTE * );
static void Aud_test_edit_tone( BYTE );
static void Aud_test_edit_sp( BYTE );
static void Aud_test_edit_loud( BYTE, BYTE * );
static void Aud_test_edit_mute( BYTE, BYTE * );
static void Aud_test_edit_beep( BYTE * );
#endif

/******************************************************************************
	関数名		：Aud_mode_MainToAudio
	引数		：BYTE main_id 
	戻り値		：BYTE mode_id
	機能概略	：Mainモジュールが管理するモードIDをAUDIO管理のIDへ変換する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static BYTE Aud_mode_MainToAudio( BYTE main_id )
{
	BYTE	i;
	BYTE	mode_id;
	BYTE	max_cnt;

	mode_id = CAUD_MODE_TUNER_AM;	/* Fail Safe */
	max_cnt = sizeof( aud_mid_tbl ) / sizeof( TAUD_MODE_CONV_DEF );

	for ( i = 0; i < max_cnt; i++ )
	{
		/* 一致するIDが見つかった */
		if ( main_id == aud_mid_tbl[i].mid )
		{
			mode_id = aud_mid_tbl[i].aid;

			/* TUNERモード中の場合は受信バンドを調べる */
			if ( aud_mid_tbl[i].mid == CMA_TUNER )
			{
				/* FM受信中の場合はFMに変換 */
				if ( Tu_band_chk() != CTU_AM )
				{
					mode_id = CAUD_MODE_TUNER_FM;
				}
			}
			break;
		}
	}
	
	return( mode_id );
}

/******************************************************************************
	関数名		：Aud_update_sound_src_info
	引数		：なし
	戻り値		：なし
	機能概略	：サウンドソースの情報を更新する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_update_src_info( void )
{
	BYTE	curt_mode;

	/* カレントモードを取得 */
	curt_mode = Ma_current_mode();

	/* モード情報を更新する */
	Aud_audio_info.prev_mode = Aud_audio_info.curt_mode;
	Aud_audio_info.curt_mode = curt_mode;

	/* サウンドソース情報を更新する */
	Aud_audio_info.prev_src  = Aud_audio_info.curt_src;
	Aud_audio_info.curt_src  = Aud_mode_MainToAudio( curt_mode );
}

/******************************************************************************
	関数名		：Aud_save_audio_info
	引数		：無し
	戻り値		：無し
	機能概略	：サウンドソース毎のAUDIO情報を退避する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_save_audio_info( void )
{
	BYTE	sund_src;

	/* 切換え前のサウンドソース */
	sund_src = Aud_audio_info.prev_src;

	/* AUDIO情報を退避する */
#if 1	/* Volume Step総合 */
	Aud_saved_list.volume[CAUD_MODE_TUNER_AM] = Aud_audio_info.vol_step;
#else	/* Volume Step独立 */
	Aud_saved_list.volume[sund_src] = Aud_audio_info.vol_step;
#endif
}

/******************************************************************************
	関数名		：Aud_read_audio_info
	引数		：なし
	戻り値		：なし
	機能概略	：audio情報読出し処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_read_audio_info( void )
{
	BYTE	sund_src;

	/* 切換え後(現在)のサウンドソース */
	sund_src  = Aud_audio_info.curt_src;

#if 1	/* Volume Step総合 */
	Aud_audio_info.vol_step = Aud_saved_list.volume[CAUD_MODE_TUNER_AM];
#else	/* Volume Step独立 */
	Aud_audio_info.vol_step = Aud_saved_list.volume[sund_src];
#endif
}

/******************************************************************************
	関数名		：Aud_msg_mode_change
	引数		：なし
	戻り値		：なし
	機能概略	：モード切換え処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_msg_mode_change( void )
{
	aud_mode_evol.event.mode_chg = ON;
}

/******************************************************************************
	関数名		：Aud_event_mcnt_on
	引数		：無し
	戻り値		：無し
	機能概略	：MAIN CONT ON処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_msg_maincnt_on( void )
{
	/* 100msタイマを設定する */
	Aud_set_timer( CAUD_CNT_25MS, CAUD_C25MS_MCNT_ON, ((100/25) + 1));

	/* set event main cont on */
	aud_mode_evol.event.mcnt_on		= ON;
	aud_mode_evol.maincnt_seq_id	= CAUD_EVOL_SEQ_START;
	
	/* 送信禁止 */
	Aud_dis_audio  = ON;
}

/******************************************************************************
	関数名		：Aud_msg_maincnt_off_req
	引数		：無し
	戻り値		：無し
	機能概略	：MAIN CONT OFF処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_msg_maincnt_off_req( void )
{	
	Aud_set_stop_beep();				/* BEEP停止		*/
	
	Aud_power_mode = CAUD_PWR_OFFSEQ;	/* PWR OFF REQ	*/
	Aud_dis_audio = ON;					/* 送信禁止		*/
	
#if _TEST_MODE_
	/* LOUD診断(テスト、隠しキー)変数クリア */
	memset(&Aud_diag_loud, 0, sizeof(Aud_diag_loud));
#endif
}

/******************************************************************************
	関数名		：Aud_msg_maincnt_off
	引数		：無し
	戻り値		：無し
	機能概略	：MAIN CONT OFF処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
	【001】		12/11/07	leejh		LOUD診断構造体クリア処理追加
******************************************************************************/
void Aud_msg_maincnt_off( void )
{
	Aud_power_mode = CAUD_PWR_OFF;		/* PWR OFF	*/
	Aud_dis_audio = ON;					/* 送信禁止	*/
}

/******************************************************************************
	関数名		：Aud_event_pwr_on
	引数		：無し
	戻り値		：無し
	機能概略	：POWER ON処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_msg_power_on( void )
{
	/* power off前の入力ソースと相違があるかを確認する	 */
	if ( Ma_current_mode() != Aud_audio_info.curt_mode )
	{
		/* mode changeメッセージが来たと場合と同じようにする */
		Aud_msg_mode_change();
	}
}

/******************************************************************************
	関数名		：Aud_event_pwr_off
	引数		：無し
	戻り値		：無し
	機能概略	：MAIN CONT ON処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 	【001】		12/09/25	leejh		LOUD診断(隠しキー)クリア処理追加
  	【002】		12/11/07	leejh		LOUD OFFが解除されない不具合対応
******************************************************************************/
void Aud_msg_power_off( void )
{
	Aud_set_stop_beep();				/* BEEP停止	*/
	
#if _TEST_MODE_
	/* LOUD診断(隠しキー)あり */
	if( Aud_diag_loud.secret_loud != CAUD_LOUD_STA_UNKNOWN )
	{
		/* LOUD診断(テスト)あり */
		if(( Aud_diag_loud.test_loud == CAUD_LOUD_STA_OFF) ||
			( Aud_diag_loud.test_loud == CAUD_LOUD_STA_ON ))
		{
			/* LOUD診断(テスト)優先にする */
			Aud_diag_loud.prio_diag = CAUD_PRIO_TEST;
		}
		else
		{
			/* UNKNOWN設定 */
			Aud_diag_loud.prio_diag = CAUD_PRIO_UNKNOWN;
		}
		
		/* LOUD 不明(隠しキー)クリア */
		Aud_diag_loud.secret_loud = CAUD_LOUD_STA_UNKNOWN;
	}
#endif
}

/******************************************************************************
	関数名		：Aud_event_maincnt_on
	引数		：なし
	戻り値		：なし
	機能概略	：EVOL MAINCNT ONイベント処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_event_maincnt_on( void )
{
	switch ( aud_mode_evol.maincnt_seq_id )
	{
		case 1:
			if ( Aud_get_timer( CAUD_CNT_25MS, CAUD_C25MS_MCNT_ON ) == 0 )
			{
				/* E-VOL初期値を送信 */
				Aud_send_initial();

				aud_mode_evol.maincnt_seq_id++;
				/* アドバンススイッチ20ms待ち */
				Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_ASP_WAIT, ((20/5) + 1));
			}
			break;
			
		case 2:
			if ( Aud_get_timer( CAUD_CNT_05MS, CAUD_C05MS_ASP_WAIT ) == 0 )
			{
				/* power on安定解除(power on確定) */
				Aud_power_mode = CAUD_PWR_ON;
				Aud_dis_audio = OFF;
				
				/* data mute on(念のため) */
				aud_mute_state.data_mute = ON;
				
				/* audio全データ送信 */
				Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
		
				aud_mode_evol.event.mcnt_on = OFF;
				aud_mode_evol.maincnt_seq_id = CAUD_EVOL_SEQ_OFF;
			}
			break;
			
		default:
			break;
	}
}

/******************************************************************************
	関数名		：Aud_event_mode_change
	引数		：なし
	戻り値		：なし
	機能概略	：モード切換え処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【002】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_event_mode_change( void )
{
	/* Mute On完了してから、入力切替を行う */
	if( Aud_is_on_mute() == TRUE )
	{
		/* サウンドソース情報を更新する */
		Aud_update_src_info();
	
		/* 切換え前のAUDIO情報を退避する */
		Aud_save_audio_info();
	
		/* 切換え後のAUDIO情報を読出す */
		Aud_read_audio_info();
		
		Aud_ctrl_ASP( CAUD_ASP_SRC_CHG );
		
		/* BAND切換えイベントクリア */
		aud_mode_evol.event.band_chg = OFF;
		
		/* ModeChange EVENTクリア */
		aud_mode_evol.event.mode_chg = OFF;
	}
}

/******************************************************************************
	関数名		：Aud_event_band_change
	引数		：なし
	戻り値		：なし
	機能概略	：
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_event_band_change( void )
{
	if (( Aud_get_timer( CAUD_CNT_10MS, CAUD_C10MS_BAND_CHG_WAIT ) == 0 )
	&& ( Aud_is_on_mute() == TRUE ))
	{
		/* サウンドソース情報を更新する */
		Aud_update_src_info();

		/* 切換え前のAUDIO情報を退避する */
		Aud_save_audio_info();

		/* 切換え後のAUDIO情報を読出す */
		Aud_read_audio_info();
		
		/* ソース切替のE-VOLデータ送信 */
		Aud_ctrl_ASP( CAUD_ASP_SRC_CHG );

		/* BAND切換えイベントクリア */
		aud_mode_evol.event.band_chg = OFF;
	}
}

/******************************************************************************
	関数名		：Aud_event_ajust_audio
	引数		：無し
	戻り値		：無し
	機能概略	：E-VOLへAUDIOデータの送信、SRS WOWデバイスを制御する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_event_ajust_audio( void )
{
	/* 電子ボリュームデバイス制御 */
	Aud_ctrl_ASP( Aud_audio_info.asp_cmd );

	/* イベントクリア */
	Aud_audio_info.asp_cmd		= 0xFF;
	aud_mode_evol.event.adjust	= OFF;
}

/******************************************************************************
	関数名		：Aud_edit_volume
	引数		：なし
	戻り値		：なし
	機能概略	：volume調整処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_edit_volume( BYTE direction )
{
	BYTE	step;
	BYTE	pre_step;
	
	/* 現在のstep値を読みだす */
	step = Aud_audio_info.vol_step;

	switch ( direction )
	{
		case CAUD_STP_UP:
			step = Aud_incr( step, CAUD_VOL_MAX );
			break;

		case CAUD_STP_DOWN:
			step = Aud_decr( step, CAUD_VOL_MIN );
			break;

		case CAUD_STP_CENTER:
			step = CAUD_VOL_CNT;
			break;

		case CAUD_STP_MAX:
			step = CAUD_VOL_MAX;
			break;

		case CAUD_STP_MIN:
			step = CAUD_VOL_MIN;
			break;

		case CAUD_STP_DEF:
			step = CAUD_VOL_INI;
			break;

		default:
			break;
	}

	/* volume step変化した */
	if ( Aud_audio_info.vol_step != step )
	{
		/* 現Stepを以前Stepに格納 */
		pre_step = Aud_audio_info.vol_step;

		/* volume stepを更新する */
		Aud_audio_info.vol_step = step;

		/* volume調整の要求をEVOLへ出す */
		Aud_audio_info.asp_cmd = CAUD_ASP_VOL;
		aud_mode_evol.event.adjust = ON;
		
		/* Vol Step 1→0、0→1は送信要求しない */
		if ((( pre_step > CAUD_VOL_MIN ) && ( step == CAUD_VOL_MIN )) ||
			(( pre_step == CAUD_VOL_MIN ) && ( step > CAUD_VOL_MIN )))
		{
			Aud_audio_info.asp_cmd = 0xFF;
			aud_mode_evol.event.adjust = OFF;
		}
	}	
}

/******************************************************************************
	関数名		：Aud_edit_bass
	引数		：なし
	戻り値		：なし
	機能概略	：BASS調整処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_edit_bass( BYTE direction )
{
	BYTE	step;

	step = Aud_audio_info.bas_step;

	switch ( direction )
	{
		case CAUD_STP_UP:
			step = Aud_incr( Aud_audio_info.bas_step, CAUD_BAS_MAX );
			break;

		case CAUD_STP_DOWN:
			step = Aud_decr( Aud_audio_info.bas_step, CAUD_BAS_MIN );
			break;

		case CAUD_STP_CENTER:
			step = CAUD_BAS_CNT;
			break;

		case CAUD_STP_MAX:
			step = CAUD_BAS_MAX;
			break;

		case CAUD_STP_MIN:
			step = CAUD_BAS_MIN;
			break;

		case CAUD_STP_DEF:
			step = CAUD_BAS_INI;
			break;

		default:
			break;
	}

	if (Aud_audio_info.bas_step != step)
	{
		/* bass stepを更新する */
		Aud_audio_info.bas_step = step;
		
		/* bass調整の要求をEVOLへ出す */
		Aud_audio_info.asp_cmd = CAUD_ASP_BAS;
		aud_mode_evol.event.adjust	= ON;

	}
}

/******************************************************************************
	関数名		：Aud_edit_treble
	引数		：なし
	戻り値		：なし
	機能概略	：TREBLE調整処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_edit_treble( BYTE direction )
{
	BYTE step;

	step = Aud_audio_info.tre_step;

	switch ( direction )
	{
		case CAUD_STP_UP:
			step = Aud_incr( Aud_audio_info.tre_step, CAUD_TRE_MAX );
			break;

		case CAUD_STP_DOWN:
			step = Aud_decr( Aud_audio_info.tre_step, CAUD_TRE_MIN );
			break;

		case CAUD_STP_CENTER:
			step = CAUD_TRE_CNT;
			break;

		case CAUD_STP_MAX:
			step = CAUD_TRE_MAX;
			break;

		case CAUD_STP_MIN:
			step = CAUD_TRE_MIN;
			break;

		case CAUD_STP_DEF:
			step = CAUD_TRE_INI;
			break;

		default:
			break;
	}

	if (Aud_audio_info.tre_step != step)
	{
		/* treble stepを更新する */
		Aud_audio_info.tre_step = step;
		
		/* treble調整の要求をEVOLへ出す */
		Aud_audio_info.asp_cmd = CAUD_ASP_TRE;
		aud_mode_evol.event.adjust = ON;
	}
}

/******************************************************************************
	関数名		：Aud_edit_balance
	引数		：なし
	戻り値		：なし
	機能概略	：BALANCE調整処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_edit_balance( BYTE direction )
{
	BYTE step;

	step = Aud_audio_info.bal_step;

	switch ( direction )
	{
		case CAUD_STP_UP:
			/* STEPを加算する */
			step = Aud_incr( Aud_audio_info.bal_step, CAUD_BAL_MAX );
			break;

		case CAUD_STP_DOWN:
			/* STEPを減算する */
			step = Aud_decr( Aud_audio_info.bal_step, CAUD_BAL_MIN );
			break;

		case CAUD_STP_CENTER:
			step = CAUD_BAL_CNT;
			break;

		case CAUD_STP_MAX:
			step = CAUD_BAL_MAX;
			break;

		case CAUD_STP_MIN:
			step = CAUD_BAL_MIN;
			break;

		case CAUD_STP_DEF:
			step = CAUD_BAL_INI;
			break;

		default:
			break;
	}

	if (Aud_audio_info.bal_step != step)
	{
		/* balance stepを更新する */
		Aud_audio_info.bal_step = step;
		
		/* balance調整の要求をEVOLへ出す */
		Aud_audio_info.asp_cmd = CAUD_ASP_BAL;
		aud_mode_evol.event.adjust = ON;
	}
}

/******************************************************************************
	関数名		：Aud_edit_fader
	引数		：なし
	戻り値		：なし
	機能概略	：FADER調整処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_edit_fader( BYTE direction )
{
	BYTE	step;

	step = Aud_audio_info.fad_step;

	switch ( direction )
	{
		case CAUD_STP_UP:
			step = Aud_incr( Aud_audio_info.fad_step, CAUD_FAD_MAX );
			break;

		case CAUD_STP_DOWN:
			step = Aud_decr( Aud_audio_info.fad_step, CAUD_FAD_MIN );
			break;

		case CAUD_STP_CENTER:
			step = CAUD_FAD_CNT;
			break;

		case CAUD_STP_MAX:
			step = CAUD_FAD_MAX;
			break;

		case CAUD_STP_MIN:
			step = CAUD_FAD_MIN;
			break;

		case CAUD_STP_DEF:
			step = CAUD_FAD_INI;
			break;

		default:
			break;
	}

	if (Aud_audio_info.fad_step != step)
	{
		/* fader stepを更新する */
		Aud_audio_info.fad_step = step;
		
		/* fader調整の要求をEVOLへ出す */
		Aud_audio_info.asp_cmd = CAUD_ASP_FAD;
		aud_mode_evol.event.adjust = ON;
	}
}

/******************************************************************************
	関数名		：Aud_get_audio_info_list
	引数		：無し
	戻り値		：無し
	機能概略	：AUDIOI情報リストを取得する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
TAUD_AUDIO_INFO *Aud_get_audio_info_list( void )
{
	return( &Aud_audio_info );
}

/******************************************************************************
	関数名		：Aud_get_taudio_info
	引数		：BYTE	ainf_typ	CAUD_ADJ_VOL：Volume Step
									CAUD_ADJ_BAS：Bass Step
 									CAUD_ADJ_TRE：Treble Step
									CAUD_ADJ_BAL：Balance Ste)
									CAUD_ADJ_FAD：Fader Step
	戻り値		：BYTE	aud_info	: 各オーディオ情報
	機能概略	：任意のオーディオ情報を通知する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 ******************************************************************************/
BYTE Aud_get_audio_info( BYTE ainf_typ )
{
	BYTE aud_info;

	/* 変数初期化 */
	aud_info = 0xFF;

	switch ( ainf_typ )
	{
		/* volume step */
		case CAUD_ADJ_VOL:
			aud_info = Aud_audio_info.vol_step;
			break;

		/* bass step */
		case CAUD_ADJ_BAS:
			aud_info = Aud_audio_info.bas_step;
			break;

		/* treble step */
		case CAUD_ADJ_TRE:
			aud_info = Aud_audio_info.tre_step;
			break;

		/* balance step */
		case CAUD_ADJ_BAL:
			aud_info = Aud_audio_info.bal_step;
			break;

		/* fader step */
		case CAUD_ADJ_FAD:
			aud_info = Aud_audio_info.fad_step;
			break;

		default:
			break;
	}

	return ( aud_info );
}

/******************************************************************************
	関数名		：Aud_get_power_mode
	引数		：無し
	戻り値		：BYTE	CAUD_PWR_OFF	: Power Off
						CAUD_PWR_ONSEQ	: Power On Seq
						CAUD_PWR_ON		: Power On
						CAUD_PWR_OFFSEQ	: Power Off Seq
	機能概略	：Audio Powerモードを取得する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
 ******************************************************************************/
BYTE Aud_get_power_mode( void )
{
	return ( Aud_power_mode );
}

/******************************************************************************
	関数名		：Aud_chk_BAND_change
	引数		：無し
	戻り値		：無し
	機能概略	：E-VOLイベント監視処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_chk_BAND_change( void )
{
	BYTE	curt_band;
	BYTE	curt_mode;

	curt_mode = Ma_current_mode();
	
	/* TUNER mode */
	if (curt_mode == CMA_TUNER)
	{
		curt_band = Tu_band_chk();

		/* BAND change */
		if (curt_band != Aud_audio_info.curt_band )
		{
			/* BANDを更新 */
			Aud_audio_info.curt_band = curt_band;

			/* バンド切替のインターバルタイム(インターバル時間が必要時使用) */
			Aud_set_timer( CAUD_CNT_10MS, CAUD_C10MS_BAND_CHG_WAIT, 0 );
			
			aud_mode_evol.event.band_chg = ON;
		}
	}
}

/******************************************************************************
	関数名		：Aud_chk_audio_event
	引数		：無し
	戻り値		：無し
	機能概略	：E-VOLイベント監視処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_chk_audio_event( void )
{
	Aud_chk_BAND_change();
}

/******************************************************************************
	関数名		：Aud_audio_event
	引数		：無し
	戻り値		：無し
	機能概略	：E-VOLイベント処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_audio_event( void )
{
	/* MAINCNT ONイベント検出 */
	if ( aud_mode_evol.event.mcnt_on == ON )
	{
		Aud_event_maincnt_on();
	}
	/* モード切り替えイベント検出 */
	else if( aud_mode_evol.event.mode_chg == ON )
	{
		Aud_event_mode_change();
	}
	/* TUNER BAND切換えイベント検出 */
	else if ( aud_mode_evol.event.band_chg == ON )
	{
		Aud_event_band_change();
	}
	/* AUDIO調整イベント検出 */
	else if ( aud_mode_evol.event.adjust == ON )
	{
		Aud_event_ajust_audio();
	}
	else	/* QAC対策 */
	{
		;
	}
}

/******************************************************************************
	関数名		：Aud_chk_audio_mute
	引数		：無し
	戻り値		：無し
	機能概略	：E-VOL制御メイン処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_chk_audio_mute( void )
{
	BYTE	vol_step;

	/* volume step取得 */
	vol_step = Aud_sub( Aud_audio_info.vol_step, Aud_audio_info.pre_kvol );

	if( vol_step == 0 )
	{
		/* mute on要求 */
		Aud_set_prg_mute( CAUD_MUTE_AUD_NR_MUTE, ON );

		aud_mute_evol.zero_mute = ON;

	}
	else
	{
		aud_mute_evol.zero_mute = OFF;

		/* mute off要求 */
		Aud_set_prg_mute( CAUD_MUTE_AUD_NR_MUTE, OFF );
	}
}

/******************************************************************************
	関数名		：Aud_evol_main
	引数		：無し
	戻り値		：無し
	機能概略	：E-VOL制御メイン処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_evol_main( void )
{
	/* EVOLイベント監視処理 */
	Aud_chk_audio_event();
	
	/* EVOLイベント処理 */
	Aud_audio_event();
	
	#if (_TEST_NON_MUTE_ == 0)	/* テスト版VOL値0時、MUTE端子制御不要 */
	/* mute制御処理 */
	Aud_chk_audio_mute();
	#endif
}

/******************************************************************************
	関数名		：Aud_set_timer
	引数		：BYTE tmr_id	: タイマ種別（1/5/10/25/1000）
				：BYTE cnt_id	: タイマカウンタid
				：BYTE cnt		: カウンタ（※255以下の値を指定する事！）
	戻り値		：無し
	機能概略	：タイマカウンタを設定する処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		06/04/03	ISHIHARA	新規作成
******************************************************************************/
void Aud_set_timer( BYTE tmr_id, BYTE cnt_id, BYTE cnt )
{
	switch( tmr_id ) {
		/* 1msタイマ */
		case CAUD_CNT_01MS:
			if (Aud_01ms_info.max > cnt_id )
			{
				Aud_01ms_info.cntr[cnt_id] =cnt;
			}
		break;

		/* 5msタイマ */
		case CAUD_CNT_05MS:
			if (Aud_05ms_info.max > cnt_id )
			{
				Aud_05ms_info.cntr[cnt_id] =cnt;
			}
		break;

		/* 10msタイマ */
		case CAUD_CNT_10MS:
			if (Aud_10ms_info.max > cnt_id )
			{
				Aud_10ms_info.cntr[cnt_id] =cnt;
			}
		break;

		/* 25msタイマ */
		case CAUD_CNT_25MS:
			if (Aud_25ms_info.max > cnt_id )
			{
				Aud_25ms_info.cntr[cnt_id] =cnt;
			}
		break;

		/* 1000msタイマ */
		case CAUD_CNT_1SEC:
			if (Aud_1sec_info.max > cnt_id )
			{
				Aud_1sec_info.cntr[cnt_id] =cnt;
			}
		break;

		/* 無効指定 */
		default:
		break;
	}
}

/******************************************************************************
	関数名		：Aud_get_timer
	引数		：BYTE tmr_typ	: タイマ種別（1/5/10/25/1000）
				：BYTE tmr_id	: タイマカウンタid
	戻り値		：BYTE tmr		: カウント中のタイマカウンタ
	機能概略	：タイマカウンタ取得処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		06/04/03	ISHIHARA	新規作成
******************************************************************************/
BYTE Aud_get_timer( BYTE tmr_id, BYTE cnt_id )
{
	BYTE	tmr;

	/* 変数初期化 */
	tmr =0xff;

	switch( tmr_id ) {
		/* 1msタイマ */
		case CAUD_CNT_01MS:
			if (Aud_01ms_info.max > cnt_id )
			{
				tmr = Aud_01ms_info.cntr[cnt_id];
			}
		break;

		/* 5msタイマ */
		case CAUD_CNT_05MS:
			if (Aud_05ms_info.max > cnt_id )
			{
				tmr = Aud_05ms_info.cntr[cnt_id];
			}
		break;

		/* 10msタイマ */
		case CAUD_CNT_10MS:
			if (Aud_10ms_info.max > cnt_id )
			{
				tmr = Aud_10ms_info.cntr[cnt_id];
			}
		break;

		/* 25msタイマ */
		case CAUD_CNT_25MS:
			if (Aud_25ms_info.max > cnt_id )
			{
				tmr = Aud_25ms_info.cntr[cnt_id];
			}
		break;

		/* 1000msタイマ */
		case CAUD_CNT_1SEC:
			if (Aud_1sec_info.max > cnt_id )
			{
				tmr = Aud_1sec_info.cntr[cnt_id];
			}
		break;

		/* 無効指定 */
		default:
		break;
	}
	return( tmr );
}

/******************************************************************************
	関数名		：Aud_dec_timer
	引数		：TAUD_TIMER_INFO	*tmr_info	:カウント処理するタイマ情報
	戻り値		：無し
	機能概略	：タイマ減算処理（各タイマ共通）
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		06/04/03	ISHIHARA	新規作成
******************************************************************************/
void Aud_dec_timer( TAUD_TIMER_INFO *tmr_info )
{
	BYTE	i;

	/* エントリされているカウンタ分、カウンタをデクリメントする */
	for(i= 0; i< tmr_info->max; i++) {
		/* タイマが"0"でなければ減算する */
		if ( tmr_info->cntr[i] != 0 )
		{
			tmr_info->cntr[i]--;
		}
	}
}

/******************************************************************************
	関数名		：Aud_msg_timer
	引数		：BYTE timer_msg	: タイマメッセージ
	戻り値		：なし
	機能概略	：EVOLタイマ処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_msg_timer( BYTE timer_msg )
{
	switch ( timer_msg )
	{
		case CAUD_EVT_TIMER_001M:
			/* 1msタイマカウント処理 */
			Aud_dec_timer(&Aud_01ms_info);
			break;

		case CAUD_EVT_TIMER_005M:
			/* 5msタイマカウント処理 */
			Aud_dec_timer(&Aud_05ms_info);
			break;

		case CAUD_EVT_TIMER_010M:
			/* 10msタイマカウント処理 */
			Aud_dec_timer(&Aud_10ms_info);
			break;

		case CAUD_EVT_TIMER_025M:
			/* 25msタイマカウント処理 */
			Aud_dec_timer(&Aud_25ms_info);
			break;

		case CAUD_EVT_TIMER_001S:
			/* 1000msタイマカウント処理 */
			Aud_dec_timer(&Aud_1sec_info);
			break;

		default:
			break;
	}
}

/******************************************************************************
	関数名		：Aud_init_volume
	引数		：なし
	戻り値		：なし
	機能概略	：volume step値を初期化する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_init_volume( void )
{
	Aud_audio_info.vol_step = CAUD_VOL_INI;
	
	memset(&Aud_saved_list.volume[0], CAUD_VOL_INI, AUD_MODE_MAX );
}

/******************************************************************************
	関数名		：Aud_init_bass
	引数		：なし
	戻り値		：なし
	機能概略	：bass step値を初期化する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_init_bass( void )
{
	Aud_audio_info.bas_step = CAUD_BAS_INI;
}

/******************************************************************************
	関数名		：Aud_init_treble
	引数		：なし
	戻り値		：なし
	機能概略	：treble step値を初期化する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_init_treble( void )
{
	Aud_audio_info.tre_step = CAUD_TRE_INI;
}

/******************************************************************************
	関数名		：Aud_init_balance
	引数		：なし
	戻り値		：なし
	機能概略	：balance step値を初期化する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_init_balance( void )
{
	Aud_audio_info.bal_step = CAUD_BAL_INI;
}

/******************************************************************************
	関数名		：Aud_init_fader
	引数		：なし
	戻り値		：なし
	機能概略	：fader step値を初期化する関数
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_init_fader( void )
{
	Aud_audio_info.fad_step = CAUD_FAD_INI;
}

/******************************************************************************
	関数名		：Aud_init_audio_list
	引数		：無し
	戻り値		：無し
	機能概略	：AUDIO情報リストの初期設定処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_init_audio_list( void )
{
	/* AUDIOクリア */
	Aud_init_volume();
	Aud_init_bass();
	Aud_init_treble();
	Aud_init_balance();
	Aud_init_fader();
	
	/* モードクリア*/
	Aud_audio_info.curt_src		= CAUD_MODE_TUNER_AM;
	Aud_audio_info.prev_src		= CAUD_MODE_TUNER_AM;
	Aud_audio_info.curt_band	= Tu_band_chk();
	
	/* キーMUTE系クリア */
	Aud_audio_info.pre_kvol		= CAUD_VOL_MIN;
	Aud_audio_info.pre_evol		= CAUD_VOL_MIN;
	
	/* ASPクリア */
	Aud_audio_info.asp_cmd		= 0xFF;
}

/******************************************************************************
	関数名		：Aud_init_evol_func
	引数		：無し
	戻り値		：無し
	機能概略	：E-VOL制御ブロック初期化処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
	【001】		12/09/25	leejh		LOUD診断(テスト、隠しキー)構造体初期化
 										内部 Power Off状態処理追加
******************************************************************************/
static void Aud_init_evol_func( void )
{
	/* 内部変数の初期化 */
	memset(&Aud_audio_info, 0, sizeof(Aud_audio_info));
	memset(Aud_01ms_timer, 0, sizeof(Aud_01ms_timer));
	memset(Aud_05ms_timer, 0, sizeof(Aud_05ms_timer));
	memset(Aud_10ms_timer, 0, sizeof(Aud_10ms_timer));
	memset(Aud_25ms_timer, 0, sizeof(Aud_25ms_timer));
	memset(Aud_1sec_timer, 0, sizeof(Aud_1sec_timer));
	memset(&Aud_saved_list, 0, sizeof(Aud_saved_list));
	memset(&aud_mode_evol, 0, sizeof(aud_mode_evol));
	
#if _TEST_MODE_
	/* LOUD診断(テスト、隠しキー)変数クリア */
	memset(&Aud_diag_loud, 0, sizeof(Aud_diag_loud));
#endif
	
	/* 1msタイマ情報の初期化 */
	Aud_01ms_info.max  = sizeof(Aud_01ms_timer);
	Aud_01ms_info.cntr = Aud_01ms_timer;

	/* 5msタイマ情報の初期化 */
	Aud_05ms_info.max  = sizeof(Aud_05ms_timer);
	Aud_05ms_info.cntr = Aud_05ms_timer;

	/* 10msタイマ情報の初期化 */
	Aud_10ms_info.max  = sizeof(Aud_10ms_timer);
	Aud_10ms_info.cntr = Aud_10ms_timer;

	/* 25msタイマ情報の初期化 */
	Aud_25ms_info.max  = sizeof(Aud_25ms_timer);
	Aud_25ms_info.cntr = Aud_25ms_timer;

	/* 1secタイマ情報の初期化 */
	Aud_1sec_info.max  = sizeof(Aud_1sec_timer);
	Aud_1sec_info.cntr = Aud_1sec_timer;

	/* AUDIO情報リストの初期設定処理	*/
	Aud_init_audio_list();

	/* MainCnt On前に送信していけない	*/
	Aud_dis_audio = ON;
	
	/* 内部 Power Off状態 */
	Aud_power_mode = CAUD_PWR_OFF;
}

/******************************************************************************
	関数名		：Aud_msg_initial
	引数		：無し
	戻り値		：無し
	機能概略	：AUDIO制御モジュール初期化処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_msg_initial( void )
{
	/* E-VOL制御ブロック初期化 */
	Aud_init_evol_func();

	/* MUTE制御ブロック初期化 */
	Aud_init_mute_func();

	/* AUDIOデバイスドライバ初期化処理 */
	Aud_init_dev_func();
	
	/* Beep 初期化 */
	Aud_init_beep_func();
}

/******************************************************************************
	関数名		：Aud_msg_main
	引数		：無し
	戻り値		：無し
	機能概略	：オーディオ制御メインループ
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_msg_main( void )
{
	/* MUTE制御ブロックメインループ */
	Aud_mute_main();

	/* E-VOL制御ブロックメインループ */
	Aud_evol_main();
	
	/* Beep制御メインループ */
	Aud_beep_main();
}

#if _TEST_MODE_
/******************************************************************************
	関数名		：Aud_secret_edit_loud
	引数		：無し
	戻り値		：無し
	機能概略	：Audio隠しキー処理(Loud Off)
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
******************************************************************************/
void Aud_secret_loud_off( void )
{
	/* 隠しキーLoud Off設定 */
	Aud_diag_loud.secret_loud = CAUD_LOUD_STA_OFF;
	
	/* 隠しキー優先 */
	Aud_diag_loud.prio_diag = CAUD_PRIO_SECRET;
	
	/* ALL DATA送信要求 */
	Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
}

/******************************************************************************
	関数名		：Aud_test_edit_data
	引数		：BYTE RcvCmd	=> 受信コマンド
				  BYTE *RcvBuf	=> 受信バッファ
				  BYTE Length	=> 受信長さ
	戻り値		：無し
	機能概略	：Audioテストモード(Data Edit)
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
******************************************************************************/
void Aud_test_edit_data( BYTE RcvCmd, BYTE *RcvBuf, BYTE Length )
{
	switch ( RcvCmd )
	{
		/* Volumeコマンド */
		case CTEST_MODE_AUD_DIR_VOL:
			Aud_test_edit_vol( RcvCmd, RcvBuf );
			break;
		
		/* Toneコマンド */
		case CTEST_MODE_AUD_BAS_MAX:
		case CTEST_MODE_AUD_BAS_MIN:
		case CTEST_MODE_AUD_TRE_MAX:
		case CTEST_MODE_AUD_TRE_MIN:
		case CTEST_MODE_AUD_BSTR_CEN:
			Aud_test_edit_tone( RcvCmd );
			break;
		
		/* Speakerコマンド */
		case CTEST_MODE_AUD_BAL_RMAX:
		case CTEST_MODE_AUD_BAL_LMAX:	
		case CTEST_MODE_AUD_BAL_CEN:	
			Aud_test_edit_sp( RcvCmd );
			break;
		
		/* Loudnessコマンド */
		case CTEST_MODE_AUD_LOUD_ONOFF:
			Aud_test_edit_loud( RcvCmd, RcvBuf );
			break;
		
		/* Muteコマンド */
		case CTEST_MODE_AUD_MUTE_ONOFF:
			Aud_test_edit_mute( RcvCmd, RcvBuf );
			break;
		
		/* Beepコマンド */
		case CTEST_MODE_OTHER_BEEP_ONOFF:
			Aud_test_edit_beep( RcvBuf );
			break;
			
		default:
			break;
	}
}

/******************************************************************************
	関数名		：Aud_test_edit_vol
	引数		：BYTE RcvCmd	=> 受信コマンド
				  BYTE *RcvBuf	=> 受信バッファ
	戻り値		：無し
	機能概略	：Audioテストモード(Volume Edit)
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
******************************************************************************/
static void Aud_test_edit_vol( BYTE RcvCmd, BYTE *RcvBuf )
{
	BYTE cur_vol_step;
	BYTE tar_vol_step;
	BYTE snd_flg;
	
	/* Fail Safe */
	snd_flg = ON;
	
	switch ( RcvCmd )
	{
		case CTEST_MODE_AUD_DIR_VOL:					/* Direct Volume	*/
			if ( RcvBuf[0] <= CAUD_VOL_MAX )			/* 範囲チェック		*/
			{
				/* 現在Step、ターゲットStep取得 */
				cur_vol_step = Aud_audio_info.vol_step;
				tar_vol_step = RcvBuf[0];
				
				/* Volume mute時は送信禁止 */
				if ((( cur_vol_step > CAUD_VOL_MIN ) && ( tar_vol_step == CAUD_VOL_MIN )) ||
					(( cur_vol_step == CAUD_VOL_MIN ) && ( tar_vol_step > CAUD_VOL_MIN )) )
				{
					snd_flg = OFF;
				}
				
				/* ターゲットのVolume Step格納 */
				Aud_audio_info.vol_step = tar_vol_step;
			}
			else
			{
				/* 範囲超過は送信禁止 */
				snd_flg = OFF;
			}
			break;
			
		default:	/* Exception */
			snd_flg = OFF;
			break;
	}
	
	/* 送信許可のみ */
	if ( snd_flg == ON )
	{
		Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
	}
}

/******************************************************************************
	関数名		：Aud_test_edit_tone
	引数		：BYTE RcvCmd	=> 受信コマンド
	戻り値		：無し
	機能概略	：Audioテストモード(Tone Edit)
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
******************************************************************************/
static void Aud_test_edit_tone( BYTE RcvCmd )
{
	BYTE tar_bas_step;
	BYTE tar_tre_step;
	BYTE cur_bas_step;
	BYTE cur_tre_step;
	BYTE snd_flg;
	BOOL cnt_det;
	
	/* Fail Safe */
	cnt_det = FALSE;
	snd_flg = ON;
	
	/* Step取得 */
	cur_bas_step = Aud_audio_info.bas_step;
	cur_tre_step = Aud_audio_info.tre_step;
	
	switch ( RcvCmd )
	{
		case CTEST_MODE_AUD_BAS_MAX:		/* Bass Max */
			tar_bas_step = CAUD_BAS_MAX;
			tar_tre_step = cur_tre_step;
			
			/* センター未満はセンターを送信してから */
			if ( cur_bas_step < CAUD_BAS_CNT )
			{
				cur_bas_step = CAUD_BAS_CNT;
				cnt_det = TRUE;
			}
			break;
			
		case CTEST_MODE_AUD_BAS_MIN:		/* Bass Min */
			tar_bas_step = CAUD_BAS_MIN;
			tar_tre_step = cur_tre_step;
			
			/* センター超過はセンターを送信してから */
			if ( cur_bas_step > CAUD_BAS_CNT )
			{
				cur_bas_step = CAUD_BAS_CNT;
				cnt_det = TRUE;
			}
			break;
			
		case CTEST_MODE_AUD_TRE_MAX:		/* Treble Max */
			tar_bas_step = cur_bas_step;
			tar_tre_step = CAUD_TRE_MAX;
			
			/* センター未満はセンターを送信してから */
			if ( cur_tre_step < CAUD_TRE_CNT )
			{
				cur_tre_step = CAUD_TRE_CNT;
				cnt_det = TRUE;
			}
			break;
			
		case CTEST_MODE_AUD_TRE_MIN:		/* Treble Min */
			tar_bas_step = cur_bas_step;
			tar_tre_step = CAUD_TRE_MIN;
			
			/* センター超過はセンターを送信してから */
			if ( cur_tre_step > CAUD_TRE_CNT )
			{
				cur_tre_step = CAUD_TRE_CNT;
				cnt_det = TRUE;
			}
			break;
			
		case CTEST_MODE_AUD_BSTR_CEN:		/* Bas/Tre Center */
			tar_bas_step = CAUD_BAS_CNT;
			tar_tre_step = CAUD_TRE_CNT;
			break;
			
		default:							/* Exception */
			snd_flg = OFF;
			break;
	}
	
	/* 送信許可のみ */
	if ( snd_flg == ON )
	{
		if( cnt_det == TRUE )
		{
			/* 0dB経由送信 */
			Aud_audio_info.bas_step = cur_bas_step;
			Aud_audio_info.tre_step = cur_tre_step;

			Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
		}
		
		/* ターゲット送信 */
		Aud_audio_info.bas_step = tar_bas_step;	
		Aud_audio_info.tre_step = tar_tre_step;

		Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
	}
}

/******************************************************************************
	関数名		：Aud_test_edit_sp
	引数		：BYTE RcvCmd	=> 受信コマンド
	戻り値		：無し
	機能概略	：Audioテストモード(Speaker Edit)
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
******************************************************************************/
static void Aud_test_edit_sp( BYTE RcvCmd )
{
	BYTE tar_bal_step;
	BYTE snd_flg;
	
	/* Fail Safe */
	snd_flg = ON;

	switch ( RcvCmd )
	{
		case CTEST_MODE_AUD_BAL_RMAX:			/* Balance R Max */
			tar_bal_step = CAUD_BAL_MAX;
			break;
			
		case CTEST_MODE_AUD_BAL_LMAX:			/* Balance L Max	 */
			tar_bal_step = CAUD_BAL_MIN;
			break;
			
		case CTEST_MODE_AUD_BAL_CEN:			/* Balance Center	*/
			tar_bal_step = CAUD_BAL_CNT;
			break;

		default:								/* Exception		*/
			snd_flg = OFF;
			break;
	}
	
	/* 送信許可のみ */
	if ( snd_flg == ON )
	{
		/* ターゲット送信 */
		Aud_audio_info.bal_step = tar_bal_step;

		Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
	}
}

/******************************************************************************
	関数名		：Aud_test_edit_loud
	引数		：BYTE RcvCmd	=> 受信コマンド
				  BYTE *RcvBuf	=> 受信バッファ
	戻り値		：無し
	機能概略	：Audioテストモード(Loudness Edit)
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
******************************************************************************/
static void Aud_test_edit_loud( BYTE RcvCmd, BYTE *RcvBuf )
{
	BYTE snd_flg;

	/* Fail Safe */
	snd_flg = ON;
	
	switch ( RcvCmd )
	{
		case CTEST_MODE_AUD_LOUD_ONOFF:
			if ( RcvBuf[0] == ON )			/* Loud On	*/
			{
				Aud_diag_loud.test_loud = CAUD_LOUD_STA_ON;
			}
			else if(RcvBuf[0] == OFF)		/* Loud Off	*/
			{
				Aud_diag_loud.test_loud = CAUD_LOUD_STA_OFF;
			}
			else
			{
				snd_flg = OFF;
			}
			break;
			
		default:	/* Exception */
			snd_flg = OFF;
			break;
	}
	
	/* 送信許可のみ */
	if ( snd_flg == ON )
	{
		/* テスト優先 */
		Aud_diag_loud.prio_diag = CAUD_PRIO_TEST;
		
		Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
	}
}

/******************************************************************************
	関数名		：Aud_test_edit_mute
	引数		：BYTE RcvCmd	=> 受信コマンド
				  BYTE *RcvBuf	=> 受信バッファ
	戻り値		：無し
	機能概略	：Audioテストモード(MUTE Edit)
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
******************************************************************************/
static void Aud_test_edit_mute( BYTE RcvCmd, BYTE *RcvBuf )
{
	switch ( RcvCmd )
	{
		case CTEST_MODE_AUD_MUTE_ONOFF:
			if ( RcvBuf[0] == ON )			/* SysMute On	*/
			{
				MIO_OUT_pSYS_MUTE( LOW );
			}
			else if ( RcvBuf[0] == OFF )	/* SysMute Off	*/
			{
				MIO_OUT_pSYS_MUTE( HIGH );
			}
			else	/* QAC対応 */
			{
				;
			}
			break;
		
		default:	/* Exception */
			break;
	}
}

/******************************************************************************
	関数名		：Aud_test_edit_beep
	引数		：BYTE *RcvBuf	=> 受信バッファ
	戻り値		：無し
	機能概略	：Audioテストモード(BEEP Edit)
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/09/25	leejh		新規作成
******************************************************************************/
static void Aud_test_edit_beep( BYTE *RcvBuf )
{
	switch ( RcvBuf[0] )
	{
		case 0x20:		/* Beep	*/
			if ( RcvBuf[1] == ON )				/* Beep On			*/
			{
				Io_beep_start();				/* BEEP開始			*/
			}
			else if ( RcvBuf[1] == OFF )		/* Beep Off			*/
			{
				Io_beep_stop();					/* BEEP停止			*/
			}
			else	/* QAC対応 */
			{
				;
			}
			break;
		
		case 0x21:		/* Alarm */
			if ( RcvBuf[1] == ON )				/* Alarm On			*/
			{
				
#if _POWER_DIAG_
				Aud_set_beep(CAUD_REQ_BEEP_ALARM);
#else
				MIO_OUT_pALARM( HIGH );			/* Alarm端子 High	*/
				Io_beep_start();				/* BEEP開始			*/
#endif
			}
			else if ( RcvBuf[1] == OFF )		/* Alarm Off		*/
			{			
#if _POWER_DIAG_
				Aud_cancel_beep();
#else
				MIO_OUT_pALARM( LOW );			/* Alarm端子 Low	*/
				Io_beep_stop();					/* BEEP停止			*/
#endif
			}
			else	/* QAC対応 */
			{
				;
			}
			break;
			
		default:	/* Exception */
			break;
	}
}
#endif
#undef _AUD_MAIN_C_

