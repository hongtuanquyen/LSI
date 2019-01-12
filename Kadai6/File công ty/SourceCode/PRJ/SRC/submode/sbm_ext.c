/*************************************************************************
   �N�����I���a���� 
   �v���W�F�N�g��   �F2012�NAMFM_RADIO����
   �t�@�C����      �Fsbm_ext.c
   ���W���[����   �FSUB���[�h
   �@   �\         �F�C���^�t�F�[�X�֐��t�@�C��
--------------------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
--------------------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
*************************************************************************/
#define      _SBM_EXT_C_

#include   "../model.h"

#include   "../comm/common.h"
#include   "../key/key_func_ext.h"
#include   "../audio/aud_ext.h"
#include   "../key/key_def.h"
#include   "../main/ma_ext.h"
#include   "../power/pw_func_ext.h"
#include   "../testmode/TEST_inc.h"

#include   "sbm_def.h"
#include   "sbm_ext.h"
#include   "sbm_keytbl.h"

/******************************************************************************
      �֐���  �Fsbm_current_mode()
      ��  ��   �F����
      �߂�l   �F�L��BIT���
      �@  �\   �F���L��bit�ʒm
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
BYTE Sbm_current_mode(void)
{
/*----------------------------------------------------
   bit 7,   6,   5,   4,   3,   2,   1,   0
   |      |   |   |   |   |   |   |
   |      |   |   |   |   |   |   ----AUDIO����Ӱ��
   |      |   |   |   |   |   --------���v����Ӱ��
   |      |   |   |   |   ------------ALARM�������[�h
   |      |   |   |   ----------------ALARM�N����
   |      |   |   --------------------VOL�������[�h
   |      |   ------------------------POWER OFF
   |      ----------------------------����
   ------------------------------------����
--------------------------------------------------------*/
   BYTE sbit;
   
   sbit = 0;
   if( fsbm_alm_beepon != OFF )
   {
      sbit |= CSBM_BIT_BEP;
   }
   else if( Vol_mode_check() != OFF )
   {
      sbit |= CSBM_BIT_VOL;
   }
   else if( Sef_mode_check() != OFF )
   {
      sbit |= CSBM_BIT_AUD;
   }
   else if( Clk_adj_check() != OFF )
   {
      sbit |= CSBM_BIT_CLK;
   }
   else if( Alm_mode_check() != OFF )
   {
      sbit |= CSBM_BIT_ALM;
   }
   else
   {
      ;
   }
   
   if( Pw_Power_status_get() == OFF )
   {
      sbit |= CSBM_BIT_PWR;
   }
   return(sbit);
}
/******************************************************************************
      �֐���  �FSbm_mode_check()
      ��  ��   �F����
      �߂�l   �FON��SUBӰ�ޒ��AOFF��SUBӰ�ނł͂Ȃ�
      �@  �\   �FSUBӰ�ޒ����ǂ�������
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
BYTE Sbm_mode_check(void)
{
   if( fsbm_alm_beepon != OFF )
   {
      return(CSBM_BEP);
   }
   else if( Vol_mode_check() != OFF )
   {
      return(CSBM_VOL);
   }
   else if( Sef_mode_check() != OFF )
   {
      return(CSBM_SEF);
   }
   else if( Clk_adj_check() != OFF )
   {
      return(CSBM_CLK);
   }
   else if( Alm_mode_check() != OFF )
   {
      return(CSBM_ALM);
   }
#if _TEST_MODE_
   else if((Sct_mode_check() == CSBM_SCT_VER)||(Sct_mode_check() == CSBM_SCT_BEEP))
   {
      return(CSBM_TES);
   }
#endif
   else
   {
      return(CSBM_OFF);
   }
}

/******************************************************************************
      �֐���  �FSbm_mode_cancel()
      ��  ��   �FModeID
      �߂�l   �F����
      �@  �\   �FSUBӰ�ނ̷�ݾ�
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
void Sbm_mode_cancel(BYTE mode_id)
{
   BYTE mode;
   
   mode = Ma_current_mode();      /* ����Ӱ�� */
   if((mode == mode_id) || (mode_id == CSBM_OFF))            /* ����Ӱ�ވȊO�������Ȃ� */
   {
      #if 0
      if( fsbm_alm_beepon != OFF )
      {
         fsbm_alm_beepon = OFF;
         /* beep stop���� */
      }
      #endif
      
      if( Vol_mode_check() != OFF )
      {
         Vol_mode_cancel();
      }
      
      if( Sef_mode_check() != OFF )
      {
         Sef_mode_cancel();
      }
      
      if( Clk_adj_check() != OFF )
      {
         Clk_adj_cancel();
      }
      
      if( Alm_mode_check() != OFF )
      {
         Alm_mode_cancel();
      }
      
#if _TEST_MODE_
      if(Sct_mode_check() != OFF)
      {
         Sct_mode_cancel();
      }
#endif
   }
}
/******************************************************************************
      �֐���  �FOff_keyCodeTable_addr_get
      ��  ��   �F����
      �߂�l   �FKEYcode�ϊ�ð����߲��
      �@  �\   �F�T�vӰ�ނ̷�����ð����߲���擾
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
KEY_CHG_TBL *Sbm_keyCodeTable_addr_get(void)
{
   if( fsbm_alm_beepon != OFF )
   {
      return((KEY_CHG_TBL*)&beep_key_chg_tbl);
   }
   else if((Alm_mode_check() != OFF)&&(Ma_get_alarm() == TRUE ))
   {
      if (Pw_ACC_DET_get() == ON)
      {
         return((KEY_CHG_TBL*)&alm_key_chg_tbl);
      }
      else
      {
         return((KEY_CHG_TBL*)&alm_key_chg_accoff_tbl);
      }
   }
   else if ((Clk_adj_check() != OFF)&&(Ma_get_clock() == TRUE ))
   {
      return((KEY_CHG_TBL*)&clk_key_chg_tbl);
   }
   else if (Sef_mode_check() != OFF)
   {
      return ((KEY_CHG_TBL*)&sef_key_chg_tbl);
   }
#if _TEST_MODE_
   else
   {
      return((KEY_CHG_TBL*)&Sct_key_chg_nor_tbl);
   }
#endif
}

/******************************************************************************
      �֐���  �FSbm_key_func_addr_get()
      ��  ��   �F����
      �߂�l   �F���@�\����ð����߲��
      �@  �\   �FSUBӰ�ނ̷�����ð����߲���擾
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
KEY_PROC_TBL *Sbm_key_func_addr_get(void)
{
   #if 0
   if(( fsbm_alm_beepon != OFF )||(Sct_mode_check() == CSBM_SCT_BEEP))
   #else
   if( fsbm_alm_beepon != OFF )
   #endif
   {
      return ((KEY_PROC_TBL*)&beep_key_tbl);
   }
   else if((Alm_mode_check() != OFF)&&(Ma_get_alarm() == TRUE ))
   {
      if (Pw_ACC_DET_get() == ON)
      {
         return ((KEY_PROC_TBL*)&alm_key_tbl);
      }
      else
      {
         return((KEY_PROC_TBL*)&alm_key_accoff_tbl);
      }
   }
   else if ((Clk_adj_check() != OFF)&&(Ma_get_clock() == TRUE ))
   {
      return ((KEY_PROC_TBL*)&clk_key_tbl);
   }
   else if (Sef_mode_check() != OFF)
   {
      return ((KEY_PROC_TBL*)&sef_key_tbl);
   }
#if _TEST_MODE_
   else
   {
      return ((KEY_PROC_TBL*)&Sct_key_nor_tbl);
   }
#endif
}

/******************************************************************************
      �֐���  �FSbm_key_volupon
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOLUP��ON����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
void Sbm_key_volupon(void)
{
   Key_long_time1000();            /* 1000ms��ϐݒ� */
}
/******************************************************************************
      �֐���  �FSbm_key_voluplong
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOLUP��LONG����
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
void Sbm_key_voluplong(void)
{
   fsbm_vol_mode = ON;
   sbm_vol_tim = CSBM_VOL_7S;
   Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);
   Aud_set_vol_step(UP);
   Key_long_time200();
}
/******************************************************************************
      �֐���  �FSbm_key_volupoff
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOLUP��OFF����
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
void Sbm_key_volupoff(void)
{
   fsbm_vol_mode = ON;
   sbm_vol_tim = CSBM_VOL_7S;
   Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);
   Aud_set_vol_step(UP);
   Key_nop();
}
/******************************************************************************
      �֐���  �FSbm_key_voldwon
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOLDW��ON����
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
void Sbm_key_voldwon(void)
{
   Key_long_time1000();            /* 1000ms��ϐݒ� */
}
/******************************************************************************
      �֐���  �FSbm_key_voldwlong
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOLDW��LONG����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
void Sbm_key_voldwlong(void)
{
   fsbm_vol_mode = ON;
   sbm_vol_tim = CSBM_VOL_7S;
   Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);
   Aud_set_vol_step(DW);
   Key_long_time200();
}
/******************************************************************************
      �֐���  �FSbm_key_voldwoff
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOLDW��OFF����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
void Sbm_key_voldwoff(void)
{
   fsbm_vol_mode = ON;
   sbm_vol_tim = CSBM_VOL_7S;
   Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);
   Aud_set_vol_step(DW);
   Key_nop();
}

/******************************************************************************
      �֐���  �FVol_lcddata_set
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOL����Ӱ�ޕ\���ް��쐬����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
BYTE Vol_lcddata_set(void)
{
   return (Aud_get_vol_step()); // aud_ext.c
}
/******************************************************************************
      �֐���  �FVol_mode_check
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOLӰ�ޒ����ǂ�������
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
BYTE Vol_mode_check(void)
{
   return (fsbm_vol_mode);
}

/******************************************************************************
      �֐���  �FVol_mode_cancel
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FVOLӰ�ނ̷�ݾ�
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/25      0.1      lrj      �V�K
******************************************************************************/
void Vol_mode_cancel(void)
{
   fsbm_vol_mode = OFF;
   sbm_vol_tim = 0;
}
/******************************************************************************
      �֐���  �FSbm_mode_initial
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FSUB���[�h�ϐ�����������
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/30      0.1      lrj      �V�K
******************************************************************************/
void Sbm_mode_initial(void)
{
   sbm_flag.byte = 0;
   sbm_vol_tim = 0;
   Sef_mode_initial();
   Clk_mode_initial();
   Alm_mode_initial();
}

void Sbm_BeepOn_Power(void)
{
   #if 1      /* �Г�OILNo.41�΍� */
   if (Pw_ACC_DET_get() == ON)
   #else
   if (Pw_Power_Key_get() == ON)
   #endif
   {
      Pw_power_key();
   }
   Alm_beep_key_stop();
   Key_nop();
}

/******************************************************************************
      �֐���  �FSct_mode_check
      ��  ��   �F����
      �߂�l   �F   CSBM_SCT_OFF�F�B���L�[���[�h OFF
            �F   �ȊO�F�B���L�[���쒆
      �@  �\   �F�B���L�[���[�h��������
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
         2007/4/17      0.1      ��      �V�K
******************************************************************************/
BYTE Sct_mode_check(void)
{
   return(sbm_sct_mode);  // trong file sbm_def
}
/******************************************************************************
      �֐���  �FSct_mode_cancel
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FSTOPӰ��OFF
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
         2007/4/17      0.1      ��      �V�K
******************************************************************************/
void Sct_mode_cancel(void)
{
   if (sbm_sct_mode != CSBM_SCT_OFF)
   {
      sbm_sct_mode = CSBM_SCT_OFF;
   }
   if (Vol_mode_check() == ON)
   {
      Vol_mode_cancel();
   }
}

void Sct_key_pwr_on(void)
{
   Pw_power_key();
   Key_nop();
}
void Sct_key_Ver_dispoff(void)
{
   Sct_mode_cancel();
   Key_nop();
}
/******************************************************************************
      �֐���  �FSct_key_test
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FTEST��On����   TEST key On processing
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z  
------------------------------------------------------------------------------
         2007/4/17      0.1      ��      �V�K
******************************************************************************/
void Sct_key_test(BYTE mode)
{
   if (sbm_sct_mode == CSBM_SCT_OFF)
   {
      sbm_sct_mode = mode;
      switch(sbm_sct_mode)
      {
         case CSBM_SCT_BEEP:
            Aud_set_beep(CAUD_REQ_BEEP_MULTIPLE);
            break;
         
         case CSBM_SCT_LOUDOFF:
            Aud_secret_loud_key();
            break;
         
         default :
            break;
      }
   }
   else 
   {
      Sct_mode_cancel();
   }
   Pw_power_key();
   Key_nop();
}

void   Sbm_key_tisc_on(void)
{
   BYTE   destno;
   
   destno = Ma_model_dest();/*�d����No�擾*/
   
   switch(destno)
   {
      case CMA_MODEL_AMFM_J:
      case CMA_MODEL_AM_J:
         Tu_key_isr_on();/*TI���[�h�֑J��*/
         break;
      
      default:
         Key_nop();/*����*/
         break;
   }
}

#if 0   /* �g�p���Ȃ����߁A�폜 */
/******************************************************************************
      �֐���  �FSbm_indicator_set
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FSUB���[�h�ݼ޹���\���ް�����
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/7/31      0.1      lrj      �V�K
******************************************************************************/
BYTE Sbm_indicator_set(void)
{
   BYTE   indicator;
   
   indicator = 0;
   
   if( fsbm_clk_ampm == CSBM_CLK_AM )
   {
      indicator |= CLCD_CL_IND_AM;
   }
   else
   {
      indicator |= CLCD_CL_IND_PM;
   }
   
   if( fsbm_alm_onoff == ON )
   {
      indicator |= CLCD_CL_IND_AL;
   }
   
   return(indicator);
}
#endif

#undef      _SBM_EXT_C_
