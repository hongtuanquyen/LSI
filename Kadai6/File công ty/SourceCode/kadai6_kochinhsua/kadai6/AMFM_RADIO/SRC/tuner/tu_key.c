/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_key.c
		機能			: Tunerｷｰ処理
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]	新規作成
******************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_

#define	_TU_KEY_C_

#include	"../comm/common.h"
#include    "../key/KEY_inc.h"
#include	"../audio/Aud_inc.h"
#include	"../submode/sbm_ext.h"
#include	"../main/MA_inc.h"
#include 	"../power/PW_inc.h"

#include	"tu_key.h"
#include	"tu_keytbl.h"
#include	"tu_main.h"
#include	"tu_event.h"
#include	"tu_test.h"

/******************************************************************************
	関   数  名	: Tu_key_func_addr_get
	引       数	: void
	戻   り  値	: (KEY_PROC_TBL) ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	機 能 概 略	: Tunerﾓｼﾞｭｰﾙ ｷｰ処理ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ取得
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
[sun060616] ﾐｽ1：TunerﾓｰﾄﾞでのTA受信中TAｷｰﾃｰﾌﾞﾙに設定していなかった
[matu120712] ISR処理有効化
********************************************************************************/
KEY_PROC_TBL * Tu_key_func_addr_get(void)
{
#if 1	/*	Tuner/HD RadioのISRを統一管理するため、ISRを独立モードとして追加、100809、岳	*/
	if(Ma_current_mode()==CMA_ISR)
	{
		return ((KEY_PROC_TBL*)&tu_isr_key_tbl);
	}
	else 
#endif
	{
		return ((KEY_PROC_TBL*)&tu_normal_key_tbl);
	}
}

/******************************************************************************
	関   数  名	: Tu_key_CDTable_addr_get
	引       数	: void
	戻   り  値	: (KEY_CHG_TBL) ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	機 能 概 略	: Tunerﾓｼﾞｭｰﾙ ｷｰ変換ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ取得
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
KEY_CHG_TBL * Tu_keyCodeTable_addr_get(void)
{
	return ((KEY_CHG_TBL*)&tu_key_chg_tbl);			/* ﾉｰﾏﾙﾓｰﾄﾞｷｰﾃｰﾌﾞﾙ */
}

/******************************************************************************
	関   数  名	: Tu_ModeID_get
	引       数	: WORD ⇒ Tunerﾒｲﾝﾓｰﾄﾞ
	戻   り  値	: 無し
	機 能 概 略	: Tunerﾓｼﾞｭｰﾙ ﾒｲﾝﾓｰﾄﾞ取得処理
    備       考 :
-------------------------------------------------------------------------------
[sun051215] 新規作成
[岳100810]  TEF6606を対応するため、FM→AM切替する場合、初期パラメータデータを
			送られない可能性がありますので、バンド切替中キー無効ように変更
********************************************************************************/
BYTE	Tu_ModeID_get(void)
{
	BYTE	modeflg;

	modeflg = 0;
	if (tu_m_mode == CTU_M_SCAN)
	{
		modeflg |= CTU_KYEN_SCAN;
	}
	else if ((tu_m_mode == CTU_M_SEEK)||
			(tu_m_mode == CTU_M_CMAN))
	{
		modeflg |= CTU_KYEN_SEEK;
	}
	else if (tu_m_mode == CTU_M_AS)
//			(tu_m_mode == CTU_M_TPAPM ))
	{
		modeflg |= CTU_KYEN_AS;
	}
	else if ((tu_m_mode == CTU_M_PWRON)||
			(tu_m_mode == CTU_M_BANDCHG))	/*	BANDﾁｪﾝｼﾞ中キー無効、100810、岳	*/
	{
		modeflg |= CTU_KYEN_PWRON;
	}
	else
	{
		;	/* QAC対策 */
	}
	return(modeflg);
}

/******************************************************************************
	関   数  名	: Tu_key_nop
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: Tunerﾓｼﾞｭｰﾙｷｰｲﾍﾞﾝﾄ終了処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_nop(void)
{
	Key_nop();					/* ｷｰ処理終了 */
}

/******************************************************************************
	関   数  名	: Tu_key_pw_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙPWRｷｰLONG処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_pw_long(void)
{
	if (Ma_get_clock() == TRUE )
	{
		Clk_adj_start();
		Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
		if(tu_m_mode == CTU_M_PSTWT)
		{
			Tu_job_clear();			/* MODEクリア */
		}
	}
	Key_nop();
}
/******************************************************************************
	関   数  名	: Tu_key_pw_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙPWRｷｰLONG処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_pw_off(void)
{
	Lcd_base_disp_cancel();
	Pw_power_key();
	Key_nop();
}

/******************************************************************************
	関   数  名	: Tu_key_md_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙMODEｷｰLONG処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_md_long(void)
{
	Sef_mode_start();
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Lcd_base_disp_cancel();
	Key_nop();
	if(tu_m_mode == CTU_M_PSTWT)
	{
		Tu_job_clear();			/* MODEクリア */
	}
}
/******************************************************************************
	関   数  名	: Tu_key_md_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙMODEｷｰOFF処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_md_off(void)
{
	Tu_e_md_off();
}

/******************************************************************************
	関   数  名	: Tu_key_up_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTuner Up ｷｰ ON処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_up_on(void)
{
	Tu_e_updw_on(CTU_UP);
}
/******************************************************************************
	関   数  名	: Tu_key_up_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTuner Up ｷｰ Long処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_up_long(void)
{
	Tu_e_updw_long(CTU_UP);
}
/******************************************************************************
	関   数  名	: Tu_key_up_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTuner Up ｷｰ Off処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_up_off(void)
{
	Tu_e_updw_off(CTU_UP);
}
/******************************************************************************
	関   数  名	: Tu_key_dw_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTunerDownｷｰON処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_dw_on(void)
{
	Tu_e_updw_on(CTU_DOWN);
}
/******************************************************************************
	関   数  名	: Tu_key_dw_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTunerDownｷｰLONG処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_dw_long(void)
{
	Tu_e_updw_long(CTU_DOWN);
}
/******************************************************************************
	関   数  名	: Tu_key_dw_off
	引       数	: TTU_EVENT ｲﾍﾞﾝﾄ要求
	戻   り  値	: 無し
	機 能 概 略	: TunerﾓｼﾞｭｰﾙTunerDownｷｰOff処理
    備       考 :
-------------------------------------------------------------------------------
[sun051214] 新規作成
********************************************************************************/
void  Tu_key_dw_off(void)
{
	Tu_e_updw_off(CTU_DOWN);
}
/******************************************************************************
	関   数  名	: Tu_key_as_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerモジュールASキーON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_as_on(void)
{
	Tu_e_as_on();
}
/******************************************************************************
	関   数  名	: Tu_key_as_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerモジュールASキーLONG処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_as_long(void)
{
	Tu_e_as_long();
}

/******************************************************************************
	関   数  名	: Tu_key_ch_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerモジュールCHキーON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void  Tu_key_ch_on(void)
{
	Tu_e_pchupdw_on(CTU_UP);
}
/******************************************************************************
	関   数  名	: Tu_key_ch_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TunerモジュールCHキーLONG処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void  Tu_key_ch_long(void)
{
	Tu_e_pchupdw_long();
}
/******************************************************************************
	関   数  名	: Tu_key_ch_off
	引       数	: TTU_EVENT ｲﾍﾞﾝﾄ要求
	戻   り  値	: 無し
	機 能 概 略	: TunerモジュールCHキーOFF処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void  Tu_key_ch_off(void)
{
	Tu_e_pchupdw_off(CTU_UP);
}

/******************************************************************************
	関   数  名	: Tu_key_scti_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: SCAN/TIキーON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_tisc_on(void)
{
	BYTE	destno;
	
	destno = Tu_dest_chk();
	
	switch(destno)
	{
		case CTU_DEST_JPN_A:
			Key_long_time2000();
			break;
		case CTU_DEST_JPN_FA:
			Tu_key_isr_on();
			Key_nop();
			break;
		default:
			Tu_e_scanupdw_on(CTU_UP);
			break;
	}
}

/******************************************************************************
	関   数  名	: Tu_key_ti_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ISRモジュールUPキーLONG処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_ti_long(void)
{
	BYTE	destno;
	
	destno = Tu_dest_chk();
	
	switch(destno)
	{
		case CTU_DEST_JPN_A:
			if (tu_m_mode == CTU_M_SEEK)
			{
				Key_nop();
			}
			else
			{
				Tu_key_md_long();
			}
			break;
		default:
			Key_nop();
			break;
	}
}

/******************************************************************************
	関   数  名	: Tu_key_ti_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ISRモジュールUPキーLONG処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_ti_off(void)
{
	BYTE	destno;
	
	destno = Tu_dest_chk();
	
	switch(destno)
	{
		case CTU_DEST_JPN_A:
			Tu_key_isr_on();
			Key_nop();
			break;
		default:
			Key_nop();
			break;
	}
}
void	Tu_key_alm_on(void)
{
	if (Ma_get_alarm() == TRUE )
	{
		Key_long_time2000();
	}
	else
	{
		Key_nop();
	}
}
/******************************************************************************
	関   数  名	: Tu_key_alm_long
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: TUNERモジュールALARMキーLONG処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_alm_long(void)
{
	Alm_mode_start();
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Key_nop();
	if(tu_m_mode == CTU_M_PSTWT)
	{
		Tu_job_clear();			/* MODEクリア */
	}	
}
/******************************************************************************
	関   数  名	: Tu_key_alm_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ISRモジュールUPキーLONG処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_alm_off(void)
{
	Alm_onoff_set();
	Key_nop();
}

/******************************************************************************
	関   数  名	: Tu_key_isr_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ISRｷｰ ON処理
    備       考 :
-------------------------------------------------------------------------------
[zhang070315] 新規作成
********************************************************************************/
void Tu_key_isr_on(void)
{
	BYTE ma_mode;
	
	ma_mode = Ma_current_mode();
	switch(ma_mode)
	{
		case CMA_TUNER:
			Tu_isron();
			break;
		case CMA_ISR:
			Tu_isroff();
			break;
		default:
			Ma_mode_event(CMA_ID_TU,CMA_ISR,CMA_MODEON);
			break;
	}
}

/******************************************************************************
	関   数  名	: Tu_key_isrup_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ISRモジュールUPキーON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_isrup_on(void)
{
	Sbm_mode_cancel(CSBM_OFF);
	Tu_e_updw_on(CTU_UP);
}


/******************************************************************************
	関   数  名	: Tu_key_isrup_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ISRモジュールUPキーOFF処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_isrup_off(void)
{
	Tu_e_updw_off(CTU_UP);
}

/******************************************************************************
	関   数  名	: Tu_key_isrdw_on
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ISRモジュールDOWNキーON処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_isrdw_on(void)
{
	Sbm_mode_cancel(CSBM_OFF);
	Tu_e_updw_on(CTU_DOWN);
}

/******************************************************************************
	関   数  名	: Tu_key_isrdw_off
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ISRモジュールDOWNキーOFF処理
    備       考 :
-------------------------------------------------------------------------------
[matu120711] 新規作成
********************************************************************************/
void	Tu_key_isrdw_off(void)
{
	Tu_e_updw_off(CTU_DOWN);
}

#undef	_TU_KEY_C_

#endif
