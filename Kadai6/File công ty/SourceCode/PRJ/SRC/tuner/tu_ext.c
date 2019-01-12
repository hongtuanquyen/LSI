/******************************************************************************
..     �N�����I���a����                    2007����

      �v���W�F�N�g��   :
      �t�@�C����      : tu_ext.c
      �@�\         : ?Tuner �O��IF����
------------------------------------------------------------------------------
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212]   �V�K�쐬
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
..       �֐���  �FTu_band_chk
      ��  ��   �F����
      �߂�l   �FTUNER BAND���
            �FCTU_FM �� FM BAND��
            �FCTU_AM �� AM BAND��
      �@  �\   �FTUNER BAND����ʒm����
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
[zhang070315] isr mode��,isr band��ʒm
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
..       �֐���  �FTu_bandno_chk
      ��  ��   �F����
      �߂�l   �FTUNER BAND NO���
            �FCTU_BAND0 �� BAND NO����
            �FCTU_BAND1 �� BAND 1
            �FCTU_BAND2 �� BAND 2
            �FCTU_BAND3 �� BAND 3
      �@  �\   �FTUNER BAND NO����ʒm����
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
******************************************************************************/
BYTE Tu_bandno_chk(void)
{
   return(tu_mem[tu_bandno].bno);
}

/******************************************************************************
..       �֐���  �FTu_pstno_chk
      ��  ��   �F����
      �߂�l   �FTUNER Preset NO���
            �F0 �� Preset��M���ł͂Ȃ�
            �F1�`6 �� Preset�ԍ�
      �@  �\   �FTUNER Preset NO��ʒm����
              disp�g�p�̂�
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
[sun051213] ����PresetNo��Ԃ��悤�ɕύX
******************************************************************************/
BYTE Tu_pstno_chk(void)
{
   return(tu_pno);
}

/******************************************************************************
..       �֐���  �FTu_1ms_timer
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FTUNER 1ms��Ϗ���
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
******************************************************************************/
void Tu_1ms_timer(void)
{
   if (tu_1ms_timer > 0)
   {
      tu_1ms_timer --;
   }
}

/******************************************************************************
..       �֐���  �FTu_5ms_timer
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FTUNER 5ms��Ϗ���
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
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
..       �֐���  �FTu_10ms_timer
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FTUNER 10ms��Ϗ���
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
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
..       �֐���  �FTu_indicator_set
      ��  ��   �F����
      �߂�l   �FTuner�ݼ޹ް�
      �@  �\   �FTuner�֌W�ݼ޹ް��\���쐬����
      ��   �l   �FCLCD_IND_MANU

------------------------------------------------------------------------------
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
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
      indicator |= CLCD_TU_IND_FM;      /* "FM"�C���W�P�[�^�_�� */
      if (Tu_st_check() == TRUE)
      {
         indicator |= CLCD_TU_IND_ST ;   /* "ST"�C���W�P�[�^�_�� */
      }
      if (tu_dispfreq >= 10000)
      {
         indicator |= CLCD_TU_IND_S1;               /* "1"00�C���W�P�[�^�_�� */
      }
      
   }
   else
   {
      indicator |= CLCD_TU_IND_AM;      /* "AM"�C���W�P�[�^�_�� */
      if (tu_dispfreq >= 1000)
      {
         indicator |= CLCD_TU_IND_S1;               /* "1"000�C���W�P�[�^�_�� */
      }
   }
   if (bandno == CTU_BAND1)
   {
      indicator |= CLCD_TU_IND_ONE;      /* band"1"�C���W�P�[�^�_�� */
   }
   else if (bandno == CTU_BAND2)
   {
      indicator |= CLCD_TU_IND_TWO;      /* band"2"�C���W�P�[�^�_�� */
   }
   else
   {
   }
   return(indicator);
}

/******************************************************************************
..       �֐���  �FTu_isr_indicator_set
      ��  ��   �F����
      �߂�l   �FTuner isr �ݼ޹ް�
      �@  �\   �FTuner isr �֌W�ݼ޹ް��\���쐬����
      ��   �l   �FCLCD_IND_MANU

------------------------------------------------------------------------------
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120730] �V�K�쐬
******************************************************************************/
BYTE   Tu_isr_indicator_set( void )
{
   BYTE   indicator;
   
   indicator = 0;
   Tu_dispfreqset();
   
   indicator |= CLCD_TU_IND_TI ;      /* "�E))"�C���W�P�[�^�_�� */
   indicator |= CLCD_TU_IND_AM;      /* "AM"�C���W�P�[�^�_�� */
   if (tu_dispfreq >= 1000)
   {
      indicator |=CLCD_TU_IND_S1;      /* "1"000�C���W�P�[�^�_�� */
   }
   
   return(indicator);
}

/******************************************************************************
..       �֐���  �FTu_mainmd_check
      ��  ��   �F����
      �߂�l   �FTRUE->seek/ps/as��
             :FALSE->��Ӱ�ޒ�
      �@  �\   �FTuner main mode check����
      ��   �l   �F
------------------------------------------------------------------------------
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[zhang070412] �V�K�쐬
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
..       �֐���  �FTu_st_check
      ��  ��   �F����
      �߂�l   �Fst���o �l
      �@  �\   �FTuner st ���o��ʒm����
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[zhang070309] �V�K�쐬
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
..       �֐���  �FTu_lcddata_set
      ��  ��   �F����
      �߂�l   �F�\���ް��i�[�ر�߲��
      �@  �\   �FTuner�\���ް��쐬����
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212] �V�K�쐬
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

   Tu_dispfreqset();               /* ���g���\���p�ϊ� */

   iRet = Cm_lib_wHEXtoBCD(tu_dispfreq,bcddata);   /* HEX��BCD�ϊ� */

   tudisp->frq[0] = bcddata[0];
   tudisp->frq[1] = bcddata[1];
   tudisp->frq[2] = bcddata[2];

   if (tu_m_mode == CTU_M_SCAN)   /* �u070723���vlow3 buglist�� 97�C��*/
   {
//      Sbm_mode_cancel(CMA_TUNER);
      if(tu_m_seq == CTU_SCANWAIT_SEQ)   /* SCAN��M�� */
      {
         tudisp->mode = CLCD_TU_SCANWAIT;
      }
      else                     /* SCAN SEEK�� */
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
..       �֐���  �FTu_u_lcddata
      ��  ��   �F����
      �߂�l   �F�\���ް��i�[�ر�߲��
      �@  �\   �FTuner isr�\���ް��쐬����
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120724] �V�K�쐬
******************************************************************************/
void Tu_isr_lcddata_set(TDP_ISR   * isrdisp,BYTE   dispmode)
{
   BYTE   bcddata[3];
   BYTE   iRet;

   isrdisp->band = Tu_band_chk();
   Tu_dispfreqset();               /* ���g���\���p�ϊ�  trong file tu_event.c*/

   iRet = Cm_lib_wHEXtoBCD(tu_dispfreq,bcddata);   /* HEX��BCD�ϊ� */

   isrdisp->frq[0] = bcddata[0];
   isrdisp->frq[1] = bcddata[1];
   isrdisp->frq[2] = bcddata[2];
}

/******************************************************************************
..       �֐���  �FTu_amband_set
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAM�o���h�ݒ菈��
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120724] �V�K�쐬
******************************************************************************/
void Tu_amband_set(void)
{
   tu_bandno = CTU_BAND_AM;   /* AM�w�� */
   Tu_lastcall();
}

/********************************************************************************/
/*   �֐���  �FTu_isrmd_on                                          */
/*   ��  ��   �Fmode:                                                */
/*   �߂�l   �F�Ȃ�                                                */
/*   �@  �\   �FTuner ISR���[�hON�ɐݒ�                                 */
/*   ���ӎ���:                                                   */
/*   �C������   �y�N�����z   �yVersion�z   �y���O�z    �y�����z               */
/*------------------------------------------------------------------------------*/
/*            2010.08.09      0.0         �x      �V�K�쐬               */
/********************************************************************************/
void   Tu_isrmd_on(void)
{
   ftu_isr = ON;
}
/********************************************************************************/
/*   �֐���  �FTu_isrmd_off                                          */
/*   ��  ��   �Fmode:                                                */
/*   �߂�l   �F�Ȃ�                                                */
/*   �@  �\   �FTuner ISR���[�hOFF�ɐݒ�                                 */
/*   ���ӎ���:                                                   */
/*   �C������   �y�N�����z   �yVersion�z   �y���O�z    �y�����z               */
/*------------------------------------------------------------------------------*/
/*            2010.08.09      0.0         �x      �V�K�쐬               */
/********************************************************************************/
void   Tu_isrmd_off(void)
{
   ftu_isr = OFF;
}

/*   �e�X�g���[�h�̒���AM/FM1�I����Ή��A100826�A�x   */
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
   Tu_lastcall();                  /* ׽Čďo */
   tu_m_mode = CTU_M_BANDCHG;         /* BandChangeӰ�޾�� */
   Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);      /* ���ׂĎ����\����ݾ� */
   Lcd_disp_base(CMA_TUNER);
   Sbm_mode_cancel(CMA_TUNER);
}

void Tu_beep_mute(BYTE on_off)
{
   Tu_beep_mute_set(on_off);
}
#undef  _TU_EXT_C_

#endif
