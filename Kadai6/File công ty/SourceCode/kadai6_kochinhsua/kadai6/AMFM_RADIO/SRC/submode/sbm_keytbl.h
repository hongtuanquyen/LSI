/*************************************************************************
	クラリオン殿向け 
	プロジェクト名	：2012年AMFM_RADIOﾓﾃﾞﾙ
	ファイル名		：sbm_keytbl.h
	モジュール名	：SUBMODE
	機   能			：SUBMODEｷｰﾃｰﾌﾞﾙ
--------------------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
--------------------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
*************************************************************************/
#ifndef		_SBM_KEYTBL_H_
#define		_SBM_KEYTBL_H_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../key/key_def.h"
#include	"../power/pw_func_ext.h"
#include	"../tuner/tu_ext.h"
#include	"../tuner/tu_key.h"

/*----------------------------------------------------
		KEYﾃｰﾌﾞﾙ
----------------------------------------------------*/
#ifdef	_SBM_EXT_C_

/*----------------↓ AUDIO調整ﾓｰﾄﾞｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ ↓----------------*/
enum
{
	CSND_KY_PWR = 1,
	CSND_KY_SRC,
	CSND_KY_CH,
	CSND_KY_RIGHT,
	CSND_KY_LEFT,
	CSND_KY_VOLUP,
	CSND_KY_VOLDW,
	CSND_KY_ALARM,
	CSND_KY_TISC,
	CSND_KY_MAX
};
const KEY_CHG_TBL sef_key_chg_tbl[] =
	{
		{CKEY_ID_LCD_PWR,		CSND_KY_PWR		},
		{CKEY_ID_LCD_SRC,		CSND_KY_SRC		},
		{CKEY_ID_LCD_CH,		CSND_KY_CH		},
		{CKEY_ID_LCD_RIGHT,		CSND_KY_RIGHT	},
		{CKEY_ID_LCD_LEFT,		CSND_KY_LEFT	},
		{CKEY_ID_LCD_AS,		CKEY_ID_INVALID	},
		{CKEY_ID_LCD_VOLUP,		CSND_KY_VOLUP	},
		{CKEY_ID_LCD_VOLDW,		CSND_KY_VOLDW	},
		{CKEY_ID_LCD_ALARM,		CSND_KY_ALARM	},
		{CKEY_ID_LCD_TI_SCAN,	CSND_KY_TISC	},
		{CKEY_ID_INVALID,		CKEY_ID_INVALID	}
	};                          
                                
                                
/*----------------AUDIO調整ﾓｰﾄﾞKEY処理ﾃｰﾌﾞﾙ-------------------*/
const KEY_PROC_TBL sef_key_tbl[]=
{
	{ CSND_KY_PWR,										/* PWRｷｰ */
	  BIN_00011111, BIN_11111111, Key_long_time1000,
	  BIN_00011111, BIN_11111111, Sef_key_pwr_long,
	  BIN_00011111, BIN_11111111, Pw_power_key  },
	{ CSND_KY_SRC,										/* SRCｷｰ */
	  BIN_00011111, BIN_11111111, Key_long_time2000,
	  BIN_00011111, BIN_11111111, Key_nop,
	  BIN_00011111, BIN_11111111, Sef_key_md_off  },
	{ CSND_KY_CH,										/* CHｷｰ */
	  BIN_00011111, BIN_11111111, Sef_mode_chg,
	  BIN_00011111, BIN_11111111, Key_nop,
	  BIN_00011111, BIN_11111111, Key_nop  },
	{ CSND_KY_RIGHT,									/* RIGHTｷｰ */
	  BIN_00011111, BIN_11111111, Key_long_time500,
	  BIN_00011111, BIN_11111111, Sef_key_up_long,
	  BIN_00011111, BIN_11111111, Sef_key_up_off  },
	{ CSND_KY_LEFT,										/* LEFTｷｰ */
	  BIN_00011111, BIN_11111111, Key_long_time500,
	  BIN_00011111, BIN_11111111, Sef_key_dw_long,
	  BIN_00011111, BIN_11111111, Sef_key_dw_off  },
	{ CSND_KY_VOLUP,									/* VOLUPｷｰ */
	  BIN_00011111, BIN_11111111, Sbm_key_volupon,
	  BIN_00011111, BIN_11111111, Sbm_key_voluplong,
	  BIN_00011111, BIN_11111111, Sbm_key_volupoff  },
	{ CSND_KY_VOLDW,									/* VOLDWｷｰ */
	  BIN_00011111, BIN_11111111, Sbm_key_voldwon,
	  BIN_00011111, BIN_11111111, Sbm_key_voldwlong,
	  BIN_00011111, BIN_11111111, Sbm_key_voldwoff  },
	{ CSND_KY_ALARM,									/* ALARMｷｰ */
	  BIN_00011111, BIN_11111111, Sef_key_alm_on,
	  BIN_00011111, BIN_11111111, Sef_key_alm_long,
	  BIN_00011111, BIN_11111111, Alm_onoff_set  },
	{ CSND_KY_TISC,										/* TISCｷｰ */
	  BIN_00011111, BIN_11111111, Sbm_key_tisc_on,
	  BIN_00011111, BIN_11111111, Key_nop,
	  BIN_00011111, BIN_11111111, Key_nop  },
	{ CKEY_ID_INVALID,									/* end */
	  0, 			0,			0,
	  0, 			0,			0,
	  0, 			0,			0	}
};
/*----------------↑ AUDIO調整ﾓｰﾄﾞｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ ↑----------------*/


/*----------------↓ CLOCK調整ﾓｰﾄﾞｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ ↓----------------*/
enum
{
	CCLK_KY_PWR = 1,
	CCLK_KY_SRC,
	CCLK_KY_RIGHT,
	CCLK_KY_LEFT,
	CCLK_KY_AS,
	CCLK_KY_VOLUP,
	CCLK_KY_VOLDW,
	CCLK_KY_TISC,
	CCLK_KY_MAX
};
const KEY_CHG_TBL clk_key_chg_tbl[] =
	{
		{CKEY_ID_LCD_PWR,		CCLK_KY_PWR		},
		{CKEY_ID_LCD_SRC,		CCLK_KY_SRC		},
		{CKEY_ID_LCD_CH,		CKEY_ID_INVALID	},
		{CKEY_ID_LCD_RIGHT,		CCLK_KY_RIGHT	},
		{CKEY_ID_LCD_LEFT,		CCLK_KY_LEFT	},
		{CKEY_ID_LCD_AS,		CCLK_KY_AS		},
#if 0
		{CKEY_ID_LCD_VOLUP,		CCLK_KY_VOLUP	},
		{CKEY_ID_LCD_VOLDW,		CCLK_KY_VOLDW	},
#else
		{CKEY_ID_LCD_VOLUP,		CKEY_ID_INVALID	},
		{CKEY_ID_LCD_VOLDW,		CKEY_ID_INVALID	},
#endif
		{CKEY_ID_LCD_ALARM,		CKEY_ID_INVALID	},
		{CKEY_ID_LCD_TI_SCAN,	CCLK_KY_TISC	},
		{CKEY_ID_INVALID,		CKEY_ID_INVALID	}
	};                          
                                
                                
/*----------------CLOCK調整ﾓｰﾄﾞKEY処理ﾃｰﾌﾞﾙ-------------------*/
const KEY_PROC_TBL clk_key_tbl[]=
{
	{ CCLK_KY_PWR,										/* PWRｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time1000,
	  BIN_00111111, BIN_11111111, Clk_key_pwr_long,
	  BIN_00111111, BIN_11111111, Pw_power_key  },
	{ CCLK_KY_SRC,										/* SRCｷｰ */
	  BIN_00011111, BIN_11111111, Key_long_time2000,
	  BIN_00011111, BIN_11111111, Key_nop,
	  BIN_00011111, BIN_11111111, Clk_key_md_off  },
	{ CCLK_KY_RIGHT,									/* RIGHTｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time500,
	  BIN_00111111, BIN_11111111, Clk_key_up_long,
	  BIN_00111111, BIN_11111111, Clk_key_up_off  },
	{ CCLK_KY_LEFT,										/* LEFTｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time500,
	  BIN_00111111, BIN_11111111, Clk_key_dw_long,
	  BIN_00111111, BIN_11111111, Clk_key_dw_off  },
	{ CCLK_KY_AS,										/* ASｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time2000,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Clk_key_as_off  },
#if 0
	{ CCLK_KY_VOLUP,									/* VOLUPｷｰ */
	  BIN_00011111, BIN_11111111, Sbm_key_volupon,
	  BIN_00011111, BIN_11111111, Clk_key_voluplong,
	  BIN_00011111, BIN_11111111, Clk_key_volupoff  },
	{ CCLK_KY_VOLDW,									/* VOLDWｷｰ */
	  BIN_00011111, BIN_11111111, Sbm_key_voldwon,
	  BIN_00011111, BIN_11111111, Clk_key_voldwlong,
	  BIN_00011111, BIN_11111111, Clk_key_voldwoff  },
#endif
	{ CCLK_KY_TISC,										/* TISCｷｰ */
	  BIN_00011111, BIN_11111111, Sbm_key_tisc_on,
	  BIN_00011111, BIN_11111111, Key_nop,
	  BIN_00011111, BIN_11111111, Key_nop  },
	{ CKEY_ID_INVALID,									/* end */
	  0, 			0,			0,
	  0, 			0,			0,
	  0, 			0,			0	}
};
/*----------------↑ CLOCK調整ﾓｰﾄﾞｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ ↑----------------*/


/*----------------↓ ALARM調整ﾓｰﾄﾞｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ ↓----------------*/
enum
{
	CALM_KY_PWR = 1,
	CALM_KY_SRC,
	CALM_KY_RIGHT,
	CALM_KY_LEFT,
	CALM_KY_VOLUP,
	CALM_KY_VOLDW,
	CALM_KY_ALARM,
	CALM_KY_TISC,
	CALM_KY_MAX
};
const KEY_CHG_TBL alm_key_chg_tbl[] =
	{
		{CKEY_ID_LCD_PWR,		CALM_KY_PWR		},
		{CKEY_ID_LCD_SRC,		CALM_KY_SRC		},
		{CKEY_ID_LCD_CH,		CKEY_ID_INVALID	},
		{CKEY_ID_LCD_RIGHT,		CALM_KY_RIGHT	},
		{CKEY_ID_LCD_LEFT,		CALM_KY_LEFT	},
		{CKEY_ID_LCD_AS,		CKEY_ID_INVALID	},
#if 0
		{CKEY_ID_LCD_VOLUP,		CALM_KY_VOLUP	},
		{CKEY_ID_LCD_VOLDW,		CALM_KY_VOLDW	},
#else
		{CKEY_ID_LCD_VOLUP,		CKEY_ID_INVALID	},
		{CKEY_ID_LCD_VOLDW,		CKEY_ID_INVALID	},
#endif
		{CKEY_ID_LCD_ALARM,		CALM_KY_ALARM	},
		{CKEY_ID_LCD_TI_SCAN,	CALM_KY_TISC	},
		{CKEY_ID_LCD_TEST1,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST2,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST3,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST1_P,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST2_P,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST3_P,	CKEY_ID_INVALID		},
		{CKEY_ID_INVALID,		CKEY_ID_INVALID	}
	};

/*----------------ALARM調整ﾓｰﾄﾞKEY処理ﾃｰﾌﾞﾙ-------------------*/
const KEY_PROC_TBL alm_key_tbl[]=
{
	{ CALM_KY_PWR,										/* PWRｷｰ */
	  BIN_00111111, BIN_11111111, Alm_key_pwr_on,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Key_nop  },
	{ CALM_KY_SRC,										/* SRCｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time2000,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Alm_mode_set  },
	{ CALM_KY_RIGHT,									/* RIGHTｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time500,
	  BIN_00111111, BIN_11111111, Alm_key_up_long,
	  BIN_00111111, BIN_11111111, Alm_key_up_off  },
	{ CALM_KY_LEFT,										/* LEFTｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time500,
	  BIN_00111111, BIN_11111111, Alm_key_dw_long,
	  BIN_00111111, BIN_11111111, Alm_key_dw_off  },
#if 0
	{ CALM_KY_VOLUP,									/* VOLUPｷｰ */
	  BIN_00011111, BIN_11111111, Alm_key_volupon,
	  BIN_00011111, BIN_11111111, Alm_key_voluplong,
	  BIN_00011111, BIN_11111111, Alm_key_volupoff  },
	{ CALM_KY_VOLDW,									/* VOLDWｷｰ */
	  BIN_00011111, BIN_11111111, Alm_key_voldwon,
	  BIN_00011111, BIN_11111111, Alm_key_voldwlong,
	  BIN_00011111, BIN_11111111, Alm_key_voldwoff  },
#endif
	{ CALM_KY_ALARM,									/* ALARMｷｰ */
	  BIN_00111111, BIN_11111111, Alm_mode_set,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Key_nop  },
	{ CALM_KY_TISC,										/* TISCｷｰ */
	  BIN_00111111, BIN_11111111, Alm_key_tisc_on,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Key_nop  },
	{ CKEY_ID_INVALID,									/* end */
	  0, 			0,			0,
	  0, 			0,			0,
	  0, 			0,			0	}
};

const KEY_CHG_TBL alm_key_chg_accoff_tbl[] =
	{
		{CKEY_ID_LCD_ALARM,		CALM_KY_ALARM		},
		{CKEY_ID_LCD_RIGHT,		CALM_KY_RIGHT		},
		{CKEY_ID_LCD_LEFT,		CALM_KY_LEFT		},
		{CKEY_ID_LCD_PWR,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_SRC,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_CH,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_AS,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_VOLUP,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_VOLDW,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TI_SCAN,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST1,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST2,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST3,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST1_P,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST2_P,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST3_P,	CKEY_ID_INVALID		},
		{CKEY_ID_INVALID,		CKEY_ID_INVALID		}
	};                          
const KEY_PROC_TBL alm_key_accoff_tbl[]=
{
	{ CALM_KY_RIGHT,									/* RIGHTｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time500,
	  BIN_00111111, BIN_11111111, Alm_key_up_long,
	  BIN_00111111, BIN_11111111, Alm_key_up_off  },
	{ CALM_KY_LEFT,										/* LEFTｷｰ */
	  BIN_00111111, BIN_11111111, Key_long_time500,
	  BIN_00111111, BIN_11111111, Alm_key_dw_long,
	  BIN_00111111, BIN_11111111, Alm_key_dw_off  },
	{ CALM_KY_ALARM,									/* ALARMｷｰ */
	  BIN_00111111, BIN_11111111, Alm_mode_set,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Key_nop  },
	{ CKEY_ID_INVALID,									/* end */
	  0, 			0,			0,
	  0, 			0,			0,
	  0, 			0,			0	}
};

/*----------------↑ ALARM調整ﾓｰﾄﾞｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ ↑----------------*/


/*----------------↓ ALARM 起動中ｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ ↓----------------*/
enum
{
	CBEEP_KY_PWR = 1,
	CBEEP_KY_BEEPON,
	CBEEP_KY_MAX
};
const KEY_CHG_TBL beep_key_chg_tbl[] =
	{
		{CKEY_ID_LCD_PWR,		CBEEP_KY_PWR		},
		{CKEY_ID_LCD_SRC,		CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_CH,		CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_RIGHT,		CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_LEFT,		CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_AS,		CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_VOLUP,		CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_VOLDW,		CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_ALARM,		CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_TI_SCAN,	CBEEP_KY_BEEPON		},
		{CKEY_ID_LCD_TEST1,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST2,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST3,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST1_P,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST2_P,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST3_P,	CKEY_ID_INVALID		},
		{CKEY_ID_INVALID,		CKEY_ID_INVALID		}
	};                          
                                
                                
/*----------------ALARM 起動中KEY処理ﾃｰﾌﾞﾙ-------------------*/
const KEY_PROC_TBL beep_key_tbl[]=
{
	{ CBEEP_KY_PWR,										/* PWRｷｰ */
	  BIN_00111111, BIN_11111111, Sbm_BeepOn_Power,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Key_nop  },
	{ CBEEP_KY_BEEPON,									/* PWR以外ｷｰ */
	  BIN_00111111, BIN_11111111, Alm_beep_key_stop,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Alm_beep_key_stop  },
	{ CKEY_ID_INVALID,									/* end */
	  0, 			0,			0,
	  0, 			0,			0,
	  0, 			0,			0	}
};
/*----------------↑ ALARM 起動中ｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ ↑----------------*/


#if _TEST_MODE_
/*---------------TESTモードｷｰｺｰﾄﾞ変換ﾃｰﾌﾞﾙ---------------*/
enum
{
	CSCT_KY_PWR = 1,
	CSCT_KY_VER_DISPOFF,
	CSCT_KY_MAX
};
const KEY_CHG_TBL Sct_key_chg_nor_tbl[] =
	{
		{CKEY_ID_LCD_PWR,		CSCT_KY_PWR			},
		{CKEY_ID_LCD_SRC,		CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_CH,		CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_RIGHT,		CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_LEFT,		CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_AS,		CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_VOLUP,		CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_VOLDW,		CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_ALARM,		CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_TI_SCAN,	CSCT_KY_VER_DISPOFF	},
		{CKEY_ID_LCD_TEST1,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST2,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST3,		CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST1_P,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST2_P,	CKEY_ID_INVALID		},
		{CKEY_ID_LCD_TEST3_P,	CKEY_ID_INVALID		},
		{CKEY_ID_INVALID,		CKEY_ID_INVALID		}
	};
const KEY_PROC_TBL Sct_key_nor_tbl[]=
{
	{ CSCT_KY_PWR,
	  BIN_00111111, BIN_11111111, Sct_key_pwr_on,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Key_nop  },
	{ CSCT_KY_VER_DISPOFF,
	  BIN_00111111, BIN_11111111, Sct_key_Ver_dispoff,
	  BIN_00111111, BIN_11111111, Key_nop,
	  BIN_00111111, BIN_11111111, Key_nop  },
	{ CKEY_ID_INVALID,									/* end */
	  0, 			0,			0,
	  0, 			0,			0,
	  0, 			0,			0	}
};
#endif


#endif		/* _SBM_EXT_C_ */

#endif
