/*************************************************************************
	クラリオン殿向け 
	プロジェクト名	：2012年AMFM_RADIOﾓﾃﾞﾙ
	ファイル名		：sbm_sys.c
	モジュール名	：SUBMODE
	機   能			：Sub systemメッセージ受付処理
--------------------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
--------------------------------------------------------------------------
			2012/7/23	0.1			lrj		新規
*************************************************************************/
#define		_SBM_SYS_C_

#include	"../model.h"


#include    <string.h>
#include	"../comm/common.h"
#include	"../key/key_func_ext.h"
#include	"../key/key_def.h"
#include	"../main/ma_ext.h"
#include	"../off/off_key.h"
#include	"../disp/lcd_ext.h"
#include	"../tuner/tu_ext.h"

#include	"sbm_sys.h"
#include	"sbm_ext.h"
#include	"sbm_keytbl.h"
#include	"sbm_def.h"

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
void	Sbm_sys_message( BYTE msg )
{
	switch(msg)
	{
		case CMSG_INITIAL:
			Sbm_mode_initial();
			break;
		case CMSG_MAIN:
			Clk_time_main();
			break;
		case CMSG_MAINCNT_OFF:
			Sbm_mode_cancel(CSBM_OFF);
			break;
		case CMSG_MODECHG:
		case CMSG_PWROFF:
			Sbm_mode_cancel(CSBM_OFF);
			fsbm_alm_off_req = FALSE;
			break;
		case CMSG_PWRON:
			break;
		case CMSG_HOLDX_OUT:
			/* 表示要求チェック */
			break;
		case CMSG_100MS_TIMER:
			Sbm_100ms_cnt();
			break;
		default:
			break;
	}

}
/****************************************************************
		関数名：Sbm_msg_modechg
		引  数：無し
		戻り値：無し
		機  能：modechgﾒｯｾｰｼﾞ受付処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/7/23		0.1		lrj		新規
****************************************************************/
void Sbm_msg_modechg(void)
{
	if (Vol_mode_check() != OFF)
	{
		Vol_mode_cancel();			/* Vol調整モードｷｬﾝｾﾙ */
	}
	if (Sef_mode_check() != OFF)
	{
		Sef_mode_cancel();			/* Audio調整モードｷｬﾝｾﾙ */
	}
	if (Clk_adj_check() != OFF)
	{
		Clk_adj_cancel();			/* 時計調整モードｷｬﾝｾﾙ */
	}
	if (Alm_mode_check() != OFF)
	{
		Alm_mode_cancel();			/* 時計調整モードｷｬﾝｾﾙ */
	}
}
/****************************************************************
		関数名：Sbm_100ms_cnt
		引  数：無し
		戻り値：無し
		機  能：100msﾀｲﾏｶｳﾝﾄ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2007/3/17		0.1		陳		新規
****************************************************************/
void Sbm_100ms_cnt(void)
{
	/* VOLﾓｰﾄﾞ保持ﾀｲﾏｶｳﾝﾄ */
	if(sbm_vol_tim > 0)
	{
		sbm_vol_tim--;
		if(sbm_vol_tim == 0)
		{
			Vol_mode_cancel();
			#if 0		/* ベース表示しない */
			Lcd_disp_base(Ma_current_mode());		/* Base表示要求 */
			#endif
		}
	}
	
	/* Audio調整モード保持ﾀｲﾏｶｳﾝﾄ */
	if(sbm_aud_tim > 0)
	{
		sbm_aud_tim--;
		if(sbm_aud_tim == 0)
		{
			Sef_mode_cancel();
			#if 0		/* ベース表示しない */
			Lcd_disp_base(Ma_current_mode());		/* Base表示要求 */
			#endif
		}
	}
	
	/* Alarm調整ﾓｰﾄﾞ保持ﾀｲﾏｶｳﾝﾄ */
	if(sbm_alm_tim > 0)
	{
		sbm_alm_tim--;
		if(sbm_alm_tim == 0)
		{
			Alm_mode_cancel();
			#if 0		/* ベース表示しない */
			Lcd_disp_base(Ma_current_mode());		/* Base表示要求 */
			#endif
		}
	}
	
	/* Clock調整ﾓｰﾄﾞ保持ﾀｲﾏｶｳﾝﾄ */
	if(sbm_clk_tim > 0)
	{
		sbm_clk_tim--;
		if(sbm_clk_tim == 0)
		{
			Clk_adj_cancel();
			#if 0		/* ベース表示しない */
			Lcd_disp_base(Ma_current_mode());		/* Base表示要求 */
			#endif
		}
	}
	
	#if 0
	/* Beep On time : 180S */
	if (sbm_beepon_tim > 0)
	{
		sbm_beepon_tim--;
		if(sbm_beepon_tim == 0)
		{
			fsbm_alm_beepon = OFF;
			fsbm_beep_en = ON;
			Aud_cancel_beep();	/* beep off */
		}
	}
	#endif
}
#undef		_SBM_SYS_C_
