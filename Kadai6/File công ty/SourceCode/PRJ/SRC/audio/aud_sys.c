/******************************************************************************
   �N�����I���a����
	�v���W�F�N�g��	: 	2012�NAM/FM���W�I�Ή����f���J��
	�t�@�C����		:	aud_sys.c
	���W���[����	:	Audio���W���[��
	�@ �\ �T ��		:	�V�X�e�����甭�s���ꂽ���b�Z�[�W����
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�֐���		�C�����e
	�y000�z		2005.12.09	�]�c			-		�V�K�쐬
******************************************************************************/
#define		_AUD_SYS_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../main//MA_inc.h"

#include	"aud_ext.h"
#include	"aud_sys.h"
#include	"aud_def.h"
#include	"aud_main.h"

/******************************************************************************
	�֐���		�F	Aud_sys_message
	����		�F	BYTE	sys_msg:	�V�X�e���R�}���h
	�߂�l		�F	����
	�@�\�T��	�F	�V�X�e���R�}���h��M���C������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
void Aud_sys_message( BYTE sys_msg )
{
	switch ( sys_msg )
	{
		/* E-VOL���C������ */
		case CMSG_MAIN:
			Aud_msg_main();
			break;

		/* ���������� */
		case CMSG_INITIAL:
			Aud_msg_initial();
			break;

		/* MAINCNT ON�d������ */
		case CMSG_MAINCNT_ON:
			Aud_msg_maincnt_on();
			break;

		/* POWER ON�d������ */
		case CMSG_PWRON:
			Aud_msg_power_on();
			break;

		/* ���[�h�`�F���W���� */
		case CMSG_MODECHG:
			Aud_msg_mode_change();
			break;

		/* POWER OFF�d������ */
		case CMSG_PWROFF:
			Aud_msg_power_off();
			break;

		/* MAINCNT OFFREQ�d������ */
		case CMSG_MAINCNT_OFFREQ:
			Aud_msg_maincnt_off_req();
			break;

		/* MAINCNT OFF�d������ */
		case CMSG_MAINCNT_OFF:
			Aud_msg_maincnt_off();
			break;

		/* �^�C�}���� */
		case CMSG_25MS_TIMER:
			Aud_msg_timer( CAUD_EVT_TIMER_025M );
			break;
			
		default:
			break;
	}
}

#undef _AUD_SYS_C_

