/******************************************************************************
..     ƒNƒ‰ƒŠƒIƒ““aŒü‚¯                    2007ÓÃŞÙ

      ƒvƒƒWƒFƒNƒg–¼   :
      ƒtƒ@ƒCƒ‹–¼      : ma_mode.c
      ‹@”\         : Ò²İÓ°ÄŞŠÄ‹ˆ—
                  : ‡@Ò²İÓ°ÄŞØ‘ÖŠÄ‹ˆ—
                  : ‡AÓ°ÄŞØ‘ÖMUTE§Œäˆ—
------------------------------------------------------------------------------
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun070313]   •ÏX
******************************************************************************/
#define   _MA_MODE_C_

#include   "../model.h"
#include   "../comm/common.h"
#include   "../audio/aud_ext.h"
#include   "../power/pw_func_ext.h"
#include   "../tuner/tu_ext.h"

#if _MODEL_EEPROM_
#include   "../eeprom/eeprom_ext.h"
#endif

#include   "main.h"
#include   "ma_ext.h"
#include   "ma_mode.h"

/*-----------------------------------------------------------
          ŠÖ”ÌßÛÄÀ²Ìß
-----------------------------------------------------------*/
static BYTE Ma_next_mode_chg(TMA_MODE, BYTE);
static TMA_MODE Ma_search_next_md(TMA_MODE );
static TMA_MODE   Ma_lastmode_get(void);
static void Ma_mode_chgst(void);
static   void Ma_lastsave(TMA_MODE );

/*-----------------------------------------------------------
         Ò²İÓ°ÄŞğŒÃ°ÌŞÙ
[sun061114] USB IN‚ÅÓ°ÄŞØ‘Ö/PowerON‚µ‚È‚¢‚æ‚¤‚Éd—l•ÏX‘Î‰
[sun070410] RDSINT PowerOFF‚ÅÓ°ÄŞ‰ğœ‚·‚é‚æ‚¤‚É•ÏX
         RDSƒ‚ƒWƒ…[ƒ‹‚ÅPowerOFFËON‚Å‹­§Š„‚İó‘Ô‰ğœ‚µ‚Ä‚¢‚é‚½‚ßB
[sun100624] HDRadio‚Æ¼Ø³½Ó°ÄŞ’Ç‰Á
[sun100816] TunerMode’†HD@RadioÚ‘±ON‚ÅHD@Radio‚ÉØ‚è‘Ö‚¦‚µ‚È‚¢
         •s‹ï‡C³
-----------------------------------------------------------*/
#define      CMA_TBLEND   0xff
const   TMA_MODE_COND   ma_mdcond_tbl[] =
{
   /*                             LAST   ONInt   OffInt   PwrOn   PwrOff   OffKeep */
   {CMA_TUNER,      CMA_PRIO_LV0,      YES,   YES,   NO,      NO,      NO,      YES,},
   {CMA_AUX,      CMA_PRIO_LV0,      YES,   YES,   NO,      NO,      NO,      YES,},
   {CMA_ISR,      CMA_PRIO_LV1,      NO,      YES,   YES,   NO,      NO,      NO,},
   {CMA_TBLEND,   CMA_TBLEND,         NO,      NO,      NO,      NO,      NO,      NO,},
};

/******************************************************************************
..       ŠÖ”–¼  FMa_main
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FÒ²İºİÄÛ°Ùˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051129] ŠÖ”–¼‚Ì‚İ
[sun060710] PowerOFF’†Ó°ÄŞÁªİ¼ŞÒ¯¾°¼Ş”­s‚µ‚È‚¢‚æ‚¤‚É•ÏX
[sun070313]   •ÏX
[sun070524] ×½ÄÓ°ÄŞ‹L‰¯‚·‚é‚æ‚¤‚É•ÏX
******************************************************************************/
void   Ma_mode_main(void)
{
   if (fma_mdchg == ON)   // Neu mode thay doi. Duoc chuyen tu OFF -> ON khi nhan key MODE.
   {
      if(ma_mutetimer == TIMER_OUT)
      {  
         fma_mdchg = OFF;
//         ma_last_mode = ma_mode[CMA_CUR_MD].cond.mode;   /* ¶ÚİÄÓ°ÄŞ×½Ä‚É‹L‰¯ */
         ma_mode[CMA_CUR_MD] = ma_mode[CMA_TARGET_MD];  // mang ma_mode[] trong file ma_mode.h. Sau khi thuc hien ham Ma_next_mode_chg(), ta co ma_mode[CMA_TARGET_MD] moi
                                                        // Khi nay curmode = target mode.
         
         if (Pw_Power_status_get() == ON)
         {
            Ma_set_Msg(CMA_MODE,CMSG_MODECHG);   /* Ó°ÄŞØ‚è‘Ö‚¦Ò¯¾°¼Ş”­s   nam trong file ma_ext.c, chuc nang = Ma_message_set */
         }
         ma_mutetimer = CMA_AFTERMUTE;      /* Ó°ÄŞØ‚è‘Ö‚¦±ÌÀMUTEÀ²Ï¾¯Ä */
      }
   }
   else if ((ma_mutetimer == TIMER_OUT) &&
          (fma_mute == ON))
   {
      fma_mute = OFF;
      Aud_set_prg_mute(CAUD_MUTE_MAIN_DT_MUTE,OFF);
   }
   else
   {
      ;
   }
}

/******************************************************************************
..       ŠÖ”–¼  FMa_source_chg
      ˆø  ”   F‚È‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   F¿°½·°‚É‚æ‚èÓ°ÄŞØ‚è‘Ö‚¦
            F‡@¶ÚİÄÓ°ÄŞÁª¯¸
            F  ¶ÚİÄÓ°ÄŞ‚ªDMË×½ÄÓ°ÄŞ‚ÉØ‚è‘Ö‚¦
            F  ¶ÚİÄÓ°ÄŞ‚ª—DæÓ°ÄŞ Ë×½Äƒ‚[ƒh‚ÌØ‘Ö
            F  ¶ÚİÄÓ°ÄŞ‚ª’ÊíÓ°ÄŞ ËŸ‚ÌÓ°ÄŞ‚ÉØ‘Ö
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060123] V‹Kì¬
[sun060620] ±×°ÑÓ°ÄŞ’†TUNER‚ÉØ‘Ö‚½‚çATUNER‚ÖØ‘Ö‚·‚é‚æ‚¤‚É•ÏX
[sun070313]   •ÏX
[sun070402] ¶ÚİÄÓ°ÄŞ‚ªLV1‚Ìê‡‚ÍAÓ°ÄŞOFF‚µ×½ÄÓ°ÄŞ‚ÖØ‚è‘Ö‚¦
         ISRÓ°ÄŞ’†‚Ésrc·° Ë ISRÓ°ÄŞOFF‚µ‚ÄA×½ÄÓ°ÄŞ‚Ö‘JˆÚ
         RDSINTÓ°ÄŞ’†src·°Ë RDSINTÓ°ÄŞOFF‚µ‚ÄA×½ÄÓ°ÄŞ‚Ö‘JˆÚ
[sun070709] Å—DæÓ°ÄŞ[Œ»óTEL‚Ì‚İ]‚Å¿°½Ø‘Ö–³Œø‚É•ÏX
******************************************************************************/
void Ma_source_chg(void)
{
   TMA_MODE   cur_mode;
   TMA_MODE   next_mode;
   BYTE      iRet;

   cur_mode = ma_mode[CMA_TARGET_MD];

   /* Ø‘ÖŒ³Ó°ÄŞ¾¯Ä */
   if (cur_mode.cond.prio == CMA_PRIO_LV2)      /* [sun070709] Å—DæÓ°ÄŞ‚Å¿°½Ø‘Ö–³Œø */
   {
      return;
   }
   else if (cur_mode.cond.prio > CMA_PRIO_LV0)      /* ¶ÚİÄÓ°ÄŞ‚Í—DæÓ°ÄŞSRC·°‚Å×½ÄÓ°ÄŞ‚Ö‘JˆÚ */
   {
      ma_mode_cont[cur_mode.id] = CMA_NOCONN;   /* ¶ÚİÄÓ°ÄŞOFF‚É   cur_mode.id chi co 2 gia tri la 0(TUNER_ID) và 1(AUX_ID). 
                                                                   ma_mode_cont[] là mang 2 ptu.Xem file ma_mode.h và ma_ext.h*/
      next_mode = ma_mode[CMA_LAST_MD];      /* ×½ÄÓ°ÄŞ‚ÉØ‚è‘Ö‚¦ */
   }
   else  // truong hop chuyen qua lai 2 mode TUNER va AUX
   {
      next_mode = Ma_search_next_md(cur_mode); 
      Ma_modetbl_srh(&next_mode);
   }
   iRet = Ma_next_mode_chg(next_mode, CMA_PRIO_NO);

   if (iRet == OK)
   {
      Ma_mode_chgst();
   }
}

/******************************************************************************
..       ŠÖ”–¼  FMa_mode_pwroff
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FPowerOffÓ°ÄŞÁª¯¸ˆ—
            F¶ÚİÄÓ°ÄŞPowerOFF‚Å‰ğœ‚·‚é‚©Áª¯¸‚ğs‚¤ˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun070410] V‹Kì¬
[sun090403] EEPROM‚Ö×½ÄÓ°ÄŞ‘‚İ—v‹’Ç‰Á
[sun090409] ¶¼ÌÄ“ü—ÍĞ½C³
[sun090411] EEPROM‚Ö‚ÌŠi”[‚ÍDCP/ACC/BATT@OFF‚É•ÏX
******************************************************************************/
void Ma_mode_pwroff(void)
{
   TMA_MODE   cur_mode;
   
   cur_mode = ma_mode[CMA_TARGET_MD];

   /* PowerOFF‚Å‰ğœ‚·‚éÓ°ÄŞ‚©Áª¯¸    Check if mode is canceled with PowerOFF*/
   if(cur_mode.cond.off_keep == NO)
   {
      /* ¶ÚİÄÓ°ÄŞOFF²ÍŞİÄ”­s  */
      Ma_mode_event_chk(cur_mode.id,cur_mode.cond.mode,CMA_MODEOFF);
   }
}

/******************************************************************************
..       ŠÖ”–¼  FMa_mode_event_chk
      ˆø  ”   Fmodeid Ë Ó°ÄŞID”Ô†
            Fmode Ë Ó°ÄŞ
            Fevent Ë ²ÍŞİÄí—Ş
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FÓ°ÄŞ²ÍŞİÄÁª¯¸ˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060307] V‹Kì¬
[sun070313]   •ÏX
[sun070628] USB—L‚è–³‚µ”»’fŠÖ”‚É•ÏX
[sun100812] TunerMode’†HDRadio@ON‚Å‚ ‚ê‚ÎAHD@Radio‚Ö‘JˆÚ
         HD@Radio’†HD@Radio@OFF‚Å‚ ‚ê‚ÎATuner‚Ö‘JˆÚ
******************************************************************************/
void Ma_mode_event_chk(BYTE mode_id, BYTE mode, BYTE event)
{
   /* §ŒäÊŞ¯Ì§XV */
   switch(event)
   {
      case   CMA_CONN:
         ma_mode_cont[mode_id] = mode;
         break;
      case   CMA_MODEON:
         ma_mode_cont[mode_id] = mode;
         Ma_mode_on_event(mode_id,mode);
         break;
      case   CMA_MODEOFF:
         ma_mode_cont[mode_id] = CMA_NOCONN;
         Ma_mode_off_event(mode_id,mode);
         break;
      case   CMA_LOAD:
         ma_mode_cont[mode_id] = mode;
         Ma_mode_on_event(mode_id,mode);
         break;
      default:
         break;
   }
}
/******************************************************************************
..       ŠÖ”–¼  FMa_mode_on_event
      ˆø  ”   Fmodeid Ë Ó°ÄŞID”Ô†
            Fmode Ë Ó°ÄŞ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FÓ¼Ş­°ÙOn’Ê’mˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060307] V‹Kì¬
[sun060605] Ğ½1: On²ÍŞİÄIDŠù‚ÉÀ°¹Ş¯Ä‚É‚È‚Á‚Ä‚¢‚é‚©Áª¯¸’Ç‰Á
         ˜A‘±±×°ÑONATA ON‚ª—ˆ‚é‚Æ‰½‰ñ‚àó•t‚µ‚¿‚á‚¤•s‹ï‡
[sun060620] Ğ½2FMP3Ó°ÄŞ‚ÅLoading‚ÅPowerON‚Å‚«‚È‚¢•s‹ï‡
         “¯‚¶Ó°ÄŞ‚ÅPowerOnÁª¯¸‚µ‚Ä‚¢‚È‚©‚Á‚½B
[sun060719]-1 ¶ÚİÄÓ°ÄŞID‚ÆEventÓ°ÄŞID‚ª“¯‚¶‚Å•Ï‚í‚Á‚½Ó°ÄŞ‚Ì—v‹‚ªó•t–³‚¢•s‹ï‡C³
            ID”äŠr‚Å‚Í‚È‚­¶ÚİÄÓ°ÄŞ‚ÆeventÓ°ÄŞ‚Å”äŠr‚·‚é‚æ‚¤‚É•ÏX
[sun060913] USB Update’†MP3 LOAD‚ÅÓ°ÄŞ‘JˆÚ‚µ‚È‚¢‚æ‚¤•ÏX
[sun070313]   •ÏX
[sun090307] USBºİÊß²ÙSW’Ç‰Á
******************************************************************************/
void Ma_mode_on_event(BYTE   mode_id ,BYTE mode)
{
   TMA_MODE   event_mode;
   TMA_MODE   cur_mode;
   BYTE   iRet;

   event_mode.id = mode_id;
   event_mode.cond.mode = mode;
   cur_mode = ma_mode[CMA_TARGET_MD];

   Ma_modetbl_srh(&event_mode);               /* ²ÍŞİÄÓ°ÄŞÃ°ÌŞÙ»°Á */

   iRet = NG;
   if (event_mode.cond.onint != NO)            /* Ó°ÄŞON‚ÅØ‘Ö‚·‚é‚© */
   {
      if (cur_mode.cond.mode != event_mode.cond.mode)      /* [sun060719]-1¶ÚİÄÓ°ÄŞ‚Æ“¯‚¶‚© */
      {
         iRet = Ma_next_mode_chg(event_mode, CMA_PRIO_CHK);
      }
   }

   if (event_mode.cond.pwr_on)
   {
      Pw_power_event(mode, ON);
   }

   /* EventMode‚ÖØ‘Ö”­¶ */
   if (iRet == OK)
   {
      Ma_mode_chgst();
   }
}
/******************************************************************************
..       ŠÖ”–¼  FMa_mode_off_event
      ˆø  ”   Fmode_id Ë Ó°ÄŞID”Ô†
            Fmode Ë Ó°ÄŞ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FÓ¼Ş­°ÙOff’Ê’mˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060307] V‹Kì¬
[sun060719]-1 ¶ÚİÄÓ°ÄŞID‚ÆEventÓ°ÄŞID‚ª“¯‚¶‚Å•Ï‚í‚Á‚½Ó°ÄŞ‚Ì—v‹‚ªó•t–³‚¢•s‹ï‡C³
            ID”äŠr‚Å‚Í‚È‚­¶ÚİÄÓ°ÄŞ‚ÆeventÓ°ÄŞ‚Å”äŠr‚·‚é‚æ‚¤‚É•ÏX
[sun070313]   •ÏX
******************************************************************************/
void Ma_mode_off_event(BYTE   mode_id, BYTE mode)
{
   TMA_MODE   cur_mode;
   TMA_MODE   lst_mode;

   cur_mode = ma_mode[CMA_TARGET_MD];

   if ((cur_mode.cond.mode != mode) ||      /* [sun060719]-1 ¶ÚİÄÓ°ÄŞ‚Æ“¯‚¶‚© */
       (cur_mode.cond.offint == NO))      /* Ó°ÄŞOFF‚ÅØ‘Ö‚·‚é‚© */
   {
      return;                        /* XV‚Ì‚İ‚ÅSRCØ‘Ö‚µ‚È‚¢ */
   }

   lst_mode = Ma_lastmode_get();
   Ma_next_mode_chg(lst_mode,CMA_PRIO_NO);

   if (cur_mode.cond.pwr_off == YES)
   {
      Pw_power_event(cur_mode.cond.mode,OFF);
   }

   Ma_mode_chgst();
}
/******************************************************************************
..       ŠÖ”–¼  FMa_next_mode_chg
      ˆø  ”   Fnext_mode Ë –Ú•WÓ°ÄŞ
            Fstatus Ë —DæÚÍŞÙÁª¯¸•K—v‚©‚Ç‚¤‚©
      –ß‚è’l   FOK Ë –Ú•WÓ°ÄŞÁªİ¼Ş‚µ‚½
            FNG Ë –Ú•WÓ°ÄŞÁªİ¼Ş‚µ‚Ä‚¢‚È‚¢
      ‹@  ”\   F–Ú•WÓ°ÄŞ¾¯Äˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060307] V‹Kì¬
[sun060713] Tuner‚ÌTAóM’†—DæÚÍŞÙ1‚Éİ’è
[sun060802] ÚËŞ­°w“EC³ == Ë = ‚É•ÏX
         TUNER TAˆÈŠO‚Ìê‡‚ÍÚÍŞÙ‚ğLVO‚É‚µ‚Ä‚¢‚½AŒ³‚Ì‚Ü‚Ü‚Å•Û‚·‚é
[sun060930] TunerÓ°ÄŞ‚ÅTA—Dæ‡ˆÊ‚Æ‚È‚Á‚Ä‚µ‚Ü‚¤‚Æƒ\[ƒX·°•·‚©‚È‚­‚È‚é
[sun070313]   •ÏX
[sun070402] —DæÚÍŞÙ‚ªLV1‚ÅŸ‚ÌÓ°ÄŞ‚Ö‚Ì‘JˆÚ‚ª”­¶‚µ‚½‚çAÓ°ÄŞOFF‚Æ‚·‚é‚æ‚¤‚É
         •ÏX
         ISRÓ°ÄŞ’†‚ÉCD LoadingËISRÓ°ÄŞOFF‚µ‚ÄAMP3Ó°ÄŞ‚Ö
         RDSINTÓ°ÄŞ’†‚ÉCD LoadingËRDSINT OFF‚µ‚ÄAMP3Ó°ÄŞ‚Ö
[sun070525] TEL‚·‚Î‚â‚­u’f‚·‚é‚ÆATELØ‘Ö‚µ‚È‚¢‚ÅCD–ß‚é‚Ì‚ÅAReload‚µ‚Ä‚µ‚Ü‚¤
         À°¹Ş¯ÄXV‚µ‚½‚çA×½Ä‚à‘O‰ñÀ°¹Ş¯Ä‚ğ‹L‰¯‚·‚é‚æ‚¤‚É•ÏX
******************************************************************************/
static BYTE   Ma_next_mode_chg(TMA_MODE next_mode, BYTE status)
{
   BYTE   iRet;
   TMA_MODE   cur_mode;

   cur_mode = ma_mode[CMA_TARGET_MD];
   /* Ó°ÄŞØ‘ÖÁª¯¸ */
   iRet = NG;

   if ((status == CMA_PRIO_NO) ||                  /* —DæÁª¯¸–³‚µ   Khong kiem tra uu tien*/
      (next_mode.cond.prio >= cur_mode.cond.prio))      /* –Ú•WÓ°ÄŞ‚ª¶ÚİÄÓ°ÄŞ‚æ‚è—Dæ */
   {
      if (cur_mode.cond.lastsave == ON)
      {
         Ma_lastsave(cur_mode);   
      }
      iRet = OK;
   }
   else if (cur_mode.cond.prio == CMA_PRIO_LV1)      /* [sun070402] ¶ÚİÄÓ°ÄŞLV1uISR/RDSINTv */
   {
      ma_mode_cont[cur_mode.id] = CMA_NOCONN;         /* ¶ÚİÄÓ°ÄŞOFF‚É */
      iRet = OK;
   }
   else
   {
      if (next_mode.cond.lastsave == ON)
      {
         Ma_lastsave(next_mode);
      }
   }

   if (iRet == OK)
   {
      ma_last_mode = ma_mode[CMA_TARGET_MD].cond.mode;   /* ¶ÚİÄÓ°ÄŞ×½Ä‚É‹L‰¯ */
      ma_mode[CMA_TARGET_MD] = next_mode;   /* –Ú•WÓ°ÄŞ¾¯Ä */
   }
   return(iRet);
}

/******************************************************************************
..       ŠÖ”–¼  FMa_search_next_md
      ˆø  ”   Fcur_id Ë w’èid‚ÌŸ‚Ì—LŒøÓ°ÄŞ»°Á
            F*next_id Ë NEXTÓ°ÄŞŠi”[´Ø±
            F*next_mode_tbl Ë NEXTÓ°ÄŞÃ°ÌŞÙÃŞ°ÀŠi”[´Ø±
      –ß‚è’l   F
      ‹@  ”\   F¿°½·°‚É‚æ‚èŸ‚ÌØ‘ÖÓ°ÄŞ»°Áˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060307] V‹Kì¬
******************************************************************************/
static TMA_MODE Ma_search_next_md(TMA_MODE cur_mode)
{
   BYTE   iLoop;
   TMA_MODE   next_mode;

   if (cur_mode.id >= CMA_SRC_MAX)   // CMA_SRC_MAX = CMA_ID_SRCMAX +1 = 2. 
                                     // cur_mode.id chi co 2 gia tri 0(TUNER_ID) và 1(AUX_ID). Neu bang 2 thiet se sai.
   {
      next_mode.id = 0;            /* w’è•s³‚ÅA‹­§æ“ª‚ğ¾¯Ä  Xac dinh ko hop le, bat buoc thiet lap lai ban dau*/
      next_mode.cond.mode = ma_mode_cont[0]; // ma_mode_cont[0] = ma_mode_cont[CMA_ID_TU] 
      return(next_mode);
   }

   iLoop = cur_mode.id;
   iLoop++;
   while (iLoop != cur_mode.id)
   {
      if (iLoop >= CMA_SRC_MAX)
      {
         iLoop = 0;
      }
      if (ma_mode_cont[iLoop] != CMA_NOCONN)
      {
         break;
      }
      iLoop ++;
   }

   if (iLoop == cur_mode.id)
   {
      iLoop = 0;               /* nextŒ©•t‚ç‚È‚¢Aæ“ª‚ğ¾¯Ä  next mode khong tim thay, thiet lap nhu ban dau*/
   }

   next_mode.id = iLoop;
   next_mode.cond.mode = ma_mode_cont[iLoop];
   return(next_mode);
}

/******************************************************************************
..       ŠÖ”–¼  FMa_modetbl_srh
      ˆø  ”   Fmode Ë »°Á–Ú•WÓ°ÄŞ                    Argument: mode => search target mode
            F* iTblCnt Ë Ã°ÌŞÙ”Ô†Ši”[•Ï”±ÄŞÚ½        * ITblCnt => table number storage variable address
            F* mode_tblË Ã°ÌŞÙÃŞ°ÀŠi”[•Ï”´Ø±         * Mode_tbl => table data storage variable area
      –ß‚è’l   FOK Ë Œ©‚Â‚©‚Á‚½                      Return value: OK => found
            FNG Ë Œ©‚Â‚©‚ç‚È‚©‚Á‚½                    NG => I could not find it
      ‹@  ”\   FÓ°ÄŞÃ°ÌŞÙ»°Áˆ—                       Mode table search processing
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060307] V‹Kì¬
******************************************************************************/
void Ma_modetbl_srh( TMA_MODE *mode_tbl )
{
   BYTE   iRet;
   BYTE   iLoop1;
   BYTE   mode;

   mode = mode_tbl->cond.mode;

   iRet = NG;
   /* ¶ÚİÄÓ°ÄŞ»°Á  Current mode search*/
   iLoop1 = 0;
   while (ma_mdcond_tbl[iLoop1].mode != CMA_TBLEND)  // ma_mdcond_tbl trong file ma_mode.c
   {
      if (ma_mdcond_tbl[iLoop1].mode == mode)
      {
         mode_tbl->cond = ma_mdcond_tbl[iLoop1];
         iRet = OK;
         break;
      }
      iLoop1 ++;
   }
   /* Ã°ÌŞÙ»°Á‚Å‚«‚È‚©‚Á‚½ê‡‚ÍAæ“ªÃŞ°À‚ğİ’è */
   if (iRet == NG)
   {
      mode_tbl->id = 0;
      mode_tbl->cond = ma_mdcond_tbl[0];
   }
}

/******************************************************************************
..       ŠÖ”–¼  FMa_lastmode_get
      ˆø  ”   F‚È‚µ
      –ß‚è’l   F×½ÄÓ°ÄŞ
      ‹@  ”\   F×½ÄÓ°ÄŞ»°Áˆ—ˆ—
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060308] V‹Kì¬
******************************************************************************/
static TMA_MODE   Ma_lastmode_get(void)
{
   TMA_MODE   lastmode;
   /* QAC‘Îô */
   lastmode.id = ma_mode[CMA_LAST_MD].id;
   lastmode.cond.mode = ma_mode_cont[lastmode.id];
   Ma_modetbl_srh(&lastmode);
   return(lastmode);
}

/******************************************************************************
..       ŠÖ”–¼  FMa_mode_chgst
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FÓ°ÄŞØ‘Ö½À°Äˆ—      Mode switching start processing
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060308] V‹Kì¬
[sun060509] Ó°ÄŞÁªİ¼ŞMUTE‰ğœ‘Ò‚¿ŠÔ‚ÉMUTEÀ²ÏXV‚µ‚Ä‚¢‚È‚¢‚Ì‚ÅA‰ğœÀ²ÏØ‚ê‚é‚Ü‚Å
         Ó°ÄŞØ‘Ö‚µ‚È‚­‚È‚é•s‹ï‡C³
[sun070822] TunerŠÔ‚Å‚ÌØ‘Ö‚ÍMute‚©‚¯‚È‚¢‚æ‚¤‚ÉC³
[sun101216] ModeØ‘Ö”­¶‚·‚é‘O‚ÉBT FastÄ¶‰ğœRequest
******************************************************************************/
static void Ma_mode_chgst(void)
{
   if (fma_mdchg == OFF)
   {
      if (Pw_Power_status_get() == ON)
      {
         if (fma_mute == OFF)
         {
            ma_mutetimer = CMA_FRONTMUTE;
         }
         else
         {
            ma_mutetimer = CMA_FRONTMUTE_50;
         }
      }
      else
      {
         ma_mutetimer = 0;
      }
   }

   fma_mdchg = ON;
   fma_mute = ON;
   Aud_set_prg_mute(CAUD_MUTE_MAIN_DT_MUTE,ON);
}

/******************************************************************************
..       ŠÖ”–¼  FMa_lastsave
      ˆø  ”   F‚È‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   F×½ÄÓ°ÄŞ¾¯Ä
      C³—š—ğ   y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060308] V‹Kì¬
******************************************************************************/
static   void Ma_lastsave(TMA_MODE lst_mode)
{
   BYTE   iLoop1;
   BYTE   iLoop2;

   /* ×½ÄÓ°ÄŞ‚Æ“¯‚¶‚©Áª¯¸ */
   if (ma_mode[CMA_LAST_MD].id == lst_mode.id)
   {
      if (ma_mode[CMA_LAST_MD].cond.mode != lst_mode.cond.mode)
      {
         ma_mode[CMA_LAST_MD].cond = lst_mode.cond;
      }
      return;
   }

   /* “¯‚¶Ó°ÄŞ×½Ä‚É“ü‚Á‚Ä‚ ‚é‚©Áª¯¸ */
   for (iLoop1 = CMA_LAST_MD; iLoop1 < CMA_MAX; iLoop1 ++)
   {
      if (lst_mode.id == ma_mode[iLoop1].id)
      {
         break;
      }
   }
   if (iLoop1 >= CMA_MAX)
   {
      iLoop1 = (CMA_MAX - 1);
   }
   /* ×½Ä“ü‚ê‘Ö‚¦ */
   for (iLoop2 = iLoop1-1; iLoop2 >= CMA_LAST_MD; iLoop2--,iLoop1--)
   {
      ma_mode[iLoop1] = ma_mode[iLoop2];
   }
   /* ˆø”Ó°ÄŞ×½Ä‚É“ü‚ê‘Ö‚¦ */
   ma_mode[CMA_LAST_MD] = lst_mode;
   
}


#undef      _MA_MODE_C_
