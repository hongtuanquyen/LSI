/********************************************************************************/
/*		tu_eeprom.h																*/
/*																				*/
/*		�v���W�F�N�g���FTuner													*/
/*		�t�@�C�����F															*/
/*		���W���[�����F															*/
/*		�@�\�F			EEPROM�֌W����											*/
/*																				*/
/*																				*/
/*																				*/
/*			�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z			*/
/*------------------------------------------------------------------------------*/
/*						2010.08.06		0.0			�x		�V�K�쐬			*/
/********************************************************************************/
#ifndef	_TU_EEPROM_H_
#define	_TU_EEPROM_H_

#include	"../eeprom/eeprom_ext.h"
#include	"tu_def.h"

#undef	EXT
#ifdef	_TU_EEPROM_C_
#define	EXT
#else
#define	EXT	extern
#endif

EXT	BYTE		tu_eepdata[CTU_EEPADR_PSTLEN];	/*	EEPROM�֕ۑ��f�[�^�o�b�t�@�A00811�A�x	*/
EXT	sEEP_PARA	tu_eepreq;						/*	EEPROM�֕ۑ��v���A100811�A�x	*/
EXT	BYTE		tu_eep_cnt;						/*	EEPROM�֕ۑ��J�E���^�[�A	*/

EXT	BYTE	Tu_ReadEepData(BYTE *savebuf,BYTE dest);
EXT	BYTE	Tu_ReadEepPara(BYTE *readbuf,BYTE dest);
EXT	BYTE	Tu_ReadEepRq(BYTE *rdbuf,BYTE offset,BYTE rdlen);
EXT	BYTE	Tu_WriteEepRq(BYTE *wrbuf,BYTE offset,BYTE wrlen);
EXT	void	Tu_WriteEepData(void);

/* QAC�΍� */
EXT	BYTE	Tu_CheckInvalidData(BYTE *checkbuf,WORD maxfm,WORD maxam);
EXT	BYTE	Tu_CheckInvalidPara(BYTE *chkbuf,BYTE destno);
EXT	void	Tu_SaveEepData(BYTE status,BYTE *BufAddr);
#endif
