/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_cont_tbl.c
		機能			: ①Tunerﾓｼﾞｭｰﾙｺﾝﾄﾛｰﾙﾃｰﾌﾞﾙﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051213]	新規作成
[sun061003]ｱﾗｰﾑｻｰﾁで局見付らないと、継続するように変更
******************************************************************************/
#ifndef		_TU_CONT_H_
#define		_TU_CONT_H_

#include	"tu_item.h"

#ifdef	_TU_ITEM_C_

const	TTU_CONTTBL  tu_m_pwron[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute */
	{	Tu_I_icreset,		2,			2,			},		/* 1:IC 初期化 */
	{	Tu_I_bndpara,		3,			3,			},		/* 2:BAND参数設定 */
	{	Tu_I_frqset,		4,			4,			},		/* 3:周波数設定 */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 4:AFTERMUTE設定 */
};

const	TTU_CONTTBL  tu_m_lastcal[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:Premute */
	{	Tu_I_frqset,		2,			2,			},		/* 1:周波数設定 */
	{	Tu_I_pstchk,		3,			3,			},		/* 2:Preset設定 */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 3:AFTERMUTE設定 */
};

const	TTU_CONTTBL  tu_m_bandchg[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute */
	{	Tu_I_bndpara,		2,			2,			},		/* 1:BAND参数設定 */
	{	Tu_I_frqset,		3,			3,			},		/* 2:周波数設定 */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 3:AFTERMUTE設定 */
};

const	TTU_CONTTBL  tu_m_man[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0: Premute */
	{	Tu_I_updw,			2,			2,			},		/* 1:周波数UP/DOWN1Step */
	{	Tu_I_frqset,		3,			3,			},		/* 2:周波数設定 */
	{	Tu_I_man,			4,			4,			},		/* 3:キーOff待ち処理 */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 4:AFTERMUTE設定 */
};
const	TTU_CONTTBL  tu_m_cman[] =
{
	{	Tu_I_updw,			1,			1,			},		/* 0:周波数UP/DOWN1Step */
	{	Tu_I_frqset,		2,			2,			},		/* 1:周波数設定 */
	{	Tu_I_cwait,			0,			0			}		/* 2:間隔タイマ待つ */
};
const	TTU_CONTTBL  tu_m_seek[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute  */
	{	Tu_I_seekst,		2,			2,			},		/* 1:seek start */
	{	Tu_I_updw,			3,			3,			},		/* 2:Stepｱｯﾌﾟﾀﾞｳﾝ */
	{	Tu_I_frqset,		4,			4,			},		/* 3:周波数設定 */
	{	Tu_I_qinfochk,		5,			2,			},		/* 4:SeeK停止判定 */
	{	Tu_I_seekend,		6,			6,			},		/* 5:Seek終了 */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 6:AFTERMUTE設定 */
};
const	TTU_CONTTBL  tu_m_pstrd[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:Premute */
	{	Tu_I_frqset,		2,			2,			},		/* 1:周波数設定 */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 2:AFTERMUTE設定 */
};

const	TTU_CONTTBL  tu_m_pstwt[] =
{
	{	Tu_I_pstwait,		1,			CTU_NGEND,	},		/* 0:プリセット書き込み待ち	 */
	{	Tu_I_pstwrite,		CTU_OKEND,	CTU_NGEND	}		/* 1:プリセット書き込み		 */
};

/* [sun061004]単体ﾃｽﾄ完[0] */
/* [sun061120]PreMute先にかけるように修正 */
const	TTU_CONTTBL  tu_m_as[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute */
	{	Tu_I_apmst,			2,			2,			},		/* 1:Apm start*/
	{	Tu_I_frqset,		3,			3,			},		/* 2:周波数設定*/
	{	Tu_I_qinfochk,		4,			5,			},		/* 3:SeeK停止判定 */
	{	Tu_I_apmset,		5,			5,			},		/* 4:有局を書き込ん */
	{	Tu_I_updw,			6,			6,			},		/* 5:Stepｱｯﾌﾟﾀﾞｳﾝ*/
	{	Tu_I_apmendchk,		7,			2,			},		/* 6:APM終了チェック*/
	{	Tu_I_apmend,		CTU_NEXT_M,	CTU_NEXT_M	}		/* 7:Apm end*/
};
/*[zhang070313] clarion 対応*/		/*	Preset Scan		*/
const		TTU_CONTTBL	tu_m_scan[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute  */
	{	Tu_I_seekst,		2,			2,			},		/* 1:seek start */
	{	Tu_I_updw,			3,			3,			},		/* 2:Stepｱｯﾌﾟﾀﾞｳﾝ */
	{	Tu_I_frqset,		4,			4,			},		/* 3:周波数設定 */
	{	Tu_I_qinfochk,		5,			2,			},		/* 4:SeeK停止判定 */
	{	Tu_I_scanwait,		0,			0			}		/* 5:受信後待ち処理 */
};

/* [zhang070312] ISR READ とISR WRITEを追加、好きなFreq(一つだけ)コール*/
const	TTU_CONTTBL  tu_m_isrrd[] =	
{
	{	Tu_I_premute,		1,			1,			},		/* 0: */
	{	Tu_I_isrread,		2,			2,			},		/* 1: */
	{	Tu_I_bndpara,		3,			3,			},		/* 2:BAND参数設定 */
	{	Tu_I_frqset,		4,			4,			},		/* 3:周波数設定 */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 4:AFTERMUTE設定 */
};

const	TTU_CONTTBL	* const TU_SeqTBL[] =
{
	tu_m_pwron,
	tu_m_lastcal,
	tu_m_bandchg,
	tu_m_man,
	tu_m_cman,
	tu_m_seek,
	tu_m_pstrd,
	tu_m_pstwt,
	tu_m_as,
	tu_m_scan,
	tu_m_isrrd,
};
#endif

#endif
