/******************************************************************************
	�N�����I���i���j�a����		2007 C3X03 �ذ��[��]

		�v���W�F�N�g��	: '07�N�t���b�v�_�E���@MP3���f���J��
		�t�@�C����		: eeprom_sys.c
------------------------------------------------------------------------------
�C������
******************************************************************************/
#include	"../model.h"
#if 	_MODEL_EEPROM_
#define		_EEPROM_SYS_C_

#include	"../comm/common.h"
#include	"../io/IO_ext.h"
#include	"eeprom_main.h"
#include	"eeprom_ext.h"
#include	"eeprom_sys.h"

/******************************************************************************
.. 		�֐���  �FEeprom_sys_message
		��  ��	�FSystemү����
		�߂�l	�F����
		�@  �\	�FEEPROM Systemү���ގ�t����
------------------------------------------------------------------------------
�C������
******************************************************************************/
void	Eeprom_sys_message(BYTE	msg)
{
	switch(msg)
	{
		case	CMSG_MAIN:
			eeprom_main();
			break;
		case	CMSG_MAINCNT_ON:
			/* �����l�F*/
			/* SYS_ON��HIGH�ɂȂ��Ă���AEEPROM�̓d����HIGH�ɂȂ�ieeprom��Pin8�j�܂�300ns */
			/* SYS_ON��HIGH�ɂȂ��Ă���ACMSG_MAINCNT_ON������܂�90��m */
			/* ����āA�����Ń��Z�b�g�������Ă��� */
			IO_EEP_Port_Reset();
			break;
		case	CMSG_INITIAL:
			eeprom_initial();
			break;
		default:
			break;
	}
}


#undef  _EEPROM_SYS_C_
#endif	/* _MODEL_EEPROM_ */
