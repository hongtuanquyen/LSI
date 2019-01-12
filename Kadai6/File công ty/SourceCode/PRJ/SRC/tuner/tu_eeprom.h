/********************************************************************************/
/*		tu_eeprom.h																*/
/*																				*/
/*		プロジェクト名：Tuner													*/
/*		ファイル名：															*/
/*		モジュール名：															*/
/*		機能：			EEPROM関係処理											*/
/*																				*/
/*																				*/
/*																				*/
/*			修正履歴	【年月日】	【Version】	【名前】 	【説明】			*/
/*------------------------------------------------------------------------------*/
/*						2010.08.06		0.0			岳		新規作成			*/
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

EXT	BYTE		tu_eepdata[CTU_EEPADR_PSTLEN];	/*	EEPROMへ保存データバッファ、00811、岳	*/
EXT	sEEP_PARA	tu_eepreq;						/*	EEPROMへ保存要求、100811、岳	*/
EXT	BYTE		tu_eep_cnt;						/*	EEPROMへ保存カウンター、	*/

EXT	BYTE	Tu_ReadEepData(BYTE *savebuf,BYTE dest);
EXT	BYTE	Tu_ReadEepPara(BYTE *readbuf,BYTE dest);
EXT	BYTE	Tu_ReadEepRq(BYTE *rdbuf,BYTE offset,BYTE rdlen);
EXT	BYTE	Tu_WriteEepRq(BYTE *wrbuf,BYTE offset,BYTE wrlen);
EXT	void	Tu_WriteEepData(void);

/* QAC対策 */
EXT	BYTE	Tu_CheckInvalidData(BYTE *checkbuf,WORD maxfm,WORD maxam);
EXT	BYTE	Tu_CheckInvalidPara(BYTE *chkbuf,BYTE destno);
EXT	void	Tu_SaveEepData(BYTE status,BYTE *BufAddr);
#endif
