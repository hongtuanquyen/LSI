/***************************************************************
   クラリオン殿向け    
   プロジェクト名   ：'12年モデル開発
   ファイル名      ：lcd_mode.h
   モジュール名   ：LCD(表示)
   機   能         ：表示モジュールﾓｰﾄﾞ分岐定義ﾍｯﾀﾞﾌｧｲﾙ
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/08/01      0.1      ha      新規作成
****************************************************************/
#ifndef      _LCD_MODE_H_
#define      _LCD_MODE_H_

#undef   EXT
#ifdef   _LCD_MODE_C_
#define   EXT   
#else
#define   EXT   extern
#endif
/* 各表示ﾓｰﾄﾞ表示位置定数定義 */
/* 送信ﾚｺｰﾄﾞ */
/* 新規--------start-------- */
#define      RECORD1         0
#define      RECORD2         8
#define      RECORD3         16
#define      SENDBYTE      8
/* 新規--------end--------- */

/* VER表示 */
#define      VER                     6
/* SELﾓｰﾄﾞ */
#define      SELVOL                  2
#define      SELBAS                  2
#define      SELTRE                  2
#define      SELBAL                  2
#define      SUBVOL                  5
/* 時計ﾓｰﾄﾞ */
#define      CLKMIN                  5
#define      CLKSEC                  7
/* ﾀﾞｲﾚｸﾄﾓｰﾄﾞ */
#define      DIRECT                  5
/* TUNERﾓｰﾄﾞ */
#define      FRQ                     0
#define      FMFRQ                  0
#define      AMFRQ                  0
#define      PRENO                  4


EXT BYTE lcd_seg_buf[5];         /* 文字ﾊﾞｯﾌｧ    Character buffer */

/*-----------------------------------------------
      関数ﾌﾟﾛﾄﾀｲﾌﾟ
-----------------------------------------------*/
EXT void Lcd_mode(void);
EXT void Lcd_test_mode(void);
EXT void Lcd_clock_mode(void);
EXT void Lcd_area_disp(void);


#endif
