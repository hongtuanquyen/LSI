/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: ma_tim.c
		�@�\			: Ҳ�Ӽޭ���ް���϶��ď���
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051125]	�V�K�쐬
******************************************************************************/
#define		_MA_TIM_C_

#include	"../model.h"
#include	"../comm/common.h"
#include	"../disp/lcd_ext.h"
#include	"../power/pw_func_ext.h"
#include	"../key/key_func_ext.h"
#include	"../audio/aud_ext.h"
#include	"../io/IO_inc.h"

#include	"../tuner/tu_ext.h"
#if 	_MODEL_EEPROM_
#include	"../eeprom/eeprom_ext.h"
#endif
#include	"ma_sys.h"
#include	"ma_ext.h"
#include	"ma_tim.h"

/******************************************************************************
.. 		�֐���  �FMa_tim_main
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FSystem��϶��ď����A���ү���ޔ��s����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051128] �V�K�쐬
[sun070228] 10ms��ϒǉ�
******************************************************************************/
void	Ma_tim_main(void)
{
	if (fma_tim_10ms == ON)
	{
		fma_tim_10ms = OFF;
		Ma_set_Msg(CMA_TIMER,CMSG_10MS_TIMER);		/* 10msTimerү���ޓo�^ */
	}
	if ( fma_tim_25ms == ON )						/* 25ms�o�� */
	{
		fma_tim_25ms = OFF;							/* 25ms�o���׸޸ر */

		Ma_set_Msg(CMA_TIMER,CMSG_25MS_TIMER);		/* 25msTimerү���ޓo�^ */
		
		if ( (--ma_tim_50ms_cnt) == TIMER_OUT )
		{
			ma_tim_50ms_cnt = CTIM_50MS_CNT;
			Ma_set_Msg(CMA_TIMER,CMSG_50MS_TIMER);	/* 50msTimerү���ޔ��s */
		}
		if ( (--ma_tim_100ms_cnt) == TIMER_OUT )
		{
			ma_tim_100ms_cnt = CTIM_100MS_CNT;
			Ma_set_Msg(CMA_TIMER,CMSG_100MS_TIMER);	/* 100msTimerү���ޔ��s */
		}
	}
}

/******************************************************************************
.. 		�֐���  �FMa_tim_int_1ms
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�F1ms��ϊ����ݏ���
		�R�����g�F2ms�A4ms�A5ms�A10ms����ϊ����ݏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051128] �V�K�쐬
[sun090307]USB���߲�SW�ǉ�
******************************************************************************/
void	Ma_tim_int_1ms(void)
{
	/* 1ms�����ݒ�����Ɋ��荞��ł����A���������ɔ����� */
	if (fma_tim_in1ms == TRUE)
	{
		return;
	}

	fma_tim_in1ms = TRUE;						/* 1ms�����ݏ����� */

	//1ms �����ݏ���
	#if (_MODEL_RADIO_ == YES)
	Tu_1ms_timer();
	#endif
	
	Pw_1ms_timer();
	Key_1ms_timer();
	#if 	_MODEL_EEPROM_
	Eeprom_1ms_timer();
	#endif

	fma_tim_2ms = ~fma_tim_2ms;					/* 2ms�׸ޔ��] */
	if (fma_tim_2ms == TRUE)
	{
		//2ms �����ݏ���
		Pw_2ms_timer();
	}

	if (--ma_tim_5ms_cnt == TIMER_OUT)
	{
		ma_tim_5ms_cnt = CTIM_5MS_CNT;
		//5ms�����ݏ���
		Key_5ms_timer();
		Aud_event_5ms();
		Tu_5ms_timer();							/* Tuner 5ms��Ϗ��� */
		Pw_5ms_timer();
	}
	if (--ma_tim_10ms_cnt == TIMER_OUT)
	{
		ma_tim_10ms_cnt = CTIM_10MS_CNT ;
		//10ms�����ݏ���
		Lcd_10ms_cnt();						/* LCD10ms��Ϗ��� */
		Aud_event_10ms();
		Pw_10ms_timer();
		Ma_10ms_timer();
		
		fma_tim_10ms = ON;					/* [sun070228]10ms�ǉ� */
	}
	if (--ma_tim_25ms_cnt == TIMER_OUT)
	{
		ma_tim_25ms_cnt = CTIM_25MS_CNT;
		//25ms�����ݏ���
		fma_tim_25ms = ON;
	}

	fma_tim_in1ms = FALSE;						/* 1ms�����ݏ����I�� */

}

#undef		_MA_TIM_C_
