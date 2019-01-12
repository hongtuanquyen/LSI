/******************************************************************************
   �N�����I���i���j�a����      2007 C3X03 �ذ��[��]

      �v���W�F�N�g��   : '07�N�t���b�v�_�E���@MP3���f���J��
      �t�@�C����      : eeprom_func_ext.c
      �@�\         : 
------------------------------------------------------------------------------
�C������
******************************************************************************/
#include   "../model.h"
#if    _MODEL_EEPROM_
#define   _EEPROM_EXT_C_

#include   <string.h>
#include   "../comm/common.h"
#include   "../io/IO_inc.h"
#include   "../testmode/TEST_inc.h"

#include   "eeprom_main.h"
#include   "eeprom_ext.h"

#if _TEST_MODE_
static BYTE eep_test_buf[19];
#endif

/******************************************************************************
..       �֐���  �FEeprom_1ms_timer
      ��  ��   �F�Ȃ�
      �߂�l   �F�Ȃ�
      �@  �\   �FEEPROM 1ms�����Ϗ���
------------------------------------------------------------------------------
�C������
[sun090330] �V�K�쐬
******************************************************************************/
void   Eeprom_1ms_timer(void)
{
   if (bEEP_Drv_Timer > TIMER_OUT)
   {
      bEEP_Drv_Timer--;
   }
}

/******************************************************************************
..       �֐���  �Feeprom_data_chk
      ��  ��   �Fdata  �� �������ް�
            �Fmap_index �� �ް��i�[�ޯ̧Index
      �߂�l   �F�Ȃ�
      �@  �\   �FEEPROM ϯ��Info�擾����
------------------------------------------------------------------------------
�C������
[sun100805]�V�K�쐬
******************************************************************************/
void   EEP_WriteReq(sEEP_PARA   WtReq)
{
   if ((sEEP_DRVWork.Busy == FALSE) &&
      (WtReq.Length <= CEEP_BUFMAX))
   {
      memcpy(bEEP_DataBuf,WtReq.BufAddr,WtReq.Length);
      sEEP_DRVWork.SlaveAddr = WtReq.EepAddr;
      sEEP_DRVWork.Length = WtReq.Length;
      sEEP_DRVWork.DataAddr = bEEP_DataBuf;
      sEEP_DRVWork.Busy = ON;
   }
}

/******************************************************************************
..       �֐���  �Feeprom_data_mapread
      ��  ��   �Fmap_index �ˎ擾ϯ�߃f�[�^Index
      �߂�l   �F�擾�����f�[�^�uWORD�P�ʁv
      �@  �\   �FEEPROM ϯ���ް��擾����
------------------------------------------------------------------------------
�C������
[sun100805]�V�K�쐬
******************************************************************************/
BYTE   EEP_ReadReq(sEEP_PARA   ReadReq)
{
   BYTE   iRet;

   iRet = FALSE;
   if (EEP_CheckIDLE() == TRUE)
   {
      sEEP_DRVWork.SlaveAddr = ReadReq.EepAddr;
      sEEP_DRVWork.DataAddr = ReadReq.BufAddr;
      sEEP_DRVWork.Length = ReadReq.Length;
      if (EEP_Drv_Read() == TRUE)
      {
         iRet = TRUE;
      }
   }
   else
   {
      iRet = FALSE;
   }
   return(iRet);
}

/******************************************************************************
..       �֐���  �FBYTE   EEP_CheckIDLE(void)
      ��  ��   �F�Ȃ�
      �߂�l   �FTRUE�@�ˁ@EEPROM�@IDLE��
            �FFALSE�@�˓��쒆
      �@  �\   �FEEPROM�@IDLE�����ǂ�����������
------------------------------------------------------------------------------
�C������
[sun100805]�V�K�쐬
******************************************************************************/
BYTE   EEP_CheckIDLE(void)
{
   if ((sEEP_DRVWork.Busy == ON) ||
      (bEEP_Drv_Timer != 0x00))
   {
      return(FALSE);
   }
   else
   {
      return(TRUE);
   }
}


/******************************************************************************
..       �֐���  �Feeprom_write_ok_chk
      ��  ��   �F�Ȃ�
      �߂�l   �FOK �� �����ݒ�
            �FNG �� �����݊���
      �@  �\   �FEEPROM Reset�v����������
------------------------------------------------------------------------------
�C������
[sun090325] �V�K�쐬
[sun090416]   �����ݗv���S���Ȃ��Ȃ�܂�NG��Ԃ��悤�ɕύX
******************************************************************************/
BOOL   eeprom_write_ok_chk(void)
{
   if ((feep_lastwt == ON) ||
      (bEEP_Drv_Timer != TIMER_OUT) ||
      (sEEP_DRVWork.Busy == ON) ||
      (feep_reset == ON) ||
      (feep_initial_req == NG))
   {
      return(NG);
   }
   else
   {
      return(OK);
   }
}
/******************************************************************************
..       �֐���  �Feeprom_adj_reset
      ��  ��   �F�Ȃ�
      �߂�l   �F�Ȃ�
      �@  �\   �FEEPROM Reset����
------------------------------------------------------------------------------
�C������
******************************************************************************/
void   EEP_Initial_Req(void)
{
   #if 0
   feep_initial_req = ON;
   #endif
   
   BYTE isys;
   
   /* FacrotyCode1 5BYTE */
   bEEP_inidata[0] = 0x5a;
   bEEP_inidata[1] = 0xa5;
   bEEP_inidata[2] = 0x00;      /* �d��������   ko co destination */
   bEEP_inidata[3] = 0xFF;      /* ���v�L       co dong ho */
   bEEP_inidata[4] = 0xFF;      /* �A���[���L    co bao thuc */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_INIDATA; // CEEP_ADDR_INIDATA trong file eeprom_ext.h
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_INIDATA; // CEEP_LENG_INIDATA = 5;
   EEP_Drv_Write(); // nam trong file eeprom_ext.c
   
   for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
   {
      Cm_wait_loop(0xFF);
   }
   
   memset(bEEP_inidata,0xFF,sizeof(bEEP_inidata));
   /* AM�̈� 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHAM;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_PCHAM;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FM1�̈� 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHFM1;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_PCHFM1;
   EEP_Drv_Write();

   for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FM2�̈� 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHFM2;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_PCHFM2;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FacrotyCode2 2BYTE */
   bEEP_inidata[0] = 0x5a;
   bEEP_inidata[1] = 0xa5;
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_FACTCODE2;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_FACTCODE;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
   {
      Cm_wait_loop(0xFF);
   }
}

void   EEP_Initial_para_Req(BYTE *buf,BYTE mode)
{
   BYTE isys;
   
   if (mode != 0xFF)
   {
      /* clock��alarm 2BYTE */
      bEEP_inidata[3] = 0x01;
      bEEP_inidata[4] = 0x01;
      sEEP_DRVWork.SlaveAddr = CEEP_ADDR_CLKALM;
      sEEP_DRVWork.DataAddr = &bEEP_inidata[3];
      sEEP_DRVWork.Length = CEEP_LENG_CLKALM;
      EEP_Drv_Write();   // nam trong file eeprom_main.c
      
      for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
      {
         Cm_wait_loop(0xFF);
      }
   }
   
   /* AM�̈� 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHAM;
   sEEP_DRVWork.DataAddr = &buf[0];
   sEEP_DRVWork.Length = CEEP_LENG_PCHAM;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FM1�̈� 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHFM1;
   sEEP_DRVWork.DataAddr = &buf[12];
   sEEP_DRVWork.Length = CEEP_LENG_PCHFM1;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FM2�̈� 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHFM2;
   sEEP_DRVWork.DataAddr = &buf[24];
   sEEP_DRVWork.Length = CEEP_LENG_PCHFM2;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms�҂� */
   {
      Cm_wait_loop(0xFF);
   }
}
/*****************************************************************************************
   �֐���      �FIO_EEP_WriteData_XBYTE
   ����      �F�Ȃ�
   ���^�[��   �FOK��NG
   �@�\�T�v   �FXBYTE���̃f�[�^��������
-----------------------------------------------------------------------------------------
[chin20120830] �V�K�쐬
*****************************************************************************************/
BYTE   EEP_WriteData_xBYTE( BYTE *buf, BYTE size )
{
   BYTE ret;
   BYTE iCnt;
   
   ret = NG;
   if (size > 16)
   {
      return ret;
   }
   
   ret = 0x01;
   for (iCnt = 0; iCnt < size; iCnt++)
   {
      ret &= IO_EEP_WriteByte( *buf++);
   }
   return ret;
}

void EEP_Test_func(BYTE cmd,BYTE *addr,BYTE len)
{
   BYTE eep_test;
   sEEP_PARA Req;
   
   if (cmd != CTEST_MODE_ATMIC_EEP)
   {
      return ;
   }
   eep_test = *(addr+1);
   switch(eep_test)
   {
      case 0x08:            /* 1BYTE ������ */
         Req.EepAddr = *(addr+ 3);
         Req.Length = 0x01;
         Req.BufAddr = addr+4;
         EEP_WriteReq(Req);
         break;
      
      case 0x09:            /* 16BYTE ������ */
         Req.EepAddr = *(addr+ 3);
         Req.Length = 0x10;
         Req.BufAddr = addr+4;
         EEP_WriteReq(Req);
         break;
      
      case 0x18:            /* 1BYTE �Ǐo�� */
         Req.EepAddr = *(addr+ 3);
         Req.Length = 0x01;
         Req.BufAddr = &eep_test_buf[3];
         if (EEP_ReadReq(Req))
         {
            eep_test_buf[0] = 0x02;
            eep_test_buf[1] = 0;
            eep_test_buf[2] = Req.EepAddr;
            Test_data_reply(CTEST_MODE_ATMIC_EEP,0x28,eep_test_buf, 0x04);
         }
         else
         {
            eep_test_buf[0] = 0xFF;
            Test_data_reply(CTEST_MODE_ATMIC_EEP,0x28,eep_test_buf, 0x01);
         }
         break;
      
      case 0x19:            /* 16BYTE �Ǐo�� */
         Req.EepAddr = *(addr+ 3);
         Req.Length = 0x10;
         Req.BufAddr = &eep_test_buf[3];
         if (EEP_ReadReq(Req))
         {
            eep_test_buf[0] = 0x04;
            eep_test_buf[1] = 0;
            eep_test_buf[2] = Req.EepAddr;
            Test_data_reply(CTEST_MODE_ATMIC_EEP,0x28,eep_test_buf, 0x13);
         }
         else
         {
            eep_test_buf[0] = 0xFF;
            Test_data_reply(CTEST_MODE_ATMIC_EEP,0x28,eep_test_buf, 0x01);
         }
         break;
      default :
         break;
   }
}
#undef  _EEPROM_EXT_C_
#endif   /* _MODEL_EEPROM_ */
