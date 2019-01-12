/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		: 	aud_def.h
	モジュール名	: 	Audioモジュール
	機能概略		: 	モジュール内部定義のヘッダーファイル
-------------------------------------------------------------------------------
修正履歴
	番号		日付		担当者			修正内容
	【000】		12/07/13	leejh			新規作成
	【001】		12/09/25	leejh			AUD_AUDIO_POWER_MODE(削除)
											 →aud_ext.hに移動
 											AUD_LOUD_STATUS(追加)
											AUD_PROI_DIAG(追加)
 											TAUD_DIAG_LOUD(追加)
 											Aud_diag_loud(追加)
********************************************************************************/

#ifndef		_AUD_DEF_H_
#define		_AUD_DEF_H_

#include	"../model.h"

/*---------------------------------------------------------------------------*
 *内部定数定義									 						 	 *
 *---------------------------------------------------------------------------*/
/* ASP(Audio Sound Processor)制御処理への命令定義  */
enum AUD_ASP_INSTRUCTION {
	CAUD_ASP_SYS_RESET	=1		,	/* ASPシステムリセット					 */
	CAUD_ASP_INIT_AUDIO			,	/* ASPデータ初期設定					 */
	CAUD_ASP_VOL				,	/* volume調整							 */
	CAUD_ASP_BAS				,	/* bass調整								 */
	CAUD_ASP_TRE				,	/* treble調整							 */
	CAUD_ASP_BAL				,	/* balance調整							 */
	CAUD_ASP_FAD				,	/* fader調整							 */
	CAUD_ASP_SMUTE_ON			,	/* soft mute on							 */
	CAUD_ASP_SMUTE_OFF			,	/* soft mute off						 */
	CAUD_ASP_SRC_CHG			,	/* モード切換え							 */
	CAUD_ASP_ALL_AUDIO			,	/* 全オーディオデータ設定				 */
	CAUD_ASP_INSTRUCTION_END
};

/* AUDIO調整要求定数定義 */
enum AUD_ADJUST_ID {
	CAUD_ADJ_VOL	= 0x01	,	/*  1:メイン音量調整		 */
	CAUD_ADJ_BAS			,	/*  2:BASS調整				 */
	CAUD_ADJ_TRE			,	/*  3:TREBLE調整			 */
	CAUD_ADJ_BAL			,	/*  4:BALANCE調整			 */
	CAUD_ADJ_FAD			,	/*  5:FADER調整				 */
	CAUD_ADJ_END				/*	XX:終端					*/	
};

/* タイマ種別 */
enum AUD_AUDIO_TIMER_TYPE_DEF{
	CAUD_CNT_01MS	,		/* 1msタイマカウンタ	 */
	CAUD_CNT_05MS	,		/* 5msタイマカウンタ	 */
	CAUD_CNT_10MS	,		/* 10msタイマカウンタ	 */
	CAUD_CNT_25MS	,		/* 25msタイマカウンタ	 */
	CAUD_CNT_1SEC			/* 1000msタイマカウンタ	 */
};

/* 1msタイマ */
enum AUD_AUDIO_TIMER_01_DEF{
	CAUD_C01MS_DUMMY	,			/* reserved			*/
	CAUD_C01MS_MAX					/* 最後				*/
};

/* 5msタイマ */
enum AUD_AUDIO_TIMER_05_DEF{
	CAUD_C05MS_DUMMY			,	/* reserved				*/
	CAUD_C05MS_ASP_WAIT			,	/* ASP waitタイマ		*/
	CAUD_C05MS_PMEUT_TIM		,	/* Premute Waitタイマ	*/
	CAUD_C05MS_BEEP_TIM			,	/* BEEP Waitタイマ		*/
	CAUD_C05MS_AMUTE_END_TIM	,	/* Mute Endタイマ		*/
	CAUD_C05MS_MAX					/* 最後					*/
};

/* 10msタイマ */
enum AUD_AUDIO_TIMER_10_DEF{
	CAUD_C10MS_DUMMY			,	/* reserved				*/
	CAUD_C10MS_BAND_CHG_WAIT	,	/* BAND切換えタイマ		*/
	CAUD_C10MS_BEEP_TIM			,	/* BEEP Waitタイマ		*/
	CAUD_C10MS_MAX					/* 最後					*/
};

/* 25msタイマ */
enum AUD_AUDIO_TIMER_25_DEF{
	CAUD_C25MS_DUMMY			,	/* reserved				*/
	CAUD_C25MS_MCNT_ON			,	/* MainCnt On Waitタイマ*/
	CAUD_C25MS_MAX					/* 最後					*/
};

/* 1000msタイマ */
enum AUD_AUDIO_TIMER_1S_DEF{
	CAUD_C1SEC_DUMMY,				/* reserved	*/
	CAUD_C1SEC_MAX					/* 最後		*/
};

/* システムコマンドID */
enum AUD_EVENT_ID_DEF{
	CAUD_EVT_MAIN		=0x01	,	/* MAINイベント				*/
	CAUD_EVT_INITIAL			,	/* INITIALイベント			*/
	CAUD_EVT_BATT_OFF			,	/* BATT OFFイベント			*/
	CAUD_EVT_PWR_ON				,	/* POWER ONイベント			*/
	CAUD_EVT_PWR_OFF			,	/* POWER OFFイベント		*/
	CAUD_EVT_MCNT_ON			,	/* MAINCNT ONイベント		*/
	CAUD_EVT_MCNT_OFFREQ		,	/* MAINCNT OFF REQイベント	*/
	CAUD_EVT_MCNT_OFF			,	/* MAINCNT OFFイベント		*/
	CAUD_EVT_MODE_CHG			,	/* MODE CHANGEイベント		*/
	CAUD_EVT_TIMER_001M			,	/* 1msタイマイベント		*/
	CAUD_EVT_TIMER_005M			,	/* 5msタイマイベント		*/
	CAUD_EVT_TIMER_010M			,	/* 10msタイマイベント		*/
	CAUD_EVT_TIMER_025M			,	/* 25msタイマイベント		*/
	CAUD_EVT_TIMER_050M			,	/* 50msタイマイベント		*/
	CAUD_EVT_TIMER_100M			,	/* 100msタイマイベント		*/
	CAUD_EVT_TIMER_125M			,	/* 125msタイマイベント		*/
	CAUD_EVT_TIMER_500M			,	/* 500msタイマイベント		*/
	CAUD_EVT_TIMER_001S				/* 1secタイマイベント		*/
};

/* モードID定数定義 */
enum AUD_MODE_ID_DEF {
	CAUD_MODE_TUNER_AM	= 0	,	/* 0：Tuner AM	*/
	CAUD_MODE_TUNER_FM		,	/* 1：Tuner FM	*/
	CAUD_MODE_AUX1			,	/* 2：AUX		*/
	CAUD_MODE_MAX				/* Max			*/
};

#if _TEST_MODE_
/* LOUDステータス定数定義(テスト、隠しキー専用) */
typedef enum {
	CAUD_LOUD_STA_UNKNOWN	= 0	,	/* 0：不明		*/
	CAUD_LOUD_STA_OFF			,	/* 1：Loud Off	*/
	CAUD_LOUD_STA_ON			,	/* 2：Loud On	*/
	CAUD_LOUD_STA_MAX				/* 3：Max		*/
} AUD_LOUD_STATUS;

/* LOUD診断優先定義(テスト、隠しキー専用) */
typedef enum {
	CAUD_PRIO_UNKNOWN		= 0	,	/* 0：不明		*/
	CAUD_PRIO_TEST				,	/* 1：テスト	*/
	CAUD_PRIO_SECRET			,	/* 2：隠しキー	*/
	CAUD_PRIO_MAX					/* 3：Max		*/
} AUD_PROI_DIAG;
#endif

/*---------------------------------------------------------------------------*
 *	構造体定義															 	 *
 *---------------------------------------------------------------------------*/
/* AUDIO情報リスト構造体定義 */
typedef struct {
	BYTE			asp_cmd;		/* AUDIOデバイスコマンド		 */
	BYTE			curt_mode;		/* 現在のモード					 */
	BYTE			prev_mode;		/* 前回のモード					 */
	BYTE			curt_src;		/* 現在のサウンドソース			 */
	BYTE			prev_src;		/* 前回のサウンドソース			 */
	BYTE			curt_band;		/* TUNER BAND					 */
	BYTE			vol_step;		/* volume step					 */
	BYTE			bas_step;		/* bass	step					 */
	BYTE			tre_step;		/* treble step					 */
	BYTE			bal_step;		/* balance step					 */
	BYTE			fad_step;		/* fader step					 */
	BYTE			pre_kvol;		/* KEY MUTE開始前のvolume step	 */
	BYTE			pre_evol;		/* 外部MUTE開始前のvolume step	 */
} TAUD_AUDIO_INFO;

/* メインモードID変換用の構造体定義 */
typedef struct {
	BYTE	mid;	/* メインモジュールが定義しているモードID	 */
	BYTE	aid;	/* E-VOLモジュールが扱うモードID			 */
} TAUD_MODE_CONV_DEF;

/* MUTE情報の構造体定義 */
typedef struct {
	BYTE	sys_mute	:1;		/* sys mute	0: off		1: on		 */
	BYTE	data_mute	:1;		/* data mute0:off		1: on		 */
	BYTE	curt_mute	:1;		/* mute状態	0: mute off	1: mute on	 */
	BYTE	onoff_seq	:1;		/* mute処理	0: off処理	1: on処理	 */
	BYTE	reserved	:4;		/* reserved							 */
} TAUD_MUTE_FLAG;

/* 内部モジュール構造体共通定義 */
typedef struct {
	BYTE	max;			/* カウンタ配列の最大数			 */
	BYTE	reserved;		/* padding						 */
	BYTE	*cntr;			/* カウンタ配列の先頭アドレス	 */
} TAUD_TIMER_INFO;

#if _TEST_MODE_
/* LOUD診断構造体定義 */
typedef struct {
	AUD_LOUD_STATUS		test_loud;			/* テスト				 */
	AUD_LOUD_STATUS		secret_loud;		/* 隠しキー				 */
	AUD_PROI_DIAG		prio_diag;			/* 診断優先				 */
} TAUD_DIAG_LOUD;
#endif

/*---------------------------------------------------------------------------*
 *	変数定義															 	 *
 *---------------------------------------------------------------------------*/
TAUD_MUTE_FLAG			aud_mute_state;		/* muteフラグ定義変数		 */
BYTE					Aud_power_mode;		/* AUDIO電源mode			 */
BYTE					Aud_dis_audio;		/* 送信禁止					 */

#if _TEST_MODE_
TAUD_DIAG_LOUD			Aud_diag_loud;		/* LOUD診断管理変数			 */
#endif

#endif
