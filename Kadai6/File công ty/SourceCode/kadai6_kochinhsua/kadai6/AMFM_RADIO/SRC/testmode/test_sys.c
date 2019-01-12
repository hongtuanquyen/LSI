/***********************************************************************
	クラリオン殿向け 
	プロジェクト名	：AMFMラジオ
	ファイル名		：test_sys.c
	モジュール名	：テストモード
	機   能			：PCとの通信制御
------------------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------
[chin20120931]	新規
***********************************************************************/
#include	"../model.h"

#if 	_TEST_MODE_

#define		_TEST_SYS_C_

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
/****************************************************************
		関数名：Sbm_sys_message
		引  数：Systemメッセージ
		戻り値：無し
		機  能：SbmSystemﾒｯｾｰｼﾞ受付処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/23		0.1		lrj		新規
			2012/7/30		0.2		lrj		initialとmodechgの処理修正
****************************************************************/
void	Test_sys_message( BYTE msg )
{
	switch(msg)
	{
		case CMSG_INITIAL:
			Test_Mode_Initial();
			break;
		case CMSG_MAIN:
			Test_Mode_main();
			break;
		case CMSG_MAINCNT_OFF:
			Test_maincnt_off();
			break;
		case CMSG_MAINCNT_ON:
			Test_maincnt_on();
			break;
		case CMSG_10MS_TIMER:
			Test_10ms_timer();
			break;
		default:
			break;
	}
}

#undef		_TEST_SYS_C_

#endif	/* _TEST_MODE_ */
