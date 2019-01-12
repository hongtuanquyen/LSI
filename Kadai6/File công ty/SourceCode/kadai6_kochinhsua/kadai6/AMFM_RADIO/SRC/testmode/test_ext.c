/***********************************************************************
	クラリオン殿向け 
	プロジェクト名	：AMFMラジオ
	ファイル名		：test_ext.c
	モジュール名	：テストモード
	機   能			：PCとの通信制御
------------------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------
[chin20120931]	新規
***********************************************************************/
#include	"../model.h"

#if 	_TEST_MODE_


#define		_TEST_EXT_C_


#include    <string.h>
#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#include	"../key/key_func_ext.h"
#include	"../key/key_def.h"
#include	"../disp/lcd_ext.h"
#include	"../main/ma_ext.h"
#include	"../audio/aud_ext.h"
#include	"../audio/aud_def.h"
#include	"../power/pw_func_ext.h"
#include	"../tuner/TU_inc.h"
#if	_MODEL_EEPROM_
#include	"../eeprom/eeprom_ext.h"
#endif

#include	"TEST_inc.h"


/******************************************************************************
		関数名  ：Test_data_reply
		引  数	：cmd⇒コマンド
				：para⇒パラメータ
				：adr⇒返信バッファのアドレス
				：len⇒返信データのレングス
		戻り値	：	無し
		機  能	：各モジュールが返信データをTestモードに渡す
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[chin20120930]
******************************************************************************/
void Test_data_reply(BYTE cmd,BYTE para,BYTE *adr,BYTE len)
{
	memset( TestReplyBuf,0x00,sizeof(TestReplyBuf));
	ftest_result = OK;
	TestReplyCmd = cmd;
	TestReplyPara = para;
	TestReplyLen = len;
	if(len <= CTEST_REPLY_MAX)
	{
		memcpy( TestReplyBuf,adr,len);
	}
}

#undef		_TEST_EXT_C_

#endif	/* _TEST_MODE_ */
