/******************************************************************************
   �N�����I���a����                    2013�N AMFM׼޵�ذ��
		��ۼު�Ė�	: '12�N AMFM׼޵�J��
		̧�ٖ�		: IO_ext.h
        Ӽޭ�ٖ�	: I/O
		�@�\		: PORT��` �O�����J�p
------------------------------------------------------------------------------
	�C�������y�N�����z�yVer�z�y���O�z �y�����z
------------------------------------------------------------------------------
[yamamoto20120727] �V�K�쐬
******************************************************************************/
#ifndef	_IO_EXT_H_
#define	_IO_EXT_H_

#include	"../model.h"

#undef	EXT
#ifdef	_IO_SFR_C_
#define	EXT
#else
#define	EXT	extern
#endif


EXT	BYTE	IO_Uart2_RXData;
/*----------------------------------------------------
  	�o�̓|�[�g�ݒ� �}�N����`
         ����(value)�� HIGH or LOW ��ݒ肷�邱��
----------------------------------------------------*/

#if 0	/* �o�X�t�@�C�g�΍� */
/* Tuner�����߰� */
#define		MIO_OUT_pTU_CLK(value)		MP_X( pTU_SCL_LAT, (value))
#define		MIO_OUT_pTU_DATA(value)		MP_X( pTU_SDA_LAT, (value))
#define 	MIO_IN_pTU_DATA()			(pTU_SDA_IN	)
#define 	MIO_CHG_pTU_DATA(value)		MP_X( pTU_SDA_DDR , (value))

/* E-VOL����|�[�g */
#define		MIO_OUT_pEVOL_CLK(value)	MP_X( pEVOL_CLK_LAT, (value))
#define		MIO_OUT_pEVOL_DATA(value)	MP_X( pEVOL_DATA_LAT, (value))
#define 	MIO_IN_pEVOL_DATA()			(pEVOL_DATA_IN	)
#define 	MIO_CHG_pEVOL_DATA(value)	MP_X( pEVOL_DATA_DDR , (value))

#endif

/* �d���֌W�|�[�g */
#if _POWER_DIAG_
#define 	MIO_IN_pDIAG()				( pALARM_IN )
#else
#define 	MIO_OUT_pALARM(value)		MP_X( pALARM_LAT, (value))
#endif /* _POWER_DIAG_ */
#define 	MIO_OUT_pSTBY(value)		MP_X( pSTBY_LAT, (value))
#define 	MIO_OUT_pSYS_ON(value)		MP_X( pSYS_ON_LAT, (value))

#define 	MIO_CHG_pILL_DET(value)		MP_X( pILL_DET_DDR, (value))
#define 	MIO_IN_pILL_DET()			(pILL_DET_IN	)	/* ILLMI ON���͊Ď��[�q				*/
#define 	MIO_IN_pACC_DET()			(pACC_DET_IN	)
#define 	MIO_IN_pALARM_KEY()			(pALARM_KEY_IN	)

/* E-VOL�֌W�|�[�g */
#define 	MIO_OUT_pSYS_MUTE(value)	MP_X( pMUTE_LAT , (value))
#define		MIO_OUT_pBEEP(value)		MP_X( pBEEP_LAT, (value))

/* EEPROM�֌W�߰� */
#define		MIO_OUT_pE2P_SCL(value) 	MP_X( pE2P_SCL_LAT, (value))
#define		MIO_OUT_pE2P_SDA(value) 	MP_X( pE2P_SDA_LAT, (value))
#define 	MIO_IN_pE2P_SDA()    		(pE2P_SDA_IN	)
#define 	MIO_CHG_pE2P_SDA(value)		MP_X( pE2P_SDA_DDR , (value))
#define 	MIO_CHG_pE2P_SCL(value)		MP_X( pE2P_SCL_DDR , (value))

/* LCD��ײ�ސ��� */
#define		MIO_OUT_pLCDCE(value )			MP_X( pLCD_CE_LAT , (value))
#define		MIO_OUT_pLCDSO(value )			MP_X( pLCD_SIO_LAT , (value))
#define		MIO_OUT_pLCDCLK(value )			MP_X( pLCD_CL_LAT , (value))
#define		MIO_IN_pLCDDI()					( pLCD_DI_IN )

/*  */
enum
{
	CIO_INIT,
//	CIO_NORMAL,
//	CIO_HALT,
//	CIO_STOP,
	CIO_HOLDX,
	CIO_ACCOFF,
	CIO_POWOFF,
	CIO_POWON
};


/*----------------------------------------------------
  	�֐���������
----------------------------------------------------*/
/* TESTMODE�֌W�ʐMIF */
EXT	void IO_Test_SetUartPort(BYTE Status);
EXT	void IO_Test_TxStart(BYTE tx_data);
EXT	void IO_Test_TxNext(BYTE tx_data);
EXT	void IO_Test_TxEnd(void);
EXT	void IO_Test_TxStop(void);
EXT	void IO_Test_RxStart(void);
EXT	BYTE IO_Test_Get_RxData(void);
EXT	void IO_Test_RxStop(void);

EXT	void  IO_SetIICPort(BYTE status);

/* TUNER�֌W�ʐMIF */
EXT	void IO_Tuner_Receive(BYTE *,BYTE  );
EXT	BYTE IO_Tuner_Send(BYTE *, BYTE );
EXT	void IO_Tuner_Send_Start(void);
EXT	void IO_Tuner_Send_Stop(void);

/* E-VOL�֌W�ʐMIF */
#if 0
EXT	void  IO_EVOL_SetIICPort(BYTE );
#endif
EXT	BYTE IO_EVOL_Send(BYTE *, BYTE );

#if _MODEL_EEPROM_
EXT void IO_EEP_Port_Reset(void);
EXT void IO_EEP_SendStart( void );
EXT	void IO_EEP_SendRestart( void );
EXT void IO_EEP_SendStop( void );

EXT BYTE IO_EEP_WriteByte(BYTE	data);
EXT void IO_EEP_ReadData(BYTE *addr,BYTE bytecnt);
#endif

/* LCD�֌W�ʐMIF */
EXT BYTE IO_lcd_ccb( BYTE  );
EXT	BYTE IO_lcd_send( BYTE *tx_data, BYTE tx_cnt );
EXT	BYTE IO_lcd_receive(BYTE *rx_data);


EXT void  Io_sfr_initial_set(void);
EXT void  Io_port_initial_set( void );

EXT	void	Io_sfr_holdx_in( void );
EXT	void	Io_port_holdx_in( void );
EXT	void	Io_int_holdx_in( void );

EXT	void	Io_sfr_holdx_out( void );
EXT	void	Io_port_holdx_out( void );
EXT	void	Io_int_holdx_out( void );

EXT	void	Io_sfr_accoff_in( void );
EXT	void	Io_port_accoff_in( void );
EXT	void	Io_int_accoff_in( void );

EXT	void	Io_sfr_accoff_out( void );
EXT	void	Io_port_accoff_out( void );
EXT	void	Io_int_accoff_out( void );

/* BEEP�֘AIF */
EXT	void Io_beep_start( void );
EXT	void Io_beep_stop( void );
EXT	void Io_beep_test_8Hz_start( void );

EXT void Io_alarmkey_enable( void );
EXT void Io_int_disable_clktim(void);
EXT void Io_int_disable_almkey(void);
EXT void Io_int_disable_accdet(void);

EXT WORD Io_mincnt_read( void );
EXT void Io_mincnt_write( WORD min );
EXT	BOOL	IO_In_alarm_key();
EXT	BOOL	IO_In_acc_det(void);
EXT	BOOL	IO_In_ill_det(void);
EXT	void	Io_wdt_clear(void);

/* UART�ʐM�ްڰĒ�` */
#define		CIO_UART_57600		0x33
#define		CIO_UART_19200		0x9c
//#define		CIO_UART_115200		0x19
#define		CIO_UART_115200		0x1A


#endif
