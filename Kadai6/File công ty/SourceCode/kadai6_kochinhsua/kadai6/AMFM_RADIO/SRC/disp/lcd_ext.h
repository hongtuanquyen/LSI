/***************************************************************
	クラリオン殿向け   
	プロジェクト名	：'12年モデル開発
	ファイル名		：lcd_ext.h
	モジュール名	：LCD(表示)
	機   能			：インタフェース定義ﾍｯﾀﾞﾌｧｲﾙ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
#ifndef		_LCD_EXT_H_
#define		_LCD_EXT_H_

#include	"../disp/lcd_main.h"

#undef	EXT
#ifdef	_LCD_EXT_C_
#define	EXT   
#else
#define	EXT	extern
#endif

/*-----------------外部参照用ここから--------------*/

/*----------------定数定義-------------------------*/
/* 表示遅延ﾀｲﾏ定義 */
/* 10msｶｳﾝﾄ */
#define		CLCD_EDITWAIT_50MS			5		/* 50ms */
#define		CLCD_EDITWAIT_100MS			10		/* 100ms */
#define		CLCD_EDITWAIT_120MS			12		/* 120ms */
#define		CLCD_EDITWAIT_150MS			15		/* 150ms */
#define		CLCD_EDITWAIT_200MS			20		/* 200ms */

/* 時計表示切替 */
#define		CLCD_CLK_CHG		2

/* 一時表示定義 */
#define		CLCD_1SHOT_1S		10			/* 1000/100 */
#define		CLCD_1SHOT_2S		21			/* 2000/100 */
#define		CLCD_1SHOT_3S		30			/* 3000/100 */
#define		CLCD_1SHOT_5S		50			/* 5000/100 */
#define		CLCD_1SHOT_10S		100			/* 10000/100 */
/*[070228he]--------------------start----------------------------*/
#define		CLCD_1SHOT_4S		40			/* 4000/100 */
#define		CLCD_1SHOT_7S		70			/* 7000/100 */
/*------------------------------end------------------------------*/

enum
{
	CLCD_OFF = 0,
	CLCD_VOL,
	CLCD_BAS,		/* 1: BASS (SEL) 	*/
	CLCD_TRE,		/* 2: TREBLE (SEL) 	*/
	CLCD_BAL,		/* 3: BALANCE(SEL)  */
	CLCD_FAD,		/* 4: FADER (SEL)	*/
	CLCD_1SHOT_MAX
};


/* ｽｸﾛｰﾙ要求 */
#define		CLCD_SCR_2SON		1		/* 2秒見せてからｽｸﾛｰﾙｽﾀｰﾄ */
#define		CLCD_SCR_ON			2		/* 一時見せずｽｸﾛｰﾙ */
#define		CLCD_SCR_2ON		3		/* 一時見せず、2回ｽｸﾛｰﾙ */
#define		CLCD_SCR_SRC		4		/* ｿｰｽ遷移ｽｸﾛｰﾙ */
#define		CLCD_SCR_DEMO		5		/* DEMOｽｸﾛｰﾙ */


/* ｲﾝｼﾞｹｰﾀ定義 */
/* TUNERモード */
#define		CLCD_TU_IND_FM				0x01
#define		CLCD_TU_IND_AM				0x02
#define		CLCD_TU_IND_ONE				0x04
#define		CLCD_TU_IND_TWO				0x08
#define		CLCD_TU_IND_ST				0x10
#define		CLCD_TU_IND_TI				0x20
#define		CLCD_TU_IND_S1				0x40

/* 時計モード */
#define		CLCD_CL_IND_AM				0x01
#define		CLCD_CL_IND_PM				0x02
#define		CLCD_CL_IND_AL				0x04

/* AUXモード */
#define		CLCD_AU_IND_AU				0x01
/*-----------------サブﾓｰﾄﾞ---------------------*/
/* TESTﾃﾞｰﾀ */
typedef struct
{
	BYTE mode;					/* 表示ﾓｰﾄﾞ 		*/
	BYTE value;					/* 値 				*/
}TDP_TES;
/* AUDIOﾃﾞｰﾀ */
 typedef struct
{
	BYTE mode;					/* SEL表示モード 	*/
	BYTE value;					/* value 			*/
}TDP_SEL;

enum
{
	CLCD_SEL_BASG,
	CLCD_SEL_BASF,
	CLCD_SEL_BASQ
};

enum
{
	CLCD_SEL_TREG,
	CLCD_SEL_TREF
};

enum
{
	CLCD_SEL_ZEHOFF,
	CLCD_SEL_ZEHB,
	CLCD_SEL_ZEHIM,
	CLCD_SEL_ZEHEX,
	CLCD_SEL_ZEHCS
};

/* ｼｽﾃﾑｺﾝﾄﾛｰﾙ */
typedef struct
{
	BYTE	mode;				/* SYSTEM表示モード */
	BYTE	value;				/* value 			*/
}TDP_SYM;
/* 時計ﾃﾞｰﾀ */
typedef struct
{
	BYTE hour;					/* 「時」情報  				*/
	BYTE min;					/* 「分」情報  				*/
	BYTE adjust;					/* 0:未調整 1：調整済み */
	BYTE mode;					
}TDP_CLK;

#define		CLCD_CLK_NOMAL		0			/* ノーマル */
#define		CLCD_CLK_HOUR		1			/* 時調整中 */
#define		CLCD_CLK_MIN		2			/* 分調整中 */
/* サブﾓｰﾄﾞの表示ﾓｰﾄﾞはサブﾓｰﾄﾞで定義します */

/*-------------------------ﾒｲﾝﾓｰﾄﾞ-------------------------*/
/* tunerﾃﾞｰﾀ情報 */
typedef struct
{
 	BYTE mode;		   			/* 表示ﾓｰﾄﾞ 							*/
	BYTE *add;		   			/* psﾃﾞｰﾀのﾎﾟｲﾝﾀ 						*/
	BYTE length;				/* PSﾃﾞｰﾀのﾊﾞｲﾄ数 						*/
	BYTE band;		  			/* AM/FM 								*/
	BYTE bandno;		   		/* BAND番号 							*/
 	BYTE base;					/* ベース表示 1→周波数 2→PS 			*/
 	BYTE frq[3];		   		/* 周波数 								*/
 	BYTE preno;		   			/* プリセットNO. 						*/
	BYTE pty;		   			/* PTY番号データ 0～34  HEX 			*/
	BYTE test;					/* RDSテスト判断 1→RDS中 2→通常中		*/

}TDP_TUNER;

#define		CLCD_TUBASE_FRQ			1
#define		CLCD_TUBASE_PS			2

typedef struct
{
	BYTE band;		  			/* AM/FM 								*/
 	BYTE frq[3];		   		/* 周波数 								*/
}TDP_ISR;
/* [100701xu]DIRモードを追加 */
typedef struct
{
	BYTE num[4];
	BYTE mode;		   			/* 表示ﾓｰﾄﾞ 							*/
}TDP_DIR;
/* [100701xu]TUNER TESTモードを追加 */
typedef struct
{
	BYTE band;
	BYTE mode;		   			/* 表示ﾓｰﾄﾞ 							*/
	BYTE value;		   			/* 表示DB数 							*/
}TDP_TUTEST;
enum
{
	CLCD_TUTEST_LITTLE,
	CLCD_TUTEST_LITING,
	CLCD_TUTEST_MIDLE,
	CLCD_TUTEST_MIDING,
	CLCD_TUTEST_CHECK,
	CLCD_TUTEST_CHKING,
};



/*--------------表示ﾓｰﾄﾞ定義--------------------------*/
/* Tuner */
#define		CLCD_DISPMODE		0		/* DISP MODE */
enum
{
	CLCD_TU_SCAN = 1,			/* 1: SCAN 				*/
	CLCD_TU_SCANWAIT,			/* 2: SCAN受信中		*/
	CLCD_TU_SEEK,				/* 3: SEEK中 			*/
	CLCD_TU_AS,					/* 4: AS中 			*/
	CLCD_TU_PSTWT,				/* 5: プリセットメモリ	*/
	CLCD_TU_MAX
};

/*----------------------外部参照用ここまで------------------*/




/*----------------------ﾓｼﾞｭｰﾙ内部定義ここから----------------*/
/* ﾀｲﾏ定数 */
#define		CLCD_BASE_5s					50			/* 5秒、5000/100 */
#define		CLCD_BASE_2s					21			/* [100904]2秒、2000/100->2.1秒 */
#define		CLCD_BASE_10s					100			/* 10秒、10000/100 */

#define		CLCD_SCRLL_2S					42			/* 2秒、2000/50 */	/*[101026xu]USB/MP3 FOL名前表示修正*/
#define		CLCD_SCRLL_700MS				14			/* 700ms, 700/50 */
#define		CLCD_SCRLL_200MS				4			/* 200ms, 200/50 */
#define		CLCD_SCRLL_300MS				6			/* 300ms, 300/50 */
#define		CLCD_SCRLL_350MS				7			/* 350ms, 350/50 */
#define		CLCD_SCRLL_500MS				10			/* 500ms, 500/50 */
#define		CLCD_SRC_SCRLL_50MS				2			/* 50ms, 50/25 */

#define		CLCD_BASEWAIT_TIM				20			/* 200ms,200/10 */

/* ベース表示ﾓｰﾄﾞ */
enum
{
	CLCD_BASE_TUNER = 1,
	CLCD_BASE_ISR,
	CLCD_BASE_AUX,
	CLCD_BASE_CLOCK,
	CLCD_BASE_MAX
};


#define		CLCD_1SHOT_OFF		0xFF			/* 一時表示強制ｷｬﾝｾﾙ */
#define		CLCD_SSP_CANCEL_TIM			60			/* 30秒、30000/500 */

/*-------------ﾀｲﾏ-----------------------*/
EXT BYTE lcd_tim_1shot;				/* 一時表示ﾀｲﾏ */
EXT BYTE lcd_tim_scr;				/* ｽｸﾛｰﾙﾀｲﾏ */
EXT BYTE lcd_tim_1shot_blk;			/* 一時消灯ﾀｲﾏ */
EXT BYTE lcd_tim_base;				/* ベース表示ﾀｲﾏ */
EXT BYTE lcd_base_wait_tim;			/* ﾍﾞｰｽ表示待ちﾀｲﾏ */


EXT BYTE lcd_ftitl_req;
/*-----------ﾓｰﾄﾞ-------------------*/
EXT BYTE lcd_1shot_mode;			/* 一時表示ﾓｰﾄﾞ */
EXT BYTE lcd_scr_cnt;				/* ｽｸﾛｰﾙｶｳﾝﾀ */
EXT BYTE lcd_1shot_blk;				/* 一時消灯ﾓｰﾄﾞ */
EXT BYTE lcd_base_mode;				/* ベースﾓｰﾄﾞ */ 
EXT BYTE lcd_test_buf[4];

/*----------------------------------------------------------------
		関数ﾌﾟﾛﾄﾀｲﾌﾟ
-----------------------------------------------------------------*/

EXT void Lcd_1shot_disp(BYTE, BYTE);		/* 一時表示要求 */
EXT void Lcd_1shot_cancel(BYTE);			/* 一時表示ｷｬﾝｾﾙ関数 */
EXT void Lcd_1shot_key_cancel(BYTE);		/* 一時表示、ｿｰｽ名表示、OPEN/CLOSE表示,SUBｷｬﾝｾﾙ(キー操作専用) */
EXT BOOL Lcd_dispkey_1shotcancel(void);
EXT void Lcd_disp_base(BYTE);
EXT void Lcd_disp_1shot_base(BYTE);			/* ﾍﾞｰｽ表示要求，各ﾓｼﾞｭｰﾙ内部状態が変わる時(キー操作以外) */
EXT void Lcd_base_scrll(BYTE);				/* mp3/ipodエラー状態解除、曲が変わった時、ﾍﾞｰｽ＆ｽｸﾛｰﾙ要求 */
EXT void Lcd_10ms_cnt(void);								/* 10ms割込み処理 */
EXT void Lcd_blktim_set(void);								/* 点滅ﾀｲﾏ再設定 */
EXT BOOL Lcd_1shot_check(void);
EXT BOOL Lcd_1shotsrc_cancel(void);
EXT void Lcd_base_disp_cancel(void);


EXT	void Lcd_send_req(void);

EXT void Lcd_accoff_send_req(void);
EXT BOOL Lcd_accoff_send_end(void);
EXT void Lcd_testmode_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length);




#endif
