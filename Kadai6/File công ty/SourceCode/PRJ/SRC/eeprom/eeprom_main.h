/******************************************************************************
	クラリオン（株）殿向け		2007 C3X03 ｼﾘｰｽﾞ[仮]

		プロジェクト名	: '07年フリップダウン　MP3モデル開発
		ファイル名		: pw_main.h
		機能			: ①電源制御ﾓｼﾞｭｰﾙ･ﾍｯﾀﾞｰ･ﾌｧｲﾙ
------------------------------------------------------------------------------
******************************************************************************/
#include	"../model.h"

#if 	_MODEL_EEPROM_
#ifndef	_EEPROM_MAIN_H_
#define	_EEPROM_MAIN_H_

#undef	EXT
#ifdef	_EEPROM_MAIN_C_
#define	EXT
#else
#define	EXT	extern
#endif

#define		CEEP_DEBUG		0


EXT	void	eeprom_main(void );
EXT	void	eeprom_initial(void);

/*----------------------------------------------------
   高速ｱｸｾｽ変数定義
----------------------------------------------------*/
EXT	TCOM_FLG	eep_flag;
#define		feep_lastwt			(eep_flag.bit.bit5)
#define		feep_power			(eep_flag.bit.bit4)
#define		feep_test_res		(eep_flag.bit.bit3)
#define		feep_initial_req	(eep_flag.bit.bit2)
#define		feep_reset			(eep_flag.bit.bit1)
/*----------------------------------------------------
  通常変数定義
----------------------------------------------------*/
#if (CEEP_DEBUG == ON)
EXT		WORD		eeprom_debug;
EXT		BYTE		eeprom_debugreq;
#endif
#if 0
EXT		BYTE	bEEP_FactCode[4];
#endif
EXT		BYTE	bEEP_inidata[12];

EXT		BYTE	bEEP_RstSeq;

EXT	BYTE	EEP_Drv_Read(void);
EXT	BYTE	EEP_Drv_Write(void);

EXT		BYTE	bEEP_Drv_Timer;

#define		CEEP_BUFMAX		250
EXT		BYTE	bEEP_DataBuf[CEEP_BUFMAX];

/* AMFMラジオ：EepromのMAPは256BYTEなので、アドレスとレングスの型はBYTEに変更 */
#if 0
EXT	struct
{
	BYTE	Busy;
	BYTE	ErrCnt;
	BYTE	*DataAddr;
	WORD	SlaveAddr;
	WORD	Length;
}sEEP_DRVWork;
#else
EXT	struct
{
	BYTE	Busy;
	BYTE	ErrCnt;
	BYTE	*DataAddr;
	BYTE	SlaveAddr;
	BYTE	Length;
}sEEP_DRVWork;
#endif

#endif

#endif	/* _MODEL_EEPROM_ */
