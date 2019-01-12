/***************************************************************
	�N�����I���a����   
	�v���W�F�N�g��	�F'07�N���f���J��
	�t�@�C����		�Flcd_main.c
	���W���[����	�FLCD(�\��)
	�@   �\			�FLCD���C������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
#define	_LCD_MAIN_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../power/pw_func_ext.h"
#include	"../main/ma_ext.h"
#include	"../submode/sbm_ext.h"
#include	"../submode/sbm_def.h"
#include	"../io/IO_inc.h"
#include	"../tuner/TU_inc.h"
#include	"../audio/aud_ext.h"


#include	"lcd_main.h"
#include	"lcd_send.h"
#include	"lcd_mode.h"
#include	"lcd_ext.h"
#include	"lcd_sys.h"
#include	"lcd_font.h"


#include	<stdio.h>
#include	<string.h> 
#include	<stdlib.h>
/****************************************************************
		�֐����FLcd_main
		��  ���F����
		�߂�l�F����
		�@  �\�F���C������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_main(void)
{
	if(flcd_send == ON)							/* ���M�v��������΁A���M */
	{
		Lcd_send();
	}
	else if ((Pw_ACC_DET_get() == OFF)&&(Alm_mode_check()== OFF))
	{
		if (flcd_accoff_send == TRUE)
		{
			Lcd_ram_initial();							/* ���M�ޯ̧�ر */
			if(Ma_model_dest() == 0x00 )
			{
				Lcd_area_disp();
			}
			else
			{
				Lcd_clock_mode();
			}
			Lcd_indictrol();
			Lcd_edit();							/* LCD�����ĕҏW */
			flcd_accoff_send = OFF;
		}
	}
	else if (lcd_edit_tim == 0)
	{
		lcd_edit_tim = CLCD_ACCON_EDIT_TIM;					/* 50ms��ύĐݒ� */
		Lcd_ram_initial();							/* ���M�ޯ̧�ر */
		if (flcd_test_mode == OFF)
		{
			Lcd_mode();								/* Ӱ�ޕ��򏈗� */
			if(flcd_blank != ON)					/* DISP OFF���_�ŁA�ݼ޹���A�����ĕҏW�������� */
			{
				Lcd_indictrol();					/* �ݼ޹������ */
				Lcd_blink();						/* �_�ŏ��� */
				Lcd_edit();							/* LCD�����ĕҏW */
			}
		}
		else
		{
			Lcd_test_edit();
		}
	}
	else
	{
		/*�����Ȃ�*/
	}
}
/****************************************************************
		�֐����FLcd_initial
		��  ���F����
		�߂�l�F����
		�@  �\�F�\��Ӽޭ�ق̏�����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_initial(void)
{
	lcd_tim_pwr_wait = 0xFF;
	lcd_edit_tim = CLCD_ACCON_EDIT_TIM;		/* �ҏW������Ϗ����� */
	flcd_test_mode   = OFF;
	Lcd_ram_initial();					/* ���M�ޯ̧�Ȃǂ̸ر */
}
/****************************************************************
		�֐����FLcd_ram_initial
		��  ���F����
		�߂�l�F����
		�@  �\�FRAM�ر
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
�E1/3�f���[�e�B
�E�L�[�X�L�����֎~(KSC:1)
�E�L�[�X�L��������(KSC:0)
****************************************************************/
void Lcd_ram_initial(void)
{
	/* ���M�ޯ̧������ */
	memset(lcd_send_buf.buf_byte,0,sizeof(lcd_send_buf.buf_byte));	/* ���M�o�b�t�@0�ر */
/*---------------------------------------------------------------------------*/
/*0 SP KC0 KC1*/
/*KC2 KSC K0 K1 P0 P1 P2 SC*/
/*DR DT FC0 FC1 FC2 OC 0 0*/
	lcd_send_buf.record.ctrl_1 = 0x00;	/* 0000���۰��ް� */
	#if 1
	if ((Pw_ACC_DET_get() == OFF)&&(Alm_mode_check() == FALSE)&&(Alm_beepon_get() == FALSE))
	#else
	//if (Pw_ACC_status_get() == OFF)
	if((Pw_ACC_status_get() == OFF)&&(Alm_mode_check() == FALSE))
	#endif
	{
		lcd_send_buf.record.ctrl_2 = 0x0B;	/* 11010000 *//* �L�[�X�L�����֎~ */
	}
	else
	{
		lcd_send_buf.record.ctrl_2 = 0x09;	/* 10010000 *//* �L�[�X�L�������� */
	}
	lcd_send_buf.record.ctrl_3 = 0x12;	/* 01001000 */		/* 1/3 Duty */
	
	lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;	/* 2ں��ޖڌŒ��ް� */
	lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;	/* 3ں��ޖڌŒ��ް� */

	/* �����ޯ̧�ر */
	memset(lcd_seg_buf,0,sizeof(lcd_seg_buf));
	/* �_��Ӱ�޸ر */
	lcd_blk_mode = 0;									/* �_��Ӱ�޸ر,���v�������ر���Ȃ� */
	
	flcd_send = ON;
}
/****************************************************************
		�֐����FLcd_pwr_on
		��  ���F����
		�߂�l�F����
		�@  �\�FLCD�h���C�o�[PwrOn(ACC ON)����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2006/3/13		0.1		��		�V�K�쐬
****************************************************************/
void Lcd_driver_send( void )
{
	/* LCD��ײ�ޑ��M */
	Lcd_ram_initial();
	flcd_blank = ON;									/* ���ݸ���M */
	Lcd_send();
	
}

/****************************************************************
		�֐����FLcd_indictrol
		��  ���F����
		�߂�l�F����
		�@  �\�F�C���W�P�[�^�ݒ菈��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_indictrol( void )
{
	BYTE ind;
	
	#if 0
	if ((Clk_adj_check() == ON)||(lcd_base_mode == OFF )||(Pw_Power_status_get() != ON))
	{
		ind = Clk_indicator_set();
		if((ind & CLCD_CL_IND_AM) != 0 )		/* "AM" */
		{
			lcd_send_buf.record1.am = ON;
		}
		if((ind & CLCD_CL_IND_PM) != 0 )		/* "PM" */
		{
			lcd_send_buf.record1.pm = ON;
		}
	}
	else if (Alm_mode_check() == ON)
	{
		ind = Alm_indicator_set();
		if((ind & CLCD_CL_IND_AM) != 0 )		/* "AM" */
		{
			lcd_send_buf.record1.am = ON;
		}
		if((ind & CLCD_CL_IND_PM) != 0 )		/* "PM" */
		{
			lcd_send_buf.record1.pm = ON;
		}
	}
	else if((Sbm_mode_check() != CSBM_VOL )&&(Sbm_mode_check() != CSBM_SEF ))
	{
		BYTE ma_mode;
		/* ���Ӱ�޲ݼ޹�� */
		ma_mode = Ma_current_mode();
		switch(ma_mode)
		{
			/* TUNER���[�h */
			case CMA_TUNER:
				ind = Tu_indicator_set();
				if((ind & CLCD_TU_IND_FM) != 0)			/* "FM"   */
				{
					lcd_send_buf.record1.fm = ON;
				}
				if((ind & CLCD_TU_IND_AM) != 0)			/* "AM"   */
				{
					lcd_send_buf.record1.am = ON;
				}
				if((ind & CLCD_TU_IND_ONE) != 0)			/* "1"     */
				{
					lcd_send_buf.record1.one = ON;
				}
				if((ind & CLCD_TU_IND_TWO) != 0)			/* "2"     */
				{
					lcd_send_buf.record1.two = ON;
				}
				if((ind & CLCD_TU_IND_ST) != 0)			/* "ST"    */
				{
					lcd_send_buf.record1.st = ON;
				}
				#if 0
				if((ind & CLCD_TU_IND_S1) != 0)			/* "1"    */
				{
					lcd_send_buf.record1.s1 = ON;
				}
				#endif
				break;
				
			case CMA_ISR:
				ind = Tu_isr_indicator_set();
				if((ind & CLCD_TU_IND_AM) != 0)			/* "AM"   */
				{
					lcd_send_buf.record1.am = ON;
				}
				#if 0
				if((ind & CLCD_TU_IND_TI) != 0)			/* "�E)))"   */
				{
					lcd_send_buf.record1.ti = ON;
				}
				if((ind & CLCD_TU_IND_S1) != 0)				/* "1"    */
				{
					lcd_send_buf.record1.s1 = ON;
				}
				#endif
				break;
				
			case CMA_AUX:	/* AUX�͂Ȃ� */
			default:
				break;
		}
	}
	else
	{
		;
	}
	
	#endif
	
	if(Alm_bell_indicator_set() == ON )		/* �u�A���[���L���v */
	{
		lcd_send_buf.record1.al = ON;
	}
	if(Ma_current_mode() == CMA_ISR )		/* �u�A���[���L���v */
	{
		lcd_send_buf.record1.ti = ON;
	}
	
}
/****************************************************************
		�֐����FLcd_blink
		��  ���F����
		�߂�l�F����
		�@  �\�F�����_�ŏ���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/30		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_blink(void)
{
	if( lcd_blk_mode == OFF ) 
	{
		lcd_tim_blk = 0;							/* �_����ω��� */
		return;
	}
	
	switch(lcd_blk_type)
	{
		case BLK_500MS:										/* 500ms���� */
			if(lcd_tim_blk == 0)
			{
				lcd_tim_blk = CLCD_TIMECNT_1S;				/* ������ύĐݒ� */
				flcd_blk_clr = OFF;
			}
			else if(lcd_tim_blk >= CLCD_TIMECNT_500MS)
			{
				return;
			}
			else
			{
				;
			}
			break;
		case BLK_1S:										/* 1s���� */
			if(lcd_tim_blk == 0)
			{
				lcd_tim_blk = CLCD_TIMECNT_2S;				/* ������ύĐݒ� */
				flcd_blk_clr = OFF;
			}
			else if(lcd_tim_blk >= CLCD_TIMECNT_1S)
			{
				return;
			}
			else
			{
				;
			}
			break;
		default:
			break;
	}
		
	if(((lcd_blk_type == BLK_1S) && (lcd_tim_blk < CLCD_TIMECNT_1S))
	|| ((lcd_blk_type == BLK_500MS) && (lcd_tim_blk < CLCD_TIMECNT_500MS)))
	{
		if(flcd_blk_clr == OFF)						/* ���������������Ȃ� */
		{
			switch(lcd_blk_mode)
			{
				case CLCD_BLK_FREQ:
					lcd_send_buf.record1.s1 = OFF;
					lcd_send_buf.record1.dot = OFF;
					memset(&lcd_seg_buf[0],0,5);
					break;
				
				case CLCD_BLK_CLOCK:
					lcd_send_buf.record1.colon = OFF;
					lcd_send_buf.record1.am = OFF;
					lcd_send_buf.record1.pm = OFF;
					memset(&lcd_seg_buf[0],0,4);
					break;
				
				case CLCD_BLK_ALARM:
					lcd_send_buf.record1.al = OFF;
					lcd_send_buf.record1.colon = OFF;
					lcd_send_buf.record1.am = OFF;
					lcd_send_buf.record1.pm = OFF;
					memset(&lcd_seg_buf[0],0,4);
					break;
				case CLCD_BLK_PSNO:	
					memset(&lcd_seg_buf[4],0,1);
					break;
				
				default:
					break;
			}
		}
	}
}
/****************************************************************
		�֐����FLcd_blink_set
		��  ���F����
		�߂�l�F����
		�@  �\�F�����_�Őݒ�
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2006/3/13		0.1		��		�V�K�쐬
****************************************************************/
void Lcd_blink_set(BYTE blkmode, BYTE blktype)
{
	lcd_blk_mode = blkmode;			/* �_��Ӱ�� */
	lcd_blk_type = blktype;			/* �_�Ŏ��� */
}
/****************************************************************
		�֐����FLcd_25ms_cnt
		��  ���F����
		�߂�l�F����
		�@  �\�F25ms��϶���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_25ms_cnt(void)
{
	/* MAIN_CNT�҂���� */
	if((lcd_tim_pwr_wait > TIMER_OUT) && (lcd_tim_pwr_wait != 0xFF))
	{
		lcd_tim_pwr_wait--;
	}
}

/****************************************************************
		�֐����FLcd_50ms_cnt
		��  ���F����
		�߂�l�F����
		�@  �\�F50ms��϶���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_50ms_cnt(void)
{
	/* �_����϶��� */
	if(lcd_tim_blk > TIMER_OUT)
	{
		lcd_tim_blk--;
	}
	/* �ꎞ������϶��� */
	if(lcd_tim_1shot_blk > TIMER_OUT)
	{
		lcd_tim_1shot_blk--;
		if(lcd_tim_1shot_blk == TIMER_OUT)
		{
			lcd_1shot_blk = OFF;
		}
	}
}
/****************************************************************
		�֐����FLcd_100ms_cnt
		��  ���F����
		�߂�l�F����
		�@  �\�F100ms��϶���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_100ms_cnt(void)
{
	/* �����ݼވꎞ�\����϶��� */
	if(lcd_src_tim > TIMER_OUT)							/* 2�b�\����� */
	{
		lcd_src_tim--;
		if(lcd_src_tim == TIMER_OUT)
		{
			lcd_src_mode = OFF;
			Lcd_disp_1shot_base(Ma_current_mode());		/* 2�b�ް��\�� */
		}
	}
	/* �ꎞ�\����϶��� */
	if(lcd_tim_1shot > TIMER_OUT)
	{
		lcd_tim_1shot--;
		if(lcd_tim_1shot == TIMER_OUT)
		{
				lcd_1shot_mode = OFF;
		}
	}
	/* �x�[�X�\����϶��� */
	if (Ma_get_clock() == TRUE )
	{
		if(lcd_tim_base > TIMER_OUT)
		{
			lcd_tim_base--;
			if(lcd_tim_base == TIMER_OUT)
			{
				lcd_base_mode = OFF;
			}
		}
	}
}


/*------------lcd_mode.c����ړ�----------------------*/
/****************************************************************
		�֐����Flcd_moji_romcpy
		��  ���F����
		�߂�l�F����
		�@  �\�FHEXtoASCII����
				ASCII�ް������RAM��2�o�C�g
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void lcd_moji_romcpy(BYTE *savebuf, BYTE *addfrom)
{
	const BYTE *add;
	
	add =  (const BYTE *)addfrom;
	*savebuf =  *add;
	*(savebuf+1) = *(add+1);
	*(savebuf+2) = *(add+2);
	*(savebuf+3) = *(add+3);
	*(savebuf+4) = *(add+4);
	*(savebuf+5) = *(add+5);
}

/****************************************************************
		�֐����Flcd_hextoascii
		��  ���F����
		�߂�l�F����
		�@  �\�FHEXtoASCII����
				ASCII�ް������RAM��2�o�C�g
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_hextoascii( BYTE hex, BYTE *ascii )
{
	*ascii = ( hex/10 ) + 0x30;
	*(ascii+1) = ( hex%10 ) +0x30;
}
/****************************************************************
		�֐����Flcd_nodisp
		��  ���Fdata�߲���Adata��size
		�߂�l�F����
		�@  �\�F0�̏ꍇ�A�\�����Ȃ�
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_nodisp( BYTE *data, BYTE size )
{
	if( *data == 0x30 )
	{
		*data = 0;
	}
	data ++;
	size --;
	while(size)
	{
		if( *(data - 1) == 0)
		{
			if( *data == 0x30 )
			{
				*data = 0;
			}
		}
		data ++;
		size --;
	}
}
#undef	_LCD_MAIN_C_
