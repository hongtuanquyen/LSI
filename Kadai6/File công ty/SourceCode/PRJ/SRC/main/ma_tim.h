/******************************************************************************
..     ƒNƒ‰ƒŠƒIƒ““aŒü‚¯                    2007ÓÃŞÙ

		ƒvƒƒWƒFƒNƒg–¼	: 
		ƒtƒ@ƒCƒ‹–¼		: ma_tim.h
		‹@”\			: Ò²İÓ¼Ş­°ÙÍŞ°½À²Ï¶³İÄˆ—Í¯ÀŞÌ§²Ù
------------------------------------------------------------------------------
		C³—š—ğ	y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051128]	V‹Kì¬
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
   ŠÖ”ƒvƒƒgƒ^ƒCƒv
----------------------------------------------------*/
EXT	void	Ma_tim_int_1ms(void);
EXT	void	Ma_tim_main(void);

/*----------------------------------------------------
   Ì×¸Ş’è‹`
----------------------------------------------------*/
EXT	TCOM_FLG	ma_tim_flag;
#define	fma_tim_25ms	(ma_tim_flag.bit.bit7)		/* 25msÀ²ÏÌ×¸Ş 1:25msŒo‰ßy¾¯Ä:1msŠ„‚İA¸Ø±Ò²İ:ˆ—z */
#define	fma_tim_2ms		(ma_tim_flag.bit.bit6)		/* 2msÀ²ÏÌ×¸Ş  1:2msŒo‰ß */
#define	fma_tim_in1ms	(ma_tim_flag.bit.bit5)		/* 1msŠ„‚İˆ—’†Ì×¸Ş 1: 1msŠ„‚İˆ—’† */
#define	fma_tim_10ms	(ma_tim_flag.bit.bit4)		/* 10msÀ²ÏÌ×¸Ş 1:10msŒo‰ßy¾¯ÄF1msŠ„‚İA¸Ø±FÒ²İˆ—z */
#define	fma_1ms_enb		(ma_tim_flag.bit.bit3)		/* 1ms‹Ö~‹–‰Â’†Ì×¸Ş  1: 1ms‹–‰Â  0F1ms‹Ö~ */
/*----------------------------------------------------
   ‚‘¬±¸¾½•Ï”’è‹`
----------------------------------------------------*/
EXT		BYTE	ma_tim_4ms_cnt;					/* 4msÀ²Ï¶³İÀyŠ„‚İz */
EXT		BYTE	ma_tim_5ms_cnt;					/* 5msÀ²Ï¶³İÀyŠ„‚İz */
EXT	 	BYTE	ma_tim_10ms_cnt;				/* 10msÀ²Ï¶³İÀyŠ„‚İz */
EXT	 	BYTE	ma_tim_25ms_cnt;				/* 25msÀ²Ï¶³İÀyŠ„‚İz */
EXT	 	BYTE	ma_tim_50ms_cnt;				/* 50msÀ²Ï¶³İÀyÒ²İz*/
EXT	 	BYTE	ma_tim_100ms_cnt;				/* 100msÀ²Ï¶³İÀyÒ²İz */
EXT		BYTE	ma_tim_500ms_cnt;				/* 500msÀ²Ï¶³İÀyÒ²İz */
EXT		BYTE	ma_tim_1s_cnt;					/* 1sÀ²Ï¶³İÀyÒ²İz */
#define		CTIM_4MS_CNT	4
#define		CTIM_5MS_CNT	5
#define		CTIM_10MS_CNT	10
#define		CTIM_25MS_CNT	25
#define		CTIM_50MS_CNT	2
#define		CTIM_100MS_CNT	4
#define		CTIM_500MS_CNT	20
#define		CTIM_1S_CNT		40


#endif
