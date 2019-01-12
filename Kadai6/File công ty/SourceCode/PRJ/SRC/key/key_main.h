/******************************************************************************
   クラリオン殿向け
   プロジェクト名   : 2012年AMFM_RADIOモデル
   ファイル名      : key_main.h
   機能         : キーモジュールメインヘッダファイル
------------------------------------------------------------------------------
日付      担当者      修正内容
2012/07/26   東海林      新規作成
******************************************************************************/
#ifndef   _KEY_MAIN_H_
#define   _KEY_MAIN_H_

 #undef   EXT
 #ifdef   _KEY_MAIN_C_
 #define   EXT
 #else
 #define   EXT   extern
 #endif

/*----------------------------------------------------
   関数プロトタイプ宣言
----------------------------------------------------*/
/*   keyモジュールメイン処理         */
EXT   void   Key_main(void);
/*   keyモジュールの変数初期化処理      */
EXT   void   Key_initial(void);

/*----------------------------------------------------
   フラグ定義
----------------------------------------------------*/
EXT   TCOM_FLG key_flag;   /*Key Module Flag Memory area */
#define      fkey_prio_key      (key_flag.bit.bit0)/* 優先キー動作中フラグ */
#define      fkey_alldis         (key_flag.bit.bit1)/* ALLキー禁止フラグ */
#define      fkey_nop         (key_flag.bit.bit2)/* 取込無効フラグ 無効 1:ON/有効 0:OFF      */
#define      fkey_alarm_key      (key_flag.bit.bit3)/* alarmKey押下中フラグ */

/*----------------------------------------------------
   高速アクセス変数定義
----------------------------------------------------*/
EXT   BYTE   key_long_timer;         /*   Long Key 確定用タイマカウンタ      */

/*----------------------------------------------------
   変数定義
----------------------------------------------------*/
EXT   BYTE   key_lcd_input;         /*   LCD Key Code Memory area      */
EXT   BYTE   key_lcd_driver_data[4];   /* LCD データ入力バッファ(32Bit)   */
EXT   BYTE   key_lcd_input_Timer;   /* LCD 取込インターバルタイマ base-time = 5ms   */
EXT   BYTE   key_in_disable_timer;   /* (Key disable)    */
EXT   BYTE   key_status;            /* Key状態 */

EXT   BYTE   key_before_key;         /* 隠しキー用Key状態 */
EXT   WORD   key_test_code;         /* Test用 */
EXT   BYTE   key_test_sta;         /* Test用 */
EXT   BYTE   key_t_data[4];      /* Test用   For Test*/
enum{
   CKEY_TSET_NOP,
   CKEY_TSET_INPUT,
   CKEY_TSET_REPLY
};


#define      CKEY_KEYNOP      0      /* Key状態変化無し */
#define      CKEY_KEYON      1      /* KEY押された */
#define      CKEY_KEYOFF      2      /* KEY離された */
#define      CKEY_KEYLONG   3      /* KEY長押し確定 */

EXT   BYTE   key_now_key;         /*   カレント処理中キーコード         */
EXT   BYTE   key_old_key;         /*   前回確定キーコード         */
EXT   BYTE   key_new_key;         /*   今回確定キーコード         */

EXT   void    (*Key_ProcFunc)(void);      /* Key動作関数アドレス   */
EXT   BYTE   key_modenb;
EXT   BYTE   key_comenb;
EXT   BYTE   key_mode_stat;
EXT   BYTE   key_com_stat;

EXT   const KEY_PROC_TBL *   ProceT_p;         /*   処理テーブルアドレスポインタ   Processing table address pointer */
EXT   const KEY_CHG_TBL *   KcodeT_p;         /*   コード・変換テーブルアドレスポインタ   Code ｷ conversion table address pointer*/


/* LCD Driver Key Data  ->  共通 Key Code 変換テーブル */

/*----------------------------------------------------
   構造体
----------------------------------------------------*/
#ifdef _KEY_MAIN_C_
#if 1      /* キー配置仕様変更 */
const   BYTE   tkey_code_tbl[]= {
   CKEY_ID_KEY_OFF,      /*   D01   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D02   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D03   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D04   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D05   （該当キー無し）      */
   CKEY_ID_LCD_VOLUP,      /*   D06   （VOLUP）            */
   CKEY_ID_LCD_AS,         /*   D07   （AS）               */
   CKEY_ID_KEY_OFF,      /*   D08   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D09   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D10   （該当キー無し）      */
   CKEY_ID_LCD_SRC,      /*   D11   （MODE）            */
   CKEY_ID_LCD_RIGHT,      /*   D12   （UP）               */
   CKEY_ID_KEY_OFF,      /*   D13   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D14   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D15   （該当キー無し）      */
   CKEY_ID_LCD_VOLDW,      /*   D16   （VOLDW）            */
   CKEY_ID_LCD_LEFT,      /*   D17   （DW）               */
   CKEY_ID_KEY_OFF,      /*   D18   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D19   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D20   （該当キー無し）      */
   CKEY_ID_LCD_PWR,      /*   D21   （PWR）               */
   CKEY_ID_KEY_OFF,      /*   D22   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D23   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D24   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D25   （該当キー無し）      */
   CKEY_ID_LCD_TI_SCAN,   /*   D26   （TI/SCAN）            */
   CKEY_ID_LCD_CH,         /*   D27   （CH）               */
   CKEY_ID_KEY_OFF,      /*   D28   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D29   （該当キー無し）      */
   CKEY_ID_KEY_OFF         /*   D30   （該当キー無し）      */
};
#else      /* キー配置仕様変更 */
const   BYTE   tkey_code_tbl[]= {
   CKEY_ID_KEY_OFF,      /*   D01   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D02   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D03   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D04   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D05   （該当キー無し）      */
   CKEY_ID_LCD_PWR,      /*   D06   （PWR）               */
   CKEY_ID_LCD_AS,         /*   D07   （AS）               */
   CKEY_ID_KEY_OFF,      /*   D08   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D09   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D10   （該当キー無し）      */
   CKEY_ID_LCD_SRC,      /*   D11   （MODE）            */
   CKEY_ID_LCD_VOLUP,      /*   D12   （VOLUP）            */
   CKEY_ID_KEY_OFF,      /*   D13   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D14   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D15   （該当キー無し）      */
   CKEY_ID_LCD_CH,         /*   D16   （CH）               */
   CKEY_ID_LCD_VOLDW,      /*   D17   （VOLDW）            */
   CKEY_ID_KEY_OFF,      /*   D18   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D19   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D20   （該当キー無し）      */
   CKEY_ID_LCD_RIGHT,      /*   D21   （UP）               */
   CKEY_ID_KEY_OFF,      /*   D22   （該当キー無し）   */
   CKEY_ID_KEY_OFF,      /*   D23   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D24   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D25   （該当キー無し）      */
   CKEY_ID_LCD_LEFT,      /*   D26   （LEFT）            */
   CKEY_ID_LCD_TI_SCAN,   /*   D27   （TI/SCAN）            */
   CKEY_ID_KEY_OFF,      /*   D28   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D29   （該当キー無し）      */
   CKEY_ID_KEY_OFF         /*   D30   （該当キー無し）      */
};
#endif      /* キー配置仕様変更 */
#else
extern const   BYTE   tkey_code_tbl[];
#endif

#ifdef _KEY_MAIN_C_

#if 1      /* キー配置仕様変更 */
const   BYTE   tkey_am_code_tbl[]= {
   CKEY_ID_KEY_OFF,      /*   D01   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D02   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D03   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D04   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D05   （該当キー無し）      */
   CKEY_ID_LCD_VOLUP,      /*   D06   （VOLUP）            */
   CKEY_ID_KEY_OFF,      /*   D07   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D08   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D09   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D10   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D11   （該当キー無し）      */
   CKEY_ID_LCD_RIGHT,      /*   D12   （UP）               */
   CKEY_ID_KEY_OFF,      /*   D13   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D14   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D15   （該当キー無し）      */
   CKEY_ID_LCD_VOLDW,      /*   D16   （VOLDW）            */
   CKEY_ID_LCD_LEFT,      /*   D17   （DW）               */
   CKEY_ID_KEY_OFF,      /*   D18   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D19   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D20   （該当キー無し）      */
   CKEY_ID_LCD_PWR,      /*   D21   （PWR）               */
   CKEY_ID_KEY_OFF,      /*   D22   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D23   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D24   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D25   （該当キー無し）      */
   CKEY_ID_LCD_TI_SCAN,   /*   D26   （TI/SCAN）            */
   CKEY_ID_LCD_CH,         /*   D27   （CH）               */
   CKEY_ID_KEY_OFF,      /*   D28   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D29   （該当キー無し）      */
   CKEY_ID_KEY_OFF         /*   D30   （該当キー無し）      */
};
#else      /* キー配置仕様変更 */
const   BYTE   tkey_am_code_tbl[]= {
   CKEY_ID_KEY_OFF,      /*   D01   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D02   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D03   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D04   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D05   （該当キー無し）      */
   CKEY_ID_LCD_PWR,      /*   D06   （PWR）               */
   CKEY_ID_KEY_OFF,      /*   D07   （AS）               */
   CKEY_ID_KEY_OFF,      /*   D08   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D09   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D10   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D11   （該当キー無し）      */
   CKEY_ID_LCD_VOLUP,      /*   D12   （VOLUP）            */
   CKEY_ID_KEY_OFF,      /*   D13   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D14   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D15   （該当キー無し）      */
   CKEY_ID_LCD_CH,         /*   D16   （CH）               */
   CKEY_ID_LCD_VOLDW,      /*   D17   （VOLDW）            */
   CKEY_ID_KEY_OFF,      /*   D18   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D19   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D20   （該当キー無し）      */
   CKEY_ID_LCD_RIGHT,      /*   D21   （UP）               */
   CKEY_ID_KEY_OFF,      /*   D22   （該当キー無し）   */
   CKEY_ID_KEY_OFF,      /*   D23   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D24   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D25   （該当キー無し）      */
   CKEY_ID_LCD_LEFT,      /*   D26   （LEFT）            */
   CKEY_ID_LCD_TI_SCAN,   /*   D27   （TI/SCAN）            */
   CKEY_ID_KEY_OFF,      /*   D28   （該当キー無し）      */
   CKEY_ID_KEY_OFF,      /*   D29   （該当キー無し）      */
   CKEY_ID_KEY_OFF         /*   D30   （該当キー無し）      */
};
#endif      /* キー配置仕様変更 */
#else
extern const   BYTE   tkey_am_code_tbl[];
#endif

#endif
