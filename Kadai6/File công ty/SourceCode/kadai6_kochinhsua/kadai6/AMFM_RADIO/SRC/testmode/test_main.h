/******************************************************************************
	クラリオン（株）殿向け		2007 C3X03 ｼﾘｰｽﾞ[仮]

		プロジェクト名	: '07年フリップダウン　MP3モデル開発
		ファイル名		: pw_main.h
		機能			: ①電源制御ﾓｼﾞｭｰﾙ･ﾍｯﾀﾞｰ･ﾌｧｲﾙ
------------------------------------------------------------------------------
******************************************************************************/
#include	"../model.h"

#if 	_TEST_MODE_

#ifndef	_TEST_MAIN_H_
#define	_TEST_MAIN_H_

#undef	EXT
#ifdef	_TEST_MAIN_C_
#define	EXT
#else
#define	EXT	extern
#endif



#define		CTEST_REPLY_MAX		19

EXT BYTE TestReplyBuf[CTEST_REPLY_MAX];
EXT BYTE TestReplyCmd;
EXT BYTE TestReplyPara;
EXT BYTE TestReplyLen;

EXT	TCOM_FLG 	test_flag;
#define   ftest_rcv_ok			test_flag.bit.bit1
#define   ftset_started			test_flag.bit.bit4
#define   ftest_result			test_flag.bit.bit5
#define   ftest_sending			test_flag.bit.bit6

void Test_maincnt_off(void );
void Test_maincnt_on(void );


#endif	/*	_TEST_MAIN_H_	*/

#endif	/*	_TEST_MODE_	*/
