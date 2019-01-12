/******************************************************************************
   �N�����I���a����                    2012  AMFM���W�I
      ��ۼު�Ė�   : '12�N AMFM׼޵�J��
      ̧�ٖ�      : IO_def.h
        Ӽޭ�ٖ�   : I/O
      �@�\      : PORT��`
------------------------------------------------------------------------------
   �C�������y�N�����z�yVer�z�y���O�z �y�����z
------------------------------------------------------------------------------
[yamamoto20120727] �V�K�쐬
******************************************************************************/
#ifndef   _IO_PORT_DEF_H_
#define   _IO_PORT_DEF_H_

#include   "../model.h"
#define      DEFINE_SFRBITS

#include <lc884200.h>


/*************************************************
         PORT0 
**************************************************/
/*****************  ���b�` LATCH ****************/
#define     pSYS_ON_LAT         __bf_P0LAT.BIT0          /* [O]    */
#define      pALARM_LAT         __bf_P0LAT.BIT1          /* [O]    */
#define    pMUTE_LAT          __bf_P0LAT.BIT2          /* [O]    */
#define      pSTBY_LAT         __bf_P0LAT.BIT3          /* [O]    */
#define     pALARM_KEY_LAT      __bf_P0LAT.BIT4         /* [O]    */
#define      pILL_DET_LAT      __bf_P0LAT.BIT5          /* [O]    */
#define      pBEEP_LAT         __bf_P0LAT.BIT6          /* [O]    */
#define     pLCD_CE_LAT         __bf_P0LAT.BIT7          /* [O]    */
/*****************  ���� INPUT ******************/
#define     pSYS_ON_IN         __bf_P0IN.BIT0          /* [O]    */
#define      pALARM_IN         __bf_P0IN.BIT1          /* [O]    */
#define    pMUTE_IN         __bf_P0IN.BIT2          /* [O]    */
#define      pSTBY_IN         __bf_P0IN.BIT3          /* [O]    */
#define     pALARM_KEY_IN      __bf_P0IN.BIT4          /* [O]    */
#define     pILL_DET_IN         __bf_P0IN.BIT5          /* [O]    */
#define      pBEEP_IN         __bf_P0IN.BIT6          /* [O]    */
#define     pLCD_CE_IN         __bf_P0IN.BIT7          /* [O]    */
/*****************  DDR ( Data Direction Register)  ******************/
#define     pSYS_ON_DDR         __bf_P0DDR.BIT0          /* [O]    */
#define      pALARM_DDR         __bf_P0DDR.BIT1          /* [O]    */
#define    pMUTE_DDR          __bf_P0DDR.BIT2          /* [O]    */
#define      pSTBY_DDR         __bf_P0DDR.BIT3          /* [O]    */
#define     pALARM_KEY_DDR      __bf_P0DDR.BIT4         /* [O]    */
#define      pILL_DET_DDR      __bf_P0DDR.BIT5          /* [O]    */
#define      pBEEP_DDR         __bf_P0DDR.BIT6          /* [O]    */
#define     pLCD_CE_DDR         __bf_P0DDR.BIT7          /* [O]    */
/*****************  FSA Function Control Register ******************/
#define     pSYS_ON_FSA         __bf_P0FSA.BIT0          /* [O]    */
#define      pALARM_FSA         __bf_P0FSA.BIT1          /* [O]    */
#define    pMUTE_FSA          __bf_P0FSA.BIT2          /* [O]    */
#define      pSTBY_FSA         __bf_P0FSA.BIT3          /* [O]    */
#define     pALARM_KEY_FSA      __bf_P0FSA.BIT4         /* [O]    */
#define      pILL_DET_FSA      __bf_P0FSA.BIT5          /* [O]    */
#define      pBEEP_FSA         __bf_P0FSA.BIT6          /* [O]    */
#define     pLCD_CE_FSA         __bf_P0FSA.P05IL          /* [O]    */
/*****************  P023INT  **************/
#define     pSYS_ON_23INT      __bf_P023INT.BIT0       /* [O]    */
#define      pALARM_23INT      __bf_P023INT.BIT1       /* [O]    */
#define    pMUTE_23INT       __bf_P023INT.BIT2       /* [O]    */
#define      pSTBY_23INT         __bf_P023INT.BIT3       /* [O]    */
#define     pALARM_KEY_23INT   __bf_P023INT.BIT4      /* [O]    */
#define      pILL_DET_23INT      __bf_P023INT.BIT5       /* [O]    */
#define      pBEEP_23INT         __bf_P023INT.BIT6       /* [O]    */
#define     pLCD_CE_23INT      __bf_P023INT.BIT7       /* [O]    */
/****************  BYTE����  Kiem soat BYTE  ***************/
#define      pP0_LAT_BYTE      __byte_P0LAT      /* �|�[�g0�f�[�^���b�`      */
#define      pP0_IN_BYTE         __byte_P0IN         /* �|�[�g0���̓A�h���X      */
#define      pP0_DDR_BYTE      __byte_P0DDR      /* �|�[�g0�@�\���䃌�W�X�^   */
#define      pP0_FSA_BYTE      __byte_P0FSA      /* �|�[�g0�@�\���䃌�W�X�^   */
#define      pP0_23INT_BYTE      __byte_P023INT      /* �|�[�g0�@�\���䃌�W�X�^   */

/* _P0LAT �ݒ�  Thiet lap P0LAT*/
#define      CP0_LAT_INI         BIN_00000001   /* �|�[�g0�f�[�^���b�`(Initial��)   */
/* _P0DDR �ݒ�  Thiet lap P0DDR*/
#if _POWER_DIAG_
#define      CP0_DDR_INI         BIN_11111101   /* �|�[�g0DDR         (Initial��)   */
#else
#define      CP0_DDR_INI         BIN_11111111   /* �|�[�g0DDR         (Initial��)   */
#endif
/* _P0FSA �ݒ�  Thiet lap P0FSA*/
#define      CP0_FSA_INI         BIN_00000000   /* �|�[�g0�@�\���䃌�W�X�^ (Initial��)   */
/* _P023INT �ݒ�  Thiet lap _P023INT */
#define      CP0_23INT_INI      BIN_00000000   /* �|�[�g02,03���荞�ݐ��䃌�W�X�^ (Initial��)   */

/*************************************************
         PORT1
**************************************************/
/*****************  �o��  ******************/
#define    pLCD_DO_LAT         __bf_P1LAT.BIT0      /* [O]   LCD��ײ��CE�o�͒[�q   */
#define    pLCD_DI_LAT         __bf_P1LAT.BIT1      /* [O] LCD��ײ���ް��o�͒[�q */
#define    pLCD_CL_LAT         __bf_P1LAT.BIT2      /* [O] LCD CLK�M���o�͒[�q */
#define     pI2C_SCK_LAT      __bf_P1LAT.BIT3
#define    pI2C_SDA_LAT      __bf_P1LAT.BIT4      /* [O] TIME BASE TEST�o�͒[�q */
#define    pACC_DET_LAT      __bf_P1LAT.BIT5
#define    pUART_RX_LAT      __bf_P1LAT.BIT6
#define      pUART_TX_LAT      __bf_P1LAT.BIT7      /* [0] HDRADIO UART TX�[�q */
/*****************  ����  ******************/
#define    pLCD_DO_IN         __bf_P1IN.BIT0
#define    pLCD_DI_IN         __bf_P1IN.BIT1
#define    LCD_CL_IN         __bf_P1IN.BIT2
#define     pI2C_SCK_IN         __bf_P1IN.BIT3/* [I] �t�r�a�R�l�N�^�̃V���[�g���o     */
#define    pI2C_SDA_IN         __bf_P1IN.BIT4
#define    pACC_DET_IN         __bf_P1IN.BIT5
#define    pUART_RX_IN           __bf_P1IN.BIT6/* [I] HDRADIO UART RX�[�q */
#define      pUART_TX_IN         __bf_P1IN.BIT7
/*****************  ����  ******************/
#define    pLCD_DO_DDR         __bf_P1DDR.BIT0
#define    pLCD_DI_DDR         __bf_P1DDR.BIT1   
#define    pLCD_CL_DDR         __bf_P1DDR.BIT2   
#define     pI2C_SCK_DDR      __bf_P1DDR.BIT3
#define    pI2C_SDA_DDR      __bf_P1DDR.BIT4   
#define    pACC_DET_DDR      __bf_P1DDR.BIT5   
#define    pUART_RX_DDR      __bf_P1DDR.BIT6   
#define      pUART_TX_DDR      __bf_P1DDR.BIT7
/*****************  FSA  *******************/
#define    pLCD_DO_FSA         __bf_P1FSA.BIT0
#define    pLCD_DI_FSA         __bf_P1FSA.BIT1   
#define    pLCD_CL_FSA         __bf_P1FSA.BIT2   
#define     pI2C_SCK_FSA      __bf_P1FSA.BIT3
#define    pI2C_SDA_FSA      __bf_P1FSA.BIT4   
#define    pACC_DET_FSA      __bf_P1FSA.BIT5   
#define    pUART_RX_FSA      __bf_P1FSA.BIT6   
#define      pUART_TX_FSA      __bf_P1FSA.BIT7
/*****************  FSB  ********************/
#define    pLCD_DO_FSB         __bf_P1FSB.BIT0
#define    pLCD_DI_FSB         __bf_P1FSB.BIT1   
#define    pLCD_CL_FSB         __bf_P1FSB.BIT2   
#define     pI2C_SCK_FSB      __bf_P1FSB.BIT3
#define    pI2C_SDA_FSB      __bf_P1FSB.BIT4   
#define    pACC_DET_FSB      __bf_P1FSB.BIT5   
#define    pUART_RX_FSB      __bf_P1FSB.BIT6   
#define      pUART_TX_FSB      __bf_P1FSB.BIT7
/*****************  IIC  ********************/
/* [20120730yamamoto]IIC�|�[�g���p�̂��ߕʓr��` */
#define      pTU_SCL_FSA         __bf_P1FSA.BIT3
#define      pTU_SCL_FSB         __bf_P1FSB.BIT3
#define      pTU_SCL_LAT         __bf_P1LAT.BIT3
#define      pTU_SCL_DDR         __bf_P1DDR.BIT3
#define    pTU_SCL_IN         __bf_P1IN.BIT3

#define      pTU_SDA_FSA         __bf_P1FSA.BIT4
#define      pTU_SDA_FSB         __bf_P1FSB.BIT4
#define      pTU_SDA_LAT         __bf_P1LAT.BIT4
#define      pTU_SDA_DDR         __bf_P1DDR.BIT4
#define     pTU_SDA_IN         __bf_P1IN.BIT4

#define      pE2P_SCL_FSA      __bf_P1FSA.BIT3
#define      pE2P_SCL_FSB      __bf_P1FSB.BIT3
#define      pE2P_SCL_LAT      __bf_P1LAT.BIT3
#define      pE2P_SCL_DDR      __bf_P1DDR.BIT3
#define    pE2P_SCL_IN         __bf_P1IN.BIT3

#define      pE2P_SDA_FSA      __bf_P1FSA.BIT4
#define      pE2P_SDA_FSB      __bf_P1FSB.BIT4
#define      pE2P_SDA_LAT      __bf_P1LAT.BIT4
#define      pE2P_SDA_DDR      __bf_P1DDR.BIT4
#define     pE2P_SDA_IN         __bf_P1IN.BIT4

#define      pEVOL_CLK_FSA      __bf_P1FSA.BIT3
#define      pEVOL_CLK_FSB      __bf_P1FSB.BIT3
#define      pEVOL_CLK_LAT      __bf_P1LAT.BIT3
#define      pEVOL_CLK_DDR      __bf_P1DDR.BIT3
#define    pEVOL_CLK_IN      __bf_P1IN.BIT3

#define      pEVOL_DATA_FSA      __bf_P1FSA.BIT4
#define      pEVOL_DATA_FSB      __bf_P1FSB.BIT4
#define      pEVOL_DATA_LAT      __bf_P1LAT.BIT4
#define      pEVOL_DATA_DDR      __bf_P1DDR.BIT4
#define     pEVOL_DATA_IN      __bf_P1IN.BIT4
/****************  BYTE����  ***************/
#define      pP1_LAT_BYTE      __byte_P1LAT
#define      pP1_IN_BYTE         __byte_P1IN
#define      pP1_DDR_BYTE      __byte_P1DDR
#define      pP1_FSA_BYTE      __byte_P1FSA
#define      pP1_FSB_BYTE      __byte_P1FSB

#if 1/* �o�X�t�@�C�g�΍� */
/* _P1LAT �ݒ�  */
#define      CP1_LAT_INI         BIN_01000010   /* �|�[�g0�f�[�^���b�`(Initial��)   */
/* _P1DDR �ݒ�  */
#define      CP1_DDR_INI         BIN_11000111   /* �|�[�g0DDR         (Initial��)   */
#else /* �o�X�t�@�C�g�΍� */
/* _P1LAT �ݒ�  */
#define      CP1_LAT_INI         BIN_01011010   /* �|�[�g0�f�[�^���b�`(Initial��)   */
/* _P1DDR �ݒ�  */
#define      CP1_DDR_INI         BIN_11011111   /* �|�[�g0DDR         (Initial��)   */
#endif/* �o�X�t�@�C�g�΍� */

/* _P1FSA �ݒ�  */
#define      CP1_FSA_INI         BIN_11000111   /* �|�[�g0�@�\���䃌�W�X�^ (Initial��)   */
/* _P1FSB �ݒ�  */
#define      CP1_FSB_INI         BIN_01000010   /* �|�[�g0�@�\���䃌�W�X�^ (Initial��)   */

#if 0/*  */
/*******************************************/
/*      SIO0 �ʐM��`    */
/*******************************************/
#define         pLCD_CE_FSA      
#define         pLCD_CE_LAT      
#define         pLCD_CE_DDR      
#define         /* SIO ���� */
#define         pLCD_SIO_FSA   
#define         pLCD_SIO_FSB   
#define         pLCD_SIO_LAT   
#define         pLCD_SIO_DDR   
#define         /* CLK HIGH */
#define         pLCD_CLK_FSA   
#define         pLCD_CLK_FSB   
#define         pLCD_CLK_LAT   
#define         pLCD_CLK_DDR   

/*******************************************/
/*      IIC �ʐM��`    */
/*******************************************/
#define _pbyte_EVOL_CNT       ((volatile unsigned char*)__SMIC0CNT)
#define _byte_EVOL_CNT       (*_pbyte_EVOL_CNT)
#define _pbf_EVOL_CNT      ((volatile __bitfield_SMIC0CNT*)__SMIC0CNT)
#define _bf_EVOL_CNT      (*_pbf_EVOL_CNT)

#define _pbyte_EVOL_STA    ((volatile unsigned char*)__SMIC0STA)
#define _byte_EVOL_STA       (*_pbyte_EVOL_STA)
#define _pbf_EVOL_STA       ((volatile __bitfield_SMIC0STA*)__SMIC0STA)
#define _bf_EVOL_STA       (*_pbf_EVOL_STA)

#define _pbyte_EVOL_BRG       ((volatile unsigned char*)__SMIC0BRG)
#define _byte_EVOL_BRG       (*_pbyte_EVOL_BRG)
#define _pbf_EVOL_BRG       ((volatile __bitfield_SMIC0BRG*)__SMIC0BRG)
#define _bf_EVOL_BRG       (*_pbf_EVOL_BRG)

#define _pbyte_EVOL_BUF      ((volatile unsigned char*)__SMIC0BUF)
#define _byte_EVOL_BUF       (*_pbyte_EVOL_BUF)

#define _pbyte_EVOL_PCNT    ((volatile unsigned char*)__SMIC0PCNT)
#define _byte_EVOL_PCNT    (*_pbyte_EVOL_PCNT)
#endif



#endif   /* _IO_PORT_DEF_H_ */

