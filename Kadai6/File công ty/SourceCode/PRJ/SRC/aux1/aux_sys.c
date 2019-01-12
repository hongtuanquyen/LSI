/******************************************************************************
   ƒNƒ‰ƒŠƒIƒ““aŒü‚¯
   ƒvƒƒWƒFƒNƒg–¼   : 2012”NAMFM_RADIOƒ‚ƒfƒ‹
   ƒtƒ@ƒCƒ‹–¼      : aux_sys.c
   ‹@”\         : AUXƒ‚ƒWƒ…[ƒ‹SystemƒƒbƒZ[ƒWó•tˆ—
------------------------------------------------------------------------------
“ú•t      ’S“–Ò      C³“à—e
2012/07/26   “ŒŠC—Ñ      V‹Kì¬
******************************************************************************/
#define      _AUX_SYS_C_

#include   "../comm/common.h"
#include   "../main/ma_ext.h"
#include   "../power/pw_func_ext.h"
#include   "../audio/aud_ext.h"

#include   "aux_sys.h"

static void Aux_main(void);

/******************************************************************************
..       ŠÖ”–¼  Faux_sys_message
      ˆø  ”   FSystemƒƒbƒZ[ƒW
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FAUXSystemƒƒbƒZ[ƒWó•tˆ—
------------------------------------------------------------------------------
“ú•t      ’S“–Ò      C³“à—e
2012/07/26   “ŒŠC—Ñ      V‹Kì¬
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
..       ŠÖ”–¼  FAux_main
      ˆø  ”   F–³‚µ
      –ß‚è’l   F–³‚µ
      ‹@  ”\   FMAINˆ—
------------------------------------------------------------------------------
“ú•t      ’S“–Ò      C³“à—e
2012/07/26   “ŒŠC—Ñ      V‹Kì¬
2012/09/25   leejh      Audio Poweró‘Ô”»’f’Ç‰Á
******************************************************************************/
static void Aux_main(void)
{
   if(((Pw_Power_status_get() == ON) &&(Ma_current_mode() == CMA_AUX)) &&
      (Pw_ACC_DET_get() == ON)&&
      (Aud_get_power_status() == CAUD_PWR_ON))
   {
      Aud_set_prg_mute(CAUD_MUTE_AUX_NR_MUTE,OFF);   /* MuteOff   AUX chi hoat dong trong trang thai PWRON, khi OFF nó se ngung hoat dong*/
   }
   else
   {
      Aud_set_prg_mute(CAUD_MUTE_AUX_NR_MUTE,ON);      /* MuteOn */
   }
}

#undef  _AUX_SYS_C_
