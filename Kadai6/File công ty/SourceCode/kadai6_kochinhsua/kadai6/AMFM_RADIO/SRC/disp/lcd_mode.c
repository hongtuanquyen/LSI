/***************************************************************
	�N�����I���a����       
	�v���W�F�N�g��	�F'12�N���f��
	�t�@�C����		�Flcd_mode.c
	���W���[����	�FLCD(�\��)
	�@   �\			�FӰ�ޕ��򏈗��֐��t�@�C��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
#define		_LCD_MODE_C_

#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#include	"../model.h"

#include	"../comm/common.h"
#include	"../audio/aud_ext.h"
#include	"../audio/aud_dev_func.h"
#include	"../main/ma_ext.h"
#include	"../tuner/TU_inc.h"
#include	"../submode/SBM_inc.h"
#include	"../testmode/TEST_inc.h"
#include	"../power/pw_def.h"
#include	"../power/pw_func_ext.h"	/* QAC�΍� */
#include	"lcd_main.h"
#include	"lcd_ext.h"
#include	"lcd_mode.h"
#include	"lcd_send.h"
#include	"lcd_sys.h"
#include	"lcd_tbl.h"


/* ���[�J���֐� */
static void Lcd_main_mode(void);
static void Lcd_off_mode(void);
static void Lcd_snd_mode(void);
static void Lcd_1shotdisp_mode(void);
static void Lcd_tu_base(TDP_TUNER *);
static void Lcd_tu_base_frq(BYTE band,BYTE bandno,BYTE *freq,BYTE preno);
static void Lcd_tu_indicator(void );
static void Lcd_st_indicator(void );
static void Lcd_clkalm_indicator(void);
static void	Lcd_blank_disp(void);

static void Lcd_tuner_mode(void);

static void Lcd_aux_mode(void);
static void Lcd_vol_mode(void);
static void Lcd_isr_mode(void);
static void Lcd_model_version(void);
/****************************************************************
		�� �� ���FLcd_mode
		��    ���F����
		�� �� �l�F����
		�@    �\�FӰ�ޕ��򏈗�
		�R�����g�F�\���D��x�̏�
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_mode(void)
{ 
	if(Ma_model_dest() == 0x00 )
	{
		Lcd_area_disp();
	}
	#if 0 
	else if (Pw_ACC_DET_get() == OFF)
	{
		Lcd_off_mode();					/* PWR OFF */
	}
	#endif
	else if(Pw_PowerOff_disp_chk() == TRUE)
	{
		Lcd_off_mode();					/* PWR OFF */
	}
	else if (Sct_mode_check() == CSBM_SCT_VER)
	{
		Lcd_model_version();
	}
	#if 0	/* ���g�p */
	else if((lcd_src_mode > 0)&&(lcd_src_mode < CLCD_SRC_MAX))
	{
		Lcd_srcchg_mode();				/* ����J�ڈꎞ�\�� */
	}
	#endif
	else if((Sbm_mode_check() != CSBM_OFF)&&(Sbm_mode_check() !=CSBM_TES))
	{
		Lcd_st_indicator();
		if(Vol_mode_check() != OFF)
		{
			Lcd_vol_mode();
		}
		else if(Sef_mode_check() !=  OFF)
		{
			Lcd_snd_mode();					/* SOUNDӰ�� */
		}
		else if((Clk_adj_check() == ON)||(Alm_mode_check() == ON)||(Alm_beepon_get() == TRUE))
		{
			Lcd_clock_mode();
		}
		else
		{
			;
		}
	}
	else
	{
		Lcd_st_indicator();
		Lcd_main_mode();				/* �eҲ�Ӱ�ޏ��� */
	}
}
/****************************************************************
		�֐����FLcd_main_mode
		��  ���F����
		�߂�l�F����
		�@  �\�F�eҲ�Ӱ�ޕ\������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_main_mode(void)
{
	BYTE ma_mode;
	ma_mode = Ma_current_mode();
	switch(ma_mode)
	{
		case CMA_TUNER:
			Lcd_tuner_mode();						/* TUNERӰ�� */
			break;
		
		case CMA_AUX:
			Lcd_aux_mode();							/* AUXӰ�ށ@ */
			break;
			
		case CMA_ISR:								/* ISR���[�h */
			Lcd_isr_mode();
			break;
		
		default:
			break;
	}
}
/****************************************************************
		�֐����FLcd_off_mode
		��  ���F����
		�߂�l�F����
		�@  �\�FOFFӰ�ޕ\������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_off_mode(void)
{
	#if 1
	Lcd_clock_mode();			/* OFFӰ�ޒ����v�\�� */
	#else
	if (Ma_get_clock() == TRUE)
	{
		Lcd_clock_mode();			/* OFFӰ�ޒ����v�\�� */
	}
	else
	{
		Lcd_blank_disp();
	}
	#endif
}

#if 0	/* ���g�p */
/****************************************************************
		�֐����FLcd_srcchg_mode
		��  ���F����
		�߂�l�F����
		�@  �\�F����J�ڈꎞ�\������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_srcchg_mode(void)
{
	//lcd_8moji_romcpy(lcd_seg_buf,(BYTE *)CLCD_SRC_TBL[lcd_src_mode-1]);
	lcd_moji_romcpy(lcd_seg_buf,(BYTE *)CLCD_SRC_TBL[lcd_src_mode-1]);
}
#endif

/****************************************************************
		�֐����FLcd_vol_mode
		��  ���F����
		�߂�l�F����
		�@  �\�FVol�����\������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_vol_mode(void)
{
	BYTE	value;
	
	value = Vol_lcddata_set();
	lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[VOL]);
	lcd_send_buf.record1.colon = ON;
	Lcd_hextoascii(value,&lcd_seg_buf[SELVOL]);		/* HEXtoASCII */
	Lcd_nodisp(&lcd_seg_buf[SELVOL],1);						/* ��ʂ�0�Ȃ�A�\�����Ȃ� */
	
}
/****************************************************************
		�֐����FLcd_snd_mode
		��  ���F����
		�߂�l�F����
		�@  �\�FAudio�����\������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_snd_mode(void)
{
	TDP_SEL seldisp;
	BYTE value;
	
	memset( &seldisp, 0, sizeof(seldisp));			/* �ر */
	Sef_lcddata_set( &seldisp );					/* AUDIO�ް����擾 */
	lcd_send_buf.record1.colon = ON;
	switch(seldisp.mode)
	{
		case CSBM_AUD_BAS:
			lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAS]);
			if(seldisp.value < CAUD_BAS_CNT)
			{
				value = (CAUD_BAS_CNT - seldisp.value);
				Lcd_hextoascii(value,&lcd_seg_buf[SELBAS]);		/* HEXtoASCII */
				Lcd_nodisp(&lcd_seg_buf[SELBAS],1);				/* ��ʂ�0�Ȃ�A�\�����Ȃ� */
				lcd_seg_buf[SELBAS] = '-';							/* "-" */
			}
			else if(seldisp.value > CAUD_BAS_CNT)
			{
				value = (seldisp.value - CAUD_BAS_CNT);
				Lcd_hextoascii(value,&lcd_seg_buf[SELBAS]);		/* HEXtoASCII */
				Lcd_nodisp(&lcd_seg_buf[SELBAS],1);				/* ��ʂ�0�Ȃ�A�\�����Ȃ� */
				lcd_seg_buf[SELBAS] = '+';
			}
			else
			{
				lcd_seg_buf[SELBAS+1] = '0';
			}
			break;
		case CSBM_AUD_TRE:
			lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[TRE]);
			if(seldisp.value < CAUD_TRE_CNT)
			{
				value = (CAUD_TRE_CNT - seldisp.value);
				Lcd_hextoascii(value,&lcd_seg_buf[SELTRE]);		/* HEXtoASCII */
				Lcd_nodisp(&lcd_seg_buf[SELTRE],1);				/* ��ʂ�0�Ȃ�A�\�����Ȃ� */
				lcd_seg_buf[SELTRE] = '-';							/* "-" */
				
			}
			else if(seldisp.value > CAUD_TRE_CNT)
			{
				value = (seldisp.value - CAUD_TRE_CNT);
				Lcd_hextoascii(value,&lcd_seg_buf[SELTRE]);		/* HEXtoASCII */
				Lcd_nodisp(&lcd_seg_buf[SELTRE],1);				/* ��ʂ�0�Ȃ�A�\�����Ȃ� */
				lcd_seg_buf[SELTRE] = '+';							/* "+" */
			}
			else
			{
				lcd_seg_buf[SELTRE+1] = '0';
			}
			break;
		case CSBM_AUD_BAL:
			if(seldisp.value == CAUD_BAL_CNT)
			{
				lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAL_C]);
			}
			else
			{
				lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAL]);
				if(seldisp.value > CAUD_BAL_CNT)
				{
					
					seldisp.value -= CAUD_BAL_CNT;
					Lcd_hextoascii( seldisp.value, &lcd_seg_buf[SELBAL] );		/* HEX to ASCII */
					Lcd_nodisp( &lcd_seg_buf[SELBAL],1 );						/* ��ʂ�0�ꍇ�A�\�����Ȃ� */
					lcd_seg_buf[SELBAL] = 'R';
				}
				else
				{
				
					seldisp.value = CAUD_BAL_CNT - seldisp.value;
					Lcd_hextoascii( seldisp.value, &lcd_seg_buf[SELBAL] );		/* HEX to ASCII */
					Lcd_nodisp( &lcd_seg_buf[SELBAL],1 );						/* ��ʂ�0�ꍇ�A�\�����Ȃ� */
					lcd_seg_buf[SELBAL] = 'L';
				}
			}
			break;
		default:
			break;
	}
}

/****************************************************************
		�֐����FLcd_tuner_mode
		��  ���F����
		�߂�l�F����
		�@  �\�FTUNERӰ�ޕ\������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/30		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_tuner_mode(void)
{
	static TDP_TUNER tudisp;
	BYTE dmode;
	BYTE ind;
	
	memset(&tudisp,0,sizeof(tudisp));				/* �ޯ̧�ر */
	
	if (Ma_get_clock() == FALSE)
	{
		dmode = CLCD_BASE_TUNER;
	}
	else
	{
		if(lcd_base_mode == CLCD_BASE_TUNER)
		{
			dmode = CLCD_BASE_TUNER;
		}
		else
		{
			dmode = CLCD_BASE_CLOCK;
		}
	}
	
	Tu_lcddata_set(&tudisp,dmode);
	if((tudisp.mode != CLCD_DISPMODE) && (tudisp.mode < CLCD_TU_MAX))		/* �����ԁA����Ӱ�� */
	{
		switch(tudisp.mode)			/* �\��Ӱ�� */
		{
			case CLCD_TU_SEEK:			/* SEEK�� */
				Lcd_tu_base(&tudisp);
				//Lcd_tu_base_frq(tudisp.band,tudisp.bandno,&(tudisp.frq[0]),tudisp.preno);
				Lcd_disp_1shot_base(CMA_TUNER);			/* SEEK���ް�5�b�\���v�� */
				break;
	
			case CLCD_TU_SCAN:			/* ��ؾ�Ľ��ݒ� */
				Lcd_tu_base(&tudisp);
				break;
				
			case CLCD_TU_SCANWAIT:
				Lcd_tu_base(&tudisp);
				Lcd_blink_set(CLCD_BLK_FREQ,BLK_500MS);		/* ���g���_�ŏ��� */
				break;
			
			case CLCD_TU_PSTWT:
				Lcd_tu_base(&tudisp);
				Lcd_blink_set(CLCD_BLK_PSNO,BLK_500MS);		/* �v���Z�b�gNo�_�ŏ��� */
				break;
			
			case CLCD_TU_AS:			/* SEEK�� */
				Lcd_tu_base(&tudisp);
				Lcd_disp_1shot_base(CMA_TUNER);			/* SEEK���ް�5�b�\���v�� */
				break;
	
			default:
				break;
		}
	}
	else								/* DISPӰ�� */
	{
		if(Clk_adj_check() == ON)
		{
			dmode = CLCD_BASE_CLOCK;		/* ���v�����\���ݒ� */
		}
		switch(dmode)
		{
			case CLCD_BASE_TUNER:
				Lcd_tu_base(&tudisp);
				break;
			case CLCD_BASE_CLOCK:
				Lcd_clock_mode();
				break;
			default:
				break;
		}
	}
}
/****************************************************************
		�֐����FLcd_tu_base
		��  ���F����
		�߂�l�F����
		�@  �\�F�x�[�X�\������(TUNER)
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_tu_base(TDP_TUNER *add)
{
	#if 0
	BYTE tu_ind;
	tu_ind = Tu_indicator_set();
	if((tu_ind & CLCD_TU_IND_S1) != 0)				/* "1"    */
	{
		lcd_send_buf.record1.s1 = ON;
	}
	#endif
	Lcd_tu_indicator();
	Lcd_tu_base_frq(add->band,add->bandno,&(add->frq[0]),add->preno);
}
/****************************************************************
		�֐����FLcd_tu_base_frq
		��  ���F����
		�߂�l�F����
		�@  �\�F�x�[�X�\������(TUNER���g��)
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_tu_base_frq(BYTE band,BYTE bandno,BYTE *freq,BYTE preno)
{
	BYTE dest;
	
	switch(band)
	{
		case CTU_FM:
/* ���g���\�� */
			lcd_seg_buf[FRQ] = (((*(freq+1))>>4) & 0x0F) + 0x30;
			lcd_send_buf.record1.dot = ON;					/* "."�_�� */
			//Lcd_nodisp(&lcd_seg_buf[FRQ],1);					/* 0�\�����Ȃ� */
			lcd_seg_buf[FRQ+1] = ((*(freq+1)) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+2] = (((*(freq+2)) >> 4) & 0x0F) + 0x30;
			if (Ma_model_dest() == CMA_MODEL_AMFM_A )
			{
				lcd_seg_buf[FRQ+3] = ((*(freq+2)) & 0x0F) + 0x30;
			}
			else
			{
				Lcd_nodisp(&lcd_seg_buf[FRQ+3],1);								/* 0�\�����Ȃ� */
			}
			break;
		case CTU_AM:
/* ���g���\�� */
			#if 0
			lcd_seg_buf[FRQ] = (((*(freq+1)) >> 4) & 0x0F) + 0x30;
			Lcd_nodisp(&lcd_seg_buf[FRQ],1);								/* 0�\�����Ȃ� */
			lcd_seg_buf[FRQ+1] = ((*(freq+1)) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+2] = (((*(freq+2)) >> 4) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+3] = ((*(freq+2)) & 0x0F) + 0x30;
			#else
			lcd_seg_buf[FRQ] = ((*(freq+1)) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+1] = (((*(freq+2))>>4) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+2] = ((*(freq+2)) & 0x0F) + 0x30;
			#endif
			break;
		default:
			break;
	}
/* ��ؾ�����ް�\�� */
	if((preno > 0) && (preno <= 7))				/* �͈͈ȊO�������Ȃ� */
	{
		lcd_seg_buf[PRENO] = preno + 0x30;
	}
}

void Lcd_tu_indicator(void )
{	
	BYTE ind;
	ind = Tu_indicator_set();
	if((ind & CLCD_TU_IND_S1) != 0)				/* "1"    */
	{
		lcd_send_buf.record1.s1 = ON;
	}
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
	#if 0	/* ST�͕ʓ_�������Ɏd�l�ύX */
	if((ind & CLCD_TU_IND_ST) != 0)			/* "ST"    */
	{
		lcd_send_buf.record1.st = ON;
	}
	#endif
}

/****************************************************************
		�֐����FLcd_st_indicator
		��  ���F����
		�߂�l�F����
		�@  �\�FST�C���W�P�[�^�L�����菈��
				�d�l�ύX�ɂ��ǉ�
				���v�\�����ł�ST�C���W�P�[�^��_�������邽��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2013/04/02		1.0		�R�{	�V�K�쐬
****************************************************************/
static void Lcd_st_indicator(void )
{	
	BYTE ind;
	if( (Ma_current_mode() 	== CMA_TUNER	)	&&
		(Tu_band_chk() 		== CTU_FM		)	)
	{
		ind = Tu_indicator_set();
		if((ind & CLCD_TU_IND_ST) != 0)			/* "ST"    */
		{
			lcd_send_buf.record1.st = ON;
		}
	}
}

/****************************************************************
		�֐����FLcd_isr_mode
		��  ���F����
		�߂�l�F����
		�@  �\�FISRӰ�ޕ\������(��ʏ��)
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/30		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_isr_mode(void)
{
	BYTE dmode;
	BYTE dest;
	BYTE ind;
	TDP_ISR isrdisp;
	BYTE tu_isr_ind;

	if (Ma_get_clock() == FALSE)
	{
		dmode = CLCD_BASE_ISR;
	}
	else
	{
		if(lcd_base_mode == CLCD_BASE_ISR)
		{
			dmode = CLCD_BASE_ISR;
		}
		else
		{
			dmode = CLCD_BASE_CLOCK;
		}
	}
	Tu_isr_lcddata_set(&isrdisp,dmode);
	tu_isr_ind = Tu_isr_indicator_set();
	if((tu_isr_ind & CLCD_TU_IND_TI) != 0)				/* "1"    */
	{
		lcd_send_buf.record1.ti = ON;
	}
	switch(dmode)
	{
		case CLCD_BASE_ISR:
				if((tu_isr_ind & CLCD_TU_IND_S1) != 0)				/* "1"    */
				{
					lcd_send_buf.record1.s1 = ON;
				}
				lcd_send_buf.record1.am = ON;
				#if 1
				lcd_seg_buf[FRQ] = (isrdisp.frq[1] & 0x0F) + 0x30;
				lcd_seg_buf[FRQ+1] = ((isrdisp.frq[2] >> 4) & 0x0F) + 0x30;
				lcd_seg_buf[FRQ+2] = (isrdisp.frq[2] & 0x0F) + 0x30;
				#else
				lcd_seg_buf[FRQ+1] = ((isrdisp.frq[1] >> 4) & 0x0F) + 0x30;
				Lcd_nodisp(&lcd_seg_buf[5],1);								/* 0�\�����Ȃ� */
				lcd_seg_buf[FRQ+2] = (isrdisp.frq[1] & 0x0F) + 0x30;
				lcd_seg_buf[FRQ+3] = ((isrdisp.frq[2] >> 4) & 0x0F) + 0x30;
				lcd_seg_buf[FRQ+4] = (isrdisp.frq[2] & 0x0F) + 0x30;
				#endif
			break;
		
		case CLCD_BASE_CLOCK:
			Lcd_clock_mode();				/* ���v�\�� */
			break;
	
		default:
			break;
	}
}
/****************************************************************
		�֐����FLcd_model_version
		��  ���F����
		�߂�l�F����
		�@  �\�F�@�햼�\��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2007/4/23		0.1		��		�V�K�쐬
			2010/6/9		0.2		xu		
****************************************************************/
static void Lcd_model_version(void)
{
	lcd_seg_buf[0] = 'U';
	lcd_seg_buf[1] = '-';
	memcpy(&lcd_seg_buf[2],_VERSION_,2);
	lcd_seg_buf[4] = Ma_model_dest()+0x30;
}
/****************************************************************
		�֐����FLcd_aux_mode
		��  ���F����
		�߂�l�F����
		�@  �\�FAUXӰ�ޕ\������
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/27		0.1		ha		�V�K�쐬
****************************************************************/
static void Lcd_aux_mode(void)
{
	BYTE dmode;
	
	if (Ma_get_clock() == FALSE)
	{
		dmode = CLCD_BASE_AUX;
	}
	else
	{
		if(lcd_base_mode == CLCD_BASE_AUX)
		{
			dmode = CLCD_BASE_AUX;
		}
		else
		{
			dmode = CLCD_BASE_CLOCK;		/* �x�[�X�\���͎��v */
		}
	}
	switch(dmode)
	{
		case CLCD_BASE_AUX:
			memcpy(lcd_seg_buf,STR_AUX,6);
			break;
		case CLCD_BASE_CLOCK:
			Lcd_clock_mode();				/* ���v�\�� */
			break;
		default:
			break;
	}
}

/****************************************************************
		�֐����FLcd_clock_mode
		��  ���Fmode_id���eҲ�Ӱ��,CLCD_OFF_COLCK��OFFӰ��
		�߂�l�F����
		�@  �\�F���v�D��\��Ӱ�ޏ���
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/30		0.1		ha		�V�K�쐬
****************************************************************/
void Lcd_clock_mode(void)
{
	static TDP_CLK clkdisp;
	
	memset(&clkdisp,0,sizeof(clkdisp));		/* �ر */
	if (Ma_get_clock() == FALSE)
	{
		Lcd_blank_disp();
		return;
	}
	
	Clk_lcddata_set(&clkdisp);
	
	/* ����Ӱ�ޒ� */
	if(clkdisp.mode != CSBM_CLK_NORMAL)
	{
		if (Alm_mode_check() != OFF)
		{
			Lcd_blink_set(CLCD_BLK_ALARM,BLK_500MS);	/* ���v�_�� */
		}
		else
		{
			Lcd_blink_set(CLCD_BLK_CLOCK,BLK_500MS);	/* ���v�_�� */
		}
	}
	lcd_send_buf.record1.colon = ON;								/* ":"�_�� */
	Lcd_hextoascii( clkdisp.hour, &lcd_seg_buf[0] );
	Lcd_nodisp( &lcd_seg_buf[0],1 );						/* 0�Ȃ�\�����Ȃ� */
	Lcd_hextoascii( clkdisp.min, &lcd_seg_buf[2] );		/* ���ް� HEX to ASCII */
	if ((Alm_mode_check() == ON))
	{
		lcd_seg_buf[4] = 'A';					/* seg5��'A'��\������ */
	}
	Lcd_clkalm_indicator();
}

void Lcd_clkalm_indicator(void)
{
	BYTE ind;
	if (Alm_mode_check() == ON)
	{
		ind = Alm_indicator_set();
	}
	else
	{
		ind = Clk_indicator_set();
	}
	if((ind & CLCD_CL_IND_AM) != 0 )		/* "AM" */
	{
		lcd_send_buf.record1.am = ON;
	}
	if((ind & CLCD_CL_IND_PM) != 0 )		/* "PM" */
	{
		lcd_send_buf.record1.pm = ON;
	}
}
void	Lcd_blank_disp(void)
{
	Lcd_nodisp( &lcd_seg_buf[0],5 );
}

void	Lcd_area_disp(void)
{
	lcd_seg_buf[0] = 'A';
	lcd_seg_buf[1] = 'R';
	lcd_seg_buf[2] = 'E';
	lcd_seg_buf[3] = 'A';
	Lcd_nodisp( &lcd_seg_buf[4],1 );
}

#if 0 /* �g�p���Ȃ����߁A�폜 */
/****************************************************************
		�֐����FLcd_allseq_on
		��  ���F����
		�߂�l�F����
		�@  �\�F�S�_��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/08/20		0.1		ha		�V�K�쐬
****************************************************************/
void	Lcd_allseq_on(void)
{
	memset(lcd_send_buf.buf_byte,0xff,sizeof(lcd_send_buf.buf_byte));	/* ���M�o�b�t�@0�ر */
	
	lcd_send_buf.record.ctrl_1 = 0x00;	/* 0000���۰��ް� */
	lcd_send_buf.record.ctrl_2 = 0x09;	/* 10010000 */
	lcd_send_buf.record.ctrl_3 = 0x12;	/* 01001000 */		/* 1/3 Duty */

	lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;	/* 2ں��ޖڌŒ��ް� */
	lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;	/* 3ں��ޖڌŒ��ް� */
	
	memset(lcd_seg_buf,0xff,sizeof(lcd_seg_buf));
	
	flcd_send = ON;
}

/****************************************************************
		�֐����FLcd_allseq_off
		��  ���F����
		�߂�l�F����
		�@  �\�F�S����
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/08/20		0.1		ha		�V�K�쐬
****************************************************************/
void	Lcd_allseq_off(void)
{
	memset(lcd_send_buf.buf_byte,0x00,sizeof(lcd_send_buf.buf_byte));	/* ���M�o�b�t�@0�ر */
	
	lcd_send_buf.record.ctrl_1 = 0x00;	/* 0000���۰��ް� */
	lcd_send_buf.record.ctrl_2 = 0x09;	/* 10010000 */
	lcd_send_buf.record.ctrl_3 = 0x12;	/* 01001000 */		/* 1/3 Duty */

	lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;	/* 2ں��ޖڌŒ��ް� */
	lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;	/* 3ں��ޖڌŒ��ް� */
	
	memset(lcd_seg_buf,0x00,sizeof(lcd_seg_buf));
	
	flcd_send = ON;
}
#endif

#undef		_LCD_MODE_C_
