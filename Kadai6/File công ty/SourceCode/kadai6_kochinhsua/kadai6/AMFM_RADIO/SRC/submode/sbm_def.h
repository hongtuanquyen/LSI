/*************************************************************************
	クラリオン殿向け 
	プロジェクト名	：2012年AMFM_RADIOﾓﾃﾞﾙ
	ファイル名		：sbm_def.h
	モジュール名	：SUBモード
	機   能			：SUBﾓｰﾄﾞ定義ファイル
--------------------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
--------------------------------------------------------------------------
			2007/3/17		0.1		陳		新規
*************************************************************************/
#ifndef		_SBM_DEF_H_
#define		_SBM_DEF_H_

#include	"../model.h"

#undef	EXT
#ifdef	_SBM_EXT_C_
#define	EXT   
#else
#define	EXT	extern
#endif


/*---------------ﾌﾗｸﾞ定義-----------------------*/
EXT	TCOM_FLG 	sbm_flag;
#define		fsbm_vol_mode		sbm_flag.bit.bit7		/* volモードﾌﾗｸﾞ */
#define		fsbm_ver_disp		sbm_flag.bit.bit6		/* バージョン表示モードﾌﾗｸﾞ */
#define		fsbm_alm_off_req	sbm_flag.bit.bit5		/* almモードAM/PMﾌﾗｸﾞ */
#define		fsbm_alm_onoff		sbm_flag.bit.bit4		/* almモードON/OFFﾌﾗｸﾞ */
//#define		fsbm_alm_setted		sbm_flag.bit.bit3		/* alm起動中かﾌﾗｸﾞ */
#define		fsbm_alm_beepon		sbm_flag.bit.bit2		/* alm起動中かﾌﾗｸﾞ */
#define		fsbm_beep_en		sbm_flag.bit.bit1		/* alm起動許可ﾌﾗｸﾞ */
	
	
	
	
	
	
	
enum
{
	CSBM_OFF,		/* SUBモードではない	*/
	CSBM_BEP,		/* ALM BEEP中 */
	CSBM_VOL,		/* VOL調整モード調整中 		*/
	CSBM_VER,		/* VOL調整モード調整中 		*/
	CSBM_SEF,		/* AUDIO調整モード調整中 	*/
	CSBM_CLK,		/* CLOCK調整モード調整中 		*/
	CSBM_ALM,		/* ALARM調整モード調整中 		*/
	CSBM_TES		/* TESEモード調整中 	*/
};

/*--------------SUBﾓｰﾄﾞ有効bit定義----------------*/
#define		CSBM_BIT_PWR		0x20
#define		CSBM_BIT_VOL		0x10
#define		CSBM_BIT_BEP		0x08
#define		CSBM_BIT_ALM		0x04
#define		CSBM_BIT_CLK		0x02
#define		CSBM_BIT_AUD		0x01



/*---------------------------VOLUME調整ﾓｰﾄﾞ----------------------*/
#define		CSBM_VOL_7S			70			/* 100msｶｳﾝﾄ */
EXT	BYTE	sbm_vol_tim;


/*---------------隠しキー（SECRET KEY）ﾓｰﾄﾞ----------------------*/
enum{
	CSBM_SCT_OFF,
	CSBM_SCT_VER,
	CSBM_SCT_BEEP,
	CSBM_SCT_LOUDOFF
};

EXT BYTE sbm_sct_mode;						/* TESモード */



/*---------------------------AUDIO調整ﾓｰﾄﾞ----------------------*/
#undef	EXT
#ifdef	_SEF_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif


#define		CSBM_AUD_7S			70			/* 100msｶｳﾝﾄ */


enum		/* 調整項目 */
{
	CSBM_AUD_OFF = 0,
	CSBM_AUD_BAS,
	CSBM_AUD_TRE,
	CSBM_AUD_BAL
};

EXT BYTE sbm_aud_mode;				/* AUDIO調整ﾓｰﾄﾞ */
EXT BYTE sbm_aud_tim;				/* AUDIO調整ﾓｰﾄﾞ保持ﾀｲﾏ */



/*---------------------------CLOCK調整ﾓｰﾄﾞ----------------------*/

#undef	EXT
#ifdef	_CLOCK_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif


#define		CSBM_CLK_15S			150			/* 100msｶｳﾝﾄ */

typedef	struct{
	BYTE	min;
	BYTE	hour;
}TSBM_CLK;

EXT	BYTE	sbm_clk_tim;					/* 時計調整モード保持ﾀｲﾏ */
EXT	BYTE	sbm_clk_mode;					/* 時計ﾓｰﾄﾞ */
#define		CSBM_CLK_NORMAL		0			/* 時計調整済み */
#define		CSBM_CLK_ADJUST		1			/* 時計時分の調整中 */

#define		CSBM_CLK_AM			0			/* 時計の午前中 */
#define		CSBM_CLK_PM			1			/* 時計の午後 */

EXT		TSBM_CLK	sbm_clk_adj;			/* 時計調整変数ｴﾘｱ */
EXT		TSBM_CLK	sbm_clk_bak;			/* 時計調整変数ｴﾘｱ */
#define		CSBM_CLK_1MIN	60
#define		CSBM_CLK_1H		60
#define		CSBM_CLK_1DAY	24

EXT	WORD	sbm_beepon_tim;					/* 時計調整モード保持ﾀｲﾏ */
#define		CSBM_BEEPON_180S	180000/100	/* 180S */


/*---------------------------ALARM調整ﾓｰﾄﾞ----------------------*/
#undef	EXT
#ifdef	_ALM_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif

#define		CSBM_ALM_15S			150			/* 100msｶｳﾝﾄ */

typedef	struct{
	BYTE	min;
	BYTE	hour;
}TSBM_ALM;

EXT	BYTE	sbm_alm_tim;					/* ALARM調整モード保持ﾀｲﾏ */
EXT	BYTE	sbm_alm_mode;					/* ALARM調整ﾓｰﾄﾞ */
#define		CSBM_ALM_NORMAL		0			/* ALARM調整済み */
#define		CSBM_ALM_ADJUST		1			/* ALARM時分の調整中 */
EXT		TSBM_ALM	sbm_alm_adj;			/* ALARM調整変数ｴﾘｱ */
EXT		TSBM_ALM	sbm_alm_bak;			/* ALARM調整変数ｴﾘｱ */




#endif

