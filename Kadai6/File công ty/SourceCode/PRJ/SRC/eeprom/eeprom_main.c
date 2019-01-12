/******************************************************************************
   �N�����I���i���j�a����      2007 C3X03 �ذ��[��]

      �v���W�F�N�g��   : '07�N�t���b�v�_�E���@MP3���f���J��
      �t�@�C����      : eeprom_main.c
      �@�\         : �d�����[�h�J�ڏ���
------------------------------------------------------------------------------
�C������
[sun070228] ����
******************************************************************************/
#include   "../model.h"
#if    _MODEL_EEPROM_
#define   _EEPROM_MAIN_C_

#include   <string.h>
#include   "../comm/common.h"
#include   "../main/ma_ext.h"
#include   "../io/IO_inc.h"

#include   "eeprom_main.h"
#include   "eeprom_ext.h"

static BYTE   EEP_Drv_AckPolling(void);

#define      CEEP_MODE_W      0xa0
#define      CEEP_MODE_R      0xa1

#define      CIO_IIC_NACK   1
#define      CIO_IIC_ACK      0

/*****************************************************************************************
   �֐���      �FBYTE   EEP_Drv_AckPolling(void)   
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM Acknowledge�̃|�[�����O����
            �F�@StartBit���M
            �F�A������Condition�ް����M
            �F�BAck����
            �FMAX�@6ms��ACK�@OK�܂ő҂�
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
*****************************************************************************************/
BYTE   EEP_Drv_AckPolling(void)
{
   BYTE   p_ret;
   BYTE   iCnt;
   
   iCnt = 10;
   p_ret = NG;
   while(iCnt > 0)
   {
      IO_EEP_SendStart();               /*�X�^�[�g�R���f�B�V�������M*/
      p_ret = IO_EEP_WriteByte(0xa0);
      if (p_ret == OK)
      {
         iCnt = 0;
      }
      else
      {
         IO_EEP_SendStop();
         Cm_wait_loop(50);
         iCnt--;
      }
   }
   return(p_ret);
}

/*****************************************************************************************
   �֐���      �FBYTE   EEP_Drv_Read(void)   
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM��ײ��Main����
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
*****************************************************************************************/
BYTE   EEP_Drv_Read(void)
{
   BYTE   r_ret;
   
   r_ret = EEP_Drv_AckPolling();
   
   if (r_ret == OK)
   {
      /* WORD ADDRESS */
      r_ret &= IO_EEP_WriteByte(sEEP_DRVWork.SlaveAddr);
      
      IO_EEP_SendRestart();
      
      r_ret &= IO_EEP_WriteByte(0xa1);
      
      if (r_ret == OK)
      {
         IO_EEP_ReadData(sEEP_DRVWork.DataAddr, sEEP_DRVWork.Length );
      }
   }
   
   /* IO_EEP_SendStop */
   IO_EEP_SendStop();
   return r_ret;
}


/*****************************************************************************************
   �֐���      �FBYTE   EEP_Drv_Write(void)
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM��ײ��Main����   Tong quan chuc nang: Xu li EEPROM Driver Main
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
*****************************************************************************************/
BYTE   EEP_Drv_Write(void)
{
   BYTE i;
   BYTE w_ret;
   
   /* start conditon */
   IO_EEP_SendStart();
   
   /* device address */
   IO_EEP_WriteByte(0xa0);
   
   /* WORD ADDRESS(00) */
   w_ret = IO_EEP_WriteByte(sEEP_DRVWork.SlaveAddr);
   
   #if 1      /* 16BYTE�����݉\�A16BYTE�ȓ��Ȃ�ꊇ�� */
   /* SantaFe�ɂ���āA1BYTE��16BYTE���������݂��Ȃ��ALength���f����K�v�͂Ȃ� */
   w_ret |= EEP_WriteData_xBYTE(sEEP_DRVWork.DataAddr,sEEP_DRVWork.Length );
   #else
   if (sEEP_DRVWork.Length <= 8)
   {
      w_ret |= EEP_WriteData_xBYTE(sEEP_DRVWork.DataAddr,sEEP_DRVWork.Length );
      sEEP_DRVWork.Busy = FALSE;
      
   }
   else
   {
      w_ret |= EEP_WriteData_8BYTE(sEEP_DRVWork.DataAddr);
      sEEP_DRVWork.Length -= 8;      /* length -8 */
      sEEP_DRVWork.SlaveAddr += 8;      /* slaveaddress + 8 */
      sEEP_DRVWork.DataAddr += 8;      /* address + 8 */
   }
   #endif
   
   /* IO_EEP_SendStop */
   IO_EEP_SendStop();
   
   bEEP_Drv_Timer = 11;
   
   return w_ret;
}


/******************************************************************************
..       �֐���  �Feeprom_initial
      ��  ��   �F�Ȃ��
      �߂�l   �F����
      �@  �\   �FEEPROM �����Ǎ��ݏ���
------------------------------------------------------------------------------
******************************************************************************/
void   eeprom_initial(void)
{
   #if 1
   IO_EEP_Port_Reset();
   #endif
   
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_INIDATA;
   sEEP_DRVWork.Length = CEEP_LENG_INIDATA+CEEP_LENG_NUMBER;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   EEP_Drv_Read();
   
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_FACTCODE2;
   sEEP_DRVWork.Length = CEEP_LENG_FACTCODE;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[10];
   EEP_Drv_Read();
   
   Ma_ini_data_set(bEEP_inidata);
}
/******************************************************************************
..       �֐���  �Feeprom_main
      ��  ��   �F�Ȃ��
      �߂�l   �F����
      �@  �\   �FEEPROM Systemү���ގ�t����
------------------------------------------------------------------------------
[sun100806] �V�K�쐬
******************************************************************************/
void   eeprom_main(void )
{
   
   BYTE   status;
   
   if (sEEP_DRVWork.Busy == ON)
   {
      if (bEEP_Drv_Timer == 0)
      {
         status = EEP_Drv_Write();
         if (status == OK)
         {
            sEEP_DRVWork.ErrCnt = 0;
            sEEP_DRVWork.Busy = OFF;
         }
         else
         {
            sEEP_DRVWork.ErrCnt++;
            if (sEEP_DRVWork.ErrCnt >= 3)
            {
               sEEP_DRVWork.Busy = OFF;
               sEEP_DRVWork.ErrCnt = 0;
            }
         }
      }
   }
   
}


#undef  _EEPROM_MAIN_C_
#endif   /* _MODEL_EEPROM_ */
