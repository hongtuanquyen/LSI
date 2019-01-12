/******************************************************************************
..     ƒNƒ‰ƒŠƒIƒ““aŒü‚¯                    2007ÓÃŞÙ

      ƒvƒƒWƒFƒNƒg–¼   :
      ƒtƒ@ƒCƒ‹–¼      : tu_sys.c
      ‹@”\         : ‡@Tuner SystemÒ¯¾°¼Şó•tˆ—
                  : ‡AÀ²ÏŠÖŒWˆ—
------------------------------------------------------------------------------
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051125]   V‹Kì¬
[matu120903] S-meter”»’è Local’l’Ç‰Á 
******************************************************************************/
#include   "../model.h"

#if   _MODEL_RADIO_

#define   _TU_MAIN_C_

#include   <string.h>
#include   "../comm/common.h"
#include   "../main/ma_ext.h"
#include   "../disp/lcd_ext.h"
#include   "../audio/aud_ext.h"
#include   "../power/pw_func_ext.h"   /* QAC‘Îô */
#if      (_MODEL_EEPROM_ == 1)
#include   "../eeprom/eeprom_ext.h"
#include   "tu_eeprom.h"
#endif
#include   "tu_def.h"
#include   "tu_ext.h"
#include   "tu_io.h"
#include   "tu_main.h"
#include   "tu_item.h"
#include   "tu_conttbl.h"
#include   "tu_event.h"
#include   "tu_test.h"
static   void   Tu_main_cont(void);
static   void   Tu_sterochk(void);
static   void   Tu_mute_cont(void);
static   void   Tu_bandtbl_set(BYTE destno);
static   void   Tu_bandpara_set(BYTE destno);




/******************************************************************************
..       ŠÖ”–¼  FTu_main
      ˆø  ”   F‚È‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FTunerMainˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051125] V‹Kì¬
******************************************************************************/
void   Tu_main(void)
{
   if (tu_pwrmode != CTU_PWROFF)
   {
      Tu_main_cont();         /* TunerÒ²İºİÄÛ°Ùˆ— */
      Tu_sterochk();         /* ½ÃÚµM†ŒŸoˆ— */
   }
   Tu_mute_cont();            /* TunerMUTEºİÄÛ°Ùˆ— */
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FTu_main_cont()
      ˆø  ”   F‚È‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FTuner“®ìºİÄÛ°Ùˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051212] V‹Kì¬
[sun060616] Ò²İÓ°ÄŞ”ÍˆÍÁª¯¸’Ç‰Á
-----------------------------------------------------------------------------*/
static   void   Tu_main_cont(void)
{
   #if 1
   if ((Pw_ACC_DET_get() == OFF)||(Pw_Power_status_get() == OFF))
   #else
   if (Pw_ACC_DET_get() == OFF)
   #endif
   {
      return;
   }
   
   if (tu_m_mode != CTU_NOP)            /* “®ì’† */
   {
      if (tu_m_mode >= CTU_M_MAX)
      {
         tu_m_mode = CTU_NOP;         /* ”ÍˆÍŠO‚Å‚ ‚ê‚ÎAƒNƒŠƒA */
      }
      else
      {
         Tu_main_process();
      }
   }
#if   (_MODEL_EEPROM_ == 1)
   if( ftu_weepreq == ON )
   {
      Tu_WriteEepData();
   }
#endif
   Tu_Test_Main();
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FTu_sterochk()
      ˆø  ”   F‚È‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FTuner½ÃÚµM†ŒŸoˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060112] V‹Kì¬
[sun060619] ’P‘ÌÃ½ÄŠ®
[zhang070301]‚r‚sƒCƒ“ƒWƒP[ƒ^‚ÌSD1.47VˆÈ‰º‚Ì”»’f‚ğíœ‚·‚é
-----------------------------------------------------------------------------*/
static   void   Tu_sterochk(void)
{
   if (Tu_band_chk() != CTU_FM) 
   {
      Tu_steroclr();                  /* STó‘Ô¸Ø± */
      return;
   }
   if ((tu_m_mode != CTU_NOP) &&         /* ScanˆÈŠO“®ì’†STŒŸo‚µ‚È‚¢ */
      (tu_m_mode != CTU_M_SCAN))
   {
      Tu_steroclr();                  /* STó‘Ô¸Ø± */
      return;
   }
   if (tu_stdistim != TIMER_OUT)         /* STŒŸo‹Ö~’† */
   {
      Tu_steroclr();                  /* STó‘Ô¸Ø± */
      return;
   }

   if (ftu_stchk == ON)               /* 10msÌ×¸ŞŒo‰ß */
   {
      ftu_stchk = OFF;
      Tu_sterochat();                  /* STÁ¬ÀØİ¸Şˆ— */
   }
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FTu_mute_cont()
      ˆø  ”   F‚È‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FTuner MUTEˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051212] V‹Kì¬
[sun061026] “®ì‚µ‚Ä‚¢‚È‚¢‚Ì‚ÉAMUTE‚©‚¯‚Ä‚¢‚é‚Å‚ ‚ê‚ÎAMUTE‰ğœ‚·‚é
         “®ì‚µ‚Ä‚¢‚È‚¢‚Æ‚«‚ÉRDS MUTE‚à‰ğœ‚·‚é‚æ‚¤‚É•ÏX
-----------------------------------------------------------------------------*/
static   void   Tu_mute_cont(void)
{
//               /* [070524] ƒL[’·‰ŸMUTEˆ—C³*/
   #if 0
   if ((ftu_mute == ON) &&
      (tu_m_mode == CTU_NOP) &&
      (tu_pwrmode == CTU_FUNCON))
   {
      Tu_aftermute(CTU_AFTER500);
   }
   #endif
   if ((ftu_mute == OFF) &&
      (tu_aftermute == TIMER_OUT)&&
      (Aud_get_power_status() == CAUD_PWR_ON))
   {
      Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,OFF);   /* TunerMute—v‹ON */
   }
}

/******************************************************************************
..       ŠÖ”–¼  FTu_initial
      ˆø  ”   F‚È‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FTuner‰Šú‰»ˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051125] V‹Kì¬
[sun051212] ‰Šúİ’èˆ—’Ç‰Á
[sun060421] listid‰Šú‰»ˆ—’Ç‰Á
[sun060501] ×½Äü”g”LIST ID‰Šú‰»˜R‚êC³
[zhang070301]×½Äü”g”‰Šúİ’è‚ğ•ÏX
[zhang070312]isr‰Šúİ’è‚ğ’Ç‰Á
[Šx100806]EEPROM‹@”\‚ğˆê•”‘Î‰(ƒ‰ƒXƒgƒƒ‚ƒŠ)
[matu120903] S-meter”»’è Local’lZoˆ—’Ç‰Á 
******************************************************************************/
void   Tu_initial(void)
{
   BYTE   destno;
   BYTE   iLoop1;
   
   /* dŒü‚¯–³Œø‚Ì”»’fˆ—’Ç‰Á */
   if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_NG)||
      (Ma_Chk_EEP_Initial() == CMA_INIT_STA_AREA_NG))
   {
      return;
   }
   
   /* dŒü‚¯‚Ìæ“¾ */
   destno = Tu_dest_chk();  // tra ve vung tan so AM hay AMFM cua USA hay JPN
   
   /* BANDî•ñ‰Šúİ’è */
   for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)  // CTU_BAND = 3
   {
      tu_mem[iLoop1].band = TU_BAND_INFO[destno][iLoop1][0];      /* FM/AM */
      tu_mem[iLoop1].bno = TU_BAND_INFO[destno][iLoop1][1];      /* Band No */
   }
   /* ƒ‰ƒXƒgî•ñİ’è */
   tu_bandno = 0;                        /* ƒ‰ƒXƒgƒoƒ“ƒhİ’è */
   for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
   {
      tu_mem[iLoop1].pstno = 0;            /* ƒ‰ƒXƒgƒvƒŠƒZƒbƒgİ’è */
      tu_mem[iLoop1].pset[0].freq = 0x0000;   /* ƒ‰ƒXƒgü”g”İ’è   .pset[] có 7 phan tu, 0: last frequency area, 1 to 6: Preset area */
   }
   
   Tu_bandtbl_set(destno);                  /* Band Tableİ’è */
   
   #if   (CTU_E_TEST == 0x01)
   tu_mem[3].pstno = 2;                           /* ×½ÄPresetNo */
   #endif

   tu_isrch.freq = TU_ISR_TBL[1];      /* ISRü”g”İ’è */

   tu_test_mode = 0xFF;

   wMemSet(tu_para[0],0xFF,sizeof(tu_para));
   Tu_bandpara_set(destno);               /* Band Parameterİ’è */

   tu_level_FM1db = Tu_Level_db_Chg(tu_chkcef[CTU_FM].sm_20db,tu_chkcef[CTU_FM].sm_40db);
   tu_level_AM1db = Tu_Level_db_Chg(tu_chkcef[CTU_AM].sm_20db,tu_chkcef[CTU_AM].sm_40db);
   /* S-meter”»’è’l@DXİ’è */
   if( tu_chkcef[CTU_FM].sm_dxdb > 20 )
   {
      tu_dx[CTU_FM]   = ((tu_level_FM1db * (tu_chkcef[CTU_FM].sm_dxdb - tu_chkcef[CTU_FM].sm_20db))/10 +  tu_chkcef[CTU_FM].sm_20db);
   }
   else
   {
      tu_dx[CTU_FM]      = (tu_chkcef[CTU_FM].sm_20db - (tu_level_FM1db * (tu_chkcef[CTU_FM].sm_20db - tu_chkcef[CTU_FM].sm_dxdb))/10);
   }
   if( tu_chkcef[CTU_AM].sm_dxdb > 20 )
   {
      tu_dx[CTU_AM]         = ((tu_level_AM1db * (tu_chkcef[CTU_AM].sm_dxdb - tu_chkcef[CTU_AM].sm_20db))/10 +  tu_chkcef[CTU_AM].sm_20db);
   }
   else
   {
      tu_dx[CTU_AM]         = (tu_chkcef[CTU_AM].sm_20db - (tu_level_AM1db * (tu_chkcef[CTU_AM].sm_20db - tu_chkcef[CTU_AM].sm_dxdb))/10);
   }
   /* S-meter”»’è’l@LOCAL’lİ’è *//* [matu120903] */
   if( tu_chkcef[CTU_AM].sm_lodb > 20 )
   {
      tu_lo[CTU_AM]         = ((tu_level_AM1db * (tu_chkcef[CTU_AM].sm_lodb - tu_chkcef[CTU_AM].sm_20db))/10 +  tu_chkcef[CTU_AM].sm_20db);
   }
   else
   {
      tu_lo[CTU_AM]         = (tu_chkcef[CTU_AM].sm_20db - (tu_level_AM1db * (tu_chkcef[CTU_AM].sm_20db - tu_chkcef[CTU_AM].sm_lodb))/10);
   }
   
   Tu_lastcall();
   tu_wrtpno = 0x01;               /* ƒvƒŠƒZƒbƒg1‚ğİ’è */
   tu_qinfochk_num = CTU_SEEK_NUM;      /* Q-infoŠm”F‰ñ”İ’è•Ï” [matu121105] */
}

/************************************************************************/
/* ŠÖ”–¼ :void   Tu_bandtbl_set(void)                           */
/* --------------------------------------------------------------------   */
/* ‹@”\   :   Bandƒe[ƒuƒ‹İ’èˆ—                              */
/* Ú×   :                                                */
/************************************************************************/
void   Tu_bandtbl_set(BYTE destno)
{
   BYTE   iLoop1,iLoop2;
#if   (_MODEL_EEPROM_ == 1)      /*   EEPROM‹@”\‚ğˆê•”‘Î‰Aƒ‰ƒXƒgƒƒ‚ƒŠA100806AŠx   */
   BYTE   tu_eepbuf[48];
   BYTE   pcnt;
#endif
   
   /* ƒvƒŠƒZƒbƒgƒƒ‚ƒŠî•ñ */
#if   (_MODEL_EEPROM_ == 1)      /*   EEPROM‹@”\‚ğˆê•”‘Î‰Aƒ‰ƒXƒgƒƒ‚ƒŠA100806AŠx   */
   if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_OK)&&   /*   EEPROM‰Šú‰»Ï‚İƒ`ƒFƒbƒNA¸”sê‡ƒfƒtƒH[ƒ‹ƒg’l‚ğg‚¤   */
      (Tu_ReadEepData(tu_eepbuf,destno) == TRUE))
   {
      /* AM‚ÆFM Presetü”g”‰Šúİ’è */
      for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
      {
         for (iLoop2 = 1; iLoop2 < (CTU_PRESET+1); iLoop2 ++)
         {
            pcnt = ((iLoop1)*16) + ((iLoop2 - 1) * 2);
            tu_mem[iLoop1].pset[iLoop2].freq = (WORD)((tu_eepbuf[pcnt+1]*0x100) + tu_eepbuf[pcnt]);
         }
      }
   }
   else
#endif
   {
      /* ×½Ä/Presetü”g”‰Šúİ’è */
      Tu_intset_pchtbl(destno);
      
      /* EEPROM‚Ö³‚µ‚¢ü”g”‚ğ‘‚«‚Ş */
      Tu_SaveEepData(CTU_EEPWR_PST,tu_eepdata);
      EEP_Initial_para_Req(tu_eepdata,0x00);
   }
}


/************************************************************************/
/* ŠÖ”–¼ :void   Tu_bandpara_set(void)                           */
/* --------------------------------------------------------------------   */
/* ‹@”\   :   Bandƒpƒ‰ƒ[ƒ^İ’èˆ—                                 */
/* Ú×   :                                                */
/************************************************************************/
void   Tu_bandpara_set(BYTE destno)
{
#if   (_MODEL_EEPROM_ == 1)      /*   EEPROM‹@”\‚ğˆê•”‘Î‰Aƒ‰ƒXƒgƒƒ‚ƒŠA100806AŠx   */
   BYTE   tu_eepbuf[96];
   BYTE   ret;
#endif

#if   (_MODEL_EEPROM_ == 1)
   ret = Tu_ReadEepPara(tu_eepbuf,destno);
   if( ret & CTU_EEP_ADR_Q_INF1_CHK )
   {
      tu_chkcef[CTU_FM].sm_20db = tu_eepbuf[CTU_EEP_ADR_FMSMETER20DB];
      tu_chkcef[CTU_FM].sm_40db = tu_eepbuf[CTU_EEP_ADR_FMSMETER40DB];
      
      tu_chkcef[CTU_AM].sm_20db = tu_eepbuf[CTU_EEP_ADR_AMSMETER20DB];
      tu_chkcef[CTU_AM].sm_40db = tu_eepbuf[CTU_EEP_ADR_AMSMETER40DB];
      
      tu_chkcef[CTU_FM].sm_dxdb = tu_eepbuf[CTU_EEP_ADR_FMSMDXLV];
      tu_chkcef[CTU_AM].sm_dxdb = tu_eepbuf[CTU_EEP_ADR_AMSMDXLV];
      
      tu_chkcef[CTU_AM].sm_lodb = tu_eepbuf[CTU_EEP_ADR_AMSMLOLV];   /* [matu120903] */
      
      tu_chkcef[CTU_FM].usn_max = tu_eepbuf[CTU_EEP_ADR_FMUSN];
      tu_chkcef[CTU_FM].wam_max = tu_eepbuf[CTU_EEP_ADR_FMWAM];
   }
   else
   {
      romcopy(&tu_chkcef[CTU_FM].sm_20db,(BYTE *)&TU_INI_CHKCOEF[CTU_FM],4);
      
      tu_chkcef[CTU_FM].usn_max = TU_INI_CHKCOEF[CTU_FM].usn_max;
      tu_chkcef[CTU_FM].wam_max = TU_INI_CHKCOEF[CTU_FM].wam_max;
      
      romcopy(&tu_chkcef[CTU_AM].sm_20db,(BYTE *)&TU_INI_CHKCOEF[CTU_AM],4);
      
      #if 0   /* d—l•ÏX */
      tu_chkcef[CTU_AM].sm_lodb = TU_INI_CHKCOEF[CTU_AM].sm_lodb;      /* [matu120903] */
      #endif
   }
   if( ret & CTU_EEP_ADR_PARA_FM_CHK )
   {
      tu_para[CTU_FM][CTU_PARA_TUNER0] = 0x20;
      if(( destno == CTU_DEST_JPN_A )
      || ( destno == CTU_DEST_JPN_FA ))
      {
         wMemCopy(&tu_eepbuf[CTU_EEP_ADR_PARA_FMJ],&tu_para[CTU_FM][CTU_PARA_TUNER2],(CTU_PARA_LEN-2));
      }
      else
      {
         wMemCopy(&tu_eepbuf[CTU_EEP_ADR_PARA_FMU],&tu_para[CTU_FM][CTU_PARA_TUNER2],(CTU_PARA_LEN-2));
      }
   }
   else
   {
      romcopy(tu_para[0],(BYTE *)TU_PARA_TBL[destno][CTU_FM],CTU_PARA_LEN);
   }
   
   if( ret & CTU_EEP_ADR_PARA_AM_CHK )
   {
      tu_para[CTU_AM][CTU_PARA_TUNER0] = 0x00;
      wMemCopy(&tu_eepbuf[CTU_EEP_ADR_PARA_AM],&tu_para[CTU_AM][CTU_PARA_TUNER2],(CTU_PARA_LEN-2));
   }
   else
   {
      romcopy(tu_para[1],(BYTE *)TU_PARA_TBL[destno][CTU_AM],CTU_PARA_LEN);
   }
   
   if( ret & CTU_EEP_ADR_Q_INF2_CHK )
   {
      tu_chkcef[CTU_FM].if_rough = tu_eepbuf[CTU_EEP_ADR_FM_IFROUGH];
      tu_chkcef[CTU_FM].if_detail = tu_eepbuf[CTU_EEP_ADR_FM_IFDETAIL];
      
      tu_chkcef[CTU_AM].if_rough = tu_eepbuf[CTU_EEP_ADR_AM_IFROUGH];
      tu_chkcef[CTU_AM].if_detail = tu_eepbuf[CTU_EEP_ADR_AM_IFDETAIL];
      
      tu_chkcef[CTU_FM].wait_tm = tu_eepbuf[CTU_EEP_ADR_FM_Q_WAIT];
      tu_chkcef[CTU_AM].wait_tm = tu_eepbuf[CTU_EEP_ADR_AM_Q_WAIT];
   }
   else
   {
      romcopy(&tu_chkcef[CTU_FM].if_rough,(BYTE *)&TU_INI_CHKCOEF[CTU_FM].if_rough,3);
      if( destno == CTU_DEST_THR )/* ‘æO’nˆædŒü‚¯‚ÍIFDetail’l‚Ì‚İˆá‚¤ */
      {
         romcopy(&tu_chkcef[CTU_AM].if_rough,(BYTE *)&TU_INI_CHKCOEF[CTU_AM].if_rough,1);
         tu_chkcef[CTU_AM].if_detail = TU_AM_IF_DETAIL_THIRD_DEST;
         romcopy(&tu_chkcef[CTU_AM].wait_tm,(BYTE *)&TU_INI_CHKCOEF[CTU_AM].wait_tm,1);
      }
      else
      {
         romcopy(&tu_chkcef[CTU_AM].if_rough,(BYTE *)&TU_INI_CHKCOEF[CTU_AM].if_rough,3);
      }
   }
   if( ret & CTU_EEP_ADR_Q_INF3_CHK )
   {
      tu_chkcef[CTU_AM].prec_sm = tu_eepbuf[CTU_EEP_ADR_AM_PERC_LV];
      tu_chkcef[CTU_FM].prec_sm = tu_eepbuf[CTU_EEP_ADR_FM_PERC_LV];
      tu_chkcef[CTU_FM].prec_noise = tu_eepbuf[CTU_EEP_ADR_FM_PERC_NS];
   }
   else
   {
      tu_chkcef[CTU_AM].prec_sm = TU_INI_CHKCOEF[CTU_AM].prec_sm;
      tu_chkcef[CTU_FM].prec_sm = TU_INI_CHKCOEF[CTU_FM].prec_sm;
      tu_chkcef[CTU_FM].prec_noise = TU_INI_CHKCOEF[CTU_FM].prec_noise;
   }
#else
      romcopy(tu_para[0],(BYTE *)TU_PARA_TBL[destno][CTU_FM],CTU_PARA_LEN);
      romcopy(tu_para[1],(BYTE *)TU_PARA_TBL[destno][CTU_AM],CTU_PARA_LEN);
      romcopy(&tu_chkcef[CTU_FM].sm_20db,(BYTE *)&TU_INI_CHKCOEF[0],sizeof(TU_INI_CHKCOEF));
#endif
   }


/************************************************************************/
/* ŠÖ”–¼ :void   Tu_aftermuteset(void)                           */
/* --------------------------------------------------------------------   */
/* ‹@”\   :   ±ÌÀMUTEÀ²Ï¾¯Äˆ—                                 */
/* Ú×   :                                                */
/************************************************************************/
void   Tu_aftermuteset(void)
{
   if (tu_updw_stus != CTU_NOTEDGE)
   {
      Tu_aftermute(CTU_AFTER300);
   }
   else
   {
      Tu_aftermute(CTU_AFTER100);
   }
}

#undef  _TU_MAIN_C_

#endif
