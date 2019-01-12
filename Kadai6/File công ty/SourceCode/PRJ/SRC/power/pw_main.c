/******************************************************************************
   ƒNƒ‰ƒŠƒIƒ“iŠ”j“aŒü‚¯      

      ƒvƒƒWƒFƒNƒg–¼   : AMFMƒ‰ƒWƒI
      ƒtƒ@ƒCƒ‹–¼      : pw_main.c
      ‹@”\         : “dŒ¹ƒ‚[ƒh‘JˆÚˆ—
------------------------------------------------------------------------------
C³—š—ğ
[chin20120725] ‰ü‘¢
******************************************************************************/
#define   _PW_MAIN_C_

#include   "../comm/common.h"
#include   "../key/key_func_ext.h"
#include   "../io/IO_inc.h"
#include   "../main/ma_ext.h"
#include   "../audio/aud_ext.h"
#include   "../disp/lcd_ext.h"
#include   "../submode/sbm_ext.h"

#include   "pw_def.h"
#include   "pw_func_ext.h"
#include   "pw_main.h"

/*-----------------------------------------------------------------------
    ŠÖ”éŒ¾
------------------------------------------------------------------------*/
static   void   Pw_set_item(void);
static   void   Pw_set_targetmode(void);
#if _POWER_TEST_COLDS_
static   BYTE   Pw_ColdS(void);
#endif
static   BYTE   Pw_HoldX(void);
static   BYTE   Pw_HoldX_In(void);
static   BYTE   Pw_HoldX_Out(void);
static   BYTE   Pw_AccOff(void);
static   BYTE   Pw_AccOff_In(void);
static   BYTE   Pw_AccOff_Out(void);
static   BYTE   Pw_PwrOff(void);
static   BYTE   Pw_PwrOn_Out(void);
static   BYTE   Pw_PwrOn_In(void);
static   BYTE   Pw_PwrOn(void);
static   BYTE   Pw_holdX_in_check(void);

/*-----------------------------------------------------------------------
   ITEMİ’èÃ°ÌŞÙ
------------------------------------------------------------------------*/

const BYTE tpw_item_tbl[4][4] = {
/* ¶ÚİÄÓ°ÄŞ   HOLDX’†        ACCOFF’†            POWER OFF          Power ON’†      */   /* –Ú•WÓ°ÄŞ  Tu tren xuong la target mode và tu trai sang la curmode*/
   {   CPW_I_HOLDX,      CPW_I_HOLDX_IN,      CPW_I_ACCOFF_IN,   CPW_I_PWRON_OUT   },   /* HOLDX */
   {   CPW_I_HOLDX_OUT,   CPW_I_ACCOFF,      CPW_I_ACCOFF_IN,   CPW_I_PWRON_OUT   },   /* ACCOFF */
   {   CPW_I_HOLDX_OUT,   CPW_I_ACCOFF_OUT,   CPW_I_PWROFF,      CPW_I_PWRON_OUT   },   /* PowerOff */
   {   CPW_I_HOLDX_OUT,   CPW_I_ACCOFF_OUT,   CPW_I_PWRON_IN,      CPW_I_PWRON      }   /* PowerOn */
};

/*-----------------------------------------------------------------------
   “dŒ¹ˆ—ITEM Ã°ÌŞÙ
šš’ˆÓF tpw_item_tbl‚Åİ’è‚³‚ê‚é’l‚Æ˜A“®‚·‚éšš
------------------------------------------------------------------------*/
typedef struct{
   BYTE   item;
   BYTE   (*func)(void);
}TPW_ITEM;

const  TPW_ITEM tpw_item[] =
{
      CPW_I_HOLDX,      Pw_HoldX
   ,   CPW_I_HOLDX_IN,      Pw_HoldX_In
   ,   CPW_I_HOLDX_OUT,   Pw_HoldX_Out
   ,   CPW_I_ACCOFF,      Pw_AccOff
   ,   CPW_I_ACCOFF_IN,   Pw_AccOff_In
   ,   CPW_I_ACCOFF_OUT,   Pw_AccOff_Out
   ,   CPW_I_PWROFF,      Pw_PwrOff
   ,   CPW_I_PWRON_OUT,   Pw_PwrOn_Out
   ,   CPW_I_PWRON_IN,      Pw_PwrOn_In
   ,   CPW_I_PWRON,      Pw_PwrOn
      #if _POWER_TEST_COLDS_
   ,   CPW_I_COLDS,      Pw_ColdS         /* Cold Startˆ— */
      #endif
};

/******************************************************************************
   ŠÖ ” –¼   Fpw_main
   ˆø   ”      F–³‚µ
   –ß ‚è ’l   F–³‚µ
   ‹@ ”\ ŠT —ª   F“dŒ¹§ŒäÓ¼Ş­°Ù ó‘ÔŠÄ‹Ò²İˆ—
------------------------------------------------------------------------------
C³—š—ğ
y”Ô†z   “ú•t      ’S“–Ò      C³“à—e
[sun060401]V‹Kì¬
******************************************************************************/
void   Pw_main(void)
{
   const   TPW_ITEM *pwitem;
   BYTE    (*pwfunc)(void);
   BYTE   work;

   do
   {
      #if _WDT_
      Io_wdt_clear();
      #endif
      
      /* À°¹Ş¯ÄÓ°ÄŞİ’èˆ— */
      Pw_set_targetmode();
      /* “®ìITEMİ’èˆ— */
      Pw_set_item();
      /* ITEMˆ—ŒÄoˆ— */
      pwitem = (const TPW_ITEM*)(tpw_item + pw_item);
      pwfunc = (pwitem->func);
      work = (*pwfunc)();
   }while(work == CPW_SEQCONT);  //CPW_SEQCONT = 1
   
   #if _POWER_DIAG_
   Pw_DIAG_check(CPW_DIAG_MD_MAIN);
   #endif   /* _POWER_DIAG_ */
}
/******************************************************************************
   ŠÖ ” –¼   FPw_set_targetmode
   ˆø   ”      F–³‚µ
   –ß ‚è ’l   F–³‚µ
   ‹@ ”\ ŠT —ª   F“dŒ¹–Ú•Wó‘Ôİ’èˆ—
            FBATT/ACC/PANELAPower·°‚È‚Ç‚Ìó‘Ô‚ğ‚ ‚í‚¹‚Ä“dŒ¹–Ú•W‚ğ¾¯Ä
------------------------------------------------------------------------------
C³—š—ğF
[sun070321] •ÏX
[sun070323] ACC OFF’†ILL-DET‚ª‚ ‚ê‚ÎAHALT‚©‚ço‚é‚æ‚¤‚É•ÏX
[sun100616] ILL ON‚ÅCTRL’[q‚ğHIGH‚É‚µ‚È‚¢AKEY ILL/BlinkLED‚Ì‚İ§Œä‚Ì‚½‚ßA
         CPW_DISPOFF‚É‚µ‚È‚¢‚æ‚¤‚É•ÏX
******************************************************************************/
static   void   Pw_set_targetmode(void)
{
   BYTE   tarmode;
   
   #if 1
   if (fpw_wakeup == TRUE)
   {
      tarmode = CPW_PWRON;
   }
   else
   if ((fpw_acc == ON)||(Alm_mode_check() == TRUE)) // check 2 TH là ACCON hoac ACCOFF nhung dang dieu chinh ALARM
   {
      if (fpw_power_key == ON)
      {
         tarmode = CPW_PWRON;      /* À°¹Ş¯ÄÓ°ÄŞ‚ğPowerOn‚Ö */
      }
      else
      {
         tarmode = CPW_PWROFF;      /* À°¹Ş¯ÄÓ°ÄŞ‚ğPOWER OFF‚Ö */
      }
   }
   else
   {
      #if _HOLDX_
      if (( fpw_acc_nor == OFF)
#if 0 /* g—p‚µ‚È‚¢‚½‚ßAíœ */
        &&( pw_accwait_timer == TIMER_OUT)
#endif
        &&( fpw_accdet_active == OFF)
        &&( fpw_almdet_active == OFF))
      {
         tarmode = CPW_HOLDX;      /* À°¹Ş¯ÄÓ°ÄŞ‚ğHOLDX‚Ö */
      }
      else
      #endif
      {
         tarmode = CPW_ACCOFF;      /* À°¹Ş¯ÄÓ°ÄŞ‚ğAccOFF’†‚Ì’Êíƒ‚[ƒh‚Ö */
      }
   }
   #else
   tarmode = CPW_ACCOFF;
   #endif
   
   /* À°¹Ş¯ÄÓ°ÄŞÁªİ¼Ş‚ ‚è */
   #if 1
   if ((tarmode != pw_tarmode) &&      /* À°¹Ş¯ÄÓ°ÄŞ•Ï‰»‚ ‚è */
           (fpw_itemchg_enb == ON))      /* ITEMŠ®—¹‘Ò‚¿’† */
   #else
   if (tarmode != pw_tarmode)      /* À°¹Ş¯ÄÓ°ÄŞ•Ï‰»‚ ‚è */
   #endif
   {
      pw_tarmode = tarmode;
      pw_power_seq = CPW_SEQEND;
   }
   
   #if _WDT_TEST_
   while(1)
   {
      ;
   }
   #endif
}
/******************************************************************************
   ŠÖ ” –¼   FPw_set_item
   ˆø   ”      F–³‚µ
   –ß ‚è ’l   F–³‚µ
   ‹@ ”\ ŠT —ª   F“dŒ¹ÀsITEMˆ—NO‚ğ¾¯Äˆ—
------------------------------------------------------------------------------
C³—š—ğF
[sun060401]V‹Kì¬
******************************************************************************/
static   void   Pw_set_item(void)
{
   BYTE   item;

   #if _POWER_TEST_COLDS_
   if(/*ColdStart*/)
   {
      pw_item = CPW_COLDS;
   }
   else
   #endif
   /* BATT OFFˆÈŠO‚Ìê‡‚ÍITEMÁªİ¼Ş‹–‰Â‚ ‚ê‚ÎITEMİ’è‚Ö */
   if (pw_power_seq == CPW_SEQEND)
   {
      fpw_itemchg_enb = OFF;
      pw_item = tpw_item_tbl[pw_tarmode][pw_curmode];
      pw_power_seq = CPW_SEQST; // CPW_SEQST = 1
   }
   
}


#if _POWER_TEST_COLDS_
/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_ColdS
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FPw_ColdS‚Ìˆ—
------------------------------------------------------------------------------
[chin20120803]V‹Kì¬
-----------------------------------------------------------------------------*/
static   BYTE   Pw_ColdS(void)
{
   //ƒV[ƒPƒ“ƒX‚ª•K—v‚©‚Ç‚¤‚©
}
#endif

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_HoldX
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FHoldXÓ°ÄŞ’†ˆ—
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
static   BYTE Pw_HoldX(void)
{
   fpw_acc_nor = ON;
   pw_power_seq = CPW_SEQEND;
   fpw_itemchg_enb = ON;
   return(CPW_SEQCONT);
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_HoldX_In
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FHoldXƒ‚[ƒh‚É‘JˆÚˆ—
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
/* ACC OFF(’Êí) ¨ HOLDX */
static   BYTE Pw_HoldX_In(void)
{
   #if 1
   switch(pw_power_seq)
   {
      case 1:
         Ma_set_Msg( CMA_POWER, CMSG_HOLDX_IN );
         pw_power_seq ++;
         break;
      case 2:
         pw_curmode = CPW_HOLDX;
         pw_power_seq = CPW_SEQEND;
         fpw_almdet_active = OFF;
         if (IO_In_acc_det() == HIGH)
         {
            Io_port_holdx_in();
            Io_int_holdx_in();
            Io_sfr_holdx_in();
         }
         else
         {
            fpw_acc_nor = ON;
            fpw_accdet_active = ON;
         }
         fpw_itemchg_enb = ON;
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
   #else
   Ma_set_Msg( CMA_POWER, CMSG_HOLDX_IN );
   Pw_almkey_clear();
   pw_curmode = CPW_HOLDX;
   pw_power_seq = CPW_SEQEND;
   Io_int_holdx_in();
   Io_sfr_holdx_in();
   #endif
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_HoldX_Out
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FHoldXƒ‚[ƒh‚©‚ç‚Ê‚¯‚éˆ—iPowerOff‚É‘JˆÚj
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
/* HOLDX ¨ ACC OFF(’Êí) */
static   BYTE Pw_HoldX_Out(void)
{
   #if 0
   switch(pw_power_seq)
   {
      case 1:
         pw_power_seq ++;
         //Io_int_holdx_out();
         //Io_sfr_holdx_out();
         break;
      case 2:
         Ma_set_Msg( CMA_POWER, CMSG_HOLDX_OUT );
         pw_curmode = CPW_ACCOFF;
         pw_power_seq = CPW_SEQEND;
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
   #else
   #if 0 /* Š„‚è‚İˆ—‚ÅÀ{‚·‚é‚½‚ßAíœ */
   Io_int_holdx_out();
   Io_sfr_holdx_out();
   Io_port_holdx_out();
   #endif
   
   Ma_set_Msg( CMA_POWER, CMSG_HOLDX_OUT );
   pw_curmode = CPW_ACCOFF;
   pw_power_seq = CPW_SEQEND;
   fpw_itemchg_enb = ON;
   return(CPW_RETMAIN);
   #endif
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_AccOff
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FAccOff’†‚Ì’Êíó‘Ôˆ—
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
static   BYTE Pw_AccOff(void)
{
   fpw_itemchg_enb = ON;
   if ((fpw_accdet_active == ON)||(fpw_almdet_active == ON))
   {
      return (CPW_RETMAIN);
   }
   
   #if 1
   if (Pw_holdX_in_check() == TRUE )
   {
      fpw_acc_nor = OFF;      /* holdX‚ÖˆÚs—v‹ */
   }
   #endif
   return (CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_AccOff_In
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FAccOff’†‚Ì’Êíó‘Ô‚É‘JˆÚˆ—
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
/* ACC ON(POWER OFF) ¨ ACC OFF(’Êí) */
static   BYTE Pw_AccOff_In(void)
{
   switch(pw_power_seq)
   {
      case 1:
         Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_OFFREQ );
         pw_seq_timer = (30+1);  // Quét phim 30ms, giam dan khi ham Ma_tim_int_1ms xay ra(do timer interrupt), no se goi ham Pw_1ms_timer de tru 1 don vi.
         pw_power_seq ++;
         break;
      case 2:
         if (pw_seq_timer == 0)
         {
            MIO_OUT_pSYS_ON(LOW);
            IO_SetIICPort(OFF);
            pw_curmode = CPW_ACCOFF;
            pw_power_seq = CPW_SEQEND;
            Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_OFF );
            Key_nop();
            Key_check_start();
            Io_port_accoff_in();
            Io_int_accoff_in();
            Io_sfr_accoff_in();
         }
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_AccOff_Out
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FAccOff’†‚Ì’Êíó‘Ô‚©‚ç‚Ê‚¯‚éˆ—iHoldXƒ‚[ƒh‚É‘JˆÚj
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
/* ACC OFF(’Êí) ¨ ACC ON(POWER OFF) */
static   BYTE Pw_AccOff_Out(void)
{
   switch(pw_power_seq)
   {
      case 1:
         Io_int_accoff_out();
         Io_sfr_accoff_out();
         pw_power_seq ++;
         break;
      case 2:
         MIO_OUT_pSYS_ON(HIGH);            /* SYS_ON */
         IO_SetIICPort(ON);
         Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_ON );
         pw_power_seq = CPW_SEQEND;
         pw_curmode = CPW_PWROFF; // Khi ACC_OFF chuyen sang ACC ON, ko can biet Power ON hay OFF, no van chuyen ve mode Power OFF. Khi wa hàm Lcd_mode() trong 
                                  // file lcd_mode.c, no se kiem tra current mode, khi nay la POWER OFF, no se chi hien thi thoi gian.
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);  // CPW_RETMAIN = 0
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_PwrOff
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FPowerOFFƒ‚[ƒh’†ˆ—
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
static   BYTE Pw_PwrOff(void)
{
   #if 0
   accoff_check
   if ()   /* AccOff’† */
   {
      ifI(/* ƒAƒ‰[ƒ€BEEP–Â“®’†||ƒAƒ‰[ƒ€’²®’†) */)
      {
         
      }
   }
   #endif
   
   fpw_itemchg_enb = ON;
   pw_power_seq = CPW_SEQEND;
   Key_check_start();  // trong file key_func_ext.c
   return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_PwrOn_Out
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FPowerONƒ‚[ƒh‚©‚ç‚Ê‚¯‚é’†ˆ—iPowerOff‚É‘JˆÚj
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
static   BYTE Pw_PwrOn_Out(void)
{
   switch(pw_power_seq)
   {
      case   1:
         #if 0
         if (Aud_get_beep_type() == CAUD_BEEP_STA_OFF)
         #endif
         {
            fpw_itemchg_enb = OFF;
            fpw_mute = ON;
            Aud_set_prg_mute(CAUD_MUTE_PWR_DT_MUTE,ON);
            pw_seq_timer = (10+1);
            Key_check_stop();
            pw_power_seq++;
         }
         break;
      case   2:
         if (pw_seq_timer == 0x00)
         {
            Ma_set_Msg( CMA_POWER, CMSG_PWROFF );
            Key_check_start();
            MIO_OUT_pSTBY(LOW);
            pw_curmode = CPW_PWROFF;   // Xet qua trinh chuyen tu PWRON(ACCON)->ACCOFF, tuc la tarmode = ACCOFF. Nhung vi curmode ko fai la ACCOFF(tarmode
                                       // ma minh muon nham toi) ma la PWROFF, nen khi nay tarmode va curmode khac nhau, dan den la sau khi quay lai Pw_main()
                                       // ta lai xet toi qua trinh chuyen tu PWROFF -> ACCOFF.Mot lan nua no se nhay sang ham Pw_AccOff_In().
            pw_power_seq = CPW_SEQEND;
            fpw_itemchg_enb = ON;
         }
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_PwrOn_In
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FPowerONƒ‚[ƒh‚É‘JˆÚˆ—
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
”õlF   PowerON‚ğŠm’è‚µ‚Ä‚©‚çA50msŒã•\¦XV‚·‚é‚½‚ßA•\¦‚ÌƒCƒ“ƒ^[ƒtƒF[ƒX‚ª•K—v
      
-----------------------------------------------------------------------------*/
static   BYTE Pw_PwrOn_In(void)
{
   switch(pw_power_seq)
   {
      case   1:
         Ma_set_Msg( CMA_POWER, CMSG_PWRON );
         Key_check_stop();
         pw_seq_timer = (50+1);
         pw_power_seq++;
         break;
      case   2:
         if (pw_seq_timer == 0)
         {
            MIO_OUT_pSTBY(HIGH);
            #if _POWER_DIAG_
            Pw_DIAG_clear();
            #endif   /* _POWER_DIAG_ */
            if (fpw_wakeup == TRUE)
            {
               Aud_set_beep(CAUD_REQ_BEEP_ALARM);
            }
            Key_check_start();
            Aud_set_prg_mute(CAUD_MUTE_PWR_DA_MUTE,OFF);
            Aud_set_prg_mute(CAUD_MUTE_PWR_DT_MUTE,OFF);
            pw_power_seq = CPW_SEQEND;
            pw_curmode = CPW_PWRON;
         }
         break;
      case   3:
      
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       ŠÖ”–¼  FPw_PwrOn
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FPowerON’†ˆ—
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
-----------------------------------------------------------------------------*/
static   BYTE Pw_PwrOn(void)
{
   pw_power_seq = CPW_SEQEND;
   Key_check_start();
   fpw_itemchg_enb = ON;
   return(CPW_RETMAIN);
}

/******************************************************************************
   ŠÖ”–¼   Fpw_initial
   ˆø  ”   F–³‚µ
   –ß‚è’l   F–³‚µ
   ‹@  ”\   F“dŒ¹§ŒäÓ¼Ş­°Ù‰Šú‰»ˆ—
------------------------------------------------------------------------------
[chin20120725]V‹Kì¬
”õlF   ƒVƒXƒeƒ€ƒƒbƒZ[ƒW‚Í”­s‚µ‚Ü‚¹‚ñB
      HoldXˆÀ’èó‘Ô‚É‚È‚é‚Ü‚ÅAACC_DET‹Ö~iƒtƒ‰ƒO‚©AŠÔ‚ğ‘Ò‚Â‚©j
      IOƒ‚ƒWƒ…[ƒ‹‚ÌƒCƒ“ƒ^[ƒtƒF[ƒX‚ğŒÄ‚Ño‚·iColdStart‚ÌŠÖ”j
******************************************************************************/
void   Pw_initial(void)
{
   /* ACCOFFƒ‚[ƒh‚É‚µ‚Ä‚¨‚­*/
   pw_curmode = CPW_ACCOFF;      /* AccOffÓ°ÄŞ‚©‚ç½À°Ä   Bat dau tu che do AccOff  */
   
   #if 0
   Io_int_initial_set();
   #endif
   pw_power_seq = CPW_SEQEND;      // CPW_SEQEND = 0 
   fpw_acc_nor = ON;            // fpw_acc_nor trong file pw_main.h
   fpw_accdet_active = ON;      /* Accƒ`ƒƒƒ^ƒŠƒ“ƒO‚ğs‚¤ */
   Lcd_accoff_send_req();   // trong file lcd_ext.c, duoc su dung trong file lcd_ext.c
   #if 0 /* g—p‚µ‚È‚¢‚½‚ßAíœ  */
   pw_accwait_timer = CPW_ACCWATI_TIM;      /* ACCÁ¬ÀØİ¸Ş’†HALT‚Ö‘JˆÚ‚µ‚È‚¢‚æ‚¤‚É   Khong the chuyen sang HALT trong khi chat voi ACC*/
   #endif
   fpw_itemchg_enb = ON;  // trong file pw_main.c, hàm Pw_targetmode  
   
   pw_AccOn_4mscnt = 0;
   pw_accon_chat_cnt = 0;
   pw_accoff_chat_cnt = 0;
   pw_almkey_cnt = 0;
}

/******************************************************************************
   ŠÖ”–¼   FPw_holdX_in_check
   ˆø  ”   F–³‚µ
   –ß‚è’l   FTRUE ¨   HOLDX‚É‘JˆÚ
         FFALSE¨   HOLDX‚É‘JˆÚ‚µ‚È‚¢
   ‹@  ”\   Faccoffó‘Ô‚©‚çHOLDX‚És‚­‚©‚Ç‚¤‚©‚Ìƒ`ƒFƒbƒNˆ—
------------------------------------------------------------------------------
[chin20120827]V‹Kì¬
******************************************************************************/
static   BYTE   Pw_holdX_in_check(void)
{
   BYTE ret;
   
   ret = TRUE;
   
   #if 0
   if( Alm_mode_check() == TRUE )
   {
      ret = FALSE;         /* ƒAƒ‰[ƒ€’²®’†‚Å‚Í‚È‚¢ */
   }
   #endif
   
   if (Lcd_accoff_send_end() == TRUE )
   {
      ret = FALSE;         /* •\¦XVÏ‚İ */
   }
   return ret;
}
#undef  _PW_MAIN_C_
