/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: tu_sys.c
		�@�\			: �@Tuner Systemү���ގ�t����
						: �A��ϊ֌W����
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051125]	�V�K�쐬
[matu120926] �e�X�g���[�h�Ή�
[yamamoto130305] ST�`���^�����O���ԕύX
******************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_
#define		_TU_SYS_C_

#include	"../comm/common.h"
#include	"../main/ma_ext.h"

#include	"tu_sys.h"
#include	"tu_main.h"
#include	"tu_event.h"
#include	"tu_ext.h"
#include	"tu_key.h"
#include	"tu_io.h"

static	void	Tu_mode_check(BYTE mode);
static	void	Tu_sys_10ms(void);
static	void	Tu_sys_50ms(void);
static	void	Tu_sys_100ms(void);

/******************************************************************************
.. 		�֐���  �FTu_sys_message
		��  ��	�FSystemү����
		�߂�l	�F����
		�@  �\	�FTunerSystemү���ގ�t����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051125] �V�K�쐬
[matu120926] DX/LO�v���N���A�����ǉ�
******************************************************************************/
void	Tu_sys_message(BYTE	msg)
{
	switch(msg)
	{

		case	CMSG_MAIN:
			Tu_main();
			break;
		case	CMSG_10MS_TIMER:
			Tu_sys_10ms();
			break;
		case	CMSG_50MS_TIMER:
			Tu_sys_50ms();
			break;
		case	CMSG_100MS_TIMER:
			Tu_sys_100ms();
			break;
		case	CMSG_PWRON:
			Tu_mode_check(CMSG_PWRON);		/* TunerӼޭ�� Function On/Off�ؑ֏��� */
			break;
		case	CMSG_MODECHG:
			Tu_mode_check(CMSG_MODECHG);	/* TunerӼޭ�� Function On/Off�ؑ֏��� */
			break;
		case	CMSG_PWROFF:
		case	CMSG_MAINCNT_OFFREQ:
			Tu_mode_check(CMSG_PWROFF);		/* TunerӼޭ�� Function On/Off�ؑ֏��� */
			break;
		case	CMSG_INITIAL:
			Tu_initial();
			break;
		case	CMSG_MAINCNT_ON:
			tu_test_dxlo = CTU_TEST_NO;		/* DX/LO�v���N���A */
			break;
		default:
			break;
	}
}
/*------------------------------------------------------------------------------
.. 		�֐���  �FTu_mode_check
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTunerӼޭ��On/Off�ؑ֏���
-----------------------------------------------------------------------------
[sun060517] н1 FUNC OFF�s���������fн
[zhang070228]MIO_CHG_pST_DATA���폜
-----------------------------------------------------------------------------*/
static	void	Tu_mode_check(BYTE mode)
{
	if (mode == CMSG_PWROFF)
	{
		if (tu_pwrmode != CTU_PWROFF)
		{
			tu_pwrmode = CTU_PWROFF;
			Tu_e_func_off();
		}
	}
	else
	{
		if ((Ma_current_mode() == CMA_TUNER)
		    ||(Ma_current_mode() == CMA_ISR))
		{
			if (tu_pwrmode != CTU_FUNCON)
			{
				tu_pwrmode = CTU_FUNCON;
				Tu_e_func_on();
			}
			else
			{
				Tu_job_clear();
				if(Ma_current_mode() == CMA_ISR)				
				{
					tu_m_mode = CTU_M_ISRRD;					/* ISR READ */
				}
				else
				{
					Tu_lastcall();
					tu_m_mode = CTU_M_BANDCHG;					/*TUNER LAST READ */
				}
			}
		}
		else
		{
			if (tu_pwrmode != CTU_PWROFF)
			{
				tu_pwrmode = CTU_PWROFF;
				Tu_e_func_off();
			}
		}
	}
}
/*------------------------------------------------------------------------------
.. 		�֐���  �FTu_tim_cntdw()
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�F��϶����޳ݏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060408]	�V�K�쐬
[sun060421] �P�̊� [н1 ���ʕt���Y��]
-----------------------------------------------------------------------------*/
void Tu_tim_cntdw(BYTE	*time)
{
	if ((*time) > 0)
	{
		(*time) --;
	}
}
/*------------------------------------------------------------------------------
.. 		�֐���  �FTu_sys_10ms
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTunerӼޭ��10ms��ώ�������
-----------------------------------------------------------------------------*/
static	void	Tu_sys_10ms(void)
{
	ftu_stchk = TRUE;
	if (tu_aftermute > TIMER_OUT)
	{
		tu_aftermute --;
	}

	if (tu_10ms_timer > 0)
	{
		tu_10ms_timer --;
	}
}

/*------------------------------------------------------------------------------
.. 		�֐���  �FTu_sys_50ms
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTunerӼޭ��50ms��ώ�������
-----------------------------------------------------------------------------*/
static	void	Tu_sys_50ms(void)
{
	/* ��ڵ���o�֎~��� */
	Tu_tim_cntdw(&tu_stdistim);
}
/*------------------------------------------------------------------------------
.. 		�֐���  �FTu_sys_100ms
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTunerӼޭ��100ms��ώ�������
-----------------------------------------------------------------------------*/
static	void	Tu_sys_100ms(void)
{
	Tu_tim_cntdw(&tu_100ms_timer);
}
#undef  _TU_SYS_C_

#endif
