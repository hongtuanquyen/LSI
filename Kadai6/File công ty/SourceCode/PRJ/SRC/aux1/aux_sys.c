/******************************************************************************
   クラリオン殿向け
   プロジェクト名   : 2012年AMFM_RADIOモデル
   ファイル名      : aux_sys.c
   機能         : AUXモジュールSystemメッセージ受付処理
------------------------------------------------------------------------------
日付      担当者      修正内容
2012/07/26   東海林      新規作成
******************************************************************************/
#define      _AUX_SYS_C_

#include   "../comm/common.h"
#include   "../main/ma_ext.h"
#include   "../power/pw_func_ext.h"
#include   "../audio/aud_ext.h"

#include   "aux_sys.h"

static void Aux_main(void);

/******************************************************************************
..       関数名  ：aux_sys_message
      引  数   ：Systemメッセージ
      戻り値   ：無し
      機  能   ：AUXSystemメッセージ受付処理
------------------------------------------------------------------------------
日付      担当者      修正内容
2012/07/26   東海林      新規作成
******************************************************************************/
void   Aux_sys_message(BYTE   msg)
{
   switch(msg)
   {
      case CMSG_MAIN:
         Aux_main();
         break;
      default:
         break;
   }
}
/******************************************************************************
..       関数名  ：Aux_main
      引  数   ：無し
      戻り値   ：無し
      機  能   ：MAIN処理
------------------------------------------------------------------------------
日付      担当者      修正内容
2012/07/26   東海林      新規作成
2012/09/25   leejh      Audio Power状態判断追加
******************************************************************************/
static void Aux_main(void)
{
   if(((Pw_Power_status_get() == ON) &&(Ma_current_mode() == CMA_AUX)) &&
      (Pw_ACC_DET_get() == ON)&&
      (Aud_get_power_status() == CAUD_PWR_ON))
   {
      Aud_set_prg_mute(CAUD_MUTE_AUX_NR_MUTE,OFF);   /* MuteOff   AUX chi hoat dong trong trang thai PWRON, khi OFF n� se ngung hoat dong*/
   }
   else
   {
      Aud_set_prg_mute(CAUD_MUTE_AUX_NR_MUTE,ON);      /* MuteOn */
   }
}

#undef  _AUX_SYS_C_
