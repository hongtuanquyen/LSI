#include   "../model.h"

#if   _MODEL_RADIO_

#define      _TU_ITEM_C_

#include      "../comm/common.h"
#include      "../audio/AUD_inc.h"
#include      "../main/MA_inc.h"
#include      "../disp/LCD_inc.h"
#include      "../submode/sbm_ext.h"

#include   "tu_def.h"
#include   "tu_main.h"
#include   "tu_ext.h"
#include   "tu_item.h"
#include   "tu_conttbl.h"
#include   "tu_io.h"
#include   "tu_test.h"
#if      (_MODEL_EEPROM_ == 1)   /*   EEPROMから初期パラメータを取得対応、100811、岳   */
#include   "tu_eeprom.h"
#endif
#include   "tu_event.h"

/************************************************************************/
/* 関数名 :void   Tu_main_process(void)                           */
/* --------------------------------------------------------------------   */
/* 機能   :   Tuner動作ｺﾝﾄﾛｰﾙ処理                                 */
/* 詳細   :                                                */
/************************************************************************/
void   Tu_main_process(void)
{
   const   TTU_CONTTBL      *m_mode;
   const   TTU_CONTTBL      *seq_tbl;
   BYTE   ret;
    BYTE    (*func)(void) ;

   m_mode = (const   TTU_CONTTBL*)TU_SeqTBL[tu_m_mode -1];

   seq_tbl = (const   TTU_CONTTBL*)(m_mode+tu_m_seq);
   func = (seq_tbl->func);
   if (func == 0)
   {
      return;
   }
   ret = (*func)();
   if (ret == CTU_I_OKEND)
   {
      if (seq_tbl->ok_seq == CTU_NEXT_M)      /* 次のﾒｲﾝ処理ｽﾀｰﾄ */
      {
         tu_m_seq = 0x00;               /* ﾒｲﾝｼｰｹﾝｽｸﾘｱ */
         Tu_I_initial();                  /* Item処理初期化 */
      }
      else if ((seq_tbl->ok_seq == CTU_OKEND) ||
           (seq_tbl->ok_seq == CTU_NGEND))
      {
         Tu_job_clear();                  /* 処理完了 */
      }
      else
      {
         tu_m_seq = seq_tbl->ok_seq;         /* 次のItemへｾｯﾄ */
         Tu_I_initial();                  /* Item処理初期化 */
      }
   }
   else if (ret == CTU_I_NGEND)
   {
      if(seq_tbl->ng_seq == CTU_NEXT_M)
      {
         tu_m_seq = 0x00;               /* ﾒｲﾝｼｰｹﾝｽｸﾘｱ */
         Tu_I_initial();                  /* Item処理初期化 */
      }
      else if((seq_tbl->ng_seq == CTU_OKEND) ||
           (seq_tbl->ng_seq == CTU_NGEND))
      {
         Tu_job_clear();                  /* 処理完了 */
      }
      else
      {
         tu_m_seq = seq_tbl->ng_seq;         /* 次のItemへｾｯﾄ */
         Tu_I_initial();                  /* Item処理初期化 */
      }
   }
   else
   {
   }
}

/************************************************************************/
/* 関数名 :void   Tu_I_initial(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   Item処理用変数初期化処理                           */
/* 詳細   :                                                */
/************************************************************************/
void   Tu_I_initial(void)
{
   tu_i_seq = 0;
   tu_5ms_timer = 0;
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_premute(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER PreMute Wait処理                              */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_premute(void)
{
   switch(tu_i_seq)
   {
      case   0:
         if((tu_m_mode == CTU_M_SEEK)
         || (tu_m_mode == CTU_M_AS))
         {
            tu_5ms_timer = 30/5;   /* キー長押しbeep音待つ */
         }
         else
         {
            tu_5ms_timer = 0;
         }
         tu_i_seq ++;
         break;
      case   1:
         if (tu_5ms_timer == TIMER_OUT)
         {
            Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);   /* TunerMute要求ON */
            if(Aud_get_mute_status() != CAUD_MUTE_ON_END)
            {
               if((tu_m_mode == CTU_M_SEEK)
               || (tu_m_mode == CTU_M_AS)
               || (tu_m_mode == CTU_M_SCAN))
               {
                  tu_5ms_timer = CTU_SEEKMUTE;   /* Front Mute timer set */
               }
               else
               {
                  tu_5ms_timer = CTU_PREMUTE;      /* Front Mute timer set */
               }
            }
            else
            {
               tu_5ms_timer = 0;
            }
            ftu_mute = ON;                        /* Mute Flagｾｯﾄ */
            tu_i_seq ++;
         }
         break;
      case   2:
         if (tu_5ms_timer == TIMER_OUT)
         {
            tu_i_seq = 0;               /* Item Seq Clear */
            return(CTU_I_OKEND);            /* PreMuteOK */
         }
         break;
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);                     /* 処理継続いったんﾒｲﾝに戻る */
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_icreset(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER IC RESET処理                                 */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_icreset(void)
{
   switch(tu_i_seq)
   {
      case   0:
         Tu_lastcall();
         tu_i_seq ++;
         break;
      case   1:
         tu_i_seq ++;
         tu_5ms_timer = 10/5;
         Tu_frq_send_set();
         break;
      case   2:
#if 0
      if(tu_5ms_timer == TIMER_OUT )
         {
            tu_ic_data.txbuf[15] = 0x14;
            #if 0
            tu_ic_data.msa.subaddr = 0x1F;      /*   課題管理表8番問題回答より、0x0F⇒0x1Fに変更、100816、岳   */
            #else
            tu_ic_data.msa.subaddr = 0x0F;      /* OIL#54対応 */
            #endif
            tu_ic_data.length = 1;
            tu_i_seq ++;
            Tu_data_write();
         }
         break;
      case   3:
#endif
      return(CTU_I_OKEND);
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_aftermute(void)                           */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER AfterMute設定処理                              */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_aftermute(void)
{
   #if 0   /* audioモジュール250ms待ってるので、削除 */
   Tu_aftermute(CTU_AFTER250);      /* AfterMute 250ms */
   #else
   Tu_aftermute(0);      /* AfterMute 250ms */
   #endif
   return(CTU_I_OKEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_frqset(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER 周波数設定処理                                 */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_frqset(void)
{
   switch(tu_i_seq)
   {
      case   0:
         tu_i_seq ++;
         tu_5ms_timer = 10/5;
         Tu_frq_send_set();
         break;
      case   1:
         if(tu_5ms_timer == TIMER_OUT )
         {
            tu_i_seq ++;
            Tu_end_send_set();
         }
         break;
      case   2:
         return(CTU_I_OKEND);
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}


/************************************************************************/
/* 関数名 :BYTE   Tu_I_bndpara(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER バンドパラメータ設定処理                        */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_bndpara(void)
{
   switch(tu_i_seq)
   {
      case   0:
         tu_i_seq ++;
         tu_5ms_timer = 10/5;
         Tu_para_send_set();
         break;
      case   1:
         if(tu_5ms_timer == TIMER_OUT )
         {
            tu_i_seq ++;
            tu_5ms_timer = 10/5;
            Tu_frq_send_set();
         }
         break;
      case   2:
#if 0
      if(tu_5ms_timer == TIMER_OUT )
         {
            tu_ic_data.txbuf[15] = 0x00;
            #if 0
            tu_ic_data.msa.subaddr = 0x1F;      /*   課題管理表8番問題回答より、0x0F⇒0x1Fに変更、100816、岳   */
            #else
            tu_ic_data.msa.subaddr = 0x0F;      /* OIL#54対応 */
            #endif
            tu_ic_data.length = 1;
            tu_i_seq ++;
            Tu_data_write();
         }
         break;
      case   3:
#endif
         Tu_set_prechno();
         return(CTU_I_OKEND);
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_pstwait(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER プリセット書き込み待ち処理                     */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_pstwait(void)
{
   switch (tu_i_seq)
   {
      case   0:
         tu_100ms_timer = CTU_PSTWAIT_TIM;            /* プリセット書き込み待ち */
         tu_i_seq++;
         break;
      case   1:
         if(ftu_write == ON)
         {
            Tu_pno_set(tu_wrtpno);
            tu_wrtpno = Tu_pch_updw(tu_wrtpno,CTU_UP);
            return(CTU_I_OKEND);
         }
         if(tu_100ms_timer == TIMER_OUT)
         {
            Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);      /* すべて時限表示ｷｬﾝｾﾙ */
            Lcd_disp_base(CMA_TUNER);
            return(CTU_I_NGEND);
         }
         break;
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_pstwrite(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER プリセット書き込み処理                        */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_pstwrite(void)
{
   #if 0   /* 仕様が無いため、削除 */
   Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);   /* [070525] MUTE処理修正 TunerMute要求ON */
   ftu_mute = ON;                        /* [070525] MUTE処理修正 Mute Flagｾｯﾄ */
   Tu_aftermute(CTU_AFTER500);
   #endif
   Tu_lastwrite();
   Tu_stdis_timset(CTU_ST_PLLCHG);      /*500msST検出禁止 */
   #if   (_MODEL_EEPROM_ == 1)
   tu_eepdata[0]   = (BYTE)(tu_ch.freq & 0xFF);
   tu_eepdata[1]   = (BYTE)((tu_ch.freq >> 8) & 0xFF);
   #if 0
   tu_eepreq.EepAddr   = (CTU_EEPOFS_PREMEM + (tu_bandno * 12) + (tu_pno -1) * 2);
   #else
   tu_eepreq.EepAddr   = (CTU_EEPOFS_PREMEM + (tu_bandno * 16) + (tu_pno -1) * 2);
   #endif
   tu_eepreq.Length   = 2;
   tu_eepreq.BufAddr   = tu_eepdata;
   ftu_weepreq         = ON;
   tu_eep_cnt         = 0;
   #endif
   return(CTU_I_OKEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_updw(void)                                 */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER 周波数アップダウン処理                        */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_updw(void)
{
#if   (CTU_E_TEST == 0x01)
   Tu_ch_updw();
   if((Tu_band_chk() == CTU_FM)&&( tu_m_mode == CTU_M_SEEK ))
   {
      Tu_ch_updw();
   }
   return(CTU_I_OKEND);
#else
   Tu_ch_updw();
   if ((tu_m_mode == CTU_M_MAN))
   {
      if(Ma_current_mode() == CMA_ISR)
      {
         Lcd_disp_base(CMA_ISR);
      }
      else
      {
         Lcd_disp_base(CMA_TUNER);
      }
   }
#if 0
   iRet = Tu_updw_typechk();
   if (iRet == CTU_UPDW_NOT)
   {
      Tu_ch_updw();
      iRet = Tu_updw_typechk();
   }
   if (iRet == CTU_UPDW_END)
   {
      return(CTU_I_NGEND);
   }
   else
   {
      return(CTU_I_OKEND);
   }
#else
   return(CTU_I_OKEND);
#endif
#endif
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_man(void)                                 */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER マニュアル処                                 */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_man(void)
{
   Tu_pno_set(0);
   Tu_set_prechno();
   Tu_lastwrite();
   Sbm_mode_cancel(CMA_TUNER);
   return(CTU_I_OKEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_cwait(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER 指定タイム待ち処理                           */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_cwait(void)
{
   switch (tu_i_seq)
   {
      case   0:
         if ((tu_updw_stus == CTU_BNDEDGE)
         || (tu_m_mode == CTU_M_SCAN))
         {
            tu_5ms_timer = tu_coef.wait_edge;
         }
         else
         {
            tu_5ms_timer = tu_coef.c_wait;
         }
         tu_i_seq++;
         break;
      case   1:
         if (tu_5ms_timer == TIMER_OUT)
         {
            return(CTU_I_OKEND);
         }
         break;
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}
/************************************************************************/
/* 関数名 :BYTE   Tu_I_seekst(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER シーク開始処理                              */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_seekst(void)
{
//   Tu_lastcall();
   Tu_pno_set(0);            /* PresetNo Clear */
   return(CTU_I_OKEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_qinfochk(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER SEEK局停止条件チェック処理                     */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_qinfochk(void)
{
   BYTE ret;
   
   ret = NG;
   switch (tu_i_seq)
   {
      case 0:
         tu_sme_cnt = 0;
         tu_if_cnt = 0;
         tu_usn_cnt = 0;
         tu_wam_cnt = 0;
         
         tu_sme_ave[0] = 0;
         tu_sme_ave[1] = 0;
         tu_if_ave[0] = 0;
         tu_if_ave[1] = 0;
         tu_usn_ave[0] = 0;
         tu_usn_ave[1] = 0;
         tu_wam_ave[0] = 0;
         tu_wam_ave[1] = 0;
         if ((tu_updw_stus == CTU_BNDEDGE) ||
            (tu_updw_stus == CTU_LMEDGE))      /*   Buglist#24対応、バンドエッジ待ち、20100712   */
         {
            tu_5ms_timer = tu_coef.wait_edge;
         }
         tu_i_seq ++;
         break;
         
      case   1:
         if( tu_5ms_timer != 0 )
         {
            break;
         }
         if(Tu_band_chk() == CTU_AM)
         {
            tu_1ms_timer = tu_chkcef[CTU_AM].wait_tm;      /*   EEPROMから初期パラメータを取得対応、100811、岳   */
         }
         else
         {
            tu_1ms_timer = tu_chkcef[CTU_FM].wait_tm;      /*   EEPROMから初期パラメータを取得対応、100811、岳   */
         }
         tu_i_seq++;
         break;
      case   2:
         if (tu_1ms_timer == TIMER_OUT)
         {
            tu_ic_data.msa.subaddr = 0xe0;
            tu_ic_data.length = 5;
            Tu_data_read();/* SD値取り込み */
            ret = Tu_Average_QInfo(tu_ic_data.rxbuf);
            if(ret == OK)
            {
               return(CTU_I_OKEND);
            }
            else if( ret == NG )
            {
               return(CTU_I_NGEND);
            }
            else
            {
               tu_i_seq --;
            }
         }
         break;
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}

#if 0
/************************************************************************/
/* 関数名 :BYTE   Tu_I_icreset(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER IC RESET処理                                 */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_sdchk(void)
{
   BYTE   band;

   band = Tu_band_chk();
   switch (tu_i_seq)
   {
      case   0:
         tu_1ms_timer = 10/1;
         tu_i_seq++;
         break;
      case   1:
         if (tu_1ms_timer == TIMER_OUT)
         {
            tu_ic_data.msa.subaddr = 0xe0;
            tu_ic_data.length = 5;
            Tu_data_read();/* SD値取り込み */
            if((tu_ic_data.rxbuf[0]&0xC0) == 0x40)
            {
#if 0         /*   S-Meter値下げチェック、20100603、岳   */
               if (tu_ic_data.rxbuf[1] > tu_coef.sd_dx)
#endif            /*   EEPROMから初期パラメータを取得対応、100811、岳   */
               if (tu_ic_data.rxbuf[1] > (tu_dx[band] * 10 / tu_chkcef[band].prec_sm))
               {
                  return(CTU_I_OKEND);
               }
               else
               {
                  return(CTU_I_NGEND);
               }
            }
         }
         break;
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_icreset(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER IC RESET処理                                 */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_ifchk(void)
{
   BYTE   ifcnt;
   BYTE   band;
   BYTE   usn;
   BYTE   wam;
   
   usn = 0;
   wam = 0;
   band = Tu_band_chk();
   
#if _TUNER_BND_WB_
   if(Tu_band_chk() == CTU_WB)
   {
      return(CTU_I_OKEND);
   }
#endif
   switch (tu_i_seq)
   {
      case   0:
         tu_10ms_timer = 160/10;
         tu_seekng_cnt = 0;
         tu_i_seq++;
         break;
      case   1:
         if (tu_1ms_timer == TIMER_OUT)
         {
            tu_1ms_timer = 10/1;
            tu_ic_data.msa.subaddr = 0xe0;
            tu_ic_data.length = 5;
            Tu_data_read();/* SD値取り込み */
            if((tu_ic_data.rxbuf[3]&0xC0) == 0xC0)
            {
               ifcnt = (tu_ic_data.rxbuf[3]&0x1F);
               if( band == CTU_FM )
               {
                  usn = tu_ic_data.rxbuf[2]>>4;
                  wam = tu_ic_data.rxbuf[2]&0x0F;
               }
               if((ifcnt <= tu_chkcef[band].if_detail)
#if 0         /*   S-Meter値下げチェック、20100603、岳   */
                  &&(tu_ic_data.rxbuf[1] > tu_coef.sd_loc)
#endif         /*   EEPROMから初期パラメータを取得対応、100811、岳   */
                  &&(tu_ic_data.rxbuf[1] > tu_dx[band])
                  &&(usn <= tu_chkcef[band].usn_max)
                  &&(wam <= tu_chkcef[band].wam_max))
               {
                  if(tu_10ms_timer == 0)
                  {
                     return(CTU_I_OKEND);
                  }
               }
               else
               {
                  tu_seekng_cnt ++;
                  if(tu_seekng_cnt >= 2)
                  {
                     return(CTU_I_NGEND);
                  }
               }
            }
         }
         break;
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}
#endif
/************************************************************************/
/* 関数名 :BYTE   Tu_I_seekend(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER シーク終了処理                              */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE   Tu_I_seekend(void)
{
   Tu_set_prechno();
   Tu_lastwrite();                  /* ﾗｽﾄ書込み */
   Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);      /* すべて時限表示ｷｬﾝｾﾙ */
   Lcd_disp_base(CMA_TUNER);
   return(CTU_I_OKEND);
}
/******************************************************************************
   関   数  名   : Tu_I_isrread
   引       数   : 無し
   戻   り  値   : 無し
   機 能 概 略   : ISR 読み出し処理
    備       考 :
-------------------------------------------------------------------------------
[zhang070312] 新規作成
********************************************************************************/
BYTE   Tu_I_isrread(void)
{
   Tu_lastcall();
   return(CTU_I_OKEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_apmst(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER オートストア開始処理                           */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_apmst(void)
{
   Tu_as_initial();               /* APMｴﾘｱｸﾘｱ */
   Tu_updw_set(TRUE);               /* ｱｯﾌﾟ方向ｾｯﾄ */
   tu_pno = 0x01;
   return(CTU_I_OKEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_apmset(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER オートストア有局書き込む処理                     */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_apmset(void)
{
   tu_as_cnt ++;
   tu_mem[tu_bandno].pset[tu_as_cnt] = tu_ch;

   if(tu_pno < CTU_ASMAX)
   {
      tu_pno ++;
   }
   return(CTU_I_OKEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_apmendchk(void)                           */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER オートストア終了チェック処理                     */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_apmendchk(void)
{
   TTU_CH   ch;

   if(tu_as_cnt >= CTU_ASMAX)
   {
      return(CTU_I_OKEND);
   }
   if (tu_updw_stus == CTU_BNDEDGE)
   {
      return(CTU_I_OKEND);
   }
   return(CTU_I_NGEND);
}

/************************************************************************/
/* 関数名 :BYTE   Tu_I_apmend(void)                              */
/* --------------------------------------------------------------------   */
/* 機能   :   TUNER オートストア終了処理                           */
/* 詳細   :   戻り値：処理状態                                 */
/*                CTU_I_OKEND ⇒ 処理OK完了                     */
/*               CTU_I_NGEND ⇒ 処理NG完了                     */
/*               CTU_I_NOTEND ⇒ 処理中                        */
/************************************************************************/
BYTE    Tu_I_apmend(void)
{
   BYTE   icnt;
   
   if(tu_as_cnt == 0)
   {
      Tu_lastcall();
      tu_m_mode = CTU_M_LSTCAL;
      Lcd_disp_base(CMA_TUNER);
      return(CTU_I_NGEND);         /* PresetScanへ遷移しない */
   }
   else
   {
      tu_pno = 1;         /* 格納のMIN PresetSET */
      Tu_presetcall();
      tu_m_mode = CTU_M_PSTRD;
      Lcd_disp_base(CMA_TUNER);
      #if   (_MODEL_EEPROM_ == 1)
      for (icnt = 0; icnt< tu_as_cnt ; icnt++)
      {
         tu_eepdata[icnt*2] = (BYTE)(tu_mem[tu_bandno].pset[icnt+1].freq & 0xFF);
         tu_eepdata[icnt*2 + 1] = (BYTE)((tu_mem[tu_bandno].pset[icnt+1].freq>>8) & 0xFF);
      }
      tu_eepreq.EepAddr   = CTU_EEPOFS_PREMEM + (tu_bandno * 16);
      tu_eepreq.Length   = tu_as_cnt * 2;
      tu_eepreq.BufAddr   = tu_eepdata;
      ftu_weepreq         = ON;
      tu_eep_cnt         = 0;
      #endif
      return(CTU_I_OKEND);
   }
}
/******************************************************************************
..       関数名  ：Tu_I_pstup
      引  数   ：無し
      戻り値   ：処理状態
            ：CTU_I_OKEND ⇒ 処理OK完了
            ：CTU_I_NGEND ⇒ 処理NG完了
            ：CTU_I_NOTEND ⇒ 処理中
      機  能   ：次のPreset番号ｾｯﾄ処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051228] 新規作成
[sun060105] 単体ﾃｽﾄ完[ﾐｽ1 PresetCallしていない]
[sun060821] Presetｺｰﾙする前にListｸﾘｱするように修正
         PresetNoｾｯﾄするだけでPLL設定しないと、ﾃﾞｺｰﾄﾞとOneMainずれてしまう
[zhang070313]処理を変更
[zhang070314]bandno 切換を変更
******************************************************************************/
BYTE   Tu_I_pstup(void)
{
   tu_pno ++;
   if (tu_pno > 6)
   {
      tu_pno = 0x01;         /* Preset1に戻る */
      if(Tu_band_chk() == CTU_FM)
      {
         tu_bandno++;
         if(Tu_band_chk() == CTU_AM)
         {
            tu_bandno = 0;
         }
      }
      else
      {
         tu_bandno++;
         if (tu_bandno >= CTU_BAND)
         {
            tu_bandno = Tu_fmbandmax() + 1;
         }
      }
   }
   Tu_pno_set(tu_pno);
   /* Preset周波数読み出し */         /* [070625zhang] 指摘20を修正 */
   tu_ch = tu_mem[tu_bandno].pset[tu_pno];      /* Preset周波数、REG、ListID */
   Tu_ch_lm_set(tu_ch.freq);               /* LW/MWﾌﾗｸﾞｾｯﾄ */
//   Tu_presetcall();
   return(CTU_I_OKEND);
}

/******************************************************************************
..       関数名  ：Tu_I_pstscan
      引  数   ：無し
      戻り値   ：処理状態
            ：CTU_I_OKEND ⇒ 処理OK完了
            ：CTU_I_NGEND ⇒ 処理NG完了
            ：CTU_I_NOTEND ⇒ 処理中
      機  能   ：ｵｰﾄﾌﾟﾘｾｯﾄScan 処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matsu120713] 新規作成
******************************************************************************/
BYTE   Tu_I_scanwait(void)
{
   switch (tu_i_seq)
   {
      case   0:
         Tu_set_prechno();
         #if 0   /* audioモジュール250ms待ってるので、削除 */
         Tu_aftermute(CTU_AFTER250);      /* AfterMute 250ms */
         #else
         Tu_aftermute(0);      /* AfterMute 250ms */
         #endif
         Tu_stdis_timset(CTU_ST_PLLCHG);      /*500msST検出禁止 */
         tu_100ms_timer = CTU_SCANWAIT_TIM;   /* Tuner Scanﾀｲﾏ */
         tu_i_seq++;
         break;
      case   1:
         if (tu_100ms_timer == TIMER_OUT)
         {
            return(CTU_I_OKEND);
         }
         break;
      default:
         tu_i_seq = 0;
         break;
   }
   return(CTU_I_NOTEND);
}

/******************************************************************************
..       関数名  ：Tu_I_pstchk
      引  数   ：無し
      戻り値   ：処理状態
            ：CTU_I_OKEND ⇒ 処理OK完了
            ：CTU_I_NGEND ⇒ 処理NG完了
            ：CTU_I_NOTEND ⇒ 処理中
      機  能   ：周波数からPresetNoを検索する
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matsu120713] 新規作成
******************************************************************************/
BYTE   Tu_I_pstchk(void)
{
   Tu_set_prechno();            
   return(CTU_I_OKEND);
}

#undef  _TU_ITEM_C_

#endif
