/******************************************************************************
   �N�����I���a����
	�v���W�F�N�g��	: 	2012�NAM/FM���W�I�Ή����f���J��
	�t�@�C����		: 	aud_beep_func.h
	���W���[����	: 	Audio���W���[��
	�@�\�T��		: 	�u�U�[���䏈���̃w�b�_�[�t�@�C��
-------------------------------------------------------------------------------
�C������
	�ԍ�		���t		�S����		�֐���			�C�����e
	�y000�z		12/07/13	leejh						�V�K�쐬
********************************************************************************/
#ifndef		_AUD_BEEP_FUNC_H_
#define		_AUD_BEEP_FUNC_H_

#undef		EXT
#ifdef		_AUD_BEEP_FUNC_C_
#define		EXT
#else
#define		EXT extern
#endif

/*---------------------------------------------------------------------------*
 *	�v���g�^�C�v��`													 	 *
 *---------------------------------------------------------------------------*/
BOOL Aud_is_beep_mute( void );
void Aud_set_stop_beep( void );
BYTE Aud_get_beep_info( void );
void Aud_req_beep( BYTE );
void Aud_beep_main( void );
void Aud_init_beep_func( void );
#endif
