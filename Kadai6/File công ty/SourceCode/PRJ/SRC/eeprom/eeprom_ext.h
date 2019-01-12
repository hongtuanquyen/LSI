/******************************************************************************
	�N�����I���i���j�a����		2007 C3X03 �ذ��[��]

		�v���W�F�N�g��	: '07�N�t���b�v�_�E���@MP3���f���J��
		�t�@�C����		: eeprom_ext.h
		�@�\			: EEPROM Module �O���Q�Ɗ֐��Q�֐��pͯ�ް�̧��
------------------------------------------------------------------------------
�C������
******************************************************************************/
#include	"../model.h"

#if 	_MODEL_EEPROM_
#ifndef	_EEPROM_EXT_H_
#define	_EEPROM_EXT_H_

#undef	EXT
#ifdef	_EEPROM_EXT_C_
#define	EXT
#else
#define	EXT	extern
#endif


#define		CEEP_ADDR_FACTCODE1		0x00
#define		CEEP_ADDR_FACTCODE2		0xFE
#define		CEEP_LENG_FACTCODE		2

#define		CEEP_ADDR_INIDATA		0x00
#define		CEEP_LENG_INIDATA		5
#define		CEEP_LENG_NUMBER		5

#define		CEEP_ADDR_AREA			0x02
#define		CEEP_LENG_AREA			1

#define		CEEP_ADDR_CLKALM		0x03
#define		CEEP_LENG_CLKALM		2

#define		CEEP_ADDR_TUNER			0x20
#define		CEEP_LENG_TUNER			48

#define		CEEP_ADDR_PCHAM			0x20
#define		CEEP_LENG_PCHAM			12

#define		CEEP_ADDR_PCHFM1		0x30
#define		CEEP_LENG_PCHFM1		12

#define		CEEP_ADDR_PCHFM2		0x40
#define		CEEP_LENG_PCHFM2		12

#define		CEEP_ADDR_ATOMIC		0x60
#define		CEEP_LENG_ATOMIC		96


/* AMFM���W�I�FEeprom��MAP��256BYTE�Ȃ̂ŁA�A�h���X�ƃ����O�X�̌^��BYTE�ɕύX */
#if 0
typedef	struct
{
	WORD	EepAddr;
	WORD	Length;
	BYTE	*BufAddr;
}sEEP_PARA;
#else
typedef	struct
{
	BYTE	EepAddr;
	BYTE	Length;
	BYTE	*BufAddr;
}sEEP_PARA;
#endif
EXT	void	EEP_WriteReq(sEEP_PARA	);
EXT	BYTE	EEP_ReadReq(sEEP_PARA	);
EXT	BYTE	EEP_CheckIDLE(void);
EXT	void	Eeprom_1ms_timer(void);
EXT	BOOL	eeprom_write_ok_chk(void);
EXT BYTE	EEP_WriteData_xBYTE( BYTE *buf, BYTE size );
EXT void	EEP_Initial_Req(void);
EXT void	EEP_Initial_para_Req(BYTE *buf,BYTE mode);
EXT void	EEP_Test_func(BYTE cmd,BYTE *addr,BYTE len);

#endif

#endif	/* _MODEL_EEPROM_ */
