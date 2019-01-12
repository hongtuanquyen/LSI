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
[matu120926] テストモード対応
[yamamoto130305] STチャタリング時間変更
******************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_
#define		_TU_SYS_C_

#include	"../comm/common.h"
#include	"../main/ma_ext.h"

#include	"tu_sys.h"
#include	"tu_main.h"
#include	"tu_event.h"
#include	"tu_ext.h"
#include	"tu_key.h"
#include	"tu_io.h"

static	void	Tu_mode_check(BYTE mode);
static	void	Tu_sys_10ms(void);
static	void	Tu_sys_50ms(void);
static	void	Tu_sys_100ms(void);

/******************************************************************************
.. 		関数名  ：Tu_sys_message
		引  数	：Systemﾒｯｾｰｼﾞ
		戻り値	：無し
		機  能	：TunerSystemﾒｯｾｰｼﾞ受付処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125] 新規作成
[matu120926] DX/LO要求クリア処理追加
******************************************************************************/
void	Tu_sys_message(BYTE	msg)
{
	switch(msg)
	{

		case	CMSG_MAIN:
			Tu_main();
			break;
		case	CMSG_10MS_TIMER:
			Tu_sys_10ms();
			break;
		case	CMSG_50MS_TIMER:
			Tu_sys_50ms();
			break;
		case	CMSG_100MS_TIMER:
			Tu_sys_100ms();
			break;
		case	CMSG_PWRON:
			Tu_mode_check(CMSG_PWRON);		/* Tunerﾓｼﾞｭｰﾙ Function On/Off切替処理 */
			break;
		case	CMSG_MODECHG:
			Tu_mode_check(CMSG_MODECHG);	/* Tunerﾓｼﾞｭｰﾙ Function On/Off切替処理 */
			break;
		case	CMSG_PWROFF:
		case	CMSG_MAINCNT_OFFREQ:
			Tu_mode_check(CMSG_PWROFF);		/* Tunerﾓｼﾞｭｰﾙ Function On/Off切替処理 */
			break;
		case	CMSG_INITIAL:
			Tu_initial();
			break;
		case	CMSG_MAINCNT_ON:
			tu_test_dxlo = CTU_TEST_NO;		/* DX/LO要求クリア */
			break;
		default:
			break;
	}
}
/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_mode_check
		引  数	：無し
		戻り値	：無し
		機  能	：TunerﾓｼﾞｭｰﾙOn/Off切替処理
-----------------------------------------------------------------------------
[sun060517] ﾐｽ1 FUNC OFF行う条件判断ﾐｽ
[zhang070228]MIO_CHG_pST_DATAを削除
-----------------------------------------------------------------------------*/
static	void	Tu_mode_check(BYTE mode)
{
	if (mode == CMSG_PWROFF)
	{
		if (tu_pwrmode != CTU_PWROFF)
		{
			tu_pwrmode = CTU_PWROFF;
			Tu_e_func_off();
		}
	}
	else
	{
		if ((Ma_current_mode() == CMA_TUNER)
		    ||(Ma_current_mode() == CMA_ISR))
		{
			if (tu_pwrmode != CTU_FUNCON)
			{
				tu_pwrmode = CTU_FUNCON;
				Tu_e_func_on();
			}
			else
			{
				Tu_job_clear();
				if(Ma_current_mode() == CMA_ISR)				
				{
					tu_m_mode = CTU_M_ISRRD;					/* ISR READ */
				}
				else
				{
					Tu_lastcall();
					tu_m_mode = CTU_M_BANDCHG;					/*TUNER LAST READ */
				}
			}
		}
		else
		{
			if (tu_pwrmode != CTU_PWROFF)
			{
				tu_pwrmode = CTU_PWROFF;
				Tu_e_func_off();
			}
		}
	}
}
/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_tim_cntdw()
		引  数	：なし
		戻り値	：無し
		機  能	：ﾀｲﾏｶｳﾝﾄﾀﾞｳﾝ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060408]	新規作成
[sun060421] 単体完 [ﾐｽ1 括弧付け忘れ]
-----------------------------------------------------------------------------*/
void Tu_tim_cntdw(BYTE	*time)
{
	if ((*time) > 0)
	{
		(*time) --;
	}
}
/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_sys_10ms
		引  数	：無し
		戻り値	：無し
		機  能	：Tunerﾓｼﾞｭｰﾙ10msﾀｲﾏ周期処理
-----------------------------------------------------------------------------*/
static	void	Tu_sys_10ms(void)
{
	ftu_stchk = TRUE;
	if (tu_aftermute > TIMER_OUT)
	{
		tu_aftermute --;
	}

	if (tu_10ms_timer > 0)
	{
		tu_10ms_timer --;
	}
}

/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_sys_50ms
		引  数	：無し
		戻り値	：無し
		機  能	：Tunerﾓｼﾞｭｰﾙ50msﾀｲﾏ周期処理
-----------------------------------------------------------------------------*/
static	void	Tu_sys_50ms(void)
{
	/* ｽﾃﾚｵ検出禁止ﾀｲﾏ */
	Tu_tim_cntdw(&tu_stdistim);
}
/*------------------------------------------------------------------------------
.. 		関数名  ：Tu_sys_100ms
		引  数	：無し
		戻り値	：無し
		機  能	：Tunerﾓｼﾞｭｰﾙ100msﾀｲﾏ周期処理
-----------------------------------------------------------------------------*/
static	void	Tu_sys_100ms(void)
{
	Tu_tim_cntdw(&tu_100ms_timer);
}
#undef  _TU_SYS_C_

#endif
