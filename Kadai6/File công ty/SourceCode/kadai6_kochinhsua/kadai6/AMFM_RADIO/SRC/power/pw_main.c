/******************************************************************************
	クラリオン（株）殿向け		

		プロジェクト名	: AMFMラジオ
		ファイル名		: pw_main.c
		機能			: 電源モード遷移処理
------------------------------------------------------------------------------
修正履歴
[chin20120725] 改造
******************************************************************************/
#define	_PW_MAIN_C_

#include	"../comm/common.h"
#include	"../key/key_func_ext.h"
#include	"../io/IO_inc.h"
#include	"../main/ma_ext.h"
#include	"../audio/aud_ext.h"
#include	"../disp/lcd_ext.h"
#include	"../submode/sbm_ext.h"

#include	"pw_def.h"
#include	"pw_func_ext.h"
#include	"pw_main.h"

/*-----------------------------------------------------------------------
    関数宣言
------------------------------------------------------------------------*/
static	void	Pw_set_item(void);
static	void	Pw_set_targetmode(void);
#if _POWER_TEST_COLDS_
static	BYTE	Pw_ColdS(void);
#endif
static	BYTE	Pw_HoldX(void);
static	BYTE	Pw_HoldX_In(void);
static	BYTE	Pw_HoldX_Out(void);
static	BYTE	Pw_AccOff(void);
static	BYTE	Pw_AccOff_In(void);
static	BYTE	Pw_AccOff_Out(void);
static	BYTE	Pw_PwrOff(void);
static	BYTE	Pw_PwrOn_Out(void);
static	BYTE	Pw_PwrOn_In(void);
static	BYTE	Pw_PwrOn(void);
static	BYTE	Pw_holdX_in_check(void);

/*-----------------------------------------------------------------------
   ITEM設定ﾃｰﾌﾞﾙ
------------------------------------------------------------------------*/

const BYTE tpw_item_tbl[4][4] = {
/* ｶﾚﾝﾄﾓｰﾄﾞ	HOLDX中 		 ACCOFF中 	  		POWER OFF 			Power ON中		*/	/* 目標ﾓｰﾄﾞ */
	{	CPW_I_HOLDX,		CPW_I_HOLDX_IN,		CPW_I_ACCOFF_IN,	CPW_I_PWRON_OUT	},	/* HOLDX */
	{	CPW_I_HOLDX_OUT,	CPW_I_ACCOFF,		CPW_I_ACCOFF_IN,	CPW_I_PWRON_OUT	},	/* ACCOFF */
	{	CPW_I_HOLDX_OUT,	CPW_I_ACCOFF_OUT,	CPW_I_PWROFF,		CPW_I_PWRON_OUT	},	/* PowerOff */
	{	CPW_I_HOLDX_OUT,	CPW_I_ACCOFF_OUT,	CPW_I_PWRON_IN,		CPW_I_PWRON		}	/* PowerOn */
};

/*-----------------------------------------------------------------------
   電源処理ITEM ﾃｰﾌﾞﾙ
★★注意： tpw_item_tblで設定される値と連動する★★
------------------------------------------------------------------------*/
typedef struct{
	BYTE	item;
	BYTE	(*func)(void);
}TPW_ITEM;

const  TPW_ITEM tpw_item[] =
{
		CPW_I_HOLDX,		Pw_HoldX
	,	CPW_I_HOLDX_IN,		Pw_HoldX_In
	,	CPW_I_HOLDX_OUT,	Pw_HoldX_Out
	,	CPW_I_ACCOFF,		Pw_AccOff
	,	CPW_I_ACCOFF_IN,	Pw_AccOff_In
	,	CPW_I_ACCOFF_OUT,	Pw_AccOff_Out
	,	CPW_I_PWROFF,		Pw_PwrOff
	,	CPW_I_PWRON_OUT,	Pw_PwrOn_Out
	,	CPW_I_PWRON_IN,		Pw_PwrOn_In
	,	CPW_I_PWRON,		Pw_PwrOn
		#if _POWER_TEST_COLDS_
	,	CPW_I_COLDS,		Pw_ColdS			/* Cold Start処理 */
		#endif
};

/******************************************************************************
	関 数 名	：pw_main
	引   数		：無し
	戻 り 値	：無し
	機 能 概 略	：電源制御ﾓｼﾞｭｰﾙ 状態監視ﾒｲﾝ処理
------------------------------------------------------------------------------
修正履歴
【番号】	日付		担当者		修正内容
[sun060401]新規作成
******************************************************************************/
void	Pw_main(void)
{
	const	TPW_ITEM *pwitem;
	BYTE 	(*pwfunc)(void);
	BYTE	work;

	do
	{
		#if _WDT_
		Io_wdt_clear();
		#endif
		
		/* ﾀｰｹﾞｯﾄﾓｰﾄﾞ設定処理 */
		Pw_set_targetmode();
		/* 動作ITEM設定処理 */
		Pw_set_item();
		/* ITEM処理呼出処理 */
		pwitem = (const TPW_ITEM*)(tpw_item + pw_item);
		pwfunc = (pwitem->func);
		work = (*pwfunc)();
	}while(work == CPW_SEQCONT);
	
	#if _POWER_DIAG_
	Pw_DIAG_check(CPW_DIAG_MD_MAIN);
	#endif	/* _POWER_DIAG_ */
}
/******************************************************************************
	関 数 名	：Pw_set_targetmode
	引   数		：無し
	戻 り 値	：無し
	機 能 概 略	：電源目標状態設定処理
				：BATT/ACC/PANEL、Powerｷｰなどの状態をあわせて電源目標時をｾｯﾄ
------------------------------------------------------------------------------
修正履歴：
[sun070321] 変更
[sun070323] ACC OFF中ILL-DETがあれば、HALTから出るように変更
[sun100616] ILL ONでCTRL端子をHIGHにしない、KEY ILL/BlinkLEDのみ制御のため、
			CPW_DISPOFFにしないように変更
******************************************************************************/
static	void	Pw_set_targetmode(void)
{
	BYTE	tarmode;
	
	#if 1
	if (fpw_wakeup == TRUE)
	{
		tarmode = CPW_PWRON;
	}
	else
	if ((fpw_acc == ON)||(Alm_mode_check() == TRUE))
	{
		if (fpw_power_key == ON)
		{
			tarmode = CPW_PWRON;		/* ﾀｰｹﾞｯﾄﾓｰﾄﾞをPowerOnへ */
		}
		else
		{
			tarmode = CPW_PWROFF;		/* ﾀｰｹﾞｯﾄﾓｰﾄﾞをPOWER OFFへ */
		}
	}
	else
	{
		#if _HOLDX_
		if (( fpw_acc_nor == OFF)
#if 0 /* 使用しないため、削除 */
		  &&( pw_accwait_timer == TIMER_OUT)
#endif
		  &&( fpw_accdet_active == OFF)
		  &&( fpw_almdet_active == OFF))
		{
			tarmode = CPW_HOLDX;		/* ﾀｰｹﾞｯﾄﾓｰﾄﾞをHOLDXへ */
		}
		else
		#endif
		{
			tarmode = CPW_ACCOFF;		/* ﾀｰｹﾞｯﾄﾓｰﾄﾞをAccOFF中の通常モードへ */
		}
	}
	#else
	tarmode = CPW_ACCOFF;
	#endif
	
	/* ﾀｰｹﾞｯﾄﾓｰﾄﾞﾁｪﾝｼﾞあり */
	#if 1
	if ((tarmode != pw_tarmode) &&		/* ﾀｰｹﾞｯﾄﾓｰﾄﾞ変化あり */
		     (fpw_itemchg_enb == ON))		/* ITEM完了待ち中 */
	#else
	if (tarmode != pw_tarmode)		/* ﾀｰｹﾞｯﾄﾓｰﾄﾞ変化あり */
	#endif
	{
		pw_tarmode = tarmode;
		pw_power_seq = CPW_SEQEND;
	}
	
	#if _WDT_TEST_
	while(1)
	{
		;
	}
	#endif
}
/******************************************************************************
	関 数 名	：Pw_set_item
	引   数		：無し
	戻 り 値	：無し
	機 能 概 略	：電源実行ITEM処理NOをｾｯﾄ処理
------------------------------------------------------------------------------
修正履歴：
[sun060401]新規作成
******************************************************************************/
static	void	Pw_set_item(void)
{
	BYTE	item;

	#if _POWER_TEST_COLDS_
	if(/*ColdStart*/)
	{
		pw_item = CPW_COLDS;
	}
	else
	#endif
	/* BATT OFF以外の場合はITEMﾁｪﾝｼﾞ許可あればITEM設定へ */
	if (pw_power_seq == CPW_SEQEND)
	{
		fpw_itemchg_enb = OFF;
		pw_item = tpw_item_tbl[pw_tarmode][pw_curmode];
		pw_power_seq = CPW_SEQST;
	}
	
}


#if _POWER_TEST_COLDS_
/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_ColdS
		引  数	：無し
		戻り値	：無し
		機  能	：Pw_ColdS時の処理
------------------------------------------------------------------------------
[chin20120803]新規作成
-----------------------------------------------------------------------------*/
static	BYTE	Pw_ColdS(void)
{
	//シーケンスが必要かどうか
}
#endif

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_HoldX
		引  数	：無し
		戻り値	：無し
		機  能	：HoldXﾓｰﾄﾞ中処理
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
static	BYTE Pw_HoldX(void)
{
	fpw_acc_nor = ON;
	pw_power_seq = CPW_SEQEND;
	fpw_itemchg_enb = ON;
	return(CPW_SEQCONT);
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_HoldX_In
		引  数	：無し
		戻り値	：無し
		機  能	：HoldXモードに遷移処理
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
/* ACC OFF(通常) → HOLDX */
static	BYTE Pw_HoldX_In(void)
{
	#if 1
	switch(pw_power_seq)
	{
		case 1:
			Ma_set_Msg( CMA_POWER, CMSG_HOLDX_IN );
			pw_power_seq ++;
			break;
		case 2:
			pw_curmode = CPW_HOLDX;
			pw_power_seq = CPW_SEQEND;
			fpw_almdet_active = OFF;
			if (IO_In_acc_det() == HIGH)
			{
				Io_port_holdx_in();
				Io_int_holdx_in();
				Io_sfr_holdx_in();
			}
			else
			{
				fpw_acc_nor = ON;
				fpw_accdet_active = ON;
			}
			fpw_itemchg_enb = ON;
			break;
		default:
			pw_power_seq = CPW_SEQEND;
			break;
	}
	return(CPW_RETMAIN);
	#else
	Ma_set_Msg( CMA_POWER, CMSG_HOLDX_IN );
	Pw_almkey_clear();
	pw_curmode = CPW_HOLDX;
	pw_power_seq = CPW_SEQEND;
	Io_int_holdx_in();
	Io_sfr_holdx_in();
	#endif
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_HoldX_Out
		引  数	：無し
		戻り値	：無し
		機  能	：HoldXモードからぬける処理（PowerOffに遷移）
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
/* HOLDX → ACC OFF(通常) */
static	BYTE Pw_HoldX_Out(void)
{
	#if 0
	switch(pw_power_seq)
	{
		case 1:
			pw_power_seq ++;
			//Io_int_holdx_out();
			//Io_sfr_holdx_out();
			break;
		case 2:
			Ma_set_Msg( CMA_POWER, CMSG_HOLDX_OUT );
			pw_curmode = CPW_ACCOFF;
			pw_power_seq = CPW_SEQEND;
			break;
		default:
			pw_power_seq = CPW_SEQEND;
			break;
	}
	return(CPW_RETMAIN);
	#else
	#if 0 /* 割り込み処理で実施するため、削除 */
	Io_int_holdx_out();
	Io_sfr_holdx_out();
	Io_port_holdx_out();
	#endif
	
	Ma_set_Msg( CMA_POWER, CMSG_HOLDX_OUT );
	pw_curmode = CPW_ACCOFF;
	pw_power_seq = CPW_SEQEND;
	fpw_itemchg_enb = ON;
	return(CPW_RETMAIN);
	#endif
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_AccOff
		引  数	：無し
		戻り値	：無し
		機  能	：AccOff中の通常状態処理
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
static	BYTE Pw_AccOff(void)
{
	fpw_itemchg_enb = ON;
	if ((fpw_accdet_active == ON)||(fpw_almdet_active == ON))
	{
		return (CPW_RETMAIN);
	}
	
	#if 1
	if (Pw_holdX_in_check() == TRUE )
	{
		fpw_acc_nor = OFF;		/* holdXへ移行要求 */
	}
	#endif
	return (CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_AccOff_In
		引  数	：無し
		戻り値	：無し
		機  能	：AccOff中の通常状態に遷移処理
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
/* ACC ON(POWER OFF) → ACC OFF(通常) */
static	BYTE Pw_AccOff_In(void)
{
	switch(pw_power_seq)
	{
		case 1:
			Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_OFFREQ );
			pw_seq_timer = (30+1);
			pw_power_seq ++;
			break;
		case 2:
			if (pw_seq_timer == 0)
			{
				MIO_OUT_pSYS_ON(LOW);
				IO_SetIICPort(OFF);
				pw_curmode = CPW_ACCOFF;
				pw_power_seq = CPW_SEQEND;
				Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_OFF );
				Key_nop();
				Key_check_start();
				Io_port_accoff_in();
				Io_int_accoff_in();
				Io_sfr_accoff_in();
			}
			break;
		default:
			pw_power_seq = CPW_SEQEND;
			break;
	}
	return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_AccOff_Out
		引  数	：無し
		戻り値	：無し
		機  能	：AccOff中の通常状態からぬける処理（HoldXモードに遷移）
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
/* ACC OFF(通常) → ACC ON(POWER OFF) */
static	BYTE Pw_AccOff_Out(void)
{
	switch(pw_power_seq)
	{
		case 1:
			Io_int_accoff_out();
			Io_sfr_accoff_out();
			pw_power_seq ++;
			break;
		case 2:
			MIO_OUT_pSYS_ON(HIGH);				/* SYS_ON */
			IO_SetIICPort(ON);
			Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_ON );
			pw_power_seq = CPW_SEQEND;
			pw_curmode = CPW_PWROFF;
			break;
		default:
			pw_power_seq = CPW_SEQEND;
			break;
	}
	return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_PwrOff
		引  数	：無し
		戻り値	：無し
		機  能	：PowerOFFモード中処理
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
static	BYTE Pw_PwrOff(void)
{
	#if 0
	accoff_check
	if ()	/* AccOff中 */
	{
		if！(/* アラームBEEP鳴動中||アラーム調整中) */)
		{
			
		}
	}
	#endif
	
	fpw_itemchg_enb = ON;
	pw_power_seq = CPW_SEQEND;
	Key_check_start();
	return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_PwrOn_Out
		引  数	：無し
		戻り値	：無し
		機  能	：PowerONモードからぬける中処理（PowerOffに遷移）
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
static	BYTE Pw_PwrOn_Out(void)
{
	switch(pw_power_seq)
	{
		case	1:
			#if 0
			if (Aud_get_beep_type() == CAUD_BEEP_STA_OFF)
			#endif
			{
				fpw_itemchg_enb = OFF;
				fpw_mute = ON;
				Aud_set_prg_mute(CAUD_MUTE_PWR_DT_MUTE,ON);
				pw_seq_timer = (10+1);
				Key_check_stop();
				pw_power_seq++;
			}
			break;
		case	2:
			if (pw_seq_timer == 0x00)
			{
				Ma_set_Msg( CMA_POWER, CMSG_PWROFF );
				Key_check_start();
				MIO_OUT_pSTBY(LOW);
				pw_curmode = CPW_PWROFF;
				pw_power_seq = CPW_SEQEND;
				fpw_itemchg_enb = ON;
			}
			break;
		default:
			pw_power_seq = CPW_SEQEND;
			break;
	}
	return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_PwrOn_In
		引  数	：無し
		戻り値	：無し
		機  能	：PowerONモードに遷移処理
------------------------------------------------------------------------------
[chin20120725]新規作成
備考：	PowerONを確定してから、50ms後表示更新するため、表示のインターフェースが必要
		
-----------------------------------------------------------------------------*/
static	BYTE Pw_PwrOn_In(void)
{
	switch(pw_power_seq)
	{
		case	1:
			Ma_set_Msg( CMA_POWER, CMSG_PWRON );
			Key_check_stop();
			pw_seq_timer = (50+1);
			pw_power_seq++;
			break;
		case	2:
			if (pw_seq_timer == 0)
			{
				MIO_OUT_pSTBY(HIGH);
				#if _POWER_DIAG_
				Pw_DIAG_clear();
				#endif	/* _POWER_DIAG_ */
				if (fpw_wakeup == TRUE)
				{
					Aud_set_beep(CAUD_REQ_BEEP_ALARM);
				}
				Key_check_start();
				Aud_set_prg_mute(CAUD_MUTE_PWR_DA_MUTE,OFF);
				Aud_set_prg_mute(CAUD_MUTE_PWR_DT_MUTE,OFF);
				pw_power_seq = CPW_SEQEND;
				pw_curmode = CPW_PWRON;
			}
			break;
		case	3:
		
		default:
			pw_power_seq = CPW_SEQEND;
			break;
	}
	return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Pw_PwrOn
		引  数	：無し
		戻り値	：無し
		機  能	：PowerON中処理
------------------------------------------------------------------------------
[chin20120725]新規作成
-----------------------------------------------------------------------------*/
static	BYTE Pw_PwrOn(void)
{
	pw_power_seq = CPW_SEQEND;
	Key_check_start();
	fpw_itemchg_enb = ON;
	return(CPW_RETMAIN);
}

/******************************************************************************
	関数名	：pw_initial
	引  数	：無し
	戻り値	：無し
	機  能	：電源制御ﾓｼﾞｭｰﾙ初期化処理
------------------------------------------------------------------------------
[chin20120725]新規作成
備考：	システムメッセージは発行しません。
		HoldX安定状態になるまで、ACC_DET禁止（フラグか、時間を待つか）
		IOモジュールのインターフェースを呼び出す（ColdStart時の関数）
******************************************************************************/
void	Pw_initial(void)
{
	/* ACCOFFモードにしておく*/
	pw_curmode = CPW_ACCOFF;		/* AccOffﾓｰﾄﾞからｽﾀｰﾄ */
	
	#if 0
	Io_int_initial_set();
	#endif
	pw_power_seq = CPW_SEQEND;
	fpw_acc_nor = ON;
	fpw_accdet_active = ON;		/* Accチャタリングを行う */
	Lcd_accoff_send_req();
	#if 0 /* 使用しないため、削除 */
	pw_accwait_timer = CPW_ACCWATI_TIM;		/* ACCﾁｬﾀﾘﾝｸﾞ中HALTへ遷移しないように */
	#endif
	fpw_itemchg_enb = ON;
	
	pw_AccOn_4mscnt = 0;
	pw_accon_chat_cnt = 0;
	pw_accoff_chat_cnt = 0;
	pw_almkey_cnt = 0;
}

/******************************************************************************
	関数名	：Pw_holdX_in_check
	引  数	：無し
	戻り値	：TRUE →	HOLDXに遷移
			：FALSE→	HOLDXに遷移しない
	機  能	：accoff状態からHOLDXに行くかどうかのチェック処理
------------------------------------------------------------------------------
[chin20120827]新規作成
******************************************************************************/
static	BYTE	Pw_holdX_in_check(void)
{
	BYTE ret;
	
	ret = TRUE;
	
	#if 0
	if( Alm_mode_check() == TRUE )
	{
		ret = FALSE;			/* アラーム調整中ではない */
	}
	#endif
	
	if (Lcd_accoff_send_end() == TRUE )
	{
		ret = FALSE;			/* 表示更新済み */
	}
	return ret;
}
#undef  _PW_MAIN_C_
