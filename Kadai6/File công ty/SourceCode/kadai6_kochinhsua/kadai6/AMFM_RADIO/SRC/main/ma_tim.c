/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: ma_tim.c
		機能			: ﾒｲﾝﾓｼﾞｭｰﾙﾍﾞｰｽﾀｲﾏｶｳﾝﾄ処理
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]	新規作成
******************************************************************************/
#define		_MA_TIM_C_

#include	"../model.h"
#include	"../comm/common.h"
#include	"../disp/lcd_ext.h"
#include	"../power/pw_func_ext.h"
#include	"../key/key_func_ext.h"
#include	"../audio/aud_ext.h"
#include	"../io/IO_inc.h"

#include	"../tuner/tu_ext.h"
#if 	_MODEL_EEPROM_
#include	"../eeprom/eeprom_ext.h"
#endif
#include	"ma_sys.h"
#include	"ma_ext.h"
#include	"ma_tim.h"

/******************************************************************************
.. 		関数名  ：Ma_tim_main
		引  数	：なし
		戻り値	：無し
		機  能	：Systemﾀｲﾏｶｳﾝﾄ処理、ﾀｲﾏﾒｯｾｰｼﾞ発行処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128] 新規作成
[sun070228] 10msﾀｲﾏ追加
******************************************************************************/
void	Ma_tim_main(void)
{
	if (fma_tim_10ms == ON)
	{
		fma_tim_10ms = OFF;
		Ma_set_Msg(CMA_TIMER,CMSG_10MS_TIMER);		/* 10msTimerﾒｯｾｰｼﾞ登録 */
	}
	if ( fma_tim_25ms == ON )						/* 25ms経過 */
	{
		fma_tim_25ms = OFF;							/* 25ms経過ﾌﾗｸﾞｸﾘｱ */

		Ma_set_Msg(CMA_TIMER,CMSG_25MS_TIMER);		/* 25msTimerﾒｯｾｰｼﾞ登録 */
		
		if ( (--ma_tim_50ms_cnt) == TIMER_OUT )
		{
			ma_tim_50ms_cnt = CTIM_50MS_CNT;
			Ma_set_Msg(CMA_TIMER,CMSG_50MS_TIMER);	/* 50msTimerﾒｯｾｰｼﾞ発行 */
		}
		if ( (--ma_tim_100ms_cnt) == TIMER_OUT )
		{
			ma_tim_100ms_cnt = CTIM_100MS_CNT;
			Ma_set_Msg(CMA_TIMER,CMSG_100MS_TIMER);	/* 100msTimerﾒｯｾｰｼﾞ発行 */
		}
	}
}

/******************************************************************************
.. 		関数名  ：Ma_tim_int_1ms
		引  数	：なし
		戻り値	：無し
		機  能	：1msﾀｲﾏ割込み処理
		コメント：2ms、4ms、5ms、10msのﾀｲﾏ割込み処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128] 新規作成
[sun090307]USBｺﾝﾊﾟｲﾙSW追加
******************************************************************************/
void	Ma_tim_int_1ms(void)
{
	/* 1ms割込み中さらに割り込んできた、何もせずに抜ける */
	if (fma_tim_in1ms == TRUE)
	{
		return;
	}

	fma_tim_in1ms = TRUE;						/* 1ms割込み処理中 */

	//1ms 割込み処理
	#if (_MODEL_RADIO_ == YES)
	Tu_1ms_timer();
	#endif
	
	Pw_1ms_timer();
	Key_1ms_timer();
	#if 	_MODEL_EEPROM_
	Eeprom_1ms_timer();
	#endif

	fma_tim_2ms = ~fma_tim_2ms;					/* 2msﾌﾗｸﾞ反転 */
	if (fma_tim_2ms == TRUE)
	{
		//2ms 割込み処理
		Pw_2ms_timer();
	}

	if (--ma_tim_5ms_cnt == TIMER_OUT)
	{
		ma_tim_5ms_cnt = CTIM_5MS_CNT;
		//5ms割込み処理
		Key_5ms_timer();
		Aud_event_5ms();
		Tu_5ms_timer();							/* Tuner 5msﾀｲﾏ処理 */
		Pw_5ms_timer();
	}
	if (--ma_tim_10ms_cnt == TIMER_OUT)
	{
		ma_tim_10ms_cnt = CTIM_10MS_CNT ;
		//10ms割込み処理
		Lcd_10ms_cnt();						/* LCD10msﾀｲﾏ処理 */
		Aud_event_10ms();
		Pw_10ms_timer();
		Ma_10ms_timer();
		
		fma_tim_10ms = ON;					/* [sun070228]10ms追加 */
	}
	if (--ma_tim_25ms_cnt == TIMER_OUT)
	{
		ma_tim_25ms_cnt = CTIM_25MS_CNT;
		//25ms割込み処理
		fma_tim_25ms = ON;
	}

	fma_tim_in1ms = FALSE;						/* 1ms割込み処理終了 */

}

#undef		_MA_TIM_C_
