/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

      プロジェクト名   :
      ファイル名      : tu_ext.c
      機能         : ?Tuner 外部IF処理
------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212]   新規作成
******************************************************************************/
#include   "../model.h"

#if   _MODEL_RADIO_
#define      _TU_EXT_C_

#include   "../comm/common.h"
#include   "../disp/lcd_ext.h"
#include   "../main/ma_ext.h"
#include   "../submode/sbm_ext.h"
#include   "../audio/aud_ext.h"
#include   "../key/key_def.h"

#include   "tu_sys.h"
#include   "tu_main.h"
#include   "tu_ext.h"
#include   "tu_event.h"
#include   "tu_io.h"
#include   "tu_key.h"
#include   "tu_test.h"

/******************************************************************************
..       関数名  ：Tu_band_chk
      引  数   ：無し
      戻り値   ：TUNER BAND情報
            ：CTU_FM ⇒ FM BAND中
            ：CTU_AM ⇒ AM BAND中
      機  能   ：TUNER BAND情報を通知処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
[zhang070315] isr mode時,isr bandを通知
******************************************************************************/
BYTE Tu_band_chk(void)
{
   if(Ma_current_mode()==CMA_ISR)
   {
      return(CTU_AM);
   }
   return(tu_mem[tu_bandno].band);
}

/******************************************************************************
..       関数名  ：Tu_bandno_chk
      引  数   ：無し
      戻り値   ：TUNER BAND NO情報
            ：CTU_BAND0 ⇒ BAND NO無し
            ：CTU_BAND1 ⇒ BAND 1
            ：CTU_BAND2 ⇒ BAND 2
            ：CTU_BAND3 ⇒ BAND 3
      機  能   ：TUNER BAND NO情報を通知処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
******************************************************************************/
BYTE Tu_bandno_chk(void)
{
   return(tu_mem[tu_bandno].bno);
}

/******************************************************************************
..       関数名  ：Tu_pstno_chk
      引  数   ：無し
      戻り値   ：TUNER Preset NO情報
            ：0 ⇒ Preset受信中ではない
            ：1〜6 ⇒ Preset番号
      機  能   ：TUNER Preset NOを通知処理
              disp使用のみ
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
[sun051213] ｶﾚﾝﾄPresetNoを返すように変更
******************************************************************************/
BYTE Tu_pstno_chk(void)
{
   return(tu_pno);
}

/******************************************************************************
..       関数名  ：Tu_1ms_timer
      引  数   ：無し
      戻り値   ：無し
      機  能   ：TUNER 1msﾀｲﾏ処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
******************************************************************************/
void Tu_1ms_timer(void)
{
   if (tu_1ms_timer > 0)
   {
      tu_1ms_timer --;
   }
}

/******************************************************************************
..       関数名  ：Tu_5ms_timer
      引  数   ：無し
      戻り値   ：無し
      機  能   ：TUNER 5msﾀｲﾏ処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
******************************************************************************/
void Tu_5ms_timer(void)
{
   if (tu_5ms_timer > TIMER_OUT)
   {
      tu_5ms_timer --;
   }
   
   Tu_5ms_test();
}
#if 0
/******************************************************************************
..       関数名  ：Tu_10ms_timer
      引  数   ：無し
      戻り値   ：無し
      機  能   ：TUNER 10msﾀｲﾏ処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
******************************************************************************/
void Tu_10ms_timer(void)
{
   ftu_stchk = TRUE;
   if (tu_aftermute > TIMER_OUT)
   {
      tu_aftermute --;
   }

   if (tu_10ms_timer > 0)
   {
      tu_10ms_timer --;
   }
}
#endif
/******************************************************************************
..       関数名  ：Tu_indicator_set
      引  数   ：無し
      戻り値   ：Tunerｲﾝｼﾞｹﾞｰﾀ
      機  能   ：Tuner関係ｲﾝｼﾞｹﾞｰﾀ表示作成処理
      備   考   ：CLCD_IND_MANU

------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
******************************************************************************/
BYTE Tu_indicator_set( void )
{
   BYTE   indicator;
   BYTE   band;
   BYTE   bandno;
   
   indicator = 0;
   band = Tu_band_chk();
   bandno = Tu_bandno_chk();
   Tu_dispfreqset();
   
   if (band == CTU_FM)
   {
      indicator |= CLCD_TU_IND_FM;      /* "FM"インジケータ点灯 */
      if (Tu_st_check() == TRUE)
      {
         indicator |= CLCD_TU_IND_ST ;   /* "ST"インジケータ点灯 */
      }
      if (tu_dispfreq >= 10000)
      {
         indicator |= CLCD_TU_IND_S1;               /* "1"00インジケータ点灯 */
      }
      
   }
   else
   {
      indicator |= CLCD_TU_IND_AM;      /* "AM"インジケータ点灯 */
      if (tu_dispfreq >= 1000)
      {
         indicator |= CLCD_TU_IND_S1;               /* "1"000インジケータ点灯 */
      }
   }
   if (bandno == CTU_BAND1)
   {
      indicator |= CLCD_TU_IND_ONE;      /* band"1"インジケータ点灯 */
   }
   else if (bandno == CTU_BAND2)
   {
      indicator |= CLCD_TU_IND_TWO;      /* band"2"インジケータ点灯 */
   }
   else
   {
   }
   return(indicator);
}

/******************************************************************************
..       関数名  ：Tu_isr_indicator_set
      引  数   ：無し
      戻り値   ：Tuner isr ｲﾝｼﾞｹﾞｰﾀ
      機  能   ：Tuner isr 関係ｲﾝｼﾞｹﾞｰﾀ表示作成処理
      備   考   ：CLCD_IND_MANU

------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120730] 新規作成
******************************************************************************/
BYTE   Tu_isr_indicator_set( void )
{
   BYTE   indicator;
   
   indicator = 0;
   Tu_dispfreqset();
   
   indicator |= CLCD_TU_IND_TI ;      /* "・))"インジケータ点灯 */
   indicator |= CLCD_TU_IND_AM;      /* "AM"インジケータ点灯 */
   if (tu_dispfreq >= 1000)
   {
      indicator |=CLCD_TU_IND_S1;      /* "1"000インジケータ点灯 */
   }
   
   return(indicator);
}

/******************************************************************************
..       関数名  ：Tu_mainmd_check
      引  数   ：無し
      戻り値   ：TRUE->seek/ps/as中
             :FALSE->他ﾓｰﾄﾞ中
      機  能   ：Tuner main mode check処理
      備   考   ：
------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[zhang070412] 新規作成
******************************************************************************/
BYTE Tu_mainmd_check( void )
{
   if((tu_m_mode == CTU_M_SEEK)
      ||(tu_m_mode == CTU_M_AS)
      ||(tu_m_mode == CTU_M_SCAN)
      ||(tu_m_mode == CTU_M_CMAN))
   {
      return(TRUE);
   }
   return(FALSE);
}

/******************************************************************************
..       関数名  ：Tu_st_check
      引  数   ：無し
      戻り値   ：st検出 値
      機  能   ：Tuner st 検出を通知処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[zhang070309] 新規作成
******************************************************************************/
BYTE Tu_st_check(void)
{
   BYTE   tu_st;

   tu_st = 0;
   if (Tu_band_chk() == CTU_FM)
   {
      if (ftu_stero == TRUE)
      {
         tu_st = 1;
//         return(tu_st);
      }
   }
   return(tu_st);
}

/******************************************************************************
..       関数名  ：Tu_lcddata_set
      引  数   ：無し
      戻り値   ：表示ﾃﾞｰﾀ格納ｴﾘｱﾎﾟｲﾝﾀ
      機  能   ：Tuner表示ﾃﾞｰﾀ作成処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
******************************************************************************/
void Tu_lcddata_set(TDP_TUNER * tudisp, BYTE dispmode)
{
   BYTE   bcddata[3];
   BYTE   iRet;

   tudisp->band = Tu_band_chk();
   if(tu_m_mode == CTU_M_PSTWT)
   {
      tudisp->preno = tu_wrtpno;
   }
   else if (tu_m_mode == CTU_M_AS)
   {
      tudisp->preno = 0;
   }
   else
   {
      tudisp->preno = tu_pno;
   }

   Tu_dispfreqset();               /* 周波数表示用変換 */

   iRet = Cm_lib_wHEXtoBCD(tu_dispfreq,bcddata);   /* HEX→BCD変換 */

   tudisp->frq[0] = bcddata[0];
   tudisp->frq[1] = bcddata[1];
   tudisp->frq[2] = bcddata[2];

   if (tu_m_mode == CTU_M_SCAN)   /* 「070723張」low3 buglistの 97修正*/
   {
//      Sbm_mode_cancel(CMA_TUNER);
      if(tu_m_seq == CTU_SCANWAIT_SEQ)   /* SCAN受信中 */
      {
         tudisp->mode = CLCD_TU_SCANWAIT;
      }
      else                     /* SCAN SEEK中 */
      {
         tudisp->mode = CLCD_TU_SEEK;
      }
   }
   else if (tu_m_mode == CTU_M_PSTWT)
   {
      tudisp->mode = CLCD_TU_PSTWT;
   }
   else if(tu_m_mode == CTU_M_SEEK)
//         (tu_m_mode == CTU_M_CMAN))
   {
//      Sbm_mode_cancel(CMA_TUNER);
      tudisp->mode = CLCD_TU_SEEK;
   }
   else if(tu_m_mode == CTU_M_AS)
   {
      tudisp->mode = CLCD_TU_AS;
   }
   else
   {
      ;
   }
}

/******************************************************************************
..       関数名  ：Tu_u_lcddata
      引  数   ：無し
      戻り値   ：表示ﾃﾞｰﾀ格納ｴﾘｱﾎﾟｲﾝﾀ
      機  能   ：Tuner isr表示ﾃﾞｰﾀ作成処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120724] 新規作成
******************************************************************************/
void Tu_isr_lcddata_set(TDP_ISR   * isrdisp,BYTE   dispmode)
{
   BYTE   bcddata[3];
   BYTE   iRet;

   isrdisp->band = Tu_band_chk();
   Tu_dispfreqset();               /* 周波数表示用変換  trong file tu_event.c*/

   iRet = Cm_lib_wHEXtoBCD(tu_dispfreq,bcddata);   /* HEX→BCD変換 */

   isrdisp->frq[0] = bcddata[0];
   isrdisp->frq[1] = bcddata[1];
   isrdisp->frq[2] = bcddata[2];
}

/******************************************************************************
..       関数名  ：Tu_amband_set
      引  数   ：無し
      戻り値   ：無し
      機  能   ：AMバンド設定処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120724] 新規作成
******************************************************************************/
void Tu_amband_set(void)
{
   tu_bandno = CTU_BAND_AM;   /* AM指定 */
   Tu_lastcall();
}

/********************************************************************************/
/*   関数名  ：Tu_isrmd_on                                          */
/*   引  数   ：mode:                                                */
/*   戻り値   ：なし                                                */
/*   機  能   ：Tuner ISRモードONに設定                                 */
/*   注意事項:                                                   */
/*   修正履歴   【年月日】   【Version】   【名前】    【説明】               */
/*------------------------------------------------------------------------------*/
/*            2010.08.09      0.0         岳      新規作成               */
/********************************************************************************/
void   Tu_isrmd_on(void)
{
   ftu_isr = ON;
}
/********************************************************************************/
/*   関数名  ：Tu_isrmd_off                                          */
/*   引  数   ：mode:                                                */
/*   戻り値   ：なし                                                */
/*   機  能   ：Tuner ISRモードOFFに設定                                 */
/*   注意事項:                                                   */
/*   修正履歴   【年月日】   【Version】   【名前】    【説明】               */
/*------------------------------------------------------------------------------*/
/*            2010.08.09      0.0         岳      新規作成               */
/********************************************************************************/
void   Tu_isrmd_off(void)
{
   ftu_isr = OFF;
}

/*   テストモードの直接AM/FM1選択を対応、100826、岳   */
void   Tu_bnd_set(BYTE bandno)
{
   Tu_job_clear();
   if( bandno == CTU_BAND_AM )
   {
      tu_bandno = CTU_BAND_AM;
   }
   else
   {
      if (( Ma_model_dest() >= CMA_MODEL_AMFM_J )
        &&( Ma_model_dest() <= CMA_MODEL_AMFM_X ))
      {
         tu_bandno = CTU_BAND_FM1;
      }
      else
      {
         return;
      }
   }
   Tu_lastcall();                  /* ﾗｽﾄ呼出 */
   tu_m_mode = CTU_M_BANDCHG;         /* BandChangeﾓｰﾄﾞｾｯﾄ */
   Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);      /* すべて時限表示ｷｬﾝｾﾙ */
   Lcd_disp_base(CMA_TUNER);
   Sbm_mode_cancel(CMA_TUNER);
}

void Tu_beep_mute(BYTE on_off)
{
   Tu_beep_mute_set(on_off);
}
#undef  _TU_EXT_C_

#endif
