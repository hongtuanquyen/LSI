/***************************************************************
	クラリオン殿向け
	プロジェクト名	：2012年AMFM_RADIOモデル
	ファイル名		：off_key.h
	モジュール名	：OFFモード
	機   能			：OFFモードヘッダファイル
----------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
****************************************************************/
#ifndef		_OFF_KEY_H_
#define		_OFF_KEY_H_

#include	"../power/pw_func_ext.h"

#undef	EXT
#ifdef	_OFF_KEY_C_
#define	EXT
#else
#define	EXT	extern
#endif

EXT	TCOM_FLG off_flag;
#define		foff_colck_mode		(off_flag.bit.bit7)			/* 時計フラグ */

/*------------------------------------------
		関数プロトタイプ
-------------------------------------------*/
EXT KEY_CHG_TBL * Off_keyCodeTable_addr_get(void);
EXT KEY_PROC_TBL * Off_key_func_addr_get(void);
void  Off_key_pw_long(void);
void  Off_key_alarm_on(void);
void  Off_key_alarm_long(void);
void  Off_key_alarm_off(void);
void  Off_key_testp1(void);
void  Off_key_testp2(void);
void  Off_key_testp3(void);

#ifdef	_OFF_KEY_C_
enum
{
	COFF_KY_PWR = 1,
	COFF_KY_SRC,
	COFF_KY_ALARM,
	COFF_KY_TEST1_P,
	COFF_KY_TEST2_P,
	COFF_KY_TEST3_P,
	COFF_KY_MAX
};

/*-------------keyコード変換テーブル-----------*/
const KEY_CHG_TBL off_key_chg_tbl[] =
	{
		{CKEY_ID_LCD_PWR,		COFF_KY_PWR		},
		{CKEY_ID_LCD_SRC,		COFF_KY_SRC		},
		{CKEY_ID_LCD_ALARM,		COFF_KY_ALARM	},
		{CKEY_ID_LCD_TEST1_P,	COFF_KY_TEST1_P	},
		{CKEY_ID_LCD_TEST2_P,	COFF_KY_TEST2_P	},
		{CKEY_ID_LCD_TEST3_P,	COFF_KY_TEST3_P	},
		{CKEY_ID_INVALID,		CKEY_ID_INVALID	}
	};

/*-------------キーテーブル---------------------*/
			/* keycode 		*/
			/* keyON処理	*/
			/* keyLONG処理	*/
			/* keyOFF処理	*/
const KEY_PROC_TBL off_key_tbl[]=
{
	{ COFF_KY_PWR,
	  BIN_11111111, BIN_11111111, Key_long_time1000,
	  BIN_11111111, BIN_11111111, Off_key_pw_long,
	  BIN_11111111, BIN_11111111, Pw_power_key },
	  
	{ COFF_KY_SRC,
	  BIN_11111111, BIN_11111111, Key_long_time2000,
	  BIN_11111111, BIN_11111111, Key_nop,
	  BIN_11111111, BIN_11111111, Pw_power_key },
	  
	{ COFF_KY_ALARM,
	  BIN_11111111, BIN_11111111, Off_key_alarm_on,
	  BIN_11111111, BIN_11111111, Off_key_alarm_long,
	  BIN_11111111, BIN_11111111, Off_key_alarm_off },

	{ COFF_KY_TEST1_P,
	  BIN_11111111, BIN_11111111, Off_key_testp1,
	  BIN_11111111, BIN_11111111, Key_nop,
	  BIN_11111111, BIN_11111111, Key_nop },

	{ COFF_KY_TEST2_P,
	  BIN_11111111, BIN_11111111, Off_key_testp2,
	  BIN_11111111, BIN_11111111, Key_nop,
	  BIN_11111111, BIN_11111111, Key_nop },

	{ COFF_KY_TEST3_P,
	  BIN_11111111, BIN_11111111, Off_key_testp3,
	  BIN_11111111, BIN_11111111, Key_nop,
	  BIN_11111111, BIN_11111111, Key_nop },


	{ CKEY_ID_INVALID,
	  0, 			0,			  Key_nop,
	  0, 			0,			  Key_nop,
	  0, 			0,			  Key_nop}
};



#endif
#endif