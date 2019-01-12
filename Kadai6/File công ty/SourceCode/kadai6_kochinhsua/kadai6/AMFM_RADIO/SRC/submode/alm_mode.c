/***************************************************************
	�N�����I���a���� 
	�v���W�F�N�g��	�F2012�NAMFM_RADIO����
	�t�@�C����		�Falm_mode.c
	���W���[����	�FSUB���[�h(ALARM)
	�@   �\			�FALARM��������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
****************************************************************/
#define	_ALM_MODE_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../disp/lcd_ext.h"
#include	"../key/key_func_ext.h"
#include	"../audio/aud_ext.h"
#include	"../main/ma_ext.h"
#include	"../power/pw_func_ext.h"

#include	"sbm_def.h"
#include	"sbm_ext.h"
#include	"sbm_sys.h"
#include	"sbm_keytbl.h"

/******************************************************************************
		�֐���  �FAlm_lcddata_set
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm�\���ް��쐬����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_lcddata_set( TDP_CLK *almdisp )
{
	if (sbm_alm_adj.hour == 0)
	{
		almdisp->hour = 12;
	}
	else if ((sbm_alm_adj.hour >= 13)&&(sbm_alm_adj.hour <= 23))
	{
		almdisp->hour = sbm_alm_adj.hour - 12;
	}
	else
	{
		almdisp->hour = sbm_alm_adj.hour;
	}
	almdisp->min = sbm_alm_adj.min;
	almdisp->mode = sbm_alm_mode;
}

BYTE Alm_indicator_set(void)
{
	BYTE	indicator;
	
	indicator = 0;
	
	if ((sbm_alm_adj.hour >= 12)&&(sbm_alm_adj.hour <= 23))
	{
		indicator |= CLCD_CL_IND_PM;
	}
	else
	{
		indicator |= CLCD_CL_IND_AM;
	}
	return (indicator);
}

BOOL Alm_bell_indicator_set(void)
{
	return (fsbm_alm_onoff);
}

/******************************************************************************
		�֐���  �FAlm_mode_start
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm�������[�h���鏈��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_mode_start(void)
{
	if (Ma_get_alarm() == FALSE )
	{
		Key_nop();
		return;
	}
	
	Lcd_base_disp_cancel();
	Vol_mode_cancel();
	sbm_alm_mode = CSBM_ALM_ADJUST;
	fsbm_alm_onoff = ON;
	sbm_alm_tim = CSBM_ALM_15S;
	Lcd_blktim_set();
	
	#if 0	/* AM1:00����Œ� */
	if (fsbm_alm_setted == FALSE)
	{
		#if 0
		sbm_alm_adj = sbm_clk_adj;
		#else
		sbm_alm_adj.min = sbm_clk_adj.min;
		sbm_alm_adj.hour = sbm_clk_adj.hour;		
		#endif
	}
	#endif
	
	Key_nop();
}
/******************************************************************************
		�֐���  �FAlm_mode_check
		��  ��	�F����
		�߂�l	�FON ��Alarm������
				�FOFF �� Alarm�������ł͂Ȃ�
		�@  �\	�FAlarm�������[�h�����`�F�b�N
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
BYTE Alm_mode_check(void)
{
	if(sbm_alm_mode != CSBM_ALM_NORMAL)
	{
		return(ON);
	}
	else
	{
		return(OFF);
	}
}
/******************************************************************************
		�֐���  �FAlm_mode_cancel
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm�������[�h����ݾُ���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_mode_cancel(void)
{
	sbm_alm_mode = CSBM_ALM_NORMAL;
	sbm_alm_tim = 0;
	//fsbm_alm_onoff = ON;
	fsbm_beep_en = OFF;
	#if 0
	fsbm_alm_setted = ON;
	Pw_almkey_clear();
	#endif
}
/******************************************************************************
		�֐���  �FAlm_mode_hour_up
		��  ��	�FUP/DW
		�߂�l	�F����
		�@  �\	�FAlarm����Ӱ�ޒ�HOUR��UP����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_mode_hour_up(void)
{
#if 0
	sbm_alm_adj.hour++;
	if( sbm_alm_adj.hour == 24 )
	{
		sbm_alm_adj.hour = 0;
	}
	if( sbm_alm_adj.hour < 12 )
	{
		fsbm_alm_ampm = CSBM_CLK_AM;
	}
	else
	{
		fsbm_alm_ampm = CSBM_CLK_PM;
	}
#else
	#if 0
	if( sbm_alm_adj.hour == 11 )
	{
		if( fsbm_alm_ampm == CSBM_CLK_AM )
		{
			sbm_alm_adj.hour++;
			fsbm_alm_ampm = CSBM_CLK_PM;
		}
		else
		{
			sbm_alm_adj.hour = 0;
			fsbm_alm_ampm = CSBM_CLK_AM;
		}
	}
	else if( sbm_alm_adj.hour == 12 )
	{
		sbm_alm_adj.hour = 1;
	}
	else
	{
		sbm_alm_adj.hour++;
	}
	#endif
	
	/* ���J�E���g */
	if( sbm_alm_adj.hour >= 23 )
	{
		sbm_alm_adj.hour = 0;
	}
	else
	{
		sbm_alm_adj.hour++;
	}
	
	#if 0
	/* AM��PM�̐ݒ� */
	if(( sbm_alm_adj.hour >= 12)&&(sbm_alm_adj.hour <= 23))
	{
		fsbm_clk_ampm = CSBM_CLK_PM;
	}
	else
	{
		fsbm_clk_ampm = CSBM_CLK_AM;
	}
	#endif
#endif
	
	sbm_alm_tim = CSBM_ALM_15S;
	Lcd_blktim_set();			/* �_����ύĐݒ� */
}
/******************************************************************************
		�֐���  �FAlm_mode_min_up
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm����Ӱ�ޒ�MIN��UP����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_mode_min_up(void)
{
	sbm_alm_adj.min++;
	if( sbm_alm_adj.min == 60 )
	{
		sbm_alm_adj.min = 0;
	}
	
	sbm_alm_tim = CSBM_ALM_15S;
	Lcd_blktim_set();			/* �_����ύĐݒ� */
}
/******************************************************************************
		�֐���  �FAlm_mode_set
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm�����ς݃f�[�^�m��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_mode_set(void)
{
	#if 0
	sbm_alm_mode = CSBM_ALM_NORMAL;
	sbm_alm_tim = 0;
	fsbm_alm_onoff = ON;
	fsbm_beep_en = ON;
	fsbm_alm_setted = ON;
	#else
	Alm_mode_cancel();
	Key_nop();
	#endif
}
/******************************************************************************
		�֐���  �FAlm_onoff_set
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm ONOFF��Ԑݒ�
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_onoff_set(void)
{
	if (Ma_get_alarm() == FALSE )
	{
		Key_nop();
		return;
	}
	
	fsbm_alm_onoff = ~fsbm_alm_onoff;
	fsbm_beep_en = OFF;
	Lcd_1shot_cancel(CLCD_1SHOT_OFF);			/* �ꎞ�\����ݾ� */
	/* Lcd_send_req(); */
	Lcd_accoff_send_req();
	Key_nop();
}
/******************************************************************************
		�֐���  �FAlm_beepon_get
		��  ��	�F����
		�߂�l	�FON/OFF
		�@  �\	�Fbeep�������ǂ����擾
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
BOOL Alm_beepon_get(void)
{
	return(fsbm_alm_beepon);
}
/******************************************************************************
		�֐���  �FAlm_beepon_get
		��  ��	�F����
		�߂�l	�FON/OFF
		�@  �\	�Fbeep�������ǂ����擾
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_key_pwr_on(void)
{
	/* �A���[���L�����Z���v�����o�� */
	fsbm_alm_off_req = TRUE;
	
	Pw_power_key();
	Key_nop();
}

/******************************************************************************
		�֐���  �FAlm_key_up_off
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm�������[�hUP��OFF����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_key_up_off(void)
{
	Alm_mode_min_up();
	Key_nop();
}
/******************************************************************************
		�֐���  �FAlm_key_up_long
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm�������[�hUP��LONG����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_key_up_long(void)
{
	Alm_mode_min_up();
	Key_long_time200();
}
/******************************************************************************
		�֐���  �FAlm_key_dw_off
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm�������[�hDOWN��OFF����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_key_dw_off(void)
{
	Alm_mode_hour_up();
	Key_nop();
}
/******************************************************************************
		�֐���  �FAlm_key_dw_long
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm�������[�hDOWN��LONG����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Alm_key_dw_long(void)
{
	Alm_mode_hour_up();
	Key_long_time200();
}

void Alm_key_tisc_on(void)
{
	BYTE	destno;
	
	destno = Ma_model_dest();/*�d����No�擾*/
	
	if ((destno == CMA_MODEL_AMFM_J)&&(Pw_Power_status_get() == ON))
	{
		Tu_key_isr_on();/*TI���[�h�֑J��*/
	}
	
	Key_nop();/*����*/
}

/******************************************************************************
		�֐���  �FAlm_beep_stop
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm beep on���Abeep����~����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/25		0.1		lrj		�V�K
******************************************************************************/
void Alm_beep_on_stop(void)
{
	fsbm_alm_beepon = OFF;
	Pw_acc_wakeup_req(OFF);
}

/******************************************************************************
		�֐���  �FAlm_beep_key_stop
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm beep on���APower�ȊO�̷�ON����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/25		0.1		lrj		�V�K
******************************************************************************/
void Alm_beep_key_stop(void)
{
	Sbm_mode_cancel(CSBM_OFF);
	Aud_cancel_beep();	/* beep off */
	Alm_beep_on_stop();
	Key_nop();
}
void Alm_key_volupon(void)
{
	if (Pw_Power_status_get() == ON)
	{
		Sbm_key_volupon();
	}
	else
	{
		Key_nop();
	}
}
void Alm_key_voluplong(void)
{
	Alm_mode_cancel();
	Sbm_key_voluplong();
}
void Alm_key_volupoff(void)
{
	Alm_mode_cancel();
	Sbm_key_volupoff();
}
void Alm_key_voldwon(void)
{
	if (Pw_Power_status_get() == ON)
	{
		Sbm_key_voldwon();
	}
	else
	{
		Key_nop();
	}
}
void Alm_key_voldwlong(void)
{
	Alm_mode_cancel();
	Sbm_key_voldwlong();
}
void Alm_key_voldwoff(void)
{
	Alm_mode_cancel();
	Sbm_key_voldwoff();
}

/******************************************************************************
		�֐���  �FAlm_time_initial
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FAlarm��������������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/30		0.1		lrj		�V�K
******************************************************************************/
void Alm_mode_initial(void)
{
	sbm_alm_adj.min = 0x00;
	sbm_alm_adj.hour = 0x01;
	sbm_alm_bak.min = 0x00;
	sbm_alm_bak.hour = 0x01;
	sbm_alm_tim = 0;
	sbm_alm_mode = CSBM_ALM_NORMAL;
}














#undef _ALM_MODE_C_
