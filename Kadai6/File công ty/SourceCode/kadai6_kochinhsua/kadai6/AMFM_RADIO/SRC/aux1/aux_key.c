/******************************************************************************
	クラリオン殿向け
	プロジェクト名	：2012年AMFM_RADIOモデル
	ファイル名		: aux_key.c
	モジュール名	: AUX
	機	能			: AUXキー定義
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
#define	_AUX_KEY_C_

#include		"../model.h"

#include 	<stdio.h>

#include	"../comm/common.h"
#include	"../submode/sbm_ext.h"
#include	"../submode/sbm_def.h"
#include 	"../key/key_func_ext.h"
#include 	"../power/pw_func_ext.h"
#include	"../audio/Aud_inc.h"
#include	"../main/ma_ext.h"
#include 	"../disp/lcd_ext.h"
#include	"../tuner/tu_ext.h"
#include	"aux_key.h"

static	void	Aux_key_tisc_on(void);
static	void	Aux_key_src_off(void);
static	void	Aux_key_pwr_long(void);
static	void	Aux_key_alarm_on(void);
static	void	Aux_key_alarm_long(void);
static	void	Aux_key_mode_long(void);

/*----------------------------------------------------
  	定数定義
----------------------------------------------------*/
/* AUX中 有効キー定義 */
enum
{
	AUX_KEYCD_PWR,
	AUX_KEYCD_SRC,
	AUX_KEYCD_NOP,
	AUX_KEYCD_VOLUP,
	AUX_KEYCD_VOLDW,
	AUX_KEYCD_ALARM,
	AUX_KEYCD_TI_SCAN,
	AUX_KEYCD_MAX
};

/* AUX中 有効BIT定義 */
#define	AUX_KEYEN_NOP	0x00		/* 該当状態なし */

static void Aux_dispkey_off( void );

/* AUX中 key処理テーブル */
const KEY_PROC_TBL aux_key_tbl[]=
{
	{ AUX_KEYCD_PWR,  						/* PWRキー	*/
	  BIN_11111111, BIN_11111111, Key_long_time1000,									/* keyON処理	*/
	  BIN_11111111, BIN_11111111, Aux_key_pwr_long,										/* keyLONG処理	*/
	  BIN_11111111, BIN_11111111, Pw_power_key		 						},			/* keyOFF処理	*/

	{ AUX_KEYCD_SRC,  						/* MODEキー	*/
	  BIN_11111111, BIN_11111111, Key_long_time2000,									/* keyON処理	*/
	  BIN_11111111, BIN_11111111, Aux_key_mode_long,										/* keyLONG処理	*/
	  BIN_11111111, BIN_11111111, Aux_key_src_off	 							},		/* keyOFF処理	*/
	  
	{ AUX_KEYCD_NOP,  						/* CH/AS/UP/DOWNキー	*/
	  BIN_11111111, BIN_11111111, Key_nop,												/* keyON処理	*/
	  BIN_11111111, BIN_11111111, Key_nop,												/* keyLONG処理	*/
	  BIN_11111111, BIN_11111111, Key_nop		 							},			/* keyOFF処理	*/
	  
	{ AUX_KEYCD_VOLUP,  					/* VOLUPキー	*/
	  BIN_11111111, BIN_11111111, Sbm_key_volupon,									/* keyON処理	*/
	  BIN_11111111, BIN_11111111, Sbm_key_voluplong,									/* keyLONG処理	*/
	  BIN_11111111, BIN_11111111, Sbm_key_volupoff		 							},	/* keyOFF処理	*/
	  
	{ AUX_KEYCD_VOLDW,  					/* VOLDOWNキー	*/
	  BIN_11111111, BIN_11111111, Sbm_key_voldwon,									/* keyON処理	*/
	  BIN_11111111, BIN_11111111, Sbm_key_voldwlong,									/* keyLONG処理	*/
	  BIN_11111111, BIN_11111111, Sbm_key_voldwoff		 							},	/* keyOFF処理	*/
	  
	{ AUX_KEYCD_ALARM,  					/* ALARMキー	*/
	  BIN_11111111, BIN_11111111, Aux_key_alarm_on,									/* keyON処理	*/
	  BIN_11111111, BIN_11111111, Aux_key_alarm_long,										/* keyLONG処理	*/
	  BIN_11111111, BIN_11111111, Alm_onoff_set},										/* keyOFF処理	*/

	{ AUX_KEYCD_TI_SCAN,  					/* TI/SCANキー	*/
	  BIN_11111111, BIN_11111111, Aux_key_tisc_on,										/* keyON処理	*/
	  BIN_11111111, BIN_11111111, Key_nop,												/* keyLONG処理	*/
	  BIN_11111111, BIN_11111111, Key_nop		 							},			/* keyOFF処理	*/

	{ CKEY_ID_INVALID,  					/* end	*/
	  0, 0, Key_nop,																	/* keyON処理	*/
	  0, 0, Key_nop,																	/* keyLONG処理	*/
	  0, 0, Key_nop		 													}			/* keyOFF処理	*/
};

/* キーコード変換テーブル */
const KEY_CHG_TBL aux_key_chg_tbl[] =
{
	{ CKEY_ID_LCD_PWR,		AUX_KEYCD_PWR		},
	{ CKEY_ID_LCD_SRC,		AUX_KEYCD_SRC		},
	{ CKEY_ID_LCD_CH,		AUX_KEYCD_NOP		},
	{ CKEY_ID_LCD_RIGHT,	AUX_KEYCD_NOP		},
	{ CKEY_ID_LCD_LEFT,		AUX_KEYCD_NOP		},
	{ CKEY_ID_LCD_AS,		AUX_KEYCD_NOP		},
	{ CKEY_ID_LCD_VOLUP,	AUX_KEYCD_VOLUP		},
	{ CKEY_ID_LCD_VOLDW,	AUX_KEYCD_VOLDW		},
	{ CKEY_ID_LCD_ALARM,	AUX_KEYCD_ALARM		},
	{ CKEY_ID_LCD_TI_SCAN,	AUX_KEYCD_TI_SCAN	},
	{ CKEY_ID_INVALID,		CKEY_ID_INVALID		}
};

/******************************************************************************
	関   数  名	: Aux_ModeID_get
	引       数	: void
	戻   り  値	: 有効状態
	機 能 概 略	:
    備       考 : キーから呼ばれる処理
-------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
********************************************************************************/
BYTE Aux_ModeID_get(void)
{
	return AUX_KEYEN_NOP ;
}

/******************************************************************************
	関   数  名	: Aux_key_func_addr_get
	引       数	: void
	戻   り  値	: (KEY_PROC_TBL) テーブルポインタ
	機 能 概 略	: PWR OFF中 キー処理テーブルポインタ取得
    備       考 : キーから呼ばれる処理
-------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
********************************************************************************/
KEY_PROC_TBL * Aux_key_func_addr_get(void)
{
	return (KEY_PROC_TBL*)&aux_key_tbl ;
}

/******************************************************************************
	関   数  名	: Aux_keyCodeTable_addr_get
	引       数	: void
	戻   り  値	: (KEY_CHG_TBL) テーブルポインタ
	機 能 概 略	: PWR OFF中 キー変換テーブルポインタ取得
    備       考 : キーから呼ばれる処理
-------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
********************************************************************************/
KEY_CHG_TBL * Aux_keyCodeTable_addr_get(void)
{
	return (KEY_CHG_TBL*)&aux_key_chg_tbl ;
}

/******************************************************************************
	関   数  名	: Aux_key_tisc_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: SCAN/TIキーON処理
    備       考 :
-------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
********************************************************************************/
static void	Aux_key_tisc_on(void)
{
	BYTE	destno;
	
	destno = Ma_model_dest();/*仕向けNo取得*/
	
	switch(destno)
	{
		case CMA_MODEL_AMFM_J:
			Tu_key_isr_on();/*TIモードへ遷移*/
			break;
		default:
			Key_nop();/*無効*/
			break;
	}
}

/******************************************************************************
	関   数  名	: Aux_key_src_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: SRCキーOFF処理
    備       考 :
-------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
********************************************************************************/
static void	Aux_key_src_off(void)
{
	Tu_amband_set();
	Ma_key_src();
}

/******************************************************************************
	関   数  名	: Aux_key_xxx_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: XXXキーLONG処理
    備       考 :
-------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
********************************************************************************/
static void	Aux_key_pwr_long(void)
{
	if (Ma_get_clock() == TRUE )
	{
		Clk_adj_start();
		Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	}
	Key_nop();
}
static void	Aux_key_alarm_on(void)
{
	if (Ma_get_alarm() == TRUE )
	{
		Key_long_time2000();
	}
	else
	{
		Key_nop();
	}
}
static void	Aux_key_alarm_long(void)
{
	Alm_mode_start();
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
}
static void	Aux_key_mode_long(void)
{
	Sef_mode_start();
	Lcd_base_disp_cancel();
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
}

#undef	_AUX_KEY_C_