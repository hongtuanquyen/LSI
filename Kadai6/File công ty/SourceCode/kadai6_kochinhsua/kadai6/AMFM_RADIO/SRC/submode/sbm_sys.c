/*************************************************************************
	�N�����I���a���� 
	�v���W�F�N�g��	�F2012�NAMFM_RADIO����
	�t�@�C����		�Fsbm_sys.c
	���W���[����	�FSUBMODE
	�@   �\			�FSub system���b�Z�[�W��t����
--------------------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
--------------------------------------------------------------------------
			2012/7/23	0.1			lrj		�V�K
*************************************************************************/
#define		_SBM_SYS_C_

#include	"../model.h"


#include    <string.h>
#include	"../comm/common.h"
#include	"../key/key_func_ext.h"
#include	"../key/key_def.h"
#include	"../main/ma_ext.h"
#include	"../off/off_key.h"
#include	"../disp/lcd_ext.h"
#include	"../tuner/tu_ext.h"

#include	"sbm_sys.h"
#include	"sbm_ext.h"
#include	"sbm_keytbl.h"
#include	"sbm_def.h"

/****************************************************************
		�֐����FSbm_sys_message
		��  ���FSystem���b�Z�[�W
		�߂�l�F����
		�@  �\�FSbmSystemү���ގ�t����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		�V�K
			2012/7/30		0.2		lrj		initial��modechg�̏����C��
****************************************************************/
void	Sbm_sys_message( BYTE msg )
{
	switch(msg)
	{
		case CMSG_INITIAL:
			Sbm_mode_initial();
			break;
		case CMSG_MAIN:
			Clk_time_main();
			break;
		case CMSG_MAINCNT_OFF:
			Sbm_mode_cancel(CSBM_OFF);
			break;
		case CMSG_MODECHG:
		case CMSG_PWROFF:
			Sbm_mode_cancel(CSBM_OFF);
			fsbm_alm_off_req = FALSE;
			break;
		case CMSG_PWRON:
			break;
		case CMSG_HOLDX_OUT:
			/* �\���v���`�F�b�N */
			break;
		case CMSG_100MS_TIMER:
			Sbm_100ms_cnt();
			break;
		default:
			break;
	}

}
/****************************************************************
		�֐����FSbm_msg_modechg
		��  ���F����
		�߂�l�F����
		�@  �\�Fmodechgү���ގ�t����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		�V�K
****************************************************************/
void Sbm_msg_modechg(void)
{
	if (Vol_mode_check() != OFF)
	{
		Vol_mode_cancel();			/* Vol�������[�h��ݾ� */
	}
	if (Sef_mode_check() != OFF)
	{
		Sef_mode_cancel();			/* Audio�������[�h��ݾ� */
	}
	if (Clk_adj_check() != OFF)
	{
		Clk_adj_cancel();			/* ���v�������[�h��ݾ� */
	}
	if (Alm_mode_check() != OFF)
	{
		Alm_mode_cancel();			/* ���v�������[�h��ݾ� */
	}
}
/****************************************************************
		�֐����FSbm_100ms_cnt
		��  ���F����
		�߂�l�F����
		�@  �\�F100ms��϶���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2007/3/17		0.1		��		�V�K
****************************************************************/
void Sbm_100ms_cnt(void)
{
	/* VOLӰ�ޕێ���϶��� */
	if(sbm_vol_tim > 0)
	{
		sbm_vol_tim--;
		if(sbm_vol_tim == 0)
		{
			Vol_mode_cancel();
			#if 0		/* �x�[�X�\�����Ȃ� */
			Lcd_disp_base(Ma_current_mode());		/* Base�\���v�� */
			#endif
		}
	}
	
	/* Audio�������[�h�ێ���϶��� */
	if(sbm_aud_tim > 0)
	{
		sbm_aud_tim--;
		if(sbm_aud_tim == 0)
		{
			Sef_mode_cancel();
			#if 0		/* �x�[�X�\�����Ȃ� */
			Lcd_disp_base(Ma_current_mode());		/* Base�\���v�� */
			#endif
		}
	}
	
	/* Alarm����Ӱ�ޕێ���϶��� */
	if(sbm_alm_tim > 0)
	{
		sbm_alm_tim--;
		if(sbm_alm_tim == 0)
		{
			Alm_mode_cancel();
			#if 0		/* �x�[�X�\�����Ȃ� */
			Lcd_disp_base(Ma_current_mode());		/* Base�\���v�� */
			#endif
		}
	}
	
	/* Clock����Ӱ�ޕێ���϶��� */
	if(sbm_clk_tim > 0)
	{
		sbm_clk_tim--;
		if(sbm_clk_tim == 0)
		{
			Clk_adj_cancel();
			#if 0		/* �x�[�X�\�����Ȃ� */
			Lcd_disp_base(Ma_current_mode());		/* Base�\���v�� */
			#endif
		}
	}
	
	#if 0
	/* Beep On time : 180S */
	if (sbm_beepon_tim > 0)
	{
		sbm_beepon_tim--;
		if(sbm_beepon_tim == 0)
		{
			fsbm_alm_beepon = OFF;
			fsbm_beep_en = ON;
			Aud_cancel_beep();	/* beep off */
		}
	}
	#endif
}
#undef		_SBM_SYS_C_
