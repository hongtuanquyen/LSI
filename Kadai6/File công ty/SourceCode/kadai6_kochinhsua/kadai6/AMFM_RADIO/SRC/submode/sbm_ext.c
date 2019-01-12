/*************************************************************************
	クラリオン殿向け 
	プロジェクト名	：2012年AMFM_RADIOﾓﾃﾞﾙ
	ファイル名		：sbm_ext.c
	モジュール名	：SUBモード
	機   能			：インタフェース関数ファイル
--------------------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
--------------------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
*************************************************************************/
#define		_SBM_EXT_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../key/key_func_ext.h"
#include	"../audio/aud_ext.h"
#include	"../key/key_def.h"
#include	"../main/ma_ext.h"
#include	"../power/pw_func_ext.h"
#include	"../testmode/TEST_inc.h"

#include	"sbm_def.h"
#include	"sbm_ext.h"
#include	"sbm_keytbl.h"

/******************************************************************************
		関数名  ：sbm_current_mode()
		引  数	：無し
		戻り値	：有効BIT情報
		機  能	：現有効bit通知
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
BYTE Sbm_current_mode(void)
{
/*----------------------------------------------------
	bit 7,	6,	5,	4,	3,	2,	1,	0
	|		|	|	|	|	|	|	|
	|		|	|	|	|	|	|	----AUDIO調整ﾓｰﾄﾞ
	|		|	|	|	|	|	--------時計調整ﾓｰﾄﾞ
	|		|	|	|	|	------------ALARM調整モード
	|		|	|	|	----------------ALARM起動中
	|		|	|	--------------------VOL調整モード
	|		|	------------------------POWER OFF
	|		----------------------------無し
	------------------------------------無し
--------------------------------------------------------*/
	BYTE sbit;
	
	sbit = 0;
	if( fsbm_alm_beepon != OFF )
	{
		sbit |= CSBM_BIT_BEP;
	}
	else if( Vol_mode_check() != OFF )
	{
		sbit |= CSBM_BIT_VOL;
	}
	else if( Sef_mode_check() != OFF )
	{
		sbit |= CSBM_BIT_AUD;
	}
	else if( Clk_adj_check() != OFF )
	{
		sbit |= CSBM_BIT_CLK;
	}
	else if( Alm_mode_check() != OFF )
	{
		sbit |= CSBM_BIT_ALM;
	}
	else
	{
		;
	}
	
	if( Pw_Power_status_get() == OFF )
	{
		sbit |= CSBM_BIT_PWR;
	}
	return(sbit);
}
/******************************************************************************
		関数名  ：Sbm_mode_check()
		引  数	：無し
		戻り値	：ON→SUBﾓｰﾄﾞ中、OFF→SUBﾓｰﾄﾞではない
		機  能	：SUBﾓｰﾄﾞ中かどうかﾁｪｯｸ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
BYTE Sbm_mode_check(void)
{
	if( fsbm_alm_beepon != OFF )
	{
		return(CSBM_BEP);
	}
	else if( Vol_mode_check() != OFF )
	{
		return(CSBM_VOL);
	}
	else if( Sef_mode_check() != OFF )
	{
		return(CSBM_SEF);
	}
	else if( Clk_adj_check() != OFF )
	{
		return(CSBM_CLK);
	}
	else if( Alm_mode_check() != OFF )
	{
		return(CSBM_ALM);
	}
#if _TEST_MODE_
	else if((Sct_mode_check() == CSBM_SCT_VER)||(Sct_mode_check() == CSBM_SCT_BEEP))
	{
		return(CSBM_TES);
	}
#endif
	else
	{
		return(CSBM_OFF);
	}
}

/******************************************************************************
		関数名  ：Sbm_mode_cancel()
		引  数	：ModeID
		戻り値	：無し
		機  能	：SUBﾓｰﾄﾞのｷｬﾝｾﾙ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
void Sbm_mode_cancel(BYTE mode_id)
{
	BYTE mode;
	
	mode = Ma_current_mode();		/* ｶﾚﾝﾄﾓｰﾄﾞ */
	if((mode == mode_id) || (mode_id == CSBM_OFF))				/* ｶﾚﾝﾄﾓｰﾄﾞ以外処理しない */
	{
		#if 0
		if( fsbm_alm_beepon != OFF )
		{
			fsbm_alm_beepon = OFF;
			/* beep stop処理 */
		}
		#endif
		
		if( Vol_mode_check() != OFF )
		{
			Vol_mode_cancel();
		}
		
		if( Sef_mode_check() != OFF )
		{
			Sef_mode_cancel();
		}
		
		if( Clk_adj_check() != OFF )
		{
			Clk_adj_cancel();
		}
		
		if( Alm_mode_check() != OFF )
		{
			Alm_mode_cancel();
		}
		
#if _TEST_MODE_
		if(Sct_mode_check() != OFF)
		{
			Sct_mode_cancel();
		}
#endif
	}
}
/******************************************************************************
		関数名  ：Off_keyCodeTable_addr_get
		引  数	：無し
		戻り値	：KEYcode変換ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
		機  能	：サプﾓｰﾄﾞのｷｰ処理ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ取得
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
KEY_CHG_TBL *Sbm_keyCodeTable_addr_get(void)
{
	if( fsbm_alm_beepon != OFF )
	{
		return((KEY_CHG_TBL*)&beep_key_chg_tbl);
	}
	else if((Alm_mode_check() != OFF)&&(Ma_get_alarm() == TRUE ))
	{
		if (Pw_ACC_DET_get() == ON)
		{
			return((KEY_CHG_TBL*)&alm_key_chg_tbl);
		}
		else
		{
			return((KEY_CHG_TBL*)&alm_key_chg_accoff_tbl);
		}
	}
	else if ((Clk_adj_check() != OFF)&&(Ma_get_clock() == TRUE ))
	{
		return((KEY_CHG_TBL*)&clk_key_chg_tbl);
	}
	else if (Sef_mode_check() != OFF)
	{
		return ((KEY_CHG_TBL*)&sef_key_chg_tbl);
	}
#if _TEST_MODE_
	else
	{
		return((KEY_CHG_TBL*)&Sct_key_chg_nor_tbl);
	}
#endif
}

/******************************************************************************
		関数名  ：Sbm_key_func_addr_get()
		引  数	：無し
		戻り値	：ｷｰ機能処理ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
		機  能	：SUBﾓｰﾄﾞのｷｰ処理ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ取得
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
KEY_PROC_TBL *Sbm_key_func_addr_get(void)
{
	#if 0
	if(( fsbm_alm_beepon != OFF )||(Sct_mode_check() == CSBM_SCT_BEEP))
	#else
	if( fsbm_alm_beepon != OFF )
	#endif
	{
		return ((KEY_PROC_TBL*)&beep_key_tbl);
	}
	else if((Alm_mode_check() != OFF)&&(Ma_get_alarm() == TRUE ))
	{
		if (Pw_ACC_DET_get() == ON)
		{
			return ((KEY_PROC_TBL*)&alm_key_tbl);
		}
		else
		{
			return((KEY_PROC_TBL*)&alm_key_accoff_tbl);
		}
	}
	else if ((Clk_adj_check() != OFF)&&(Ma_get_clock() == TRUE ))
	{
		return ((KEY_PROC_TBL*)&clk_key_tbl);
	}
	else if (Sef_mode_check() != OFF)
	{
		return ((KEY_PROC_TBL*)&sef_key_tbl);
	}
#if _TEST_MODE_
	else
	{
		return ((KEY_PROC_TBL*)&Sct_key_nor_tbl);
	}
#endif
}

/******************************************************************************
		関数名  ：Sbm_key_volupon
		引  数	：無し
		戻り値	：無し
		機  能	：VOLUPｷｰON処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
void Sbm_key_volupon(void)
{
	Key_long_time1000();				/* 1000msﾀｲﾏ設定 */
}
/******************************************************************************
		関数名  ：Sbm_key_voluplong
		引  数	：無し
		戻り値	：無し
		機  能	：VOLUPｷｰLONG処理
		修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
void Sbm_key_voluplong(void)
{
	fsbm_vol_mode = ON;
	sbm_vol_tim = CSBM_VOL_7S;
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);
	Aud_set_vol_step(UP);
	Key_long_time200();
}
/******************************************************************************
		関数名  ：Sbm_key_volupoff
		引  数	：無し
		戻り値	：無し
		機  能	：VOLUPｷｰOFF処理
		修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
void Sbm_key_volupoff(void)
{
	fsbm_vol_mode = ON;
	sbm_vol_tim = CSBM_VOL_7S;
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);
	Aud_set_vol_step(UP);
	Key_nop();
}
/******************************************************************************
		関数名  ：Sbm_key_voldwon
		引  数	：無し
		戻り値	：無し
		機  能	：VOLDWｷｰON処理
		修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
void Sbm_key_voldwon(void)
{
	Key_long_time1000();				/* 1000msﾀｲﾏ設定 */
}
/******************************************************************************
		関数名  ：Sbm_key_voldwlong
		引  数	：無し
		戻り値	：無し
		機  能	：VOLDWｷｰLONG処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
void Sbm_key_voldwlong(void)
{
	fsbm_vol_mode = ON;
	sbm_vol_tim = CSBM_VOL_7S;
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);
	Aud_set_vol_step(DW);
	Key_long_time200();
}
/******************************************************************************
		関数名  ：Sbm_key_voldwoff
		引  数	：無し
		戻り値	：無し
		機  能	：VOLDWｷｰOFF処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
void Sbm_key_voldwoff(void)
{
	fsbm_vol_mode = ON;
	sbm_vol_tim = CSBM_VOL_7S;
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);
	Aud_set_vol_step(DW);
	Key_nop();
}

/******************************************************************************
		関数名  ：Vol_lcddata_set
		引  数	：無し
		戻り値	：無し
		機  能	：VOL調整ﾓｰﾄﾞ表示ﾃﾞｰﾀ作成処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
BYTE Vol_lcddata_set(void)
{
	return (Aud_get_vol_step());
}
/******************************************************************************
		関数名  ：Vol_mode_check
		引  数	：無し
		戻り値	：無し
		機  能	：VOLﾓｰﾄﾞ中かどうかﾁｪｯｸ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
BYTE Vol_mode_check(void)
{
	return (fsbm_vol_mode);
}

/******************************************************************************
		関数名  ：Vol_mode_cancel
		引  数	：無し
		戻り値	：無し
		機  能	：VOLﾓｰﾄﾞのｷｬﾝｾﾙ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
******************************************************************************/
void Vol_mode_cancel(void)
{
	fsbm_vol_mode = OFF;
	sbm_vol_tim = 0;
}
/******************************************************************************
		関数名  ：Sbm_mode_initial
		引  数	：無し
		戻り値	：無し
		機  能	：SUBモード変数初期化処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/30		0.1		lrj		新規
******************************************************************************/
void Sbm_mode_initial(void)
{
	sbm_flag.byte = 0;
	sbm_vol_tim = 0;
	Sef_mode_initial();
	Clk_mode_initial();
	Alm_mode_initial();
}

void Sbm_BeepOn_Power(void)
{
	#if 1		/* 社内OILNo.41対策 */
	if (Pw_ACC_DET_get() == ON)
	#else
	if (Pw_Power_Key_get() == ON)
	#endif
	{
		Pw_power_key();
	}
	Alm_beep_key_stop();
	Key_nop();
}

/******************************************************************************
		関数名  ：Sct_mode_check
		引  数	：無し
		戻り値	：	CSBM_SCT_OFF：隠しキーモード OFF
				：	以外：隠しキー動作中
		機  能	：隠しキーモード中かﾁｪｯｸ
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
			2007/4/17		0.1		陳		新規
******************************************************************************/
BYTE Sct_mode_check(void)
{
	return(sbm_sct_mode);
}
/******************************************************************************
		関数名  ：Sct_mode_cancel
		引  数	：無し
		戻り値	：無し
		機  能	：STOPﾓｰﾄﾞOFF
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
			2007/4/17		0.1		陳		新規
******************************************************************************/
void Sct_mode_cancel(void)
{
	if (sbm_sct_mode != CSBM_SCT_OFF)
	{
		sbm_sct_mode = CSBM_SCT_OFF;
	}
	if (Vol_mode_check() == ON)
	{
		Vol_mode_cancel();
	}
}

void Sct_key_pwr_on(void)
{
	Pw_power_key();
	Key_nop();
}
void Sct_key_Ver_dispoff(void)
{
	Sct_mode_cancel();
	Key_nop();
}
/******************************************************************************
		関数名  ：Sct_key_test
		引  数	：無し
		戻り値	：無し
		機  能	：TESTｷｰOn処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
			2007/4/17		0.1		陳		新規
******************************************************************************/
void Sct_key_test(BYTE mode)
{
	if (sbm_sct_mode == CSBM_SCT_OFF)
	{
		sbm_sct_mode = mode;
		switch(sbm_sct_mode)
		{
			case CSBM_SCT_BEEP:
				Aud_set_beep(CAUD_REQ_BEEP_MULTIPLE);
				break;
			
			case CSBM_SCT_LOUDOFF:
				Aud_secret_loud_key();
				break;
			
			default :
				break;
		}
	}
	else 
	{
		Sct_mode_cancel();
	}
	Pw_power_key();
	Key_nop();
}

void	Sbm_key_tisc_on(void)
{
	BYTE	destno;
	
	destno = Ma_model_dest();/*仕向けNo取得*/
	
	switch(destno)
	{
		case CMA_MODEL_AMFM_J:
		case CMA_MODEL_AM_J:
			Tu_key_isr_on();/*TIモードへ遷移*/
			break;
		
		default:
			Key_nop();/*無効*/
			break;
	}
}

#if 0	/* 使用しないため、削除 */
/******************************************************************************
		関数名  ：Sbm_indicator_set
		引  数	：無し
		戻り値	：無し
		機  能	：SUBモードｲﾝｼﾞｹｰﾀ表示ﾃﾞｰﾀ処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/31		0.1		lrj		新規
******************************************************************************/
BYTE Sbm_indicator_set(void)
{
	BYTE	indicator;
	
	indicator = 0;
	
	if( fsbm_clk_ampm == CSBM_CLK_AM )
	{
		indicator |= CLCD_CL_IND_AM;
	}
	else
	{
		indicator |= CLCD_CL_IND_PM;
	}
	
	if( fsbm_alm_onoff == ON )
	{
		indicator |= CLCD_CL_IND_AL;
	}
	
	return(indicator);
}
#endif

#undef		_SBM_EXT_C_
