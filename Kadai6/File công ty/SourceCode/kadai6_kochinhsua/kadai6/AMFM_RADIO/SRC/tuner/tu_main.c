/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_sys.c
		機能			: ①Tuner Systemﾒｯｾｰｼﾞ受付処理
						: ②ﾀｲﾏ関係処理
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]	新規作成
[matu120903] S-meter判定 Local値追加 
******************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_

#define	_TU_MAIN_C_

#include	<string.h>
#include	"../comm/common.h"
#include	"../main/ma_ext.h"
#include	"../disp/lcd_ext.h"
#include	"../audio/aud_ext.h"
#include	"../power/pw_func_ext.h"	/* QAC対策 */
#if		(_MODEL_EEPROM_ == 1)
#include	"../eeprom/eeprom_ext.h"
#include	"tu_eeprom.h"
#endif
#include	"tu_def.h"
#include	"tu_ext.h"
#include	"tu_io.h"
#include	"tu_main.h"
#include	"tu_item.h"
#include	"tu_conttbl.h"
#include	"tu_event.h"
#include	"tu_test.h"
static	void	Tu_main_cont(void);
static	void	Tu_sterochk(void);
static	void	Tu_mute_cont(void);
static	void	Tu_bandtbl_set(BYTE destno);
static	void	Tu_bandpara_set(BYTE destno);




/******************************************************************************
.. 		関数名  ：Tu_main
		引  数	：なし
		戻り値	：無し
		機  能	：TunerMain処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125] 新規作成
******************************************************************************/
void	Tu_main(void)
{
	if (tu_pwrmode != CTU_PWROFF)
	{
		Tu_main_cont();			/* Tunerﾒｲﾝｺﾝﾄﾛｰﾙ処理 */
		Tu_sterochk();			/* ｽﾃﾚｵ信号検出処理 */
	}
	Tu_mute_cont();				/* TunerMUTEｺﾝﾄﾛｰﾙ処理 */
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_main_cont()
		引  数	：なし
		戻り値	：無し
		機  能	：Tuner動作ｺﾝﾄﾛｰﾙ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
[sun060616] ﾒｲﾝﾓｰﾄﾞ範囲ﾁｪｯｸ追加
-----------------------------------------------------------------------------*/
static	void	Tu_main_cont(void)
{
	#if 1
	if ((Pw_ACC_DET_get() == OFF)||(Pw_Power_status_get() == OFF))
	#else
	if (Pw_ACC_DET_get() == OFF)
	#endif
	{
		return;
	}
	
	if (tu_m_mode != CTU_NOP)				/* 動作中 */
	{
		if (tu_m_mode >= CTU_M_MAX)
		{
			tu_m_mode = CTU_NOP;			/* 範囲外であれば、クリア */
		}
		else
		{
			Tu_main_process();
		}
	}
#if	(_MODEL_EEPROM_ == 1)
	if( ftu_weepreq == ON )
	{
		Tu_WriteEepData();
	}
#endif
	Tu_Test_Main();
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_sterochk()
		引  数	：なし
		戻り値	：無し
		機  能	：Tunerｽﾃﾚｵ信号検出処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060112] 新規作成
[sun060619] 単体ﾃｽﾄ完
[zhang070301]ＳＴインジケータのSD1.47V以下の判断を削除する
-----------------------------------------------------------------------------*/
static	void	Tu_sterochk(void)
{
	if (Tu_band_chk() != CTU_FM) 
	{
		Tu_steroclr();						/* ST状態ｸﾘｱ */
		return;
	}
	if ((tu_m_mode != CTU_NOP) &&			/* Scan以外動作中ST検出しない */
		(tu_m_mode != CTU_M_SCAN))
	{
		Tu_steroclr();						/* ST状態ｸﾘｱ */
		return;
	}
	if (tu_stdistim != TIMER_OUT)			/* ST検出禁止中 */
	{
		Tu_steroclr();						/* ST状態ｸﾘｱ */
		return;
	}

	if (ftu_stchk == ON)					/* 10msﾌﾗｸﾞ経過 */
	{
		ftu_stchk = OFF;
		Tu_sterochat();						/* STﾁｬﾀﾘﾝｸﾞ処理 */
	}
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_mute_cont()
		引  数	：なし
		戻り値	：無し
		機  能	：Tuner MUTE処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
[sun061026] 動作していないのに、MUTEかけているであれば、MUTE解除する
			動作していないときにRDS MUTEも解除するように変更
-----------------------------------------------------------------------------*/
static	void	Tu_mute_cont(void)
{
//					/* [070524] キー長押MUTE処理修正*/
	#if 0
	if ((ftu_mute == ON) &&
		(tu_m_mode == CTU_NOP) &&
		(tu_pwrmode == CTU_FUNCON))
	{
		Tu_aftermute(CTU_AFTER500);
	}
	#endif
	if ((ftu_mute == OFF) &&
		(tu_aftermute == TIMER_OUT)&&
		(Aud_get_power_status() == CAUD_PWR_ON))
	{
		Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,OFF);	/* TunerMute要求ON */
	}
}

/******************************************************************************
.. 		関数名  ：Tu_initial
		引  数	：なし
		戻り値	：無し
		機  能	：Tuner初期化処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125] 新規作成
[sun051212] 初期設定処理追加
[sun060421] listid初期化処理追加
[sun060501] ﾗｽﾄ周波数LIST ID初期化漏れ修正
[zhang070301]ﾗｽﾄ周波数初期設定を変更
[zhang070312]isr初期設定を追加
[岳100806]EEPROM機能を一部対応(ラストメモリ)
[matu120903] S-meter判定 Local値算出処理追加 
******************************************************************************/
void	Tu_initial(void)
{
	BYTE	destno;
	BYTE	iLoop1;
	
	/* 仕向け無効の判断処理追加 */
	if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_NG)||
		(Ma_Chk_EEP_Initial() == CMA_INIT_STA_AREA_NG))
	{
		return;
	}
	
	/* 仕向けの取得 */
	destno = Tu_dest_chk();
	
	/* BAND情報初期設定 */
	for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
	{
		tu_mem[iLoop1].band = TU_BAND_INFO[destno][iLoop1][0];		/* FM/AM */
		tu_mem[iLoop1].bno = TU_BAND_INFO[destno][iLoop1][1];		/* Band No */
	}
	/* ラスト情報設定 */
	tu_bandno = 0;								/* ラストバンド設定 */
	for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
	{
		tu_mem[iLoop1].pstno = 0;				/* ラストプリセット設定 */
		tu_mem[iLoop1].pset[0].freq = 0x0000;	/* ラスト周波数設定 */
	}
	
	Tu_bandtbl_set(destno);						/* Band Table設定 */
	
	#if	(CTU_E_TEST == 0x01)
	tu_mem[3].pstno = 2;									/* ﾗｽﾄPresetNo */
	#endif

	tu_isrch.freq = TU_ISR_TBL[1];		/* ISR周波数設定 */

	tu_test_mode = 0xFF;

	wMemSet(tu_para[0],0xFF,sizeof(tu_para));
	Tu_bandpara_set(destno);					/* Band Parameter設定 */

	tu_level_FM1db = Tu_Level_db_Chg(tu_chkcef[CTU_FM].sm_20db,tu_chkcef[CTU_FM].sm_40db);
	tu_level_AM1db = Tu_Level_db_Chg(tu_chkcef[CTU_AM].sm_20db,tu_chkcef[CTU_AM].sm_40db);
	/* S-meter判定値　DX設定 */
	if( tu_chkcef[CTU_FM].sm_dxdb > 20 )
	{
		tu_dx[CTU_FM]	= ((tu_level_FM1db * (tu_chkcef[CTU_FM].sm_dxdb - tu_chkcef[CTU_FM].sm_20db))/10 +  tu_chkcef[CTU_FM].sm_20db);
	}
	else
	{
		tu_dx[CTU_FM]		= (tu_chkcef[CTU_FM].sm_20db - (tu_level_FM1db * (tu_chkcef[CTU_FM].sm_20db - tu_chkcef[CTU_FM].sm_dxdb))/10);
	}
	if( tu_chkcef[CTU_AM].sm_dxdb > 20 )
	{
		tu_dx[CTU_AM]			= ((tu_level_AM1db * (tu_chkcef[CTU_AM].sm_dxdb - tu_chkcef[CTU_AM].sm_20db))/10 +  tu_chkcef[CTU_AM].sm_20db);
	}
	else
	{
		tu_dx[CTU_AM]			= (tu_chkcef[CTU_AM].sm_20db - (tu_level_AM1db * (tu_chkcef[CTU_AM].sm_20db - tu_chkcef[CTU_AM].sm_dxdb))/10);
	}
	/* S-meter判定値　LOCAL値設定 *//* [matu120903] */
	if( tu_chkcef[CTU_AM].sm_lodb > 20 )
	{
		tu_lo[CTU_AM]			= ((tu_level_AM1db * (tu_chkcef[CTU_AM].sm_lodb - tu_chkcef[CTU_AM].sm_20db))/10 +  tu_chkcef[CTU_AM].sm_20db);
	}
	else
	{
		tu_lo[CTU_AM]			= (tu_chkcef[CTU_AM].sm_20db - (tu_level_AM1db * (tu_chkcef[CTU_AM].sm_20db - tu_chkcef[CTU_AM].sm_lodb))/10);
	}
	
	Tu_lastcall();
	tu_wrtpno = 0x01;					/* プリセット1を設定 */
	tu_qinfochk_num = CTU_SEEK_NUM;		/* Q-info確認回数設定変数 [matu121105] */
}

/************************************************************************/
/* 関数名 :void	Tu_bandtbl_set(void)									*/
/* --------------------------------------------------------------------	*/
/* 機能   :	Bandテーブル設定処理										*/
/* 詳細   :																*/
/************************************************************************/
void	Tu_bandtbl_set(BYTE destno)
{
	BYTE	iLoop1,iLoop2;
#if	(_MODEL_EEPROM_ == 1)		/*	EEPROM機能を一部対応、ラストメモリ、100806、岳	*/
	BYTE	tu_eepbuf[48];
	BYTE	pcnt;
#endif
	
	/* プリセットメモリ情報 */
#if	(_MODEL_EEPROM_ == 1)		/*	EEPROM機能を一部対応、ラストメモリ、100806、岳	*/
	if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_OK)&&	/*	EEPROM初期化済みチェック、失敗場合デフォールト値を使う	*/
		(Tu_ReadEepData(tu_eepbuf,destno) == TRUE))
	{
		/* AMとFM Preset周波数初期設定 */
		for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
		{
			for (iLoop2 = 1; iLoop2 < (CTU_PRESET+1); iLoop2 ++)
			{
				pcnt = ((iLoop1)*16) + ((iLoop2 - 1) * 2);
				tu_mem[iLoop1].pset[iLoop2].freq = (WORD)((tu_eepbuf[pcnt+1]*0x100) + tu_eepbuf[pcnt]);
			}
		}
	}
	else
#endif
	{
		/* ﾗｽﾄ/Preset周波数初期設定 */
		Tu_intset_pchtbl(destno);
		
		/* EEPROMへ正しい周波数を書き込む */
		Tu_SaveEepData(CTU_EEPWR_PST,tu_eepdata);
		EEP_Initial_para_Req(tu_eepdata,0x00);
	}
}


/************************************************************************/
/* 関数名 :void	Tu_bandpara_set(void)									*/
/* --------------------------------------------------------------------	*/
/* 機能   :	Bandパラメータ設定処理											*/
/* 詳細   :																*/
/************************************************************************/
void	Tu_bandpara_set(BYTE destno)
{
#if	(_MODEL_EEPROM_ == 1)		/*	EEPROM機能を一部対応、ラストメモリ、100806、岳	*/
	BYTE	tu_eepbuf[96];
	BYTE	ret;
#endif

#if	(_MODEL_EEPROM_ == 1)
	ret = Tu_ReadEepPara(tu_eepbuf,destno);
	if( ret & CTU_EEP_ADR_Q_INF1_CHK )
	{
		tu_chkcef[CTU_FM].sm_20db = tu_eepbuf[CTU_EEP_ADR_FMSMETER20DB];
		tu_chkcef[CTU_FM].sm_40db = tu_eepbuf[CTU_EEP_ADR_FMSMETER40DB];
		
		tu_chkcef[CTU_AM].sm_20db = tu_eepbuf[CTU_EEP_ADR_AMSMETER20DB];
		tu_chkcef[CTU_AM].sm_40db = tu_eepbuf[CTU_EEP_ADR_AMSMETER40DB];
		
		tu_chkcef[CTU_FM].sm_dxdb = tu_eepbuf[CTU_EEP_ADR_FMSMDXLV];
		tu_chkcef[CTU_AM].sm_dxdb = tu_eepbuf[CTU_EEP_ADR_AMSMDXLV];
		
		tu_chkcef[CTU_AM].sm_lodb = tu_eepbuf[CTU_EEP_ADR_AMSMLOLV];	/* [matu120903] */
		
		tu_chkcef[CTU_FM].usn_max = tu_eepbuf[CTU_EEP_ADR_FMUSN];
		tu_chkcef[CTU_FM].wam_max = tu_eepbuf[CTU_EEP_ADR_FMWAM];
	}
	else
	{
		romcopy(&tu_chkcef[CTU_FM].sm_20db,(BYTE *)&TU_INI_CHKCOEF[CTU_FM],4);
		
		tu_chkcef[CTU_FM].usn_max = TU_INI_CHKCOEF[CTU_FM].usn_max;
		tu_chkcef[CTU_FM].wam_max = TU_INI_CHKCOEF[CTU_FM].wam_max;
		
		romcopy(&tu_chkcef[CTU_AM].sm_20db,(BYTE *)&TU_INI_CHKCOEF[CTU_AM],4);
		
		#if 0	/* 仕様変更 */
		tu_chkcef[CTU_AM].sm_lodb = TU_INI_CHKCOEF[CTU_AM].sm_lodb;		/* [matu120903] */
		#endif
	}
	if( ret & CTU_EEP_ADR_PARA_FM_CHK )
	{
		tu_para[CTU_FM][CTU_PARA_TUNER0] = 0x20;
		if(( destno == CTU_DEST_JPN_A )
		|| ( destno == CTU_DEST_JPN_FA ))
		{
			wMemCopy(&tu_eepbuf[CTU_EEP_ADR_PARA_FMJ],&tu_para[CTU_FM][CTU_PARA_TUNER2],(CTU_PARA_LEN-2));
		}
		else
		{
			wMemCopy(&tu_eepbuf[CTU_EEP_ADR_PARA_FMU],&tu_para[CTU_FM][CTU_PARA_TUNER2],(CTU_PARA_LEN-2));
		}
	}
	else
	{
		romcopy(tu_para[0],(BYTE *)TU_PARA_TBL[destno][CTU_FM],CTU_PARA_LEN);
	}
	
	if( ret & CTU_EEP_ADR_PARA_AM_CHK )
	{
		tu_para[CTU_AM][CTU_PARA_TUNER0] = 0x00;
		wMemCopy(&tu_eepbuf[CTU_EEP_ADR_PARA_AM],&tu_para[CTU_AM][CTU_PARA_TUNER2],(CTU_PARA_LEN-2));
	}
	else
	{
		romcopy(tu_para[1],(BYTE *)TU_PARA_TBL[destno][CTU_AM],CTU_PARA_LEN);
	}
	
	if( ret & CTU_EEP_ADR_Q_INF2_CHK )
	{
		tu_chkcef[CTU_FM].if_rough = tu_eepbuf[CTU_EEP_ADR_FM_IFROUGH];
		tu_chkcef[CTU_FM].if_detail = tu_eepbuf[CTU_EEP_ADR_FM_IFDETAIL];
		
		tu_chkcef[CTU_AM].if_rough = tu_eepbuf[CTU_EEP_ADR_AM_IFROUGH];
		tu_chkcef[CTU_AM].if_detail = tu_eepbuf[CTU_EEP_ADR_AM_IFDETAIL];
		
		tu_chkcef[CTU_FM].wait_tm = tu_eepbuf[CTU_EEP_ADR_FM_Q_WAIT];
		tu_chkcef[CTU_AM].wait_tm = tu_eepbuf[CTU_EEP_ADR_AM_Q_WAIT];
	}
	else
	{
		romcopy(&tu_chkcef[CTU_FM].if_rough,(BYTE *)&TU_INI_CHKCOEF[CTU_FM].if_rough,3);
		if( destno == CTU_DEST_THR )/* 第三地域仕向けはIFDetail値のみ違う */
		{
			romcopy(&tu_chkcef[CTU_AM].if_rough,(BYTE *)&TU_INI_CHKCOEF[CTU_AM].if_rough,1);
			tu_chkcef[CTU_AM].if_detail = TU_AM_IF_DETAIL_THIRD_DEST;
			romcopy(&tu_chkcef[CTU_AM].wait_tm,(BYTE *)&TU_INI_CHKCOEF[CTU_AM].wait_tm,1);
		}
		else
		{
			romcopy(&tu_chkcef[CTU_AM].if_rough,(BYTE *)&TU_INI_CHKCOEF[CTU_AM].if_rough,3);
		}
	}
	if( ret & CTU_EEP_ADR_Q_INF3_CHK )
	{
		tu_chkcef[CTU_AM].prec_sm = tu_eepbuf[CTU_EEP_ADR_AM_PERC_LV];
		tu_chkcef[CTU_FM].prec_sm = tu_eepbuf[CTU_EEP_ADR_FM_PERC_LV];
		tu_chkcef[CTU_FM].prec_noise = tu_eepbuf[CTU_EEP_ADR_FM_PERC_NS];
	}
	else
	{
		tu_chkcef[CTU_AM].prec_sm = TU_INI_CHKCOEF[CTU_AM].prec_sm;
		tu_chkcef[CTU_FM].prec_sm = TU_INI_CHKCOEF[CTU_FM].prec_sm;
		tu_chkcef[CTU_FM].prec_noise = TU_INI_CHKCOEF[CTU_FM].prec_noise;
	}
#else
		romcopy(tu_para[0],(BYTE *)TU_PARA_TBL[destno][CTU_FM],CTU_PARA_LEN);
		romcopy(tu_para[1],(BYTE *)TU_PARA_TBL[destno][CTU_AM],CTU_PARA_LEN);
		romcopy(&tu_chkcef[CTU_FM].sm_20db,(BYTE *)&TU_INI_CHKCOEF[0],sizeof(TU_INI_CHKCOEF));
#endif
	}


/************************************************************************/
/* 関数名 :void	Tu_aftermuteset(void)									*/
/* --------------------------------------------------------------------	*/
/* 機能   :	ｱﾌﾀMUTEﾀｲﾏｾｯﾄ処理											*/
/* 詳細   :																*/
/************************************************************************/
void	Tu_aftermuteset(void)
{
	if (tu_updw_stus != CTU_NOTEDGE)
	{
		Tu_aftermute(CTU_AFTER300);
	}
	else
	{
		Tu_aftermute(CTU_AFTER100);
	}
}

#undef  _TU_MAIN_C_

#endif
