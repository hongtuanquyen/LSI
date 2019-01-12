/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

      プロジェクト名   :
      ファイル名      : main.c
      機能         : Systemﾒｲﾝﾙｰﾌﾟ処理
------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]   新規作成
******************************************************************************/
#define   _MA_EXT_C_

#include   <string.h>
#include   "../model.h"
#include   "../comm/common.h"
#include   "../io/IO_inc.h"
#if (_MODEL_EEPROM_ == YES)
#include   "../power/pw_main.h"
#include   "../eeprom/eeprom_ext.h"
#endif
#if _TEST_MODE_
#include   "../power/pw_func_ext.h"
#include   "../testmode/TEST_inc.h"
#if _MODEL_RADIO_
#include   "../tuner/tu_ext.h"
#endif
#endif
#include   "main.h"
#include   "ma_ext.h"
#include   "ma_mode.h"
#include   "ma_tim.h"


#if _TEST_MODE_
/******************************************************************************
..       関数名  ：Ma_Test_Func
      引  数   ：RcvCmd ⇒ Testｺﾏﾝﾄﾞ
            ：RcvBuf ⇒ ｺﾏﾝﾄﾞﾊﾟﾗﾒｰﾀ格納ﾊﾞｯﾌｧｱﾄﾞﾚｽ
            ：Length ⇒ ﾊﾟﾗﾒｰﾀﾃﾞｰﾀ長
      戻り値   ：なし
      機  能   ：入れたMainモードはTunerﾓｰﾄﾞであるかどうか通知処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun070822] 新規作成
[sun100929] PowerOff中ﾓｰﾄﾞ遷移要求受け付けないように変更
******************************************************************************/
void   Ma_Test_Func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Lenth)
{
   switch(RcvCmd)
   {
      case   CTEST_MODE_POWERON:
         if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_OK)||
            (Ma_Chk_EEP_Initial() == CMA_INIT_STA_CKAL_NG))
         {
            Pw_power_event(0xff,ON);
         }
         break;
      case   CTEST_MODE_POWEROFF:
         Pw_power_event(0xff,OFF);
         break;
      case   CTEST_MODE_SRC:
         if (Pw_Power_status_get() == OFF)
         {
            return;
         }
         switch(*RcvBuf)
         {
            #if _MODEL_RADIO_
            /* EXT1 = 07  EXT2 = 11 ⇒ AM  EXT2 = 01 ⇒FM */
            case CTEST_MODE_SRC_TUNER_EXTR1:
               if (*(RcvBuf+1) == CTEST_MODE_SRC_TUNER_AM_EXTR2)
               {
                   Tu_bnd_set(0);
                  Ma_mode_event(CMA_ID_TU, CMA_TUNER, CMA_MODEON);
               }
               else if (*(RcvBuf+1) == CTEST_MODE_SRC_TUNER_FM_EXTR2)
               {
                   Tu_bnd_set(1);
                  Ma_mode_event(CMA_ID_TU, CMA_TUNER, CMA_MODEON);
               }
               else
               {
                  /*処理無*/
               }
               break;
            #endif
            case   CTEST_MODE_SRC_AUX_EXTR1:
               if (( Ma_model_dest() >= CMA_MODEL_AMFM_J )
                 &&( Ma_model_dest() <= CMA_MODEL_AMFM_X ))
               {
                  Ma_mode_event(CMA_ID_AUX1, CMA_AUX, CMA_MODEON);
               }
               break;
            default:
               break;
         }
         break;
      default:
         break;
   }
}
#endif

/******************************************************************************
..       関数名  ：Ma_set_Msg
      引  数   ：mode⇒ﾒｯｾｰｼﾞ発行元ﾓｼﾞｭｰﾙID
            ：Msg⇒発行予定ﾒｯｾｰｼﾞID
      戻り値   ：無し
      機  能   ：ﾒｯｾｰｼﾞ登録処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051207] 新規作成
******************************************************************************/
void   Ma_set_Msg(BYTE mode,BYTE Msg)
{
   Ma_message_set(mode,Msg);
}
/******************************************************************************
..       関数名  ：Ma_key_src
      引  数   ：無し
      戻り値   ：無し
      機  能   ：source ｷｰ処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051215] 新規作成
[sun060725] DM関係処理コメント
[sun070704] MODE切替最中にｿｰｽｷｰ無効に変更
******************************************************************************/
void Ma_key_src(void)
{
   if (fma_mdchg == OFF)      /* [sun070704] */
   {
      Ma_source_chg();
   }
}
/******************************************************************************
..       関数名  ：Ma_mode_event
      引  数   ：mode_id ⇒ 制御ﾊﾞｯﾌｧID
            ：mode ⇒ ﾒｲﾝﾓｰﾄﾞ値
            ：
      戻り値   ：無し
      機  能   ：ﾓｼﾞｭｰﾙOn/OFF通知処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060118] 新規作成
******************************************************************************/
void Ma_mode_event(BYTE mode_id, BYTE mode, BYTE event)
{
   if ((mode_id > CMA_ID_MAX) ||
      (mode > CMA_MODE_MAX) ||      // CMA_MODE_MAX = CMA_ISR = 3. CMA_TUNER = 1, CMA_AUX = 2. 
      (event >CMA_EVENT_MAX ))
   {
      return;               /* 指定不正 */
   }
   Ma_mode_event_chk(mode_id, mode, event);
}
/******************************************************************************
..       関数名  ：Ma_target_mode
      引  数   ：無し
      戻り値   ：現在のMainMode
      機  能   ：ﾀｰｹﾞｯﾄﾒｲﾝﾓｰﾄﾞ通知処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060605] 新規作成
******************************************************************************/
BYTE  Ma_target_mode(void)
{
   return(ma_mode[CMA_TARGET_MD].cond.mode);
}
/******************************************************************************
..       関数名  ：Ma_current_mode
      引  数   ：無し
      戻り値   ：現在のMainMode
      機  能   ：ｶﾚﾝﾄﾒｲﾝﾓｰﾄﾞ通知処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051207] 新規作成
******************************************************************************/
BYTE  Ma_current_mode(void)
{
   return(ma_mode[CMA_CUR_MD].cond.mode);  // tat ca trong file ma_mode.h. CMA_CUR_MD = 1;
}
/******************************************************************************
..       関数名  ：Ma_model_dest
      引  数   ：なし
      戻り値   ：ModelType：
            ：CMA_MODEL_U ⇒ USA向け
            ：CMA_MODEL_T ⇒ K向け
            ：CMA_MODEL_J ⇒ Japan向け
            ：CMA_MODEL_E ⇒ 欧州向け
      機  能   ：ModelType通知処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128] 新規作成
******************************************************************************/
BYTE    Ma_model_dest(void)
{
   return( ma_dest );
}


/******************************************************************************
..       関数名  ：Ma_ini_data_set
      引  数   ：data→EEPROMの初期値バッファアドレス
      戻り値   ：無し
      機  能   ：初期値を格納処理
------------------------------------------------------------------------------
[chin120918] 新規作成
******************************************************************************/
void   Ma_ini_data_set(BYTE *data)
{
   memcpy((BYTE*)&ma_ini_data,data,5);            /* FactoryCode1、仕向け、時計、アラーム */
   memcpy(&ma_ini_data.fac2[0],(data+10),2);      /* FactoryCode2 */
   memcpy(ma_part_num,data+5,sizeof(ma_part_num));   /* 部品番号 */
   ma_ini_data.ini_status = CMA_INIT_STA_ALL_NG;   /* 初期値失敗にしておく */
}

/******************************************************************************
..       関数名  ：Ma_part_num_get
      引  数   ：n_buf→部品番号格納バッファアドレス
      戻り値   ：無し
      機  能   ：部品番号を渡す
------------------------------------------------------------------------------
[chin120918] 新規作成
******************************************************************************/
void   Ma_part_num_get(BYTE * n_buf)
{
   BYTE i;
   BYTE te;
   
   for (i = 0;i<sizeof(ma_part_num);i++)
   {
      te = ma_part_num[i];
      n_buf[i*2] = (BYTE)((te>>4)&0x0F);
      n_buf[i*2+1] = (BYTE)(te&0x0F);
   }
}

/******************************************************************************
..       関数名  ：Ma_get_clock
      引  数   ：無し
      戻り値   ：TRUE→時計有
              FALSE→時計無
      機  能   ：時計有無しの結果を通知する
------------------------------------------------------------------------------
[chin120918] 新規作成
******************************************************************************/
BYTE    Ma_get_clock(void)
{
   if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_NG)||
      (Ma_Chk_EEP_Initial() == CMA_INIT_STA_AREA_NG))
   {
      return FALSE;
   }
   return( ma_ini_data.clock );
}

/******************************************************************************
..       関数名  ：Ma_get_alarm
      引  数   ：無し
      戻り値   ：TRUE→アラーム有
              FALSE→アラーム無
      機  能   ：アラーム有無しの結果を通知する
------------------------------------------------------------------------------
[chin120918] 新規作成
******************************************************************************/
BYTE    Ma_get_alarm(void)
{
   if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_NG)||
      (Ma_Chk_EEP_Initial() == CMA_INIT_STA_AREA_NG))
   {
      return FALSE;
   }
   
   #if 0
   if (ma_ini_data.clock == TRUE)
   {
      return ( ma_ini_data.alarm );
   }
   #else
   if ((ma_dest >= 2)&&
       (ma_dest <= 7)&&
       (ma_ini_data.clock == 0x01)&&
       (ma_ini_data.alarm == 0x01))
   {
      return( TRUE );
   }
   #endif
   else
   {
      return( FALSE );
   }
}

BYTE Ma_is_mode_chg(void)
{
   if(( fma_mdchg == ON ) || ( fma_mute == ON ))
   {
      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}
/******************************************************************************
..       関数名  ：Ma_get_alarm
      引  数   ：無し
      戻り値   ：CMA_INIT_STA_ALL_NG →初期値NG
              CMA_INIT_STA_AREA_NG→仕向けNG
              CMA_INIT_STA_CKAL_NG→時計、アラームNG
              CMA_INIT_STA_ALL_OK →初期値OK
      機  能   ：初期値のチェック結果を通知する
------------------------------------------------------------------------------
[chin120918] 新規作成
******************************************************************************/
BYTE    Ma_Chk_EEP_Initial(void) 
{
   #if 1
   return ma_ini_data.ini_status; // ma_ini_data_ini khai bao trong ma_ext.h, duoc khoi tao luc thuc hien message CMSG_MAIN trong ma_sys.c
   #else
   if (ma_ini_data.ini_status == CMA_INIT_STA_ALL_OK)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
   #endif
}

#if 0
/* _MODEL_EEPROM_ */
void Ma_SaveEepData(BYTE *BufAddr)
{
   *BufAddr++ = fpw_power_key;
   *BufAddr++ = ma_mode[CMA_TARGET_MD].id;
   *BufAddr++ = ma_mode[CMA_TARGET_MD].cond.mode;
   *BufAddr++ = ma_mode[CMA_LAST_MD].id;
   *BufAddr++ = ma_mode[CMA_LAST_MD].cond.mode;
}
/* ******************************************************************************
    関数名  ：Ma_eeprom_datachk
   引  数   ：data ⇒ EEPROMﾃﾞｰﾀ
         ：status ⇒ ﾃﾞｰﾀ種類
   戻り値   ：OK  ⇒ ﾃﾞｰﾀ正常
         ：NG  ⇒ ﾃﾞｰﾀ異常
   機  能   ：EEPROMに格納してあるﾃﾞｰﾀ範囲ﾁｪｯｸ処理
   修正履歴   【年月日】【名前】 【説明】
------------------------------------------------------------------------------
[chin090326]新規作成
[sun090403]割込みﾓｰﾄﾞEEPROMへ記憶しない
**********************************************************************************/
BYTE   Ma_eeprom_datachk(WORD      data,   BYTE   eep_id)
{
   BYTE   iRet;
   iRet = FALSE;
   if (eep_id == CEEP_MN_LSTMODE)
   {
      if ((data == 0x0001)||(data == 0x0102)||(data == 0x0304))
      {
         iRet = TRUE;
      }
   }
   return iRet;
}
/*******************************************************************************
    関数名  ：Ma_eeprom_initial
   引  数   ：なし
   戻り値   ：なし
   機  能   ：EEPROMに格納してあるﾃﾞｰﾀ初期値書込み処理
         flcd_dimmer   :
         flcd_scroll   :
         flcd_ssp   :
   修正履歴   【年月日】【名前】 【説明】
------------------------------------------------------------------------------
[chin090326]新規作成
**********************************************************************************/
void Ma_eeprom_initial(void)
{
   WORD lstmode;
   lstmode = (WORD)CMA_ID_TU;
   lstmode = lstmode << 8;
   lstmode |= (WORD)CMA_TUNER;
   eeprom_data_writereq(CEEP_MN_LSTMODE,lstmode);
}

/******************************************************************************
..       関数名  ：Ma_mode_eeprom_set
      引  数   ：なし
      戻り値   ：無し
      機  能   ：ACC/DCP/BATT　OFFでMainﾓｰﾄﾞをEEPROMへ格納処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun090411] Ma_mode_pwroffから移動
******************************************************************************/
void   Ma_eeprom_data_set(void)
{
   WORD   mode;
   
   mode = (WORD)ma_mode[CMA_TARGET_MD].id;
   mode <<= 8;
   mode |= (WORD)ma_mode[CMA_TARGET_MD].cond.mode;
   eeprom_data_writereq(CEEP_MN_LSTMODE,mode);
}

#endif

#undef   _MA_EXT_C_
