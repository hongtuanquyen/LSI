/***************************************************************
	クラリオン殿向け       
	プロジェクト名	：'07年モデル開発
	ファイル名		：lcd_main.h
	モジュール名	：LCD(表示)
	機   能			：表示モジュールメイン定義ﾍｯﾀﾞﾌｧｲﾙ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2006/3/8		0.1		徐		新規作成
[070302he]	ｲﾝｼﾞｹｰﾀ点滅設定変更
			LEVEL設定削除
****************************************************************/
#ifndef		_LCD_MAIN_H_
#define		_LCD_MAIN_H_

#undef	EXT
#ifdef	_LCD_MAIN_C_
#define	EXT   
#else
#define	EXT	extern
#endif


/*-----------------ﾌﾗｸﾞ定義----------------------*/
EXT	TCOM_FLG	lcd_flag1;
#define		flcd_send			lcd_flag1.bit.bit0		/* 送信要求ﾌﾗｸﾞ 0=送信要求なし 1=送信要求 */
#define		flcd_blank			lcd_flag1.bit.bit1		/* ﾌﾞﾗﾝｸ送信要求 */

EXT	TCOM_FLG	lcd_flag2;

EXT	TCOM_FLG	lcd_flag3;
#define	flcd_blk_clr		lcd_flag3.bit.bit0		/* 点滅ｸﾘｱ要求 1= 要求あり,0=要求無し */

EXT	TCOM_FLG	lcd_flag4;
#define	flcd_test_mode		lcd_flag4.bit.bit0		/* テストモード用　1=テストモード　0=　テストモード以外 */

EXT	TCOM_FLG	lcd_flag5;
#define	flcd_accoff_send	lcd_flag5.bit.bit0		/* accoff中の更新要求 */

/*---------------定数定義-----------------------*/
#define		CLCD_TIMECNT_2S			40			/* 点滅ﾀｲﾏ2000/50 */
#define		CLCD_TIMECNT_1S			20			/* 点滅ﾀｲﾏ1000/50 */
#define		CLCD_TIMECNT_500MS		10			/* 点滅ﾀｲﾏ500/50 */

#define		CLCD_ACCON_EDIT_TIM		5



/* 文字点滅 */
enum
{
	CLCD_NONE = 0,
	CLCD_BLK_FREQ,
	CLCD_BLK_CLOCK,
	CLCD_BLK_ALARM,
	CLCD_BLK_PSNO,
	CLCD_BLK_MAX
};
/*---------------変数定義----------------------*/
#define		BLK_500MS				0
#define		BLK_1S					1
EXT BYTE lcd_tim_blk;				/* 点滅ﾀｲﾏ */
EXT BYTE lcd_tim_blkcolon;			/* 点滅ﾀｲﾏ */
EXT BYTE lcd_tim_pwr_wait;			/* PWR_CNT_ON 500ms待ちﾀｲﾏ */
EXT BYTE lcd_ind_blk;				/* ｲﾝｼﾞｹｰﾀ点滅ﾓｰﾄﾞ */
EXT BYTE lcd_blk_mode;				/* 点滅ﾓｰﾄﾞ */
EXT BYTE lcd_blk_type;				/* 点滅ﾀｲﾏ種類 */
EXT BYTE lcd_edit_tim;				/* 編集ﾀｲﾏ */
EXT BYTE lcd_send_tim;				/* 送信ﾀｲﾏ */

/*-----------------------------------------------
		関数ﾌﾟﾛﾄﾀｲﾌﾟ
-----------------------------------------------*/
EXT void Lcd_main(void);
EXT void Lcd_initial(void);
EXT void Lcd_ram_initial(void);
EXT void Lcd_driver_send( void );
EXT void Lcd_indictrol( void );
EXT void Lcd_blink(void);
EXT void Lcd_blink_set(BYTE, BYTE);
EXT void Lcd_25ms_cnt(void);
EXT void Lcd_50ms_cnt(void);
EXT void Lcd_100ms_cnt(void);
EXT void lcd_moji_romcpy(BYTE *savebuf, BYTE *addfrom);
EXT void Lcd_hextoascii( BYTE, BYTE * );
EXT void Lcd_nodisp( BYTE *, BYTE);


#endif
