/******************************************************************************
	�N�����I���a����
	�v���W�F�N�g��	�F2012�NAMFM_RADIO���f��
	�t�@�C����		: aux_key.h
    ���W���[����	: AUX
	�@	�\			: AUX KEY�����w�b�_�[�t�@�C��
------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
******************************************************************************/
#ifndef	_AUX_KEY_H_
#define	_AUX_KEY_H_

#include    "../key/key_def.h"

#undef	EXT
#ifdef	_AUX_KEY_C_
#define	EXT
#else
#define	EXT	extern
#endif


/*----------------------------------------------------
  	�֐��v���g�^�C�v
----------------------------------------------------*/
EXT KEY_PROC_TBL * Aux_key_func_addr_get(void);
EXT KEY_CHG_TBL * Aux_keyCodeTable_addr_get(void);
EXT BYTE Aux_ModeID_get(void);

#endif
