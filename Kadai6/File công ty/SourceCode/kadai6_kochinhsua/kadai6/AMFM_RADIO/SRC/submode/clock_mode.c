/***************************************************************
	�N�����I���a���� 
	�v���W�F�N�g��	�F2012�NAMFM_RADIO����
	�t�@�C����		�Fclock_mode.c
	���W���[����	�FSUB���[�h(���v)
	�@   �\			�F���v���ď����A���v��������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		�V�K
****************************************************************/
#define	_CLOCK_MODE_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#include	"../disp/lcd_ext.h"
#include	"../key/key_func_ext.h"
#include	"../main/ma_ext.h"
#include	"../tuner/tu_key.h"
#include	"../audio/Aud_inc.h"

#include	"sbm_def.h"
#include	"sbm_ext.h"
#include	"sbm_sys.h"
#include	"sbm_keytbl.h"

static void Clk_io_data_set(void);
static void Clk_Acc_Sta_Monitor(void);
static BYTE acc_sta;
static BYTE acc_sta_bak;
static BYTE pwr_sta;
static BYTE pwr_sta_bak;

/******************************************************************************
		�֐���  �FClk_lcddata_set
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v�\���ް��쐬����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		�V�K
******************************************************************************/
void	Clk_lcddata_set( TDP_CLK *clkdisp )
{
	if (Alm_mode_check() != OFF)
	{
		/* alm �̕\���f�[�^��n�� */
		Alm_lcddata_set(clkdisp);
	}
	else
	{
		if (sbm_clk_adj.hour == 0)
		{
			clkdisp->hour = 12;
		}
		else if ((sbm_clk_adj.hour>=13)&&(sbm_clk_adj.hour<=23))
		{
			clkdisp->hour = sbm_clk_adj.hour - 12;
		}
		else
		{
			clkdisp->hour = sbm_clk_adj.hour;
		}
		clkdisp->min = sbm_clk_adj.min;
		
		clkdisp->mode = sbm_clk_mode;
	}
}

BYTE Clk_indicator_set(void)
{
	BYTE	indicator;
	
	indicator = 0;
	
	#if 0
	if (Alm_mode_check() != OFF)
	{
		indicator = Alm_indicator_set();
	}
	else
	#endif
	{
		if ((sbm_clk_adj.hour >= 12)&&(sbm_clk_adj.hour <= 23))
		{
			indicator |= CLCD_CL_IND_PM;
		}
		else
		{
			indicator |= CLCD_CL_IND_AM;
		}
	}
	
	#if 0
	if( fsbm_alm_onoff == ON )
	{
		indicator |= CLCD_CL_IND_AL;
	}
	#endif
	
	return(indicator);
}
/******************************************************************************
		�֐���  �FClk_time_main
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�F���v��ώ擾����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		�V�K
******************************************************************************/
void	Clk_time_main(void)
{
	WORD io_min;
	
	if (Ma_get_clock() == FALSE )
	{
		return;
	}
	
	//Pw_acc_wakeup();
	io_min = Io_mincnt_read();
	
	/* IO������ς��擾 */
	sbm_clk_adj.hour = (BYTE)(io_min/60);
	sbm_clk_adj.min = (BYTE)(io_min%60);
	
	if (( sbm_clk_bak.min != sbm_clk_adj.min )
	  ||( sbm_clk_bak.hour != sbm_clk_adj.hour ))
	{
		sbm_clk_bak = sbm_clk_adj;
		if (Pw_ACC_status_get() == OFF)
		{
			/* Lcd_send_req(); */
			Lcd_accoff_send_req();
		}
	}
	
	/* ALARM�`�F�b�N���� */
	#if 0
	if  (( fsbm_alm_onoff == ON )
		&&( fsbm_beep_en == ON )
		&&( Alm_mode_check() == OFF )
		&&( sbm_clk_adj.min == sbm_alm_adj.min )
		&&( sbm_clk_adj.hour == sbm_alm_adj.hour ))
	{
		/* BEEP���� */
		if (fsbm_alm_beepon == OFF)/* ALARM�N����Ԑݒ� */
		{
			fsbm_alm_beepon = ON;
			fsbm_beep_en = OFF;
			Aud_set_beep(CAUD_REQ_BEEP_ALARM);
			/* 180S�^�C���ݒ� */
			sbm_beepon_tim = CSBM_BEEPON_180S;
			//if (Pw_ACC_status_get() == OFF)
			{
				Pw_acc_wakeup_req(ON);
			}
		}
	}
	#else
	
	if (Ma_get_alarm() == TRUE)
	{
		if  (( sbm_clk_adj.min == sbm_alm_adj.min )
			&&( sbm_clk_adj.hour == sbm_alm_adj.hour ))
		{
			if  (( fsbm_alm_onoff == ON )
				&&( Alm_mode_check() == OFF )
				&&( Clk_adj_check() == OFF )
				&&( fsbm_alm_beepon == OFF )
				&&( fsbm_beep_en == ON ))
			{
				fsbm_alm_beepon = ON;
				fsbm_beep_en = OFF;
				if (Pw_Power_status_get()  == ON)
				{
					Sbm_mode_cancel(CSBM_OFF);	
					Aud_cancel_beep();
					/* beep req */
					/* PowerON����Ȃ���΁APW���Ǘ����� */
					Aud_set_beep(CAUD_REQ_BEEP_ALARM);
				}
				else
				//if (Pw_ACC_DET_get() == OFF)
				{
					Pw_acc_wakeup_req(ON);
				}
			}
		}
		else
		{
			fsbm_beep_en = ON;
		}
	}
	#endif
	
	/*	acc��ԊĎ�����	*/
	Clk_Acc_Sta_Monitor();
	
	
}

void Clk_Acc_Sta_Monitor(void)
{
	acc_sta = Pw_ACC_DET_get();
	
	if (acc_sta_bak != acc_sta)
	{
		Alm_beep_key_stop();
		acc_sta_bak = acc_sta;
		Lcd_disp_base(Ma_current_mode());
	}
	
	pwr_sta = Pw_Power_status_get();
	
	if (pwr_sta_bak != pwr_sta)
	{
		if (fsbm_alm_off_req == TRUE)
		{
			Alm_mode_cancel();
			fsbm_alm_off_req = FALSE;
		}
		Clk_adj_cancel();
		pwr_sta_bak = pwr_sta;
	}
}
/******************************************************************************
		�֐���  �FClk_key_up_off
		��  ��	�Fvoid
		�߂�l	�F����
		�@  �\	�F���v��������������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/30		0.1		lrj		�V�K
******************************************************************************/
void Clk_mode_initial(void)
{
	if (Ma_get_clock() == FALSE )
	{
		return;
	}
	sbm_clk_adj.min = 0x00;
	sbm_clk_adj.hour = 0x01;
	Clk_io_data_set();
	//fsbm_clk_ampm = CSBM_CLK_AM;
	sbm_clk_tim = 0;
	sbm_clk_mode = CSBM_CLK_NORMAL;
	
	acc_sta = 0;
	acc_sta_bak = 0;
	pwr_sta = 0;
	pwr_sta_bak = 0;

}


void Clk_io_data_set(void)
{
	WORD io_min;
	
	io_min = (sbm_clk_adj.hour * 60) + sbm_clk_adj.min;
	if (io_min <= 0x59F)	/* 23�F59�܂� */
	{
		Io_mincnt_write(io_min);
	}
}
/******************************************************************************
		�֐���  �FClk_adj_start
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v����Ӱ�ގn�܂鏈��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		�V�K
******************************************************************************/
void	Clk_adj_start(void)
{
	if (Ma_get_clock() == FALSE )
	{
		Key_nop();
		return;
	}
	
	Vol_mode_cancel();
	Lcd_base_disp_cancel();
	sbm_clk_mode = CSBM_CLK_ADJUST;				/* ���̒���Ӱ�ނ־�� */
	sbm_clk_tim = CSBM_CLK_15S;
	Lcd_blktim_set();
	Key_nop();
}
/******************************************************************************
		�֐���  �FClk_adj_check
		��  ��	�F����
		�߂�l	�FON �ˎ��v������
				�FOFF �� ���v�������ł͂Ȃ�
		�@  �\	�F���v����Ӱ�ޒ����`�F�b�N����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		�V�K
******************************************************************************/
BYTE	Clk_adj_check(void)
{
	if (sbm_clk_mode != CSBM_CLK_NORMAL)
	{
		return(ON);
	}
	else
	{
		return(OFF);
	}
}
/******************************************************************************
		�֐���  �FClk_adj_cancel
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v����Ӱ�ޒ���ݾُ���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void	Clk_adj_cancel(void)
{
	sbm_clk_mode = CSBM_CLK_NORMAL;			/* ����Ӱ�޷�ݾ� */
	sbm_clk_tim = 0;
	fsbm_beep_en = OFF;
	#if 0	/* hour��min�𒲐�����x�ɁA�����ݒ肷�� */
	Io_seccnt_clear();
	Clk_io_data_set();
	#endif
}

/******************************************************************************
		�֐���  �FClk_adj_hour_up
		��  ��	�FUP/DW
		�߂�l	�F����
		�@  �\	�F���vӰ�ޒ�HOUR��UP����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_adj_hour_up(void)
{
	/* ���J�E���g */
	if( sbm_clk_adj.hour >= 23 )
	{
		sbm_clk_adj.hour = 0;
	}
	else
	{
		sbm_clk_adj.hour++;
	}
	#if 0	/* Clk_io_data_set�����ŕb�𓯎��ɃN���A���� */
	Io_seccnt_clear();
	#endif
	Clk_io_data_set();
	sbm_clk_tim = CSBM_CLK_15S;
	Lcd_blktim_set();
}
/******************************************************************************
		�֐���  �FClk_adj_min_up
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���vӰ�ޒ�MIN��UP����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_adj_min_up(void)
{
	sbm_clk_adj.min++;
	if( sbm_clk_adj.min == 60 )
	{
		sbm_clk_adj.min = 0;
	}
	#if 0	/* Clk_io_data_set�����ŕb�𓯎��ɃN���A���� */
	Io_seccnt_clear();
	#endif
	Clk_io_data_set();
	sbm_clk_tim = CSBM_CLK_15S;
	Lcd_blktim_set();
}
/******************************************************************************
		�֐���  �FClk_adj_tim_rst
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���vӰ�ޒ��������X�g����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_adj_tim_rst(void)
{
	#if 0	/* �u�������Z�b�g�@�\�v���A�u�b���Z�b�g�@�\�v�ɂȂ��� */
	if( sbm_clk_adj.min >= 30 )
	{
		Clk_adj_hour_up();
	}
	sbm_clk_adj.min = 0;
	
	Clk_io_data_set();
	sbm_clk_tim = CSBM_CLK_15S;
	#else
	
	#if 0	/* �b���Z�b�g��MIN���W�X�^������ */
	Io_seccnt_clear();
	#else
	Clk_io_data_set();
	#endif
	Lcd_blktim_set();
	sbm_clk_tim = CSBM_CLK_15S;
	#endif
}
/******************************************************************************
		�֐���  �FClk_adj_set
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v�����ς݃f�[�^�m��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void	Clk_adj_set(void)
{
	Clk_adj_cancel();
	Key_nop();
}
/******************************************************************************
		�֐���  �FClk_key_up_off
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v�������[�hUP��OFF����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_key_up_off(void)
{
	if(Ma_model_dest() == CMA_MODEL_AM_J)
	{
		Clk_adj_hour_up();
	}
	else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
	{
		Clk_adj_min_up();
	}
	else
	{
		/* No Operation */
	}
	Key_nop();
}
/******************************************************************************
		�֐���  �FClk_key_up_long
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v�������[�hUP��LONG����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_key_up_long(void)
{
	if(Ma_model_dest() == CMA_MODEL_AM_J)
	{
		Clk_adj_hour_up();
		Key_long_time200();
	}
	else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
	{
		Clk_adj_min_up();
		Key_long_time200();
	}
	else
	{
		/* No Operation */
	}
}
/******************************************************************************
		�֐���  �FClk_key_dw_off
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v�������[�hDOWN��OFF����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_key_dw_off(void)
{
	if(Ma_model_dest() == CMA_MODEL_AM_J)
	{
		Clk_adj_min_up();
	}
	else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
	{
		Clk_adj_hour_up();
	}
	else
	{
		/* No Operation */
	}
	Key_nop();
}
/******************************************************************************
		�֐���  �FClk_key_dw_long
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v�������[�hDOWN��LONG����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_key_dw_long(void)
{
	if(Ma_model_dest() == CMA_MODEL_AM_J)
	{
		Clk_adj_min_up();
		Key_long_time200();
	}
	else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
	{
		Clk_adj_hour_up();
		Key_long_time200();
	}
	else
	{
		/* No Operation */
	}
}
/******************************************************************************
		�֐���  �FClk_key_as_off
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v�������[�h��AS��OFF����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_key_as_off(void)
{
	Clk_adj_tim_rst();
	Clk_adj_cancel();
	Key_nop();
}

/******************************************************************************
		�֐���  �FClk_key_as_off
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�F���v�������[�h��AS��OFF����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		�V�K
******************************************************************************/
void Clk_key_md_off(void)
{
	BYTE mo;
	
	mo = Ma_current_mode();
	if (mo == CMA_AUX)
	{
		Tu_amband_set();
		Ma_key_src();
	}
	else
	{
		Tu_key_md_off();
	}
}

void Clk_key_pwr_long(void)
{
	Clk_adj_set();
	Key_nop();
}

void Clk_key_voluplong(void)
{
	Clk_adj_cancel();
	Sbm_key_voluplong();
}
void Clk_key_volupoff(void)
{
	Clk_adj_cancel();
	Sbm_key_volupoff();
}
void Clk_key_voldwlong(void)
{
	Clk_adj_cancel();
	Sbm_key_voldwlong();
}
void Clk_key_voldwoff(void)
{
	Clk_adj_cancel();
	Sbm_key_voldwoff();
}

#if _TEST_ALARM_ON_
void Clk_alarm_test(void)
{
	fsbm_alm_beepon = ON;
	fsbm_beep_en = OFF;
	if (Pw_Power_status_get()  == ON)
	{
		Aud_cancel_beep();
		/* beep req */
		/* PowerON����Ȃ���΁APW���Ǘ����� */
		Aud_set_beep(CAUD_REQ_BEEP_ALARM);
	}
	else
	{
		Pw_acc_wakeup_req(ON);
	}
}
#endif

#undef _CLOCK_MODE_C_
