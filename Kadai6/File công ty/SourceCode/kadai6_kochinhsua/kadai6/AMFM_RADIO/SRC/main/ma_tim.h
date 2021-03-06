/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	: 
		ファイル名		: ma_tim.h
		機能			: ﾒｲﾝﾓｼﾞｭｰﾙﾍﾞｰｽﾀｲﾏｶｳﾝﾄ処理ﾍｯﾀﾞﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128]	新規作成
******************************************************************************/
#ifndef		_MA_TIM_H_
#define		_MA_TIM_H_

#undef	EXT
#ifdef	_MA_TIM_C_
#define	EXT	    
#else
#define	EXT		extern
#endif

/*----------------------------------------------------
   関数プロトタイプ
----------------------------------------------------*/
EXT	void	Ma_tim_int_1ms(void);
EXT	void	Ma_tim_main(void);

/*----------------------------------------------------
   ﾌﾗｸﾞ定義
----------------------------------------------------*/
EXT	TCOM_FLG	ma_tim_flag;
#define	fma_tim_25ms	(ma_tim_flag.bit.bit7)		/* 25msﾀｲﾏﾌﾗｸﾞ 1:25ms経過【ｾｯﾄ:1ms割込み、ｸﾘｱﾒｲﾝ:処理】 */
#define	fma_tim_2ms		(ma_tim_flag.bit.bit6)		/* 2msﾀｲﾏﾌﾗｸﾞ  1:2ms経過 */
#define	fma_tim_in1ms	(ma_tim_flag.bit.bit5)		/* 1ms割込み処理中ﾌﾗｸﾞ 1: 1ms割込み処理中 */
#define	fma_tim_10ms	(ma_tim_flag.bit.bit4)		/* 10msﾀｲﾏﾌﾗｸﾞ 1:10ms経過【ｾｯﾄ：1ms割込み、ｸﾘｱ：ﾒｲﾝ処理】 */
#define	fma_1ms_enb		(ma_tim_flag.bit.bit3)		/* 1ms禁止許可中ﾌﾗｸﾞ  1: 1ms許可  0：1ms禁止 */
/*----------------------------------------------------
   高速ｱｸｾｽ変数定義
----------------------------------------------------*/
EXT		BYTE	ma_tim_4ms_cnt;					/* 4msﾀｲﾏｶｳﾝﾀ【割込み】 */
EXT		BYTE	ma_tim_5ms_cnt;					/* 5msﾀｲﾏｶｳﾝﾀ【割込み】 */
EXT	 	BYTE	ma_tim_10ms_cnt;				/* 10msﾀｲﾏｶｳﾝﾀ【割込み】 */
EXT	 	BYTE	ma_tim_25ms_cnt;				/* 25msﾀｲﾏｶｳﾝﾀ【割込み】 */
EXT	 	BYTE	ma_tim_50ms_cnt;				/* 50msﾀｲﾏｶｳﾝﾀ【ﾒｲﾝ】*/
EXT	 	BYTE	ma_tim_100ms_cnt;				/* 100msﾀｲﾏｶｳﾝﾀ【ﾒｲﾝ】 */
EXT		BYTE	ma_tim_500ms_cnt;				/* 500msﾀｲﾏｶｳﾝﾀ【ﾒｲﾝ】 */
EXT		BYTE	ma_tim_1s_cnt;					/* 1sﾀｲﾏｶｳﾝﾀ【ﾒｲﾝ】 */
#define		CTIM_4MS_CNT	4
#define		CTIM_5MS_CNT	5
#define		CTIM_10MS_CNT	10
#define		CTIM_25MS_CNT	25
#define		CTIM_50MS_CNT	2
#define		CTIM_100MS_CNT	4
#define		CTIM_500MS_CNT	20
#define		CTIM_1S_CNT		40


#endif
