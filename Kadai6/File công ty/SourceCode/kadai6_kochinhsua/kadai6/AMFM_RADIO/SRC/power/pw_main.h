/******************************************************************************
	クラリオン（株）殿向け		2007 C3X03 ｼﾘｰｽﾞ[仮]

		プロジェクト名	: '07年フリップダウン　MP3モデル開発
		ファイル名		: pw_main.h
		機能			: ①電源制御ﾓｼﾞｭｰﾙ･ﾍｯﾀﾞｰ･ﾌｧｲﾙ
------------------------------------------------------------------------------
修正履歴
【番号】	日付		担当者	修正内容
【NKC-0000】'05/12/09	仲地	新規作成
******************************************************************************/
#ifndef	_PW_MAIN_H_
#define	_PW_MAIN_H_

#undef	EXT
#ifdef	_PW_MAIN_C_
#define	EXT
#else
#define	EXT	extern
#endif

#include	"../power/pw_def.h"

/*----------------------------------------------------
   関数ﾌﾟﾛﾄｺｰﾙ
----------------------------------------------------*/
EXT	void	Pw_main(void);
EXT	void	Pw_initial(void);

/*----------------------------------------------------
   ﾌﾗｸﾞ 定義
----------------------------------------------------*/
EXT	TCOM_FLG	power_flag;				/*Power*/
#define		fpw_itemchg_enb		(power_flag.bit.bit6)
#define		fpw_wakeup			(power_flag.bit.bit5)
#define		fpw_mute			(power_flag.bit.bit4)
#define		fpw_power_key		(power_flag.bit.bit3)
#define		fpw_ill_det			(power_flag.bit.bit2)
#define		fpw_alm_key_bak		(power_flag.bit.bit1)
#define		fpw_alm_key			(power_flag.bit.bit0)

EXT	TCOM_FLG	power_flag1;
#define		fpw_almdet_active	(power_flag1.bit.bit7)	/* AlarmKEY ONチャタリング中 */
#define		fpw_accdet_active	(power_flag1.bit.bit6)	/* AccONチャタリング中 */
#define		fpw_accbak			(power_flag1.bit.bit5)
#define		fpw_acc				(power_flag1.bit.bit4)
#define		fpw_acc_nor			(power_flag1.bit.bit0)

EXT	TCOM_FLG	pw_illdet_cnt;
#define		fpw_illdet_now		(pw_illdet_cnt.bit.bit0)

/*----------------------------------------------------
   高速アクセス変数定義
----------------------------------------------------*/
EXT	BYTE	pw_curmode;		/* ｶﾚﾝﾄ電源ﾓｰﾄﾞ */
EXT	BYTE	pw_tarmode;		/* 目標電源ﾓｰﾄﾞ */
EXT	BYTE	pw_item;		/* 実行ITEM */

EXT	BYTE	pw_power_seq;
EXT	WORD	pw_seq_timer;		/*	100msﾍﾞｰｽ･ﾀｲﾏｰ	*/

#if 0 /* 使用しないため、削除 */
EXT	BYTE	pw_accwait_timer;	/* 初期化時の安定待ち時間 */
#define		CPW_ACCWATI_TIM		8		/* 500ms/62.5ms */

EXT	BYTE	pw_chatwait_tim;				/* MainCont待ちMAXﾀｲﾏ[30sec] */
#define		CPW_MAINCNT_TIM		90000/500		/* [sun100823] 90secに変更 */
#endif

EXT BYTE pw_AccOn_4mscnt;		/*  */
#define		CPW_ACCON_4MSCNT	16		/* 500ms/62.5ms */

/* ACC Port ﾁｬﾀﾘﾝｸﾞ ｶｳﾝﾀ */
EXT BYTE	pw_accon_chat_cnt;
EXT BYTE	pw_accoff_chat_cnt;

/* AlarmKey Port ﾁｬﾀﾘﾝｸﾞ ｶｳﾝﾀ */
EXT BYTE	pw_almkey_cnt;


#if _POWER_DIAG_
/* DIAG機能 */
EXT	WORD	pw_diag_cnt;
#define		CPW_DIAG_CNT_ON		1000
#define		CPW_DIAG_MD_1MS		0
#define		CPW_DIAG_MD_MAIN	1
#endif	/* _POWER_DIAG_ */

#endif
