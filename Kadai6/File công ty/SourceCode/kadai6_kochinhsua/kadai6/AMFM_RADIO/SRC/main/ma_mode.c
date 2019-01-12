/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: ma_mode.c
		機能			: ﾒｲﾝﾓｰﾄﾞ監視処理
						: ①ﾒｲﾝﾓｰﾄﾞ切替監視処理
						: ②ﾓｰﾄﾞ切替MUTE制御処理
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun070313]	変更
******************************************************************************/
#define	_MA_MODE_C_

#include	"../model.h"
#include	"../comm/common.h"
#include	"../audio/aud_ext.h"
#include	"../power/pw_func_ext.h"
#include	"../tuner/tu_ext.h"

#if _MODEL_EEPROM_
#include	"../eeprom/eeprom_ext.h"
#endif

#include	"main.h"
#include	"ma_ext.h"
#include	"ma_mode.h"

/*-----------------------------------------------------------
          関数ﾌﾟﾛﾄﾀｲﾌﾟ
-----------------------------------------------------------*/
static BYTE Ma_next_mode_chg(TMA_MODE, BYTE);
static TMA_MODE Ma_search_next_md(TMA_MODE );
static TMA_MODE	Ma_lastmode_get(void);
static void Ma_mode_chgst(void);
static	void Ma_lastsave(TMA_MODE );

/*-----------------------------------------------------------
         ﾒｲﾝﾓｰﾄﾞ条件ﾃｰﾌﾞﾙ
[sun061114] USB INでﾓｰﾄﾞ切替/PowerONしないように仕様変更対応
[sun070410] RDSINT PowerOFFでﾓｰﾄﾞ解除するように変更
			RDSモジュールでPowerOFF⇒ONで強制割込み状態解除しているため。
[sun100624] HDRadioとｼﾘｳｽﾓｰﾄﾞ追加
[sun100816] TunerMode中HD　Radio接続ONでHD　Radioに切り替えしない
			不具合修正
-----------------------------------------------------------*/
#define		CMA_TBLEND	0xff
const	TMA_MODE_COND	ma_mdcond_tbl[] =
{
	/* 								    LAST	ONInt	OffInt	PwrOn	PwrOff	OffKeep */
	{CMA_TUNER,		CMA_PRIO_LV0,		YES,	YES,	NO,		NO,		NO,		YES,},
	{CMA_AUX,		CMA_PRIO_LV0,		YES,	YES,	NO,		NO,		NO,		YES,},
	{CMA_ISR,		CMA_PRIO_LV1,		NO,		YES,	YES,	NO,		NO,		NO,},
	{CMA_TBLEND,	CMA_TBLEND,			NO,		NO,		NO,		NO,		NO,		NO,},
};

/******************************************************************************
.. 		関数名  ：Ma_main
		引  数	：無し
		戻り値	：無し
		機  能	：ﾒｲﾝｺﾝﾄﾛｰﾙ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051129] 関数名のみ
[sun060710] PowerOFF中ﾓｰﾄﾞﾁｪﾝｼﾞﾒｯｾｰｼﾞ発行しないように変更
[sun070313]	変更
[sun070524] ﾗｽﾄﾓｰﾄﾞ記憶するように変更
******************************************************************************/
void	Ma_mode_main(void)
{
	if (fma_mdchg == ON)
	{
		if(ma_mutetimer == TIMER_OUT)
		{
			fma_mdchg = OFF;
//			ma_last_mode = ma_mode[CMA_CUR_MD].cond.mode;	/* ｶﾚﾝﾄﾓｰﾄﾞﾗｽﾄに記憶 */
			ma_mode[CMA_CUR_MD] = ma_mode[CMA_TARGET_MD];
			
			if (Pw_Power_status_get() == ON)
			{
				Ma_set_Msg(CMA_MODE,CMSG_MODECHG);	/* ﾓｰﾄﾞ切り替えﾒｯｾｰｼﾞ発行 */
			}
			ma_mutetimer = CMA_AFTERMUTE;		/* ﾓｰﾄﾞ切り替えｱﾌﾀMUTEﾀｲﾏｾｯﾄ */
		}
	}
	else if ((ma_mutetimer == TIMER_OUT) &&
			 (fma_mute == ON))
	{
		fma_mute = OFF;
		Aud_set_prg_mute(CAUD_MUTE_MAIN_DT_MUTE,OFF);
	}
	else
	{
		;
	}
}

/******************************************************************************
.. 		関数名  ：Ma_source_chg
		引  数	：なし
		戻り値	：無し
		機  能	：ｿｰｽｷｰによりﾓｰﾄﾞ切り替え
				：①ｶﾚﾝﾄﾓｰﾄﾞﾁｪｯｸ
				：  ｶﾚﾝﾄﾓｰﾄﾞがDM⇒ﾗｽﾄﾓｰﾄﾞに切り替え
				：  ｶﾚﾝﾄﾓｰﾄﾞが優先ﾓｰﾄﾞ ⇒ﾗｽﾄモードの切替
				：  ｶﾚﾝﾄﾓｰﾄﾞが通常ﾓｰﾄﾞ ⇒次のﾓｰﾄﾞに切替
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060123] 新規作成
[sun060620] ｱﾗｰﾑﾓｰﾄﾞ中TUNERに切替たら、TUNERへ切替するように変更
[sun070313]	変更
[sun070402] ｶﾚﾝﾄﾓｰﾄﾞがLV1の場合は、ﾓｰﾄﾞOFFしﾗｽﾄﾓｰﾄﾞへ切り替え
			ISRﾓｰﾄﾞ中にsrcｷｰ ⇒ ISRﾓｰﾄﾞOFFして、ﾗｽﾄﾓｰﾄﾞへ遷移
			RDSINTﾓｰﾄﾞ中srcｷｰ⇒ RDSINTﾓｰﾄﾞOFFして、ﾗｽﾄﾓｰﾄﾞへ遷移
[sun070709] 最優先ﾓｰﾄﾞ[現状TELのみ]でｿｰｽ切替無効に変更
******************************************************************************/
void Ma_source_chg(void)
{
	TMA_MODE	cur_mode;
	TMA_MODE	next_mode;
	BYTE		iRet;

	cur_mode = ma_mode[CMA_TARGET_MD];

	/* 切替元ﾓｰﾄﾞｾｯﾄ */
	if (cur_mode.cond.prio == CMA_PRIO_LV2)		/* [sun070709] 最優先ﾓｰﾄﾞでｿｰｽ切替無効 */
	{
		return;
	}
	else if (cur_mode.cond.prio > CMA_PRIO_LV0)		/* ｶﾚﾝﾄﾓｰﾄﾞは優先ﾓｰﾄﾞSRCｷｰでﾗｽﾄﾓｰﾄﾞへ遷移 */
	{
		ma_mode_cont[cur_mode.id] = CMA_NOCONN;	/* ｶﾚﾝﾄﾓｰﾄﾞOFFに */
		next_mode = ma_mode[CMA_LAST_MD];		/* ﾗｽﾄﾓｰﾄﾞに切り替え */
	}
	else
	{
		next_mode = Ma_search_next_md(cur_mode);
		Ma_modetbl_srh(&next_mode);
	}
	iRet = Ma_next_mode_chg(next_mode, CMA_PRIO_NO);

	if (iRet == OK)
	{
		Ma_mode_chgst();
	}
}

/******************************************************************************
.. 		関数名  ：Ma_mode_pwroff
		引  数	：無し
		戻り値	：無し
		機  能	：PowerOffﾓｰﾄﾞﾁｪｯｸ処理
				：ｶﾚﾝﾄﾓｰﾄﾞPowerOFFで解除するかﾁｪｯｸを行う処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun070410] 新規作成
[sun090403] EEPROMへﾗｽﾄﾓｰﾄﾞ書込み要求追加
[sun090409] 左ｼﾌﾄ入力ﾐｽ修正
[sun090411] EEPROMへの格納はDCP/ACC/BATT　OFFに変更
******************************************************************************/
void Ma_mode_pwroff(void)
{
	TMA_MODE	cur_mode;
	
	cur_mode = ma_mode[CMA_TARGET_MD];

	/* PowerOFFで解除するﾓｰﾄﾞかﾁｪｯｸ */
	if(cur_mode.cond.off_keep == NO)
	{
		/* ｶﾚﾝﾄﾓｰﾄﾞOFFｲﾍﾞﾝﾄ発行 */
		Ma_mode_event_chk(cur_mode.id,cur_mode.cond.mode,CMA_MODEOFF);
	}
}

/******************************************************************************
.. 		関数名  ：Ma_mode_event_chk
		引  数	：modeid ⇒ ﾓｰﾄﾞID番号
				：mode ⇒ ﾓｰﾄﾞ
				：event ⇒ ｲﾍﾞﾝﾄ種類
		戻り値	：無し
		機  能	：ﾓｰﾄﾞｲﾍﾞﾝﾄﾁｪｯｸ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060307] 新規作成
[sun070313]	変更
[sun070628] USB有り無し判断関数に変更
[sun100812] TunerMode中HDRadio　ONであれば、HD　Radioへ遷移
			HD　Radio中HD　Radio　OFFであれば、Tunerへ遷移
******************************************************************************/
void Ma_mode_event_chk(BYTE mode_id, BYTE mode, BYTE event)
{
	/* 制御ﾊﾞｯﾌｧ更新 */
	switch(event)
	{
		case	CMA_CONN:
			ma_mode_cont[mode_id] = mode;
			break;
		case	CMA_MODEON:
			ma_mode_cont[mode_id] = mode;
			Ma_mode_on_event(mode_id,mode);
			break;
		case	CMA_MODEOFF:
			ma_mode_cont[mode_id] = CMA_NOCONN;
			Ma_mode_off_event(mode_id,mode);
			break;
		case	CMA_LOAD:
			ma_mode_cont[mode_id] = mode;
			Ma_mode_on_event(mode_id,mode);
			break;
		default:
			break;
	}
}
/******************************************************************************
.. 		関数名  ：Ma_mode_on_event
		引  数	：modeid ⇒ ﾓｰﾄﾞID番号
				：mode ⇒ ﾓｰﾄﾞ
		戻り値	：無し
		機  能	：ﾓｼﾞｭｰﾙOn通知処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060307] 新規作成
[sun060605] ﾐｽ1: OnｲﾍﾞﾝﾄID既にﾀｰｹﾞｯﾄになっているかﾁｪｯｸ追加
			連続ｱﾗｰﾑON、TA ONが来ると何回も受付しちゃう不具合
[sun060620] ﾐｽ2：MP3ﾓｰﾄﾞでLoadingでPowerONできない不具合
			同じﾓｰﾄﾞでPowerOnﾁｪｯｸしていなかった。
[sun060719]-1 ｶﾚﾝﾄﾓｰﾄﾞIDとEventﾓｰﾄﾞIDが同じで変わったﾓｰﾄﾞの要求が受付無い不具合修正
            ID比較ではなくｶﾚﾝﾄﾓｰﾄﾞとeventﾓｰﾄﾞで比較するように変更
[sun060913] USB Update中MP3 LOADでﾓｰﾄﾞ遷移しないよう変更
[sun070313]	変更
[sun090307] USBｺﾝﾊﾟｲﾙSW追加
******************************************************************************/
void Ma_mode_on_event(BYTE	mode_id ,BYTE mode)
{
	TMA_MODE	event_mode;
	TMA_MODE	cur_mode;
	BYTE	iRet;

	event_mode.id = mode_id;
	event_mode.cond.mode = mode;
	cur_mode = ma_mode[CMA_TARGET_MD];

	Ma_modetbl_srh(&event_mode);					/* ｲﾍﾞﾝﾄﾓｰﾄﾞﾃｰﾌﾞﾙｻｰﾁ */

	iRet = NG;
	if (event_mode.cond.onint != NO)				/* ﾓｰﾄﾞONで切替するか */
	{
		if (cur_mode.cond.mode != event_mode.cond.mode)		/* [sun060719]-1ｶﾚﾝﾄﾓｰﾄﾞと同じか */
		{
			iRet = Ma_next_mode_chg(event_mode, CMA_PRIO_CHK);
		}
	}

	if (event_mode.cond.pwr_on)
	{
		Pw_power_event(mode, ON);
	}

	/* EventModeへ切替発生 */
	if (iRet == OK)
	{
		Ma_mode_chgst();
	}
}
/******************************************************************************
.. 		関数名  ：Ma_mode_off_event
		引  数	：mode_id ⇒ ﾓｰﾄﾞID番号
				：mode ⇒ ﾓｰﾄﾞ
		戻り値	：無し
		機  能	：ﾓｼﾞｭｰﾙOff通知処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060307] 新規作成
[sun060719]-1 ｶﾚﾝﾄﾓｰﾄﾞIDとEventﾓｰﾄﾞIDが同じで変わったﾓｰﾄﾞの要求が受付無い不具合修正
            ID比較ではなくｶﾚﾝﾄﾓｰﾄﾞとeventﾓｰﾄﾞで比較するように変更
[sun070313]	変更
******************************************************************************/
void Ma_mode_off_event(BYTE	mode_id, BYTE mode)
{
	TMA_MODE	cur_mode;
	TMA_MODE	lst_mode;

	cur_mode = ma_mode[CMA_TARGET_MD];

	if ((cur_mode.cond.mode != mode) ||		/* [sun060719]-1 ｶﾚﾝﾄﾓｰﾄﾞと同じか */
	    (cur_mode.cond.offint == NO))		/* ﾓｰﾄﾞOFFで切替するか */
	{
		return;								/* 更新のみでSRC切替しない */
	}

	lst_mode = Ma_lastmode_get();
	Ma_next_mode_chg(lst_mode,CMA_PRIO_NO);

	if (cur_mode.cond.pwr_off == YES)
	{
		Pw_power_event(cur_mode.cond.mode,OFF);
	}

	Ma_mode_chgst();
}
/******************************************************************************
.. 		関数名  ：Ma_next_mode_chg
		引  数	：next_mode ⇒ 目標ﾓｰﾄﾞ
				：status ⇒ 優先ﾚﾍﾞﾙﾁｪｯｸ必要かどうか
		戻り値	：OK ⇒ 目標ﾓｰﾄﾞﾁｪﾝｼﾞした
				：NG ⇒ 目標ﾓｰﾄﾞﾁｪﾝｼﾞしていない
		機  能	：目標ﾓｰﾄﾞｾｯﾄ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060307] 新規作成
[sun060713] TunerのTA受信中優先ﾚﾍﾞﾙ1に設定
[sun060802] ﾚﾋﾞｭｰ指摘修正 == ⇒ = に変更
			TUNER TA以外の場合はﾚﾍﾞﾙをLVOにしていた、元のままで保持する
[sun060930] TunerﾓｰﾄﾞでTA優先順位となってしまうとソースｷｰ聞かなくなる
[sun070313]	変更
[sun070402] 優先ﾚﾍﾞﾙがLV1で次のﾓｰﾄﾞへの遷移が発生したら、ﾓｰﾄﾞOFFとするように
			変更
			ISRﾓｰﾄﾞ中にCD Loading⇒ISRﾓｰﾄﾞOFFして、MP3ﾓｰﾄﾞへ
			RDSINTﾓｰﾄﾞ中にCD Loading⇒RDSINT OFFして、MP3ﾓｰﾄﾞへ
[sun070525] TELすばやく瞬断すると、TEL切替しないでCD戻るので、Reloadしてしまう
			ﾀｰｹﾞｯﾄ更新したら、ﾗｽﾄも前回ﾀｰｹﾞｯﾄを記憶するように変更
******************************************************************************/
static BYTE	Ma_next_mode_chg(TMA_MODE next_mode, BYTE status)
{
	BYTE	iRet;
	TMA_MODE	cur_mode;

	cur_mode = ma_mode[CMA_TARGET_MD];
	/* ﾓｰﾄﾞ切替ﾁｪｯｸ */
	iRet = NG;

	if ((status == CMA_PRIO_NO) ||						/* 優先ﾁｪｯｸ無し */
		(next_mode.cond.prio >= cur_mode.cond.prio))		/* 目標ﾓｰﾄﾞがｶﾚﾝﾄﾓｰﾄﾞより優先 */
	{
		if (cur_mode.cond.lastsave == ON)
		{
			Ma_lastsave(cur_mode);
		}
		iRet = OK;
	}
	else if (cur_mode.cond.prio == CMA_PRIO_LV1)		/* [sun070402] ｶﾚﾝﾄﾓｰﾄﾞLV1「ISR/RDSINT」 */
	{
		ma_mode_cont[cur_mode.id] = CMA_NOCONN;			/* ｶﾚﾝﾄﾓｰﾄﾞOFFに */
		iRet = OK;
	}
	else
	{
		if (next_mode.cond.lastsave == ON)
		{
			Ma_lastsave(next_mode);
		}
	}

	if (iRet == OK)
	{
		ma_last_mode = ma_mode[CMA_TARGET_MD].cond.mode;	/* ｶﾚﾝﾄﾓｰﾄﾞﾗｽﾄに記憶 */
		ma_mode[CMA_TARGET_MD] = next_mode;	/* 目標ﾓｰﾄﾞｾｯﾄ */
	}
	return(iRet);
}

/******************************************************************************
.. 		関数名  ：Ma_search_next_md
		引  数	：cur_id ⇒ 指定idの次の有効ﾓｰﾄﾞｻｰﾁ
				：*next_id ⇒ NEXTﾓｰﾄﾞ格納ｴﾘｱ
				：*next_mode_tbl ⇒ NEXTﾓｰﾄﾞﾃｰﾌﾞﾙﾃﾞｰﾀ格納ｴﾘｱ
		戻り値	：
		機  能	：ｿｰｽｷｰにより次の切替ﾓｰﾄﾞｻｰﾁ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060307] 新規作成
******************************************************************************/
static TMA_MODE Ma_search_next_md(TMA_MODE cur_mode)
{
	BYTE	iLoop;
	TMA_MODE	next_mode;

	if (cur_mode.id >= CMA_SRC_MAX)
	{
		next_mode.id = 0;				/* 指定不正で、強制先頭をｾｯﾄ*/
		next_mode.cond.mode = ma_mode_cont[0];
		return(next_mode);
	}

	iLoop = cur_mode.id;
	iLoop++;
	while (iLoop != cur_mode.id)
	{
		if (iLoop >= CMA_SRC_MAX)
		{
			iLoop = 0;
		}
		if (ma_mode_cont[iLoop] != CMA_NOCONN)
		{
			break;
		}
		iLoop ++;
	}

	if (iLoop == cur_mode.id)
	{
		iLoop = 0;					/* next見付らない、先頭をｾｯﾄ */
	}

	next_mode.id = iLoop;
	next_mode.cond.mode = ma_mode_cont[iLoop];
	return(next_mode);
}

/******************************************************************************
.. 		関数名  ：Ma_modetbl_srh
		引  数	：mode ⇒ ｻｰﾁ目標ﾓｰﾄﾞ
				：* iTblCnt ⇒ ﾃｰﾌﾞﾙ番号格納変数ｱﾄﾞﾚｽ
				：* mode_tbl⇒ ﾃｰﾌﾞﾙﾃﾞｰﾀ格納変数ｴﾘｱ
		戻り値	：OK ⇒ 見つかった
				：NG ⇒ 見つからなかった
		機  能	：ﾓｰﾄﾞﾃｰﾌﾞﾙｻｰﾁ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060307] 新規作成
******************************************************************************/
void Ma_modetbl_srh( TMA_MODE *mode_tbl )
{
	BYTE	iRet;
	BYTE	iLoop1;
	BYTE	mode;

	mode = mode_tbl->cond.mode;

	iRet = NG;
	/* ｶﾚﾝﾄﾓｰﾄﾞｻｰﾁ */
	iLoop1 = 0;
	while (ma_mdcond_tbl[iLoop1].mode != CMA_TBLEND)
	{
		if (ma_mdcond_tbl[iLoop1].mode == mode)
		{
			mode_tbl->cond = ma_mdcond_tbl[iLoop1];
			iRet = OK;
			break;
		}
		iLoop1 ++;
	}
	/* ﾃｰﾌﾞﾙｻｰﾁできなかった場合は、先頭ﾃﾞｰﾀを設定 */
	if (iRet == NG)
	{
		mode_tbl->id = 0;
		mode_tbl->cond = ma_mdcond_tbl[0];
	}
}

/******************************************************************************
.. 		関数名  ：Ma_lastmode_get
		引  数	：なし
		戻り値	：ﾗｽﾄﾓｰﾄﾞ
		機  能	：ﾗｽﾄﾓｰﾄﾞｻｰﾁ処理処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060308] 新規作成
******************************************************************************/
static TMA_MODE	Ma_lastmode_get(void)
{
	TMA_MODE	lastmode;
	/* QAC対策 */
	lastmode.id = ma_mode[CMA_LAST_MD].id;
	lastmode.cond.mode = ma_mode_cont[lastmode.id];
	Ma_modetbl_srh(&lastmode);
	return(lastmode);
}

/******************************************************************************
.. 		関数名  ：Ma_mode_chgst
		引  数	：無し
		戻り値	：無し
		機  能	：ﾓｰﾄﾞ切替ｽﾀｰﾄ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060308] 新規作成
[sun060509] ﾓｰﾄﾞﾁｪﾝｼﾞMUTE解除待ち間にMUTEﾀｲﾏ更新していないので、解除ﾀｲﾏ切れるまで
			ﾓｰﾄﾞ切替しなくなる不具合修正
[sun070822] Tuner間での切替はMuteかけないように修正
[sun101216] Mode切替発生する前にBT Fast再生解除Request
******************************************************************************/
static void Ma_mode_chgst(void)
{
	if (fma_mdchg == OFF)
	{
		if (Pw_Power_status_get() == ON)
		{
			if (fma_mute == OFF)
			{
				ma_mutetimer = CMA_FRONTMUTE;
			}
			else
			{
				ma_mutetimer = CMA_FRONTMUTE_50;
			}
		}
		else
		{
			ma_mutetimer = 0;
		}
	}

	fma_mdchg = ON;
	fma_mute = ON;
	Aud_set_prg_mute(CAUD_MUTE_MAIN_DT_MUTE,ON);
}

/******************************************************************************
.. 		関数名  ：Ma_lastsave
		引  数	：なし
		戻り値	：無し
		機  能	：ﾗｽﾄﾓｰﾄﾞｾｯﾄ
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060308] 新規作成
******************************************************************************/
static	void Ma_lastsave(TMA_MODE lst_mode)
{
	BYTE	iLoop1;
	BYTE	iLoop2;

	/* ﾗｽﾄﾓｰﾄﾞと同じかﾁｪｯｸ */
	if (ma_mode[CMA_LAST_MD].id == lst_mode.id)
	{
		if (ma_mode[CMA_LAST_MD].cond.mode != lst_mode.cond.mode)
		{
			ma_mode[CMA_LAST_MD].cond = lst_mode.cond;
		}
		return;
	}

	/* 同じﾓｰﾄﾞﾗｽﾄに入ってあるかﾁｪｯｸ */
	for (iLoop1 = CMA_LAST_MD; iLoop1 < CMA_MAX; iLoop1 ++)
	{
		if (lst_mode.id == ma_mode[iLoop1].id)
		{
			break;
		}
	}
	if (iLoop1 >= CMA_MAX)
	{
		iLoop1 = (CMA_MAX - 1);
	}
	/* ﾗｽﾄ入れ替え */
	for (iLoop2 = iLoop1-1; iLoop2 >= CMA_LAST_MD; iLoop2--,iLoop1--)
	{
		ma_mode[iLoop1] = ma_mode[iLoop2];
	}
	/* 引数ﾓｰﾄﾞﾗｽﾄに入れ替え */
	ma_mode[CMA_LAST_MD] = lst_mode;
	
}


#undef		_MA_MODE_C_
