/******************************************************************************
   クラリオン殿向け                    2013年 AMFMﾗｼﾞｵｼﾘｰｽﾞ
      ﾌﾟﾛｼﾞｪｸﾄ名   : '12年 AMFMﾗｼﾞｵ開発
      ﾌｧｲﾙ名      : IO_ext.h
        ﾓｼﾞｭｰﾙ名   : I/O
      機能      : PORT定義 外部公開用
------------------------------------------------------------------------------
   修正履歴【年月日】【Ver】【名前】 【説明】
------------------------------------------------------------------------------
[yamamoto20120727] 新規作成
******************************************************************************/
#ifndef   _IO_EXT_H_
#define   _IO_EXT_H_

#include   "../model.h"

#undef   EXT
#ifdef   _IO_SFR_C_
#define   EXT
#else
#define   EXT   extern
#endif


EXT   BYTE   IO_Uart2_RXData;
/*----------------------------------------------------
     出力ポート設定 マクロ定義       Ouput Port    Macro Define
         引数(value)へ HIGH or LOW を設定すること      Thiet lap HIGH or LOW len doi so (value)
----------------------------------------------------*/

#if 0   /* バスファイト対策 */
/* Tuner制御ﾎﾟｰﾄ Port kiem soat Tuner */    
#define      MIO_OUT_pTU_CLK(value)      MP_X( pTU_SCL_LAT, (value))
#define      MIO_OUT_pTU_DATA(value)      MP_X( pTU_SDA_LAT, (value))
#define    MIO_IN_pTU_DATA()         (pTU_SDA_IN   )
#define    MIO_CHG_pTU_DATA(value)      MP_X( pTU_SDA_DDR , (value))

/* E-VOL制御ポート  Port Kiem soat E-VOL*/  
#define      MIO_OUT_pEVOL_CLK(value)   MP_X( pEVOL_CLK_LAT, (value))
#define      MIO_OUT_pEVOL_DATA(value)   MP_X( pEVOL_DATA_LAT, (value))
#define    MIO_IN_pEVOL_DATA()         (pEVOL_DATA_IN   )
#define    MIO_CHG_pEVOL_DATA(value)   MP_X( pEVOL_DATA_DDR , (value))

#endif

/* 電源関係ポート   PORT lien quan den nguon dien*/
#if _POWER_DIAG_
#define    MIO_IN_pDIAG()            ( pALARM_IN )
#else
#define    MIO_OUT_pALARM(value)      MP_X( pALARM_LAT, (value))
#endif /* _POWER_DIAG_ */
#define    MIO_OUT_pSTBY(value)      MP_X( pSTBY_LAT, (value))
#define    MIO_OUT_pSYS_ON(value)      MP_X( pSYS_ON_LAT, (value))

#define    MIO_CHG_pILL_DET(value)      MP_X( pILL_DET_DDR, (value))
#define    MIO_IN_pILL_DET()         (pILL_DET_IN   )   /* ILLMI ON入力監視端子            */
#define    MIO_IN_pACC_DET()         (pACC_DET_IN   )
#define    MIO_IN_pALARM_KEY()         (pALARM_KEY_IN   )

/* E-VOL関係ポート PORT lien quan den E-VOL */
#define    MIO_OUT_pSYS_MUTE(value)   MP_X( pMUTE_LAT , (value))
#define      MIO_OUT_pBEEP(value)      MP_X( pBEEP_LAT, (value))

/* EEPROM関係ﾎﾟｰﾄ PORT lien quan den EEPROM */
#define      MIO_OUT_pE2P_SCL(value)    MP_X( pE2P_SCL_LAT, (value))
#define      MIO_OUT_pE2P_SDA(value)    MP_X( pE2P_SDA_LAT, (value))
#define    MIO_IN_pE2P_SDA()          (pE2P_SDA_IN   )
#define    MIO_CHG_pE2P_SDA(value)      MP_X( pE2P_SDA_DDR , (value))
#define    MIO_CHG_pE2P_SCL(value)      MP_X( pE2P_SCL_DDR , (value))

/* LCDﾄﾞﾗｲﾊﾞ制御 Kiem soat Driver LCD*/
#define      MIO_OUT_pLCDCE(value )         MP_X( pLCD_CE_LAT , (value))
#define      MIO_OUT_pLCDSO(value )         MP_X( pLCD_SIO_LAT , (value))
#define      MIO_OUT_pLCDCLK(value )         MP_X( pLCD_CL_LAT , (value))
#define      MIO_IN_pLCDDI()               ( pLCD_DI_IN )

/*  */
enum
{
   CIO_INIT,
//   CIO_NORMAL,
//   CIO_HALT,
//   CIO_STOP,
   CIO_HOLDX,
   CIO_ACCOFF,
   CIO_POWOFF,
   CIO_POWON
};


/*----------------------------------------------------
     関数ﾌﾟﾛﾄﾀｲﾌﾟ  :  Funtion Prototype 
----------------------------------------------------*/
/* TESTMODE関係通信IF      Su truyen tin lien quan toi TESTMODE */
EXT   void IO_Test_SetUartPort(BYTE Status);
EXT   void IO_Test_TxStart(BYTE tx_data);
EXT   void IO_Test_TxNext(BYTE tx_data);
EXT   void IO_Test_TxEnd(void);
EXT   void IO_Test_TxStop(void);
EXT   void IO_Test_RxStart(void);
EXT   BYTE IO_Test_Get_RxData(void);
EXT   void IO_Test_RxStop(void);

EXT   void  IO_SetIICPort(BYTE status);

/* TUNER関係通信IF  Su truyen tin lien quan toi TUNER*/
EXT   void IO_Tuner_Receive(BYTE *,BYTE  );
EXT   BYTE IO_Tuner_Send(BYTE *, BYTE );
EXT   void IO_Tuner_Send_Start(void);
EXT   void IO_Tuner_Send_Stop(void);

/* E-VOL関係通信IF      Su truyen tin lien quan toi E-VOL*/
#if 0
EXT   void  IO_EVOL_SetIICPort(BYTE );
#endif
EXT   BYTE IO_EVOL_Send(BYTE *, BYTE );

#if _MODEL_EEPROM_
EXT void IO_EEP_Port_Reset(void);
EXT void IO_EEP_SendStart( void );
EXT   void IO_EEP_SendRestart( void );
EXT void IO_EEP_SendStop( void );

EXT BYTE IO_EEP_WriteByte(BYTE   data);
EXT void IO_EEP_ReadData(BYTE *addr,BYTE bytecnt);
#endif

/* LCD関係通信IF    Su truyen tin lien quan toi LCD*/
EXT BYTE IO_lcd_ccb( BYTE  );
EXT   BYTE IO_lcd_send( BYTE *tx_data, BYTE tx_cnt );
EXT   BYTE IO_lcd_receive(BYTE *rx_data);


EXT void  Io_sfr_initial_set(void);
EXT void  Io_port_initial_set( void );

EXT   void   Io_sfr_holdx_in( void );
EXT   void   Io_port_holdx_in( void );
EXT   void   Io_int_holdx_in( void );

EXT   void   Io_sfr_holdx_out( void );
EXT   void   Io_port_holdx_out( void );
EXT   void   Io_int_holdx_out( void );

EXT   void   Io_sfr_accoff_in( void );
EXT   void   Io_port_accoff_in( void );
EXT   void   Io_int_accoff_in( void );

EXT   void   Io_sfr_accoff_out( void );
EXT   void   Io_port_accoff_out( void );
EXT   void   Io_int_accoff_out( void );

/* BEEP関連IF     Lien quan toi BEEP*/
EXT   void Io_beep_start( void );
EXT   void Io_beep_stop( void );
EXT   void Io_beep_test_8Hz_start( void );

EXT void Io_alarmkey_enable( void );
EXT void Io_int_disable_clktim(void);
EXT void Io_int_disable_almkey(void);
EXT void Io_int_disable_accdet(void);

EXT WORD Io_mincnt_read( void );
EXT void Io_mincnt_write( WORD min );
EXT   BOOL   IO_In_alarm_key();
EXT   BOOL   IO_In_acc_det(void);
EXT   BOOL   IO_In_ill_det(void);
EXT   void   Io_wdt_clear(void);

/* UART通信ﾎﾞｰﾚｰﾄ定義  Define toc do truyen tin (Baud rate) UART */
#define      CIO_UART_57600      0x33
#define      CIO_UART_19200      0x9c
//#define      CIO_UART_115200      0x19
#define      CIO_UART_115200      0x1A


#endif
