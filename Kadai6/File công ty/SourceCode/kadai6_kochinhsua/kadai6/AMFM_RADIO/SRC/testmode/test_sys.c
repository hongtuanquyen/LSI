/***********************************************************************
	�N�����I���a���� 
	�v���W�F�N�g��	�FAMFM���W�I
	�t�@�C����		�Ftest_sys.c
	���W���[����	�F�e�X�g���[�h
	�@   �\			�FPC�Ƃ̒ʐM����
------------------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------
[chin20120931]	�V�K
***********************************************************************/
#include	"../model.h"

#if 	_TEST_MODE_

#define		_TEST_SYS_C_

#include    <string.h>
#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#include	"../key/key_func_ext.h"
#include	"../key/key_def.h"
#include	"../disp/lcd_ext.h"
#include	"../main/ma_ext.h"
#include	"../audio/aud_ext.h"
#include	"../audio/aud_def.h"
#include	"../power/pw_func_ext.h"
#include	"../tuner/TU_inc.h"
#if	_MODEL_EEPROM_
#include	"../eeprom/eeprom_ext.h"
#endif

#include	"TEST_inc.h"
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
void	Test_sys_message( BYTE msg )
{
	switch(msg)
	{
		case CMSG_INITIAL:
			Test_Mode_Initial();
			break;
		case CMSG_MAIN:
			Test_Mode_main();
			break;
		case CMSG_MAINCNT_OFF:
			Test_maincnt_off();
			break;
		case CMSG_MAINCNT_ON:
			Test_maincnt_on();
			break;
		case CMSG_10MS_TIMER:
			Test_10ms_timer();
			break;
		default:
			break;
	}
}

#undef		_TEST_SYS_C_

#endif	/* _TEST_MODE_ */
