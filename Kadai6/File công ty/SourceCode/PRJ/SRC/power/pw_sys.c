/******************************************************************************
   クラリオン（株）殿向け      2007 C3X03 ｼﾘｰｽﾞ[仮]

      プロジェクト名   : '07年フリップダウン　MP3モデル開発
      ファイル名      : pw_sys.c
      機能         : �@電源制御ﾓｼﾞｭｰﾙ Systemﾒｯｾｰｼﾞ受付処理
                  : �A電源制御ﾓｼﾞｭｰﾙ ﾀｲﾏｶｳﾝﾄ処理
------------------------------------------------------------------------------
修正履歴
【番号】   日付      担当者   修正内容
【NKC-0000】'05/12/09   仲地   新規作成
******************************************************************************/
#define      _PW_SYS_C_

#include   "../comm/common.h"

#include   "pw_sys.h"
#include   "pw_main.h"

static   void   Pw_sys_500ms(void);
static   void   Pw_sys_10ms(void);

/******************************************************************************
..       関数名  ：Pw_sys_message
      引  数   ：Systemﾒｯｾｰｼﾞ
      戻り値   ：無し
      機  能   ：TunerSystemﾒｯｾｰｼﾞ受付処理
------------------------------------------------------------------------------
修正履歴
【番号】   日付      担当者   修正内容
【NKC-0000】'05/12/09   仲地   新規作成
******************************************************************************/
void   Pw_sys_message(BYTE   msg)
{
   switch(msg)
   {
      case   CMSG_MAIN:
         Pw_main();
         break;
      case   CMSG_10MS_TIMER:
         Pw_sys_10ms();
         break;
      case   CMSG_INITIAL:
         Pw_initial();
         break;
      default:
         break;
   }
}
/******************************************************************************
   関数名   ：Pw_sys_10ms
   引  数   ：無し
   戻り値   ：無し
   機  能   ：電源制御ﾓｼﾞｭｰﾙ10ms周期ﾀｲﾏ処理
         ：1ms許可中DCP、ILLDETのﾁｬﾀﾘﾝｸﾞ処理を呼び出し
------------------------------------------------------------------------------
修正履歴
[sun070301] DCP検出処理追加
******************************************************************************/
static   void   Pw_sys_10ms(void)
{
   #if 0
   if (Ma_chk_1ms() == ON)
   {
      Pw_dcp_check();
      Pw_illdet_check();
   }
   #endif
}


#undef  _PW_SYS_C_
