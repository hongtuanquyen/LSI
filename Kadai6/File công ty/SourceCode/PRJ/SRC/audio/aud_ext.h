/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		: 	aud_ext.h
	モジュール名	: 	Audioモジュール
	機能概略		: 	外部モジュール向けインターフェース関数定義
						ヘッダーファイル
-------------------------------------------------------------------------------
修正履歴
	番号		日付		担当者			修正内容
	【000】		12/07/13	leejh			新規作成
	【001】		12/09/11	leejh			AUD_BEEP_STATUS(変更)
											 →CAUD_BEEP_STA_ALARM_M_NCONT(追加)
 											 →CAUD_BEEP_STA_ALARM_M_CONT(追加)
 											 →CAUD_BEEP_STA_MULTIPLE_M_NCONT(追加)
 											 →CAUD_BEEP_STA_MULTIPLE_M_CONT(追加)
	【002】		12/09/25	leejh			AUD_AUDIO_POWER_MODE(追加)
********************************************************************************/
#ifndef		_AUD_EXT_H_
#define		_AUD_EXT_H_

#include	"../model.h"

#undef		EXT
#ifdef		_AUD_EXT_C_
#define		EXT
#else
#define		EXT extern
#endif

/*---------------------------------------------------------------------------*
 *	外部モジュール向け定数定義											 	 *
 *---------------------------------------------------------------------------*/
/* 操作情報定数定義 */
enum AUD_AUDIO_INFO_CTRL_DEF {
	CAUD_STP_DOWN		,		/* ステップDOWN			*/
	CAUD_STP_UP			,		/* ステップUP			*/
	CAUD_STP_CENTER		,		/* ステップCENTER		*/
	CAUD_STP_MAX		,		/* ステップMAX			*/
	CAUD_STP_MIN		,		/* ステップMIN			*/
	CAUD_STP_DEF		,		/* 出荷時設定			*/
	CAUD_STP_NOP = 0x0F			/* non operation		*/
};

/* 戻り値形式定数定義 */
enum AUD_RET_VAL_DEF {
	CAUD_HEX			,		/* HEX形式で通知 */
	CAUD_BCD					/* BCD形式で通知 */
};

/* MUTEモジュールID定数定義 */
enum AUD_MUTE_MODULE_ID_DEF{ 
	CAUD_MDL_AUDIO	= 0x01	,	/*	1:AUDIO MODULE		 */
	CAUD_MDL_AUX			,	/*	2:AUX MODULE		 */
	CAUD_MDL_MAIN			,	/*	3:MAIN MODULE		 */
	CAUD_MDL_POWER			,	/*	4:POWER MODULE		 */
	CAUD_MDL_TUNER			,	/*	5:TUNER MODULE		 */
	CAUD_MDL_BAND				/*	6:Bandチェンジ		 */
};

/* MUTE MID定数定義 */
enum AUD_MUTE_MID_DEF {
	/* 通常MUTE */
	CAUD_MUTE_AUD_NR_MUTE	  = ( 0x00 | CAUD_MDL_AUDIO		),
	CAUD_MUTE_AUX_NR_MUTE	  = ( 0x00 | CAUD_MDL_AUX		),
	CAUD_MUTE_MAIN_NR_MUTE	  = ( 0x00 | CAUD_MDL_MAIN		),
	CAUD_MUTE_TUN_NR_MUTE	  = ( 0x00 | CAUD_MDL_TUNER		),
	CAUD_MUTE_BANDCHG_NR_MUTE = ( 0x00 | CAUD_MDL_BAND		),
	
	/* システムMUTE */
	CAUD_MUTE_MAIN_DT_MUTE	  = ( 0x20 | CAUD_MDL_MAIN		),
	CAUD_MUTE_PWR_DT_MUTE	  = ( 0x20 | CAUD_MDL_POWER		),
	CAUD_MUTE_AUD_DT_MUTE	  = ( 0x20 | CAUD_MDL_AUDIO		),
	
	/* POWER MUTE */
	CAUD_MUTE_PWR_DA_MUTE	  = ( 0x40 | CAUD_MDL_POWER		)
};

/* Volume定数定義 */
enum AUD_AUDIO_VOL_DEF {
	CAUD_VOL_MAX =	31,
	CAUD_VOL_CNT =	13,
	CAUD_VOL_MIN =	 0,
	CAUD_VOL_INI =	13
};

/* Bass定数定義 */
enum AUD_AUDIO_BAS_DEF {
	CAUD_BAS_MAX =	10,
	CAUD_BAS_CNT =	5,
	CAUD_BAS_MIN =	0,
	CAUD_BAS_INI =	5
};

/* Treble定数定義 */
enum AUD_AUDIO_TRE_DEF {
	CAUD_TRE_MAX =	10,
	CAUD_TRE_CNT =	 5,
	CAUD_TRE_MIN =	 0,
	CAUD_TRE_INI =	 5
};

/* Balance定数定義 */
enum AUD_AUDIO_BAL_DEF {
	CAUD_BAL_MAX =	10,
	CAUD_BAL_CNT =	 5,
	CAUD_BAL_MIN =	 0,
	CAUD_BAL_INI =	 5
};

/* Fader定数定義 */
enum AUD_AUDIO_FAD_DEF {
	CAUD_FAD_MAX =	10,
	CAUD_FAD_CNT =	 5,
	CAUD_FAD_MIN =	 0,
	CAUD_FAD_INI =	 5
};

/* BEEP制御要求タイプ定数定義 */
enum AUD_REQ_BEEP_DEF {
	CAUD_REQ_BEEP_NOP		=0x00	,
	CAUD_REQ_BEEP_LONGKEY			,		/* キー長押し	*/
	CAUD_REQ_BEEP_ALARM				,		/* アラーム		*/
	CAUD_REQ_BEEP_MULTIPLE			,		/* 連続			*/
	CAUD_REQ_BEEP_MAX
};

/* BEEP制御状態定数定義 */
enum AUD_BEEP_STATUS {
	CAUD_BEEP_STA_OFF		=0x00	,	/* OFF						*/
	
	/*---------Mute制御無し-------*/
	CAUD_BEEP_STA_LONGKEY_M_NCONT	,	/* キー長押し				*/
	CAUD_BEEP_STA_ALARM_M_NCONT		,	/* アラーム					*/
	CAUD_BEEP_STA_MULTIPLE_M_NCONT	,	/* 連続						*/
	
	/*---------Mute制御あり-------*/
	CAUD_BEEP_STA_LONGKEY_M_CONT	,	/* キー長押し				*/
	CAUD_BEEP_STA_ALARM_M_CONT		,	/* アラーム					*/
	CAUD_BEEP_STA_MULTIPLE_M_CONT		/* 連続						*/
};

/* MUTE状態 */
enum AUD_MUTE_STATUS {
	CAUD_MUTE_ON_END	,			/* MUTE ON処理完了			*/
	CAUD_MUTE_OFF_END	,			/* MUTE OFF処理完了			*/
	CAUD_MUTE_ON_BUSY	,			/* MUTE ON処理中			*/
	CAUD_MUTE_OFF_BUSY				/* MUTE OFF処理中			*/
};

/* 内部電源モード */
enum AUD_AUDIO_POWER_MODE {
 	CAUD_PWR_OFF	,				/* Power Off				*/
 	CAUD_PWR_ONSEQ	,				/* Power On Seq				*/
 	CAUD_PWR_ON		,				/* Power On					*/
 	CAUD_PWR_OFFSEQ					/* Power Off Seq			*/
};

/*---------------------------------------------------------------------------*
 *	外部モジュール向けプロトタイプ宣言									 	 *
 *---------------------------------------------------------------------------*/
/* E-VOL制御関数 */
EXT void Aud_set_vol_step( BYTE );
EXT void Aud_set_bas_step( BYTE );
EXT void Aud_set_tre_step( BYTE );
EXT void Aud_set_bal_step( BYTE );
EXT void Aud_set_fad_step( BYTE );

EXT BYTE Aud_get_vol_step( void );
EXT BYTE Aud_get_bas_step( void );
EXT BYTE Aud_get_tre_step( void );
EXT BYTE Aud_get_bal_step( void );
EXT BYTE Aud_get_fad_step( void );

/* ミュート制御関数 */
EXT void Aud_set_prg_mute( BYTE, BYTE );

/* BEEP制御関数 */
EXT void Aud_set_beep( BYTE );
EXT void Aud_cancel_beep( void );
EXT BYTE Aud_get_beep_type( void );

EXT void Aud_event_1ms( void );
EXT void Aud_event_5ms( void );
EXT void Aud_event_10ms( void );

BYTE Aud_incr( BYTE, BYTE );
BYTE Aud_decr( BYTE, BYTE );
BYTE Aud_add( BYTE, BYTE, BYTE );
BYTE Aud_sub( BYTE, BYTE );

EXT BYTE Aud_get_mute_status( void );
EXT BYTE Aud_get_power_status( void );

#if _TEST_MODE_
EXT void Aud_test_func( BYTE, BYTE *, BYTE );
EXT void Aud_secret_loud_key( void );
#endif
#endif
