/******************************************************************************
   ÉNÉâÉäÉIÉììaå¸ÇØ                    2007  C3X03ºÿ∞Ωﬁ[âº]
      Ãﬂ€ºﬁ™∏ƒñº   : '07îNÃÿØÃﬂ¿ﬁ≥› MP3”√ﬁŸäJî≠    Phat trien mo hinh MP3 Flip down nam 07
      Ãß≤Ÿñº      : IO_sfr.c
        ”ºﬁ≠∞Ÿñº   : I/O
      ã@î\         : LC87ºÿ∞Ωﬁ ⁄ºﬁΩ¿íËã`    Dinh nghia thanh ghi  Series LC87
------------------------------------------------------------------------------
   èCê≥óöóÅyîNåéì˙ÅzÅyVerÅzÅyñºëOÅz Åyê‡ñæÅz
------------------------------------------------------------------------------
            2005.11.29  0.00   üNìc    êVãKçÏê¨
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
    LC87 SFR íËã`
----------------------------------------------------*/
static TCOM_FLG   fIO_flags1;
#define   fio_adc_active   (fIO_flags1.bit.bit7) /* ADïœä∑íÜÉtÉâÉO 1ÅFïœä∑íÜ */
#define   fIO_ill         (fIO_flags1.bit.bit6)
#define   fIO_acc         (fIO_flags1.bit.bit5)
#define   fIO_alarm      (fIO_flags1.bit.bit4)   /* BATTäÑÇËçûÇ›ÉåÉxÉãêÿÇËë÷Ç¶ã÷é~Ã◊∏ﬁ */
static BYTE   bIO_INTStatus;
static BYTE io_sio_buf[32] ;//= (BYTE*)RAM_ADDRESS;      /* SIOëóéÛêM ﬁØÃß  */
/*----------------------------------------------------
    éûåvï‚ê≥Ç…ä÷Ç∑ÇÈíËã`
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
    ÉçÅ[ÉJÉãä÷êî
----------------------------------------------------*/
static BYTE   IO_Tuner_Send_Byte(BYTE );
static BYTE   IO_Tuner_Receive_Byte(BYTE);

static void   IO_EVOL_Send_Start(void);
static void   IO_EVOL_Send_Stop(void);
static BYTE   IO_EVOL_Send_Byte(BYTE );

static BYTE   IO_EEP_ReadByte(BYTE   ack);

#if _TEST_MODE_
/******************************************************************************
   ä÷  êî ñº   : IO_Test_xxxx
   ã@î\äTó™   : UART2í êMÉCÉìÉ^Å[ÉtÉFÅ[ÉX
-------------------------------------------------------------------------------
[chin20120903] êVãKçÏê¨
********************************************************************************/
void IO_Test_SetUartPort(BYTE Status)
{
   if (Status == ON)
   {
      pUART_TX_FSA = 1;               /*ACCONéûUARTèoóÕê›íË*/
      __byte_U2CNT0 = BIN_00001001;
   }
   else
   {
      __byte_U2CNT0 = BIN_00000000;
      pUART_TX_FSA = 0;               /*ACCOFFéûLOWèoóÕê›íË*/
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

#if 1   /* chin20121009 ÉoÉXÉtÉ@ÉCÉgëŒçÙ */
/******************************************************************************
   ä÷   êî  ñº   : IO_SetIICPort
   à¯       êî   : status ÅÀ IICÉ|Å[ÉgON/OFFê›íË
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : IICÉ|Å[ÉgÇÃê›íËèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
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
   else      /* LOWèoóÕ */
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
   ä÷   êî  ñº   : IO_Tuner_Receive
   à¯       êî   : *addr ÅÀ éÛêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
               : bytecntÅÀ éÛêM√ﬁ∞¿í∑
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : Tuner√ﬁ∞¿éÛêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
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
   ä÷   êî  ñº   : IO_Tuner_Send
   à¯       êî   : *addr ÅÀ ëóêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
               : bytecntÅÀ ëóêM√ﬁ∞¿í∑
   ñﬂ   ÇË  íl   : OK ÅÀ ëóêMê≥èÌèIóπ
            : NG ÅÀ ACK¥◊∞î≠ê∂
   ã@ î\ äT ó™   : Tuner√ﬁ∞¿ëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
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
   ä÷   êî  ñº   : IO_Tuner_Send_Start
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : ÉXÉ^Å[ÉgëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
void   IO_Tuner_Send_Start(void)
{
   MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
}

/******************************************************************************
   ä÷   êî  ñº   : IO_Tuner_Send_Stop
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : ÉXÉgÉbÉvëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
void   IO_Tuner_Send_Stop(void)
{
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( IN );            /* DATAÅ®HIGH */
}

/******************************************************************************
   ä÷   êî  ñº   : IO_Tuner_Send_Byte
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : 1BYTEëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
static BYTE   IO_Tuner_Send_Byte(BYTE data)
{
   BYTE   count;
   BYTE   ack;
   
   for(count = 0 ; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );      /* CLKÅ®LOW */
      Cm_wait_loop(1);
      if ((data & 0x80) == 0x80)
      {
         MIO_CHG_pE2P_SDA( IN );      /* DATAÅ®HIGH */
      }
      else
      {
         MIO_CHG_pE2P_SDA( OUT );   /* DATAÅ®LOW */
      }
      Cm_wait_loop(0);
      MIO_CHG_pE2P_SCL( IN );         /* CLKÅ®HIGH */
      Cm_wait_loop(3);
      data <<= 1;
   }

   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   Cm_wait_loop(3);
   MIO_CHG_pE2P_SDA( IN );            /* DATAÅ®HIGH */
   MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
   Cm_wait_loop(2);
   ack = MIO_IN_pE2P_SDA();         /*   ACKéÊìæ   */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   
   return(ack);
}

/******************************************************************************
   ä÷   êî  ñº   : IO_Tuner_Receive_Byte
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : 1BYTEëóéÛêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
static BYTE   IO_Tuner_Receive_Byte(BYTE    ack)
{
   BYTE   count;
   BYTE   data;

   data = 0;
   for(count = 0; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
      data <<= 1;
      Cm_wait_loop(3);
      MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
      if (MIO_IN_pE2P_SDA() == HIGH)      /*1ÉrÉbÉgÉVÉtÉgéÛêM*/
      {
         data |= 0x01;
      }
      else
      {
         data &= 0xfe;
      }
      Cm_wait_loop(2);
   }
   
   MIO_CHG_pE2P_SCL( OUT );            /* CLKÅ®LOW */
   
   Cm_wait_loop(1);
   if (ack == CIO_IIC_ACK)
   {
      MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   }
   else
   {
      MIO_CHG_pE2P_SDA( IN );            /* DATAÅ®HIGH */
   }
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( IN );               /* CLKÅ®HIGH */

   Cm_wait_loop(4);
   MIO_CHG_pE2P_SCL( OUT );            /* CLKÅ®LOW */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SDA( IN );               /* DATAÅ®HIGH */
   return(data);
}

/******************************************************************************
   ä÷   êî  ñº   : IO_EVOL_Send
   à¯       êî   : *addr ÅÀ ëóêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
               : bytecntÅÀ ëóêM√ﬁ∞¿í∑
   ñﬂ   ÇË  íl   : OK ÅÀ ëóêMê≥èÌèIóπ
            : NG ÅÀ ACK¥◊∞î≠ê∂
   ã@ î\ äT ó™   : EVOL√ﬁ∞¿ëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
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
   ä÷   êî  ñº   : IO_EVOL_Send_Start
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : ÉXÉ^Å[ÉgëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
static void   IO_EVOL_Send_Start(void)
{
   MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
}

/******************************************************************************
   ä÷   êî  ñº   : IO_EVOL_Send_Stop
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : ÉXÉgÉbÉvëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
static void   IO_EVOL_Send_Stop(void)
{
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( IN );            /* DATAÅ®HIGH */
}

/******************************************************************************
   ä÷   êî  ñº   : IO_EVOL_Send_Byte
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : 1BYTEëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
static BYTE   IO_EVOL_Send_Byte(BYTE data)
{
   BYTE   count;
   BYTE   ack;
   
   for(count = 0 ; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );      /* CLKÅ®LOW */
      Cm_wait_loop(1);
      if ((data & 0x80) == 0x80)
      {
         MIO_CHG_pE2P_SDA( IN );      /* DATAÅ®HIGH */
      }
      else
      {
         MIO_CHG_pE2P_SDA( OUT );   /* DATAÅ®LOW */
      }
      Cm_wait_loop(0);
      MIO_CHG_pE2P_SCL( IN );         /* CLKÅ®HIGH */
      Cm_wait_loop(3);
      data <<= 1;
   }

   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   Cm_wait_loop(3);
   MIO_CHG_pE2P_SDA( IN );            /* DATAÅ®HIGH */
   MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
   Cm_wait_loop(2);
   ack = MIO_IN_pE2P_SDA();         /*   ACKéÊìæ   */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   
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
   ä÷   êî  ñº   : IO_EEP_SendStart
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : ÉXÉ^Å[ÉgëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
void IO_EEP_SendStart( void )
{
   MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
}

/******************************************************************************
   ä÷   êî  ñº   : IO_EEP_SendRestart
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : ÉäÉXÉ^Å[ÉgëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
void IO_EEP_SendRestart( void )
{
   Cm_wait_loop(2);
   MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
   Cm_wait_loop(3);
   MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
}

/******************************************************************************
   ä÷   êî  ñº   : IO_EEP_SendStop
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : ÉXÉgÉbÉvëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
void IO_EEP_SendStop(void)
{
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
   Cm_wait_loop(1);
   MIO_CHG_pE2P_SDA( IN );            /* DATAÅ®HIGH */
}

/******************************************************************************
   ä÷   êî  ñº   : IO_EEP_WriteByte
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : 1BYTEëóêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
BYTE   IO_EEP_WriteByte(BYTE   data)
{
   BYTE   count;
   BYTE   ack;
   
   Cm_wait_loop(2);

   for(count = 0 ; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );      /* CLKÅ®LOW */
      Cm_wait_loop(1);
      if ((data & 0x80) == 0x80)
      {
         MIO_CHG_pE2P_SDA( IN );      /* DATAÅ®HIGH */
      }
      else
      {
         MIO_CHG_pE2P_SDA( OUT );   /* DATAÅ®LOW */
      }
      Cm_wait_loop(0);
      MIO_CHG_pE2P_SCL( IN );         /* CLKÅ®HIGH */
      Cm_wait_loop(3);
      data <<= 1;
   }

   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   Cm_wait_loop(3);
   MIO_CHG_pE2P_SDA( IN );            /* DATAÅ®HIGH */
   MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
   Cm_wait_loop(2);
   ack = MIO_IN_pE2P_SDA();         /*   ACKéÊìæ   */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
   
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
   ä÷   êî  ñº   : IO_EEP_ReadByte
   à¯       êî   : ñ≥Çµ
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : 1BYTEëóéÛêMèàóù
-------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
********************************************************************************/
static BYTE   IO_EEP_ReadByte(BYTE   ack)
{
   BYTE   count;
   BYTE   data;

   data = 0;
   for(count = 0; count < 8; count++)
   {
      MIO_CHG_pE2P_SCL( OUT );         /* CLKÅ®LOW */
      data <<= 1;
      Cm_wait_loop(3);
      MIO_CHG_pE2P_SCL( IN );            /* CLKÅ®HIGH */
      if (MIO_IN_pE2P_SDA() == HIGH)      /*1ÉrÉbÉgÉVÉtÉgéÛêM*/
      {
         data |= 0x01;
      }
      else
      {
         data &= 0xfe;
      }
      Cm_wait_loop(2);
   }
   
   MIO_CHG_pE2P_SCL( OUT );            /* CLKÅ®LOW */
   
   Cm_wait_loop(1);
   if (ack == CIO_IIC_ACK)
   {
      MIO_CHG_pE2P_SDA( OUT );         /* DATAÅ®LOW */
   }
   else
   {
      MIO_CHG_pE2P_SDA( IN );            /* DATAÅ®HIGH */
   }
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SCL( IN );               /* CLKÅ®HIGH */

   Cm_wait_loop(4);
   MIO_CHG_pE2P_SCL( OUT );            /* CLKÅ®LOW */
   
   Cm_wait_loop(0);
   MIO_CHG_pE2P_SDA( IN );               /* DATAÅ®HIGH */
   return(data);
}

/*****************************************************************************************
   ä÷êîñº      ÅFvoid IO_EEP_ReadData(BYTE *addr,BYTE  bytecnt)
   à¯êî      ÅF *addr ÅÀ éÛêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
            ÅF bytecntÅÀ éÛêM√ﬁ∞¿í∑
   ÉäÉ^Å[Éì   ÅFÇ»Çµ
   ã@î\äTóv   ÅFEEPROM√ﬁ∞¿éÛêMèàóù
-----------------------------------------------------------------------------------------
[chin20121009] êVãKçÏê¨
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

#else   /* ÉoÉXÉtÉ@ÉCÉgëŒçÙ */
/******************************************************************************
   ä÷   êî  ñº   : IO_Tuner_SetIICPort
   à¯       êî   : statusÅF ON ÅÀ í êMPortÇ…ê›íË
            :          OFF ÅÀ îƒópŒﬂ∞ƒÇ…ê›íË
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : Tunerí êMŒﬂ∞ƒêÿë÷èàóù
    îı       çl : í êMPortÇ…ê›íËÇ∑ÇÈÇ∆ÅACLKÇ∆DATAóºï˚èoóÕÇÃHIGHÇ…ê›íËÇ≥ÇÍÇÈ
-------------------------------------------------------------------------------
[sun100420] êVãKçÏê¨
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
   ä÷   êî  ñº   : IO_Tuner_Receive
   à¯       êî   : *addr ÅÀ éÛêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
               : bytecntÅÀ éÛêM√ﬁ∞¿í∑
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : Tuner√ﬁ∞¿éÛêMèàóù
-------------------------------------------------------------------------------
[sun100420] êVãKçÏê¨
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
   ä÷   êî  ñº   : IO_Tuner_Send
   à¯       êî   : *addr ÅÀ ëóêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
               : bytecntÅÀ ëóêM√ﬁ∞¿í∑
   ñﬂ   ÇË  íl   : OK ÅÀ ëóêMê≥èÌèIóπ
            : NG ÅÀ ACK¥◊∞î≠ê∂
   ã@ î\ äT ó™   : Tuner√ﬁ∞¿ëóêMèàóù
-------------------------------------------------------------------------------
[20120730yamamoto] êVãKçÏê¨
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

/* [sun100505] í êMë¨ìxí≤êÆÅ@250kHzÇÆÇÁÇ¢ */
/* [sun100817] í êMë¨ìxí≤êÆÅ@100kHzÇÆÇÁÇ¢ */
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

/* [sun100505] í êMë¨ìxí≤êÆÅ@250kHzÇÆÇÁÇ¢ */
/* [sun100817] í êMë¨ìxí≤êÆÅ@100kHzÇÆÇÁÇ¢ */
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

/* [sun100505] í êMë¨ìxí≤êÆÅ@250kHzÇÆÇÁÇ¢ */
/* [sun100817] í êMë¨ìxí≤êÆÅ@100kHzÇÆÇÁÇ¢ */
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

/* [sun100505] í êMë¨ìxí≤êÆÅ@250kHzÇÆÇÁÇ¢ */
/* [sun100817] í êMë¨ìxí≤êÆÅ@100kHzÇÆÇÁÇ¢ */
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
   ä÷   êî  ñº   : IO_EVOL_Send
   à¯       êî   : *addr ÅÀ ëóêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
               : bytecntÅÀ ëóêM√ﬁ∞¿í∑
   ñﬂ   ÇË  íl   : ñ≥Çµ
   ã@ î\ äT ó™   : EVOL√ﬁ∞¿ëóêMèàóù
    îı       çl : 
-------------------------------------------------------------------------------
èCê≥óöó
   î‘çÜ   ì˙ït       íSìñé“      èCê≥ì‡óe
-------------------------------------------------------------------------------
[sun100420] êVãKçÏê¨
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
/* [20120731yamamoto] í êMë¨ìxí≤êÆ 100kHzÇÆÇÁÇ¢ */
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
/* [20120731yamamoto] í êMë¨ìxí≤êÆ 100kHzÇÆÇÁÇ¢ */
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

/* [20120731yamamoto] í êMë¨ìxí≤êÆ 100kHzÇÆÇÁÇ¢ */
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

/* [20120731yamamoto] í êMë¨ìxí≤êÆ 100kHzÇÆÇÁÇ¢ */
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
   ä÷êîñº      ÅFvoid IO_EEP_SendStart( void )
   à¯êî      ÅFÇ»Çµ
   ÉäÉ^Å[Éì   ÅFÇ»Çµ
   ã@î\äTóv   ÅFEEPROMƒﬁ◊≤ ﬁMainèàóù
-----------------------------------------------------------------------------------------
[sun100806] êVãKçÏê¨
*****************************************************************************************/
void IO_EEP_SendStart( void )
{
   MIO_OUT_pE2P_SDA( HIGH );         /*   EEPROMÉfÅ[É^É|Å[ÉgÇ…HIGHèoóÕ   */
   MIO_OUT_pE2P_SCL( HIGH );         /*   EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…HIGHèoóÕ   */
   MIO_CHG_pE2P_SDA( OUT );         /*   EEPROMÉfÅ[É^É|Å[ÉgèoóÕÉ|Å[ÉgÇ…ïœä∑   */
   //Cm_wait_loop(3);
   MIO_OUT_pE2P_SDA( LOW );         /*   EEPROMÉfÅ[É^É|Å[ÉgÇ…LOWèoóÕ      */
   Cm_wait_loop(1);
   MIO_OUT_pE2P_SCL( LOW );         /*   EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…HIGHèoóÕ   */
}
/*****************************************************************************************
   ä÷êîñº      ÅFvoid IO_EEP_SendRestart( void )
   à¯êî      ÅFÇ»Çµ
   ÉäÉ^Å[Éì   ÅFÇ»Çµ
   ã@î\äTóv   ÅFEEPROMƒﬁ◊≤ ﬁMainèàóù
-----------------------------------------------------------------------------------------
[sun100806] êVãKçÏê¨
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
   ä÷êîñº      ÅFvoid IO_EEP_SendStop(void)
   à¯êî      ÅFÇ»Çµ
   ÉäÉ^Å[Éì   ÅFÇ»Çµ
   ã@î\äTóv   ÅFEEPROMƒﬁ◊≤ ﬁMainèàóù
-----------------------------------------------------------------------------------------
[sun100806] êVãKçÏê¨
*****************************************************************************************/
void IO_EEP_SendStop(void)
{
   MIO_OUT_pE2P_SCL( LOW );         /*   EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…HIGHèoóÕ   */
   MIO_CHG_pE2P_SDA( OUT );      /*   EEPROMÉfÅ[É^É|Å[ÉgèoóÕÉ|Å[ÉgÇ…ïœä∑   */
   MIO_OUT_pE2P_SDA( LOW );         /*   EEPROMÉfÅ[É^É|Å[ÉgÇ…HIGHèoóÕ   */
   Cm_wait_loop(1);
   MIO_OUT_pE2P_SCL( HIGH );         /*   EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…HIGHèoóÕ   */
   Cm_wait_loop(1);
   MIO_OUT_pE2P_SDA( HIGH );         /*   EEPROMÉfÅ[É^É|Å[ÉgÇ…LOWèoóÕ      */
}
/*****************************************************************************************
   ä÷êîñº      ÅFBYTE   IO_EEP_SendByteData(BYTE   data)
   à¯êî      ÅFÇ»Çµ
   ÉäÉ^Å[Éì   ÅFÇ»Çµ
   ã@î\äTóv   ÅFEEPROMƒﬁ◊≤ ﬁMainèàóù
-----------------------------------------------------------------------------------------
[sun100806] êVãKçÏê¨
*****************************************************************************************/
BYTE   IO_EEP_WriteByte(BYTE   data)
{
   BYTE   count;
   BYTE   ack;
   
   //MIO_OUT_pE2P_SCL( LOW );      /*EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…LOWèoóÕ*/
   MIO_CHG_pE2P_SDA( OUT );      /*   EEPROMÉfÅ[É^É|Å[ÉgèoóÕÉ|Å[ÉgÇ…ïœä∑   */
   Cm_wait_loop(2);

   for(count = 0 ; count < 8; count++)
   {
      MIO_OUT_pE2P_SCL( LOW );      /*EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…LOWèoóÕ*/
      if ((data & 0x80) == 0x80)
      {
         MIO_OUT_pE2P_SDA(HIGH);      /*1ÉrÉbÉgÉVÉtÉgÇ≈ëóêM*/
      }
      else
      {
         MIO_OUT_pE2P_SDA(LOW);   /*1ÉrÉbÉgÉVÉtÉgÇ≈ëóêM*/
      }
      Cm_wait_loop(1);
      MIO_OUT_pE2P_SCL( HIGH );      /*EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…HIGHèoóÕ*/
      Cm_wait_loop(1);
      data <<= 1;
   }

   MIO_OUT_pE2P_SCL( LOW );      /*   EEPROMÉNÉbÉçÉNÉ|Å[ÉgÇ…LOWèoóÕ      */
   MIO_OUT_pE2P_SDA(LOW);   /*1ÉrÉbÉgÉVÉtÉgÇ≈ëóêM*/
   MIO_CHG_pE2P_SDA( IN );         /*   EEPROMÉfÅ[É^É|Å[Égì¸óÕÉ|Å[ÉgÇ…ïœä∑   */
   Cm_wait_loop(0);
   MIO_OUT_pE2P_SCL( HIGH );      /*   EEPROMÉNÉbÉçÉNÉ|Å[ÉgÇ…HIGHèoóÕ      */
   ack = MIO_IN_pE2P_SDA();         /*   ACKéÊìæ                        */
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
   ä÷êîñº      ÅFBYTE   IO_EEP_ReadByte(BYTE   ack)
   à¯êî      ÅFÇ»Çµ
   ÉäÉ^Å[Éì   ÅFÇ»Çµ
   ã@î\äTóv   ÅFEEPROMƒﬁ◊≤ ﬁMainèàóù
-----------------------------------------------------------------------------------------
[sun100806] êVãKçÏê¨
*****************************************************************************************/
BYTE   IO_EEP_ReadByte(BYTE   ack)
{
   BYTE   count;
   BYTE   data;

   MIO_OUT_pE2P_SCL( LOW );      /*EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…LOWèoóÕ*/
   MIO_CHG_pE2P_SDA(IN);            /*EEPROMÉfÅ[É^É|Å[Égì¸óÕÉ|Å[ÉgÇ…ïœä∑*/

   data = 0;
   for(count = 0; count < 8; count++)
   {
      MIO_OUT_pE2P_SCL( LOW );      /*EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…LOWèoóÕ*/
      data <<= 1;
      Cm_wait_loop(1);
      MIO_OUT_pE2P_SCL( HIGH );      /*EEPROMÉNÉçÉbÉNÉ|Å[ÉgÇ…HIGHèoóÕ*/
      if (MIO_IN_pE2P_SDA() == HIGH)      /*1ÉrÉbÉgÉVÉtÉgéÛêM*/
      {
         data |= 0x01;
      }
      else
      {
         data &= 0xfe;
      }
      Cm_wait_loop(0);
   }
   
   MIO_OUT_pE2P_SCL( LOW );      /*   EEPROMÉNÉbÉçÉNÉ|Å[ÉgÇ…LOWèoóÕ      */
   MIO_OUT_pE2P_SDA(ack);         /*1ÉrÉbÉgÉVÉtÉgÇ≈ëóêM*/
   MIO_CHG_pE2P_SDA( OUT );      /*   EEPROMÉfÅ[É^É|Å[Égì¸óÕÉ|Å[ÉgÇ…ïœä∑   */
   Cm_wait_loop(1);
   MIO_OUT_pE2P_SCL( HIGH );      /*   EEPROMÉNÉbÉçÉNÉ|Å[ÉgÇ…HIGHèoóÕ      */
   return(data);
}

/*****************************************************************************************
   ä÷êîñº      ÅFvoid IO_EEP_ReadData(BYTE *addr,BYTE  bytecnt)
   à¯êî      ÅFÇ»Çµ
   ÉäÉ^Å[Éì   ÅFÇ»Çµ
   ã@î\äTóv   ÅFEEPROMƒﬁ◊≤ ﬁMainèàóù
-----------------------------------------------------------------------------------------
[sun100806] êVãKçÏê¨
[sun100823] –ΩèCê≥ÅFÅ@bytecntÇBYTEÅÀWORDÇ…ïœä∑
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

#endif   /* ÉoÉXÉtÉ@ÉCÉgëŒçÙ */

/******************************************************************************
   ä÷   êî  ñº   : IO_lcd_ccb
   à¯       êî   : adr ÅÀ 1byteëóêM√ﬁ∞¿(CCB±ƒﬁ⁄Ωóp)
   ñﬂ   ÇË  íl   : OK    ÅÀ èàóùäÆóπ
              : NG    ÅÀ èàóùé∏îs
   ã@ î\ äT ó™   : LCDÉhÉâÉCÉoÇ÷ÇPÉoÉCÉg √ﬁ∞¿ÇëóêMÇµÇ‹Ç∑ÅB
    îı       çl : LSBêÊì™Ç≈ëóêM
-------------------------------------------------------------------------------
èCê≥óöó
   î‘çÜ   ì˙ït       íSìñé“      èCê≥ì‡óe
-------------------------------------------------------------------------------
  Åy000Åz   12/08/03   ha         êVãKçÏê¨
********************************************************************************/
BYTE IO_lcd_ccb( BYTE adr )
{
   BYTE   roop_cnt;
   
   
   #if 0
   /* portê›íË 
      P10: SIO0ÉfÅ[É^ÅiCMOS)   
      P11: ÉIÅ[ÉvÉì         
      P12: SIO0ÉNÉçÉbÉN(CMOS)   */
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
   /* É{Å[ÉåÅ[ÉgÉNÉçÉbÉNê›íË */
   __byte_S0BG = 0x0D;      /* 850kHz */
   #endif
   
   __EnableInterrupt(0);

   __bf_IL1H.IRQ6  = 0x03;  // thiet lap level IRQ6 = 3
   
   __byte_S0CNT    = 0x01;      /* ëóêMÉÇÅ[Éh    LSB    an intterrupt request to vector address 08038H is generated, chon transimission mode, chon LSB first */
   __byte_S0INTVL  = 0x00;      /*  Ket hop cac bit WAKEUP, AUTO, REC cua S0CNT va bit XCHNG cua S0INTVL de chon mode, o day cac bit la 0000 => mode Transmission*/
   
   __byte_S0BUF     = adr;  // Gui data la dia chi

   __bf_S0CNT.RUN = 1;      /* ëóêMäJén         Å@    This bit must be set with an instruction. O mode 0, Khi ket thuc viec truyen, bit RUN se tu dong clear. */
   
   for ( roop_cnt=0; roop_cnt< 0xff ; roop_cnt++ )
   {
      if( __bf_S0CNT.FLG == ON )        // Khi truyen xong thi FLG tu dong len 1
      {
         __bf_S0CNT.FLG = OFF ;      /* ëóêMäÆóπÇ‹Ç≈Ÿ∞Ãﬂ */
         return OK ;
      }
   }

   return NG ;         /* time out */
}
/******************************************************************************
   ä÷   êî  ñº   : Io_sio_lcd_send
   à¯       êî   : *tx_data ÅÀ ëóêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
               : tx_cnt    ÅÀ ëóêM√ﬁ∞¿ByteêîÅiÇPÅ`ÇRÇQÅj
   ñﬂ   ÇË  íl   : OK    ÅÀ èàóùäÆóπ
              : NG    ÅÀ èàóùé∏îs
   ã@ î\ äT ó™   : LCDÉhÉâÉCÉoÇ÷éwíËÉoÉCÉgêî√ﬁ∞¿ÇëóêMÇµÇ‹Ç∑ÅB
    îı       çl : ëóêM√ﬁ∞¿ByteêîÇÕÅAMax32byteÅBLSBêÊì™Ç≈ëóêM
-------------------------------------------------------------------------------
èCê≥óöó
   î‘çÜ   ì˙ït       íSìñé“      èCê≥ì‡óe
-------------------------------------------------------------------------------
  Åy000Åz   12/08/03   ha         êVãKçÏê¨
********************************************************************************/
BYTE IO_lcd_send( BYTE *tx_data, BYTE tx_cnt )
{
   BYTE   roop_cnt;
   BYTE   i;
   
   i = 0;
   #if 0
   /* portê›íË 
      P10: SIO0ÉfÅ[É^ÅiCMOS)   
      P11: ÉIÅ[ÉvÉì         
      P12: SIO0ÉNÉçÉbÉN(CMOS)   */
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

   /* É{Å[ÉåÅ[ÉgÉNÉçÉbÉNê›íË */
   __byte_S0BG = 0x0D;      /* 850kHz */
   #endif
   
   memcpy( io_sio_buf, tx_data, tx_cnt );   /* ëóêM√ﬁ∞¿ê›íË  io_sio_buf la mang co 32 phan tu kieu byte. io_sio_buf trong file IO_sfr*/
   
   __EnableInterrupt(0);

   __bf_IL1H.IRQ6  = 0x03;
   
   __byte_S0CNT    = 0x01;      /* ëóêMÉÇÅ[Éh    LSB     */
   __byte_S0INTVL  = 0x00;
   
   
   for(i=0;i < tx_cnt;i++)
   {
      __byte_S0BUF     = io_sio_buf[i];
   
      __bf_S0CNT.RUN = 1;      /* ëóêMäJén         Å@ */
      
      for ( roop_cnt=0; roop_cnt< 0xff ; roop_cnt++ )
      {
         if( __bf_S0CNT.FLG == ON )
         {
            __bf_S0CNT.FLG = OFF ;      /* ëóêMäÆóπÇ‹Ç≈Ÿ∞Ãﬂ */
         }
      }
   }
   return OK ;         /* time out */
}

/******************************************************************************
   ä÷   êî  ñº   : IO_lcd_receive
   à¯       êî   : *rx_data ÅÀ éÛêM√ﬁ∞¿ ﬁØÃßŒﬂ≤›¿
   ñﬂ   ÇË  íl   : OK    ÅÀ èàóùäÆóπ
              : NG    ÅÀ èàóùé∏îs
   ã@ î\ äT ó™   : LCDÉhÉâÉCÉoÇ©ÇÁÇSÉoÉCÉg√ﬁ∞¿ÇéÛêMÇµÇ‹Ç∑ÅB
    îı       çl : ñﬂÇËílÇ™NGÇÃèÍçáÅAéÛêM ﬁØÃßÇÃì‡óeÇÕïœâªÇµÇ‹ÇπÇÒÅB
-------------------------------------------------------------------------------
èCê≥óöó
   î‘çÜ   ì˙ït       íSìñé“      èCê≥ì‡óe
-------------------------------------------------------------------------------
  Åy000Åz   12/08/03   ha         êVãKçÏê¨
********************************************************************************/
BYTE  IO_lcd_receive(BYTE *rx_data)
{
   WORD   roop_cnt;
   BYTE   i;
   if( rx_data == 0 )
   {
      return NG ;         /*  ﬂ◊“∞¿¥◊∞   Parameter error */
   }
   
   #if 0
   /* portê›íË 
      P10: SIO0ÉfÅ[É^ÅiCMOS)   
      P11: SIO0ÉfÅ[É^ÅiCMOSÅj   
      P12: SIO0ÉNÉçÉbÉN(CMOS)   */
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
   
   /* É{Å[ÉåÅ[ÉgÉNÉçÉbÉNê›íË */
   __byte_S0BG = 0x0D;      /* 850kHz */
   #endif
   
   /* ì]ëóâÒêî */
   i = 0;
   __EnableInterrupt(0);
   __bf_IL1H.IRQ6  = 1;
   
   
   __byte_S0INTVL  = 0x00;      /* 32bitéÛêM     nhan 32 bit    */
   
   __byte_S0BUF   = 0x00;
   __byte_S0CNT   = 0x41;      /* éÛêMÉÇÅ[Éh    LSB    Thiet lap mode Receice va nhan o che do LSB    */
   
//   __bf_S0CNT.RUN = 1;      /* éÛêMäJén         Å@ */
   
   /*Nhan data tu LCD driver dua vao SOBUF. Se mat 4 lan nhan de dua vao 4 byte cua key_lcd_driver_data[]*/
   for(i = 0;i< 4;i++)
   {
      __bf_S0CNT.RUN = 1;
      for ( roop_cnt=0; roop_cnt< 0x00ff; roop_cnt++ )
      {
         if( __bf_S0CNT.FLG == ON )
         {
            /* éÛêMäÆóπ */
            *(rx_data + i)     = __byte_S0BUF ;
            __bf_S0CNT.FLG = OFF ;
         }
      }
   }

   return OK ;         /* time out */
}


/* äÑÇËçûÇ›ä÷òA */
/******************************************************************************
   ä÷   êî  ñº   : Io_sfr_initial_set
   à¯       êî   : void
   ñﬂ   ÇË  íl   : void
   ã@ î\ äT ó™   : ≤∆º¨ŸéûÇÃSFRê›íËÇçsÇ¢Ç‹Ç∑°       Thuc hien viec thiet lap SFR luc khoi tao
    îı       çl :
-------------------------------------------------------------------------------
[20120730yamamoto] êVãKçÏê¨
********************************************************************************/
void Io_sfr_initial_set(void)    // xem thu vien sfr884200 trong phan mem SanyoXstomrmy
{
   volatile DWORD   loopcnt;
   DWORD tmp;
   sWORD dif_q;
   
   /* WDTí‚é~ */
   __byte_WDTCR = BIN_00000000;   /*   __byte_WDTCR  xem them thu vien sfr884200
                   |||||||+--- START   1:WDT START (0ÇÃèëÇ´çûÇ›Ç≈í‚é~Ç≈Ç´Ç»Ç¢)
                   ||||||+---- OVF     0:ñ\ëñåüèoéûÿæØƒã÷é~
                   |||||+----- USERFLG 0:å≈íË
                   ||||+------ PDNSTOP 0:HOLDXìÀì¸éûWDTÿæØƒ 1:HOLDXìÀì¸éûWDTí‚é~
                   |||+------- SRFLG   0:ÿæØƒé¿çsåüèoÃ◊∏ﬁ
                   ||+-------- MDSEL   0:≤›¿◊Ãﬂƒ”∞ƒﬁ 1:ÿæØƒ”∞ƒﬁ
                   |+--------- -       0:å≈íË
                   +---------- -       0:å≈íË  */
   
   /* PLL VCOêßå‰⁄ºﬁΩ¿ÅAå≈íËíl Thanh ghi dieu khien PLL VCO, gia tri co dinh */
   __byte_VCOCNT0 = 0x02;
   __byte_VCOCNT1 = 0x04;
   __byte_VCOCNT2 = 0x35;
   __byte_VCOCNT3 = 0xB6;
   __byte_VCOCNT4 = 0x00;
   
   /* 32kî≠êUÅïOSC0ÅïOSC1 Start */
   __byte_OCR0 = BIN_10000011;      /* 32kî≠êUÅïOSC0 Start ---  chon System clock la RC Ocsillator, chon OSC0 la General purpose port, OSC1 la VCO va Khoi dong ch˙ng
                                       Clock source VCO duoc lay ra tu pin LPFO
                                    */
   
   Io_port_initial_set();

   
#if 0   /* ì‡ïîÿæØƒã@î\égóp âΩÇ‡ê›íËÇµÇ»Ç¢ */
   __byte_TMPCNT0 |= BIN_01000000;      /* Bit6  LVDSTOP */
#endif
   
     /* î≠êUà¿íË Total 1000msà»è„  1s On dinh dao dong  */
   for (loopcnt = 0; loopcnt < 0xAf00; loopcnt++)
   {
   }
   
#if 0   /* EEPROMÇÃÇΩÇﬂÅAOSC1î≠êUÇ µÇ»Ç¢Ç∆Ç¢ÇØÇ»Ç¢ */
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;   /* ºΩ√—∏€Ø∏ëIëÅFRCî≠êU(1MHz) */
   
   #if 0   /* RCî≠êUÇÕä˘Ç…ìÆÇ¢ÇƒÇÈ */
   __bf_OCR0.RCSTOP= 0;   /* RCî≠êUäJén */
   #endif
#else
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC1;  // Chon xung nguon la OSC1, bo RC Oscillator 
   __bf_OCR0.RCSTOP = __bfval_OCR0_RCSTOP_STOP;  // ngung RC Oscillator
#endif
   
   __byte_OCR1 = BIN_00000000;         /* timer8ÇÃ∏€Ø∏ëIë(OSC0)/ºΩ√—∏€Ø∏ï™é¸ñ≥Çµ   Lua chon xung thach anh 32K cho timer8 voi ti le chia 1/32.
                                                                                    Day la xung cung cap cho WDT
                                                                                    Ko chia xung he thong ma giu nguyen */

   /* äÑÇËçûÇ›ÉåÉxÉãê›íË 3:X 2:H 1:L 0:ã÷é~ */
   __byte_IL1L = BIN_00101011;         /* IRQ3:0  T0 :2  BT :2  WDT:3    Interrupt level setting register 1L*/
   __byte_IL1H = BIN_11010000;         /* IRQ7:3 IRQ6:1 IRQ5:0 IRQ4:0 Interrupt level setting register 1H*/
   __byte_IL2L = BIN_00000000;         /* IRQB:0 IRQA:0 IRQ9:0 IRQ8:0 Interrupt level setting register 2L*/
   __byte_IL2H = BIN_01000000;         /* IRQF:1 IRQE:0 IRQD:0 IRQC:0 Interrupt level setting register 2H*/

   __byte_INT01CR = BIN_00000000;      /* INT0,INT1ñ¢égóp  Khong dung den INT0,INT1 (IRQ3 duoc set bang 0 = Disabled) */
   __byte_INT23CR = BIN_10000000;      /* INT2ñ¢égópÅCINT3äÑÇËçûÇ›ê›íË    Khong dung den INT2, thiet lap ngat falling edge detection INT3*/
   /* INT3ÇÕAcc_Detí[éqÇÃÅuóßâ∫ÇËÅvåüèoÇ∑ÇÈÇÊÇ§Ç…ê›íË    INT3: d˘ng de kiem tra canh xuong cua ACC_DET */

   #if 0      /* ê∏ìxå¸è„ */
   __byte_BTCR = BIN_01110101;         /* T8 2048TBST(62.5ms),WDT 8192TBST */
   #else
   __byte_BTCR = BIN_01010100;         /* T8 128TBST(3.9ms),WDT 8192TBST(0.25ms) */
   #endif
/*                  |||||||+--- IE0   0:BaseTime0äÑçûÇ›ã÷é~
                  ||||||+---- FLG0  0:
                  |||||+----- IE1   0:BaseTime1äÑçûÇ›ã÷é~
                  ||||+------ FLG1  0:
                  |||+------- CNT   11:BT1Åi2048TBSTÅj
                  ||+-------- CNT   
                  |+--------- RUN   1:ìÆçÏäJén
                  +---------- FST   0:ñ¢égóp  */

   /* T0 8bit BEEPóp 4KHz */
   __byte_T0LR  = 0x00;               /* T0ìÆçÏéû:8bitPWM    */
   __byte_T0HR  = 0x00;               /* T0ìÆçÏéû:í‚é~    */
   __byte_T0CNT = BIN_00010000;         /* T0∂≥›ƒ∏€Ø∏ OSC0   Chon xung nguon la thach anh . Luu y la xung nguon thach anh co ty le chia tan so la 1/32 => timer0 dung xung 1024Hz*/
   __byte_T0PR  = BIN_01100000;         /* MODE3,T0PRé¸ä˙ = (PR+1)Å~∂≥›ƒ∏€Ø∏   MODE3: TOL operation 8bit Toggle, TOH: Stopped*/

   /* T1 16BIT TIMER ACCOFFéûACC_DETí[éqÉ`ÉÉÉ^óp  Dung cho viec chatter ngı ACC_DET khi ACCOFF*/
   /* (0x270F(9999)+1)Å~1um = 10ms */
   __byte_T1LR  = 0x0F;
   __byte_T1HR  = 0x27;
   __byte_T1PR  = BIN_00000000; // 
   __byte_T1CNT = BIN_00001101; // dung xung internal RC 1Mhz
   
   /* T2 16BIT TIMER  (0x960+1)*(4+1)*83.3ns = 1ms  (0x9601 = 2401 he thap phan)*/
   __byte_T2LR   = 0x60;
   __byte_T2HR   = 0x09;
   __byte_T2CNT0 = BIN_00000000;
   __byte_T2CNT2 = BIN_00000100;
   
   /* ì˙ï™ïb∂≥›¿  ngay phut giay counter */
   __byte_DMSCNT = BIN_00000000;
   __byte_SEC    = BIN_00000000;
   __byte_MINL   = BIN_00000000;
   __byte_MINH   = BIN_00000000;
   __byte_DAYL   = BIN_00000000;
   __byte_DAYH   = BIN_00000000;
   __byte_DMSCNT = BIN_10000000;
   
   #if 1
   /* 25ìxA/Dïœä∑ √ﬁ∞¿â∫à  8bitäiî[⁄ºﬁΩ¿2 */
   __byte_CALC0L    = __byte_TEMPCONV0;
   /* 25ìxA/Dïœä∑ √ﬁ∞¿â∫à  4bitäiî[⁄ºﬁΩ¿2 */
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
   
   /*çÇâ∑ë§ï‚ê≥åãâ */
   __word_CALC1 = (WORD)tmp;
   
   tmp = COMPENSATE_CYCLE * BCOLD;
   tmp = tmp / (WORD)dif_q;
   tmp = tmp * DIF_TEMPERTURE;
   tmp = tmp / (WORD)dif_q;
   tmp = tmp * DIF_TEMPERTURE;   
   tmp = tmp / BIT_ADJUST;
   
   /*í·â∑ë§ï‚ê≥åãâ */
   __word_CALC4 = (WORD)tmp;
   
   /* ï‚ê≥ó í≤êÆ⁄ºﬁΩ¿(Ç∆ÇËÇ†Ç¶Ç∏0Ç…ê›íË) */
   __byte_CALC2     = BIN_00000000;
   /* A/Dïœä∑≥™≤ƒéûä‘ê›íË⁄ºﬁΩ¿ */
   __byte_CALC3     = BIN_00000011;
   
   #endif
   
   /* éûåv¿≤œ éûåv¿≤œí‚é~éû∂≥›ƒílï€éù,äÑçûÇ›é¸ä˙ 500ms  Clock timer  Giu lai gia tri counter khi stop clock timer, chu ki ngat 500ms */
   __byte_CKTIMCTRL = BIN_10001110;

   
   /* SIO0 */
   __byte_S0CNT = BIN_00000000;  // LSB First
   /* É{Å[ÉåÅ[ÉgÉNÉçÉbÉNê›íË Thiet lap toc do Baud*/
   __byte_S0BG = 0x0D;      /* 850kHz */
   __byte_S0BUF = 0x00;
   __byte_S0INTVL = BIN_00000000;
   
   
   /* UART2 */
   __byte_U2CNT0 = BIN_00000000;
   __byte_U2CNT1 = BIN_00000100;
   __byte_U2TBUF = BIN_00000000;
   __byte_U2BG   = CIO_UART_19200;      /*19200bpsÅ‡12MHz/{(U2BG+1)Å~4}*/
   
#if ( _WDT_ == 1 )   // _WDT_ duoc define trong model.h
   /* WDTäJén */
   __byte_WDTCR = BIN_00101001;   /*      WDT is in reset mode (bit 5(MDSEL) = 1)
                   |||||||+--- START   1:WDT START (0ÇÃèëÇ´çûÇ›Ç≈í‚é~Ç≈Ç´Ç»Ç¢)
                   ||||||+---- OVF     0:ñ\ëñåüèoéûÿæØƒã÷é~
                   |||||+----- USERFLG 0:å≈íË
                   ||||+------ PDNSTOP 0:HOLDXìÀì¸éûWDTÿæØƒ 1:HOLDXìÀì¸éûWDTí‚é~
                   |||+------- SRFLG   0:ÿæØƒé¿çsåüèoÃ◊∏ﬁ
                   ||+-------- MDSEL   0:≤›¿◊Ãﬂƒ”∞ƒﬁ 1:ÿæØƒ”∞ƒﬁ
                   |+--------- -       0:å≈íË
                   +---------- -       0:å≈íË  */
   /* WDT ∂≥›¿∏ÿ±  WDT conuter clear */
   __SFR_BITCLR(__WDTCR,0);  // lenh nam trong file cpuinsn.h o link SanyoX16\Gcc\SanyoInc\itron
#endif
}


/******************************************************************************
   ä÷   êî  ñº   : Io_port_initial_set
   à¯       êî   : Ç»Çµ
   ñﬂ   ÇË  íl   : Ç»Çµ
   ã@ î\ äT ó™   : ëSÇƒÇÃŒﬂ∞ƒÇÃèÛë‘Çèâä˙ê›íËÇ…ÇµÇ‹Ç∑°INITAILéûÇ…∫∞ŸÇµÇƒâ∫Ç≥Ç¢ÅB ---- Thiet lap gia tri cua toan bo PORT vao thoi giam ban dau. Xin hay goi khi Khoi tao 
    îı       çl : ê›íËílÇÕÅAio_port_ext.héQè∆ÅB   Gia tri thiet lap tham khao file io_port_ext.h
-------------------------------------------------------------------------------
   èCê≥óöóÅyîNåéì˙ÅzÅyVerÅzÅyñºëOÅz Åyê‡ñæÅz
-------------------------------------------------------------------------------
[sun100419] 16ÀﬁØƒœ≤∫›ópêVãKçÏê¨
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

#if 0   /* ñ¢égóp */
void  Io_int_initial_set( void )
{
   /* Õﬁ∞Ω¿≤œ 62.5ms start */
   __bf_BTCR.IE1 = 1;
   __bf_BTCR.RUN = __bfval_BTCR_RUN_RUN;
   
   /* éûåv¿≤œäÑçûÇ›ã÷é~ */
   __byte_CKTIMCTRL = BIN_10001110;
   
   /* ALARM_KEYäÑçûÇ›ã÷é~ */
   #if 0
   __bf_P0LAT.BIT4  = 0;
   __bf_P0DDR.BIT4  = 0;
   #endif
   __bf_P0FSA.P04IL = 0;
   __bf_P0FSA.P04IE = 0;
   
   /* ACC_DETäÑçûÇ›ãñâ¬ */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = 0;
   
}
#endif

/******************************************************************************
ACC OFF(í èÌ) Å® HOLDX
*******************************************************************************
   ä÷êîñº   ÅFIo_xxx_holdx_in
   à¯  êî   ÅFñ≥Çµ
   ñﬂÇËíl   ÅFñ≥Çµ
   ã@  î\   ÅFHOLDX IN éûÇÃ⁄ºﬁΩ¿ê›íËèàóù
------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
******************************************************************************/
void   Io_sfr_holdx_in( void )
{
   #if 0 /* HOLDXÇ…ì¸ÇÈÇ∆ÅAêßå‰ÇµÇ»Ç≠ÇƒÇÊÇ¢ */
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC0;   /* ºΩ√—∏€Ø∏ëIëÅFOSC0(32KHz) */
   __bf_OCR0.RCSTOP = 1;   /* RCî≠êUí‚é~ */
   #endif
   
   __HoldX();
}
void   Io_port_holdx_in( void )
{
   /* LCD_DO LOWèoóÕê›íË */
   pLCD_DO_FSA = 0;
   pLCD_DO_FSB = 0;
   pLCD_DO_LAT = 0;
   pLCD_DO_DDR = 1;
   /* LCD_CL LOWèoóÕê›íË */
   pLCD_CL_FSA = 0;
   pLCD_CL_FSB = 0;
   pLCD_CL_LAT = 0;
   pLCD_CL_DDR = 1;
}
void   Io_int_holdx_in( void )
{
   Io_beep_stop();
   
   /* Õﬁ∞Ω¿≤œ 62.5ms Stop ACC_DETÉ`ÉÉÉ^óp */
   //__bf_BTCR.RUN = 0;
   __bf_BTCR.IE1 = 0;
   
   /* éûåv¿≤œäÑçûÇ›äJén(500ms) */
   __byte_CKTIMCTRL = BIN_10001111;
   
   /* 10msÉ^ÉCÉ}äÑÇËçûÇ›í‚é~ÅöÅöÅö */
   __bf_T1CNT.IE = 0;
   __bf_T1CNT.RUN = 0;
   
   /* ACC_DETäÑçûÇ›ãñâ¬ */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_ENABLE;
   
   /* ALARMKEYåüèoäÑçûÇ›ãñâ¬ÅiAMFMÉâÉWÉIÇÃÇ›Åj */
   #if 0
   if ((Ma_model_dest() >= 2)&&(Ma_model_dest() <= 6))
   #else
   if (Ma_get_alarm() == TRUE)
   #endif
   {
      __bf_P0FSA.P04IE = 1;
      __bf_P0FSA.P04IL = 0;      /* LÉåÉxÉã */
   }
}

/******************************************************************************
HOLDX Å® ACC OFF(í èÌ)
*******************************************************************************
   ä÷êîñº   ÅFIo_xxx_holdx_out
   à¯  êî   ÅFñ≥Çµ
   ñﬂÇËíl   ÅFñ≥Çµ
   ã@  î\   ÅFHOLDX OUT éûÇÃ⁄ºﬁΩ¿ê›íËèàóù
------------------------------------------------------------------------------
******************************************************************************/
void   Io_sfr_holdx_out(void)
{
#if ( _WDT_ == 1 )
   /* WDTäJén */
   __byte_WDTCR = BIN_00101001;   /*
                   |||||||+--- START   1:WDT START (0ÇÃèëÇ´çûÇ›Ç≈í‚é~Ç≈Ç´Ç»Ç¢)
                   ||||||+---- OVF     0:ñ\ëñåüèoéûÿæØƒã÷é~
                   |||||+----- USERFLG 0:å≈íË
                   ||||+------ PDNSTOP 0:HOLDXìÀì¸éûWDTÿæØƒ 1:HOLDXìÀì¸éûWDTí‚é~
                   |||+------- SRFLG   0:ÿæØƒé¿çsåüèoÃ◊∏ﬁ
                   ||+-------- MDSEL   0:≤›¿◊Ãﬂƒ”∞ƒﬁ 1:ÿæØƒ”∞ƒﬁ
                   |+--------- -       0:å≈íË
                   +---------- -       0:å≈íË  */
   /* WDT ∂≥›¿∏ÿ±  WDT counter clear */
   __SFR_BITCLR(__WDTCR,0);
#endif

   #if 0 /* HOLDXÇ…ì¸ÇÈÇ∆ÅAêßå‰ÇµÇ»Ç≠ÇƒÇÊÇ¢ */
   __bf_OCR0.RCSTOP = 0;   /* RCî≠êUäJén */
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;   /* ºΩ√—∏€Ø∏ëIëÅFRCî≠êU(1MHz) */
   #endif
}
void   Io_port_holdx_out( void )
{
   /* LCD_DO SIOÉfÅ[É^(CMOS)ê›íË */
   pLCD_DO_FSA = 1;
   pLCD_DO_FSB = 0;
   pLCD_DO_LAT = 0;
   pLCD_DO_DDR = 1;
   /* LCD_CL SIOÉNÉçÉbÉN(CMOS)ê›íË */
   pLCD_CL_FSA = 1;
   pLCD_CL_FSB = 0;
   pLCD_CL_LAT = 0;
   pLCD_CL_DDR = 1;
}
void   Io_int_holdx_out( void )
{
   /* Õﬁ∞Ω¿≤œ 62.5ms start ACC_DETÉ`ÉÉÉ^óp */
   //__bf_BTCR.RUN = 1;
   __bf_BTCR.IE1 = 1;
   
   /* éûåv¿≤œäÑçûÇ›ã÷é~ */
   __byte_CKTIMCTRL = BIN_10001110;
   
   /* 10msÉ^ÉCÉ}äÑÇËçûÇ›ãñâ¬ÅöÅöÅö */
   __bf_T1CNT.IE = 1;
   __bf_T1CNT.RUN = 1;
   
   /* ACC_DETäÑçûÇ›í‚é~ */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
   
   /* ALARMKEYåüèoäÑçûÇ›ã÷é~ */
   __bf_P0FSA.P04IE = 0;
   __bf_P0FSA.P04IL = 0;
   __bf_P0FSA.P04FLG = 0;
}






/******************************************************************************
ACC ON(POWER OFF) Å® ACC OFF(í èÌ)
*******************************************************************************
   ä÷êîñº   ÅFIo_xxx_accoff_in
   à¯  êî   ÅFñ≥Çµ
   ñﬂÇËíl   ÅFñ≥Çµ
   ã@  î\   ÅFPOWER OFF IN éûÇÃ⁄ºﬁΩ¿ê›íËèàóù
           ÉVÉXÉeÉÄÉNÉçÉbÉNOSC0ëIë
           RCî≠êU/OSC1í‚é~
------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
******************************************************************************/
void   Io_sfr_accoff_in( void )
{
   volatile WORD   wait_cnt ;
   
   __bf_OCR0.RCSTOP = 0;   /* RCî≠êUäJén */
   
     /* RCî≠êUà¿íË  */
   for (wait_cnt = 0; wait_cnt < 0x10; wait_cnt++)
   {
   }
   
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;
   
   __bf_OCR0.ENOSC1 = 0;         /* OSC1 Stop */
   
}
void   Io_port_accoff_in( void )
{
   /* I2C_SCK LOWèoóÕê›íË */
   pI2C_SCK_FSA = 0;
   pI2C_SCK_FSB = 0;
   pI2C_SCK_LAT = 0;
   pI2C_SCK_DDR = 1;
   /* I2C_SDA LOWèoóÕê›íË */
   pI2C_SDA_FSA = 0;
   pI2C_SDA_FSB = 0;
   pI2C_SDA_LAT = 0;
   pI2C_SDA_DDR = 1;
}
void   Io_int_accoff_in( void )
{
   /* Õﬁ∞Ω¿≤œ 62.5ms äJén ACC_DETÉ`ÉÉÉ^óp */
   //__bf_BTCR.RUN = 1;
   __bf_BTCR.IE1 = 1;
   
   /* 1ms¿≤œí‚é~ */
   __byte_T2CNT0 = BIN_00000000;
   
   /* 10msÉ^ÉCÉ}äÑÇËçûÇ›ãñâ¬ÅöÅöÅö */
   __bf_T1CNT.IE = 1;
   __bf_T1CNT.RUN = 1;
   
   /* ACC_DETäÑçûÇ›í‚é~ */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
}

/******************************************************************************
ACC OFF(í èÌ) Å® ACC ON(POWER OFF)
*******************************************************************************
   ä÷êîñº   ÅFIo_xxx_accoff_out
   à¯  êî   ÅFñ≥Çµ
   ñﬂÇËíl   ÅFñ≥Çµ
   ã@  î\   ÅFPOWER OFF IN éûÇÃ⁄ºﬁΩ¿ê›íËèàóù
           ÉVÉXÉeÉÄÉNÉçÉbÉNOSC0ëIë
           RCî≠êU/OSC1í‚é~
------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
******************************************************************************/
void   Io_sfr_accoff_out( void )
{
   volatile WORD   wait_cnt ;
   
   __bf_OCR0.ENOSC1 = 1;         /* OSC1 Start */

   /* 50ms à»è„Wait   Cho hon 50ms */
   for (wait_cnt = 0; wait_cnt < 0x0A0; wait_cnt++)
   {
   }
   
   
   __bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC1;
   
   __bf_OCR0.RCSTOP = __bfval_OCR0_RCSTOP_STOP;   /* RCî≠êUí‚é~  Ngung dao dong RC */
   
}
void   Io_port_accoff_out( void )
{
   /* I2C_SCK HIGHèoóÕê›íË */
   pI2C_SCK_FSA = 0;
   pI2C_SCK_FSB = 0;
   pI2C_SCK_LAT = 1;
   pI2C_SCK_DDR = 1;
   /* I2C_SDA HIGHèoóÕê›íË */
   pI2C_SDA_FSA = 0;
   pI2C_SDA_FSB = 0;
   pI2C_SDA_LAT = 1;
   pI2C_SDA_DDR = 1;
}
void   Io_int_accoff_out( void )
{
   /* Õﬁ∞Ω¿≤œ 62.5ms í‚é~ ACC_DETÉ`ÉÉÉ^óp Dung cho quÈt port ACC_DET, base timer 62.5ms (Xem them sheet 5 file Gaiyousekkei)*/
   //__bf_BTCR.RUN = 0;
   __bf_BTCR.IE1 = 0;
   
   /* 1ms¿≤œäJén   Bat dau timer 1ms*/
   __byte_T2CNT0 = BIN_00000101;
   
   /* 10msäÑÇËçûÇ›í‚é~ AccOFFéûAlarmÉLÅ[åüèoóp  Dung ngat 10ms, dung cho viec detect key ALARM trong khi ACCOFF */
   __bf_T1CNT.IE = 0;
   __bf_T1CNT.RUN = 0;
   
   /* ACC_DETäÑçûÇ›í‚é~   Dung ngat ACC OFF */
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
   /* éûåvÉ^ÉCÉ}500msäÑÇËçûÇ›ã÷é~ */
   __byte_CKTIMCTRL = BIN_10001110;
}

void Io_int_disable_almkey(void)
{
   /* ALARMKEYåüèoäÑçûÇ›ã÷é~ */
   __bf_P0FSA.P04IE = 0;
   __bf_P0FSA.P04IL = 0;
}

void Io_int_disable_accdet(void)
{
   /* ACC_DETäÑçûÇ›í‚é~ */
   __bf_INT23CR.INT3IF = 0;
   __bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
}
/******************************************************************************
   ä÷êîñº   ÅFIO_In_acc_det
   à¯  êî   ÅFñ≥Çµ
   ñﬂÇËíl   ÅFHIGH
            LOW
   ã@  î\   ÅFACC-DETí[éqèÛë‘ï‘ãp
------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
******************************************************************************/
BOOL   IO_In_acc_det(void)
{
   fIO_acc = MIO_IN_pACC_DET();

#if 0   /* îΩì]ÇégópÇµÇ»Ç¢ÇÃÇ≈ÉRÉÅÉìÉgÉAÉEÉg */
   if ((pACC_DET_FSA == 0) &&
      (pACC_DET_FSB == 1))
   {
      fIO_acc = ~fIO_acc;
   }
#endif
   return(fIO_acc);
}
/******************************************************************************
   ä÷êîñº   ÅFIO_In_ill_det
   à¯  êî   ÅFñ≥Çµ
   ñﬂÇËíl   ÅFHIGH
            LOW
   ã@  î\   ÅFILL-DETí[éqèÛë‘ï‘ãp
------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
******************************************************************************/
BOOL   IO_In_ill_det(void)
{
   fIO_ill = MIO_IN_pILL_DET();
   return(fIO_ill);
}
/******************************************************************************
   ä÷êîñº   ÅFIO_In_acc_det
   à¯  êî   ÅFñ≥Çµ
   ñﬂÇËíl   ÅFHIGH
            LOW
   ã@  î\   ÅFALARM_KEYí[éqèÛë‘ï‘ãp
------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
******************************************************************************/
BOOL   IO_In_alarm_key(void)
{
   fIO_alarm = MIO_IN_pALARM_KEY();
   return(fIO_alarm);
}

/******************************************************************************
   ä÷  êî ñº   : Io_beep_start
   à¯     êî   : ñ≥Çµ
   ñﬂ  ÇË íl   : ñ≥Çµ
   ã@î\äTó™   : 2kHzÇÃBEEPÇñ¬ìÆÇµÇ‹Ç∑
    îı     çl    : í‚é~ÇÕÅA Io_beep_stop ÇÉRÅ[ÉãÇµÇƒÇ≠ÇæÇ≥Ç¢
-------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
[20130415yamamoto] édólïœçX 4kHzÅÀ2kHz
********************************************************************************/
void Io_beep_start( void )
{
   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;      /* ÉgÉOÉãèoóÕí‚é~ */

   __bf_T0CNT.CKSEL = __bfval_T0CNT_CKSEL_OSC0;
   
   __byte_T0LR = 0x07;                        /* MODE3 2kHz */
   __bf_T0PR.PR = 0;
   
   __bf_T0PR.MODE = __bfval_T0PR_MODE_MODE3;
   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_RUN;         /* ÉgÉOÉãèoóÕäJén */
}
/******************************************************************************
   ä÷  êî ñº   : Io_beep_start
   à¯     êî   : ñ≥Çµ
   ñﬂ  ÇË íl   : ñ≥Çµ
   ã@î\äTó™   : 8HzÇÃBEEPÇñ¬ìÆÇµÇ‹Ç∑,TESTÉÇÅ[Éhóp
    îı     çl    : í‚é~ÇÕÅA Io_beep_stop ÇÉRÅ[ÉãÇµÇƒÇ≠ÇæÇ≥Ç¢
-------------------------------------------------------------------------------
[20120926yamamoto] êVãKçÏê¨
********************************************************************************/
void Io_beep_test_8Hz_start( void )
{
   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;      /* ÉgÉOÉãèoóÕí‚é~ */

   __bf_T0CNT.CKSEL = __bfval_T0CNT_CKSEL_OSC0;
   
   __byte_T0LR = 0xFF;                     /* MODE3 8Hz */
   __byte_T0PR = 0x67;                     /* √Ωƒ”∞ƒﬁóp */

   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_RUN;         /* ÉgÉOÉãèoóÕäJén */
}


/******************************************************************************
   ä÷  êî ñº   : Io_beep_stop
   à¯     êî   : ñ≥Çµ
   ñﬂ  ÇË íl   : ñ≥Çµ
   ã@î\äTó™   : 4kHzÇÃBEEPÇí‚é~ÇµÇ‹Ç∑
    îı     çl    :
-------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
********************************************************************************/
void Io_beep_stop( void )
{
   __bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;      /* ÉgÉOÉãèoóÕí‚é~ */
}
/******************************************************************************
   ä÷  êî ñº   : Io_mincnt_read
   à¯     êî   : ñ≥Çµ
   ñﬂ  ÇË íl   : 0-1439(ï™)
   ã@î\äTó™   : ì˙ï™ïbÉJÉEÉìÉ^ÇÃï™ÇÃílÇì«Ç›èoÇ∑
    îı     çl    : ïKÇ∏ì˙ï™ïbÉJÉEÉìÉ^Ç™ìÆçÏÇµÇƒÇ¢ÇÈéûÇ…égópÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB
-------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
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
   
   /* ï™ïbÇòAë±ÇµÇƒì«Ç›èoÇµÇƒàÍívÇ∑ÇÍÇŒï™Çreturn */
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
   ä÷  êî ñº   : Io_mincnt_write
   à¯     êî   : WORD minÅFï™ÇÃílÇçXêVÇµÇ‹Ç∑ÅB
   ñﬂ  ÇË íl   : ñ≥Çµ
   ã@î\äTó™   : ï™ÉJÉEÉìÉ^ÇÃílÇçXêVÇ∑ÇÈ
    îı     çl    : 
-------------------------------------------------------------------------------
[20120731yamamoto] êVãKçÏê¨
[20121017yamamoto] édólïœçXëŒâû éûåv¿≤œÇí‚é~Ç∑ÇÈólÇ…ïœçX
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
   ä÷   êî  ñº   : Io_wdt_clear
   à¯       êî   : Ç»Çµ
   ñﬂ   ÇË  íl   : Ç»Çµ
   ã@ î\ äT ó™   : ÉEÉHÉbÉ`ÉhÉbÉOÉ^ÉCÉ}ÇÃÉNÉäÉAÇçsÇ§
    îı       çl : 
-------------------------------------------------------------------------------
èCê≥óöó
   î‘çÜ   ì˙ït       íSìñé“      èCê≥ì‡óe
-------------------------------------------------------------------------------
  Åy000Åz   09/6/18    ç≤ãvìc      êVãKçÏê¨
********************************************************************************/
void Io_wdt_clear(void)
{
   /* WDT ∂≥›¿∏ÿ± */
   __SFR_BITCLR(__WDTCR,0);
}

#endif


#undef   _IO_SFR_C_

