/***************************************************************
	クラリオン殿向け
	プロジェクト名	：2012年AMFM_RADIOモデル
	ファイル名		：off_key.c
	モジュール名	：OFFモード
	機   能			：KEY処理関数ファイル
----------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
****************************************************************/
#define	_OFF_KEY_C_

#include	"../model.h"
#include	"../comm/common.h"
#include	"../main/ma_ext.h"
#include	"../audio/Aud_inc.h"
#include	"../key/key_func_ext.h"
#include	"../key/key_def.h"
#include	"../submode/SBM_inc.h"
#include	"../testmode/TEST_inc.h"
#include	"off_key.h"

#define		OFF_KEYEN_NOP		0x00

/******************************************************************************
		関数名  ：Off_keyCodeTable_addr_get
		引  数	：無し
		戻り値	：キーコード変換テーブルポインタ
		機  能	：OFFモードのキーコード変換テーブルポインタ取得
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
KEY_CHG_TBL *Off_keyCodeTable_addr_get(void)
{
	return ((KEY_CHG_TBL*)&off_key_chg_tbl);
}

/******************************************************************************
		関数名  ：Off_key_func_addr_get
		引  数	：無し
		戻り値	：キーテーブルポインタ
		機  能	：OFFモードのキー処理テーブルポインタ取得
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
KEY_PROC_TBL * Off_key_func_addr_get(void)
{
	return ((KEY_PROC_TBL*)&off_key_tbl);
}

/******************************************************************************
		関数名  ：Off_key_pw_long
		引  数	：無し
		戻り値	：no
		機  能	：power key long function
------------------------------------------------------------------------------
lirongjun20121119		新規作成
******************************************************************************/
void  Off_key_pw_long(void)
{
	if ((Ma_get_clock() == TRUE )
	&&(Pw_ACC_status_get() == ON))
	{
		Clk_adj_start();
	}
	Key_nop();
}
/******************************************************************************
		関数名  ：Off_key_alarm_xxx
		引  数	：無し
		戻り値	：no
		機  能	：alarm key function
------------------------------------------------------------------------------
chin20120821		新規作成
******************************************************************************/
void  Off_key_alarm_on(void)
{
	if (Ma_get_alarm() == FALSE )
	{
		Key_nop();
		return;
	}
	
	if (Pw_ACC_status_get() == OFF)
	{
		Key_long_time2000_accoff();
	}
	else
	{
		Key_long_time2000();
	}
}
void  Off_key_alarm_long(void)
{
	Alm_mode_start();
	//Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	//Pw_acc_wakeup_req(ON);
	Key_nop();
}
void  Off_key_alarm_off(void)
{
	Alm_onoff_set();
	Key_nop();
}
void  Off_key_testp1(void)
{
	Sct_key_test(CSBM_SCT_VER);
	Key_nop();
}

void  Off_key_testp2(void)
{
	/* alarm on */
	Sct_key_test(CSBM_SCT_BEEP);
	Key_nop();
}

void  Off_key_testp3(void)
{
	/* LOUD OFF  */
	Sct_key_test(CSBM_SCT_LOUDOFF);
	Key_nop();
}

#undef	_OFF_KEY_C_