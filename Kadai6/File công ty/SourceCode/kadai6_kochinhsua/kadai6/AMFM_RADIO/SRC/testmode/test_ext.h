/******************************************************************************
	クラリオン（株）殿向け		

		プロジェクト名	: AMFMラジオ
		ファイル名		: test_ext.h
		機能			: testモードのI/Fを提供する
------------------------------------------------------------------------------
******************************************************************************/
#include	"../model.h"

#if 	_TEST_MODE_

#ifndef	_TEST_EXT_H_
#define	_TEST_EXT_H_

#undef	EXT
#ifdef	_TEST_EXT_C_
#define	EXT
#else
#define	EXT	extern
#endif


enum
{
	CTEST_MODE_TEST_ON = 1,				/* 1 */
	
	CTEST_MODE_POWERON,					/* 2 */
	CTEST_MODE_POWEROFF,				/* 3 */
	CTEST_MODE_SRC,						/* 4 */
	
	CTEST_MODE_AUD_DIR_VOL,				/* 5 */
	CTEST_MODE_AUD_BAL_RMAX,			/* 6 */
	CTEST_MODE_AUD_BAL_LMAX,			/* 7 */
	CTEST_MODE_AUD_BAL_CEN,				/* 8 */
	CTEST_MODE_AUD_BAS_MAX,				/* 9 */
	CTEST_MODE_AUD_BAS_MIN,				/* A */
	CTEST_MODE_AUD_TRE_MAX,				/* B */
	CTEST_MODE_AUD_TRE_MIN,				/* C */
	CTEST_MODE_AUD_BSTR_CEN,			/* D */
	CTEST_MODE_AUD_LOUD_ONOFF,			/* E */
	CTEST_MODE_AUD_MUTE_ONOFF,			/* F */
	
	CTEST_MODE_TUNER_DIR_PCH,			/* 10 */
	CTEST_MODE_TUNER_SEEKUP,			/* 11 */
	CTEST_MODE_TUNER_SEEKDW,			/* 12 */
	CTEST_MODE_TUNER_DIR_NCH,			/* 13 */
	CTEST_MODE_TUNER_LO_DX,				/* 14 */
	CTEST_MODE_TUNER_GET_STA,			/* 15 */

	CTEST_MODE_ATMIC_IC,				/* 16 */
	CTEST_MODE_ATMIC_EEP,				/* 17 */
	
	CTEST_MODE_OTHER_GET_PORT,			/* 18 */
	CTEST_MODE_OTHER_BEEP_ONOFF,		/* 19 */
	CTEST_MODE_OTHER_STBY_ONOFF,		/* 1A */
	CTEST_MODE_OTHER_CLK_OUTPUT,		/* 1B */
	CTEST_MODE_TUNER_INI_PCH,			/* 1C */
	
	CTEST_MODE_LCD_TEST,				/* 1D */
	CTEST_MODE_KEY_TEST,				/* 1E */
	CTEST_MODE_OTHER_GET_VER,			/* 1F */
	
	CTEST_MODE_MAX						/* MAX */
};
#define		CTEST_MODE_SRC_TUNER_EXTR1		0x07
#define		CTEST_MODE_SRC_TUNER_AM_EXTR2		0x11
#define		CTEST_MODE_SRC_TUNER_FM_EXTR2		0x01

#define		CTEST_MODE_SRC_AUX_EXTR1		0x7E



EXT void Test_data_reply(BYTE cmd,BYTE para,BYTE *adr,BYTE len);
EXT void Test_Mode_main(void);
EXT void Test_Mode_Int_Snd(void);
EXT void Test_Mode_Int_Rcv(void);
EXT void Test_Mode_Initial(void);
EXT void Test_10ms_timer(void);




#endif	/*	_TEST_EXT_H_	*/

#endif	/*	_TEST_MODE_	*/
