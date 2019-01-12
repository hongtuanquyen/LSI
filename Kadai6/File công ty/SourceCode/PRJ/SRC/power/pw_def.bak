/******************************************************************************
	クラリオン（株）殿向け		2007 C3X03 ｼﾘｰｽﾞ[仮]

		プロジェクト名	: '07年フリップダウン　MP3モデル開発
		ファイル名		: pw_def.h
		機能			: ①電源制御ﾓｼﾞｭｰﾙ ﾃﾞｰﾀ型、定数定義ﾍｯﾀﾞｰ･ﾌｧｲﾙ
------------------------------------------------------------------------------
修正履歴
【番号】	日付		担当者	修正内容
【NKC-0000】'05/12/09	仲地	新規作成
******************************************************************************/
#ifndef		_PW_DEF_H_
#define		_PW_DEF_H_

/* ------------------------------------------------------------- */
/* chin20120803                                                  */
/* INITIAL処理だけでは足りなかったら、コールドスタートシーケンスを使用 */
/* ------------------------------------------------------------- */
#define		_POWER_TEST_COLDS_		0

/*----------------------------------------------------
   定数定義
----------------------------------------------------*/
/* ------------------------------------------------------------------------ */
/*	Batt off/on ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値												*/
#define	CPW_BATT_OFF_CHAT_CNT		6	/*	Batt off検出、ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値	*/
//[sun060821] test Version
//#define	CPW_BATT_ON_CHAT_CNT		(BYTE)(1000/MIO_BTIM1_TIM)	/*	Batt on検出、ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値 64*15.625=1sec */
#define	CPW_BATT_ON_CHAT_CNT		2	/*	Batt on検出*/

/* ------------------------------------------------------------------------ */
/*	Acc off/on ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値												*/
#define	CPW_ACC_OFF_CHAT_CNT		4	/*	ACC off検出、ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値		*/
#define	CPW_ACC_ON_CHAT_CNT			40	/*	ACC on検出、ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値		*/
/*	Acc off/on 判定閾値 A/D値		5V÷255×A/D値＝ｶﾚﾝﾄ電圧				*/
#define	CPW_ACC_ON_AD		0xBB	/*	ACC on検出判定  188*(5V/255)=3.665V	*/
//#define	CPW_ACC_ON_AD		0xa0	/*	ACC on検出判定  188*(5V/255)=3.665V	*/
#define	CPW_ACC_OFF_AD		0x8C	/*	ACC off検出判定 141*(5V/255)=2.763V	*/

#if 0
/* ------------------------------------------------------------------------ */
/*	Panel in/out ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値												*/
#define	CPW_PANEL_OUT_CHAT_CNT		2	/*	Panel out検出、ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値	*/
#define	CPW_PANEL_IN_CHAT_CNT		2	/*	Panel in検出、ﾁｬﾀﾘﾝｸﾞ･ｶｳﾝﾄ値	*/

/* ------------------------------------------------------------------------ */
/*	Power 内部状態　シーケンスID											*/
#define		CPW_BATT_ON_SEQ0		0	/*	ｺｰﾙﾄﾞ･ｽﾀｰﾄ時　Power ON			*/
/* ------------------------------------------------------------------------ */
#define		CPW_HALT_IN_MODE		15	/*									*/
#endif

/* ------------------------------------------------------------------------ */
/*		タイマー値															*/
/* ------------------------------------------------------------------------ */
//[sun060821] test Version
//#define	CPW_BATT_ON_TIM				(BYTE)(1000/MIO_BTIM1_TIM)
#define	CPW_BATT_ON_TIM				(BYTE)(500/MIO_BTIM1_TIM)
#define	CPW_HALT_IN_TIM				(BYTE)(1000/MIO_BTIM1_TIM)

/*	ウエイトタイマ値定義		値		内容							*/
#define	CPW_TIMEUP				0		/*	Timer タイムUP					*/
#define	CPW_SEQEND				0
#define	CPW_SEQST				1
#define	CPW_SEQCONT				1
#define	CPW_RETMAIN				0

/*-----------------------------------------------------------------------
   電源状態定義
★★tpw_item_tblの順番になりますので、変更際にtpw_item_tblの確認必要★★
------------------------------------------------------------------------*/
enum
{
	CPW_HOLDX,					/* ACC OFF中のholdXﾓｰﾄﾞ */
	CPW_ACCOFF,					/* ACC OFF中の通常ﾓｰﾄﾞ */
	CPW_PWROFF,					/* POWER OFFﾓｰﾄﾞ */
	CPW_PWRON					/* POWER ONﾓｰﾄﾞ */
};
#if _POWER_TEST_COLDS_
#define		CPW_COLDS	0xFF
#endif
/*-----------------------------------------------------------------------
   処理Item定数定義
------------------------------------------------------------------------*/
enum
{
		CPW_I_HOLDX = 0			/* ACCOFF中のHOLDXモード（安定状態） */
	,	CPW_I_HOLDX_IN				/* ACCOFF中のHOLDXモードに遷移 */
	,	CPW_I_HOLDX_OUT			/* ACCOFF中のHOLDXモードからぬける */
	,	CPW_I_ACCOFF				/* ACCOFF中の通常モード（安定状態） */
	,	CPW_I_ACCOFF_IN			/* ACCOFF中の通常モードに遷移 */
	,	CPW_I_ACCOFF_OUT			/* ACCOFF中の通常モードからぬける */
	,	CPW_I_PWROFF				/* PowerOFF（安定状態） */
	,	CPW_I_PWRON_OUT			/* PowerONからぬける（PowerOFFへ遷移） */
	,	CPW_I_PWRON_IN				/* PowerONに遷移 */
	,	CPW_I_PWRON					/* PowerON（安定状態） */
		#if _POWER_TEST_COLDS_
	,	CPW_I_COLDS					/* COLD START時の処理 */
		#endif
};


/* HOLDX中に割り込み要因種類定義 */

#define		CPW_HOLDX_INTOFF	0x00		/* 要因クリア */
#define		CPW_HOLDX_INT_ACC	0x01		/* ACCDET検出 */
#define		CPW_HOLDX_INT_ALM	0x02		/* AlamrKEY検出 */
#define		CPW_HOLDX_INT_CKL	0x03		/* 時計タイマー500ms間隔 */

#endif
