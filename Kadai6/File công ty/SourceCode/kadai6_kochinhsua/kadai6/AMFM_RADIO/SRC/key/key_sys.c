/******************************************************************************
	�N�����I���a����
	�v���W�F�N�g��	: 2012�NAMFM_RADIO���f��
	�t�@�C����		: key_sys.c
	�@�\			: Key���W���[��System���b�Z�[�W��t����
------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
******************************************************************************/
#define		_KEY_SYS_C_

#include	"../comm/common.h"
#include	"key_def.h"
#include	"key_func_ext.h"
#include	"key_sys.h"
#include	"key_main.h"

static	void	Key_sys_25ms(void);
static	void	Key_sys_50ms(void);

/******************************************************************************
		�֐���  �FKey_sys_message
		��  ��	�FSystem���b�Z�[�W
		�߂�l	�F����
		�@  �\	�FKey���W���[�� System���b�Z�[�W��t����
------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
******************************************************************************/
void	Key_sys_message(BYTE msg)
{
	switch(msg)
	{
		case	CMSG_MAIN:
			Key_main();
			break;
		case	CMSG_INITIAL:
		case	CMSG_MAINCNT_OFFREQ:
			Key_initial();
			break;
		case	CMSG_MODECHG:
			Key_nop();						/* ���[�h�ؑ֒�������Ă���L�[�����ɂ��� */
			break;
		case	CMSG_25MS_TIMER:
			Key_sys_25ms();
			break;
		default:
			break;
	}
	return;
}

/******************************************************************************
	�֐���	�FKey_sys_25ms
	��  ��	�F����
	�߂�l	�F����
	�@  �\	�FKey���W���[��25ms�����^�C�}����
------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
******************************************************************************/
static	void	Key_sys_25ms(void)
{
	if (key_in_disable_timer  != CKEY_TIME_UP)
	{
		key_in_disable_timer--;
	}
	if (key_long_timer != 0xff)
	{
		if(key_long_timer > CKEY_TIME_UP)
		{
			key_long_timer--;
		}
	}
	return;
}

#undef _KEY_SYS_C_