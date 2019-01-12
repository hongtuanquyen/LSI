/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: main.c
		�@�\			: SystemҲ�ٰ�ߏ���
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051125]	�V�K�쐬
******************************************************************************/
#define	_MAIN_C_

#include 	<stdio.h>
#include 	<string.h>
#include	"../model.h"
#include	"../comm/common.h"
#include    "../audio/aud_sys.h"
#include    "../disp/lcd_sys.h"
#include    "../power/PW_inc.h"
#include    "../key/key_sys.h"
#include	"../io/IO_inc.h"
#include    "../tuner/tu_sys.h"
#include	"../submode/sbm_sys.h"
#include	"../testmode/test_sys.h"
#include    "../aux1/aux_sys.h"
#if 	_MODEL_EEPROM_
#include    "../eeprom/eeprom_ext.h"
#include    "../eeprom/eeprom_sys.h"	/* QAC�΍� */
#endif

#include	"main.h"
#include	"ma_ext.h"
#include	"ma_sys.h"

/*----------------------------------------------------
   �֐��v���g�^�C�v
----------------------------------------------------*/
static void	Ma_message_call(BYTE  msg);
static void	Ma_message_process(void);
static void	Ma_message_clear(void);
static void	Ma_cur_message_get(void);

/*----------------------------------------------------
   ���Ѻ���ގ�t�����o�^ð���
----------------------------------------------------*/
typedef	struct{
	void	(*func)(BYTE);
}TMA_SYSTBL;

const TMA_SYSTBL tma_sys_tbl[] =
{
	Key_sys_message,
	Test_sys_message,
	Sbm_sys_message,
	Tu_sys_message,
	Aux_sys_message,
	Aud_sys_message,
	Lcd_sys_message
};

const TMA_SYSTBL tma_sys_ini_ng_tbl[] =
{
	Key_sys_message,
	Test_sys_message,
	Aud_sys_message,
	Lcd_sys_message
};

/******************************************************************************
.. 		�֐���  �Fmain
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FSystemMainٰ�ߏ����A��϶��ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051128] �V�K�쐬
[sakurada060729] testVer��ؾ�ĉ�H��~�i _RESET_DISABLE_ �j
[sun070313] _RESET_DISABLE_��`�폜�ύX
[sun090325] EEPROM RESET��Mainٰ�߂��甲����悤�ɕύX
[yamamoto20120727] ؾ�ď����폜
[yamamoto20121112] WDT���ď����폜�AIo_sfr_initial_set���ōs��
******************************************************************************/
int	main(void)
{
	__DisableInterrupt();

	Io_sfr_initial_set();				/* ڼ޽������ݒ� WDT���� */
	
	Ma_message_clear();					/* ү�����ޯ̧������ */
	Ma_message_call(CMSG_INITIAL);		/* ������ү���ޔ��s */
	
	/*-EEPROM�Ή�-----------------------------*/
	MIO_OUT_pSYS_ON(LOW);				/* SYS_ON�[�q���� */
	Io_sfr_accoff_in();					/* 12M���U��~�ARC���U�J�n */
	/*----------------------------------------*/
	
	ma_cur_msg = CMSG_MAIN;				/* ����ү����Mainٰ�߂ɏ����� */

	__EnableInterrupt( 0 ) ;
	
	//Io_beep_test_8Hz_start();

	while(1)
	{
		Ma_message_process();					/* Mainٰ��ү���ޔ��s */

	}
	#if 0		/* ���r���[�w�E���� */
	__Reset();
	#endif
}

/******************************************************************************
.. 		�֐���  �FMa_message_process
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FSystemү���ޔ��s���䏈��
				�Fү�����ޯ̧���m�F���A���s����Ă��Ȃ�ү���ނ���Δ��s�A
				�F�Ȃ���΁AҲ�ٰ��ү���ނ𔭍s
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
******************************************************************************/
static void	Ma_message_process(void)
{
	void	(*sys_func)(BYTE);
	BYTE	iLoop;
	BYTE	iSize;
	BYTE	ini_sta;

	Pw_sys_message(ma_cur_msg);			/* �d��Ӽޭ�� */
	Ma_sys_message(ma_cur_msg);			/* MainӼޭ�� */
	#if (_MODEL_EEPROM_ == YES)
	Eeprom_sys_message(ma_cur_msg);		/* BATT OFF��EEPROM�̂ݓ���\ */
	#endif
	Ma_cur_message_get();				/* ���s����ލľ�� */
	
	if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_OK)||
		(Ma_Chk_EEP_Initial() == CMA_INIT_STA_CKAL_NG))
	{
		ini_sta = TRUE;
	}
	else
	{
		ini_sta = FALSE;
	}
	
	/* ���C�����[�v */
	if (ini_sta == TRUE)
	{
		iSize =  sizeof(tma_sys_tbl)/sizeof(TMA_SYSTBL);
		for (iLoop = 0; iLoop < iSize; iLoop ++)
		{
			sys_func = tma_sys_tbl[iLoop].func;
			
			(*sys_func)( ma_cur_msg );
		}
	}
	else
	{
		iSize =  sizeof(tma_sys_ini_ng_tbl)/sizeof(TMA_SYSTBL);
		for (iLoop = 0; iLoop < iSize; iLoop ++)
		{
			sys_func = tma_sys_ini_ng_tbl[iLoop].func;
			(*sys_func)( ma_cur_msg );
		}
	}
}
/******************************************************************************
.. 		�֐���  �FMa_message_set
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FSystemү���ޓo�^����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
[sun051213] ү���ޓo�^��A�߲���ݸ���Ă���悤�ɕύX
******************************************************************************/
static void	Ma_cur_message_get(void)
{
	if (ma_msg_outptr != ma_msg_inptr)
	{
		ma_cur_msg = ma_sysmsg[ma_msg_outptr];
		ma_msg_outptr ++;
		if( ma_msg_outptr >= CMA_MSGMAX )
		{
			ma_msg_outptr = 0;
		}
	}
	else
	{
		ma_cur_msg = CMSG_MAIN;
	}
}

/******************************************************************************
.. 		�֐���  �FMa_message_clear
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FSystemү���޸ر����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
******************************************************************************/
static void	Ma_message_clear(void)
{
	memset(ma_sysmsg,0x00,sizeof(ma_sysmsg));		/* ү���ފi�[�ޯ̧0�ر */
	ma_msg_inptr = 0;								/* ү���ފi�[�߲���ر */
	ma_msg_outptr = 0;								/* ү���ޔ��s�߲���ر */
}

/******************************************************************************
.. 		�֐���  �FMa_message_call
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�F����ү���ޒʒm����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051128] �V�K�쐬
[sun090401] ������EEPROM�K���擪�ɺ�ق���邱��
******************************************************************************/
static void	Ma_message_call(BYTE  msg)
{
	#if 	_MODEL_EEPROM_
	Eeprom_sys_message( msg );
	#endif
	Ma_sys_message( msg );				/* �����ݒ�ǂݍ��݂���̂ŁA�K����ԍŏ��ź�� */
	Pw_sys_message( msg );
	Test_sys_message( msg );
	Key_sys_message( msg );
	Sbm_sys_message( msg );
	Tu_sys_message( msg );
	Aux_sys_message( msg);
	Aud_sys_message( msg );
	Lcd_sys_message( msg );
}

/******************************************************************************
.. 		�֐���  �FMa_message_set
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FSystemү���ޓo�^����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
[sun051213] ү���ޓo�^��A�߲���ݸ���Ă���悤�ɕύX
******************************************************************************/
void	Ma_message_set(BYTE mode, BYTE  msg)
{
	/* HOLDXIN�ł���΁A����ү���ޑS���ر */
	if ((mode == CMA_POWER) && ( msg == CMSG_HOLDX_IN))
	{
		Ma_message_clear();
	}
	ma_sysmsg[ma_msg_inptr] = msg;

	ma_msg_inptr ++ ;

	if (ma_msg_inptr >= CMA_MSGMAX)
	{
		ma_msg_inptr = 0;
	}
}

#undef		_MAIN_C_
