/***************************************************************
   �N�����I���a����
   �v���W�F�N�g��   �F2012�NAMFM_RADIO���f��
   �t�@�C����      �Foff_key.c
   ���W���[����   �FOFF���[�h
   �@   �\         �FKEY�����֐��t�@�C��
----------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
****************************************************************/
#define   _OFF_KEY_C_

#include   "../model.h"
#include   "../comm/common.h"
#include   "../main/ma_ext.h"
#include   "../audio/Aud_inc.h"
#include   "../key/key_func_ext.h"
#include   "../key/key_def.h"
#include   "../submode/SBM_inc.h"
#include   "../testmode/TEST_inc.h"
#include   "off_key.h"

#define      OFF_KEYEN_NOP      0x00

/******************************************************************************
      �֐���  �FOff_keyCodeTable_addr_get
      ��  ��   �F����
      �߂�l   �F�L�[�R�[�h�ϊ��e�[�u���|�C���^
      �@  �\   �FOFF���[�h�̃L�[�R�[�h�ϊ��e�[�u���|�C���^�擾
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
KEY_CHG_TBL *Off_keyCodeTable_addr_get(void)
{
   return ((KEY_CHG_TBL*)&off_key_chg_tbl);
}

/******************************************************************************
      �֐���  �FOff_key_func_addr_get
      ��  ��   �F����
      �߂�l   �F�L�[�e�[�u���|�C���^
      �@  �\   �FOFF���[�h�̃L�[�����e�[�u���|�C���^�擾
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
KEY_PROC_TBL * Off_key_func_addr_get(void)
{
   return ((KEY_PROC_TBL*)&off_key_tbl);
}

/******************************************************************************
      �֐���  �FOff_key_pw_long
      ��  ��   �F����
      �߂�l   �Fno
      �@  �\   �Fpower key long function
------------------------------------------------------------------------------
lirongjun20121119      �V�K�쐬
******************************************************************************/
void  Off_key_pw_long(void)
{
   if ((Ma_get_clock() == TRUE )
   &&(Pw_ACC_status_get() == ON))
   {
      Clk_adj_start();
   }
   Key_nop();
}
/******************************************************************************
      �֐���  �FOff_key_alarm_xxx
      ��  ��   �F����
      �߂�l   �Fno
      �@  �\   �Falarm key function
------------------------------------------------------------------------------
chin20120821      �V�K�쐬
******************************************************************************/
void  Off_key_alarm_on(void)
{
   if (Ma_get_alarm() == FALSE )
   {
      Key_nop();
      return;
   }
   
   if (Pw_ACC_status_get() == OFF)
   {
      Key_long_time2000_accoff();
   }
   else
   {
      Key_long_time2000();
   }
}
void  Off_key_alarm_long(void)
{
   Alm_mode_start();
   //Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
   //Pw_acc_wakeup_req(ON);
   Key_nop();
}
void  Off_key_alarm_off(void)
{
   Alm_onoff_set();
   Key_nop();
}
void  Off_key_testp1(void)
{
   Sct_key_test(CSBM_SCT_VER);   // trong file sbm_ext.c
   Key_nop();
}

void  Off_key_testp2(void)
{
   /* alarm on */
   Sct_key_test(CSBM_SCT_BEEP);
   Key_nop();
}

void  Off_key_testp3(void)
{
   /* LOUD OFF  */
   Sct_key_test(CSBM_SCT_LOUDOFF);
   Key_nop();
}

#undef   _OFF_KEY_C_
