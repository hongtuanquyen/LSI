/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_event.c
		機能			: TUNER ｲﾍﾞﾝﾄ処理
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051215]	新規作成
[sun060710] FUNCOn処理でﾍﾞｰｽ表示要求出さないように変更
[matu120903] S-meter判定 Local値追加 
******************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_
#define	_TU_EVENT_C_

#include	<string.h>
#include	"../comm/common.h"
#include	"../key/key_func_ext.h"
#include	"../audio/aud_ext.h"
#include	"../disp/lcd_ext.h"
#include	"../main/ma_ext.h"
#include	"../io/IO_inc.h"
#include	"../submode/sbm_ext.h"
#include	"../power/pw_func_ext.h"
#include	"tu_def.h"
#include	"tu_event.h"
#include	"tu_ext.h"
#include	"tu_main.h"
#include	"tu_key.h"
#include	"tu_io.h"
#include	"tu_item.h"

/* 内部関数プロトタイプ宣言 */
static void Tu_mixch_up(void);
static void Tu_mixch_down(void);
/******************************************************************************
	関   数  名	: Tu_e_bndon
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｰﾄﾞOn処理
    備       考 :
-------------------------------------------------------------------------------
[sun051223] 新規作成
[sun060709] ｽﾃﾚｵ検出禁止ﾀｲﾏ1100msに設定処理追加
[sun060710] ﾍﾞｰｽ表示要求出さないように変更
[zhang070323]ｽﾃﾚｵ検出禁止ﾀｲﾏ1100msに設定を削除
********************************************************************************/
void	Tu_e_func_on(void)
{
	Tu_job_clear();
	tu_m_mode = CTU_M_PWRON;					/* PowerOnﾓｰﾄﾞｾｯﾄ */
	Tu_stdis_timset(CTU_ST_ANTON);
	Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);	/* TunerMute要求ON */
	ftu_mute = ON;								/* Mute Flagｾｯﾄ */
}
/******************************************************************************
	関   数  名	: Tu_e_func_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｰﾄﾞOff処理
    備       考 :
-------------------------------------------------------------------------------
[sun051223] 新規作成
[zhang070308]ｱﾝﾃﾅ100ms->2000ms後NoActiveにｾｯﾄ
********************************************************************************/
void	Tu_e_func_off(void)
{
	Tu_lastcall();				/* ﾗｽﾄ読み出し */
	ftu_sttest = OFF;		/* 「070727張」ST表示を追加*/
	Tu_job_clear();
	Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);	/* TunerMute要求ON */
	ftu_mute = ON;								/* Mute Flagｾｯﾄ */
}

/******************************************************************************
	関   数  名	: Tu_e_md_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: 他モードﾞModeOff処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成 新規作成
********************************************************************************/
void  Tu_e_md_off(void)
{
	BYTE chg_req;
	
	if(Ma_current_mode() == CMA_ISR)
	{
		Ma_key_src();
	}
#if 0 /* QAC対策 */
	else if(Ma_current_mode() == CMA_AUX)
	{
		Tu_amband_set();
		Ma_key_src();
	}
#endif
	else
	{
		Tu_job_clear();
		switch(tu_bandno)
		{
			case	CTU_BAND_AM:
			case	CTU_BAND_FM1:
				Tu_BndChange();
				Tu_lastcall();						/* ﾗｽﾄ呼出 */
				tu_m_mode = CTU_M_BANDCHG;			/* BandChangeﾓｰﾄﾞｾｯﾄ */
				Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
				Lcd_disp_base(CMA_TUNER);			
				Sbm_mode_cancel(CSBM_OFF);
				break;
			default:
				Ma_key_src();
				break;
		}
	}
	Tu_key_nop();						/* ｷｰ処理完了 */
}

#if 0	/*	11Model最新仕様[20100519]Bandｷｰ長押無効、20100601、岳↓	*/
/******************************************************************************
	関   数  名	: Tu_e_bndon
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙBandｷｰOn処理
    備       考 :
-------------------------------------------------------------------------------
[sun051221] 新規作成
[zhang070308]seek,ps,as中に,チュ－ニングモ－ド切換はできず
********************************************************************************/
void	Tu_e_bndon(void)
{
	switch (tu_m_mode)
	{
		case	CTU_M_SCAN:
			Tu_lastwrite();
		case	CTU_M_AS:
		case	CTU_M_SEEK:
			Tu_e_bndoff();
			Tu_key_nop();
//			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
			break;
		default:
			Tu_job_clear();
			Key_long_time1000();
			break;
	}
	Sbm_mode_cancel(CMA_TUNER);
}

/******************************************************************************
	関   数  名	: Tu_e_bndlong
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙBandｷｰLong処理
    備       考 :
-------------------------------------------------------------------------------
[sun051221] 新規作成
[zhang070308]T-MODEモ－ド切換を変更
********************************************************************************/
void	Tu_e_bndlong(void)
{
	Tu_job_clear();
	ftu_seek = ~ftu_seek;				/* seekﾌﾗｸﾞ逆転 */
	Tu_key_nop();						/* ｷｰ処理完了 */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
	Lcd_disp_base(CMA_TUNER);
}
#endif
/******************************************************************************
	関   数  名	: Tu_e_bndoff
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙBandｷｰOff処理
    備       考 :
-------------------------------------------------------------------------------
[sun051221] 新規作成
********************************************************************************/
void	Tu_e_bndoff(void)
{
	if(tu_m_mode == CTU_M_SCAN)
	{
		Tu_lastwrite();
	}
	else
	{
	
	}
	Tu_job_clear();
	Tu_BndChange();						/* BandChange */
	Tu_lastcall();						/* ﾗｽﾄ呼出 */
	tu_m_mode = CTU_M_BANDCHG;			/* BandChangeﾓｰﾄﾞｾｯﾄ */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
	Lcd_disp_base(CMA_TUNER);
	Sbm_mode_cancel(CMA_TUNER);
}

/******************************************************************************
	関   数  名	: tu_e_updw_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTuner Up/DOWN ｷｰ ON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_e_updw_on(BYTE updw)
{
	Tu_updw_set(updw);
	switch (tu_m_mode)
	{
		case	CTU_M_SEEK:
		case	CTU_M_SCAN:
			Tu_lastwrite();
			Tu_job_clear();
			tu_m_mode = CTU_M_LSTCAL;
			Sbm_mode_cancel(CMA_TUNER);
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */	
			Lcd_disp_base(CMA_TUNER);
			Tu_key_nop();				/* ｷｰ無効 */
			break;
		default:
			Tu_job_clear();
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
			Key_long_time500();		/*	500msタイマー設定	*/
			break;
	}
}
/******************************************************************************
	関   数  名	: Tu_e_updw_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTuner Up/DOWN ｷｰ Long処理
    備       考 :
-------------------------------------------------------------------------------
********************************************************************************/
void	Tu_e_updw_long(BYTE updw)
{
	Tu_job_clear();
	tu_m_mode = CTU_M_SEEK;
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
	Tu_key_nop();
}
/******************************************************************************
	関   数  名	: Tu_e_updw_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTuner Up/DOWN ｷｰ Off処理
    備       考 :
-------------------------------------------------------------------------------
[sun051221] 新規作成
[zhang070309]CTU_M_CMAN終了後seek続->CTU_M_CMAN終了後動作無
********************************************************************************/
void	Tu_e_updw_off(BYTE updw)
{
	Tu_job_clear();
	tu_m_mode = CTU_M_MAN;		/* ﾏﾆｭｱﾙｱｯﾌﾟﾀﾞｳﾝﾓｰﾄﾞｾｯﾄ */
	Tu_key_nop();				/* ｷｰ無効 */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
	#if 0
	if(Ma_current_mode() == CMA_ISR)
	{
		Lcd_disp_base(CMA_ISR);
	}
	else
	{
		Lcd_disp_base(CMA_TUNER);
	}
	#endif
}

/******************************************************************************
	関   数  名	: Tu_e_presetlong
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙPresetｷｰLong処理
    備       考 :
-------------------------------------------------------------------------------
[sun051221] 新規作成
[sun060106] 単体ﾃｽﾄ完[ﾐｽ1 ｷｰNOPしていない]
********************************************************************************/
void	Tu_e_presetlong(BYTE pstno )
{
	Tu_pno_set(pstno);					/* ｶﾚﾝﾄPresetNoｾｯﾄ */
	Tu_job_clear();
	tu_m_mode = CTU_M_PSTWT;			/* Preset Writeﾓｰﾄﾞｾｯﾄ */
	Tu_key_nop();					/* ｷｰ処理完了 */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
	Lcd_disp_base(CMA_TUNER);
}
/******************************************************************************
	関   数  名	: Tu_e_presetoff
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙPresetｷｰoff処理
    備       考 :
-------------------------------------------------------------------------------
[sun051221] 新規作成
********************************************************************************/
void	Tu_e_presetoff(BYTE pstno )
{
	Tu_pno_set(pstno);				/* ｶﾚﾝﾄPresetNoｾｯﾄ */
	Tu_job_clear();
	Tu_presetcall();
	tu_m_mode = CTU_M_PSTRD;		/* Preset Writeﾓｰﾄﾞｾｯﾄ */
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
	Lcd_disp_base(CMA_TUNER);
}
/******************************************************************************
	関   数  名	: Tu_isron
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙISRモードON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_isron(void)
{
	Tu_job_clear();
	Ma_mode_event(CMA_ID_TU,CMA_ISR,CMA_MODEON);
}

/******************************************************************************
	関   数  名	: Tu_e_isroff
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙISRモードOff処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_isroff(void)
{
	Tu_job_clear();
	Ma_mode_event(CMA_ID_TU,CMA_ISR,CMA_MODEOFF);
}
/******************************************************************************
	関   数  名	: Tu_e_ason
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙASキー ON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_e_as_on(void)
{
	switch (tu_m_mode)
	{
		case	CTU_M_SEEK:
		case	CTU_M_SCAN:
		case	CTU_M_AS:
			Tu_lastcall();
			Tu_job_clear();
			tu_m_mode = CTU_M_LSTCAL;
			Sbm_mode_cancel(CMA_TUNER);
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */	
			Lcd_disp_base(CMA_TUNER);
			Tu_key_nop();
			break;
		default:
			Key_long_time2000();
			break;
	}
}
/******************************************************************************
	関   数  名	: Tu_e_as_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙASキー LONG処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_e_as_long(void)
{
	Tu_job_clear();
	tu_m_mode = CTU_M_AS;
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */	
	Lcd_disp_base(CMA_TUNER);
	Tu_key_nop();
}

/******************************************************************************
	関   数  名	: Tu_e_pchupdw_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙCHｷｰON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_e_pchupdw_on(BYTE updw)
{
	switch (tu_m_mode)
	{
		case	CTU_M_AS:
		case	CTU_M_SCAN:
		case	CTU_M_SEEK:
			Tu_job_clear();
			Tu_lastcall();
			tu_pno = Tu_pch_updw(tu_pno,updw);
			Tu_presetcall();
			tu_m_mode = CTU_M_PSTRD;					/* Preset readﾓｰﾄﾞｾｯﾄ */
			Sbm_mode_cancel(CMA_TUNER);
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
			Lcd_disp_base(CMA_TUNER);
			Tu_key_nop();
			break;
		case	CTU_M_PSTWT:
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
			Key_long_time500();							/*	500msタイマー設定	*/
			break;
		default:
			Tu_job_clear();
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
			Key_long_time500();							/*	500msタイマー設定	*/
			break;
	}
}
/******************************************************************************
	関   数  名	: Tu_e_pchupdw_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙCHｷｰLONG処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_e_pchupdw_long(void)
{
	switch (tu_m_mode)
	{
		case	CTU_M_PSTWT:
			ftu_write = ON;							/* 書き込み要求ON */
			break;
		default:
			Tu_job_clear();
			ftu_write = OFF;							/* 書き込み要求OFF */
			tu_m_mode = CTU_M_PSTWT;					/* Preset writeﾓｰﾄﾞｾｯﾄ */
			break;
	}
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
	Lcd_disp_base(CMA_TUNER);
	Tu_key_nop();
}
/******************************************************************************
	関   数  名	: Tu_e_pchupdw_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙCHｷｰOFF処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_e_pchupdw_off(BYTE updw)
{
	switch (tu_m_mode)
	{
		case	CTU_M_PSTWT:
			tu_wrtpno = Tu_pch_updw(tu_wrtpno,updw);
			tu_100ms_timer = CTU_PSTWAIT_TIM;	 		 /* プリセット書き込み待ち */
			Lcd_blktim_set();
			break;
		default:
			Tu_job_clear();
			tu_pno = Tu_pch_updw(tu_pno,updw);
			Tu_presetcall();
			tu_m_mode = CTU_M_PSTRD;					/* Preset readﾓｰﾄﾞｾｯﾄ */
			break;
	}
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);				/* すべて時限表示ｷｬﾝｾﾙ */
	Lcd_disp_base(CMA_TUNER);
	Tu_key_nop();
}

/******************************************************************************
	関   数  名	: Tu_e_stteston
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙSOUND+P1ｷｰ ON処理
    備       考 :
-------------------------------------------------------------------------------
[[zhang070727] 新規作成
********************************************************************************/
void	Tu_e_stteston(void)
{
#if 0 /* QAC対策 */
/*	Key_long_time5000();
	Sel_mode_cancel(); *//* C12 */
#endif
	Lcd_1shot_key_cancel(CMA_TUNER);
}
/******************************************************************************
	関   数  名	: Tu_e_sttestlong
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	:  TunerﾓｼﾞｭｰﾙSOUND+P1ｷｰ LONG処理
    備       考 :
-------------------------------------------------------------------------------
[zhang070727] 新規作成
********************************************************************************/
void	Tu_e_sttestlong(void)
{
	if (Tu_band_chk() == CTU_FM)
	{
		ftu_sttest = ~ftu_sttest;
	}
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
	Sbm_mode_cancel(CMA_TUNER);
	Tu_key_nop();					/* ｷｰ処理完了 */
}
/******************************************************************************
	関   数  名	: Tu_e_scanupdw_on
	引       数	: updw =
	 				CTU_UP:SCAN UP
	 				CTU_DW:SCAN DW
	戻   り  値	: 無し
	機 能 概 略	:  SCANキー ON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
void	Tu_e_scanupdw_on(BYTE updw)
{
	if(Sbm_mode_check() != CSBM_SEF)
	{
		switch (tu_m_mode)
		{
			case	CTU_M_PWRON:
			case	CTU_M_BANDCHG:
				break;
			case	CTU_M_SCAN:
				Tu_lastwrite();
				Tu_job_clear();
				tu_m_mode = CTU_M_LSTCAL;
				Sbm_mode_cancel(CMA_TUNER);
				Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
				Lcd_disp_base(CMA_TUNER);
				break;
			default:
				Tu_job_clear();
				Tu_updw_set(updw);
				tu_m_mode = CTU_M_SCAN;
				Sbm_mode_cancel(CMA_TUNER);
				Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
				Lcd_disp_base(CMA_TUNER);
				break;
		}
	}
	Tu_key_nop();
}

void	Tu_e_seek_updw(BYTE ud)
{
	Tu_updw_set(ud);
	switch (tu_m_mode)
	{
		case	CTU_M_AS:
			Tu_lastcall();
		/* QAC対策 */
			Tu_lastwrite();
			Tu_job_clear();
			tu_m_mode = CTU_M_LSTCAL;
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
			Lcd_disp_base(CMA_TUNER);
			break;
		case	CTU_M_SEEK:
		case	CTU_M_SCAN:
			Tu_lastwrite();
			Tu_job_clear();
			tu_m_mode = CTU_M_LSTCAL;
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
			Lcd_disp_base(CMA_TUNER);
			break;
		default:
			Tu_job_clear();
			tu_m_mode = CTU_M_SEEK;
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */
			Lcd_disp_base(CMA_TUNER);
			break;
	}
	Sbm_mode_cancel(CMA_TUNER);
}
/*	テストモード対応、100826、岳↑	*/

/******************************************************************************
	関   数  名	: Tu_pch_updw
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: Tunerﾓｼﾞｭｰﾙpch up/dw判定処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
BYTE	Tu_pch_updw(BYTE get_pno, BYTE updw)
{
	BYTE pno;
	
	if (updw == CTU_UP)
	{
		pno = Tu_pch_up(get_pno);
	}
	else
	{
		pno = Tu_pch_down(get_pno);
	}
	return(pno);
}

/******************************************************************************
	関   数  名	: Tu_pch_up
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: PCH UP処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
BYTE	Tu_pch_up(BYTE get_pno)
{
	BYTE pno;

	pno = get_pno + 1;

	if (pno > 6)
	{
		pno = 0x01;			/* Preset1に戻る */
	}
	return(pno);
}
/******************************************************************************
	関   数  名	: Tu_pch_down
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: PCH down処理
    備       考 :
-------------------------------------------------------------------------------
[matu120713] 新規作成
********************************************************************************/
BYTE	Tu_pch_down(BYTE get_pno)
{
	BYTE pno;

	pno = get_pno;
	if(pno == 0)
	{
		pno = 0x06;			/* Preset6に戻る */
	}
	else
	{
		pno --;
		if (pno < 1)
		{
			pno = 0x06;			/* Preset6に戻る */
		}
	}
	/* QAC対策 */
	return(pno);

}

/******************************************************************************
	関   数  名	: Tu_updw_set
	引       数	: ｱｯﾌﾟﾀﾞｳﾝ要求
				: TRUE ⇒ ｱｯﾌﾟ方向
				: FALSE ⇒ ﾀﾞｳﾝ方向
	戻   り  値	: 無し
	機 能 概 略	: Tunerｱｯﾌﾟﾀﾞｳﾝ方向設定処理
-------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[0]
********************************************************************************/
void	Tu_updw_set(BYTE updw)
{
	if (updw == CTU_UP)
	{
		ftu_updw = TRUE;
	}
	else
	{
		ftu_updw = FALSE;
	}
}

/******************************************************************************
.. 		関数名  ：Tu_job_clear
		引  数	：無し
		戻り値	：無し
		機  能	：Tunerﾒｲﾝ動作ｸﾘｱ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[0]
******************************************************************************/
void	Tu_job_clear(void)						/* ﾒｲﾝﾓｰﾄﾞ動作関係ｸﾘｱ処理 */
{
	tu_m_mode = CTU_NOP;
	tu_m_seq = 0;
	tu_i_seq = 0;
	tu_10ms_timer = 0;
	tu_5ms_timer = 0;
#if 0 /* QAC対策 */
//	tu_updw_type = CTU_NORMAL;
#endif
}
/******************************************************************************
.. 		関数名  ：Tu_as_initial
		引  数	：無し
		戻り値	：無し
		機  能	：Tunerｵｰﾄﾌﾟﾘｾｯﾄ初期化処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu20120713] 新規作成
******************************************************************************/
void	Tu_as_initial(void)					/* AS初期化処理 */
{
	tu_as_cnt = 0x00;
	tu_ch.freq = tu_coef.min_ch;		/* 最小周波数からスタート */
}
/******************************************************************************
.. 		関数名  ：Tu_fmbandmax
		引  数	：無し
		戻り値	：無し
		機  能	：FM MAX Bandサーチ処理
				：ASスタート/PS時のFM BAND MAX番号設定処理
				：TU_BAND_INFOの順番はFMからAMの順になるのが前提です。
				：AM先になる場合は、変更必要。
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun070319]新規作成
******************************************************************************/
BYTE Tu_fmbandmax(void)
{
	BYTE	iCnt;
	BYTE	destno;

	destno = Tu_dest_chk();
	
	for (iCnt = 0; iCnt < CTU_BAND; iCnt++)
	{
		if (TU_BAND_INFO[destno][iCnt][0] == CTU_AM)
		{
			break;
		}
	}
	return(iCnt-1);
}

/******************************************************************************
.. 		関数名  ：Tu_BndChange
		引  数	：無し
		戻り値	：無し
		機  能	：TunerBandﾁｪﾝｼﾞ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[0]
******************************************************************************/
void	Tu_BndChange(void)						/* BandChange */
{
	tu_bandno++;
	if (tu_bandno >= CTU_BAND)
	{
		tu_bandno = 0;
	}
}
/******************************************************************************
.. 		関数名  ：Tu_presetcall
		引  数	：無し
		戻り値	：処理状態
		機  能	：TUNER Preset周波数呼出処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051223] 新規作成
[sun051226] 単体ﾃｽﾄ完[ﾐｽ1 ﾗｽﾄ周波数書込み漏れ]
******************************************************************************/
void	Tu_presetcall(void)
{
	/* Preset周波数読み出し */
	tu_ch = tu_mem[tu_bandno].pset[tu_pno];		/* Preset周波数、REG、ListID */
	Tu_ch_lm_set(tu_ch.freq);					/* LW/MWﾌﾗｸﾞｾｯﾄ */
	/* ﾗｽﾄ周波数に書込み */
	tu_mem[tu_bandno].pstno = tu_pno;			/* ﾗｽﾄPresetNo */
	tu_mem[tu_bandno].pset[0] = tu_ch;			/* ﾗｽﾄ周波数 */
}

/******************************************************************************
.. 		関数名  ：Tu_dxlo_chk
		引  数	：BYTE band
		戻り値	:BYTE ret
		機  能	：DX、LOCAL判定処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120926] 新規作成
******************************************************************************/
BYTE	Tu_dxlo_chk(BYTE band)
{
	BYTE sm_chk;
	BYTE ill_det;
	
	ill_det = Pw_ret_illdet();
	
	switch(tu_test_dxlo)
	{
		case CTU_TEST_LOCAL:
			sm_chk = tu_chkcef[band].sm_lodb;	/* LOCAL設定 */
			break;
			
		case CTU_TEST_DX:
			sm_chk = tu_chkcef[band].sm_dxdb;	/* DX設定 */
			break;
			
		default:
			if(ill_det == HIGH)						/* ILL HIGH */
			{
				sm_chk = tu_chkcef[band].sm_dxdb;	/* DX設定 */
			}
			else									/* ILL LOW */
			{
				sm_chk = tu_chkcef[band].sm_lodb;	/* LOCAL設定 */
			}
			break;
	}
	return(sm_chk);
}

/******************************************************************************
.. 		関数名  ：Tu_intset_pchtbl
		引  数	：destno
		戻り値	:なし
		機  能	：プリセットテーブル初期設定処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120926] 新規作成
******************************************************************************/
void	Tu_intset_pchtbl(BYTE destno)
{
	BYTE	iLoop1,iLoop2;
	
	for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
	{
		for (iLoop2 = 1; iLoop2 < (CTU_PRESET+1); iLoop2 ++)
		{
			tu_mem[iLoop1].pset[iLoop2].freq = TU_PRESET_TBL[destno][iLoop1][iLoop2-1];
		}
	}
}

/******************************************************************************
.. 		関数名  ：Tu_isrcall
		引  数	：無し
		戻り値	：処理状態
		機  能	：ISR周波数呼出処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[zhang070312] 新規作成
******************************************************************************/
void	Tu_isrcall(void)
{
	/* ISR周波数読み出し */
	tu_ch = tu_isrch;		/* Preset周波数、REG、ListID */
	Tu_ch_lm_set(tu_ch.freq);					/* LW/MWﾌﾗｸﾞｾｯﾄ */
}

/******************************************************************************
.. 		関数名  ：Tu_lastcall
		引  数	：無し
		戻り値	：処理状態
		機  能	：TUNER ﾗｽﾄ周波数呼出処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[0]
[sun060608] ﾐｽ1：設計漏れAMﾓｰﾄﾞ中他ﾓｰﾄﾞでﾗｽﾄｺｰﾙを行う場合は、0をｾｯﾄする
******************************************************************************/
void	Tu_lastcall(void)
{
	if(Ma_current_mode() == CMA_ISR)				/* [sakuda101017] */
	{
		tu_ch = tu_isrch;		/* Preset周波数、REG、ListID */
		Tu_ch_lm_set(tu_ch.freq);					/* LW/MWﾌﾗｸﾞｾｯﾄ */
	}
	else
	{
		tu_pno = tu_mem[tu_bandno].pstno;			/* ﾗｽﾄPresetNo */
		tu_ch = tu_mem[tu_bandno].pset[tu_pno];		/* ﾗｽﾄ周波数、REG、ListID */
		Tu_ch_lm_set(tu_ch.freq);					/* LW/MWﾌﾗｸﾞｾｯﾄ */
	}
}
/******************************************************************************
.. 		関数名  ：Tu_lastwrite
		引  数	：無し
		戻り値	：無し
		機  能	：Tunerﾗｽﾄ書込み処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[0]
[sun060608] ﾐｽ1：設計漏れAMﾓｰﾄﾞ中他ﾓｰﾄﾞでﾗｽﾄ書き込みしないよう変更
******************************************************************************/
void	Tu_lastwrite(void)						/* ｶﾚﾝﾄ周波数をﾗｽﾄへ書き込み */
{
	if(Ma_current_mode() == CMA_ISR)				/* [sakuda101017] */
	{
		tu_isrch = tu_ch;
	}
	else
	{
		tu_mem[tu_bandno].pstno = tu_pno;			/* ﾗｽﾄPresetNo */
		tu_mem[tu_bandno].pset[tu_pno] = tu_ch;		/* ﾗｽﾄ周波数、REG、ListID */
		tu_mem[tu_bandno].pset[0] = tu_ch;			/* ﾗｽﾄ周波数、REG、ListID */
	}
}

/******************************************************************************
.. 		関数名  ：Tu_set_prechno
		引  数	：無し
		戻り値	：無し
		機  能	：Presetに登録されている周波数かﾁｪｯｸ。されていればをPno表示する
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120918] 新規作成
******************************************************************************/
void Tu_set_prechno(void)
{
	BYTE preno;

	preno = Tu_pstno_chk();
	if(preno == 0)				/* [sakuda100528]ﾌﾟﾘｾｯﾄ登録されていない時のみ処理 */
	{
		preno = Tu_chk_presetfreq();
		if(preno != 0)
		{
			Tu_pno_set(preno);				/* ｶﾚﾝﾄPresetNoｾｯﾄ */
			Tu_presetcall_nolast();			/*[sakuda100804]*/
		}
	}
}

/******************************************************************************
.. 		関数名  ：Tu_chk_presetfreq
		引  数	：無し
		戻り値	：無し
		機  能	：Presetに登録されている周波数かﾁｪｯｸ。
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120918] 新規作成
******************************************************************************/
BYTE Tu_chk_presetfreq(void)
{
	TTU_CH prefreq;
	BYTE cnt;
	
	for(cnt = 0; cnt < 6; cnt++)
	{
		prefreq = tu_mem[tu_bandno].pset[cnt+1];		/* 現ﾊﾞﾝﾄﾞのPreset周波数取得 */
		if(tu_ch.freq == prefreq.freq)
		{
			return(cnt+1);
		}
	}
	return(0);
}

/******************************************************************************
.. 		関数名  ：Tu_presetcall_nolast
		引  数	：無し
		戻り値	：無し
		機  能	：TUNER プリセット情報呼出処理(ﾗｽﾄには登録しない)
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120918] 新規作成
******************************************************************************/
void	Tu_presetcall_nolast(void)
{
	/* Preset周波数読み出し */
	tu_ch = tu_mem[tu_bandno].pset[tu_pno];		/* Preset周波数、REG、ListID */
	Tu_ch_lm_set(tu_ch.freq);					/* LW/MWﾌﾗｸﾞｾｯﾄ */
}

/******************************************************************************
.. 		関数名  ：Tu_pno_set
		引  数	：新しいPresetNo
		戻り値	：無し
		機  能	：ｶﾚﾝﾄﾌﾟﾘｾｯﾄNoｾｯﾄ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[0]
******************************************************************************/
void	Tu_pno_set(BYTE pno)					/* ｶﾚﾝﾄPresetNoｾｯﾄ */
{
	tu_pno = pno;
}
/******************************************************************************
.. 		関数名  ：Tu_aftermute
		引  数	：ｱﾌﾀMUTEﾀｲﾏ
		戻り値	：無し
		機  能	：ｱﾌﾀMUTEﾀｲﾏｾｯﾄ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun060106] 単体ﾃｽﾄ完[変更1 MUTEﾌﾗｸﾞ追加]
******************************************************************************/
void	Tu_aftermute(BYTE time)
{
	ftu_mute = OFF;						/* Mute要求OFF */
	tu_aftermute = time;				/* Mute解除ﾀｲﾏｾｯﾄ */
}

/******************************************************************************
.. 		関数名  ：Tu_ch_updw
		引  数	：無し
		戻り値	：tu_updw_stus
		機  能	：Tuner周波数ｱｯﾌﾟﾀﾞｳﾝ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[0]
******************************************************************************/
void	Tu_ch_updw(void)
{
	if( (Ma_model_dest() == CMA_MODEL_AMFM_X)	&&
		((tu_m_mode == CTU_M_AS) 				||
		 (tu_m_mode == CTU_M_SCAN)				||
		 (tu_m_mode == CTU_M_SEEK))				&&
		(Tu_band_chk() == CTU_AM)				 )
	{
		if (ftu_updw == TRUE)
		{
			Tu_mixch_up();
		}
		else
		{
			Tu_mixch_down();
		}
	}
	else
	{
		if (ftu_updw == TRUE)
		{
			Tu_ch_up();
		}
		else
		{
			Tu_ch_down();
		}
	}
}
/******************************************************************************
.. 		関数名  ：Tu_ch_lm_set
		引  数	：freq ⇒ 指定周波数
		戻り値	：ftu_lw ⇒ 指定周波数 LW/MW情報
		機  能	：Tuner周波数範囲ｾｯﾄ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051225] 新規作成
[sun051226] 単体ﾃｽﾄ完[0]
******************************************************************************/
void Tu_ch_lm_set(WORD freq)
{
	ftu_lw = CTU_MW;				/* とりあえず MW Band */

	Tu_coef_read();

	if (tu_bndmode == CTU_LW)				/* LW ﾊﾞﾝﾄﾞ有り */
	{
		if (freq < tu_coef.min_ch)				/* 指定周波数MWﾊﾞﾝﾄﾞ最小周波数より小さい */
		{
			ftu_lw = CTU_LW;		/* 指定周波数LWﾊﾞﾝﾄﾞ */
			Tu_coef_read();			/* LW BAND情報読み直す */
		}
	}
}

/******************************************************************************
.. 		関数名  ：Tu_dest_chk
		引  数	：無し
		戻り値	：Tuner仕向けNo
		機  能	：Tuner仕向け番号ﾁｪｯｸ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060109] 新規作成
[zhang070301]仕向け値を変更
[zhang070305]仕向けの取得値を変更
******************************************************************************/
BYTE	Tu_dest_chk(void)
{
#if	(CTU_E_TEST == 0x01)
	return (CTU_DEST_E);
#else
	BYTE	dest;
	BYTE	iRet;
	
	dest = Ma_model_dest();		/* 仕向けの取得 */
	
	/* 日本仕向け AMラジオ */
	if (dest == CMA_MODEL_AM_J) 	/* AF_CHANGE */
	{
		iRet = CTU_DEST_JPN_A;
	}
	/* 日本仕向け FM/AMラジオ */
	else if (dest == CMA_MODEL_AMFM_J)	/* AF_CHANGE */
	{
		iRet = CTU_DEST_JPN_FA;
	}
	/* 北米/中南米仕向け */
	else if (dest == CMA_MODEL_AMFM_U)			/* AF_CHANGE */
	{
		iRet = CTU_DEST_USA;
	}
	/* 中南米仕向け */
	else if (dest == CMA_MODEL_AMFM_B)			/* AF_CHANGE */
	{
		iRet = CTU_DEST_CSA;
	}
	/* オセアニア仕向け */
	else if (dest == CMA_MODEL_AMFM_T)			/* AF_CHANGE */
	{
		iRet = CTU_DEST_OCA;
	}
	/* アジア/アフリカ/中東仕向け */
	else if (dest == CMA_MODEL_AMFM_A)
	{
		iRet = CTU_DEST_AAM;
	}
	/* 第三地域仕向け */
	else
	{
		iRet = CTU_DEST_THR;
	}
	
	return(iRet);
#endif
}
/******************************************************************************
.. 		関数名  ：Tu_coef_read
		引  数	：無し
		戻り値	：無し
		機  能	：TunerBAND係数ｾｯﾄ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060109] 新規作成
******************************************************************************/
void Tu_coef_read(void)
{
	BYTE	bandmd;
	BYTE	destno;

	bandmd = Tu_band_chk();
	destno = Tu_dest_chk();

	tu_bndmode = TU_COEF_TBL[destno].mode;

	if (bandmd == CTU_FM)
	{
		/* FM LWﾓｰﾄﾞありなし情報取り出す */
		if ((tu_bndmode & CTU_FMLW) == CTU_FMLW)
		{
			tu_bndmode = CTU_LW;					/* LWﾊﾞﾝﾄﾞ有り */
		}
		else
		{
			tu_bndmode = CTU_MW;					/* MWﾊﾞﾝﾄﾞのみ */
		}

		if (ftu_lw == CTU_MW)
		{
			tu_coef = TU_COEF_TBL[destno].fm;
		}
		else
		{
			tu_coef = TU_COEF_TBL[destno].fm_lw;
		}
	}
	else
	{
		/* AM LWﾓｰﾄﾞありなし情報取り出す */
		if ((tu_bndmode & CTU_AMLW) == CTU_AMLW)
		{
			tu_bndmode = CTU_LW;					/* LWﾊﾞﾝﾄﾞ有り */
		}
		else
		{
			tu_bndmode = CTU_MW;					/* MWﾊﾞﾝﾄﾞのみ */
		}

		if (ftu_lw == CTU_MW)
		{
			tu_coef = TU_COEF_TBL[destno].am;
		}
		else
		{
			tu_coef = TU_COEF_TBL[destno].am_lw;
		}
		
		if(Ma_current_mode() == CMA_ISR)
		{
			tu_coef.min_freq = TU_ISR_TBL[0];
			tu_coef.min_ch = TU_ISR_TBL[1];
			tu_coef.max_ch = TU_ISR_TBL[2];
		}
	}
}
/******************************************************************************
.. 		関数名  ：Tu_ch_up
		引  数	：無し
		戻り値	：tu_updw_stus
		機  能	：Tuner周波数ｱｯﾌﾟ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[ﾐｽ1 MAX Step取得ﾐｽ]
******************************************************************************/
void	Tu_ch_up(void)
{
	tu_updw_stus = CTU_NOTEDGE;
	if (tu_ch.freq >= tu_coef.max_ch)
	{
		if (tu_bndmode == CTU_LW)				/* LW ﾊﾞﾝﾄﾞ有り */
		{
			if (ftu_lw == CTU_MW)				/* MW Band */
			{
				tu_updw_stus = CTU_BNDEDGE;		/* ﾊﾞﾝﾄﾞｴｯｼﾞ */
			}
			else
			{
				tu_updw_stus = CTU_LMEDGE;		/* LW⇒MW切替ｴｯｼﾞ */
			}
			ftu_lw = ~ftu_lw;					/* LW Flag反転 */
			Tu_coef_read();						/* BAND情報読み直す */
		}
		else
		{
			tu_updw_stus = CTU_BNDEDGE;			/* ﾊﾞﾝﾄﾞｴｯｼﾞ */
		}
		tu_ch.freq = tu_coef.min_ch;					/* 最初周波数ｾｯﾄ */
	}
	else
	{
		tu_ch.freq ++;
	}
}
/******************************************************************************
.. 		関数名  ：Tu_ch_down
		引  数	：無し
		戻り値	：tu_updw_stus
		機  能	：Tuner周波数ﾀﾞｳﾝ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051221] 新規作成
[sun051226] 単体ﾃｽﾄ完[ﾐｽ1 MAX Step取得ﾐｽ]
******************************************************************************/
void	Tu_ch_down(void)
{
	tu_updw_stus = CTU_NOTEDGE;
	if (tu_ch.freq <= tu_coef.min_ch)
	{
		if (tu_bndmode == CTU_LW)						/* LW ﾊﾞﾝﾄﾞ有り */
		{
			if (ftu_lw == CTU_LW)				/* MW Band */
			{
				tu_updw_stus = CTU_BNDEDGE;		/* ﾊﾞﾝﾄﾞｴｯｼﾞ */
			}
			else
			{
				tu_updw_stus = CTU_LMEDGE;		/* LW⇒MW切替ｴｯｼﾞ */
			}
			ftu_lw = ~ftu_lw;					/* LW Flag反転 */
			Tu_coef_read();						/* BAND情報読み直す */
		}
		else
		{
			tu_updw_stus = CTU_BNDEDGE;			/* ﾊﾞﾝﾄﾞｴｯｼﾞ */
		}
		tu_ch.freq = tu_coef.max_ch;				/* 最大周波数ｾｯﾄ */
	}
	else
	{
		tu_ch.freq --;
	}
}

/******************************************************************************
.. 		関数名  ：Tu_dispfreqset
		引  数	：無し
		戻り値	：無し
		機  能	：Tuner周波数表示ﾃﾞｰﾀ作成処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051223] 新規作成
[sun051228] 単体ﾃｽﾄ完
[sun060602] N向けEON、AF、BS動作中ﾗｽﾄ周波数を表示ﾃﾞｰﾀにｾｯﾄ
[sun060619] ﾐｽ1: LWありBANDで、MWの周波数が計算ﾐｽ
******************************************************************************/
void	Tu_dispfreqset(void)
{
	BYTE	bandmd;
	WORD	step;
	WORD	min_freq;

	bandmd = Tu_band_chk();
	step = tu_coef.step;
	min_freq = tu_coef.min_freq;

	if (bandmd == CTU_FM)		/* FM STEP数 */
	{
		step = (step/10);				/* step 10kHzに変換 */
	}	
	tu_dispfreq = ((tu_ch.freq - tu_coef.min_ch) * step) + min_freq;
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_steroclr()
		引  数	：なし
		戻り値	：無し
		機  能	：Tunerｽﾃﾚｵ強制OFF設定処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060112] 新規作成
-----------------------------------------------------------------------------*/
void Tu_steroclr(void)
{
	ftu_stero = OFF;
	ftu_stero_b = OFF;
	ftu_stchk = OFF;
	tu_stcnt = 0;
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_steroclr()
		引  数	：なし
		戻り値	：無し
		機  能	：Tunerｽﾃﾚｵﾁｬﾀﾘﾝｸﾞ処理
				：ONﾁｬﾀ  → 50回*10ms
				：OFFﾁｬﾀ →  2回*10ms
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060112] 新規作成
[sun060619] ﾐｽ1 STﾎﾟｰﾄ極性逆でした
[zhang070226] 修正  
              pFM_STSD制御ポート    
-----------------------------------------------------------------------------*/
void Tu_sterochat(void)
{
	tu_ic_data.msa.subaddr = 0xe0;
	tu_ic_data.length = 1;
	Tu_data_read();/* SD値取り込み */
	if((tu_ic_data.rxbuf[0]&0x10) == 0x10)
	{
		if (ftu_stero_b == LOW)
		{
			tu_stcnt = CTU_STON_CNT;		/* ONﾁｬﾀﾘﾝｸﾞｶｳﾝﾀｾｯﾄ */
		}
		ftu_stero_b = HIGH;
	}
	else
	{
		if (ftu_stero_b == HIGH)
		{
			tu_stcnt = CTU_STOFF_CNT;		/* OFFﾁｬﾀﾘﾝｸﾞｶｳﾝﾀｾｯﾄ */
		}
		ftu_stero_b = LOW;
	}

	if (tu_stcnt == 0)				/* ﾁｬﾀﾘﾝｸﾞｶｳﾝﾀ完了 */
	{
		ftu_stero = ftu_stero_b;	/* ｽﾃﾚｵ確定 */
	}
	else
	{
		tu_stcnt --;
	}
}
#if 0 /* 未使用のためコメントアウト */
/******************************************************************************
.. 		関数名  ：Tu_as_samefrqchk
		引  数	：無し
		戻り値	：TRUE ⇒同じ周波数APMｴﾘｱに格納されている
				：FALSE⇒同じ周波数見つからなかった
		機  能	：同じ局ﾌﾟﾘｾｯﾄﾒﾓﾘされているかのﾁｪｯｸ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051228] 新規作成
[sun060105] 単体ﾃｽﾄ完[変更1 0xffと比較しない]
******************************************************************************/
BYTE	Tu_as_samefrqchk(void)
{
	BYTE	iCnt;

	for (iCnt = 1; iCnt <= CTU_ASMAX; iCnt++)
	{
		if (tu_mem[tu_bandno].pset[iCnt].freq == tu_ch.freq)
		{
			return(iCnt);				/* 同じ局ありました */
		}
	}
	return(FALSE);						/* 同じ局格納されていない */
}
#endif
/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_stdis_timset()
		引  数	：ﾀｲﾏ値
		戻り値	：無し
		機  能	：Tunerｽﾃﾚｵ禁止ﾀｲﾏｾｯﾄ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060112] 新規作成
-----------------------------------------------------------------------------*/
void	Tu_stdis_timset(BYTE	tim)
{
	if (tu_stdistim < tim)
	{
		tu_stdistim = tim;
	}
}

/************************************************************************/
/* 関数名 :void	Tu_band_freq_set(void)									*/
/* --------------------------------------------------------------------	*/
/* 機能   :	TunerBANDと周波数送信データ設定処理							*/
/* 詳細   :																*/
/************************************************************************/
void	Tu_band_freq_set(void)
{
	BYTE band;
	BYTE destno;
	WORD_BYTE setfrq;
	
	band = Tu_band_chk();
	destno = Tu_dest_chk();
	Tu_dispfreqset();
	
	if(band == CTU_AM)
	{
		setfrq.word = tu_dispfreq;
	}
	else
	{
		if ((destno == CTU_DEST_USA)
		&&  (tu_dispfreq == 8770))
		{
			tu_dispfreq = 8775;		/* 北米最小周波数の場合微調整が必要 */
		}
		setfrq.word = (tu_dispfreq/5);
	}
#if 0		/*	EEPROMから初期パラメータを取得対応、100811、岳	*/
	tu_ic_data.txbuf[0] = (setfrq.byte.byte1 & 0x0f) | TU_PARA_TBL[destno][band][CTU_PARA_TUNER0];
	tu_ic_data.txbuf[1] = setfrq.byte.byte0;
	tu_ic_data.txbuf[2] = TU_PARA_TBL[destno][band][CTU_PARA_TUNER2];
#else
	tu_ic_data.txbuf[0] = (setfrq.byte.byte1 & 0x0f) | tu_para[band][CTU_PARA_TUNER0];
	tu_ic_data.txbuf[1] = setfrq.byte.byte0;
	tu_ic_data.txbuf[2] = tu_para[band][CTU_PARA_TUNER2];
#endif
	Tu_stdis_timset(CTU_ST_PLLCHG);
}
/************************************************************************/
/* 関数名 :void	Tu_band_para_set(void)									*/
/* --------------------------------------------------------------------	*/
/* 機能   :	TunerBAND関連PARA送信データ設定処理							*/
/* 詳細   :																*/
/************************************************************************/
void	Tu_band_para_set(void)
{
	BYTE band;
	BYTE destno;
	
	band = Tu_band_chk();
	destno = Tu_dest_chk();
	#if	(CTU_E_TEST == 0x01)
	if ((ftu_lw != CTU_MW)&&(band == CTU_AM))
	{
		memcpy(&tu_ic_data.txbuf[CTU_PARA_RADIO],TU_PARA_LW_E_TBL,13);
		return;
	}
	#endif
	#if 0		/*	EEPROMから初期パラメータを取得対応、100811、岳	*/
	memcpy(&tu_ic_data.txbuf[CTU_PARA_RADIO],&TU_PARA_TBL[destno][band][CTU_PARA_RADIO],13);
	#else
	memcpy(&tu_ic_data.txbuf[CTU_PARA_RADIO],&tu_para[band][CTU_PARA_RADIO],13);
	#endif
}

/************************************************************************/
/* 関数名 :void	Tu_band_para_set(void)									*/
/* --------------------------------------------------------------------	*/
/* 機能   :	TunerBAND関連PARA送信処理									*/
/* 詳細   :																*/
/************************************************************************/
void	Tu_para_send_set(void)
{
	Tu_band_freq_set();
	Tu_band_para_set();
	tu_ic_data.msa.subaddr = 0x20;
	tu_ic_data.length = 16;
	Tu_data_write();
}
/************************************************************************/
/* 関数名 :void	Tu_band_para_set(void)									*/
/* --------------------------------------------------------------------	*/
/* 機能   :	TunerBANDと周波数送信処理									*/
/* 詳細   :																*/
/************************************************************************/
void	Tu_frq_send_set(void)
{
	Tu_band_freq_set();
	tu_ic_data.msa.subaddr = 0x20;
	tu_ic_data.length = 3;
	Tu_data_write();
}

/************************************************************************/
/* 関数名 :void	Tu_band_para_set(void)									*/
/* --------------------------------------------------------------------	*/
/* 機能   :	TunerBANDと周波数送信処理									*/
/* 詳細   :																*/
/************************************************************************/
void	Tu_end_send_set(void)
{
	Tu_band_freq_set();
	tu_ic_data.msa.subaddr = 0xe0;
	tu_ic_data.length = 3;
	Tu_data_write();
}
void	Tu_beep_mute_set(BYTE on_off)
{
	if( on_off == ON )
	{
		Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);	/* TunerMute要求ON */
		ftu_mute = ON;								/* Mute Flagｾｯﾄ */
	}
	else if(on_off == OFF)
	{
		Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,OFF);	/* TunerMute要求OFF */
		ftu_mute = OFF;								/* Mute Flagｾｯﾄ */
	}
	else
	{
		;
	}
}

/******************************************************************************
	関   数  名	: Tu_mixch_up
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: Tuner混合周波数アップ処理
    備       考 : 第三地域仕向け仕様対応周波数アップ処理
				  5k/9k混合ステップで周波数をアップする。
				  525kHzの時は停止しない。
-------------------------------------------------------------------------------
[yamamoto130314] 新規作成
********************************************************************************/
static void Tu_mixch_up(void)
{
	/* 変数宣言 */
	WORD i;
	
	tu_updw_stus = CTU_NOTEDGE;							/* TunerUp/Dw状態クリア	*/
	
	if (tu_ch.freq >= tu_coef.max_ch)					/* カレント周波数が最大	*/
	{
		tu_updw_stus = CTU_BNDEDGE;						/* バンドエッジ設定		*/
		tu_ch.freq = tu_coef.min_ch;					/* 最小周波数セット		*/
	}
	else if ( (tu_ch.freq >= 0) && (tu_ch.freq <= 7) )	/* 525kHzは停止しない	*/
	{
		tu_ch.freq = 8;									/* 530kHzに設定			*/
	}
	else
	{
		for ( i = 1; i <= 5; i++ )
		{
			if ( ((tu_ch.freq + i) % 5 == 3) ||
				 ((tu_ch.freq + i) % 9 == 0)  )
			{
				tu_ch.freq += i;
				return;
			}
		}
	}
}
/******************************************************************************
	関   数  名	: Tu_mixch_down
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: Tuner混合周波数ダウン処理
    備       考 : 第三地域仕向け仕様対応周波数ダウン処理
				  5k/9k混合ステップで周波数をダウンする。
				  525kHzの時は停止しない。
-------------------------------------------------------------------------------
[yamamoto130314] 新規作成
********************************************************************************/
static void Tu_mixch_down(void)
{
	/* 変数宣言 */
	WORD i;
	
	tu_updw_stus = CTU_NOTEDGE;							/* TunerUp/Dw状態クリア	*/
	
	if (tu_ch.freq <= tu_coef.min_ch)					/* カレント周波数が最大	*/
	{
		tu_updw_stus = CTU_BNDEDGE;						/* バンドエッジ設定		*/
		tu_ch.freq = tu_coef.max_ch;					/* 最大周波数セット		*/
	}
	else if ( (tu_ch.freq >= 1) && (tu_ch.freq <= 8) )	/* 525kHzは停止しない	*/
	{
		tu_ch.freq = 0;									/* 522kHzに設定			*/
	}
	else
	{
		for ( i = 1; i <= 5; i++ )
		{
			if ( ((tu_ch.freq - i) % 5 == 3) ||
				 ((tu_ch.freq - i) % 9 == 0)  )
			{
				tu_ch.freq -= i;
				return;
			}
		}
	}
}

#undef  _TU_EVENT_C_

#endif