/***************************************************************
   �N�����I���a���� 
   �v���W�F�N�g��   �F2012�NAMFM_RADIO����
   �t�@�C����      �Fsef_mode.c
   ���W���[����   �FSUB���[�h(Audio����)
   �@   �\         �FAudio��������
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/23      0.1      lrj      �V�K
****************************************************************/
#define   _SEF_MODE_C_

#include   "../model.h"

#include   "../comm/common.h"
#include   "../disp/lcd_ext.h"
#include   "../key/key_func_ext.h"
#include   "../main/ma_ext.h"
#include   "../audio/Aud_inc.h"

#include   "sbm_def.h"
#include   "sbm_ext.h"
#include   "sbm_sys.h"
#include   "sbm_keytbl.h"

/******************************************************************************
      �֐���  �FSef_lcddata_set
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h�\���ް��쐬����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void Sef_lcddata_set( TDP_SEL *auddisp )
{
   auddisp->mode = sbm_aud_mode;
   switch(sbm_aud_mode)
   {
      case CSBM_AUD_BAS:
         auddisp->value = Aud_get_bas_step();   // trong aud_ext.c
      break;
      case CSBM_AUD_TRE:
         auddisp->value = Aud_get_tre_step();
      break;
      case CSBM_AUD_BAL:
         auddisp->value = Aud_get_bal_step();
      break;
      default:
      break;
   }
   
}
/******************************************************************************
      �֐���  �FSef_mode_start
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h���鏈��
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void Sef_mode_start(void)
{
   Vol_mode_cancel();
   sbm_aud_mode = CSBM_AUD_BAS;
   sbm_aud_tim = CSBM_AUD_7S;
   Key_nop();
}
/******************************************************************************
      �֐���  �FSef_mode_start
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h�ؑ֏���
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void Sef_mode_chg(void)
{
   if( sbm_aud_mode == CSBM_AUD_BAS )
   {
      sbm_aud_mode = CSBM_AUD_TRE;
      sbm_aud_tim = CSBM_AUD_7S;
   }
   else if( sbm_aud_mode == CSBM_AUD_TRE )
   {
      if (Ma_model_dest() == CMA_MODEL_AM_J)
      {
         sbm_aud_mode = CSBM_AUD_OFF;
         sbm_aud_tim = 0;
      }
      else
      {
         sbm_aud_mode = CSBM_AUD_BAL;
         sbm_aud_tim = CSBM_AUD_7S;
      }
   }
   else if( sbm_aud_mode == CSBM_AUD_BAL )
   {
      sbm_aud_mode = CSBM_AUD_OFF;
      sbm_aud_tim = 0;
   }
   else
   {
      ;
   }
   Key_nop();
}
/******************************************************************************
      �֐���  �FSef_mode_start
      ��  ��   �F����
      �߂�l   �F   CSBM_AUD_OFF = 0,
               CSBM_AUD_BAS,
               CSBM_AUD_TRE,
               CSBM_AUD_BAL
      �@  �\   �FAudio�������[�h�����`�F�b�N����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
BYTE Sef_mode_check(void)
{
   return(sbm_aud_mode);
}
/******************************************************************************
      �֐���  �FSef_mode_start
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h��������
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void Sef_mode_cancel(void)
{
   sbm_aud_mode = CSBM_AUD_OFF;
   sbm_aud_tim = 0;
}
/******************************************************************************
      �֐���  �FSef_audio_updw
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h�AAudio�����l�ύX����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void Sef_audio_updw(BYTE req)
{
   sbm_aud_tim = CSBM_AUD_7S;
   
   switch(sbm_aud_mode)
   {
      case CSBM_AUD_BAS:
         Aud_set_bas_step(req);         /* BASS���� */
      break;
      case CSBM_AUD_TRE:
         Aud_set_tre_step(req);         /* TREBLE���� */
      break;
      case CSBM_AUD_BAL:
         Aud_set_bal_step(req);         /* BALANCE���� */
      break;
      default:
      break;
   }
}
/******************************************************************************
      �֐���  �FSef_key_power_long
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h�APOWER��LONG����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void Sef_key_pwr_long(void)
{
   if (Ma_get_clock() == TRUE )
   {
      Sef_mode_cancel();
      Clk_adj_start();
      Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
   }
   Key_nop();
}
/******************************************************************************
      �֐���  �FSef_key_up_off
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h�AUP��OFF����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void Sef_key_up_long(void)
{
   Sef_audio_updw(UP);
   Key_long_time200();
}
void Sef_key_up_off(void)
{
   Sef_audio_updw(UP);
   Key_nop();
}
/******************************************************************************
      �֐���  �FSef_key_dw_off
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h�ADOWN��OFF����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void Sef_key_dw_long(void)
{
   Sef_audio_updw(DW);
   Key_long_time200();
}
void Sef_key_dw_off(void)
{
   Sef_audio_updw(DW);
   Key_nop();
}
/******************************************************************************
      �֐���  �FSef_key_alm_long
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h�AALARM��LONG����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/24      0.1      lrj      �V�K
******************************************************************************/
void   Sef_key_alm_on(void)
{
   if (Ma_get_alarm() == TRUE )
   {
      Key_long_time2000();
   }
   else
   {
      Key_nop();
   }
}
void Sef_key_alm_long(void)
{
   Sef_mode_cancel();
   Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
   Alm_mode_start();
}

void Sef_key_md_off(void)
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
/******************************************************************************
      �֐���  �FSef_mode_initial
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAudio�������[�h�ϐ�����������
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/30      0.1      lrj      �V�K
******************************************************************************/
void Sef_mode_initial(void)
{
   sbm_aud_tim = 0;
   sbm_aud_mode = CSBM_AUD_OFF;
}

#if 0   /*�d�l���Ȃ����߁A�폜*/   /*_TEST_MODE_*/
void SEL_TestAudio_Edit(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length)
{
   Sef_mode_cancel();
   #if 0
   switch(RcvCmd)
   {
      case CTEST_MODE_DIR_VOLUME:
      case CTEST_MODE_BT_VOLUME_SET:
      break;
      case CTEST_MODE_RCH_MAX:
      case CTEST_MODE_LCH_MAX:
      case CTEST_MODE_BAL_CENTER:
         sbm_snd_mode = CSBM_SND_BAL;
         sbm_snd_tim = CSBM_SND_10S;      /* �ێ����10�b */
         Vol_mode_cancel();
      break;
      case CTEST_MODE_FFRONT_MAX:
      case CTEST_MODE_FREAR_MAX:
      case CTEST_MODE_FAD_CENTER:
         sbm_snd_mode = CSBM_SND_FAD;
         sbm_snd_tim = CSBM_SND_10S;      /* �ێ����10�b */
         Vol_mode_cancel();
      break;
      case CTEST_MODE_BASS_MAX:
      case CTEST_MODE_BASS_MIN:
         sbm_snd_mode = CSBM_SND_BAS;
         sbm_snd_bas  = CSBM_SND_BAS_G;
         fsbm_bas_adjust = ON;
         fsbm_mid_adjust = ON;
         fsbm_tre_adjust = ON;
         sbm_snd_tim = CSBM_SND_10S;      /* �ێ����10�b */
         Vol_mode_cancel();
      break;
      case CTEST_MODE_TREBLE_MAX:
      case CTEST_MODE_TREBLE_MIN:
         sbm_snd_mode = CSBM_SND_TRE;
         sbm_snd_bas  = CSBM_SND_TRE_G;
         fsbm_bas_adjust = ON;
         fsbm_mid_adjust = ON;
         fsbm_tre_adjust = ON;
         sbm_snd_tim = CSBM_SND_10S;      /* �ێ����10�b */
         Vol_mode_cancel();
      break;
      case CTEST_MODE_BASTRE_CENTER:
         sbm_snd_mode = CSBM_SND_BAS;
         sbm_snd_bas  = CSBM_SND_BAS_G;
         fsbm_bas_adjust = ON;
         fsbm_mid_adjust = ON;
         fsbm_tre_adjust = ON;
         sbm_snd_tim = CSBM_SND_10S;      /* �ێ����10�b */
         Vol_mode_cancel();
      break;
      case CTEST_MODE_MBEX:
         sbm_snd_mode = CSBM_SND_MABEX;
         sbm_snd_tim = CSBM_SND_10S;      /* �ێ����10�b */
         Vol_mode_cancel();
      break;
      case CTEST_MODE_BEQ:
         if(RcvBuf[0] == 0x00)
         {
            Lcd_1shot_disp(CLCD_FLAT,CLCD_1SHOT_2S);
            Aud_edit_flat();
         }
         else
         {
            sbm_snd_mode = CSBM_SND_ZEH;
            sbm_snd_tim = CSBM_SND_10S;      /* �ێ����10�b */
         }
         Vol_mode_cancel();
      break;
      case CTEST_MODE_SUBWOOFER:
         sbm_snd_mode = CSBM_SND_SUBLPF;
         sbm_snd_tim = CSBM_SND_10S;      /* �ێ����10�b */
         Vol_mode_cancel();
      break;
      default:
      break;
   
   }
   AUD_Test_Func(RcvCmd,RcvBuf,Length);
   #endif
}

#endif   /*_TEST_MODE_*/


#undef _CLOCK_MODE_C_
