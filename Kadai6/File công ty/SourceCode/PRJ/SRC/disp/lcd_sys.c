/***************************************************************
   クラリオン殿向け   
   プロジェクト名   ：'07年モデル開発
   ファイル名      ：lcd_sys.c
   モジュール名   ：LCD(表示)
   機   能         ：Lcd systemメッセージ受付処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/31      0.1      ha      新規作成
****************************************************************/
#define      _LCD_SYS_C_

#include    <stdio.h>
#include    <string.h>

#include   "../model.h"

#include   "../comm/common.h"
#include   "../main/ma_ext.h"
#include   "../power/pw_func_ext.h"
#include   "lcd_sys.h"
#include   "lcd_main.h"
#include   "lcd_send.h"
#include   "lcd_ext.h"
#include   "lcd_mode.h"

static void Lcd_pwr_on(void);
static void Lcd_pwr_off(void);
static void Lcd_mode_changer(void);
static void Lcd_maincnt_on(void);
static void Lcd_maincnt_offreq(void);

/****************************************************************
      関数名：lcd_sys_message
      引  数：Systemメッセージ
      戻り値：無し
      機  能：LcdSystemﾒｯｾｰｼﾞ受付処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/31      0.1      ha      新規作成
****************************************************************/
void   Lcd_sys_message( BYTE msg )
{
   switch(msg)
   {
      case   CMSG_MAIN:
         Lcd_main();                           /* LCDメイン処理 */
         break;
      case   CMSG_INITIAL:
         Lcd_initial();                        /* ﾃﾞｰﾀ、ﾊﾞｯﾌｧなどの初期設定 */
         break;
      case CMSG_MAINCNT_ON:
         Lcd_maincnt_on();
         break;
      case CMSG_MAINCNT_OFFREQ:      // duoc set trong ham Pw_AccOff_In cua file pw_main.c
         Lcd_maincnt_offreq();
         break;
      case   CMSG_PWRON:
         Lcd_pwr_on();                        /* PWR ON処理   Nhan nut Power, khi dang OFF -> ON. 
                                               Trong folder 003, file excel DISP, muc 6, lien quan den viec khoi dong 5s*/
         break;
      case   CMSG_MODECHG:
         Lcd_mode_changer();                     /* ｿｰｽ遷移処理 */
         break;
      case   CMSG_PWROFF:
         Lcd_pwr_off();                        /* PWR OFF処理 */
         break;
      case   CMSG_10MS_TIMER:
         break;
      case   CMSG_25MS_TIMER:
         Lcd_25ms_cnt();                        /* ﾀｲﾏｶｳﾝﾄ */
         break;
      case   CMSG_50MS_TIMER:
         Lcd_50ms_cnt();                        /* ﾀｲﾏｶｳﾝﾄ */
         break;
      case   CMSG_100MS_TIMER:
         Lcd_100ms_cnt();                     /* ﾀｲﾏｶｳﾝﾄ */
         break;
      default:
         break;
   }
}
/****************************************************************
      関数名：Lcd_maincnt_on
      引  数：無し
      戻り値：無し
      機  能：Acc Onﾒｯｾｰｼﾞ受付処理           Acc On message acceptance processing
      コメント：キースキャン許可(KSC：0)      Key scan permission (KSC: 0)
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/31      0.1      ha      新規作成
****************************************************************/
static void Lcd_maincnt_on(void)
{
   flcd_test_mode = FALSE;
}
/****************************************************************
      関数名：Lcd_maincnt_off
      引  数：無し
      戻り値：無し
      機  能：Acc Offﾒｯｾｰｼﾞ受付処理
      コメント：キースキャン禁止(KSC：1)
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/31      0.1      ha      新規作成
****************************************************************/
static void Lcd_maincnt_offreq(void)
{
   flcd_test_mode = FALSE;
   
   /* AccOFF時Alarm調整に入って解除する時に、ここで表示更新要求をだす */
   flcd_accoff_send = TRUE;
}
/****************************************************************
      関数名：Lcd_pwr_on
      引  数：無し
      戻り値：無し
      機  能：PWR ONメッセージを受けてから処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/31      0.1      ha      新規作成
****************************************************************/
static void Lcd_pwr_on(void)
{
   Lcd_base_scrll(Ma_current_mode());         /* ﾍﾞｰｽ表示要求 Base display request, trong file lcd_ext.c*/
   flcd_test_mode = FALSE;
}
/****************************************************************
      関数名：Lcd_pwr_off
      引  数：無し
      戻り値：無し
      機  能：PWR OFFメッセージを受けてから処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/31      0.1      ha      新規作成
****************************************************************/
static void Lcd_pwr_off(void)
{
   Lcd_1shot_cancel( CLCD_1SHOT_OFF );         /* 一時表示強制ｷｬﾝｾﾙ  CLCD_1SHOT_OFF =  0xFF    Forced Cancel Temporary Display CLCD_1SHOT_OFF = 0xFF
                                                 Lcd_1shot_cancel trong file lcd_ext.c.  CLCD_1SHOT_OFF = 0xFF*/
   flcd_test_mode = FALSE;
}
/****************************************************************
      関数名：Lcd_mode_changer
      引  数：無し
      戻り値：無し
      機  能：ﾓｰﾄﾞ遷移メッセージを受けてから処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2006/6/8      0.1      徐      新規作成
****************************************************************/
static void Lcd_mode_changer(void)
{
   BYTE mode;
   if(Pw_Power_status_get() == ON)               /* PWR ON時しか処理しない                */
   {
      Lcd_1shot_cancel(CLCD_1SHOT_OFF);         /* 一時表示強制ｷｬﾝｾﾙ                   */
      Lcd_1shot_src();                     /* ソース遷移一時表示設定                */
      mode =Ma_current_mode();
      Lcd_base_scrll(Ma_current_mode());         /* ﾍﾞｰｽ表示要求   Base display required                    */
   }
}
/****************************************************************
      関数名：Lcd_1shot_src
      引  数：無し
      戻り値：無し
      機  能：ｿｰｽ切替一時表示設定   Source switching temporary display setting
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/31      0.1      ha      新規作成
****************************************************************/
void Lcd_1shot_src(void)
{
   BYTE ma_mode;
   ma_mode = Ma_current_mode();
   #if 1
   Lcd_disp_1shot_base(ma_mode);
   #else
   switch(ma_mode)
   {
      case CMA_TUNER:
         lcd_src_mode = CLCD_SRC_TUNER;
         lcd_src_tim = CLCD_SRC_TIM;                  /* 2秒ﾀｲﾏ */
         break;
      case CMA_ISR:
         lcd_src_mode = CLCD_SRC_ISR;
         lcd_src_tim = CLCD_SRC_TIM;
         break;
      case CMA_AUX:
         lcd_src_mode = CLCD_SRC_AUX;            /* HUBなし */
         lcd_src_tim = CLCD_SRC_TIM;                  /* 2秒ﾀｲﾏ */
         break;
      default:
         break;
   }
   #endif
}

#undef   _LCD_SYS_C_
