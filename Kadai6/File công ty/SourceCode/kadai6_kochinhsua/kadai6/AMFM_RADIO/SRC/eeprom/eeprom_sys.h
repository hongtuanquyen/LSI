/******************************************************************************
	�N�����I���i���j�a����		2007 C3X03 �ذ��[��]

		�v���W�F�N�g��	: '07�N�t���b�v�_�E���@MP3���f���J��
		�t�@�C����		: eeprom_sys.h
		�@�\			: �@EEPROM����Ӽޭ�٥System Message�֐��Q�@ͯ�ް�̧��
------------------------------------------------------------------------------
�C������
******************************************************************************/
#include	"../model.h"
#if 	_MODEL_EEPROM_
#ifndef	_EEPROM_SYS_H_
#define	_EEPROM_SYS_H_

#undef	EXT
#ifdef	_EEPROM_SYS_C_
#define	EXT   
#else
#define	EXT	extern
#endif

EXT	void	Eeprom_sys_message(BYTE );

#endif
#endif	/* _MODEL_EEPROM_ */
