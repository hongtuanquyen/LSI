/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: ma_mode.h
		機能			: ①ﾒｲﾝﾓｼﾞｭｰﾙﾓｰﾄﾞ監視処理ﾍｯﾀﾞﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128]	新規作成
******************************************************************************/
#ifndef		_MA_MODE_H_
#define		_MA_MODE_H_

#include	"../model.h"
#include	"../main/ma_ext.h"

#undef	EXT
#ifdef	_MA_MODE_C_
#define	EXT
#else
#define	EXT	extern
#endif


/*----------------------------------------------------
   ﾓｰﾄﾞ切り替え条件定義ﾃｰﾌﾞﾙ
----------------------------------------------------*/
typedef	 struct{
	BYTE	mode;				/* ﾒｲﾝﾓｰﾄﾞ */
	BYTE	prio;				/* 優先順位 0x00→優先無し、 0x01→優先ﾚﾍﾞﾙ1、数字高いほど優先度高い */
	BYTE	lastsave:1;			/* ﾗｽﾄﾓｰﾄﾞに記憶するか YES⇒ﾗｽﾄ記憶する  NO⇒ﾗｽﾄ記憶しない */
	BYTE	onint:1;			/* 割込みONでﾓｰﾄﾞ切替ﾓｰﾄﾞか  YES⇒割込みONで切替  NO⇒割込みで切替しない */
	BYTE	offint:1;			/* 割込みOFFでﾓｰﾄﾞ切替ﾓｰﾄﾞか  YES⇒割込みOFFで切替  NO⇒割込みで切替しない */
	BYTE	pwr_on:1;			/* ﾓｰﾄﾞONでPower ONさせるか  YES⇒させる NO⇒させない */
	BYTE	pwr_off:1;			/* ﾓｰﾄﾞOFFでPower OFFさせるか  YES⇒させる  No⇒させない */
	BYTE	off_keep:1;			/* Power Off 中ﾓｰﾄﾞ継続されるか  Yes⇒PowerOffでもMainﾓｰﾄﾞ保持  No⇒PowerOffでﾓｰﾄﾞ解除 */
}TMA_MODE_COND;

/*----------------------------------------------------
   ﾒｲﾝﾓｰﾄﾞ定義
----------------------------------------------------*/
typedef	struct{
	BYTE	id;
	TMA_MODE_COND	cond;
}TMA_MODE;

/*----------------------------------------------------
   関数プロトタイプ
----------------------------------------------------*/
EXT	void	Ma_mode_main(void);
EXT	void	Ma_source_chg(void);
EXT void 	Ma_mode_on_event(BYTE , BYTE );
EXT void 	Ma_mode_off_event(BYTE , BYTE  );
EXT	void 	Ma_mode_event_chk(BYTE, BYTE, BYTE );
EXT	void	Ma_modetbl_srh(TMA_MODE * );
EXT	void	Ma_mode_pwroff(void);

/*----------------------------------------------------
   	定数定義
----------------------------------------------------*/
#define		CMA_PRIO_LV0		0
#define		CMA_PRIO_LV1		1
#define		CMA_PRIO_LV2		2
#define		CMA_PRIO_LV3		3

#define		CMA_FRONTMUTE		120/10+1
#define		CMA_FRONTMUTE_50	50/10
//#define		CMA_AFTERMUTE		700/10+1
#define		CMA_AFTERMUTE		250/10+1

#define		CMA_PRIO_CHK		1
#define		CMA_PRIO_NO			0

/*----------------------------------------------------
   ﾌﾗｸﾞ定義
----------------------------------------------------*/
EXT	TCOM_FLG	ma_flag;
#define	fma_BlueTooth	(ma_flag.bit.bit3)			/* BlueTooth有り無し 1: あり  0: 無し */
#define	fma_mdchg	(ma_flag.bit.bit2)			/* Mode切替要求 1:要求あり  0:要求無し */
#define	fma_mute	(ma_flag.bit.bit1)			/* MainMute要求  1: Mute要求あり  0:Mute要求無し */

/*----------------------------------------------------
   高速ｱｸｾｽ変数定義
----------------------------------------------------*/
EXT		BYTE	ma_model;						/* 仕向け【値定義ma_ext.h】 */
EXT 	BYTE	ma_dest;						/* 機種名【値定義ma_ext.h】 */
EXT		BYTE	ma_mutetimer;					/* MasterMuteﾀｲﾏ [10msﾍﾞｰｽ] */

#define		CMA_MAX		8
EXT		TMA_MODE	ma_mode[CMA_MAX];			/* Mainﾓｰﾄﾞ履歴管理ﾊﾞｯﾌｧ */
#define		CMA_TARGET_MD		0
#define		CMA_CUR_MD			1
#define		CMA_LAST_MD			2
#define		CMA_MODE_ID			0
#define		CMA_MODE			1

#define		CMA_MD_MAX		CMA_ID_MAX + 1
#define		CMA_SRC_MAX		CMA_ID_SRCMAX + 1
EXT		BYTE	ma_mode_cont[CMA_MD_MAX];		/* Mainｿｰｽ切替制御ﾊﾞｯﾌｧ */
EXT		BYTE	ma_mode_ptr;					/* Mainｿｰｽ切替制御ﾊﾞｯﾌｧﾎﾟｲﾝﾀ */

EXT	BYTE	ma_test_mode;				/* ﾃｽﾄﾓｰﾄﾞ */
EXT	BYTE	ma_last_mode;				/* ﾗｽﾄﾓｰﾄﾞ */

#endif
