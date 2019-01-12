/***************************************************************
	�N�����I���a����     
	�v���W�F�N�g��	�F'12�N���f���J��
	�t�@�C����		�Flcd_ext.c
	���W���[����	�FLCD(�\��)
	�@   �\			�F�C���^�t�F�[�X�֐��t�@�C��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
#define	_LCD_EXT_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../power/pw_func_ext.h"
#include	"../submode/sbm_ext.h"
#include	"../main/ma_ext.h"
#include	"../tuner/TU_inc.h"

#include 	<stdio.h>
#include	<string.h>


#include	"lcd_main.h"
#include	"lcd_mode.h"
#include	"lcd_sys.h"
#include	"lcd_ext.h"
#include	"lcd_tbl.h"
#include	"lcd_send.h"

#define		EDITWAIT		0xFF
static  void Lcd_disp_baseset(BYTE mode_id,BYTE tim);

/****************************************************************
		�֐����FLcd_1shotsrc_cancel
		��  ���F����
		�߂�l�F����
		�@  �\�FӰ����ݼޕ\����ݾ�
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
BOOL Lcd_1shotsrc_cancel(void)
{
	if((lcd_src_mode > OFF) && (lcd_src_mode < CLCD_SRC_MAX))
	{
		lcd_src_mode = OFF;			/* �����ݼ�Ӱ�މ��� */
		lcd_src_tim = OFF;			/* ��ω��� */
		return(OK);
	}
	else
	{
		return(NG);
	}
}
/****************************************************************
		�֐����FLcd_1shot_disp
		�� ��1�Flcd_ext.ḩ�َQ��
		�� ��2�Flcd_ext.ḩ�َQ��
		�߂�l�F����
		�@  �\�F�ꎞ�\���֐�
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_1shot_disp(BYTE mode, BYTE tim)
{
	if((mode > OFF) && (mode < CLCD_1SHOT_MAX))
	{
		lcd_1shot_mode = mode;		/* �\��Ӱ�ސݒ� */
		lcd_tim_1shot = tim;		/* ��ϐݒ� */
		lcd_edit_tim = 0;
	}
}
/****************************************************************
		�֐����FLcd_1shot_cancel
		��  ���Fmode_id(ma_ext.h�Q��)
				CLCD_1SHOT_OFF�ˋ�����ݾ�
		�߂�l�F����
		�@  �\�F�ꎞ�\����ݾي֐�
		�R�����g�F�L�[�ȊO�ŷ�ݾ�(��ԕς���p)
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_1shot_cancel(BYTE mode_id)
{
	BYTE mode;
	mode = Ma_current_mode();
	if((mode_id != mode)&&(mode_id != CLCD_1SHOT_OFF))
	{
		return;				/* ������ݾٗv��������΁A��Ӱ�ނł͂Ȃ��ꍇ�A��ݾق��Ȃ� */
	}
	if((lcd_1shot_mode > OFF) && (lcd_1shot_mode < CLCD_1SHOT_MAX))		/* �ꎞ�\���ł͂Ȃ� */
	{
		/* ADJ/SOUND���[�h��edit timer�������Ȃ�[070825he]*/
		#if 0
		if((Sym_mode_check() == OFF)&&(Sel_mode_check() ==  OFF))
		{
			/*[20070526]edit timer����*/
			Lcd_editime_set();
		}
		#endif
		lcd_1shot_mode = OFF;		/* Ӱ�މ��� */
		lcd_tim_1shot = OFF;		/* ��ϸر */
	}
}
/****************************************************************
		�֐����FLcd_1shot_cancel
		��  ���Fmode_id(ma_ext.h�Q��)
				CLCD_1SHOT_OFF�ˋ�����ݾ�
		�߂�l�F����
		�@  �\�F�ꎞ�\����ݾي֐�
		�R�����g�F�L�[�ŷ�ݾ�(�L�[�����p)
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_1shot_key_cancel(BYTE mode_id)
{
	BYTE mode;
	
	mode = Ma_current_mode();
	/* ����Ӱ�ވȊO��������ݾٗv�����Ȃ���΁A�������Ȃ� */
	if((mode_id != mode) && (mode_id != CLCD_1SHOT_OFF))
	{
		return;
	}
	/* ����J�ڈꎞ�\����ݾ� */
	Lcd_1shotsrc_cancel();
}
/****************************************************************
		�֐����FLcd_disp_base
		��  ���Fmode_id(ma_ext.ḩ�َQ��)
		�߂�l�F����
		�@  �\�F�x�[�X�\���v��(�ꎞ�\����ݾ�)
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_disp_base(BYTE mode_id)
{
	BYTE mode;
	
	mode = Ma_current_mode();
	if(mode == mode_id)				/* ����Ӱ�ވȊO��t���Ȃ� */
	{
		Lcd_1shot_key_cancel(mode_id);				/* �L�[����Ŏ����\����ݾ� */
		Lcd_disp_baseset(mode_id,CLCD_BASE_5s);	/* 5s�ް��\�� */
	}
}
/****************************************************************
		�֐����FLcd_disp_1shot_base
		��  ���Fmode_id(ma_ext.ḩ�َQ��)
		�߂�l�F����
		�@  �\�F�x�[�X�\���v��(�ꎞ�\����ݾق��Ȃ�)
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_disp_1shot_base(BYTE mode_id)
{
	BYTE mode;
	
	mode = Ma_current_mode();
	if(mode == mode_id)							/* ����Ӱ�ވȊO��t���Ȃ� */
	{
		Lcd_disp_baseset(mode_id,CLCD_BASE_5s);	/* 5s�ް��\�� */
	}
}

/****************************************************************
		�֐����FLcd_base_scrll
		��  ���Fmode_id(ma_ext.ḩ�َQ��)
		�߂�l�F����
		�@  �\�F�x�[�X�\���v��&��۰ٗv��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_base_scrll(BYTE mode_id)
{
	BYTE ma_mode;
	
	ma_mode = Ma_current_mode();		/* ����Ӱ�ގ擾			  */
	if(ma_mode == mode_id )				/* ����Ӱ�ވȊO�������Ȃ� */
	{
		Lcd_disp_baseset(mode_id,CLCD_BASE_5s);			/* 5s�ް��\���v�� */
	}
}

void Lcd_base_disp_cancel(void)
{
	if (Ma_get_clock() == TRUE )
	{
		lcd_base_mode = OFF;
		lcd_tim_base = TIMER_OUT;
	}
}
/********************************************************************
			�� �� ���FLcd_disp_baseset
			��    ���F����
			�� �� �l�F����
			�@    �\�F�ް��\���v��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
static  void Lcd_disp_baseset(BYTE mode_id,BYTE tim)
{
	BYTE mode;
	switch(mode_id)
	{
		case CMA_TUNER:
			lcd_base_mode = CLCD_BASE_TUNER;
			break;
		case CMA_ISR:
			lcd_base_mode = CLCD_BASE_ISR;
			break;
		case CMA_AUX:
			lcd_base_mode = CLCD_BASE_AUX;
			break;
		default:
			break;
	}
	lcd_tim_base = tim;					/* ��ϐݒ� */
}
/****************************************************************
		�֐����FLcd_10ms_cnt
		��  ���F����
		�߂�l�F����
		�@  �\�F10ms�����ݏ���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_10ms_cnt(void)
{
/* �\���ҏW��� */
	if(lcd_edit_tim > TIMER_OUT)
	{
		lcd_edit_tim--;
	}
	if(lcd_base_wait_tim > TIMER_OUT)
	{
		lcd_base_wait_tim--;
		if(lcd_base_wait_tim == TIMER_OUT)
		{
			Lcd_disp_baseset(Ma_current_mode(),CLCD_BASE_5s);	/* 5s�ް��\���v�� */
		}
	}
}

/********************************************************************
			�֐����FLcd_blktim_set
			��  ���F����
			�߂�l�F����
			�@  �\�F�������쒆�_�ł����Ȃ��悤��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/31		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_blktim_set(void)
{
	#if 0
	flcd_blk_clr = ON;						/* �_�ł��Ȃ��v�� */
	#else
	lcd_tim_blk = CLCD_TIMECNT_1S;
	#endif
}

/********************************************************************
			�֐����FLcd_send_req
			��  ���F����
			�߂�l�F����
			�@  �\�F���M�v���ݒ�
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/08/20		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_send_req(void)
{
	#if 0
	flcd_send = ON;						/* �_�ł��Ȃ��v�� */
	#else
	lcd_edit_tim = 0;
	#endif
}

#if 1
void Lcd_accoff_send_req(void)
{
	flcd_accoff_send = ON;
}

BOOL Lcd_accoff_send_end(void)
{
	return (flcd_accoff_send);
}

#endif
/********************************************************************
			�֐����FLcd_allseg_req
			��  ���Fonoff
					0x00�F	�e�X�g���[�h�ȊO
					0x01�F	�S�_��
					0x02�F	�S����
			�߂�l�F����
			�@  �\�F�S�_��������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/08/20		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_testmode_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length)
{
	flcd_test_mode = ON;
	if ( Length<= 4)
	{
		memcpy(lcd_test_buf,RcvBuf,Length);
	}
}

#undef	_LCD_EXT_C_
