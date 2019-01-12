/***************************************************************
	クラリオン殿向け       
	プロジェクト名	：'12年モデル開発
	ファイル名		：lcd_sys.h
	モジュール名	：LCD(表示)
	機   能			：定義ﾍｯﾀﾞﾌｧｲﾙ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
#ifndef		_LCD_SYS_H_
#define		_LCD_SYS_H_

#undef	EXT
#ifdef	_LCD_SYS_C_
#define	EXT   
#else
#define	EXT	extern
#endif


/*---------------ｿｰｽ切替----------------------*/
enum
{
	CLCD_SRC_TUNER = 1,
	CLCD_SRC_ISR,
	CLCD_SRC_AUX,
	CLCD_SRC_MAX
};



/*-------------デモ一時表示-------------------*/
#define		CLCD_OPEN				1
#define		CLCD_CLOSE				2

#define		CLCD_SRC_TIM			20
#define		CLCD_SRCTA_TIM			70			/* 7000/100 */

EXT BYTE lcd_src_mode;					/* ｿｰｽﾁｪﾝｼﾞ一時表示ﾓｰﾄﾞ */
EXT BYTE lcd_src_tim;					/* ｿｰｽﾁｪﾝｼﾞ一時表示ﾀｲﾏ */
EXT BYTE lcd_src_rdsint;

EXT	void Lcd_sys_message( BYTE);
EXT void Lcd_1shot_src(void);

#endif
