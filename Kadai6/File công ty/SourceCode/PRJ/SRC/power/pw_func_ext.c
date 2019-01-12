/******************************************************************************
   クラリオン（株）殿向け      2007 C3X03 ｼﾘｰｽﾞ[仮]

      プロジェクト名   : '07年フリップダウン　MP3モデル開発
      ファイル名      : pw_func_ext.c
      機能         : 
------------------------------------------------------------------------------
修正履歴
【番号】   日付      担当者   関数名      修正内容
【NKC-0000】'05/12/09   仲地      新規作成
------------------------------------------------------------------------------
[sun060717-1]BATT ON検出で即MUTEﾎﾟｰﾄをｱｸﾃｨﾌﾞに変更、BATT ONでボツ音が出るため
[sun060725]DM関係削除
******************************************************************************/
#define   _PW_FUNC_C_

#include   "../comm/common.h"
#include   "../main/ma_ext.h"
#include   "../key/key_func_ext.h"
#include   "../io/IO_inc.h"
#include   "../disp/lcd_ext.h"
#include   "../audio/aud_ext.h"
#include   "../testmode/test_ext.h"
#include   "../submode/sbm_ext.h"

#include   "pw_main.h"
#include   "pw_def.h"
#include   "pw_func_ext.h"

/* ACC_DETチャタリング処理 */
static   void   Pw_accdet_ONchat( void );
static   void   Pw_accdet_OFFchat( void );
static   void   Pw_illdet_chat(void);
static   void   Pw_almkey_chat(void);

/******************************************************************************
   関　数　名      ：Pw_1ms_timer
   引　　　数      ：無し
   戻　り　値      ：無し
   機　能　概　略   ：●Pw Module 1ms ｲﾝﾀｰﾊﾞﾙ処理
               ①　1msﾍﾞｰｽ･ﾀｲﾏｰｶｳﾝﾄ処理
   コ　メ　ン　ト   ：1ms割込み処理よりCallされる。
------------------------------------------------------------------------------
修正履歴
【番号】   日付      担当者      修正内容
【NKC-0000】'05/12/09   仲地      新規作成
------------------------------------------------------------------------------
[chin20120816]新規作成
******************************************************************************/
void   Pw_1ms_timer(void)
{
   if (pw_seq_timer > 0)
   {
      pw_seq_timer--;
   }
   
   #if _POWER_DIAG_
   Pw_DIAG_check(CPW_DIAG_MD_1MS);
   #endif   /* _POWER_DIAG_ */
}
void   Pw_2ms_timer(void)
{
   /* AccON中のAccOFFチャタリング処理 */
   Pw_accdet_OFFchat();
   
   /* AccOFF中、Alarm時刻一致した場合、Alarm調整に入った場合 */
   /* 62.5msが動いてない、而もOSC1(12M)発振しているので      */
   /* ここでAccONのチャタリングを行う */
   Pw_accdet_ONchat();
}
/******************************************************************************
   関数名      ：Pw_5ms_timer
   引数      ：無し
   戻り値      ：無し
   機能概略   ：Power Module 5ms ｲﾝﾀｰﾊﾞﾙ処理
   コメント   ：ill_det検出用
------------------------------------------------------------------------------
修正履歴
[chin20120816]新規作成
******************************************************************************/
void   Pw_5ms_timer(void)
{
   if (Pw_Power_status_get() == ON)
   {
      if ((Ma_model_dest() >= CMA_MODEL_AMFM_J)&&(Ma_model_dest() <= CMA_MODEL_AMFM_X))
      {
         Pw_illdet_chat();
      }
   }
}

/******************************************************************************
   関　数　名      ：Pw_10ms_timer
   引　　　数      ：無し
   戻　り　値      ：無し
   機　能　概　略   ：●Power Module 10ms ｲﾝﾀｰﾊﾞﾙ処理
   コ　メ　ン　ト   ：AccON時メインの10msカウント処理よりCallされる。
               ：AccOFF時RC発振の10ms割込み処理よりCallされる。
------------------------------------------------------------------------------
修正履歴
[chin20120816]新規作成
******************************************************************************/
void   Pw_10ms_timer(void)
{
   Pw_almkey_chat();
}

void   Pw_4ms_timer(void)   // Duoc su dung trong file io_int.c, duoc goi boi basetimer. Xem file io_int.c
{
   if (++pw_AccOn_4mscnt == CPW_ACCON_4MSCNT)  // CPW_ACCON_4MSCNT = 16
   {
      pw_AccOn_4mscnt = 0;
      Pw_62ms_cycle();
   }
}
/******************************************************************************
   関数名   ：Pw_62ms_cycle
   引  数   ：無し
   戻り値   ：無し
   機  能   ：Haltモード、電源62.5ms周期監視処理
         ：DCP Port、ILL_DET,のOnﾁｬﾀﾘﾝｸﾞ処理
         ：1ms禁止中ﾁｬﾀﾘﾝｸﾞを行う
------------------------------------------------------------------------------
修正履歴
【番号】   日付      担当者      修正内容
【NKC-0000】'05/12/09   仲地      新規作成
[sun080715] HALT中Powerｷｰ反応遅い問題対策、HALT中62.5ms毎にAD　KEYﾁｪｯｸするように変更
******************************************************************************/
void   Pw_62ms_cycle(void)
{
   #if 0 /* 使用しないため、削除 */
   if (pw_accwait_timer > TIMER_OUT)
   {
      pw_accwait_timer --;
   }
   #endif
   
   Pw_accdet_ONchat();
}
/******************************************************************************
   関　数　名      ：Pw_ACC_status_get
   引　　　数      ：無し
   戻　り　値      ：ACC Port 状態
               　ON:ACC on確定状態／OFF:ACC off確定状態
   機　能　概　略   ：ACC Port 状態取得関数。
               　ACCﾎﾟｰﾄを監視しHi/Lowﾁｬﾀﾘﾝｸﾞ後の状態を返す。
   コ　メ　ン　ト   ：10ms割込み処理よりCallされる。
------------------------------------------------------------------------------
修正履歴
【番号】   日付      担当者      修正内容
【NKC-0000】'05/12/09   仲地      新規作成
******************************************************************************/
BYTE   Pw_ACC_status_get(void)
{
   #if 1
   if (( pw_curmode == CPW_PWRON )||( pw_curmode == CPW_PWROFF ))
   #else
   if (fpw_acc == ON)
   #endif
   {
      return(ON);
   }
   else
   {
      return(OFF);
   }
}

BYTE   Pw_ACC_DET_get(void)
{
   if (fpw_acc == ON)  // trong file pw_main.h
   {
      return(ON);
   }
   else
   {
      return(OFF);
   }
   
}
/******************************************************************************
   関　数　名      ：Pw_Power_OnOff_get
   引　　　数      ：無し
   戻　り　値      ：Power ON/OFF 状態
               　ON:panel on確定状態／OFF:Panel off確定状態
   機　能　概　略   ：Power ON/OFF 状態取得関数。
------------------------------------------------------------------------------
修正履歴
【番号】   日付      担当者      修正内容
【NKC-0000】'05/12/09   仲地      新規作成
******************************************************************************/
BYTE   Pw_Power_status_get(void)
{
   if (pw_curmode == CPW_PWRON)
   {
      return(ON);
   }
   else
   {
      return(OFF);
   }
}

BYTE   Pw_Power_Key_get(void)
{
   if (fpw_power_key == ON)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}
BYTE   Pw_PowerOff_disp_chk(void)
{
   #if 1
   if ((pw_tarmode == CPW_PWRON)
     &&(pw_curmode == CPW_PWRON))
   #else
   if (pw_tarmode == CPW_PWRON)
   #endif
   {
      return FALSE;
   }
   else
   {
      return TRUE;
   }
}
/******************************************************************************
   関数名   ：Pw_power_key
   引  数   ：無し
   戻り値   ：無し
   機  能   ：Powerｷｰ処理
------------------------------------------------------------------------------
[sun070326] HALT要求あるときもPowerKeyがくる場合はあるため、HALT要求あり
         中ｷｰ無効に変更
[sun070622] E向けPowerOFF HALTへ遷移するため、PowerｷｰはBATT/ACC/DCP ON中
         有効になるように変更
******************************************************************************/
void Pw_power_key(void)      // duoc dung key nhan key PW(Xem key module.)
{
   fpw_power_key = ~fpw_power_key;            /* PowerKeyﾌﾗｸﾞ逆転 */
   if (fpw_power_key == ON)
   {
      Lcd_disp_base(Ma_current_mode());
   }
   Key_check_stop();
   Key_nop();
}

/******************************************************************************
   関数名   ：Pw_Power_event
   引  数   ：mode ⇒ ｲﾍﾞﾝﾄ発生したﾒｲﾝﾓｰﾄﾞ
         ：status ⇒ ｲﾍﾞﾝﾄ内容
         ：      ON⇒PowerON要求
         ：      OFF⇒PowerOFF要求
   戻り値   ：無し
   機  能   ：Powerｲﾍﾞﾝﾄ受付処理
------------------------------------------------------------------------------
[sun060327]空関数作成
[sun060725]DM関係削除
[sun100828]TestModeMessageでPowerOn/Off切り替え処理追加
******************************************************************************/
void Pw_power_event(BYTE mode, BYTE status)
{
   #if _TEST_MODE_
   if (mode == 0xff)
   {
      if (status == ON)
      {
         fpw_power_key = ON;
      }
      else
      {
         fpw_power_key = OFF;
      }
   }
   else 
   #endif
   if (status == ON)
   {
      fpw_power_key = ON;
   }
   else
   {
      ;
   }
}

void   Pw_acc_off_nor_req(BYTE mode)
{
   if (mode != CPW_HOLDX_INTOFF)
   {
      if (pw_curmode == CPW_HOLDX)
      {
         #if 1
         /* RC発振 */
         Io_sfr_holdx_out();
         
         /* SIOポート設定 */
         Io_port_holdx_out();
         
         /* 62.5msタイマー割り込み許可 */
         /* 時計の500ms割り込み禁止 */
         /* 10msタイマ割り込み許可 */
         /* ACC_DET割り込み禁止 */
         /* ALARMキー検出割り込み禁止 */
         Io_int_holdx_out();
         
         #else
         /* 時計タイマ500ms割り込み禁止 */
         Io_int_disable_clktim();
         
         /* alarmKey割り込み禁止 */
         Io_int_disable_almkey();
         
         /* AccDET割り込み禁止 */
         Io_int_disable_accdet();
         #endif
         
         fpw_acc_nor = ON;
         
         switch(mode)
         {
            case CPW_HOLDX_INT_ACC:
               fpw_accdet_active = ON;
               pw_AccOn_4mscnt = 0;
               pw_accon_chat_cnt = 0;
               pw_accoff_chat_cnt = 0;
               break;
            case CPW_HOLDX_INT_ALM:
               fpw_almdet_active = ON;
               break;
            case CPW_HOLDX_INT_CKL:
            default:
               break;
         }
         
      }
   }
   else
   {
      fpw_acc_nor = OFF;
   }
   return;
}
static void   Pw_accdet_ONchat( void )
{
   BYTE   acc_now;
   BYTE   cnt;
   
   acc_now = IO_In_acc_det();
   
   if (acc_now == LOW)
   {
      pw_accon_chat_cnt ++;
      if (pw_accon_chat_cnt >=3)
      {
         pw_accon_chat_cnt = 3;
         if (fpw_acc == OFF)
         {
            fpw_acc = ON;
            Alm_mode_set();
         }
         fpw_accdet_active = OFF;
      }
   }
   else
   {
      pw_accon_chat_cnt = 0;
      fpw_accdet_active = OFF;
   }
   
}
void   Pw_accdet_OFFchat( void )
{
   BYTE   acc_now;
   BYTE   cnt;
   
   acc_now = IO_In_acc_det();
   
   if (acc_now == HIGH)
   {
      pw_accoff_chat_cnt ++;
      if (pw_accoff_chat_cnt >=5)
      {
         pw_accoff_chat_cnt = 5;
         if (fpw_acc == ON)
         {
            fpw_acc = OFF;
            Lcd_accoff_send_req();
            Alm_mode_set();
         }
      }
   }
   else
   {
      pw_accoff_chat_cnt = 0;
   }
}

/******************************************************************************
   関数名   ：Pw_ret_illdet
   引  数   ：無し
   戻り値   ：無し
   機  能   ：ILLDETﾎﾟｰﾄ状態通知処理
------------------------------------------------------------------------------
[chin20120816]新規作成
******************************************************************************/
BOOL   Pw_ret_illdet(void)
{
   if (Ma_model_dest() == CMA_MODEL_AM_J )
   {
      return HIGH;
   }
   else
   {
      return(fpw_ill_det);
   }
}

/******************************************************************************
   関数名   ：Pw_illdet_chat
   引  数   ：無し
   戻り値   ：無し
   機  能   ：ILL DET端子検出処理
         ：ILL DETﾎﾟｰﾄの値を読み出し,LOW Active
         ：powerON中に5ms毎に呼ばれて、2回一致でILL DET状態確定
         ：TunerのSEEK用
------------------------------------------------------------------------------
修正履歴
[chin20120816] 新規作成
******************************************************************************/
static void   Pw_illdet_chat(void)
{
   BYTE   illdet_cnt;
   
   pw_illdet_cnt.byte <<= 1;                     /* ﾁｬﾀﾘﾝｸﾞｶｳﾝﾀｼﾌﾄ */

   if (MIO_IN_pILL_DET() == HIGH)
   {
      fpw_illdet_now = ON;                  /* ﾁｬﾀﾘﾝｸﾞｶｳﾝﾀbit0 ｸﾘｱ */
   }
   else
   {
      fpw_illdet_now = OFF;                  /* ﾁｬﾀﾘﾝｸﾞｶｳﾝﾀbit0 ｾｯﾄ */
   }

   illdet_cnt = (pw_illdet_cnt.byte & 0x03);      /* 5ms×2回一致ﾁｪｯｸ */
   
   if (illdet_cnt == BIN_00000011)
   {
      fpw_ill_det = HIGH;
   }
   else if (illdet_cnt == BIN_00000000)
   {
      fpw_ill_det = LOW;
   }
   else
   {
      ;
   }
}

/******************************************************************************
   関数名   ：Pw_ret_almkey
   引  数   ：無し
   戻り値   ：無し
   機  能   ：almkeyﾎﾟｰﾄ状態通知処理
------------------------------------------------------------------------------
[chin20120816]新規作成
******************************************************************************/
BOOL   Pw_ret_almkey(void)
{
   if (Ma_get_alarm() == FALSE)
   {
      return FALSE;
   }
   else
   {
      return(fpw_alm_key);
   }
}

/******************************************************************************
   関数名   ：Pw_almkey_chat
   引  数   ：無し
   戻り値   ：無し
   機  能   ：ALARM_KEY端子検出処理                             ALARM_KEY terminal detection processing
         ：ALARM_KEYﾎﾟｰﾄの値を読み出し,LOW Active             Read the value of ALARM_KEY port, LOW Active
         ：62.5ms毎に呼ばれて、3回一致でALARM_KEY状態確定
------------------------------------------------------------------------------
[chin20120816]新規作成
******************************************************************************/
static void   Pw_almkey_chat(void)
{
   BYTE   almkey_now;
   
   almkey_now = MIO_IN_pALARM_KEY();
   
   #if 1
   if (almkey_now  != fpw_alm_key_bak)            /* ﾎﾟｰﾄ変化あり */
   {
      pw_almkey_cnt = 0;
   }
   else
   {
      pw_almkey_cnt ++;
      if (pw_almkey_cnt >= 2)
      {
         pw_almkey_cnt = 2;
         fpw_alm_key = ~fpw_alm_key_bak;
         if (fpw_alm_key == OFF)
         {
            fpw_almdet_active = OFF;
         }
      }
   }
   fpw_alm_key_bak = almkey_now;
   #endif
}

void   Pw_almkey_clear(void)
{
   fpw_alm_key = OFF;
}
/******************************************************************************
   関数名   ：Pw_acc_wakeup_req
   引  数   ：ON ⇒ AccOFF中の通常モードに遷移
         ：OFF⇒ AccOFF中のHoldXモードに遷移
   戻り値   ：無し
   機  能   ：HoldX中に通常モードに遷移要求を設定処理
------------------------------------------------------------------------------
[chin20120725]新規作成
******************************************************************************/
void   Pw_acc_wakeup_req(BYTE onoff)
{
   if (onoff == OFF)
   {
      fpw_wakeup = OFF;
   }
   else
   {
      fpw_wakeup = ON;
   }
   return;
}

/******************************************************************************
   関数名   ：Pw_test_func
   引  数   ：RcvCmd：コマンド
         ：RcvBuf：パラメータのアドレス
         ：Length：パラメータのレングス
   戻り値   ：無し
   機  能   ：コマンド受付処理
------------------------------------------------------------------------------
[chin20120928]新規作成
******************************************************************************/
void   Pw_test_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length)
{
   if (RcvCmd == CTEST_MODE_OTHER_STBY_ONOFF )
   {
      if (RcvBuf[0] == ON )
      {
         MIO_OUT_pSTBY(HIGH);
      }
      else
      {
         MIO_OUT_pSTBY(LOW);
      }
   }
}

#if _POWER_DIAG_
/******************************************************************************
   関数名   ：Pw_DIAG_check
   引  数   ：mode→検出タイプ
   戻り値   ：無し
   機  能   ：PowerDiag検出処理
------------------------------------------------------------------------------
[chin20121015]新規作成
******************************************************************************/
void Pw_DIAG_check(BYTE mode)
{
   if ((pw_diag_cnt < CPW_DIAG_CNT_ON) &&         /* DIAG確定中ではない */
      (pw_curmode == CPW_PWRON))               /* Power ON中 */
   {
      if (MIO_IN_pDIAG() == HIGH)
      {
         pw_diag_cnt = 0;
      }
      else if (mode == CPW_DIAG_MD_1MS)
      {
         pw_diag_cnt ++;
      }
      else
      {
         ;
      }
      
      if (pw_diag_cnt == CPW_DIAG_CNT_ON)
      {
         /* Power off */
         fpw_power_key = OFF;
         MIO_OUT_pSTBY(LOW);
         fpw_wakeup = FALSE;
      }
   }
}

void Pw_DIAG_clear( void )
{
   pw_diag_cnt = 0;
}
#endif   /* _POWER_DIAG_ */


#undef  _PW_FUNC_C_
