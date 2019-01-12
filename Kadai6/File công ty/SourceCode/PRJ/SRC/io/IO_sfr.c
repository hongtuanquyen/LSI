/******************************************************************************
   �N�����I���a����                    2007  C3X03�ذ��[��]
      ��ۼު�Ė�   : '07�N�د���޳� MP3���يJ��    Phat trien mo hinh MP3 Flip down nam 07
      ̧�ٖ�      : IO_sfr.c
        Ӽޭ�ٖ�   : I/O
      �@�\         : LC87�ذ�� ڼ޽���`    Dinh nghia thanh ghi  Series LC87
------------------------------------------------------------------------------
   �C�������y�N�����z�yVer�z�y���O�z �y�����z
------------------------------------------------------------------------------
            2005.11.29  0.00   �N�c    �V�K�쐬
******************************************************************************/
#define   _IO_SFR_C_

#include    <stdio.h>
#include    <string.h>
#include   "../model.h"

#define      DEFINE_SFRBITS
#include <lc884200.h>

#include   "../comm/common.h"
#include   "../power/PW_inc.h"
#include   "../power/pw_main.h"
#include   "../submode/SBM_inc.h"
#include   "../main/ma_ext.h"

#include   "IO_def.h"
#include   "IO_ext.h"

#define      _CLK_DEBUG_      0

#define      CIO_IIC_ACK      0
#define      CIO_IIC_NACK   1

#define      RAM_ADDRESS      0x0300

#if 0
#define      BHOT      39
#define      BCOLD      31
#else
#define      BHOT      38
#define      BCOLD      32
#endif

#define      COMPENSATE_CYCLE   (1UL << 20)
#define      BIT_ADJUST      (1000000000UL>>20)
#define      DIF_TEMPERTURE      65

/*----------------------------------------------------
    LC87 SFR ��`
----------------------------------------------------*/
static TCOM_FLG   fIO_flags1;
#define   fio_adc_active   (fIO_flags1.bit.bit7) /* AD�ϊ����t���O 1�F�ϊ��� */
#define   fIO_ill         (fIO_flags1.bit.bit6)
#define   fIO_acc         (fIO_flags1.bit.bit5)
#define   fIO_alarm      (fIO_flags1.bit.bit4)   /* BATT���荞�݃��x���؂�ւ��֎~�׸� */
static BYTE   bIO_INTStatus;
static BYTE io_sio_buf[32] ;//= (BYTE*)RAM_ADDRESS;      /* SIO����M�ޯ̧  */
/*----------------------------------------------------
    ���v�␳�Ɋւ����`
----------------------------------------------------*/
                              /*  TEMPCONV0  */
   #define __pword_TEMPCONV0 ((volatile WORD*)__TEMPCONV0)
   #define __word_TEMPCONV0 (*__pword_TEMPCONV0)
                              /*  TEMPCONV2  */
   #define __pword_TEMPCONV2 ((volatile WORD*)__TEMPCONV2)
   #define __word_TEMPCONV2 (*__pword_TEMPCONV2)
                              /*  CALC1L  */
   #define __pword_CALC1L ((volatile WORD*)__CALC1L)
   #define __word_CALC1 (*__pword_CALC1L)
                                       /*  CALC4L  */
   #define __pword_CALC4L ((volatile WORD*)__CALC4L)
   #define __word_CALC4 (*__pword_CALC4L)
/*----------------------------------------------------
    ���[�J���֐�
----------------------------------------------------*/
static BYTE   IO_Tuner_Send_Byte(BYTE );
static BYTE   IO_Tuner_Receive_Byte(BYTE);

static void   IO_EVOL_Send_Start(void);
static void   IO_EVOL_Send_Stop(void);
static BYTE   IO_EVOL_Send_Byte(BYTE );

static BYTE   IO_EEP_ReadByte(BYTE   ack);

#if _TEST_MODE_
/******************************************************************************
   ��  �� ��   : IO_Test_xxxx
   �@�\�T��   : UART2�ʐM�C���^�[�t�F�[�X
-------------------------------------------------------------------------------
[chin20120903] �V�K�쐬
********************************************************************************/
void IO_Test_SetUartPort(BYTE Status)
{
   if (Status == ON)
   {
      pUART_TX_FSA = 1;               /*ACCON��UART�o�͐ݒ�*/
      __byte_U2CNT0 = BIN_00001001;
   }
   else
   {
      __byte_U2CNT0 = BIN_00000000;
      pUART_TX_FSA = 0;               /*ACCOFF��LOW�o�͐ݒ�*/
   }
}

void IO_Test_TxStart(BYTE tx_data)
{
   __byte_U2CNT0 &= BIN_00001111;
   __byte_U2TBUF = tx_data;
   __byte_U2CNT0 |= BIN_00011000;
}
void IO_Test_TxNext(BYTE tx_data)
{
   __byte_U2TBUF = tx_data;
}

void IO_Test_TxEnd(void)
{
   __bf_U2CNT0.TEMPTYIE = OFF;
}
void IO_Test_TxStop(void)
{
   __byte_U2CNT0 &= BIN_00001111;
}
void IO_Test_RxStart(void)
{
   __byte_U2CNT0 &= BIN_11111000;
   __byte_U2CNT0 |= BIN_00001001;
}
BYTE IO_Test_Get_RxData(void)
{
   return(IO_Uart2_RXData);
}
void IO_Test_RxStop(void)
{
   __byte_U2CNT0 &= BIN_11111000;
}

#endif

#if 1   /* chin20121009 �o�X�t�@�C�g�΍� */
/******************************************************************************
   ��   ��  ��   : IO_SetIICPort
   ��       ��   : status �� IIC�|�[�gON/OFF�ݒ�
   ��   ��  �l   : ����
   �@ �\ �T ��   : IIC�|�[�g�̐ݒ菈��
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
void  IO_SetIICPort(BYTE status)
{
   if (status == ON)
   {
      pEVOL_CLK_FSA = 0;
      pEVOL_CLK_FSB = 0;
      pEVOL_CLK_LAT = 0;
      pEVOL_CLK_DDR = 0;
      
      pEVOL_DATA_FSA = 0;
      pEVOL_DATA_FSB = 0;
      pEVOL_DATA_LAT = 0;
      pEVOL_DATA_DDR = 0;
   }
   else      /* LOW�o�� */
   {
      pEVOL_CLK_FSA = 0;
      pEVOL_CLK_FSB = 0;
      pEVOL_CLK_LAT = 0;
      pEVOL_CLK_DDR = 1;
      
      pEVOL_DATA_FSA = 0;
      pEVOL_DATA_FSB = 0;
      pEVOL_DATA_LAT = 0;
      pEVOL_DATA_DDR = 1;
   }
}

/******************************************************************************
   ��   ��  ��   : IO_Tuner_Receive
   ��       ��   : *addr �� ��M�ް��ޯ̧�߲��
               : bytecnt�� ��M�ް���
   ��   ��  �l   : ����
   �@ �\ �T ��   : Tuner�ް���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
void  IO_Tuner_Receive(BYTE *addr,BYTE  bytecnt)
{
   BYTE   cnt;
   
   for (cnt = 0; cnt < (bytecnt - 1); cnt++)
   {
      *addr++ = IO_Tuner_Receive_Byte(CIO_IIC_ACK);
   }
   *addr = IO_Tuner_Receive_Byte(CIO_IIC_NACK);
}

/******************************************************************************
   ��   ��  ��   : IO_Tuner_Send
   ��       ��   : *addr �� ���M�ް��ޯ̧�߲��
               : bytecnt�� ���M�ް���
   ��   ��  �l   : OK �� ���M����I��
            : NG �� ACK�װ����
   �@ �\ �T ��   : Tuner�ް����M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
BYTE  IO_Tuner_Send(BYTE *addr, BYTE bytecnt)
{
   BYTE   cnt;
   BYTE   data;
   BYTE   ret;
   
   ret = CIO_IIC_ACK;
   for (cnt = 0; cnt < bytecnt; cnt++)
   {
      data = *addr++;
      ret = IO_Tuner_Send_Byte(data);
      if (ret == CIO_IIC_NACK)
      {
         break;
      }
   }
   
   if (ret == CIO_IIC_ACK)
   {
      return(OK);
   }
   else
   {
      return(NG);
   }
}

/******************************************************************************
   ��   ��  ��   : IO_Tuner_Send_Start
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : �X�^�[�g���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
void   IO_Tuner_Send_Start(void)
{
   MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
}

/******************************************************************************
   ��   ��  ��   : IO_Tuner_Send_Stop
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : �X�g�b�v���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
void   IO_Tuner_Send_Stop(void)
{
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( IN );            /* DATA��HIGH */
}

/******************************************************************************
   ��   ��  ��   : IO_Tuner_Send_Byte
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : 1BYTE���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
static BYTE   IO_Tuner_Send_Byte(BYTE data)
{
   BYTE   count;
   BYTE   ack;
   
   for(count = 0 ; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );      /* CLK��LOW */
      Cm_wait_loop(1);
      if ((data & 0x80) == 0x80)
      {
         MIO_CHG_pE2P_SDA( IN );      /* DATA��HIGH */
      }
      else
      {
         MIO_CHG_pE2P_SDA( OUT );   /* DATA��LOW */
      }
      Cm_wait_loop(0);
      MIO_CHG_pE2P_SCL( IN );         /* CLK��HIGH */
      Cm_wait_loop(3);
      data <<= 1;
   }

   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   Cm_wait_loop(3);
   MIO_CHG_pE2P_SDA( IN );            /* DATA��HIGH */
   MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
   Cm_wait_loop(2);
   ack = MIO_IN_pE2P_SDA();         /*   ACK�擾   */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   
   return(ack);
}

/******************************************************************************
   ��   ��  ��   : IO_Tuner_Receive_Byte
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : 1BYTE����M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
static BYTE   IO_Tuner_Receive_Byte(BYTE    ack)
{
   BYTE   count;
   BYTE   data;

   data = 0;
   for(count = 0; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
      data <<= 1;
      Cm_wait_loop(3);
      MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
      if (MIO_IN_pE2P_SDA() == HIGH)      /*1�r�b�g�V�t�g��M*/
      {
         data |= 0x01;
      }
      else
      {
         data &= 0xfe;
      }
      Cm_wait_loop(2);
   }
   
   MIO_CHG_pE2P_SCL( OUT );            /* CLK��LOW */
   
   Cm_wait_loop(1);
   if (ack == CIO_IIC_ACK)
   {
      MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   }
   else
   {
      MIO_CHG_pE2P_SDA( IN );            /* DATA��HIGH */
   }
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( IN );               /* CLK��HIGH */

   Cm_wait_loop(4);
   MIO_CHG_pE2P_SCL( OUT );            /* CLK��LOW */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SDA( IN );               /* DATA��HIGH */
   return(data);
}

/******************************************************************************
   ��   ��  ��   : IO_EVOL_Send
   ��       ��   : *addr �� ���M�ް��ޯ̧�߲��
               : bytecnt�� ���M�ް���
   ��   ��  �l   : OK �� ���M����I��
            : NG �� ACK�װ����
   �@ �\ �T ��   : EVOL�ް����M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
BYTE  IO_EVOL_Send(BYTE *addr, BYTE bytecnt)
{
   BYTE   cnt;
   BYTE   data;
   BYTE   ret;
   
   IO_EVOL_Send_Start();
   ret = CIO_IIC_ACK;
   for (cnt = 0; cnt < bytecnt; cnt++)
   {
      data = *addr++;
      ret = IO_EVOL_Send_Byte(data);
      if (ret == CIO_IIC_NACK)
      {
         break;
      }
   }
   IO_EVOL_Send_Stop();
   
   if (ret == CIO_IIC_ACK)
   {
      return(OK);
   }
   else
   {
      return(NG);
   }
}

/******************************************************************************
   ��   ��  ��   : IO_EVOL_Send_Start
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : �X�^�[�g���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
static void   IO_EVOL_Send_Start(void)
{
   MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
}

/******************************************************************************
   ��   ��  ��   : IO_EVOL_Send_Stop
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : �X�g�b�v���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
static void   IO_EVOL_Send_Stop(void)
{
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( IN );            /* DATA��HIGH */
}

/******************************************************************************
   ��   ��  ��   : IO_EVOL_Send_Byte
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : 1BYTE���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
static BYTE   IO_EVOL_Send_Byte(BYTE data)
{
   BYTE   count;
   BYTE   ack;
   
   for(count = 0 ; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );      /* CLK��LOW */
      Cm_wait_loop(1);
      if ((data & 0x80) == 0x80)
      {
         MIO_CHG_pE2P_SDA( IN );      /* DATA��HIGH */
      }
      else
      {
         MIO_CHG_pE2P_SDA( OUT );   /* DATA��LOW */
      }
      Cm_wait_loop(0);
      MIO_CHG_pE2P_SCL( IN );         /* CLK��HIGH */
      Cm_wait_loop(3);
      data <<= 1;
   }

   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   Cm_wait_loop(3);
   MIO_CHG_pE2P_SDA( IN );            /* DATA��HIGH */
   MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
   Cm_wait_loop(2);
   ack = MIO_IN_pE2P_SDA();         /*   ACK�擾   */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   
   return(ack);
}

#if _MODEL_EEPROM_
void IO_EEP_Port_Reset(void)
{
   /* DATA (0xaa) */
   static BYTE   ack;
   static BYTE   ret;
   BYTE   count;
   
   IO_EEP_SendStart();
   
   IO_EEP_WriteByte(0xFF);
   
   IO_EEP_SendRestart();
   
   IO_EEP_SendStop();
}

/******************************************************************************
   ��   ��  ��   : IO_EEP_SendStart
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : �X�^�[�g���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
void IO_EEP_SendStart( void )
{
   MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
}

/******************************************************************************
   ��   ��  ��   : IO_EEP_SendRestart
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : ���X�^�[�g���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
void IO_EEP_SendRestart( void )
{
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
   Cm_wait_loop(3);
   MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
}

/******************************************************************************
   ��   ��  ��   : IO_EEP_SendStop
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : �X�g�b�v���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
void IO_EEP_SendStop(void)
{
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( IN );            /* DATA��HIGH */
}

/******************************************************************************
   ��   ��  ��   : IO_EEP_WriteByte
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : 1BYTE���M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
BYTE   IO_EEP_WriteByte(BYTE   data)
{
   BYTE   count;
   BYTE   ack;
   
   Cm_wait_loop(2);

   for(count = 0 ; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );      /* CLK��LOW */
      Cm_wait_loop(1);
      if ((data & 0x80) == 0x80)
      {
         MIO_CHG_pE2P_SDA( IN );      /* DATA��HIGH */
      }
      else
      {
         MIO_CHG_pE2P_SDA( OUT );   /* DATA��LOW */
      }
      Cm_wait_loop(0);
      MIO_CHG_pE2P_SCL( IN );         /* CLK��HIGH */
      Cm_wait_loop(3);
      data <<= 1;
   }

   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   Cm_wait_loop(3);
   MIO_CHG_pE2P_SDA( IN );            /* DATA��HIGH */
   MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
   Cm_wait_loop(2);
   ack = MIO_IN_pE2P_SDA();         /*   ACK�擾   */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
   
   if (ack == CIO_IIC_NACK)
   {
      return(NG);
   }
   else
   {
      return(OK);
   }
}

/******************************************************************************
   ��   ��  ��   : IO_EEP_ReadByte
   ��       ��   : ����
   ��   ��  �l   : ����
   �@ �\ �T ��   : 1BYTE����M����
-------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
********************************************************************************/
static BYTE   IO_EEP_ReadByte(BYTE   ack)
{
   BYTE   count;
   BYTE   data;

   data = 0;
   for(count = 0; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );         /* CLK��LOW */
      data <<= 1;
      Cm_wait_loop(3);
      MIO_CHG_pE2P_SCL( IN );            /* CLK��HIGH */
      if (MIO_IN_pE2P_SDA() == HIGH)      /*1�r�b�g�V�t�g��M*/
      {
         data |= 0x01;
      }
      else
      {
         data &= 0xfe;
      }
      Cm_wait_loop(2);
   }
   
   MIO_CHG_pE2P_SCL( OUT );            /* CLK��LOW */
   
   Cm_wait_loop(1);
   if (ack == CIO_IIC_ACK)
   {
      MIO_CHG_pE2P_SDA( OUT );         /* DATA��LOW */
   }
   else
   {
      MIO_CHG_pE2P_SDA( IN );            /* DATA��HIGH */
   }
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( IN );               /* CLK��HIGH */

   Cm_wait_loop(4);
   MIO_CHG_pE2P_SCL( OUT );            /* CLK��LOW */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SDA( IN );               /* DATA��HIGH */
   return(data);
}

/*****************************************************************************************
   �֐���      �Fvoid IO_EEP_ReadData(BYTE *addr,BYTE  bytecnt)
   ����      �F *addr �� ��M�ް��ޯ̧�߲��
            �F bytecnt�� ��M�ް���
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM�ް���M����
-----------------------------------------------------------------------------------------
[chin20121009] �V�K�쐬
*****************************************************************************************/
void IO_EEP_ReadData(BYTE *addr,BYTE bytecnt)
{
   WORD   iCnt;
   
   for (iCnt = 0; iCnt < (bytecnt - 1); iCnt++)
   {
      *addr++ = IO_EEP_ReadByte(CIO_IIC_ACK);
   }
   *addr = IO_EEP_ReadByte(CIO_IIC_NACK);
}
#endif

#else   /* �o�X�t�@�C�g�΍� */
/******************************************************************************
   ��   ��  ��   : IO_Tuner_SetIICPort
   ��       ��   : status�F ON �� �ʐMPort�ɐݒ�
            :          OFF �� �ėp�߰Ăɐݒ�
   ��   ��  �l   : ����
   �@ �\ �T ��   : Tuner�ʐM�߰Đؑ֏���
    ��       �l : �ʐMPort�ɐݒ肷��ƁACLK��DATA�����o�͂�HIGH�ɐݒ肳���
-------------------------------------------------------------------------------
[sun100420] �V�K�쐬
********************************************************************************/
void  IO_Tuner_SetIICPort(BYTE status)
{
   if (status == ON)
   {
      pTU_SCL_FSA = 0;
      pTU_SCL_FSB = 0;
      pTU_SCL_LAT = 1;
      pTU_SCL_DDR = 1;
      
      pTU_SDA_FSA = 0;
      pTU_SDA_FSB = 0;
      pTU_SDA_LAT = 1;
      pTU_SDA_DDR = 1;
   }
   else
   {
      pTU_SCL_FSA = 0;
      pTU_SCL_FSB = 0;
      pTU_SCL_LAT = 0;
      pTU_SCL_DDR = 1;
      
      pTU_SDA_FSA = 0;
      pTU_SDA_FSB = 0;
      pTU_SDA_LAT = 0;
      pTU_SDA_DDR = 1;
   }
}
/******************************************************************************
   ��   ��  ��   : IO_Tuner_Receive
   ��       ��   : *addr �� ��M�ް��ޯ̧�߲��
               : bytecnt�� ��M�ް���
   ��   ��  �l   : ����
   �@ �\ �T ��   : Tuner�ް���M����
-------------------------------------------------------------------------------
[sun100420] �V�K�쐬
********************************************************************************/
void  IO_Tuner_Receive(BYTE *addr,BYTE  bytecnt)
{
   BYTE   cnt;
   
//   pTU_SDA_FSA = 0;
//   pTU_SCL_FSA = 0;

   for (cnt = 0; cnt < (bytecnt - 1); cnt++)
   {
      *addr++ = IO_Tuner_Receive_Byte(CIO_IIC_ACK);
   }
   *addr = IO_Tuner_Receive_Byte(CIO_IIC_NACK);
}

/******************************************************************************
   ��   ��  ��   : IO_Tuner_Send
   ��       ��   : *addr �� ���M�ް��ޯ̧�߲��
               : bytecnt�� ���M�ް���
   ��   ��  �l   : OK �� ���M����I��
            : NG �� ACK�װ����
   �@ �\ �T ��   : Tuner�ް����M����
-------------------------------------------------------------------------------
[20120730yamamoto] �V�K�쐬
********************************************************************************/
BYTE  IO_Tuner_Send(BYTE *addr, BYTE bytecnt)
{
   BYTE   cnt;
   BYTE   data;
   BYTE   ret;
   



   ret = CIO_IIC_ACK;
   for (cnt = 0; cnt < bytecnt; cnt++)
   {
      data = *addr++;
      ret = IO_Tuner_Send_Byte(data);
      if (ret == CIO_IIC_NACK)
      {
         break;
      }
   }
   
   if (ret == CIO_IIC_ACK)
   {
      return(OK);
   }
   else
   {
      return(NG);
   }
}

/* [sun100505] �ʐM���x�����@250kHz���炢 */
/* [sun100817] �ʐM���x�����@100kHz���炢 */
void   IO_Tuner_Send_Start(void)
{
   MIO_OUT_pTU_DATA(HIGH);
   MIO_OUT_pTU_CLK(HIGH);
   MIO_CHG_pTU_DATA(OUT);
   Cm_wait_loop(4);
   MIO_OUT_pTU_DATA(LOW);
   Cm_wait_loop(2);
   MIO_OUT_pTU_CLK(LOW);
}

/* [sun100505] �ʐM���x�����@250kHz���炢 */
/* [sun100817] �ʐM���x�����@100kHz���炢 */
void   IO_Tuner_Send_Stop(void)
{
   MIO_OUT_pTU_CLK(LOW);
   MIO_CHG_pTU_DATA(OUT);
   MIO_OUT_pTU_DATA(LOW);
   Cm_wait_loop(2);
   MIO_OUT_pTU_CLK(HIGH);
   Cm_wait_loop(4);
   MIO_OUT_pTU_DATA(HIGH);
}

/* [sun100505] �ʐM���x�����@250kHz���炢 */
/* [sun100817] �ʐM���x�����@100kHz���炢 */
static BYTE   IO_Tuner_Send_Byte(BYTE data)
{
   BYTE   bitcnt;
   BYTE   ack;
   
   MIO_OUT_pTU_CLK(LOW);
   MIO_CHG_pTU_DATA(OUT);
   for (bitcnt = 0; bitcnt < 8; bitcnt++ )
   {
      MIO_OUT_pTU_CLK(LOW);
      if ((data & 0x80) == 0x80)
      {
         MIO_OUT_pTU_DATA(HIGH);
      }
      else
      {
         MIO_OUT_pTU_DATA(LOW);
      }
      Cm_wait_loop(3);
      MIO_OUT_pTU_CLK(HIGH);
      data <<= 1;
      Cm_wait_loop(2);
   }
   MIO_OUT_pTU_CLK(LOW);
   MIO_OUT_pTU_DATA(LOW);
   Cm_wait_loop(0);
   MIO_CHG_pTU_DATA(IN);
   Cm_wait_loop(2);
   MIO_OUT_pTU_CLK(HIGH);
   Cm_wait_loop(2);
   ack = MIO_IN_pTU_DATA();
   return(ack);
}

/* [sun100505] �ʐM���x�����@250kHz���炢 */
/* [sun100817] �ʐM���x�����@100kHz���炢 */
static BYTE   IO_Tuner_Receive_Byte(BYTE    ack)
{
   BYTE   bitcnt;
   BYTE   data;

   MIO_OUT_pTU_CLK(LOW);
   MIO_CHG_pTU_DATA(IN);
   data = 0x00;
   for (bitcnt = 0; bitcnt < 8; bitcnt++ )
   {
      MIO_OUT_pTU_CLK(LOW);
      data <<= 1;
      Cm_wait_loop(2);
      MIO_OUT_pTU_CLK(HIGH);
      if (MIO_IN_pTU_DATA() == HIGH)
      {
         data |= 0x01;
      }
      else
      {
         data &= 0xfe;
      }
      Cm_wait_loop(1);
   }
   MIO_OUT_pTU_CLK(LOW);
   MIO_OUT_pTU_DATA(ack);
   MIO_CHG_pTU_DATA(OUT);
   Cm_wait_loop(2);
   MIO_OUT_pTU_CLK(HIGH);
   return(data);
}

/******************************************************************************
   ��   ��  ��   : IO_EVOL_Send
   ��       ��   : *addr �� ���M�ް��ޯ̧�߲��
               : bytecnt�� ���M�ް���
   ��   ��  �l   : ����
   �@ �\ �T ��   : EVOL�ް����M����
    ��       �l : 
-------------------------------------------------------------------------------
�C������
   �ԍ�   ���t       �S����      �C�����e
-------------------------------------------------------------------------------
[sun100420] �V�K�쐬
********************************************************************************/
void  IO_EVOL_SetIICPort(BYTE status)
{
   if (status == ON)
   {
      pEVOL_CLK_FSA = 0;
      pEVOL_CLK_FSB = 0;
      pEVOL_CLK_LAT = 1;
      pEVOL_CLK_DDR = 1;
      
      pEVOL_DATA_FSA = 0;
      pEVOL_DATA_FSB = 0;
      pEVOL_DATA_LAT = 1;
      pEVOL_DATA_DDR = 1;
   }
   else
   {
      pEVOL_CLK_FSA = 0;
      pEVOL_CLK_FSB = 0;
      pEVOL_CLK_LAT = 0;
      pEVOL_CLK_DDR = 1;
      
      pEVOL_DATA_FSA = 0;
      pEVOL_DATA_FSB = 0;
      pEVOL_DATA_LAT = 0;
      pEVOL_DATA_DDR = 1;
   }
}
/* [20120731yamamoto] �ʐM���x���� 100kHz���炢 */
BYTE  IO_EVOL_Send(BYTE *addr, BYTE bytecnt)
{
   BYTE   cnt;
   BYTE   data;
   BYTE   ret;
   
//   pEVOL_DATA_FSA = 0;
//   pEVOL_CLK_FSA = 0;
   IO_EVOL_Send_Start();
   ret = CIO_IIC_ACK;
   for (cnt = 0; cnt < bytecnt; cnt++)
   {
      data = *addr++;
      ret = IO_EVOL_Send_Byte(data);
      if (ret == CIO_IIC_NACK)
      {
         break;
      }
   }
   IO_EVOL_Send_Stop();
   
   if (ret == CIO_IIC_ACK)
   {
      return(OK);
   }
   else
   {
      return(NG);
   }
}
/* [20120731yamamoto] �ʐM���x���� 100kHz���炢 */
static void   IO_EVOL_Send_Start(void)
{
   MIO_OUT_pEVOL_DATA(HIGH);
   MIO_OUT_pEVOL_CLK(HIGH);
   MIO_CHG_pEVOL_DATA(OUT);
   Cm_wait_loop(4);
   MIO_OUT_pEVOL_DATA(LOW);
   Cm_wait_loop(2);
   MIO_OUT_pEVOL_CLK(LOW);
}

/* [20120731yamamoto] �ʐM���x���� 100kHz���炢 */
static void   IO_EVOL_Send_Stop(void)
{
   MIO_OUT_pEVOL_CLK(LOW);
   MIO_CHG_pEVOL_DATA(OUT);
   MIO_OUT_pEVOL_DATA(LOW);
   Cm_wait_loop(2);
   MIO_OUT_pEVOL_CLK(HIGH);
   Cm_wait_loop(4);
   MIO_OUT_pEVOL_DATA(HIGH);
}

/* [20120731yamamoto] �ʐM���x���� 100kHz���炢 */
static BYTE   IO_EVOL_Send_Byte(BYTE data)
{
   BYTE   bitcnt;
   BYTE   ack;
   
   MIO_OUT_pEVOL_CLK(LOW);
   MIO_CHG_pEVOL_DATA(OUT);
   for (bitcnt = 0; bitcnt < 8; bitcnt++ )
   {
      MIO_OUT_pEVOL_CLK(LOW);
      if ((data & 0x80) == 0x80)
      {
         MIO_OUT_pEVOL_DATA(HIGH);
      }
      else
      {
         MIO_OUT_pEVOL_DATA(LOW);
      }
      Cm_wait_loop(3);
      MIO_OUT_pEVOL_CLK(HIGH);
      data <<= 1;
      Cm_wait_loop(2);
   }
   MIO_OUT_pEVOL_CLK(LOW);
   MIO_OUT_pEVOL_DATA(LOW);
   Cm_wait_loop(0);
   MIO_CHG_pEVOL_DATA(IN);
   Cm_wait_loop(2);
   MIO_OUT_pEVOL_CLK(HIGH);
   Cm_wait_loop(2);
   ack = MIO_IN_pEVOL_DATA();
   return(ack);
}

#if _MODEL_EEPROM_
/*****************************************************************************************
   �֐���      �Fvoid IO_EEP_SendStart( void )
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM��ײ��Main����
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
*****************************************************************************************/
void IO_EEP_SendStart( void )
{
   MIO_OUT_pE2P_SDA( HIGH );         /*   EEPROM�f�[�^�|�[�g��HIGH�o��   */
   MIO_OUT_pE2P_SCL( HIGH );         /*   EEPROM�N���b�N�|�[�g��HIGH�o��   */
   MIO_CHG_pE2P_SDA( OUT );         /*   EEPROM�f�[�^�|�[�g�o�̓|�[�g�ɕϊ�   */
   //Cm_wait_loop(3);
   MIO_OUT_pE2P_SDA( LOW );         /*   EEPROM�f�[�^�|�[�g��LOW�o��      */
   Cm_wait_loop(1);
   MIO_OUT_pE2P_SCL( LOW );         /*   EEPROM�N���b�N�|�[�g��HIGH�o��   */
}
/*****************************************************************************************
   �֐���      �Fvoid IO_EEP_SendRestart( void )
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM��ײ��Main����
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
*****************************************************************************************/
void IO_EEP_SendRestart( void )
{
   MIO_OUT_pE2P_SCL( LOW );
   Cm_wait_loop(2);
   MIO_OUT_pE2P_SDA( HIGH );
   MIO_CHG_pE2P_SDA( OUT );
   Cm_wait_loop(2);
   MIO_OUT_pE2P_SCL( HIGH );
   Cm_wait_loop(4);
   MIO_OUT_pE2P_SDA( LOW );
   Cm_wait_loop(3);
   MIO_OUT_pE2P_SCL( LOW );
}

/*****************************************************************************************
   �֐���      �Fvoid IO_EEP_SendStop(void)
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM��ײ��Main����
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
*****************************************************************************************/
void IO_EEP_SendStop(void)
{
   MIO_OUT_pE2P_SCL( LOW );         /*   EEPROM�N���b�N�|�[�g��HIGH�o��   */
   MIO_CHG_pE2P_SDA( OUT );      /*   EEPROM�f�[�^�|�[�g�o�̓|�[�g�ɕϊ�   */
   MIO_OUT_pE2P_SDA( LOW );         /*   EEPROM�f�[�^�|�[�g��HIGH�o��   */
   Cm_wait_loop(1);
   MIO_OUT_pE2P_SCL( HIGH );         /*   EEPROM�N���b�N�|�[�g��HIGH�o��   */
   Cm_wait_loop(1);
   MIO_OUT_pE2P_SDA( HIGH );         /*   EEPROM�f�[�^�|�[�g��LOW�o��      */
}
/*****************************************************************************************
   �֐���      �FBYTE   IO_EEP_SendByteData(BYTE   data)
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM��ײ��Main����
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
*****************************************************************************************/
BYTE   IO_EEP_WriteByte(BYTE   data)
{
   BYTE   count;
   BYTE   ack;
   
   //MIO_OUT_pE2P_SCL( LOW );      /*EEPROM�N���b�N�|�[�g��LOW�o��*/
   MIO_CHG_pE2P_SDA( OUT );      /*   EEPROM�f�[�^�|�[�g�o�̓|�[�g�ɕϊ�   */
   Cm_wait_loop(2);

   for(count = 0 ; count < 8; count++)
   {
      MIO_OUT_pE2P_SCL( LOW );      /*EEPROM�N���b�N�|�[�g��LOW�o��*/
      if ((data & 0x80) == 0x80)
      {
         MIO_OUT_pE2P_SDA(HIGH);      /*1�r�b�g�V�t�g�ő��M*/
      }
      else
      {
         MIO_OUT_pE2P_SDA(LOW);   /*1�r�b�g�V�t�g�ő��M*/
      }
      Cm_wait_loop(1);
      MIO_OUT_pE2P_SCL( HIGH );      /*EEPROM�N���b�N�|�[�g��HIGH�o��*/
      Cm_wait_loop(1);
      data <<= 1;
   }

   MIO_OUT_pE2P_SCL( LOW );      /*   EEPROM�N�b���N�|�[�g��LOW�o��      */
   MIO_OUT_pE2P_SDA(LOW);   /*1�r�b�g�V�t�g�ő��M*/
   MIO_CHG_pE2P_SDA( IN );         /*   EEPROM�f�[�^�|�[�g���̓|�[�g�ɕϊ�   */
   Cm_wait_loop(0);
   MIO_OUT_pE2P_SCL( HIGH );      /*   EEPROM�N�b���N�|�[�g��HIGH�o��      */
   ack = MIO_IN_pE2P_SDA();         /*   ACK�擾                        */
   if (ack == CIO_IIC_NACK)
   {
      return(NG);
   }
   else
   {
      return(OK);
   }
}


/*****************************************************************************************
   �֐���      �FBYTE   IO_EEP_ReadByte(BYTE   ack)
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM��ײ��Main����
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
*****************************************************************************************/
BYTE   IO_EEP_ReadByte(BYTE   ack)
{
   BYTE   count;
   BYTE   data;

   MIO_OUT_pE2P_SCL( LOW );      /*EEPROM�N���b�N�|�[�g��LOW�o��*/
   MIO_CHG_pE2P_SDA(IN);            /*EEPROM�f�[�^�|�[�g���̓|�[�g�ɕϊ�*/

   data = 0;
   for(count = 0; count < 8; count++)
   {
      MIO_OUT_pE2P_SCL( LOW );      /*EEPROM�N���b�N�|�[�g��LOW�o��*/
      data <<= 1;
      Cm_wait_loop(1);
      MIO_OUT_pE2P_SCL( HIGH );      /*EEPROM�N���b�N�|�[�g��HIGH�o��*/
      if (MIO_IN_pE2P_SDA() == HIGH)      /*1�r�b�g�V�t�g��M*/
      {
         data |= 0x01;
      }
      else
      {
         data &= 0xfe;
      }
      Cm_wait_loop(0);
   }
   
   MIO_OUT_pE2P_SCL( LOW );      /*   EEPROM�N�b���N�|�[�g��LOW�o��      */
   MIO_OUT_pE2P_SDA(ack);         /*1�r�b�g�V�t�g�ő��M*/
   MIO_CHG_pE2P_SDA( OUT );      /*   EEPROM�f�[�^�|�[�g���̓|�[�g�ɕϊ�   */
   Cm_wait_loop(1);
   MIO_OUT_pE2P_SCL( HIGH );      /*   EEPROM�N�b���N�|�[�g��HIGH�o��      */
   return(data);
}

/*****************************************************************************************
   �֐���      �Fvoid IO_EEP_ReadData(BYTE *addr,BYTE  bytecnt)
   ����      �F�Ȃ�
   ���^�[��   �F�Ȃ�
   �@�\�T�v   �FEEPROM��ײ��Main����
-----------------------------------------------------------------------------------------
[sun100806] �V�K�쐬
[sun100823] н�C���F�@bytecnt��BYTE��WORD�ɕϊ�
*****************************************************************************************/
void IO_EEP_ReadData(BYTE *addr,BYTE bytecnt)
{
   WORD   iCnt;
   
   for (iCnt = 0; iCnt < (bytecnt - 1); iCnt++)
   {
      *addr++ = IO_EEP_ReadByte(CIO_IIC_ACK);
   }
   *addr = IO_EEP_ReadByte(CIO_IIC_NACK);
}


#endif

#endif   /* �o�X�t�@�C�g�΍� */

/******************************************************************************
   ��   ��  ��   : IO_lcd_ccb
   ��       ��   : adr �� 1byte���M�ް�(CCB���ڽ�p)
   ��   ��  �l   : OK    �� ��������
              : NG    �� �������s
   �@ �\ �T ��   : LCD�h���C�o�ւP�o�C�g �ް��𑗐M���܂��B
    ��       �l : LSB�擪�ő��M
-------------------------------------------------------------------------------
�C������
   �ԍ�   ���t       �S����      �C�����e
-------------------------------------------------------------------------------
  �y000�z   12/08/03   ha         �V�K�쐬
********************************************************************************/
BYTE IO_lcd_ccb( BYTE adr )
{
   BYTE   roop_cnt;
   
   
   #if 0
   /* port�ݒ� 
      P10: SIO0�f�[�^�iCMOS)   
      P11: �I�[�v��         
      P12: SIO0�N���b�N(CMOS)   */
   __bf_P1LAT.BIT2 = 0;
   __bf_P1DDR.BIT2 = 1;
   __bf_P1FSA.BIT2 = 1;
   __bf_P1FSB.BIT2 = 0;
   
   __bf_P1LAT.BIT1 = 0;
   __bf_P1DDR.BIT1 = 1;
   __bf_P1FSA.BIT1 = 0;
   __bf_P1FSB.BIT1 = 0;
   
   __bf_P1LAT.BIT0 = 0;
   __bf_P1DDR.BIT0 = 1;
   __bf_P1FSA.BIT0 = 1;
   __bf_P1FSB.BIT0 = 0;
   /* �{�[���[�g�N���b�N�ݒ� */
   __byte_S0BG = 0x0D;      /* 850kHz */
   #endif
   
   __EnableInterrupt(0);

   __bf_IL1H.IRQ6  = 0x03;  // thiet lap level IRQ6 = 3
   
   __byte_S0CNT    = 0x01;      /* ���M���[�h    LSB    an intterrupt request to vector address 08038H is generated, chon transimission mode, chon LSB first */
   __byte_S0INTVL  = 0x00;      /*  Ket hop cac bit WAKEUP, AUTO, REC cua S0CNT va bit XCHNG cua S0INTVL de chon mode, o day cac bit la 0000 => mode Transmission*/
   
   __byte_S0BUF     = adr;  // Gui data la dia chi

   __bf_S0CNT.RUN = 1;      /* ���M�J�n         �@    This bit must be set with an instruction. O mode 0, Khi ket thuc viec truyen, bit RUN se tu dong clear. */
   
   for ( roop_cnt=0; roop_cnt< 0xff ; roop_cnt++ )
   {
      if( __bf_S0CNT.FLG == ON )        // Khi truyen xong thi FLG tu dong len 1
      {
         __bf_S0CNT.FLG = OFF ;      /* ���M�����܂�ٰ�� */
         return OK ;
      }
   }

   return NG ;         /* time out */
}
/******************************************************************************
   ��   ��  ��   : Io_sio_lcd_send
   ��       ��   : *tx_data �� ���M�ް��ޯ̧�߲��
               : tx_cnt    �� ���M�ް�Byte���i�P�`�R�Q�j
   ��   ��  �l   : OK    �� ��������
              : NG    �� �������s
   �@ �\ �T ��   : LCD�h���C�o�֎w��o�C�g���ް��𑗐M���܂��B
    ��       �l : ���M�ް�Byte���́AMax32byte�BLSB�擪�ő��M
-------------------------------------------------------------------------------
�C������
   �ԍ�   ���t       �S����      �C�����e
-------------------------------------------------------------------------------
  �y000�z   12/08/03   ha         �V�K�쐬
********************************************************************************/
BYTE IO_lcd_send( BYTE *tx_data, BYTE tx_cnt )
{
   BYTE   roop_cnt;
   BYTE   i;
   
   i = 0;
   #if 0
   /* port�ݒ� 
      P10: SIO0�f�[�^�iCMOS)   
      P11: �I�[�v��         
      P12: SIO0�N���b�N(CMOS)   */
   __bf_P1LAT.BIT2 = 0;
   __bf_P1DDR.BIT2 = 1;
   __bf_P1FSA.BIT2 = 1;
   __bf_P1FSB.BIT2 = 0;
   
   __bf_P1LAT.BIT1 = 1;
   __bf_P1DDR.BIT1 = 1;
   __bf_P1FSA.BIT1 = 0;
   __bf_P1FSB.BIT1 = 0;
   
   __bf_P1LAT.BIT0 = 0;
   __bf_P1DDR.BIT0 = 1;
   __bf_P1FSA.BIT0 = 1;
   __bf_P1FSB.BIT0 = 0;

   /* �{�[���[�g�N���b�N�ݒ� */
   __byte_S0BG = 0x0D;      /* 850kHz */
   #endif
   
   memcpy( io_sio_buf, tx_data, tx_cnt );   /* ���M�ް��ݒ�  io_sio_buf la mang co 32 phan tu kieu byte. io_sio_buf trong file IO_sfr*/
   
   __EnableInterrupt(0);

   __bf_IL1H.IRQ6  = 0x03;
   
   __byte_S0CNT    = 0x01;      /* ���M���[�h    LSB     */
   __byte_S0INTVL  = 0x00;
   
   
   for(i=0;i < tx_cnt;i++)
   {
      __byte_S0BUF     = io_sio_buf[i];
   
      __bf_S0CNT.RUN = 1;      /* ���M�J�n         �@ */
      
      for ( roop_cnt=0; roop_cnt< 0xff ; roop_cnt++ )
      {
         if( __bf_S0CNT.FLG == ON )
         {
            __bf_S0CNT.FLG = OFF ;      /* ���M�����܂�ٰ�� */
         }
      }
   }
   return OK ;         /* time out */
}

/******************************************************************************
   ��   ��  ��   : IO_lcd_receive
   ��       ��   : *rx_data �� ��M�ް��ޯ̧�߲��
   ��   ��  �l   : OK    �� ��������
              : NG    �� �������s
   �@ �\ �T ��   : LCD�h���C�o����S�o�C�g�ް�����M���܂��B
    ��       �l : �߂�l��NG�̏ꍇ�A��M�ޯ̧�̓��e�͕ω����܂���B
-------------------------------------------------------------------------------
�C������
   �ԍ�   ���t       �S����      �C�����e
-------------------------------------------------------------------------------
  �y000�z   12/08/03   ha         �V�K�쐬
********************************************************************************/
BYTE  IO_lcd_receive(BYTE *rx_data)
{
   WORD   roop_cnt;
   BYTE   i;
   if( rx_data == 0 )
   {
      return NG ;         /* ���Ұ��װ   Parameter error */
   }
   
   #if 0
   /* port�ݒ� 
      P10: SIO0�f�[�^�iCMOS)   
      P11: SIO0�f�[�^�iCMOS�j   
      P12: SIO0�N���b�N(CMOS)   */
   __bf_P1LAT.BIT2 = 0;
   __bf_P1DDR.BIT2 = 1;
   __bf_P1FSA.BIT2 = 1;
   __bf_P1FSB.BIT2 = 0;
   
   __bf_P1LAT.BIT1 = 1;
   __bf_P1DDR.BIT1 = 1;
   __bf_P1FSA.BIT1 = 1;
   __bf_P1FSB.BIT1 = 1;
   
   __bf_P1LAT.BIT0 = 0;
   __bf_P1DDR.BIT0 = 1;
   __bf_P1FSA.BIT0 = 0;
   __bf_P1FSB.BIT0 = 0;
   
   /* �{�[���[�g�N���b�N�ݒ� */
   __byte_S0BG = 0x0D;      /* 850kHz */
   #endif
   
   /* �]���� */
   i = 0;
   __EnableInterrupt(0);
   __bf_IL1H.IRQ6  = 1;
   
   
   __byte_S0INTVL  = 0x00;      /* 32bit��M     nhan 32 bit    */
   
   __byte_S0BUF   = 0x00;
   __byte_S0CNT   = 0x41;      /* ��M���[�h    LSB    Thiet lap mode Receice va nhan o che do LSB    */
   
//   __bf_S0CNT.RUN = 1;      /* ��M�J�n         �@ */
   
   /*Nhan data tu LCD driver dua vao SOBUF. Se mat 4 lan nhan de dua vao 4 byte cua key_lcd_driver_data[]*/
   for(i = 0;i< 4;i++)
   {
      __bf_S0CNT.RUN = 1;
      for ( roop_cnt=0; roop_cnt< 0x00ff; roop_cnt++ )
      {
         if( __bf_S0CNT.FLG == ON )
         {
            /* ��M���� */
            *(rx_data + i)     = __byte_S0BUF ;
            __bf_S0CNT.FLG = OFF ;
         }
      }
   }

   return OK ;         /* time out */
}


/* ���荞�݊֘A */
/******************************************************************************
   ��   ��  ��   : Io_sfr_initial_set
   ��       ��   : void
   ��   ��  �l   : void
   �@ �\ �T ��   : �Ƽ�َ���SFR�ݒ���s���܂��       Thuc hien viec thiet lap SFR luc khoi tao
    ��       �l :
-------------------------------------------------------------------------------
[20120730yamamoto] �V�K�쐬
********************************************************************************/
void Io_sfr_initial_set(void)    // xem thu vien sfr884200 trong phan mem SanyoXstomrmy
{
   volatile DWORD   loopcnt;
   DWORD tmp;
   sWORD dif_q;
   
   /* WDT��~ */
   __byte_WDTCR = BIN_00000000;   /*   __byte_WDTCR  xem them thu vien sfr884200
                   |||||||+--- START   1:WDT START (0�̏������݂Œ�~�ł��Ȃ�)
                   ||||||+---- OVF     0:�\�����o��ؾ�ċ֎~
                   |||||+----- USERFLG 0:�Œ�
                   ||||+------ PDNSTOP 0:HOLDX�˓���WDTؾ�� 1:HOLDX�˓���WDT��~
                   |||+------- SRFLG   0:ؾ�Ď��s���o�׸�
                   ||+-------- MDSEL   0:�������Ӱ�� 1:ؾ��Ӱ��
                   |+--------- -       0:�Œ�
                   +---------- -       0:�Œ�  */
   
   /* PLL VCO����ڼ޽��A�Œ�l Thanh ghi dieu khien PLL VCO, gia tri co dinh */
   __byte_VCOCNT0 = 0x02;
   __byte_VCOCNT1 = 0x04;
   __byte_VCOCNT2 = 0x35;
   __byte_VCOCNT3 = 0xB6;
   __byte_VCOCNT4 = 0x00;
   
   /* 32k���U��OSC0��OSC1 Start */
   __byte_OCR0 = BIN_10000011;      /* 32k���U��OSC0 Start ---  chon System clock la RC Ocsillator, chon OSC0 la General purpose port, OSC1 la VCO va Khoi dong ch�ng
                                       Clock source VCO duoc lay ra tu pin LPFO
                                    */
   
   Io_port_initial_set();

   
#if 0   /* ����ؾ�ċ@�\�g�p �����ݒ肵�Ȃ� */
   __byte_TMPCNT0 |= BIN_01000000;      /* Bit6  LVDSTOP */
#endif
   
     /* ���U���� Total 1000ms�ȏ�  1s On dinh dao dong  */
   for (loopcnt = 0; loopcnt < 0xAf00; loopcnt++)
   {
   }
   
#if 0   /* EEPROM�̂��߁AOSC1���U� ��Ȃ��Ƃ����Ȃ� */
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;   /* ���Ѹۯ��I���FRC���U(1MHz) */
   
   #if 0   /* RC���U�͊��ɓ����Ă� */
   __bf_OCR0.RCSTOP= 0;   /* RC���U�J�n */
   #endif
#else
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC1;  // Chon xung nguon la OSC1, bo RC Oscillator 
   __bf_OCR0.RCSTOP = __bfval_OCR0_RCSTOP_STOP;  // ngung RC Oscillator
#endif
   
   __byte_OCR1 = BIN_00000000;         /* timer8�̸ۯ��I��(OSC0)/���Ѹۯ���������   Lua chon xung thach anh 32K cho timer8 voi ti le chia 1/32.
                                                                                    Day la xung cung cap cho WDT
                                                                                    Ko chia xung he thong ma giu nguyen */

   /* ���荞�݃��x���ݒ� 3:X 2:H 1:L 0:�֎~ */
   __byte_IL1L = BIN_00101011;         /* IRQ3:0  T0 :2  BT :2  WDT:3    Interrupt level setting register 1L*/
   __byte_IL1H = BIN_11010000;         /* IRQ7:3 IRQ6:1 IRQ5:0 IRQ4:0 Interrupt level setting register 1H*/
   __byte_IL2L = BIN_00000000;         /* IRQB:0 IRQA:0 IRQ9:0 IRQ8:0 Interrupt level setting register 2L*/
   __byte_IL2H = BIN_01000000;         /* IRQF:1 IRQE:0 IRQD:0 IRQC:0 Interrupt level setting register 2H*/

   __byte_INT01CR = BIN_00000000;      /* INT0,INT1���g�p  Khong dung den INT0,INT1 (IRQ3 duoc set bang 0 = Disabled) */
   __byte_INT23CR = BIN_10000000;      /* INT2���g�p�CINT3���荞�ݐݒ�    Khong dung den INT2, thiet lap ngat falling edge detection INT3*/
   /* INT3��Acc_Det�[�q�́u������v���o����悤�ɐݒ�    INT3: d�ng de kiem tra canh xuong cua ACC_DET */

   #if 0      /* ���x���� */
   __byte_BTCR = BIN_01110101;         /* T8 2048TBST(62.5ms),WDT 8192TBST */
   #else
   __byte_BTCR = BIN_01010100;         /* T8 128TBST(3.9ms),WDT 8192TBST(0.25ms) */
   #endif
/*                  |||||||+--- IE0   0:BaseTime0�����݋֎~
                  ||||||+---- FLG0  0:
                  |||||+----- IE1   0:BaseTime1�����݋֎~
                  ||||+------ FLG1  0:
                  |||+------- CNT   11:BT1�i2048TBST�j
                  ||+-------- CNT   
                  |+--------- RUN   1:����J�n
                  +---------- FST   0:���g�p  */

   /* T0 8bit BEEP�p 4KHz */
   __byte_T0LR  = 0x00;               /* T0���쎞:8bitPWM    */
   __byte_T0HR  = 0x00;               /* T0���쎞:��~    */
   __byte_T0CNT = BIN_00010000;         /* T0���ĸۯ� OSC0   Chon xung nguon la thach anh . Luu y la xung nguon thach anh co ty le chia tan so la 1/32 => timer0 dung xung 1024Hz*/
   __byte_T0PR  = BIN_01100000;         /* MODE3,T0PR���� = (PR+1)�~���ĸۯ�   MODE3: TOL operation 8bit Toggle, TOH: Stopped*/

   /* T1 16BIT TIMER ACCOFF��ACC_DET�[�q�`���^�p  Dung cho viec chatter ng� ACC_DET khi ACCOFF*/
   /* (0x270F(9999)+1)�~1um = 10ms */
   __byte_T1LR  = 0x0F;
   __byte_T1HR  = 0x27;
   __byte_T1PR  = BIN_00000000; // 
   __byte_T1CNT = BIN_00001101; // dung xung internal RC 1Mhz
   
   /* T2 16BIT TIMER  (0x960+1)*(4+1)*83.3ns = 1ms  (0x9601 = 2401 he thap phan)*/
   __byte_T2LR   = 0x60;
   __byte_T2HR   = 0x09;
   __byte_T2CNT0 = BIN_00000000;
   __byte_T2CNT2 = BIN_00000100;
   
   /* �����b����  ngay phut giay counter */
   __byte_DMSCNT = BIN_00000000;
   __byte_SEC    = BIN_00000000;
   __byte_MINL   = BIN_00000000;
   __byte_MINH   = BIN_00000000;
   __byte_DAYL   = BIN_00000000;
   __byte_DAYH   = BIN_00000000;
   __byte_DMSCNT = BIN_10000000;
   
   #if 1
   /* 25�xA/D�ϊ� �ް����� 8bit�i�[ڼ޽�2 */
   __byte_CALC0L    = __byte_TEMPCONV0;
   /* 25�xA/D�ϊ� �ް����� 4bit�i�[ڼ޽�2 */
   __byte_CALC0H    = __byte_TEMPCONV1;
   
   dif_q = __word_TEMPCONV0 - __word_TEMPCONV2;
   
   if( dif_q < 0 )
   {
      dif_q = -dif_q;
   }
   
   tmp = COMPENSATE_CYCLE * BHOT;
   tmp = tmp / (WORD)dif_q;
   tmp = tmp * DIF_TEMPERTURE;
   tmp = tmp / (WORD)dif_q;
   tmp = tmp * DIF_TEMPERTURE;   
   tmp = tmp / BIT_ADJUST;
   
   /*�������␳����*/
   __word_CALC1 = (WORD)tmp;
   
   tmp = COMPENSATE_CYCLE * BCOLD;
   tmp = tmp / (WORD)dif_q;
   tmp = tmp * DIF_TEMPERTURE;
   tmp = tmp / (WORD)dif_q;
   tmp = tmp * DIF_TEMPERTURE;   
   tmp = tmp / BIT_ADJUST;
   
   /*�ቷ���␳����*/
   __word_CALC4 = (WORD)tmp;
   
   /* �␳�ʒ���ڼ޽�(�Ƃ肠����0�ɐݒ�) */
   __byte_CALC2     = BIN_00000000;
   /* A/D�ϊ����Ď��Ԑݒ�ڼ޽� */
   __byte_CALC3     = BIN_00000011;
   
   #endif
   
   /* ���v��� ���v��ϒ�~�����Ēl�ێ�,�����ݎ��� 500ms  Clock timer  Giu lai gia tri counter khi stop clock timer, chu ki ngat 500ms */
   __byte_CKTIMCTRL = BIN_10001110;

   
   /* SIO0 */
   __byte_S0CNT = BIN_00000000;  // LSB First
   /* �{�[���[�g�N���b�N�ݒ� Thiet lap toc do Baud*/
   __byte_S0BG = 0x0D;      /* 850kHz */
   __byte_S0BUF = 0x00;
   __byte_S0INTVL = BIN_00000000;
   
   
   /* UART2 */
   __byte_U2CNT0 = BIN_00000000;
   __byte_U2CNT1 = BIN_00000100;
   __byte_U2TBUF = BIN_00000000;
   __byte_U2BG   = CIO_UART_19200;      /*19200bps��12MHz/{(U2BG+1)�~4}*/
   
#if ( _WDT_ == 1 )   // _WDT_ duoc define trong model.h
   /* WDT�J�n */
   __byte_WDTCR = BIN_00101001;   /*      WDT is in reset mode (bit 5(MDSEL) = 1)
                   |||||||+--- START   1:WDT START (0�̏������݂Œ�~�ł��Ȃ�)
                   ||||||+---- OVF     0:�\�����o��ؾ�ċ֎~
                   |||||+----- USERFLG 0:�Œ�
                   ||||+------ PDNSTOP 0:HOLDX�˓���WDTؾ�� 1:HOLDX�˓���WDT��~
                   |||+------- SRFLG   0:ؾ�Ď��s���o�׸�
                   ||+-------- MDSEL   0:�������Ӱ�� 1:ؾ��Ӱ��
                   |+--------- -       0:�Œ�
                   +---------- -       0:�Œ�  */
   /* WDT �����ر  WDT conuter clear */
   __SFR_BITCLR(__WDTCR,0);  // lenh nam trong file cpuinsn.h o link SanyoX16\Gcc\SanyoInc\itron
#endif
}


/******************************************************************************
   ��   ��  ��   : Io_port_initial_set
   ��       ��   : �Ȃ�
   ��   ��  �l   : �Ȃ�
   �@ �\ �T ��   : �S�Ă��߰Ă̏�Ԃ������ݒ�ɂ��܂��INITAIL���ɺ�ق��ĉ������B ---- Thiet lap gia tri cua toan bo PORT vao thoi giam ban dau. Xin hay goi khi Khoi tao 
    ��       �l : �ݒ�l�́Aio_port_ext.h�Q�ƁB   Gia tri thiet lap tham khao file io_port_ext.h
-------------------------------------------------------------------------------
   �C�������y�N�����z�yVer�z�y���O�z �y�����z
-------------------------------------------------------------------------------
[sun100419] 16�ޯ�ϲ�ݗp�V�K�쐬
*******************************************************************************/
void  Io_port_initial_set( void )
{
   pP0_LAT_BYTE = CP0_LAT_INI;        //00000001
   pP0_DDR_BYTE = CP0_DDR_INI;        //11111101 
   pP0_FSA_BYTE = CP0_FSA_INI;        //11111111
   pP0_23INT_BYTE = CP0_23INT_INI;    //00000000
   
   pP1_LAT_BYTE = CP1_LAT_INI;        //01000010
   pP1_DDR_BYTE = CP1_DDR_INI;        //11000111
   pP1_FSA_BYTE = CP1_FSA_INI;        //11000111
   pP1_FSB_BYTE = CP1_FSB_INI;        //01000010

   #if 0
   __bf_P0LAT.BIT4 = 0;
   __bf_P0DDR.BIT4 = 0;
   
   /* ACC_DET */
   __bf_P1FSA.BIT5 = 0;
   __bf_P1FSB.BIT5 = 0;
   __bf_P1LAT.BIT5 = 0;
   __bf_P1DDR.BIT5 = 0;
   #endif
}
#if 0
void XXXXXXXXXXXXXX(void)
#endif

#if 0   /* ���g�p */
void  Io_int_initial_set( void )
{
   /* �ް���� 62.5ms start */
   __bf_BTCR.IE1 = 1;
   __bf_BTCR.RUN = __bfval_BTCR_RUN_RUN;
   
   /* ���v��ϊ����݋֎~ */
   __byte_CKTIMCTRL = BIN_10001110;
   
   /* ALARM_KEY�����݋֎~ */
   #if 0
   __bf_P0LAT.BIT4  = 0;
   __bf_P0DDR.BIT4  = 0;
   #endif
   __bf_P0FSA.P04IL = 0;
   __bf_P0FSA.P04IE = 0;
   
   /* ACC_DET�����݋��� */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = 0;
   
}
#endif

/******************************************************************************
ACC OFF(�ʏ�) �� HOLDX
*******************************************************************************
   �֐���   �FIo_xxx_holdx_in
   ��  ��   �F����
   �߂�l   �F����
   �@  �\   �FHOLDX IN ����ڼ޽��ݒ菈��
------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
******************************************************************************/
void   Io_sfr_holdx_in( void )
{
   #if 0 /* HOLDX�ɓ���ƁA���䂵�Ȃ��Ă悢 */
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC0;   /* ���Ѹۯ��I���FOSC0(32KHz) */
   __bf_OCR0.RCSTOP = 1;   /* RC���U��~ */
   #endif
   
   __HoldX();
}
void   Io_port_holdx_in( void )
{
   /* LCD_DO LOW�o�͐ݒ� */
   pLCD_DO_FSA = 0;
   pLCD_DO_FSB = 0;
   pLCD_DO_LAT = 0;
   pLCD_DO_DDR = 1;
   /* LCD_CL LOW�o�͐ݒ� */
   pLCD_CL_FSA = 0;
   pLCD_CL_FSB = 0;
   pLCD_CL_LAT = 0;
   pLCD_CL_DDR = 1;
}
void   Io_int_holdx_in( void )
{
   Io_beep_stop();
   
   /* �ް���� 62.5ms Stop ACC_DET�`���^�p */
   //__bf_BTCR.RUN = 0;
   __bf_BTCR.IE1 = 0;
   
   /* ���v��ϊ����݊J�n(500ms) */
   __byte_CKTIMCTRL = BIN_10001111;
   
   /* 10ms�^�C�}���荞�ݒ�~������ */
   __bf_T1CNT.IE = 0;
   __bf_T1CNT.RUN = 0;
   
   /* ACC_DET�����݋��� */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_ENABLE;
   
   /* ALARMKEY���o�����݋��iAMFM���W�I�̂݁j */
   #if 0
   if ((Ma_model_dest() >= 2)&&(Ma_model_dest() <= 6))
   #else
   if (Ma_get_alarm() == TRUE)
   #endif
   {
      __bf_P0FSA.P04IE = 1;
      __bf_P0FSA.P04IL = 0;      /* L���x�� */
   }
}

/******************************************************************************
HOLDX �� ACC OFF(�ʏ�)
*******************************************************************************
   �֐���   �FIo_xxx_holdx_out
   ��  ��   �F����
   �߂�l   �F����
   �@  �\   �FHOLDX OUT ����ڼ޽��ݒ菈��
------------------------------------------------------------------------------
******************************************************************************/
void   Io_sfr_holdx_out(void)
{
#if ( _WDT_ == 1 )
   /* WDT�J�n */
   __byte_WDTCR = BIN_00101001;   /*
                   |||||||+--- START   1:WDT START (0�̏������݂Œ�~�ł��Ȃ�)
                   ||||||+---- OVF     0:�\�����o��ؾ�ċ֎~
                   |||||+----- USERFLG 0:�Œ�
                   ||||+------ PDNSTOP 0:HOLDX�˓���WDTؾ�� 1:HOLDX�˓���WDT��~
                   |||+------- SRFLG   0:ؾ�Ď��s���o�׸�
                   ||+-------- MDSEL   0:�������Ӱ�� 1:ؾ��Ӱ��
                   |+--------- -       0:�Œ�
                   +---------- -       0:�Œ�  */
   /* WDT �����ر  WDT counter clear */
   __SFR_BITCLR(__WDTCR,0);
#endif

   #if 0 /* HOLDX�ɓ���ƁA���䂵�Ȃ��Ă悢 */
   __bf_OCR0.RCSTOP = 0;   /* RC���U�J�n */
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;   /* ���Ѹۯ��I���FRC���U(1MHz) */
   #endif
}
void   Io_port_holdx_out( void )
{
   /* LCD_DO SIO�f�[�^(CMOS)�ݒ� */
   pLCD_DO_FSA = 1;
   pLCD_DO_FSB = 0;
   pLCD_DO_LAT = 0;
   pLCD_DO_DDR = 1;
   /* LCD_CL SIO�N���b�N(CMOS)�ݒ� */
   pLCD_CL_FSA = 1;
   pLCD_CL_FSB = 0;
   pLCD_CL_LAT = 0;
   pLCD_CL_DDR = 1;
}
void   Io_int_holdx_out( void )
{
   /* �ް���� 62.5ms start ACC_DET�`���^�p */
   //__bf_BTCR.RUN = 1;
   __bf_BTCR.IE1 = 1;
   
   /* ���v��ϊ����݋֎~ */
   __byte_CKTIMCTRL = BIN_10001110;
   
   /* 10ms�^�C�}���荞�݋������� */
   __bf_T1CNT.IE = 1;
   __bf_T1CNT.RUN = 1;
   
   /* ACC_DET�����ݒ�~ */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
   
   /* ALARMKEY���o�����݋֎~ */
   __bf_P0FSA.P04IE = 0;
   __bf_P0FSA.P04IL = 0;
   __bf_P0FSA.P04FLG = 0;
}






/******************************************************************************
ACC ON(POWER OFF) �� ACC OFF(�ʏ�)
*******************************************************************************
   �֐���   �FIo_xxx_accoff_in
   ��  ��   �F����
   �߂�l   �F����
   �@  �\   �FPOWER OFF IN ����ڼ޽��ݒ菈��
           �V�X�e���N���b�NOSC0�I��
           RC���U/OSC1��~
------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
******************************************************************************/
void   Io_sfr_accoff_in( void )
{
   volatile WORD   wait_cnt ;
   
   __bf_OCR0.RCSTOP = 0;   /* RC���U�J�n */
   
     /* RC���U����  */
   for (wait_cnt = 0; wait_cnt < 0x10; wait_cnt++)
   {
   }
   
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;
   
   __bf_OCR0.ENOSC1 = 0;         /* OSC1 Stop */
   
}
void   Io_port_accoff_in( void )
{
   /* I2C_SCK LOW�o�͐ݒ� */
   pI2C_SCK_FSA = 0;
   pI2C_SCK_FSB = 0;
   pI2C_SCK_LAT = 0;
   pI2C_SCK_DDR = 1;
   /* I2C_SDA LOW�o�͐ݒ� */
   pI2C_SDA_FSA = 0;
   pI2C_SDA_FSB = 0;
   pI2C_SDA_LAT = 0;
   pI2C_SDA_DDR = 1;
}
void   Io_int_accoff_in( void )
{
   /* �ް���� 62.5ms �J�n ACC_DET�`���^�p */
   //__bf_BTCR.RUN = 1;
   __bf_BTCR.IE1 = 1;
   
   /* 1ms��ϒ�~ */
   __byte_T2CNT0 = BIN_00000000;
   
   /* 10ms�^�C�}���荞�݋������� */
   __bf_T1CNT.IE = 1;
   __bf_T1CNT.RUN = 1;
   
   /* ACC_DET�����ݒ�~ */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
}

/******************************************************************************
ACC OFF(�ʏ�) �� ACC ON(POWER OFF)
*******************************************************************************
   �֐���   �FIo_xxx_accoff_out
   ��  ��   �F����
   �߂�l   �F����
   �@  �\   �FPOWER OFF IN ����ڼ޽��ݒ菈��
           �V�X�e���N���b�NOSC0�I��
           RC���U/OSC1��~
------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
******************************************************************************/
void   Io_sfr_accoff_out( void )
{
   volatile WORD   wait_cnt ;
   
   __bf_OCR0.ENOSC1 = 1;         /* OSC1 Start */

   /* 50ms �ȏ�Wait   Cho hon 50ms */
   for (wait_cnt = 0; wait_cnt < 0x0A0; wait_cnt++)
   {
   }
   
   
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC1;
   
   __bf_OCR0.RCSTOP = __bfval_OCR0_RCSTOP_STOP;   /* RC���U��~  Ngung dao dong RC */
   
}
void   Io_port_accoff_out( void )
{
   /* I2C_SCK HIGH�o�͐ݒ� */
   pI2C_SCK_FSA = 0;
   pI2C_SCK_FSB = 0;
   pI2C_SCK_LAT = 1;
   pI2C_SCK_DDR = 1;
   /* I2C_SDA HIGH�o�͐ݒ� */
   pI2C_SDA_FSA = 0;
   pI2C_SDA_FSB = 0;
   pI2C_SDA_LAT = 1;
   pI2C_SDA_DDR = 1;
}
void   Io_int_accoff_out( void )
{
   /* �ް���� 62.5ms ��~ ACC_DET�`���^�p Dung cho qu�t port ACC_DET, base timer 62.5ms (Xem them sheet 5 file Gaiyousekkei)*/
   //__bf_BTCR.RUN = 0;
   __bf_BTCR.IE1 = 0;
   
   /* 1ms��ϊJ�n   Bat dau timer 1ms*/
   __byte_T2CNT0 = BIN_00000101;
   
   /* 10ms���荞�ݒ�~ AccOFF��Alarm�L�[���o�p  Dung ngat 10ms, dung cho viec detect key ALARM trong khi ACCOFF */
   __bf_T1CNT.IE = 0;
   __bf_T1CNT.RUN = 0;
   
   /* ACC_DET�����ݒ�~   Dung ngat ACC OFF */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
}

void Io_alarmkey_enable( void )
{
   __bf_P0LAT.BIT4  = 0;
   __bf_P0DDR.BIT4  = 0;
}
void Io_int_disable_clktim(void)
{
   /* ���v�^�C�}500ms���荞�݋֎~ */
   __byte_CKTIMCTRL = BIN_10001110;
}

void Io_int_disable_almkey(void)
{
   /* ALARMKEY���o�����݋֎~ */
   __bf_P0FSA.P04IE = 0;
   __bf_P0FSA.P04IL = 0;
}

void Io_int_disable_accdet(void)
{
   /* ACC_DET�����ݒ�~ */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
}
/******************************************************************************
   �֐���   �FIO_In_acc_det
   ��  ��   �F����
   �߂�l   �FHIGH
            LOW
   �@  �\   �FACC-DET�[�q��ԕԋp
------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
******************************************************************************/
BOOL   IO_In_acc_det(void)
{
   fIO_acc = MIO_IN_pACC_DET();

#if 0   /* ���]���g�p���Ȃ��̂ŃR�����g�A�E�g */
   if ((pACC_DET_FSA == 0) &&
      (pACC_DET_FSB == 1))
   {
      fIO_acc = ~fIO_acc;
   }
#endif
   return(fIO_acc);
}
/******************************************************************************
   �֐���   �FIO_In_ill_det
   ��  ��   �F����
   �߂�l   �FHIGH
            LOW
   �@  �\   �FILL-DET�[�q��ԕԋp
------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
******************************************************************************/
BOOL   IO_In_ill_det(void)
{
   fIO_ill = MIO_IN_pILL_DET();
   return(fIO_ill);
}
/******************************************************************************
   �֐���   �FIO_In_acc_det
   ��  ��   �F����
   �߂�l   �FHIGH
            LOW
   �@  �\   �FALARM_KEY�[�q��ԕԋp
------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
******************************************************************************/
BOOL   IO_In_alarm_key(void)
{
   fIO_alarm = MIO_IN_pALARM_KEY();
   return(fIO_alarm);
}

/******************************************************************************
   ��  �� ��   : Io_beep_start
   ��     ��   : ����
   ��  �� �l   : ����
   �@�\�T��   : 2kHz��BEEP������܂�
    ��     �l    : ��~�́A Io_beep_stop ���R�[�����Ă�������
-------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
[20130415yamamoto] �d�l�ύX 4kHz��2kHz
********************************************************************************/
void Io_beep_start( void )
{
   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;      /* �g�O���o�͒�~ */

   __bf_T0CNT.CKSEL = __bfval_T0CNT_CKSEL_OSC0;
   
   __byte_T0LR = 0x07;                        /* MODE3 2kHz */
   __bf_T0PR.PR = 0;
   
   __bf_T0PR.MODE = __bfval_T0PR_MODE_MODE3;
   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_RUN;         /* �g�O���o�͊J�n */
}
/******************************************************************************
   ��  �� ��   : Io_beep_start
   ��     ��   : ����
   ��  �� �l   : ����
   �@�\�T��   : 8Hz��BEEP������܂�,TEST���[�h�p
    ��     �l    : ��~�́A Io_beep_stop ���R�[�����Ă�������
-------------------------------------------------------------------------------
[20120926yamamoto] �V�K�쐬
********************************************************************************/
void Io_beep_test_8Hz_start( void )
{
   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;      /* �g�O���o�͒�~ */

   __bf_T0CNT.CKSEL = __bfval_T0CNT_CKSEL_OSC0;
   
   __byte_T0LR = 0xFF;                     /* MODE3 8Hz */
   __byte_T0PR = 0x67;                     /* ý�Ӱ�ޗp */

   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_RUN;         /* �g�O���o�͊J�n */
}


/******************************************************************************
   ��  �� ��   : Io_beep_stop
   ��     ��   : ����
   ��  �� �l   : ����
   �@�\�T��   : 4kHz��BEEP���~���܂�
    ��     �l    :
-------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
********************************************************************************/
void Io_beep_stop( void )
{
   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;      /* �g�O���o�͒�~ */
}
/******************************************************************************
   ��  �� ��   : Io_mincnt_read
   ��     ��   : ����
   ��  �� �l   : 0-1439(��)
   �@�\�T��   : �����b�J�E���^�̕��̒l��ǂݏo��
    ��     �l    : �K�������b�J�E���^�����삵�Ă��鎞�Ɏg�p���Ă��������B
-------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
********************************************************************************/
WORD Io_mincnt_read( void )
{
   BYTE i;
   BYTE old_sec;
   WORD old_min;
//   WORD old_day;
   BYTE new_sec;
   WORD new_min;
//   WORD new_day;
   
   /* ���b��A�����ēǂݏo���Ĉ�v����Ε���return */
   for(i=0;i<2;i++)
   {
      old_sec = __byte_SEC;
      old_min = __word_MINL;
   //   old_day = __word_DAYL;
      new_sec = __byte_SEC;
      new_min = __word_MINL;
   //   new_day = __word_DAYL;
      
      if ((old_sec == new_sec) &&
         #if 1
         (old_min == new_min))
         #else
         (old_min == new_min) &&
         (old_day == new_day))
         #endif
      {
         break;
      }
      
   }
   
   return(new_min);
}
/******************************************************************************
   ��  �� ��   : Io_mincnt_write
   ��     ��   : WORD min�F���̒l���X�V���܂��B
   ��  �� �l   : ����
   �@�\�T��   : ���J�E���^�̒l���X�V����
    ��     �l    : 
-------------------------------------------------------------------------------
[20120731yamamoto] �V�K�쐬
[20121017yamamoto] �d�l�ύX�Ή� ���v��ς��~����l�ɕύX
********************************************************************************/
void Io_mincnt_write( WORD min )
{
   __byte_CKTIMCTRL = BIN_00001100;

   __bf_DMSCNT.DMSRUN = 0;
   __bf_DMSCNT.DMSRUN = 1;

   __byte_SEC = 0x00;
   __word_MINL = min;
   __byte_CKTIMCTRL = BIN_10001100;
}

#if _WDT_
/******************************************************************************
   ��   ��  ��   : Io_wdt_clear
   ��       ��   : �Ȃ�
   ��   ��  �l   : �Ȃ�
   �@ �\ �T ��   : �E�H�b�`�h�b�O�^�C�}�̃N���A���s��
    ��       �l : 
-------------------------------------------------------------------------------
�C������
   �ԍ�   ���t       �S����      �C�����e
-------------------------------------------------------------------------------
  �y000�z   09/6/18    ���v�c      �V�K�쐬
********************************************************************************/
void Io_wdt_clear(void)
{
   /* WDT �����ر */
   __SFR_BITCLR(__WDTCR,0);
}

#endif


#undef   _IO_SFR_C_

