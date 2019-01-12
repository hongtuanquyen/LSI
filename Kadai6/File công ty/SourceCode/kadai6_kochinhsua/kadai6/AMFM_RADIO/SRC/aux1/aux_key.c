/******************************************************************************
	�N�����I���a����
	�v���W�F�N�g��	�F2012�NAMFM_RADIO���f��
	�t�@�C����		: aux_key.c
	���W���[����	: AUX
	�@	�\			: AUX�L�[��`
------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
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
  	�萔��`
----------------------------------------------------*/
/* AUX�� �L���L�[��` */
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

/* AUX�� �L��BIT��` */
#define	AUX_KEYEN_NOP	0x00		/* �Y����ԂȂ� */

static void Aux_dispkey_off( void );

/* AUX�� key�����e�[�u�� */
const KEY_PROC_TBL aux_key_tbl[]=
{
	{ AUX_KEYCD_PWR,  						/* PWR�L�[	*/
	  BIN_11111111, BIN_11111111, Key_long_time1000,									/* keyON����	*/
	  BIN_11111111, BIN_11111111, Aux_key_pwr_long,										/* keyLONG����	*/
	  BIN_11111111, BIN_11111111, Pw_power_key		 						},			/* keyOFF����	*/

	{ AUX_KEYCD_SRC,  						/* MODE�L�[	*/
	  BIN_11111111, BIN_11111111, Key_long_time2000,									/* keyON����	*/
	  BIN_11111111, BIN_11111111, Aux_key_mode_long,										/* keyLONG����	*/
	  BIN_11111111, BIN_11111111, Aux_key_src_off	 							},		/* keyOFF����	*/
	  
	{ AUX_KEYCD_NOP,  						/* CH/AS/UP/DOWN�L�[	*/
	  BIN_11111111, BIN_11111111, Key_nop,												/* keyON����	*/
	  BIN_11111111, BIN_11111111, Key_nop,												/* keyLONG����	*/
	  BIN_11111111, BIN_11111111, Key_nop		 							},			/* keyOFF����	*/
	  
	{ AUX_KEYCD_VOLUP,  					/* VOLUP�L�[	*/
	  BIN_11111111, BIN_11111111, Sbm_key_volupon,									/* keyON����	*/
	  BIN_11111111, BIN_11111111, Sbm_key_voluplong,									/* keyLONG����	*/
	  BIN_11111111, BIN_11111111, Sbm_key_volupoff		 							},	/* keyOFF����	*/
	  
	{ AUX_KEYCD_VOLDW,  					/* VOLDOWN�L�[	*/
	  BIN_11111111, BIN_11111111, Sbm_key_voldwon,									/* keyON����	*/
	  BIN_11111111, BIN_11111111, Sbm_key_voldwlong,									/* keyLONG����	*/
	  BIN_11111111, BIN_11111111, Sbm_key_voldwoff		 							},	/* keyOFF����	*/
	  
	{ AUX_KEYCD_ALARM,  					/* ALARM�L�[	*/
	  BIN_11111111, BIN_11111111, Aux_key_alarm_on,									/* keyON����	*/
	  BIN_11111111, BIN_11111111, Aux_key_alarm_long,										/* keyLONG����	*/
	  BIN_11111111, BIN_11111111, Alm_onoff_set},										/* keyOFF����	*/

	{ AUX_KEYCD_TI_SCAN,  					/* TI/SCAN�L�[	*/
	  BIN_11111111, BIN_11111111, Aux_key_tisc_on,										/* keyON����	*/
	  BIN_11111111, BIN_11111111, Key_nop,												/* keyLONG����	*/
	  BIN_11111111, BIN_11111111, Key_nop		 							},			/* keyOFF����	*/

	{ CKEY_ID_INVALID,  					/* end	*/
	  0, 0, Key_nop,																	/* keyON����	*/
	  0, 0, Key_nop,																	/* keyLONG����	*/
	  0, 0, Key_nop		 													}			/* keyOFF����	*/
};

/* �L�[�R�[�h�ϊ��e�[�u�� */
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
	��   ��  ��	: Aux_ModeID_get
	��       ��	: void
	��   ��  �l	: �L�����
	�@ �\ �T ��	:
    ��       �l : �L�[����Ă΂�鏈��
-------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
********************************************************************************/
BYTE Aux_ModeID_get(void)
{
	return AUX_KEYEN_NOP ;
}

/******************************************************************************
	��   ��  ��	: Aux_key_func_addr_get
	��       ��	: void
	��   ��  �l	: (KEY_PROC_TBL) �e�[�u���|�C���^
	�@ �\ �T ��	: PWR OFF�� �L�[�����e�[�u���|�C���^�擾
    ��       �l : �L�[����Ă΂�鏈��
-------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
********************************************************************************/
KEY_PROC_TBL * Aux_key_func_addr_get(void)
{
	return (KEY_PROC_TBL*)&aux_key_tbl ;
}

/******************************************************************************
	��   ��  ��	: Aux_keyCodeTable_addr_get
	��       ��	: void
	��   ��  �l	: (KEY_CHG_TBL) �e�[�u���|�C���^
	�@ �\ �T ��	: PWR OFF�� �L�[�ϊ��e�[�u���|�C���^�擾
    ��       �l : �L�[����Ă΂�鏈��
-------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
********************************************************************************/
KEY_CHG_TBL * Aux_keyCodeTable_addr_get(void)
{
	return (KEY_CHG_TBL*)&aux_key_chg_tbl ;
}

/******************************************************************************
	��   ��  ��	: Aux_key_tisc_on
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: SCAN/TI�L�[ON����
    ��       �l :
-------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
********************************************************************************/
static void	Aux_key_tisc_on(void)
{
	BYTE	destno;
	
	destno = Ma_model_dest();/*�d����No�擾*/
	
	switch(destno)
	{
		case CMA_MODEL_AMFM_J:
			Tu_key_isr_on();/*TI���[�h�֑J��*/
			break;
		default:
			Key_nop();/*����*/
			break;
	}
}

/******************************************************************************
	��   ��  ��	: Aux_key_src_off
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: SRC�L�[OFF����
    ��       �l :
-------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
********************************************************************************/
static void	Aux_key_src_off(void)
{
	Tu_amband_set();
	Ma_key_src();
}

/******************************************************************************
	��   ��  ��	: Aux_key_xxx_long
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: XXX�L�[LONG����
    ��       �l :
-------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
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