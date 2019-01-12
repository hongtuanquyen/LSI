/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_keytbl.h
		機能			: Tunerｷｰ定義ﾍｯﾀﾞﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051214]	新規作成
[zhang070306]CLARION仕様変更対応
[zhang070307]Tuner ISRﾓｰﾄﾞキーﾃｰﾌﾞﾙ
[zhang070308]CLARION仕様変更対応
[zhang070309]CLARION仕様変更対応
******************************************************************************/
#ifndef		_TU_KEYTBL_H_
#define		_TU_KEYTBL_H_

#include	"../comm/common.h"
#include	"../submode/SBM_inc.h"		/* jyo修正 */

#include	"tu_ext.h"

/* ｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ */
#ifdef	_TU_KEY_C_
const KEY_CHG_TBL tu_key_chg_tbl[] =
{
	{	CKEY_ID_LCD_VOLUP,		CTU_KY_VOL_UP	},
	{	CKEY_ID_LCD_VOLDW,		CTU_KY_VOL_DW	},
	{	CKEY_ID_LCD_PWR,		CTU_KY_PWR		},
	{	CKEY_ID_LCD_SRC,		CTU_KY_SRC		},
	{	CKEY_ID_LCD_RIGHT,		CTU_KY_FREQ_UP	},
	{	CKEY_ID_LCD_LEFT,		CTU_KY_FREQ_DW	},
	{	CKEY_ID_LCD_CH,			CTU_KY_CH		},
	{	CKEY_ID_LCD_AS,			CTU_KY_AS		},
	{	CKEY_ID_LCD_ALARM,		CTU_KY_ALARM	},
	{	CKEY_ID_LCD_TI_SCAN,	CTU_KY_TISC		},

#if 0		/*testモード関連*/	
	{	CKEY_ID_LCD_TEST,		CTU_KY_TEST		},
	{	CKEY_ID_LCD_SP1,		CTU_KY_STMN		},	/*[zhang070727] ST TEST込みをキー追加*/
#endif		/*testモード関連*/
	{	CKEY_ID_INVALID,		CKEY_ID_INVALID	}			/* end */
};

/*------------------------------------------------------------------
  	Tuner ｷｰ処理 Table
--------------------------------------------------------------------
ｷｰ有効ﾋﾞｯﾄ 16ﾋﾞｯﾄ：
xx|xx|xx|xx|xx|MUTE|SOUND|ADJ|xx|xx|xx|xx|SCAN|AS|SEEK|PWRON
--------------------------------------------------------------------*/
/* 通常受信状態ｷｰﾃｰﾌﾞﾙ */
/*-----------------------------------------------------------------
	UWHD向けキーﾃｰﾌﾞﾙ
-------------------------------------------------------------------*/
const KEY_PROC_TBL tu_normal_key_tbl[]=
{
	{ CTU_KY_VOL_UP,
	  BIN_00010111,BIN_00000001, Sbm_key_volupon,
	  BIN_00010111,BIN_00000001, Sbm_key_voluplong,
	  BIN_00010111,BIN_00000001, Sbm_key_volupoff  },
	{ CTU_KY_VOL_DW,
	  BIN_00010111,BIN_00000001, Sbm_key_voldwon,
	  BIN_00010111,BIN_00000001, Sbm_key_voldwlong,
	  BIN_00010111,BIN_00000001, Sbm_key_voldwoff  },
	{ CTU_KY_PWR,
	  BIN_00010111,BIN_00001111, Key_long_time1000,
	  BIN_00010111,BIN_00001111, Tu_key_pw_long,
	  BIN_00010111,BIN_00001111, Tu_key_pw_off	},	
	{ CTU_KY_SRC,
	  BIN_00010111,BIN_00001111, Key_long_time2000,
	  BIN_00010111,BIN_00000001, Tu_key_md_long,
	  BIN_00010111,BIN_00001111, Tu_key_md_off	},	
	{ CTU_KY_FREQ_UP,
	  BIN_00010111,BIN_00001010, Tu_key_up_on,
	  BIN_00010111,BIN_00000000, Tu_key_up_long,
	  BIN_00010111,BIN_00000000, Tu_key_up_off	},
	{ CTU_KY_FREQ_DW,
	  BIN_00010111,BIN_00001010, Tu_key_dw_on,
	  BIN_00010111,BIN_00000000, Tu_key_dw_long,
	  BIN_00010111,BIN_00000000, Tu_key_dw_off	},
	{ CTU_KY_CH,
	  BIN_00010111,BIN_00001110, Tu_key_ch_on,
	  BIN_00010101,BIN_00000000, Tu_key_ch_long,
	  BIN_00010101,BIN_00000000, Tu_key_ch_off	},
	{ CTU_KY_AS,
	  BIN_00010101,BIN_00001110, Tu_key_as_on,
	  BIN_00010101,BIN_00000000, Tu_key_as_long,
	  BIN_00000000,BIN_00000000, Key_nop		},
	{ CTU_KY_ALARM,
	  BIN_00010111,BIN_00001111, Tu_key_alm_on,
	  BIN_00010111,BIN_00001111, Tu_key_alm_long,
	  BIN_00010111,BIN_00001111, Tu_key_alm_off	},
	{ CTU_KY_TISC,
	  BIN_00010111,BIN_00001111, Tu_key_tisc_on,
	  BIN_00010111,BIN_00001111, Tu_key_ti_long,
	  BIN_00010111,BIN_00001111, Tu_key_ti_off	},
#if 0		/*testモード関連*/	
	{ CTU_KY_TEST,
	  BIN_00000111,BIN_00001110, Tu_key_test,
	  BIN_00000000,BIN_00000000, Key_nop,
	  BIN_00000000,BIN_00000000, Key_nop	},	  
	{ CTU_KY_STMN,
	  BIN_00000110,BIN_00001110, Tu_key_stmo_on,
	  BIN_00000110,BIN_00001110, Tu_key_stmo_long,
	  BIN_00000000,BIN_00000000, Key_nop	},	  
#endif		/*testモード関連*/
	{ CKEY_ID_INVALID,						/* end */
	  0, 			0,			  0,
	  0, 			0,            0,
	  0, 			0,			  0				}
};
/*-----------------------------------------------------------------
	ISRﾓｰﾄﾞキーﾃｰﾌﾞﾙ

ｷｰ有効ﾋﾞｯﾄ 16ﾋﾞｯﾄ：
xx|xx|xx|xx|xx|MUTE|SOUND|ADJ|xx|xx|xx|xx|SCAN|APM|SEEK|PWRON
-------------------------------------------------------------------*/
const KEY_PROC_TBL tu_isr_key_tbl[]=
{
	{ CTU_KY_VOL_UP,
	  BIN_00010111,BIN_00000001, Sbm_key_volupon,
	  BIN_00010111,BIN_00000001, Sbm_key_voluplong,
	  BIN_00010111,BIN_00000001, Sbm_key_volupoff  },
	{ CTU_KY_VOL_DW,
	  BIN_00010111,BIN_00000001, Sbm_key_voldwon,
	  BIN_00010111,BIN_00000001, Sbm_key_voldwlong,
	  BIN_00010111,BIN_00000001, Sbm_key_voldwoff  },
	{ CTU_KY_PWR,
	  BIN_00010111,BIN_00001111, Key_long_time1000,
	  BIN_00010111,BIN_00001111, Tu_key_pw_long,
	  BIN_00010111,BIN_00001111, Tu_key_pw_off	},	
	{ CTU_KY_SRC,
	  BIN_00010111,BIN_00001111, Key_long_time2000,
	  BIN_00010111,BIN_00001111, Tu_key_md_long,
	  BIN_00010111,BIN_00001111, Ma_key_src	},	
	{ CTU_KY_FREQ_UP,
	  BIN_00010111,BIN_00001010, Tu_key_isrup_on,
	  BIN_00000000,BIN_00000000, Key_nop,
	  BIN_00010111,BIN_00001010, Tu_key_isrup_off	},
	{ CTU_KY_FREQ_DW,
	  BIN_00010111,BIN_00001010, Tu_key_isrdw_on,
	  BIN_00000000,BIN_00000000, Key_nop,
	  BIN_00010111,BIN_00001010, Tu_key_isrdw_off	},
	{ CTU_KY_ALARM,
	  BIN_00010111,BIN_00001111, Tu_key_alm_on,
	  BIN_00010111,BIN_00001111, Tu_key_alm_long,
	  BIN_00010111,BIN_00001111, Tu_key_alm_off	},
	{ CTU_KY_TISC,
	  BIN_00010111,BIN_00001111, Tu_key_tisc_on,
	  BIN_00010111,BIN_00001111, Tu_key_ti_long,
	  BIN_00010111,BIN_00001111, Tu_key_ti_off		},
#if 0		/*testモード関連*/	
	{ CTU_KY_TEST,
	  BIN_00000111,BIN_00001110, Tes_key_test,
	  BIN_00000000,BIN_00000000, Key_nop,
	  BIN_00000000,BIN_00000000, Key_nop	},	  	  
#if 0
	{ CTU_KY_STMN,
	  BIN_00000111,BIN_00001110, Tu_key_stmo_on,
	  BIN_00000111,BIN_00001110, Tu_key_stmo_long,
	  BIN_00000000,BIN_00000000, Key_nop	},	  
#endif
#endif		/*testモード関連*/
	{ CKEY_ID_INVALID,						/* end */
	  0, 			0,			  0,
	  0, 			0,            0,
	  0, 			0,			  0				}
};
#if 0		/*testモード関連*/
/*-----------------------------------------------------------------
	Tuner S-Meter調整ﾓｰﾄﾞキーﾃｰﾌﾞﾙ

ｷｰ有効ﾋﾞｯﾄ 16ﾋﾞｯﾄ：
xx|xx|xx|xx|xx|MUTE|SOUND|ADJ|xx|xx|xx|xx|SCAN|APM|SEEK|PWRON
-------------------------------------------------------------------*/
const KEY_PROC_TBL tu_test_key_tbl[]=
{
	{ CTU_KY_SRC,
	  BIN_00000111,BIN_11111111, Key_long_time1000,
	  BIN_00000111,BIN_11111111, Pw_power_key,
	  BIN_00000111,BIN_11111111, Ma_key_src	},
#if 0
	{ CTU_KY_DISP,
	  BIN_00000101,BIN_00001111, Tu_key_test_disp_on,
	  BIN_00000100,BIN_00001111, Key_nop,
	  BIN_00000101,BIN_00001111, Tu_key_test_disp_off	},
#endif
	{ CTU_KY_FREQ_UP,
	  BIN_00000111,BIN_00001110, Tu_key_test_up,
	  BIN_00000111,BIN_00001110, Key_nop,
	  BIN_00000111,BIN_00001110, Key_nop	},
	{ CTU_KY_FREQ_DW,
	  BIN_00000111,BIN_00001110, Tu_key_test_dw,
	  BIN_00000111,BIN_00001110, Key_nop,
	  BIN_00000111,BIN_00001110, Key_nop	},
#if 0
	{ CTU_KY_PUSH,
	  BIN_00000001, BIN_00000001, Key_long_time2000,
	  BIN_00000001, BIN_00000001, Tu_key_test_ent,
	  BIN_00000001, BIN_00000001, Key_nop	},
#endif
	{ CKEY_ID_INVALID,						/* end */
	  0, 			0,			  0,
	  0, 			0,            0,
	  0, 			0,			  0				}
};
#endif		/*testモード関連*/
#else
extern	const KEY_CHG_TBL tu_key_chg_tbl[];
extern	const KEY_PROC_TBL tu_normal_key_tbl[];
extern	const KEY_PROC_TBL tu_isr_key_tbl[];
extern	const KEY_PROC_TBL tu_test_key_tbl[];
#endif

#endif
