/***************************************************************
   クラリオン殿向け 
   プロジェクト名   ：2012年AMFM_RADIOﾓﾃﾞﾙ
   ファイル名      ：clock_mode.c
   モジュール名   ：SUBモード(時計)
   機   能         ：時計ｶｳﾝﾄ処理、時計調整処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/23      0.1      lrj      新規
****************************************************************/
#define   _CLOCK_MODE_C_

#include   "../model.h"

#include   "../comm/common.h"
#include   "../io/IO_inc.h"
#include   "../disp/lcd_ext.h"
#include   "../key/key_func_ext.h"
#include   "../main/ma_ext.h"
#include   "../tuner/tu_key.h"
#include   "../audio/Aud_inc.h"

#include   "sbm_def.h"
#include   "sbm_ext.h"
#include   "sbm_sys.h"
#include   "sbm_keytbl.h"

static void Clk_io_data_set(void);
static void Clk_Acc_Sta_Monitor(void);
static BYTE acc_sta;
static BYTE acc_sta_bak;
static BYTE pwr_sta;
static BYTE pwr_sta_bak;

/******************************************************************************
      関数名  ：Clk_lcddata_set
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計表示ﾃﾞｰﾀ作成処理    // Xu li viec tao data hien thi dong ho
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/23      0.1      lrj      新規
******************************************************************************/
void   Clk_lcddata_set( TDP_CLK *clkdisp )
{
   if (Alm_mode_check() != OFF)  // trong file alm_mode.c  Neu dang trong qua trinh dieu chinh Alarm
   {
      /* alm の表示データを渡す */
      Alm_lcddata_set(clkdisp);
   }
   else     // Neu dang trong qua trinh dieu chinh Dong ho
   {
      if (sbm_clk_adj.hour == 0) // trong file sbm_def.h
      {
         clkdisp->hour = 12;
      }
      else if ((sbm_clk_adj.hour>=13)&&(sbm_clk_adj.hour<=23))
      {
         clkdisp->hour = sbm_clk_adj.hour - 12;   // 12-hour format
      }
      else
      {
         clkdisp->hour = sbm_clk_adj.hour;
      }
      clkdisp->min = sbm_clk_adj.min;
      
      clkdisp->mode = sbm_clk_mode; /* sbm_clk_mode duoc khoi tao lan dau va = voi CSBM_CLK_NORMAL trong ham Clk_mode_initial() cua ham Sbm_mode_initial 
                                          trong file sbm_ext.c */
   }
}

BYTE Clk_indicator_set(void)
{
   BYTE   indicator;
   
   indicator = 0;
   
   #if 0
   if (Alm_mode_check() != OFF)
   {
      indicator = Alm_indicator_set();
   }
   else
   #endif
   {
      if ((sbm_clk_adj.hour >= 12)&&(sbm_clk_adj.hour <= 23))
      {
         indicator |= CLCD_CL_IND_PM; // trong file lcd_ext.h, CLCD_CL_IND_PM=0x02;
      }
      else
      {
         indicator |= CLCD_CL_IND_AM; // trong file lcd_ext.h, CLCD_CL_IND_PM=0x01;
      }
   }
   
   #if 0
   if( fsbm_alm_onoff == ON )
   {
      indicator |= CLCD_CL_IND_AL;
   }
   #endif
   
   return(indicator);
}
/******************************************************************************
      関数名  ：Clk_time_main
      引  数   ：なし
      戻り値   ：無し
      機  能   ：時計ﾀｲﾏ取得処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/23      0.1      lrj      新規
******************************************************************************/
void   Clk_time_main(void)
{
   WORD io_min;
   
   if (Ma_get_clock() == FALSE )
   {
      return;
   }
   
   //Pw_acc_wakeup();
   io_min = Io_mincnt_read();
   
   /* IOからﾀｲﾏを取得 */
   sbm_clk_adj.hour = (BYTE)(io_min/60);
   sbm_clk_adj.min = (BYTE)(io_min%60);
   
   if (( sbm_clk_bak.min != sbm_clk_adj.min )
     ||( sbm_clk_bak.hour != sbm_clk_adj.hour ))
   {
      sbm_clk_bak = sbm_clk_adj;
      if (Pw_ACC_status_get() == OFF)
      {
         /* Lcd_send_req(); */
         Lcd_accoff_send_req();
      }
   }
   
   /* ALARMチェック処理 */
   #if 0
   if  (( fsbm_alm_onoff == ON )
      &&( fsbm_beep_en == ON )
      &&( Alm_mode_check() == OFF )
      &&( sbm_clk_adj.min == sbm_alm_adj.min )
      &&( sbm_clk_adj.hour == sbm_alm_adj.hour ))
   {
      /* BEEPする */
      if (fsbm_alm_beepon == OFF)/* ALARM起動状態設定 */
      {
         fsbm_alm_beepon = ON;
         fsbm_beep_en = OFF;
         Aud_set_beep(CAUD_REQ_BEEP_ALARM);
         /* 180Sタイム設定 */
         sbm_beepon_tim = CSBM_BEEPON_180S;
         //if (Pw_ACC_status_get() == OFF)
         {
            Pw_acc_wakeup_req(ON);
         }
      }
   }
   #else
   
   if (Ma_get_alarm() == TRUE)
   {
      if  (( sbm_clk_adj.min == sbm_alm_adj.min )        // Khi ma thoi gian CLOCK chay toi bang voi thoi gian ALARM dc set th� se thiet lap de b痂 cho ng dung
         &&( sbm_clk_adj.hour == sbm_alm_adj.hour ))
      {
         if  (( fsbm_alm_onoff == ON )
            &&( Alm_mode_check() == OFF )  // ALARM dang normal
            &&( Clk_adj_check() == OFF )   // CLOCK dang normal
            &&( fsbm_alm_beepon == OFF )
            &&( fsbm_beep_en == ON ))
         {
            fsbm_alm_beepon = ON;
            fsbm_beep_en = OFF;
            /*Trong TH dang o PWRON, Alarm lap tuc se b痂. 
              Neu dang o PWROFF, se set fpw_wakeup = ON, POWER module se dua vao co nay ma set he thong len PWRON va ALARM se b痂*/
            if (Pw_Power_status_get()  == ON)    // check pw_curmode
            {
               Sbm_mode_cancel(CSBM_OFF);   
               Aud_cancel_beep();
               /* beep req */
               /* PowerONじゃなければ、PWが管理する */
               Aud_set_beep(CAUD_REQ_BEEP_ALARM);
            }
            else
            //if (Pw_ACC_DET_get() == OFF)
            {
               Pw_acc_wakeup_req(ON);   // set ON fpw_wakeup. Neu dang o PWROFF -> PWRON v� alarm b痂. 
                                        // Neu dang o ACCOFF(pw_curmode = ACCOFF) th� se chuyen sang PWRON v� alarm b痂. (khi nay ta goi ACCOFF dang trong tinh trang NORMAL)
            }
         }
      }
      else    // Truong hop CLOCK chua toi hoac da qua thoi gian ALARM, viec bao thuc se dc reset lai
      {
         fsbm_beep_en = ON;
      }
   }
   #endif
   
   /*   acc状態監視処理   */
   Clk_Acc_Sta_Monitor();
   
   
}

void Clk_Acc_Sta_Monitor(void)
{
   acc_sta = Pw_ACC_DET_get();
   
   if (acc_sta_bak != acc_sta)  // Kiem tra status ACC co thay doi ko, neu co thi dung hoat dong cua alarm va clock
   {
      Alm_beep_key_stop();
      acc_sta_bak = acc_sta;
      Lcd_disp_base(Ma_current_mode());
   }
   
   pwr_sta = Pw_Power_status_get();
   
   if (pwr_sta_bak != pwr_sta)     // Kiem tra status power co thay doi ko, neu co thi dung hoat dong cua alarm va clock
   {
      if (fsbm_alm_off_req == TRUE)
      {
         Alm_mode_cancel();
         fsbm_alm_off_req = FALSE;
      }
      Clk_adj_cancel();
      pwr_sta_bak = pwr_sta;
   }
}
/******************************************************************************
      関数名  ：Clk_key_up_off
      引  数   ：void
      戻り値   ：無し
      機  能   ：時計時刻初期化処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/30      0.1      lrj      新規
******************************************************************************/
void Clk_mode_initial(void)
{
   if (Ma_get_clock() == FALSE )
   {
      return;
   }
   sbm_clk_adj.min = 0x00;
   sbm_clk_adj.hour = 0x01;
   Clk_io_data_set();
   //fsbm_clk_ampm = CSBM_CLK_AM;
   sbm_clk_tim = 0;
   sbm_clk_mode = CSBM_CLK_NORMAL;
   
   acc_sta = 0;
   acc_sta_bak = 0;
   pwr_sta = 0;
   pwr_sta_bak = 0;

}


void Clk_io_data_set(void)
{
   WORD io_min;
   
   io_min = (sbm_clk_adj.hour * 60) + sbm_clk_adj.min;
   if (io_min <= 0x59F)   /* 23：59まで   TH l� 23h59 => io_min = 23*60 + 59 = 1439(DEC) = 0x59F(HEX)*/
   {
      Io_mincnt_write(io_min);
   }
}
/******************************************************************************
      関数名  ：Clk_adj_start
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整ﾓｰﾄﾞ始まる処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/23      0.1      lrj      新規
******************************************************************************/
void   Clk_adj_start(void)
{
   if (Ma_get_clock() == FALSE )
   {
      Key_nop();
      return;
   }
   
   Vol_mode_cancel();
   Lcd_base_disp_cancel();
   sbm_clk_mode = CSBM_CLK_ADJUST;            /* 時の調整ﾓｰﾄﾞへｾｯﾄ */
   sbm_clk_tim = CSBM_CLK_15S;
   Lcd_blktim_set();
   Key_nop();
}
/******************************************************************************
      関数名  ：Clk_adj_check
      引  数   ：無し
      戻り値   ：ON ⇒時計調整中
            ：OFF ⇒ 時計調整中ではない
      機  能   ：時計調整ﾓｰﾄﾞ中かチェック処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/23      0.1      lrj      新規
******************************************************************************/
BYTE   Clk_adj_check(void)
{
   if (sbm_clk_mode != CSBM_CLK_NORMAL)
   {
      return(ON);
   }
   else
   {
      return(OFF);
   }
}
/******************************************************************************
      関数名  ：Clk_adj_cancel
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整ﾓｰﾄﾞ中ｷｬﾝｾﾙ処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void   Clk_adj_cancel(void)
{
   sbm_clk_mode = CSBM_CLK_NORMAL;         /* 調整ﾓｰﾄﾞｷｬﾝｾﾙ */
   sbm_clk_tim = 0;
   fsbm_beep_en = OFF;
   #if 0   /* hourとminを調整する度に、時刻設定する */
   Io_seccnt_clear();
   Clk_io_data_set();
   #endif
}

/******************************************************************************
      関数名  ：Clk_adj_hour_up
      引  数   ：UP/DW
      戻り値   ：無し
      機  能   ：時計ﾓｰﾄﾞ中HOURのUP調整
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_adj_hour_up(void)
{
   /* 時カウント */
   if( sbm_clk_adj.hour >= 23 )
   {
      sbm_clk_adj.hour = 0;
   }
   else
   {
      sbm_clk_adj.hour++;
   }
   #if 0   /* Clk_io_data_set処理で秒を同時にクリアする */
   Io_seccnt_clear();
   #endif
   Clk_io_data_set();
   sbm_clk_tim = CSBM_CLK_15S;
   Lcd_blktim_set();
}
/******************************************************************************
      関数名  ：Clk_adj_min_up
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計ﾓｰﾄﾞ中MINのUP調整
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_adj_min_up(void)
{
   sbm_clk_adj.min++;
   if( sbm_clk_adj.min == 60 )
   {
      sbm_clk_adj.min = 0;
   }
   #if 0   /* Clk_io_data_set処理で秒を同時にクリアする */
   Io_seccnt_clear();
   #endif
   Clk_io_data_set();
   sbm_clk_tim = CSBM_CLK_15S;
   Lcd_blktim_set();
}
/******************************************************************************
      関数名  ：Clk_adj_tim_rst
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計ﾓｰﾄﾞ中時刻リスト処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_adj_tim_rst(void)
{
   #if 0   /* 「時刻リセット機能」が、「秒リセット機能」になった */
   if( sbm_clk_adj.min >= 30 )
   {
      Clk_adj_hour_up();
   }
   sbm_clk_adj.min = 0;
   
   Clk_io_data_set();
   sbm_clk_tim = CSBM_CLK_15S;
   #else
   
   #if 0   /* 秒リセットもMINレジスタ書込む */
   Io_seccnt_clear();
   #else
   Clk_io_data_set();
   #endif
   Lcd_blktim_set();
   sbm_clk_tim = CSBM_CLK_15S;
   #endif
}
/******************************************************************************
      関数名  ：Clk_adj_set
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整済みデータ確定
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void   Clk_adj_set(void)
{
   Clk_adj_cancel();
   Key_nop();
}
/******************************************************************************
      関数名  ：Clk_key_up_off
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整モードUPｷｰOFF処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_key_up_off(void)
{
   if(Ma_model_dest() == CMA_MODEL_AM_J)
   {
      Clk_adj_hour_up();
   }
   else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
   {
      Clk_adj_min_up();
   }
   else
   {
      /* No Operation */
   }
   Key_nop();
}
/******************************************************************************
      関数名  ：Clk_key_up_long
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整モードUPｷｰLONG処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_key_up_long(void)
{
   if(Ma_model_dest() == CMA_MODEL_AM_J)
   {
      Clk_adj_hour_up();
      Key_long_time200();
   }
   else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
   {
      Clk_adj_min_up();
      Key_long_time200();
   }
   else
   {
      /* No Operation */
   }
}
/******************************************************************************
      関数名  ：Clk_key_dw_off
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整モードDOWNｷｰOFF処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_key_dw_off(void)
{
   if(Ma_model_dest() == CMA_MODEL_AM_J)
   {
      Clk_adj_min_up();
   }
   else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
   {
      Clk_adj_hour_up();
   }
   else
   {
      /* No Operation */
   }
   Key_nop();
}
/******************************************************************************
      関数名  ：Clk_key_dw_long
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整モードDOWNｷｰLONG処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_key_dw_long(void)
{
   if(Ma_model_dest() == CMA_MODEL_AM_J)
   {
      Clk_adj_min_up();
      Key_long_time200();
   }
   else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
   {
      Clk_adj_hour_up();
      Key_long_time200();
   }
   else
   {
      /* No Operation */
   }
}
/******************************************************************************
      関数名  ：Clk_key_as_off
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整モードのASｷｰOFF処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_key_as_off(void)
{
   Clk_adj_tim_rst();
   Clk_adj_cancel();
   Key_nop();
}

/******************************************************************************
      関数名  ：Clk_key_as_off
      引  数   ：無し
      戻り値   ：無し
      機  能   ：時計調整モードのASｷｰOFF処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/7/24      0.1      lrj      新規
******************************************************************************/
void Clk_key_md_off(void)
{
   BYTE mo;
   
   mo = Ma_current_mode();
   if (mo == CMA_AUX)
   {
      Tu_amband_set();
      Ma_key_src();
   }
   else
   {
      Tu_key_md_off();
   }
}

void Clk_key_pwr_long(void)
{
   Clk_adj_set();
   Key_nop();
}

void Clk_key_voluplong(void)
{
   Clk_adj_cancel();
   Sbm_key_voluplong();
}
void Clk_key_volupoff(void)
{
   Clk_adj_cancel();
   Sbm_key_volupoff();
}
void Clk_key_voldwlong(void)
{
   Clk_adj_cancel();
   Sbm_key_voldwlong();
}
void Clk_key_voldwoff(void)
{
   Clk_adj_cancel();
   Sbm_key_voldwoff();
}

#if _TEST_ALARM_ON_
void Clk_alarm_test(void)
{
   fsbm_alm_beepon = ON;
   fsbm_beep_en = OFF;
   if (Pw_Power_status_get()  == ON)
   {
      Aud_cancel_beep();
      /* beep req */
      /* PowerONじゃなければ、PWが管理する */
      Aud_set_beep(CAUD_REQ_BEEP_ALARM);
   }
   else
   {
      Pw_acc_wakeup_req(ON);
   }
}
#endif

#undef _CLOCK_MODE_C_
