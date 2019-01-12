/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_main.h
		機能			: ①Tuner Mainﾓｼﾞｭｰﾙヘッダﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]	新規作成
[sun060915] SD閾値ﾃｽﾄ版仕様正式変更対応
[zhang070313] clarion 対応修正
[matu120903] S-meter判定 Local値追加 
[yamamoto120905] ｵｾｱﾆｱ地区のAMﾊﾞﾝﾄﾞ周波数受信範囲上限修正
[matu120926] テストモード対応
[yamamoto130305] STチャタリング時間変更
******************************************************************************/
#ifndef	_TU_MAIN_H_
#define	_TU_MAIN_H_

#include	"../comm/common.h"
#include	"tu_def.h"

#undef	EXT
#ifdef	_TU_MAIN_C_
#define	EXT
#else
#define	EXT	extern
#endif

/*----------------------------------------------------
   関数ﾌﾟﾛﾄｺｰﾙ
----------------------------------------------------*/
EXT	void	Tu_main(void);
EXT	void	Tu_initial(void);
EXT	void	Tu_aftermuteset(void);			/* AfterMute ｾｯﾄ処理 */
/*----------------------------------------------------
   ﾌﾗｸﾞ 定義
   [zhang070308]CLARION仕様変更対応
----------------------------------------------------*/
EXT	TCOM_FLG	tu_flag;		/*Tuner*/
#define		ftu_mute		tu_flag.bit.bit7		/* Tuner Mute Request  1:mute on  0:mute off */
#define		ftu_write		tu_flag.bit.bit6		/* Tuner 書き込み要求 1: on 0: off */
#define		ftu_updw		tu_flag.bit.bit5		/* Up/DownFlag  1: Up 0：Down */
#define		ftu_stero		tu_flag.bit.bit4		/* ｽﾃﾚｵ確定Flag  1: Stero On  0: Stero Off */
#define		ftu_stero_b		tu_flag.bit.bit3		/* ｽﾃﾚｵﾁｬﾀﾘﾝｸﾞ用ﾊﾞｯｸFlag */
#define		ftu_lw			tu_flag.bit.bit2		/* LW Band Flag 1: LW  0: MW */
#define		ftu_isr			tu_flag.bit.bit1		/* ISR モードON：1/OFF:0	Tuner/HD RadioのISRを統一管理するため、
														ISRを独立モードとして追加、100809、岳			*/
#define		ftu_ifcnt		tu_flag.bit.bit0		/* Tuner IF ｶｳﾝﾄ中ﾌﾗｸﾞ 	1: ｶｳﾝﾄ中  0:ｶｳﾝﾄ終了 */

EXT	TCOM_FLG	tu_flag1;		/*Tuner*/
#define		ftu_stchk		tu_flag1.bit.bit7		/* Steroﾁｬﾀﾘﾝｸﾞﾁｪｯｸ10msﾌﾗｸﾞ  1: 10ms毎ﾁｬﾀﾘﾝｸﾞ  0: 10ms経過していない */
#define		ftu_seek		tu_flag1.bit.bit6		/* Tuner Seek On Flag 1: off 0: on */
#define		ftu_sttest		tu_flag1.bit.bit5		/* Tuner ST表示 Flag 1: ST表示中 0: 他表示中 */
#if	(_MODEL_EEPROM_ == 1)
#define		ftu_weepreq		tu_flag1.bit.bit4		/* Tuner Data Save Request	100811、岳	*/
#endif

/*----------------------------------------------------
   高速アクセス変数定義
----------------------------------------------------*/
EXT	BYTE	tu_m_mode;		/* Tuner Main Mode */
EXT	BYTE	tu_m_seq;		/* Tuner Main sequence */
EXT	BYTE	tu_i_seq;		/* Tuner Item sequence */
EXT	BYTE	tu_aftermute;	/* Tuner AfterMuteﾀｲﾏ [10ms Base] */
EXT	BYTE	tu_bandno;		/* band情報 */
EXT	BYTE	tu_pwrmode;		/* Tuner Powerﾓｰﾄﾞ */
#define		CTU_PWROFF		0
#define		CTU_FUNCON		1

EXT	TTU_CH	tu_ch;			/* カレント周波数エリア */
EXT	TTU_CH	tu_isrch;			/* ISR周波数エリア */
EXT	BYTE	tu_pno;			/* ｶﾚﾝﾄ周波数PresetNo */
EXT	BYTE	tu_wrtpno;	/* WRITEモード中周波数PresetNo */
EXT	BYTE	tu_bndmode;		/* Bandﾓｰﾄﾞ情報 */
EXT	TTU_INIT tu_coef;		/* Tuning係数用エリア */
EXT	BYTE	tu_stcnt;		/* ST信号ﾁｬﾀﾘﾝｸﾞｶｳﾝﾀ */
EXT	BYTE	tu_stdistim;	/* ST信号禁止ﾀｲﾏ */
#define		CTU_ST_ANTON		1500/50
#define		CTU_ST_PLLCHG		500/50
#define		CTU_STON_CNT	100		/* ST ONﾁｬﾀﾘﾝｸﾞｶｳﾝﾀ   100回*10ms */
#define		CTU_STOFF_CNT	50		/* ST OFFﾁｬﾀﾘﾝｸﾞｶｳﾝﾀ   50回*10ms */

EXT	BYTE	tu_1ms_timer;		/* 1msｶｳﾝﾄﾀｲﾏ */
EXT	BYTE	tu_5ms_timer;		/* 5msｶｳﾝﾄﾀｲﾏ */
EXT	BYTE	tu_10ms_timer;		/* 10msｶｳﾝﾄﾀｲﾏ */
EXT	BYTE	tu_100ms_timer;		/* 100msｶｳﾝﾄﾀｲﾏ */

EXT	BYTE	tu_test_dxlo;		/* DX/LO切替変数 [matu120926] */
EXT	BYTE	tu_qinfochk_num;	/* Q-info確認回数設定変数 [matu121105] */

/*----------------------------------------------------
   変数定義
----------------------------------------------------*/
EXT	TTU_BAND	tu_mem[CTU_BAND];	/* ﾊﾞﾝﾄﾞのﾗｽﾄ周波数/Preset周波数エリア */
EXT	BYTE		tu_as_cnt;			/* オートストア中に見つけた局数 */
EXT	BYTE		tu_updw_stus;		/* Tuner Up/Down状態 */

/* APM基本情報定義 */
#define		CTU_ASMAX		6			/* APM検出MAX局数 */


/* ｱｯﾌﾟﾀﾞｳﾝﾁｪｯｸ処理戻り値 */
#define		CTU_UPDW_OK		0
#define		CTU_UPDW_END	1
#define		CTU_UPDW_NOT	2

#define		CTU_PSTSCAN		0x05			/* PS時Tu_I_pstscan のmain seq番号*/


EXT	WORD		tu_dispfreq;		/* 表示周波数 */
EXT	LONG		tu_ifcnt;			/* IFｶｳﾝﾄ値 */
EXT	BYTE		tu_seekng_cnt;	/* SEEK状態NG回数 */

EXT	BYTE		tu_para[2][CTU_PARA_LEN];		/*	EEPROMから取得した初期データを保存する、100811、岳	*/
EXT	TTU_CHKCOEF	tu_chkcef[2];					/*	20dbのS-Meterレベルなど、局チェック用データ、100811、岳	*/
EXT	BYTE		tu_dx[2];						/*	SEEKのS-Meter閾値、100811、岳	*/
EXT	BYTE		tu_lo[2];						/*	SEEKのS-Meter閾値 LOCAl [matu120903] */
/*----------------------------------------------------
   ROMﾃｰﾌﾞﾙ定義
----------------------------------------------------*/
/*----------------------------------------------------
   BAND情報設定ﾃｰﾌﾞﾙ
[zhang070305]CLARION仕様変更対応
----------------------------------------------------*/
#ifdef _TU_MAIN_C_
const	BYTE	TU_BAND_INFO[CTU_DEST][CTU_BAND][2] =
{
/* 日本向け AMラジオ */
	{
		CTU_AM,	CTU_BAND0,				/* AM */
		0xFF,	0xFF,					/* なし */
		0xFF,	0xFF					/* なし */
	},
/* 日本向け AM/FMラジオ */
	{
		CTU_AM,	CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* 北米向け */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* 中南米向け */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* オセアニア向け */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* アジア向け */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* オセアニア向け */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	}
#if	(CTU_E_TEST == 0x01)
/* E向け */
	}
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2,				/* fm2 */
		CTU_FM, CTU_BAND3,				/* fm3 */
		CTU_AM, CTU_BAND0				/* AM */
	}
#endif
};
#else
extern	const	BYTE	TU_BAND_INFO[CTU_DEST][CTU_BAND][2];
#endif
/*----------------------------------------------------
   Preset初期設定ﾃｰﾌﾞﾙ
[zhang070301]CLARION仕様変更対応
----------------------------------------------------*/
#ifdef _TU_MAIN_C_
const WORD	 TU_PRESET_TBL[CTU_DEST][CTU_BAND][CTU_PRESET]= {
/* 日本向け AMラジオ */
/*AM*/
	{{0x0001, 0x0009, 0x0035, 0x0062, 0x0078, 0x007b},
/*  */
	 {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/*  */
	 {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},},
/* 日本向け AM/FMラジオ */
/*AM*/
	{{0x0001, 0x0009, 0x0035, 0x0062, 0x0078, 0x007b},
/*FM1*/
	 {0x0000, 0x0014, 0x0046, 0x0078, 0x008c, 0x0000},
/*FM2*/
	 {0x0000, 0x0014, 0x0046, 0x0078, 0x008c, 0x0000},},
/* 北米向け */
/*AM*/
	{{0x0000, 0x0007, 0x002f, 0x0057, 0x006c, 0x0076},
/*FM1*/
	 {0x0001, 0x000c, 0x0034, 0x005c, 0x0065, 0x0065},
/*FM2*/
	 {0x0001, 0x000c, 0x0034, 0x005c, 0x0065, 0x0065},},
/* 中南米向け */
/*AM*/
	{{0x0000, 0x000e, 0x005e, 0x00ae, 0x00d8, 0x00ec},
/*FM1*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},
/*FM2*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},},
/* オセアニア向け */
/*AM*/
	{{0x0001, 0x0009, 0x0035, 0x0062, 0x0078, 0x007b},
/*FM1*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},
/*FM2*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},},
/* アジア向け */
/*AM*/
	{{0x0000, 0x0008, 0x0034, 0x0061, 0x0077, 0x007a},
/*FM1*/
	 {0x0008, 0x0034, 0x00d4, 0x0174, 0x0198, 0x0198},
/*FM2*/
	 {0x0008, 0x0034, 0x00d4, 0x0174, 0x0198, 0x0198},},
/* 第三地域向け */
/*AM*/
	{{0x0009, 0x0051, 0x01DD, 0x0372, 0x0438, 0x0453},
/*FM1*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},
/*FM2*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},},

#if	(CTU_E_TEST == 0x01)
/*E Dest*/
/*FM1*/
	,{{0x00, 0x08, 0xd4, 0x14c,0x198, 0x19A},
/*FM2*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/*FM3*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/*AM*/
	{0x33, 0x5f, 0x8c, 0x03, 0x15, 0x2a},}
#endif
};
#else
extern	const WORD TU_PRESET_TBL[CTU_DEST][CTU_BAND][CTU_PRESET];
#endif

/*----------------------------------------------------
   Tunning係数定義ﾃｰﾌﾞﾙ
[sun060915] SD閾値ﾃｽﾄ版仕様正式変更対応
[zhang070301]CLARION仕様変更対応
	max_ch, min_ch, step,   min_n, 	if_min, 	if_max, 	if_wait, sd_dx, sd_loc, wait_edge, sd_wait, fref, min_freq, c_wait
----------------------------------------------------*/
#ifdef _TU_MAIN_C_
const TTU_DEST	 TU_COEF_TBL[CTU_DEST]= {
/* 日本向け AMラジオ */
	{0x00,	0x00,					/* FM/AM LW Bandなし */
/* FM_MW */
	{7600,	0x008c,	0x00,	100,	500/5,	50/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{522,	0x007b,	0x00,	9,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* 日本向け AM/FMラジオ */
	{0x00,	0x00,					/* FM/AM LW Bandなし */
/* FM_MW */
	{7600,	0x008c,	0x00,	100,	500/5,	50/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{522,	0x007b,	0x00,	9,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},
	
/* 北米向け */
	{0x00,	0x00,					/* FM/AM LW Bandなし */
/* FM_MW */
	{8770,	0x0065,	0x00,	200,	500/5,	50/5},                     
/* FM_LW */
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */
	{530,	0x0076,	0x00,	10,		500/5,	50/5},
/* AM_LW */
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* 中南米向け */
	{0x00,	0x0000,					/* FM/AM LW Bandなし */
/* FM_MW */
	{8750,	0x00cd,	0x00,	100,	500/5,	50/5},                     
/* FM_LW */
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */
	{530,	0x00ec,	0x00,	5,		500/5,	50/5},
/* AM_LW */
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* オセアニア向け */
	{0x00,	0x00,					/* FM/AM LW Bandなし */
/* FM_MW */
	{8750,	0x00cd,	0x00,	100,	500/5,	30/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{522,	0x0084,	0x00,	9,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* アジア向け */
	{0x00,	0x00,					/* FM/AM LW Bandなし */
/* FM_MW */
	{8750,	0x019a,	0x00,	50,		500/5,	30/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{531,	0x007a,	0x00,	9,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* 第三地域仕向け */
	{0x00,	0x00,					/* FM/AM LW Bandなし */
/* FM_MW */
	{8750,	0x00cd,	0x00,	100,	500/5,	30/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{522,	0x0453,	0x00,	1,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

#if	(CTU_E_TEST == 0x01)
/* E Dest */
	,{0x02,	0x00,					/* FM/AM LW Bandなし */
/* FM_MW */
	{8750,	0x19A,	0x00,	50,		500/5,	50/5},                     
/* FM_LW */                         
	{0x00,	0x00,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{531,	0xa3,	0x2b,	9,		500/5,	50/5},
/* AM_LW */                         
	{153,	0x2a,	0x00,	3,		500/5,	50/5}}
#endif
};
#else
extern const TTU_DEST	 TU_COEF_TBL[CTU_DEST];
#endif

#ifdef _TU_MAIN_C_
const WORD	 TU_ISR_TBL[3]= 
{
/* min_freq	min_ch	max_ch */
	1620,	0x007a,	0x007b,
};
#else
extern	const WORD TU_ISR_TBL[3];
#endif

/*	U向けFM	*/
#ifdef _TU_MAIN_C_
const	BYTE TU_PARA_FM_U_TBL[] = {
	0x20 , // TUNER0
	0x00 , // TUNER1
	0x40 , // TUNER2
	0x85 , // RADIO
	0x05 , // SOFTMUTE0
	0x44 , // SOFTMUTE1
	0x90 , // SOFTMUTE_FM
	0x03 , // HIGHFCUT0
	0x58 , // HIGHFCUT1
	0x00 , // HIGHFCUT2
	0x07 , // STEREO0
	0x50 , // STEREO1
	0x05 , // STEREO2
	0x16 , // CONTROL
	0x40 , // LEVEL_OFFSET
	0x04  // AM_LNA
};
#else
extern	const BYTE TU_PARA_FM_U_TBL[];
#endif

/*	J向けFM	*/
#ifdef _TU_MAIN_C_
const	BYTE TU_PARA_FM_J_TBL[] = {
	0x20 , // TUNER0
	0x00 , // TUNER1
	0x40 , // TUNER2
	0x81 , // RADIO
	0x05 , // SOFTMUTE0
	0x49 , // SOFTMUTE1
	0x90 , // SOFTMUTE_FM
	0x03 , // HIGHFCUT0
	0x50 , // HIGHFCUT1
	0x00 , // HIGHFCUT2
	0x07 , // STEREO0
	0x50 , // STEREO1
	0x05 , // STEREO2
	0x16 , // CONTROL
	0x40 , // LEVEL_OFFSET
	0x04   // AM_LNA
};
#else
extern	const BYTE TU_PARA_FM_J_TBL[];
#endif

/*	AMラジオ(国内)向けMW	*/
#ifdef _TU_MAIN_C_
const	BYTE TU_PARA_MW_AM_TBL[] = {
	0x00 , // TUNER0
	0x00 , // TUNER1
	0xC0 , // TUNER2
	0x81 , // RADIO
	0x06 , // SOFTMUTE0
	0x50 , // SOFTMUTE1
	0x0A , // SOFTMUTE_AM
	0x06 , // HIGHFCUT0
	0x48 , // HIGHFCUT1
	0x00 , // HIGHFCUT2
	0x00 , // STEREO0
	0x00 , // STEREO1
	0x00 , // STEREO2
	0x14 , // CONTROL
	0x40 , // LEVEL_OFFSET
	0x04   // AM_LNA
};
#else
extern	const BYTE TU_PARA_MW_AM_TBL[];
#endif

/*	AMFMラジオ向けMW	*/
#ifdef _TU_MAIN_C_
const	BYTE TU_PARA_MW_AMFM_TBL[] = {
	0x00 , // TUNER0
	0x00 , // TUNER1
	0xC0 , // TUNER2
	0xA5 , // RADIO
	0x06 , // SOFTMUTE0
	0x50 , // SOFTMUTE1
	0x0A , // SOFTMUTE_AM
	0x06 , // HIGHFCUT0
	0x48 , // HIGHFCUT1
	0x00 , // HIGHFCUT2
	0x00 , // STEREO0
	0x00 , // STEREO1
	0x00 , // STEREO2
	0x14 , // CONTROL
	0x40 , // LEVEL_OFFSET
	0x04   // AM_LNA
};
#else
extern	const BYTE TU_PARA_MW_AMFM_TBL[];
#endif


#ifdef _TU_MAIN_C_
const BYTE*	 TU_PARA_TBL[CTU_DEST][2]= 
{
/*JAPAN Dest AM*/
	TU_PARA_FM_J_TBL,TU_PARA_MW_AM_TBL
/*JAPAN Dest AM/FM*/
	,TU_PARA_FM_J_TBL,TU_PARA_MW_AMFM_TBL
/*U Dest 北米*/
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
/*B Dest 中南米*/
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
/* ｱｼﾞｱ向け オセアニア*/
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
/* ｱｼﾞｱ向け */
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
/* 第三地域向け */
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
#if	(CTU_E_TEST == 0x01)
/*Europe Dest*/
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
#endif
};
#else
extern	const BYTE* TU_PARA_TBL[CTU_DEST][2];
#endif

#ifdef _TU_MAIN_C_
const	TTU_CHKCOEF		TU_INI_CHKCOEF[2] = 
{
	{	0x67,	0xA6,	0xff,	0x18,	0x05,	0x03,	0x14,	0x0F,	0x0F,	0x05,	0x05	},
	{	0x92,	0xC9,	0x3C,	0x24,	0x05,	0x02,	0x1e,	0x0F,	0x0F,	0xFF,	0xFF	}
};	/* [matu120903] */

#define		TU_AM_IF_DETAIL_THIRD_DEST	0x00	/* 16OTP仕様変更 第三地域仕向け IFDetail値 */
#else
extern	const	TTU_CHKCOEF		TU_INI_CHKCOEF[2];
#endif

#endif
