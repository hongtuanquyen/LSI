/******************************************************************************
   クラリオン殿向け
   プロジェクト名   : 2012年AMFM_RADIOモデル
   ファイル名      : key_sys.c
   機能         : KeyモジュールSystemメッセージ受付処理
------------------------------------------------------------------------------
日付      担当者      修正内容
2012/07/26   東海林      新規作成
******************************************************************************/
#define      _KEY_SYS_C_

#include   "../comm/common.h"
#include   "key_def.h"
#include   "key_func_ext.h"
#include   "key_sys.h"
#include   "key_main.h"

static   void   Key_sys_25ms(void);
static   void   Key_sys_50ms(void);

/*****************************************************************************
      関数名  ：Key_sys_message
      引  数   ：Systemメッセージ
      戻り値   ：無し
      機  能   ：Keyモジュール Systemメッセージ受付処理     Xu li viec tiep nhan System message module key
------------------------------------------------------------------------------
日付      担当者      修正内容
2012/07/26   東海林      新規作成
******************************************************************************/
void   Key_sys_message(BYTE msg)
{
   switch(msg)
   {
      case   CMSG_MAIN:
         Key_main();
         break;
      case   CMSG_INITIAL:
      case   CMSG_MAINCNT_OFFREQ:
         Key_initial();
         break;
      case   CMSG_MODECHG:
         Key_nop();                  /* モード切替中押されているキー無効にする vo hieu hoa key dang duoc nhan trong qua trinh chuyen doi MODE*/
         break;
      case   CMSG_25MS_TIMER:
         Key_sys_25ms();
         break;
      default:
         break;
   }
   return;
}

/******************************************************************************
   関数名   ：Key_sys_25ms
   引  数   ：無し    Khong co doi so
   戻り値   ：無し   Khong co gia tri tra ve
   機  能   ：Keyモジュール25ms周期タイマ処理   Xu li timer chu ki 25ms module key
------------------------------------------------------------------------------
日付      担当者      修正内容
2012/07/26   東海林      新規作成
******************************************************************************/
static   void   Key_sys_25ms(void)
{
   if (key_in_disable_timer  != CKEY_TIME_UP)
   {
      key_in_disable_timer--;
   }
   if (key_long_timer != 0xff)
   {
      if(key_long_timer > CKEY_TIME_UP)
      {
         key_long_timer--;
      }
   }
   return;
}

#undef _KEY_SYS_C_
