/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		: 	aud_dev_func.c
	モジュール名	: 	Audioモジュール
	機能概略		: 	E-VOLデバイスドライバ処理
-------------------------------------------------------------------------------
修正履歴
	番号		日付		担当者		関数名			修正内容
	【000】		12/07/13	leejh						新規作成
	【001】		12/09/25	leejh		Aud_encode_volume_gain(変更)
										Aud_encode_fader(変更)
										Aud_encode_loudness(変更)
  	【002】		12/11/20	leejh		Aud_get_tone_index(追加)
										Aud_encode_bass_gain(変更)
										Aud_encode_treble_gain(変更)
										Aud_data_send_req(変更)
										Aud_set_volume(変更)
  	【003】		13/03/05	leejh		Aud_encode_loudness(変更)
  	【004】		13/03/21	leejh		マクロ定義(変更)
********************************************************************************/
#define		_AUD_DEV_FUNC_C_

#include	<string.h>
#include	"../model.h"

#include	"../comm/common.h"
#include	"../io/IO_inc.h"

#include	"aud_ext.h"
#include	"aud_sys.h"
#include	"aud_def.h"
#include	"aud_main.h"
#include	"aud_dev_func.h"
#include	"aud_mute_func.h"
#include	"aud_beep_func.h"

/*---------------------------------------------------------------------------*
 *	構造体定義															 	 *
 *---------------------------------------------------------------------------*/

/* fader情報 */
typedef struct {
	BYTE			left_front;				/* left(1ch) front encode data	 [V13]2chに変更	*/
	BYTE			right_front;			/* right(2ch) front encode data	 [V13]1chに変更	*/
	BYTE			left_rear;				/* left(1ch) rear encode data	 [V13]2chに変更	*/
	BYTE			right_rear;				/* right(2ch) front encode data	 [V13]1chに変更	*/
} TAUD_FADER_INFO;

/* データエンコード情報	 */
typedef struct {
	BYTE			instruction;						/* 命令					*/
	TAUD_FADER_INFO	fader;								/* faderエンコード情報	*/
	BYTE			length;								/* 転送データ長			*/
	BYTE			index;								/* 転送データ書込み位置	*/
	BYTE			buff[CAUD_SET_AUDIO_DATA_LENGHT];	/* 転送バッファ			*/
} TAUD_ENCODE_INFO;

/*---------------------------------------------------------------------------*
 *	内部変数定義														 	 *
 *---------------------------------------------------------------------------*/
static TAUD_AUDIO_INFO		*aud_list_info;		/* AUDIO情報が格納されれるバッファへのポインタ	*/
static TAUD_MUTE_FLAG		*aud_list_mute;		/* MUTE情報が格納されれるバッファへのポインタ	*/
static TAUD_ENCODE_INFO		aud_encode_info;	/* 送信データエンコード情報						*/

static BYTE					aud_prev_bas;		/* 以前 Bass Step	*/
static BYTE					aud_prev_tre;		/* 以前 Treble Step	*/

/*---------------------------------------------------------------------------*
 *	定数定義															 	 *
 *---------------------------------------------------------------------------*/
#define CAUD_BMT_CUT_FLG			0x80
#define CAUD_FADER_GAIN_MUTE		0xFF

/*---------------------------------------------------------------------------*
 *	マクロ定義															 	 *
 *---------------------------------------------------------------------------*/
#define MAUD_FADER_1CH_FRONT			aud_encode_info.fader.right_front
#define MAUD_FADER_2CH_FRONT			aud_encode_info.fader.left_front
#define MAUD_FADER_1CH_REAR				aud_encode_info.fader.right_rear
#define MAUD_FADER_2CH_REAR				aud_encode_info.fader.left_rear

#define	MAUD_SET_ENCODE_DATA(data)								\
		aud_encode_info.buff[aud_encode_info.index] = data;		\
		aud_encode_info.index++

/*---------------------------------------------------------------------------*
 *	プロトタイプ定義													 	 *
 *---------------------------------------------------------------------------*/
static void Aud_send_data( void );
static BYTE Aud_get_volume_step( void );
static BYTE Aud_get_tone_index( BYTE );

static void Aud_encode_pack_header( BYTE );
static void Aud_encode_initial_setup1( void );
static void Aud_encode_initial_setup2( void );
static void Aud_encode_input_selector( void );
static void Aud_encode_input_gain( void );
static void Aud_encode_volume_gain( void );
static void Aud_encode_fader( BYTE );
static void Aud_encode_bass_gain( void );
static void Aud_encode_treble_gain( void );
static void Aud_encode_loudness( void );
static void Aud_encode_system_reset( void );

static void Aud_data_send_req( BYTE, BYTE, BYTE );
static void Aud_set_volume( void );
static void Aud_set_bass( void );
static void Aud_set_treble( void );
static void Aud_set_smute_on( void );
static void Aud_set_smute_off( void );
static void Aud_set_audio_data( void );
static void Aud_set_system_reset( void );
static void Aud_set_init_data( void );

/******************************************************************************
	関数名		：Aud_send_data
	引数		：なし
	戻り値		：なし
	機能概略	：I2C BUS データ転送
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_send_data( void )
{
	BYTE retry_cnt;
	BYTE state;

	/* コンパイラWarning除去 */
	retry_cnt	= 0;
	state		= OK;

	while ( retry_cnt < 3 )
	{
		/* 1バイトデータ出力 */
		state = IO_EVOL_Send( &aud_encode_info.buff[0], aud_encode_info.length );

		/* 通信OKの場合は強制終了 */
		if ( state == OK )
		{
			break;
		}
		retry_cnt++;	/* NGの場合は3回リトライ */
	}
}

/******************************************************************************
	関数名		：Aud_get_volume_step
	引数		：無し
	戻り値		：BYTE	vol_step	:volume step値
	機能概略	：現在有効なvolume step値を算出して通知する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static BYTE Aud_get_volume_step( void )
{
	BYTE vol_step;

	vol_step = 0;

	/* key mute, 外部muteを考慮したvolume stepを算出する	 */
	/* 	vol step - (key mute減衰step + 外部mute減衰step)	 */
	vol_step = Aud_sub( aud_list_info->vol_step,
					 ( aud_list_info->pre_kvol + aud_list_info->pre_evol ));
	
	return ( vol_step );
}

/******************************************************************************
	関数名		：Aud_get_tone_index
	引数		：BYTE	vol_step	:VOL STEP値
	戻り値		：BYTE	tone_index	:VOL STEPにより、Toneテーブルインデックス
	機能概略	：トーンテーブル用インデックスを取得する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/11/20	leejh		新規作成
******************************************************************************/
#define		CAUD_VOL_STEP_IDX_1_MIN			(19)
#define		CAUD_VOL_STEP_IDX_1_MAX			(20)

#define		CAUD_VOL_STEP_IDX_2_MIN			(21)
#define		CAUD_VOL_STEP_IDX_2_MAX			(22)

#define		CAUD_VOL_STEP_IDX_3_MIN			(23)
#define		CAUD_VOL_STEP_IDX_3_MAX			(24)

#define		CAUD_VOL_STEP_IDX_4_MIN			(25)
#define		CAUD_VOL_STEP_IDX_4_MAX			(27)

#define		CAUD_VOL_STEP_IDX_5_MIN			(28)
#define		CAUD_VOL_STEP_IDX_5_MAX			(31)

static BYTE Aud_get_tone_index(BYTE vol_step)
{
	/* ローカル変数宣言 */
	BYTE tone_index;
	
	/* ローカル変数初期化 */
	tone_index = 0;
	
	/* Index 1 */
	if (( vol_step >= CAUD_VOL_STEP_IDX_1_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_1_MAX ))
	{
		tone_index = 1;
	}
	/* Index 2 */
	else if (( vol_step >= CAUD_VOL_STEP_IDX_2_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_2_MAX ))
	{
		tone_index = 2;
	}
	/* Index 3 */
	else if (( vol_step >= CAUD_VOL_STEP_IDX_3_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_3_MAX ))
	{
		tone_index = 3;
	}
	/* Index 4 */
	else if (( vol_step >= CAUD_VOL_STEP_IDX_4_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_4_MAX ))
	{
		tone_index = 4;
	}
	/* Index 5 */
	else if (( vol_step >= CAUD_VOL_STEP_IDX_5_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_5_MAX ))
	{
		tone_index = 5;
	}
	/* Index 0 */
	else
	{
		;
	}
	
	return ( tone_index );
}

#if 1 /*_EVOL_BD37503FV_*/
/******************************************************************************
	関数名		：Aud_get_BD37503FV_slave_address
	引数		：BYTE sel_adrs：Select Address
	戻り値		：なし
	機能概略	：Slave addressを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_encode_pack_header( BYTE sel_adrs )
{
	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( CAUD_DEVICE_ADRS );
	MAUD_SET_ENCODE_DATA( sel_adrs );
}

/******************************************************************************
	関数名		：Aud_encode_initial_setup1
	引数		：なし
	戻り値		：なし
	機能概略	：Initial Setup1を設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
#define		CAUD_INIT_SETUP1		0x00		/* Initial Set */

static void Aud_encode_initial_setup1( void )
{
	BYTE ebuf;

	/* Initial Setup1 */
	ebuf = CAUD_INIT_SETUP1;

	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	関数名		：Aud_encode_initial_setup2
	引数		：なし
	戻り値		：なし
	機能概略	：Initial Setup2を設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
#define		CAUD_INIT_SETUP2		0x00		/* Initial Set */

static void Aud_encode_initial_setup2( void )
{
	BYTE ebuf;

	/* Initial Setup2 */
	ebuf = CAUD_INIT_SETUP2;

	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	関数名		：Aud_encode_input_selector
	引数		：なし
	戻り値		：なし
	機能概略	：Input Selectorを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 ******************************************************************************/
static void Aud_encode_input_selector( void )
{
	BYTE src;
	BYTE ebuf;
	
	/* current modeを取得する */
	src = aud_list_info->curt_src;

	/* 取得したモードがテーブル外 */
	if ( src >= CAUD_MODE_MAX )
	{
		src = CAUD_MODE_TUNER_AM;
	}

	ebuf = aud_input_sel_tbl[src];

	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	関数名		：Aud_encode_input_gain
	引数		：なし
	戻り値		：なし
	機能概略	：Input gainを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_encode_input_gain( void )
{
	BYTE src;
	BYTE ebuf;
	
	/* current modeを取得する */
	src = aud_list_info->curt_src;

	/* 取得したモードがテーブル外 */
	if ( src >= CAUD_MODE_MAX )
	{
		src = CAUD_MODE_TUNER_AM;
	}
	
	ebuf = aud_input_gain_tbl[src];
	
	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	関数名		：Aud_encode_volume_gain
	引数		：なし
	戻り値		：なし
	機能概略	：volume gainを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 ******************************************************************************/
#define CAUD_VOLUME_GAIN_MUTE 0xFF

static void Aud_encode_volume_gain( void )
{
	BYTE vol_step;
	BYTE ebuf;
	
	/* muteを考慮したvolume step値を取得する */
	vol_step = Aud_get_volume_step();

	/* VOL STEPの最大値を超過 */
	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_INI;
	}
	
	/* volumeを設定 */
	ebuf = aud_vol_gain_tbl[vol_step];

	/* Power Offアラームは無音声にする */
	if ( aud_list_mute->data_mute == ON )
	{
		ebuf = (BYTE) CAUD_VOLUME_GAIN_MUTE;
	}
	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	関数名		：Aud_encode_fader
	引数		：BYTE get_data：	CAUD_ENC_FADER_ALL
									CAUD_ENC_FADER_1CH_FRONT
									CAUD_ENC_FADER_2CH_FRONT
									CAUD_ENC_FADER_1CH_REAR
									CAUD_ENC_FADER_2CH_REAR
	戻り値		：なし
	機能概略	：Fader Xch Front/Rearを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
	【001】		13/03/22	leejh		仕様変更(LRch反転)
 ******************************************************************************/
enum
{
	CAUD_ENC_FADER_ALL		= 0x00	,	/* Fader CH	All		*/
	CAUD_ENC_FADER_1CH_FRONT		,	/* Fader 1CH Front	*/
	CAUD_ENC_FADER_2CH_FRONT		,	/* Fader 2CH Front	*/
	CAUD_ENC_FADER_1CH_REAR			,	/* Fader 1CH Rear	*/
	CAUD_ENC_FADER_2CH_REAR				/* Fader 2CH Rear	*/
};

enum
{
	LF = 0x00	,	/* LEFT FRONT	*/
	RF			,	/* RIGHT FRONT	*/
	LR			,	/* LEFT REAR	*/
	RR				/* RIGHT REAR	*/
};

static void Aud_encode_fader( BYTE enc_data )
{
	BYTE bal_step;
	BYTE fad_step;
	BYTE vol_step;
	BYTE fad_att;
	
	BYTE balfad_step[4];
	BYTE balfad_att[4];
	BYTE balfad_enc[4];
	
	/* 変数の初期化（fail safe） */
	MAUD_FADER_1CH_FRONT	= \
	MAUD_FADER_2CH_FRONT	= \
	MAUD_FADER_1CH_REAR		= \
	MAUD_FADER_2CH_REAR		= CAUD_FADER_GAIN_MUTE;

	bal_step = aud_list_info->bal_step;
	fad_step = aud_list_info->fad_step;
	
	vol_step = Aud_get_volume_step();

	if ( bal_step > CAUD_BAL_MAX )
	{
		bal_step = CAUD_BAL_CNT;
	}

	if ( fad_step > CAUD_FAD_MAX )
	{
		fad_step = CAUD_FAD_CNT;
	}

	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_MIN;
	}
	
	/* VOL 0、VOL MUTE、Power Offアラームはfail safeの-∞を送信 */
	if (( vol_step > CAUD_VOL_MIN ) && ( aud_list_mute->data_mute == OFF ))
	{
		/* BALANCE stepをエンコード用のstepに変換する */
		balfad_step[LF] = aud_balfad_step_tbl[bal_step].right;	/* LEFT  FRONT	*/
		balfad_step[RF] = aud_balfad_step_tbl[bal_step].left;	/* RIGHT FRONT	*/
		balfad_step[LR] = aud_balfad_step_tbl[bal_step].right;	/* LEFT  REAR	*/
		balfad_step[RR] = aud_balfad_step_tbl[bal_step].left;	/* RIGHT REAR	*/

		/* FADER stepをエンコード用のstepに変換する */
		balfad_step[LF] += aud_balfad_step_tbl[fad_step].left;	/* LEFT  FRONT	*/
		balfad_step[RF] += aud_balfad_step_tbl[fad_step].left;	/* RIGHT FRONT	*/
		balfad_step[LR] += aud_balfad_step_tbl[fad_step].right;	/* LEFT  REAR	*/
		balfad_step[RR] += aud_balfad_step_tbl[fad_step].right;	/* RIGHT REAR	*/

		/*調整による、BalFad ATT取得 */
		balfad_att[LF] = aud_balfad_tbl[balfad_step[LF]] & 0x7F;
		balfad_att[RF] = aud_balfad_tbl[balfad_step[RF]] & 0x7F;
		balfad_att[LR] = aud_balfad_tbl[balfad_step[LR]] & 0x7F;
		balfad_att[RR] = aud_balfad_tbl[balfad_step[RR]] & 0x7F;
		
		/* Volume Stepによる、Fad ATT取得 */
		fad_att = aud_fad_att_tbl[vol_step] & 0x7F;

		/* エンコード値を格納 */
		balfad_enc[LF] = balfad_att[LF] + fad_att;
		balfad_enc[RF] = balfad_att[RF] + fad_att;
		balfad_enc[LR] = balfad_att[LR] + fad_att;
		balfad_enc[RR] = balfad_att[RR] + fad_att;
		
		if ( balfad_enc[RF] <= 0x3F )
		{
			MAUD_FADER_1CH_FRONT = balfad_enc[RF] | 0x80;
		}
		
		if ( balfad_enc[LF] <= 0x3F )
		{
			MAUD_FADER_2CH_FRONT = balfad_enc[LF] | 0x80;
		}
		
		if ( balfad_enc[RR] <= 0x3F )
		{
#if 1		/* 本モデルはRear CHが-∞dB */
			MAUD_FADER_1CH_REAR = CAUD_FADER_GAIN_MUTE;
#else
			MAUD_FADER_1CH_REAR  = balfad_enc[RR] | 0x80;
#endif
		}
		
		if ( balfad_enc[LR] <= 0x3F )
		{
#if 1		/* 本モデルはRear CHが-∞dB */
			MAUD_FADER_2CH_REAR = CAUD_FADER_GAIN_MUTE;
#else
			MAUD_FADER_2CH_REAR  = balfad_enc[LR] | 0x80;
#endif
		}
	}
	
	/* エンコードしたデータを転送バッファに書込む */
	switch ( enc_data )
	{
		case CAUD_ENC_FADER_1CH_FRONT:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_1CH_FRONT );
			break;

		case CAUD_ENC_FADER_2CH_FRONT:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_2CH_FRONT );
			break;

		case CAUD_ENC_FADER_1CH_REAR:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_1CH_REAR );
			break;

		case CAUD_ENC_FADER_2CH_REAR:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_2CH_REAR );
			break;

		case CAUD_ENC_FADER_ALL:
		default:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_1CH_FRONT );
			MAUD_SET_ENCODE_DATA( MAUD_FADER_2CH_FRONT );
			MAUD_SET_ENCODE_DATA( MAUD_FADER_1CH_REAR );
			MAUD_SET_ENCODE_DATA( MAUD_FADER_2CH_REAR );
			break;
	}
}

/******************************************************************************
	関数名		：Aud_encode_bass_gain
	引数		：なし
	戻り値		：なし
	機能概略	：Bass gainを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
   	【001】		12/11/20	leejh		VOL STEPによる、トーンエンコード処理追加
 ******************************************************************************/
static void Aud_encode_bass_gain( void )
{
	BYTE bas_step;
	BYTE ebuf;
	BYTE vol_step;
	BYTE tone_index;
		
	/* ローカル変数初期化 */
	tone_index = 0;
	
	vol_step = aud_list_info->vol_step;		/* vol step	 */
	bas_step = aud_list_info->bas_step;		/* bass step */

	/* VOL STEPの最大値を超過 */
	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_INI;
	}
	
	/* トーンインデックスを取得する */
	tone_index = Aud_get_tone_index(vol_step);
	
	/* bass step値が範囲を超えている場合 */
	if ( bas_step > CAUD_BAS_MAX )
	{
		bas_step = CAUD_BAS_INI;
	}

	/* データテーブルから値を抽出する */
	ebuf = aud_bas_tre_gain_tbl[tone_index][bas_step];
	
	/* cutフラグ設定										 */
	/* -2dB以下から0dBを設定する場合cutフラグをセットする	 */
	if (( ebuf == 0x00 ) && (( aud_prev_bas & CAUD_BMT_CUT_FLG ) == 0x80 ))
	{
		ebuf |= (BYTE) CAUD_BMT_CUT_FLG;
	}

	aud_prev_bas = ebuf;

	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( ebuf );
}
/******************************************************************************
	関数名		：Aud_encode_treble_gain
	引数		：なし
	戻り値		：なし
	機能概略	：treble gainを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
  	【001】		12/11/20	leejh		VOL STEPによる、トーンエンコード処理追加
 ******************************************************************************/
static void Aud_encode_treble_gain( void )
{
	BYTE tre_step;
	BYTE ebuf;
	BYTE vol_step;
	BYTE tone_index;
	
	/* ローカル変数初期化 */
	tone_index = 0;
	
	vol_step = aud_list_info->vol_step;		/* vol step	 */
	tre_step = aud_list_info->tre_step;		/* treble step */

	/* VOL STEPの最大値を超過 */
	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_INI;
	}
	
	/* トーンインデックスを取得する */
	tone_index = Aud_get_tone_index(vol_step);
	
	/* treble step値が範囲を超えている場合 */
	if ( tre_step > CAUD_TRE_MAX )
	{
		tre_step = CAUD_TRE_INI;
	}

	/* データテーブルから値を抽出する */
	ebuf = aud_bas_tre_gain_tbl[tone_index][tre_step];
	
	/* cutフラグ設定										 */
	/* -2dB以下から0dBを設定する場合cutフラグをセットする	 */
	if (( ebuf == 0x00 ) && (( aud_prev_tre & CAUD_BMT_CUT_FLG ) == 0x80 ))
	{
		ebuf |= (BYTE) CAUD_BMT_CUT_FLG;
	}

	aud_prev_tre = ebuf;

	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	関数名		：Aud_encode_loudness
	引数		：無し
	戻り値		：無し
	機能概略	：loudness gainを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
	【001】		12/09/25	leejh		LOUD診断(テスト、隠しキー)機能対応
	【002】		13/03/05	leejh		LOUD ONモード追加
 ******************************************************************************/
#define		CAUD_LOUDNESS_GAIN_0DB		0x00
	
static void Aud_encode_loudness( void )
{
	BYTE vol_step;
	BYTE ebuf;
	BYTE src;
	
#if _TEST_MODE_
	BYTE loud_onoff;
#endif
	loud_onoff = OFF;		/* Fail Safe */
	
	/* muteを考慮したvolume step値を取得する */
	vol_step = Aud_get_volume_step();

	/* VOL STEPの最大値を超過 */
	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_INI;
	}
	
	/* current modeを取得する */
	src = aud_list_info->curt_src;
	
#if _TEST_MODE_
	/* LOUD診断(テスト)が優先 */
	if ( Aud_diag_loud.prio_diag == CAUD_PRIO_TEST )
	{
		switch( Aud_diag_loud.test_loud )
		{
			/* テストモードLOUD ON */
			case CAUD_LOUD_STA_ON:
				loud_onoff = ON;
				break;

			/* テストモードLOUD OFF */
			case CAUD_LOUD_STA_OFF:
				break;

			/* その他 */
			default:
				break;
		}
	}
	/* LOUD診断(隠しキー)が優先 */
	else if ( Aud_diag_loud.prio_diag == CAUD_PRIO_SECRET )
	{
		switch( Aud_diag_loud.secret_loud )
		{
			/* 隠しキーLOUD ON */
			case CAUD_LOUD_STA_ON:
				loud_onoff = ON;
				break;

			/* 隠しキーLOUD OFF */
			case CAUD_LOUD_STA_OFF:
				break;

			/* その他 */
			default:
				break;
		}
	}
	/* その他(通常) */
	else
	{
		/* FMモード LOUD ON */
		if ( (src == CAUD_MODE_TUNER_FM) || (src == CAUD_MODE_AUX1) )
		{
			loud_onoff = ON;		
		}
	}
	
	if ( loud_onoff == ON )		/* LOUD ONエンコード　*/
	{
		/* Loudness Gain取得 */
		ebuf = aud_loudness_gain_tbl[vol_step];
	}
	else						/* LOUD OFFエンコード　*/
	{
		/* その他 Loudness Gain 0dB */
		ebuf = (BYTE) CAUD_LOUDNESS_GAIN_0DB;
	}
#else
	if ( src == CAUD_MODE_TUNER_FM )
	{
		/* Loudness Gain取得 */
		ebuf = aud_loudness_gain_tbl[vol_step];
	}
	else
	{
		/* その他 Loudness Gain 0dB */
		ebuf = (BYTE) CAUD_LOUDNESS_GAIN_0DB;
	}
#endif
	
	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	関数名		：Aud_encode_system_reset
	引数		：なし
	戻り値		：なし
	機能概略	：System Resetを設定する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 ******************************************************************************/
/* -------------System Reset (SA FE[D7~D0])------------------- */
#define	CAUD_SYSTEM_RESET				0x81

static void Aud_encode_system_reset( void )
{
	/* エンコードしたデータを転送バッファに書込む */
	MAUD_SET_ENCODE_DATA( CAUD_SYSTEM_RESET );
}

/******************************************************************************	
	Item送信：各Itemごとに送信を行う。
	操作送信：キー操作により、必要なItem送信を行う。
	その他：その他、本システムにて必要なItem送信を行う
	※各Item(Blk)のサイズを考慮し、送信要求設定を行ってください。
	  詳細は「BD35703FVデータシート」を参照してください。
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
enum
{
	CAUD_SEND_TYPE_NOP		= 0x00	,		/* NOP				*/
	/* Item送信 */
	CAUD_SEND_TYPE_INITAL_SETUP1	,		/* Initial Setup1	*/
	CAUD_SEND_TYPE_INITAL_SETUP2	,		/* Initial Setup2	*/
	CAUD_SEND_TYPE_INPUT_SELECTOR	,		/* Input Selector	*/
	CAUD_SEND_TYPE_INPUT_GAIN		,		/* Input Gain		*/
	CAUD_SEND_TYPE_VOLUME_GAIN		,		/* Volume Gain		*/
	CAUD_SEND_TYPE_FADER_1CHF		,		/* Fader 1CH Front	*/
	CAUD_SEND_TYPE_FADER_2CHF		,		/* Fader 2CH Front	*/
	CAUD_SEND_TYPE_FADER_1CHR		,		/* Fader 1CH Rear	*/
	CAUD_SEND_TYPE_FADER_2CHR		,		/* Fader 2CH Rear	*/
	CAUD_SEND_TYPE_BASS_GAIN		,		/* Bass Gain		*/
	CAUD_SEND_TYPE_TREBLE_GAIN		,		/* Treble Gain		*/
	CAUD_SEND_TYPE_LOUDNESS_GAIN	,		/* Loudness Gain	*/
	CAUD_SEND_TYPE_SYSTEM_RESET		,		/* System Reset		*/
	/* 操作送信*/
	CAUD_SEND_TYPE_VOLUME_CONTROL	,		/* Volume Control	*/
	CAUD_SEND_TYPE_BASS_CONTROL		,		/* Bass	Control		*/
	CAUD_SEND_TYPE_TREBLE_CONTROL	,		/* Treble Control	*/
	CAUD_SEND_TYPE_BALANCE_CONTROL	,		/* Balance Control	*/
	CAUD_SEND_TYPE_FADER_CONTROL	,		/* Fader Control	*/
	/* その他 */
	CAUD_SEND_TYPE_SOFTMUTE_ON		,		/* Softmute On		*/
	CAUD_SEND_TYPE_SOFTMUTE_OFF		,		/* Softmute Off		*/
	CAUD_SEND_TYPE_ALL_AUDIO		,		/* All				*/
	CAUD_SEND_TYPE_INIT_AUDIO		,		/* Initial			*/
	CAUD_SEND_TYPE_MAX						/* MAX				*/
};

/******************************************************************************
	関数名		：Aud_data_send_req
	引数		：BYTE	saddr	：	Select Address
				  BYTE	type	：	送信タイプ
				  BYTE	len		：	送信長さ(MAX：CAUD_SET_AUDIO_DATA_LENGHT)
	戻り値		：なし
	機能概略	：Audioデータ送信を要求する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
	【000】		12/11/20	leejh		VOL/BAL/FAD調整時、BAS/TREエンコード
******************************************************************************/
static void Aud_data_send_req( BYTE saddr, BYTE type, BYTE len )
{
	BYTE send_req;
	
	send_req = ON;		/* デフォルトは送信 ON		*/

	if ( len > 2 )		/* 最低3Blkから受け付ける	*/
	{
		aud_encode_info.index	= 0;		/* Indexクリア(Fail Safe)	*/
		aud_encode_info.length	= len;		/* 転送情報を設定する		*/

		Aud_encode_pack_header( saddr );	/* Slave, Select addr(2Blk)	*/

		switch ( type )		/* 要求による、Itemエンコード */
		{
			/*-------------各Item送信-------------*/
			case CAUD_SEND_TYPE_INITAL_SETUP1:	/* Initial Setup1(1Blk)	*/
				Aud_encode_initial_setup1();
				break;
				
			case CAUD_SEND_TYPE_INITAL_SETUP2:	/* Initial Setup2(1Blk)	*/
				Aud_encode_initial_setup2();
				break;
				
			case CAUD_SEND_TYPE_INPUT_SELECTOR:	/* Input Selector(1Blk)	*/
				Aud_encode_input_selector();
				break;
				
			case CAUD_SEND_TYPE_INPUT_GAIN:		/* Input Gain(1Blk)		*/
				Aud_encode_input_gain();
				break;
				
			case CAUD_SEND_TYPE_VOLUME_GAIN:	/* Volume Gain(1Blk)	*/
				Aud_encode_volume_gain();
				break;
				
			case CAUD_SEND_TYPE_FADER_1CHF:		/* Fader 1CH Front(1Blk)*/
				Aud_encode_fader( CAUD_ENC_FADER_1CH_FRONT );	
				break;

			case CAUD_SEND_TYPE_FADER_2CHF:		/* Fader 2CH Front(1Blk)*/
				Aud_encode_fader( CAUD_ENC_FADER_2CH_FRONT );
				break;

			case CAUD_SEND_TYPE_FADER_1CHR:		/* Fader 1CH Rear(1Blk)	*/
				Aud_encode_fader( CAUD_ENC_FADER_1CH_REAR );
				break;

			case CAUD_SEND_TYPE_FADER_2CHR:		/* Fader 2CH Rear(1Blk)	*/
				Aud_encode_fader( CAUD_ENC_FADER_2CH_REAR );
				break;
						
			case CAUD_SEND_TYPE_BASS_GAIN:		/* Bass Gain(1Blk)		*/
				Aud_encode_bass_gain();
				break;
				
			case CAUD_SEND_TYPE_TREBLE_GAIN:	/* Treble Gain(1Blk)	*/
				Aud_encode_treble_gain();		
				break;
				
			case CAUD_SEND_TYPE_LOUDNESS_GAIN:	/* Loudness Gain(1Blk)	*/
				Aud_encode_loudness();
				break;
			
			case CAUD_SEND_TYPE_SYSTEM_RESET:
				Aud_encode_system_reset();		/* System Reset(1Blk)	*/
				break;
				
			/*-------------操作送信-------------*/
			case CAUD_SEND_TYPE_VOLUME_CONTROL:	/* Volume Control		*/
			case CAUD_SEND_TYPE_BALANCE_CONTROL:/* Balance Control		*/
			case CAUD_SEND_TYPE_FADER_CONTROL:	/* Fader Control		*/
				Aud_encode_volume_gain();				/* 1Blk	*/
				Aud_encode_fader( CAUD_ENC_FADER_ALL );	/* 4Blk	*/
				Aud_encode_bass_gain();					/* 1Blk	*/
				Aud_encode_treble_gain();				/* 1Blk	*/
				Aud_encode_loudness();					/* 1Blk	*/
				break;

			case CAUD_SEND_TYPE_BASS_CONTROL:	/* Bass Control			*/
				Aud_encode_bass_gain();			/* 1Blk	*/
				break;

			case CAUD_SEND_TYPE_TREBLE_CONTROL:	/* Treble Control		*/
				Aud_encode_treble_gain();		/* 1Blk	*/
				break;

			/*-------------その他-------------*/
			/* 上位E-VOLシリーズ対応(Softmute) */
			case CAUD_SEND_TYPE_SOFTMUTE_ON:	/* Softmute On			*/
			case CAUD_SEND_TYPE_SOFTMUTE_OFF:	/* Softmute Off			*/
				Aud_encode_input_gain();		/* 1Blk	*/
				break;
				
			case CAUD_SEND_TYPE_ALL_AUDIO:				/* All Send		*/
				Aud_encode_input_selector();			/* 1Blk			*/
				Aud_encode_input_gain();				/* 1Blk			*/
				Aud_encode_volume_gain();				/* 1Blk			*/
				Aud_encode_fader( CAUD_ENC_FADER_ALL );	/* 4Blk			*/
				Aud_encode_bass_gain();					/* 1Blk			*/
				Aud_encode_treble_gain();				/* 1Blk			*/
				Aud_encode_loudness();					/* 1Blk			*/
				break;

			case CAUD_SEND_TYPE_INIT_AUDIO:		/* Initial Send(12Blk)	*/
				/* 初期値テーブルの値を持ってくる						*/
				/* buffはSlave Adress、Select addressを考慮して書き込むこと */
				romcopy(( BYTE * ) & aud_encode_info.buff[2],
						( BYTE * ) & aud_EVOL_init_data_tbl[0], sizeof ( aud_EVOL_init_data_tbl ));
				break;
				
			default:	/* Exception */
				send_req = OFF;					/* 送信しない		*/
				break;
		}
	}
	else
	{
		send_req = OFF;		/* 送信しない */
	}
	
	if ( send_req == ON )	/* 送信要求あり? */
	{
		Aud_send_data();	/* エンコードしたデータを転送する */
	}
}

/******************************************************************************
	関数名		：Aud_set_volume
	引数		：なし
	戻り値		：なし
	機能概略	：volume,balance,fader調整時のaudioデータ転送処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
	【001】		12/11/20	leejh		VOL/BAL/FAD調整時、BAS/TREエンコード
 *										Item(Loudness Gain)個別送信削除
******************************************************************************/
static void Aud_set_volume( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Vol~Loud(8Blk)= 10Blk */
	Aud_data_send_req( CAUD_SADR_VOLUME_GAIN, CAUD_SEND_TYPE_VOLUME_CONTROL, 10 );
}

/******************************************************************************
	関数名		：Aud_set_bass
	引数		：なし
	戻り値		：なし
	機能概略	：bass調整時のaudioデータ転送処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_set_bass( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Bass(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_BASS_GAIN,  CAUD_SEND_TYPE_BASS_CONTROL, 3 );
}

/******************************************************************************
	関数名		：Aud_set_treble
	引数		：なし
	戻り値		：なし
	機能概略	：treble調整時のaudioデータ転送処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
static void Aud_set_treble( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Treb(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_TREBLE_GAIN,  CAUD_SEND_TYPE_TREBLE_CONTROL, 3 );
}

/******************************************************************************
	関数名		：Aud_set_smute_on
	引数		：なし
	戻り値		：なし
	機能概略	：soft mute onを転送する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成(上位シリーズ用)
******************************************************************************/
static void Aud_set_smute_on( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Input(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_INPUT_GAIN,  CAUD_SEND_TYPE_SOFTMUTE_ON, 3 );
}

/******************************************************************************
	関数名		：Aud_set_smute_off
	引数		：なし
	戻り値		：なし
	機能概略	：soft mute offを転送する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成(上位シリーズ用)
******************************************************************************/
static void Aud_set_smute_off( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+InputGain(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_INPUT_GAIN,  CAUD_SEND_TYPE_SOFTMUTE_OFF, 3 );
}

/******************************************************************************
	関数名		：Aud_set_audio_data
	引数		：なし
	戻り値		：なし
	機能概略	：オーディオデータ転送
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 ******************************************************************************/
static void Aud_set_audio_data( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+InputSel~Loud(10Blk)= 12Blk */
	Aud_data_send_req( CAUD_SADR_INPUT_SELECTOR,  CAUD_SEND_TYPE_ALL_AUDIO, 12 );
}

/******************************************************************************
	関数名		：Aud_set_system_reset
	引数		：なし
	戻り値		：なし
	機能概略	：BD37503FV system resetデータを転送する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 ******************************************************************************/
static void Aud_set_system_reset( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+SysRst(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_SYSTEM_RESET,  CAUD_SEND_TYPE_SYSTEM_RESET, 3 );
}

/******************************************************************************
	関数名		：Aud_set_init_data
	引数		：なし
	戻り値		：なし
	機能概略	：BD37503FV初期設定データを転送する
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 ******************************************************************************/
static void Aud_set_init_data( void )
{
	BYTE len;
	
	/* テーブルの長さ取得(Slave Address+Select Address(2Blk)を足す */
	len =  sizeof( aud_EVOL_init_data_tbl ) + 2;
	
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Init1~Loud(12Blk)= 14Blk */
	Aud_data_send_req( CAUD_SADR_INITIAL_SETUP1,  CAUD_SEND_TYPE_INIT_AUDIO, len );
}
#endif /* _EVOL_BD37503FV_ */

/******************************************************************************
	関数名		：Aud_ctrl_ASP
	引数		：BYTE cmd_adrs	:	コマンドID
	戻り値		：なし
	機能概略	：ASP(Audio Sound Processor)制御メイン処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
typedef void (*CAUD_ASP_FUNC)( void );
const CAUD_ASP_FUNC aud_ASP_func_tbl[] = {
	(void *)0				,
	Aud_set_system_reset	,		/* CAUD_ASP_SYS_RESET			: ASPシステムリセット	 */
	Aud_set_init_data		,		/* CAUD_ASP_INIT_AUDIO			: ASPデータ初期設定		 */
	Aud_set_volume			,		/* CAUD_ASP_VOL					: volume調整			 */
	Aud_set_bass			,		/* CAUD_ASP_BAS					: bass調整				 */
	Aud_set_treble			,		/* CAUD_ASP_TRE					: treble調整			 */
	Aud_set_volume			,		/* CAUD_ASP_BAL					: balance調整			 */
	Aud_set_volume			,		/* CAUD_ASP_FAD					: fader調整				 */
	Aud_set_smute_on		,		/* CAUD_ASP_SMUTE_ON			: soft mute on			 */
	Aud_set_smute_off		,		/* CAUD_ASP_SMUTE_OFF			: soft mute off			 */
	Aud_set_audio_data		,		/* CAUD_ASP_SRC_CHG				: モード切換え			 */
	Aud_set_audio_data				/* CAUD_ASP_ALL_AUDIO			: 全オーディオデータ設定 */
};

void Aud_ctrl_ASP( BYTE instruction )
{
	if ( instruction < CAUD_ASP_INSTRUCTION_END )
	{
		/* ASPへの命令を保存する */
		aud_encode_info.instruction = instruction;

		if ( aud_ASP_func_tbl[instruction] )
		{
			/* データエンコード・転送処理を開始する */
			(*( aud_ASP_func_tbl[instruction] ))();
		}
	}
}

/******************************************************************************
	関数名		：Aud_init_dev_func
	引数		：なし
	戻り値		：なし
	機能概略	：E-VOLデバイス制御ブロック初期化処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_init_dev_func( void )
{
	/* AUDIO情報が格納されている領域のアドレスを取得する */
	aud_list_info = Aud_get_audio_info_list();
	aud_list_mute = Aud_get_mute_state_list();
	
	/* encode情報初期化 */
	memset((BYTE *) &aud_encode_info, (BYTE) 0, (BYTE) sizeof( aud_encode_info ));
}

/******************************************************************************
	関数名		：Aud_send_initial
	引数		：無し
	戻り値		：無し
	機能概略	：E-VOL初期化データを送信
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
 ******************************************************************************/
void Aud_send_initial( void )
{
	/* BD37503FV system reset */
	Aud_set_system_reset();

	/* BD37503FV initial */
	Aud_set_init_data();
}

#undef _AUD_DEV_FUNC_C_

