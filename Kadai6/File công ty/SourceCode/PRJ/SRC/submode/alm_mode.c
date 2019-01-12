/***************************************************************
   クラリオン殿向け 
   プロジェクト名   ：2012年AMFM_RADIOﾓﾃﾞﾙ
   ファイル名      ：alm_mode.c
   モジュール名   ：SUBモード(ALARM)
   機   能         ：ALARM調整処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
****************************************************************/
#define   _ALM_MODE_C_

#include   "../model.h"

#include   "../comm/common.h"
#include   "../disp/lcd_ext.h"
#include   "../key/key_func_ext.h"
#include   "../audio/aud_ext.h"
#include   "../main/ma_ext.h"
#include   "../power/pw_func_ext.h"

#include   "sbm_def.h"
#include   "sbm_ext.h"
#include   "sbm_sys.h"
#include   "sbm_keytbl.h"

/******************************************************************************
      関数名  ：Alm_lcddata_set
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm表示ﾃﾞｰﾀ作成処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_lcddata_set( TDP_CLK *almdisp )
{
   if (sbm_alm_adj.hour == 0)
   {
      almdisp->hour = 12;
   }
   else if ((sbm_alm_adj.hour >= 13)&&(sbm_alm_adj.hour <= 23))
   {
      almdisp->hour = sbm_alm_adj.hour - 12;
   }
   else
   {
      almdisp->hour = sbm_alm_adj.hour;
   }
   almdisp->min = sbm_alm_adj.min;
   almdisp->mode = sbm_alm_mode;
}

BYTE Alm_indicator_set(void)
{
   BYTE   indicator;
   
   indicator = 0;
   
   if ((sbm_alm_adj.hour >= 12)&&(sbm_alm_adj.hour <= 23))
   {
      indicator |= CLCD_CL_IND_PM;
   }
   else
   {
      indicator |= CLCD_CL_IND_AM;
   }
   return (indicator);
}

BOOL Alm_bell_indicator_set(void)
{
   return (fsbm_alm_onoff);  // nam trong file sbm_def.h
}

/******************************************************************************
      関数名  ：Alm_mode_start
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm調整モード入る処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_mode_start(void)
{
   if (Ma_get_alarm() == FALSE )
   {
      Key_nop();
      return;
   }
   
   Lcd_base_disp_cancel();
   Vol_mode_cancel();
   sbm_alm_mode = CSBM_ALM_ADJUST;
   fsbm_alm_onoff = ON;
   sbm_alm_tim = CSBM_ALM_15S;
   Lcd_blktim_set();
   
   #if 0   /* AM1:00から固定 */
   if (fsbm_alm_setted == FALSE)
   {
      #if 0
      sbm_alm_adj = sbm_clk_adj;
      #else
      sbm_alm_adj.min = sbm_clk_adj.min;
      sbm_alm_adj.hour = sbm_clk_adj.hour;      
      #endif
   }
   #endif
   
   Key_nop();
}
/******************************************************************************
      関数名  ：Alm_mode_check
      引  数   ：無し
      戻り値   ：ON ⇒Alarm調整中
            ：OFF ⇒ Alarm調整中ではない
      機  能   ：Alarm調整モード中かチェック
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
BYTE Alm_mode_check(void)
{
   if(sbm_alm_mode != CSBM_ALM_NORMAL)  // sbm_alm_mode va CSBM_ALM_NORMAL=0 duoc khoi tao trong file sbm_def.h
                                        // sbm_alm_mode duoc khoi tao lan dau voi gia tri CSBM_ALM_NORMAL trong ham Alm_mode_initial cua file alm_mode.c
   {
      return(ON);
   }
   else                // Khi mode cua alarm la NORMAL thi tra ve OFF
   {
      return(OFF);
   }
}
/******************************************************************************
      関数名  ：Alm_mode_cancel
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm調整モード中ｷｬﾝｾﾙ処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_mode_cancel(void)
{
   sbm_alm_mode = CSBM_ALM_NORMAL;
   sbm_alm_tim = 0;
   //fsbm_alm_onoff = ON;
   fsbm_beep_en = OFF;
   #if 0
   fsbm_alm_setted = ON;
   Pw_almkey_clear();
   #endif
}
/******************************************************************************
      関数名  ：Alm_mode_hour_up
      引  数   ：UP/DW
      戻り値   ：無し
      機  能   ：Alarm調整ﾓｰﾄﾞ中HOURのUP調整
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_mode_hour_up(void)
{
#if 0
   sbm_alm_adj.hour++;
   if( sbm_alm_adj.hour == 24 )
   {
      sbm_alm_adj.hour = 0;
   }
   if( sbm_alm_adj.hour < 12 )
   {
      fsbm_alm_ampm = CSBM_CLK_AM;
   }
   else
   {
      fsbm_alm_ampm = CSBM_CLK_PM;
   }
#else
   #if 0
   if( sbm_alm_adj.hour == 11 )
   {
      if( fsbm_alm_ampm == CSBM_CLK_AM )
      {
         sbm_alm_adj.hour++;
         fsbm_alm_ampm = CSBM_CLK_PM;
      }
      else
      {
         sbm_alm_adj.hour = 0;
         fsbm_alm_ampm = CSBM_CLK_AM;
      }
   }
   else if( sbm_alm_adj.hour == 12 )
   {
      sbm_alm_adj.hour = 1;
   }
   else
   {
      sbm_alm_adj.hour++;
   }
   #endif
   
   /* 時カウント */
   if( sbm_alm_adj.hour >= 23 )
   {
      sbm_alm_adj.hour = 0;
   }
   else
   {
      sbm_alm_adj.hour++;
   }
   
   #if 0
   /* AMかPMの設定 */
   if(( sbm_alm_adj.hour >= 12)&&(sbm_alm_adj.hour <= 23))
   {
      fsbm_clk_ampm = CSBM_CLK_PM;
   }
   else
   {
      fsbm_clk_ampm = CSBM_CLK_AM;
   }
   #endif
#endif
   
   sbm_alm_tim = CSBM_ALM_15S;
   Lcd_blktim_set();         /* 点滅ﾀｲﾏ再設定 */
}
/******************************************************************************
      関数名  ：Alm_mode_min_up
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm調整ﾓｰﾄﾞ中MINのUP調整
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_mode_min_up(void)
{
   sbm_alm_adj.min++;
   if( sbm_alm_adj.min == 60 )
   {
      sbm_alm_adj.min = 0;
   }
   
   sbm_alm_tim = CSBM_ALM_15S;
   Lcd_blktim_set();         /* 点滅ﾀｲﾏ再設定 */
}
/******************************************************************************
      関数名  ：Alm_mode_set
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm調整済みデータ確定
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_mode_set(void)
{
   #if 0
   sbm_alm_mode = CSBM_ALM_NORMAL;
   sbm_alm_tim = 0;
   fsbm_alm_onoff = ON;
   fsbm_beep_en = ON;
   fsbm_alm_setted = ON;
   #else
   Alm_mode_cancel();
   Key_nop();
   #endif
}
/******************************************************************************
      関数名  ：Alm_onoff_set
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm ONOFF状態設定
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_onoff_set(void)
{
   if (Ma_get_alarm() == FALSE )
   {
      Key_nop();
      return;
   }
   
   fsbm_alm_onoff = ~fsbm_alm_onoff;
   fsbm_beep_en = OFF;
   Lcd_1shot_cancel(CLCD_1SHOT_OFF);         /* 一時表示ｷｬﾝｾﾙ */
   /* Lcd_send_req(); */
   Lcd_accoff_send_req();
   Key_nop();
}
/******************************************************************************
      関数名  ：Alm_beepon_get
      引  数   ：無し
      戻り値   ：ON/OFF
      機  能   ：beep鳴動中かどうか取得
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
BOOL Alm_beepon_get(void)
{
   return(fsbm_alm_beepon);
}
/******************************************************************************
      関数名  ：Alm_beepon_get
      引  数   ：無し
      戻り値   ：ON/OFF
      機  能   ：beep鳴動中かどうか取得
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_key_pwr_on(void)
{
   /* アラームキャンセル要求を出す */
   fsbm_alm_off_req = TRUE;
   
   Pw_power_key();
   Key_nop();
}

/******************************************************************************
      関数名  ：Alm_key_up_off
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm調整モードUPｷｰOFF処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_key_up_off(void)
{
   Alm_mode_min_up();
   Key_nop();
}
/******************************************************************************
      関数名  ：Alm_key_up_long
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm調整モードUPｷｰLONG処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_key_up_long(void)
{
   Alm_mode_min_up();
   Key_long_time200();
}
/******************************************************************************
      関数名  ：Alm_key_dw_off
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm調整モードDOWNｷｰOFF処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_key_dw_off(void)
{
   Alm_mode_hour_up();
   Key_nop();
}
/******************************************************************************
      関数名  ：Alm_key_dw_long
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm調整モードDOWNｷｰLONG処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Alm_key_dw_long(void)
{
   Alm_mode_hour_up();
   Key_long_time200();
}

void Alm_key_tisc_on(void)
{
   BYTE   destno;
   
   destno = Ma_model_dest();/*仕向けNo取得*/
   
   if ((destno == CMA_MODEL_AMFM_J)&&(Pw_Power_status_get() == ON))
   {
      Tu_key_isr_on();/*TIモードへ遷移*/
   }
   
   Key_nop();/*無効*/
}

/******************************************************************************
      関数名  ：Alm_beep_stop
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm beep on時、beep鳴動停止処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/25      0.1      lrj      新規
******************************************************************************/
void Alm_beep_on_stop(void)
{
   fsbm_alm_beepon = OFF;
   Pw_acc_wakeup_req(OFF);
}

/******************************************************************************
      関数名  ：Alm_beep_key_stop
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm beep on時、Power以外のｷｰON処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/25      0.1      lrj      新規
******************************************************************************/
void Alm_beep_key_stop(void)
{
   Sbm_mode_cancel(CSBM_OFF);
   Aud_cancel_beep();   /* beep off */
   Alm_beep_on_stop();
   Key_nop();
}
void Alm_key_volupon(void)
{
   if (Pw_Power_status_get() == ON)
   {
      Sbm_key_volupon();
   }
   else
   {
      Key_nop();
   }
}
void Alm_key_voluplong(void)
{
   Alm_mode_cancel();
   Sbm_key_voluplong();
}
void Alm_key_volupoff(void)
{
   Alm_mode_cancel();
   Sbm_key_volupoff();
}
void Alm_key_voldwon(void)
{
   if (Pw_Power_status_get() == ON)
   {
      Sbm_key_voldwon();
   }
   else
   {
      Key_nop();
   }
}
void Alm_key_voldwlong(void)
{
   Alm_mode_cancel();
   Sbm_key_voldwlong();
}
void Alm_key_voldwoff(void)
{
   Alm_mode_cancel();
   Sbm_key_voldwoff();
}

/******************************************************************************
      関数名  ：Alm_time_initial
      引  数   ：無し
      戻り値   ：無し
      機  能   ：Alarm時刻初期化処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/30      0.1      lrj      新規
******************************************************************************/
void Alm_mode_initial(void)
{
   sbm_alm_adj.min = 0x00;
   sbm_alm_adj.hour = 0x01;
   sbm_alm_bak.min = 0x00;   // bak = backup
   sbm_alm_bak.hour = 0x01;
   sbm_alm_tim = 0;
   sbm_alm_mode = CSBM_ALM_NORMAL;
}














#undef _ALM_MODE_C_
