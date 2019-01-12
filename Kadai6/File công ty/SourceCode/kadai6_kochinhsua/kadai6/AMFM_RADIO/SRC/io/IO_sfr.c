/******************************************************************************
   クラリオン殿向け                    2007  C3X03ｼﾘｰｽﾞ[仮]
		ﾌﾟﾛｼﾞｪｸﾄ名	: '07年ﾌﾘｯﾌﾟﾀﾞｳﾝ MP3ﾓﾃﾞﾙ開発
		ﾌｧｲﾙ名		: IO_sfr.c
        ﾓｼﾞｭｰﾙ名	: I/O
		機能			: LC87ｼﾘｰｽﾞ ﾚｼﾞｽﾀ定義
------------------------------------------------------------------------------
	修正履歴【年月日】【Ver】【名前】 【説明】
------------------------------------------------------------------------------
            2005.11.29  0.00   櫻田    新規作成
******************************************************************************/
#define	_IO_SFR_C_

#include 	<stdio.h>
#include 	<string.h>
#include	"../model.h"

#define		DEFINE_SFRBITS
#include <lc884200.h>

#include	"../comm/common.h"
#include	"../power/PW_inc.h"
#include	"../power/pw_main.h"
#include	"../submode/SBM_inc.h"
#include	"../main/ma_ext.h"

#include	"IO_def.h"
#include	"IO_ext.h"

#define		_CLK_DEBUG_		0

#define		CIO_IIC_ACK		0
#define		CIO_IIC_NACK	1

#define		RAM_ADDRESS		0x0300

#if 0
#define		BHOT		39
#define		BCOLD		31
#else
#define		BHOT		38
#define		BCOLD		32
#endif

#define		COMPENSATE_CYCLE	(1UL << 20)
#define		BIT_ADJUST		(1000000000UL>>20)
#define		DIF_TEMPERTURE		65

/*----------------------------------------------------
    LC87 SFR 定義
----------------------------------------------------*/
static TCOM_FLG	fIO_flags1;
#define	fio_adc_active	(fIO_flags1.bit.bit7) /* AD変換中フラグ 1：変換中 */
#define	fIO_ill			(fIO_flags1.bit.bit6)
#define	fIO_acc			(fIO_flags1.bit.bit5)
#define	fIO_alarm		(fIO_flags1.bit.bit4)	/* BATT割り込みレベル切り替え禁止ﾌﾗｸﾞ */
static BYTE	bIO_INTStatus;
static BYTE io_sio_buf[32] ;//= (BYTE*)RAM_ADDRESS;		/* SIO送受信ﾊﾞｯﾌｧ  */
/*----------------------------------------------------
    時計補正に関する定義
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
    ローカル関数
----------------------------------------------------*/
static BYTE	IO_Tuner_Send_Byte(BYTE );
static BYTE	IO_Tuner_Receive_Byte(BYTE);

static void	IO_EVOL_Send_Start(void);
static void	IO_EVOL_Send_Stop(void);
static BYTE	IO_EVOL_Send_Byte(BYTE );

static BYTE	IO_EEP_ReadByte(BYTE	ack);

#if _TEST_MODE_
/******************************************************************************
	関  数 名	: IO_Test_xxxx
	機能概略	: UART2通信インターフェース
-------------------------------------------------------------------------------
[chin20120903] 新規作成
********************************************************************************/
void IO_Test_SetUartPort(BYTE Status)
{
	if (Status == ON)
	{
		pUART_TX_FSA = 1;					/*ACCON時UART出力設定*/
		__byte_U2CNT0 = BIN_00001001;
	}
	else
	{
		__byte_U2CNT0 = BIN_00000000;
		pUART_TX_FSA = 0;					/*ACCOFF時LOW出力設定*/
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

#if 1	/* chin20121009 バスファイト対策 */
/******************************************************************************
	関   数  名	: IO_SetIICPort
	引       数	: status ⇒ IICポートON/OFF設定
	戻   り  値	: 無し
	機 能 概 略	: IICポートの設定処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
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
	else		/* LOW出力 */
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
	関   数  名	: IO_Tuner_Receive
	引       数	: *addr ⇒ 受信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	         	: bytecnt⇒ 受信ﾃﾞｰﾀ長
	戻   り  値	: 無し
	機 能 概 略	: Tunerﾃﾞｰﾀ受信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
void  IO_Tuner_Receive(BYTE *addr,BYTE  bytecnt)
{
	BYTE	cnt;
	
	for (cnt = 0; cnt < (bytecnt - 1); cnt++)
	{
		*addr++ = IO_Tuner_Receive_Byte(CIO_IIC_ACK);
	}
	*addr = IO_Tuner_Receive_Byte(CIO_IIC_NACK);
}

/******************************************************************************
	関   数  名	: IO_Tuner_Send
	引       数	: *addr ⇒ 送信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	         	: bytecnt⇒ 送信ﾃﾞｰﾀ長
	戻   り  値	: OK ⇒ 送信正常終了
				: NG ⇒ ACKｴﾗｰ発生
	機 能 概 略	: Tunerﾃﾞｰﾀ送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
BYTE  IO_Tuner_Send(BYTE *addr, BYTE bytecnt)
{
	BYTE	cnt;
	BYTE	data;
	BYTE	ret;
	
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
	関   数  名	: IO_Tuner_Send_Start
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: スタート送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
void	IO_Tuner_Send_Start(void)
{
	MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	Cm_wait_loop(2);
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
}

/******************************************************************************
	関   数  名	: IO_Tuner_Send_Stop
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ストップ送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
void	IO_Tuner_Send_Stop(void)
{
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	Cm_wait_loop(1);
	MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	Cm_wait_loop(2);
	MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
	Cm_wait_loop(1);
	MIO_CHG_pE2P_SDA( IN );				/* DATA→HIGH */
}

/******************************************************************************
	関   数  名	: IO_Tuner_Send_Byte
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: 1BYTE送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
static BYTE	IO_Tuner_Send_Byte(BYTE data)
{
	BYTE	count;
	BYTE	ack;
	
	for(count = 0 ; count < 8; count++)
	{
		MIO_CHG_pE2P_SCL( OUT );		/* CLK→LOW */
		Cm_wait_loop(1);
		if ((data & 0x80) == 0x80)
		{
			MIO_CHG_pE2P_SDA( IN );		/* DATA→HIGH */
		}
		else
		{
			MIO_CHG_pE2P_SDA( OUT );	/* DATA→LOW */
		}
		Cm_wait_loop(0);
		MIO_CHG_pE2P_SCL( IN );			/* CLK→HIGH */
		Cm_wait_loop(3);
		data <<= 1;
	}

	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	Cm_wait_loop(3);
	MIO_CHG_pE2P_SDA( IN );				/* DATA→HIGH */
	MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
	Cm_wait_loop(2);
	ack = MIO_IN_pE2P_SDA();			/*	ACK取得	*/
	
	Cm_wait_loop(0);
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	
	return(ack);
}

/******************************************************************************
	関   数  名	: IO_Tuner_Receive_Byte
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: 1BYTE送受信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
static BYTE	IO_Tuner_Receive_Byte(BYTE	 ack)
{
	BYTE	count;
	BYTE	data;

	data = 0;
	for(count = 0; count < 8; count++)
	{
		MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
		data <<= 1;
		Cm_wait_loop(3);
		MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
		if (MIO_IN_pE2P_SDA() == HIGH)		/*1ビットシフト受信*/
		{
			data |= 0x01;
		}
		else
		{
			data &= 0xfe;
		}
		Cm_wait_loop(2);
	}
	
	MIO_CHG_pE2P_SCL( OUT );				/* CLK→LOW */
	
	Cm_wait_loop(1);
	if (ack == CIO_IIC_ACK)
	{
		MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	}
	else
	{
		MIO_CHG_pE2P_SDA( IN );				/* DATA→HIGH */
	}
	Cm_wait_loop(0);
	MIO_CHG_pE2P_SCL( IN );					/* CLK→HIGH */

	Cm_wait_loop(4);
	MIO_CHG_pE2P_SCL( OUT );				/* CLK→LOW */
	
	Cm_wait_loop(0);
	MIO_CHG_pE2P_SDA( IN );					/* DATA→HIGH */
	return(data);
}

/******************************************************************************
	関   数  名	: IO_EVOL_Send
	引       数	: *addr ⇒ 送信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	         	: bytecnt⇒ 送信ﾃﾞｰﾀ長
	戻   り  値	: OK ⇒ 送信正常終了
				: NG ⇒ ACKｴﾗｰ発生
	機 能 概 略	: EVOLﾃﾞｰﾀ送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
BYTE  IO_EVOL_Send(BYTE *addr, BYTE bytecnt)
{
	BYTE	cnt;
	BYTE	data;
	BYTE	ret;
	
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
	関   数  名	: IO_EVOL_Send_Start
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: スタート送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
static void	IO_EVOL_Send_Start(void)
{
	MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	Cm_wait_loop(2);
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
}

/******************************************************************************
	関   数  名	: IO_EVOL_Send_Stop
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ストップ送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
static void	IO_EVOL_Send_Stop(void)
{
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	Cm_wait_loop(1);
	MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	Cm_wait_loop(2);
	MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
	Cm_wait_loop(1);
	MIO_CHG_pE2P_SDA( IN );				/* DATA→HIGH */
}

/******************************************************************************
	関   数  名	: IO_EVOL_Send_Byte
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: 1BYTE送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
static BYTE	IO_EVOL_Send_Byte(BYTE data)
{
	BYTE	count;
	BYTE	ack;
	
	for(count = 0 ; count < 8; count++)
	{
		MIO_CHG_pE2P_SCL( OUT );		/* CLK→LOW */
		Cm_wait_loop(1);
		if ((data & 0x80) == 0x80)
		{
			MIO_CHG_pE2P_SDA( IN );		/* DATA→HIGH */
		}
		else
		{
			MIO_CHG_pE2P_SDA( OUT );	/* DATA→LOW */
		}
		Cm_wait_loop(0);
		MIO_CHG_pE2P_SCL( IN );			/* CLK→HIGH */
		Cm_wait_loop(3);
		data <<= 1;
	}

	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	Cm_wait_loop(3);
	MIO_CHG_pE2P_SDA( IN );				/* DATA→HIGH */
	MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
	Cm_wait_loop(2);
	ack = MIO_IN_pE2P_SDA();			/*	ACK取得	*/
	
	Cm_wait_loop(0);
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	
	return(ack);
}

#if _MODEL_EEPROM_
void IO_EEP_Port_Reset(void)
{
	/* DATA (0xaa) */
	static BYTE	ack;
	static BYTE	ret;
	BYTE	count;
	
	IO_EEP_SendStart();
	
	IO_EEP_WriteByte(0xFF);
	
	IO_EEP_SendRestart();
	
	IO_EEP_SendStop();
}

/******************************************************************************
	関   数  名	: IO_EEP_SendStart
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: スタート送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
void IO_EEP_SendStart( void )
{
	MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	Cm_wait_loop(1);
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
}

/******************************************************************************
	関   数  名	: IO_EEP_SendRestart
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: リスタート送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
void IO_EEP_SendRestart( void )
{
	Cm_wait_loop(2);
	MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
	Cm_wait_loop(3);
	MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	Cm_wait_loop(1);
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
}

/******************************************************************************
	関   数  名	: IO_EEP_SendStop
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: ストップ送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
void IO_EEP_SendStop(void)
{
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	Cm_wait_loop(1);
	MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
	Cm_wait_loop(1);
	MIO_CHG_pE2P_SDA( IN );				/* DATA→HIGH */
}

/******************************************************************************
	関   数  名	: IO_EEP_WriteByte
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: 1BYTE送信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
BYTE	IO_EEP_WriteByte(BYTE	data)
{
	BYTE	count;
	BYTE	ack;
	
	Cm_wait_loop(2);

	for(count = 0 ; count < 8; count++)
	{
		MIO_CHG_pE2P_SCL( OUT );		/* CLK→LOW */
		Cm_wait_loop(1);
		if ((data & 0x80) == 0x80)
		{
			MIO_CHG_pE2P_SDA( IN );		/* DATA→HIGH */
		}
		else
		{
			MIO_CHG_pE2P_SDA( OUT );	/* DATA→LOW */
		}
		Cm_wait_loop(0);
		MIO_CHG_pE2P_SCL( IN );			/* CLK→HIGH */
		Cm_wait_loop(3);
		data <<= 1;
	}

	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	Cm_wait_loop(3);
	MIO_CHG_pE2P_SDA( IN );				/* DATA→HIGH */
	MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
	Cm_wait_loop(2);
	ack = MIO_IN_pE2P_SDA();			/*	ACK取得	*/
	
	Cm_wait_loop(0);
	MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
	
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
	関   数  名	: IO_EEP_ReadByte
	引       数	: 無し
	戻   り  値	: 無し
	機 能 概 略	: 1BYTE送受信処理
-------------------------------------------------------------------------------
[chin20121009] 新規作成
********************************************************************************/
static BYTE	IO_EEP_ReadByte(BYTE	ack)
{
	BYTE	count;
	BYTE	data;

	data = 0;
	for(count = 0; count < 8; count++)
	{
		MIO_CHG_pE2P_SCL( OUT );			/* CLK→LOW */
		data <<= 1;
		Cm_wait_loop(3);
		MIO_CHG_pE2P_SCL( IN );				/* CLK→HIGH */
		if (MIO_IN_pE2P_SDA() == HIGH)		/*1ビットシフト受信*/
		{
			data |= 0x01;
		}
		else
		{
			data &= 0xfe;
		}
		Cm_wait_loop(2);
	}
	
	MIO_CHG_pE2P_SCL( OUT );				/* CLK→LOW */
	
	Cm_wait_loop(1);
	if (ack == CIO_IIC_ACK)
	{
		MIO_CHG_pE2P_SDA( OUT );			/* DATA→LOW */
	}
	else
	{
		MIO_CHG_pE2P_SDA( IN );				/* DATA→HIGH */
	}
	Cm_wait_loop(0);
	MIO_CHG_pE2P_SCL( IN );					/* CLK→HIGH */

	Cm_wait_loop(4);
	MIO_CHG_pE2P_SCL( OUT );				/* CLK→LOW */
	
	Cm_wait_loop(0);
	MIO_CHG_pE2P_SDA( IN );					/* DATA→HIGH */
	return(data);
}

/*****************************************************************************************
	関数名		：void IO_EEP_ReadData(BYTE *addr,BYTE  bytecnt)
	引数		： *addr ⇒ 受信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
				： bytecnt⇒ 受信ﾃﾞｰﾀ長
	リターン	：なし
	機能概要	：EEPROMﾃﾞｰﾀ受信処理
-----------------------------------------------------------------------------------------
[chin20121009] 新規作成
*****************************************************************************************/
void IO_EEP_ReadData(BYTE *addr,BYTE bytecnt)
{
	WORD	iCnt;
	
	for (iCnt = 0; iCnt < (bytecnt - 1); iCnt++)
	{
		*addr++ = IO_EEP_ReadByte(CIO_IIC_ACK);
	}
	*addr = IO_EEP_ReadByte(CIO_IIC_NACK);
}
#endif

#else	/* バスファイト対策 */
/******************************************************************************
	関   数  名	: IO_Tuner_SetIICPort
	引       数	: status： ON ⇒ 通信Portに設定
				:          OFF ⇒ 汎用ﾎﾟｰﾄに設定
	戻   り  値	: 無し
	機 能 概 略	: Tuner通信ﾎﾟｰﾄ切替処理
    備       考 : 通信Portに設定すると、CLKとDATA両方出力のHIGHに設定される
-------------------------------------------------------------------------------
[sun100420] 新規作成
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
	関   数  名	: IO_Tuner_Receive
	引       数	: *addr ⇒ 受信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	         	: bytecnt⇒ 受信ﾃﾞｰﾀ長
	戻   り  値	: 無し
	機 能 概 略	: Tunerﾃﾞｰﾀ受信処理
-------------------------------------------------------------------------------
[sun100420] 新規作成
********************************************************************************/
void  IO_Tuner_Receive(BYTE *addr,BYTE  bytecnt)
{
	BYTE	cnt;
	
//	pTU_SDA_FSA = 0;
//	pTU_SCL_FSA = 0;

	for (cnt = 0; cnt < (bytecnt - 1); cnt++)
	{
		*addr++ = IO_Tuner_Receive_Byte(CIO_IIC_ACK);
	}
	*addr = IO_Tuner_Receive_Byte(CIO_IIC_NACK);
}

/******************************************************************************
	関   数  名	: IO_Tuner_Send
	引       数	: *addr ⇒ 送信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	         	: bytecnt⇒ 送信ﾃﾞｰﾀ長
	戻   り  値	: OK ⇒ 送信正常終了
				: NG ⇒ ACKｴﾗｰ発生
	機 能 概 略	: Tunerﾃﾞｰﾀ送信処理
-------------------------------------------------------------------------------
[20120730yamamoto] 新規作成
********************************************************************************/
BYTE  IO_Tuner_Send(BYTE *addr, BYTE bytecnt)
{
	BYTE	cnt;
	BYTE	data;
	BYTE	ret;
	



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

/* [sun100505] 通信速度調整　250kHzぐらい */
/* [sun100817] 通信速度調整　100kHzぐらい */
void	IO_Tuner_Send_Start(void)
{
	MIO_OUT_pTU_DATA(HIGH);
	MIO_OUT_pTU_CLK(HIGH);
	MIO_CHG_pTU_DATA(OUT);
	Cm_wait_loop(4);
	MIO_OUT_pTU_DATA(LOW);
	Cm_wait_loop(2);
	MIO_OUT_pTU_CLK(LOW);
}

/* [sun100505] 通信速度調整　250kHzぐらい */
/* [sun100817] 通信速度調整　100kHzぐらい */
void	IO_Tuner_Send_Stop(void)
{
	MIO_OUT_pTU_CLK(LOW);
	MIO_CHG_pTU_DATA(OUT);
	MIO_OUT_pTU_DATA(LOW);
	Cm_wait_loop(2);
	MIO_OUT_pTU_CLK(HIGH);
	Cm_wait_loop(4);
	MIO_OUT_pTU_DATA(HIGH);
}

/* [sun100505] 通信速度調整　250kHzぐらい */
/* [sun100817] 通信速度調整　100kHzぐらい */
static BYTE	IO_Tuner_Send_Byte(BYTE data)
{
	BYTE	bitcnt;
	BYTE	ack;
	
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

/* [sun100505] 通信速度調整　250kHzぐらい */
/* [sun100817] 通信速度調整　100kHzぐらい */
static BYTE	IO_Tuner_Receive_Byte(BYTE	 ack)
{
	BYTE	bitcnt;
	BYTE	data;

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
	関   数  名	: IO_EVOL_Send
	引       数	: *addr ⇒ 送信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	         	: bytecnt⇒ 送信ﾃﾞｰﾀ長
	戻   り  値	: 無し
	機 能 概 略	: EVOLﾃﾞｰﾀ送信処理
    備       考 : 
-------------------------------------------------------------------------------
修正履歴
	番号	日付       担当者		修正内容
-------------------------------------------------------------------------------
[sun100420] 新規作成
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
/* [20120731yamamoto] 通信速度調整 100kHzぐらい */
BYTE  IO_EVOL_Send(BYTE *addr, BYTE bytecnt)
{
	BYTE	cnt;
	BYTE	data;
	BYTE	ret;
	
//	pEVOL_DATA_FSA = 0;
//	pEVOL_CLK_FSA = 0;
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
/* [20120731yamamoto] 通信速度調整 100kHzぐらい */
static void	IO_EVOL_Send_Start(void)
{
	MIO_OUT_pEVOL_DATA(HIGH);
	MIO_OUT_pEVOL_CLK(HIGH);
	MIO_CHG_pEVOL_DATA(OUT);
	Cm_wait_loop(4);
	MIO_OUT_pEVOL_DATA(LOW);
	Cm_wait_loop(2);
	MIO_OUT_pEVOL_CLK(LOW);
}

/* [20120731yamamoto] 通信速度調整 100kHzぐらい */
static void	IO_EVOL_Send_Stop(void)
{
	MIO_OUT_pEVOL_CLK(LOW);
	MIO_CHG_pEVOL_DATA(OUT);
	MIO_OUT_pEVOL_DATA(LOW);
	Cm_wait_loop(2);
	MIO_OUT_pEVOL_CLK(HIGH);
	Cm_wait_loop(4);
	MIO_OUT_pEVOL_DATA(HIGH);
}

/* [20120731yamamoto] 通信速度調整 100kHzぐらい */
static BYTE	IO_EVOL_Send_Byte(BYTE data)
{
	BYTE	bitcnt;
	BYTE	ack;
	
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
	関数名		：void IO_EEP_SendStart( void )
	引数		：なし
	リターン	：なし
	機能概要	：EEPROMﾄﾞﾗｲﾊﾞMain処理
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
*****************************************************************************************/
void IO_EEP_SendStart( void )
{
	MIO_OUT_pE2P_SDA( HIGH );			/*	EEPROMデータポートにHIGH出力	*/
	MIO_OUT_pE2P_SCL( HIGH );			/*	EEPROMクロックポートにHIGH出力	*/
	MIO_CHG_pE2P_SDA( OUT );			/*	EEPROMデータポート出力ポートに変換	*/
	//Cm_wait_loop(3);
	MIO_OUT_pE2P_SDA( LOW );			/*	EEPROMデータポートにLOW出力		*/
	Cm_wait_loop(1);
	MIO_OUT_pE2P_SCL( LOW );			/*	EEPROMクロックポートにHIGH出力	*/
}
/*****************************************************************************************
	関数名		：void IO_EEP_SendRestart( void )
	引数		：なし
	リターン	：なし
	機能概要	：EEPROMﾄﾞﾗｲﾊﾞMain処理
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
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
	関数名		：void IO_EEP_SendStop(void)
	引数		：なし
	リターン	：なし
	機能概要	：EEPROMﾄﾞﾗｲﾊﾞMain処理
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
*****************************************************************************************/
void IO_EEP_SendStop(void)
{
	MIO_OUT_pE2P_SCL( LOW );			/*	EEPROMクロックポートにHIGH出力	*/
	MIO_CHG_pE2P_SDA( OUT );		/*	EEPROMデータポート出力ポートに変換	*/
	MIO_OUT_pE2P_SDA( LOW );			/*	EEPROMデータポートにHIGH出力	*/
	Cm_wait_loop(1);
	MIO_OUT_pE2P_SCL( HIGH );			/*	EEPROMクロックポートにHIGH出力	*/
	Cm_wait_loop(1);
	MIO_OUT_pE2P_SDA( HIGH );			/*	EEPROMデータポートにLOW出力		*/
}
/*****************************************************************************************
	関数名		：BYTE	IO_EEP_SendByteData(BYTE	data)
	引数		：なし
	リターン	：なし
	機能概要	：EEPROMﾄﾞﾗｲﾊﾞMain処理
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
*****************************************************************************************/
BYTE	IO_EEP_WriteByte(BYTE	data)
{
	BYTE	count;
	BYTE	ack;
	
	//MIO_OUT_pE2P_SCL( LOW );		/*EEPROMクロックポートにLOW出力*/
	MIO_CHG_pE2P_SDA( OUT );		/*	EEPROMデータポート出力ポートに変換	*/
	Cm_wait_loop(2);

	for(count = 0 ; count < 8; count++)
	{
		MIO_OUT_pE2P_SCL( LOW );		/*EEPROMクロックポートにLOW出力*/
		if ((data & 0x80) == 0x80)
		{
			MIO_OUT_pE2P_SDA(HIGH);		/*1ビットシフトで送信*/
		}
		else
		{
			MIO_OUT_pE2P_SDA(LOW);	/*1ビットシフトで送信*/
		}
		Cm_wait_loop(1);
		MIO_OUT_pE2P_SCL( HIGH );		/*EEPROMクロックポートにHIGH出力*/
		Cm_wait_loop(1);
		data <<= 1;
	}

	MIO_OUT_pE2P_SCL( LOW );		/*	EEPROMクッロクポートにLOW出力		*/
	MIO_OUT_pE2P_SDA(LOW);	/*1ビットシフトで送信*/
	MIO_CHG_pE2P_SDA( IN );			/*	EEPROMデータポート入力ポートに変換	*/
	Cm_wait_loop(0);
	MIO_OUT_pE2P_SCL( HIGH );		/*	EEPROMクッロクポートにHIGH出力		*/
	ack = MIO_IN_pE2P_SDA();			/*	ACK取得								*/
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
	関数名		：BYTE	IO_EEP_ReadByte(BYTE	ack)
	引数		：なし
	リターン	：なし
	機能概要	：EEPROMﾄﾞﾗｲﾊﾞMain処理
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
*****************************************************************************************/
BYTE	IO_EEP_ReadByte(BYTE	ack)
{
	BYTE	count;
	BYTE	data;

	MIO_OUT_pE2P_SCL( LOW );		/*EEPROMクロックポートにLOW出力*/
	MIO_CHG_pE2P_SDA(IN);				/*EEPROMデータポート入力ポートに変換*/

	data = 0;
	for(count = 0; count < 8; count++)
	{
		MIO_OUT_pE2P_SCL( LOW );		/*EEPROMクロックポートにLOW出力*/
		data <<= 1;
		Cm_wait_loop(1);
		MIO_OUT_pE2P_SCL( HIGH );		/*EEPROMクロックポートにHIGH出力*/
		if (MIO_IN_pE2P_SDA() == HIGH)		/*1ビットシフト受信*/
		{
			data |= 0x01;
		}
		else
		{
			data &= 0xfe;
		}
		Cm_wait_loop(0);
	}
	
	MIO_OUT_pE2P_SCL( LOW );		/*	EEPROMクッロクポートにLOW出力		*/
	MIO_OUT_pE2P_SDA(ack);			/*1ビットシフトで送信*/
	MIO_CHG_pE2P_SDA( OUT );		/*	EEPROMデータポート入力ポートに変換	*/
	Cm_wait_loop(1);
	MIO_OUT_pE2P_SCL( HIGH );		/*	EEPROMクッロクポートにHIGH出力		*/
	return(data);
}

/*****************************************************************************************
	関数名		：void IO_EEP_ReadData(BYTE *addr,BYTE  bytecnt)
	引数		：なし
	リターン	：なし
	機能概要	：EEPROMﾄﾞﾗｲﾊﾞMain処理
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
[sun100823] ﾐｽ修正：　bytecntをBYTE⇒WORDに変換
*****************************************************************************************/
void IO_EEP_ReadData(BYTE *addr,BYTE bytecnt)
{
	WORD	iCnt;
	
	for (iCnt = 0; iCnt < (bytecnt - 1); iCnt++)
	{
		*addr++ = IO_EEP_ReadByte(CIO_IIC_ACK);
	}
	*addr = IO_EEP_ReadByte(CIO_IIC_NACK);
}


#endif

#endif	/* バスファイト対策 */

/******************************************************************************
	関   数  名	: IO_lcd_ccb
	引       数	: adr ⇒ 1byte送信ﾃﾞｰﾀ(CCBｱﾄﾞﾚｽ用)
	戻   り  値	: OK    ⇒ 処理完了
	        	: NG    ⇒ 処理失敗
	機 能 概 略	: LCDドライバへ１バイト ﾃﾞｰﾀを送信します。
    備       考 : LSB先頭で送信
-------------------------------------------------------------------------------
修正履歴
	番号	日付       担当者		修正内容
-------------------------------------------------------------------------------
  【000】	12/08/03   ha			新規作成
********************************************************************************/
BYTE IO_lcd_ccb( BYTE adr )
{
	BYTE	roop_cnt;
	
	
	#if 0
	/* port設定 
		P10: SIO0データ（CMOS)	
		P11: オープン			
		P12: SIO0クロック(CMOS)	*/
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
	/* ボーレートクロック設定 */
	__byte_S0BG = 0x0D;		/* 850kHz */
	#endif
	
	__EnableInterrupt(0);

	__bf_IL1H.IRQ6  = 0x03;
	
	__byte_S0CNT    = 0x01;		/* 送信モード 	LSB	  */
	__byte_S0INTVL  = 0x00;
	
	__byte_S0BUF     = adr;

	__bf_S0CNT.RUN = 1;		/* 送信開始			　 */
	
	for ( roop_cnt=0; roop_cnt< 0xff ; roop_cnt++ )
	{
		if( __bf_S0CNT.FLG == ON )
		{
			__bf_S0CNT.FLG = OFF ;		/* 送信完了までﾙｰﾌﾟ */
			return OK ;
		}
	}

	return NG ;			/* time out */
}
/******************************************************************************
	関   数  名	: Io_sio_lcd_send
	引       数	: *tx_data ⇒ 送信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	         	: tx_cnt 	⇒ 送信ﾃﾞｰﾀByte数（１～３２）
	戻   り  値	: OK    ⇒ 処理完了
	        	: NG    ⇒ 処理失敗
	機 能 概 略	: LCDドライバへ指定バイト数ﾃﾞｰﾀを送信します。
    備       考 : 送信ﾃﾞｰﾀByte数は、Max32byte。LSB先頭で送信
-------------------------------------------------------------------------------
修正履歴
	番号	日付       担当者		修正内容
-------------------------------------------------------------------------------
  【000】	12/08/03   ha			新規作成
********************************************************************************/
BYTE IO_lcd_send( BYTE *tx_data, BYTE tx_cnt )
{
	BYTE	roop_cnt;
	BYTE	i;
	
	i = 0;
	#if 0
	/* port設定 
		P10: SIO0データ（CMOS)	
		P11: オープン			
		P12: SIO0クロック(CMOS)	*/
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

	/* ボーレートクロック設定 */
	__byte_S0BG = 0x0D;		/* 850kHz */
	#endif
	
	memcpy( io_sio_buf, tx_data, tx_cnt );	/* 送信ﾃﾞｰﾀ設定 */
	
	__EnableInterrupt(0);

	__bf_IL1H.IRQ6  = 0x03;
	
	__byte_S0CNT    = 0x01;		/* 送信モード 	LSB	  */
	__byte_S0INTVL  = 0x00;
	
	
	for(i=0;i < tx_cnt;i++)
	{
		__byte_S0BUF     = io_sio_buf[i];
	
		__bf_S0CNT.RUN = 1;		/* 送信開始			　 */
		
		for ( roop_cnt=0; roop_cnt< 0xff ; roop_cnt++ )
		{
			if( __bf_S0CNT.FLG == ON )
			{
				__bf_S0CNT.FLG = OFF ;		/* 送信完了までﾙｰﾌﾟ */
			}
		}
	}
	return OK ;			/* time out */
}

/******************************************************************************
	関   数  名	: IO_lcd_receive
	引       数	: *rx_data ⇒ 受信ﾃﾞｰﾀﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	戻   り  値	: OK    ⇒ 処理完了
	        	: NG    ⇒ 処理失敗
	機 能 概 略	: LCDドライバから４バイトﾃﾞｰﾀを受信します。
    備       考 : 戻り値がNGの場合、受信ﾊﾞｯﾌｧの内容は変化しません。
-------------------------------------------------------------------------------
修正履歴
	番号	日付       担当者		修正内容
-------------------------------------------------------------------------------
  【000】	12/08/03   ha			新規作成
********************************************************************************/
BYTE  IO_lcd_receive(BYTE *rx_data)
{
	WORD	roop_cnt;
	BYTE	i;
	if( rx_data == 0 )
	{
		return NG ;			/* ﾊﾟﾗﾒｰﾀｴﾗｰ */
	}
	
	#if 0
	/* port設定 
		P10: SIO0データ（CMOS)	
		P11: SIO0データ（CMOS）	
		P12: SIO0クロック(CMOS)	*/
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
	
	/* ボーレートクロック設定 */
	__byte_S0BG = 0x0D;		/* 850kHz */
	#endif
	
	/* 転送回数 */
	i = 0;
	__EnableInterrupt(0);
	__bf_IL1H.IRQ6  = 1;
	
	
	__byte_S0INTVL  = 0x00;		/* 32bit受信		  */
	
	__byte_S0BUF   = 0x00;
	__byte_S0CNT	= 0x41;		/* 受信モード 	LSB	  */
	
//	__bf_S0CNT.RUN = 1;		/* 受信開始			　 */
	
	for(i = 0;i< 4;i++)
	{
		__bf_S0CNT.RUN = 1;
		for ( roop_cnt=0; roop_cnt< 0x00ff; roop_cnt++ )
		{
			if( __bf_S0CNT.FLG == ON )
			{
				/* 受信完了 */
				*(rx_data + i)     = __byte_S0BUF ;
				__bf_S0CNT.FLG = OFF ;
			}
		}
	}

	return OK ;			/* time out */
}


/* 割り込み関連 */
/******************************************************************************
	関   数  名	: Io_sfr_initial_set
	引       数	: void
	戻   り  値	: void
	機 能 概 略	: ｲﾆｼｬﾙ時のSFR設定を行います｡
    備       考 :
-------------------------------------------------------------------------------
[20120730yamamoto] 新規作成
********************************************************************************/
void Io_sfr_initial_set(void)
{
	volatile DWORD	loopcnt;
	DWORD tmp;
	sWORD dif_q;
	
	/* WDT停止 */
	__byte_WDTCR = BIN_00000000;	/*
				       |||||||+--- START   1:WDT START (0の書き込みで停止できない)
				       ||||||+---- OVF     0:暴走検出時ﾘｾｯﾄ禁止
				       |||||+----- USERFLG 0:固定
				       ||||+------ PDNSTOP 0:HOLDX突入時WDTﾘｾｯﾄ 1:HOLDX突入時WDT停止
				       |||+------- SRFLG   0:ﾘｾｯﾄ実行検出ﾌﾗｸﾞ
				       ||+-------- MDSEL   0:ｲﾝﾀﾗﾌﾟﾄﾓｰﾄﾞ 1:ﾘｾｯﾄﾓｰﾄﾞ
				       |+--------- -       0:固定
				       +---------- -       0:固定  */
	
	/* PLL VCO制御ﾚｼﾞｽﾀ、固定値 */
	__byte_VCOCNT0 = 0x02;
	__byte_VCOCNT1 = 0x04;
	__byte_VCOCNT2 = 0x35;
	__byte_VCOCNT3 = 0xB6;
	__byte_VCOCNT4 = 0x00;
	
	/* 32k発振＆OSC0＆OSC1 Start */
	__byte_OCR0 = BIN_10000011;		/* 32k発振＆OSC0 Start */
	
	Io_port_initial_set();

	
#if 0	/* 内部ﾘｾｯﾄ機能使用 何も設定しない */
	__byte_TMPCNT0 |= BIN_01000000;		/* Bit6  LVDSTOP */
#endif
	
  	/* 発振安定 Total 1000ms以上 */
	for (loopcnt = 0; loopcnt < 0xAf00; loopcnt++)
	{
	}
	
#if 0	/* EEPROMのため、OSC1発振しないといけない */
	__bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;	/* ｼｽﾃﾑｸﾛｯｸ選択：RC発振(1MHz) */
	
	#if 0	/* RC発振は既に動いてる */
	__bf_OCR0.RCSTOP = 0;	/* RC発振開始 */
	#endif
#else
	__bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC1;
	__bf_OCR0.RCSTOP = __bfval_OCR0_RCSTOP_STOP;
#endif
	
	__byte_OCR1 = BIN_00000000;			/* timer8のｸﾛｯｸ選択(OSC0)/ｼｽﾃﾑｸﾛｯｸ分周無し */

	/* 割り込みレベル設定 3:X 2:H 1:L 0:禁止 */
	__byte_IL1L = BIN_00101011;			/* IRQ3:0  T0 :2  BT :2  WDT:3 */
	__byte_IL1H = BIN_11010000;			/* IRQ7:3 IRQ6:1 IRQ5:0 IRQ4:0 */
	__byte_IL2L = BIN_00000000;			/* IRQB:0 IRQA:0 IRQ9:0 IRQ8:0 */
	__byte_IL2H = BIN_01000000;			/* IRQF:1 IRQE:0 IRQD:0 IRQC:0 */

	__byte_INT01CR = BIN_00000000;		/* INT0,INT1未使用 */
	__byte_INT23CR = BIN_10000000;		/* INT2未使用，INT3割り込み設定 */
	/* INT3はAcc_Det端子の「立下り」検出するように設定 */

	#if 0		/* 精度向上 */
	__byte_BTCR = BIN_01110101;			/* T8 2048TBST(62.5ms),WDT 8192TBST */
	#else
	__byte_BTCR = BIN_01010100;			/* T8 128TBST(3.9ms),WDT 8192TBST */
	#endif
/*				      |||||||+--- IE0   0:BaseTime0割込み禁止
				      ||||||+---- FLG0  0:
				      |||||+----- IE1   0:BaseTime1割込み禁止
				      ||||+------ FLG1  0:
				      |||+------- CNT   11:BT1（2048TBST）
				      ||+-------- CNT   
				      |+--------- RUN   1:動作開始
				      +---------- FST   0:未使用  */

	/* T0 8bit BEEP用 4KHz */
	__byte_T0LR  = 0x00;					/* T0動作時:8bitPWM	 */
	__byte_T0HR  = 0x00;					/* T0動作時:停止	 */
	__byte_T0CNT = BIN_00010000;			/* T0ｶｳﾝﾄｸﾛｯｸ OSC0	 */
	__byte_T0PR  = BIN_01100000;			/* MODE3,T0PR周期 = (PR+1)×ｶｳﾝﾄｸﾛｯｸ */

	/* T1 16BIT TIMER ACCOFF時ACC_DET端子チャタ用 */
	/* (0x270F(9999)+1)×1um = 10ms */
	__byte_T1LR  = 0x0F;
	__byte_T1HR  = 0x27;
	__byte_T1PR  = BIN_00000000;
	__byte_T1CNT = BIN_00001101;
	
	/* T2 16BIT TIMER  (0x960+1)*(4+1)*83.3ns = 1ms */
	__byte_T2LR   = 0x60;
	__byte_T2HR   = 0x09;
	__byte_T2CNT0 = BIN_00000000;
	__byte_T2CNT2 = BIN_00000100;
	
	/* 日分秒ｶｳﾝﾀ */
	__byte_DMSCNT = BIN_00000000;
	__byte_SEC    = BIN_00000000;
	__byte_MINL   = BIN_00000000;
	__byte_MINH   = BIN_00000000;
	__byte_DAYL   = BIN_00000000;
	__byte_DAYH   = BIN_00000000;
	__byte_DMSCNT = BIN_10000000;
	
	#if 1
	/* 25度A/D変換 ﾃﾞｰﾀ下位 8bit格納ﾚｼﾞｽﾀ2 */
	__byte_CALC0L    = __byte_TEMPCONV0;
	/* 25度A/D変換 ﾃﾞｰﾀ下位 4bit格納ﾚｼﾞｽﾀ2 */
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
	
	/*高温側補正結果*/
	__word_CALC1 = (WORD)tmp;
	
	tmp = COMPENSATE_CYCLE * BCOLD;
	tmp = tmp / (WORD)dif_q;
	tmp = tmp * DIF_TEMPERTURE;
	tmp = tmp / (WORD)dif_q;
	tmp = tmp * DIF_TEMPERTURE;	
	tmp = tmp / BIT_ADJUST;
	
	/*低温側補正結果*/
	__word_CALC4 = (WORD)tmp;
	
	/* 補正量調整ﾚｼﾞｽﾀ(とりあえず0に設定) */
	__byte_CALC2     = BIN_00000000;
	/* A/D変換ｳｪｲﾄ時間設定ﾚｼﾞｽﾀ */
	__byte_CALC3     = BIN_00000011;
	
	#endif
	
	/* 時計ﾀｲﾏ 時計ﾀｲﾏ停止時ｶｳﾝﾄ値保持,割込み周期 500ms */
	__byte_CKTIMCTRL = BIN_10001110;

	
	/* SIO0 */
	__byte_S0CNT = BIN_00000000;
	/* ボーレートクロック設定 */
	__byte_S0BG = 0x0D;		/* 850kHz */
	__byte_S0BUF = 0x00;
	__byte_S0INTVL = BIN_00000000;
	
	
	/* UART2 */
	__byte_U2CNT0 = BIN_00000000;
	__byte_U2CNT1 = BIN_00000100;
	__byte_U2TBUF = BIN_00000000;
	__byte_U2BG   = CIO_UART_19200;		/*19200bps≒12MHz/{(U2BG+1)×4}*/
	
#if ( _WDT_ == 1 )
	/* WDT開始 */
	__byte_WDTCR = BIN_00101001;	/*
				       |||||||+--- START   1:WDT START (0の書き込みで停止できない)
				       ||||||+---- OVF     0:暴走検出時ﾘｾｯﾄ禁止
				       |||||+----- USERFLG 0:固定
				       ||||+------ PDNSTOP 0:HOLDX突入時WDTﾘｾｯﾄ 1:HOLDX突入時WDT停止
				       |||+------- SRFLG   0:ﾘｾｯﾄ実行検出ﾌﾗｸﾞ
				       ||+-------- MDSEL   0:ｲﾝﾀﾗﾌﾟﾄﾓｰﾄﾞ 1:ﾘｾｯﾄﾓｰﾄﾞ
				       |+--------- -       0:固定
				       +---------- -       0:固定  */
	/* WDT ｶｳﾝﾀｸﾘｱ */
	__SFR_BITCLR(__WDTCR,0);
#endif
}


/******************************************************************************
	関   数  名	: Io_port_initial_set
	引       数	: なし
	戻   り  値	: なし
	機 能 概 略	: 全てのﾎﾟｰﾄの状態を初期設定にします｡INITAIL時にｺｰﾙして下さい。
    備       考 : 設定値は、io_port_ext.h参照。
-------------------------------------------------------------------------------
   修正履歴【年月日】【Ver】【名前】 【説明】
-------------------------------------------------------------------------------
[sun100419] 16ﾋﾞｯﾄﾏｲｺﾝ用新規作成
*******************************************************************************/
void  Io_port_initial_set( void )
{
	pP0_LAT_BYTE = CP0_LAT_INI;
	pP0_DDR_BYTE = CP0_DDR_INI;
	pP0_FSA_BYTE = CP0_FSA_INI;
	pP0_23INT_BYTE = CP0_23INT_INI;
	
	pP1_LAT_BYTE = CP1_LAT_INI;
	pP1_DDR_BYTE = CP1_DDR_INI;
	pP1_FSA_BYTE = CP1_FSA_INI;
	pP1_FSB_BYTE = CP1_FSB_INI;

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

#if 0	/* 未使用 */
void  Io_int_initial_set( void )
{
	/* ﾍﾞｰｽﾀｲﾏ 62.5ms start */
	__bf_BTCR.IE1 = 1;
	__bf_BTCR.RUN = __bfval_BTCR_RUN_RUN;
	
	/* 時計ﾀｲﾏ割込み禁止 */
	__byte_CKTIMCTRL = BIN_10001110;
	
	/* ALARM_KEY割込み禁止 */
	#if 0
	__bf_P0LAT.BIT4  = 0;
	__bf_P0DDR.BIT4  = 0;
	#endif
	__bf_P0FSA.P04IL = 0;
	__bf_P0FSA.P04IE = 0;
	
	/* ACC_DET割込み許可 */
	__bf_INT23CR.INT3IF = 0;
	__bf_INT23CR.INT3IE = 0;
	
}
#endif

/******************************************************************************
ACC OFF(通常) → HOLDX
*******************************************************************************
	関数名	：Io_xxx_holdx_in
	引  数	：無し
	戻り値	：無し
	機  能	：HOLDX IN 時のﾚｼﾞｽﾀ設定処理
------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
******************************************************************************/
void	Io_sfr_holdx_in( void )
{
	#if 0 /* HOLDXに入ると、制御しなくてよい */
	__bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC0;	/* ｼｽﾃﾑｸﾛｯｸ選択：OSC0(32KHz) */
	__bf_OCR0.RCSTOP = 1;	/* RC発振停止 */
	#endif
	
	__HoldX();
}
void	Io_port_holdx_in( void )
{
	/* LCD_DO LOW出力設定 */
	pLCD_DO_FSA = 0;
	pLCD_DO_FSB = 0;
	pLCD_DO_LAT = 0;
	pLCD_DO_DDR = 1;
	/* LCD_CL LOW出力設定 */
	pLCD_CL_FSA = 0;
	pLCD_CL_FSB = 0;
	pLCD_CL_LAT = 0;
	pLCD_CL_DDR = 1;
}
void	Io_int_holdx_in( void )
{
	Io_beep_stop();
	
	/* ﾍﾞｰｽﾀｲﾏ 62.5ms Stop ACC_DETチャタ用 */
	//__bf_BTCR.RUN = 0;
	__bf_BTCR.IE1 = 0;
	
	/* 時計ﾀｲﾏ割込み開始(500ms) */
	__byte_CKTIMCTRL = BIN_10001111;
	
	/* 10msタイマ割り込み停止★★★ */
	__bf_T1CNT.IE = 0;
	__bf_T1CNT.RUN = 0;
	
	/* ACC_DET割込み許可 */
	__bf_INT23CR.INT3IF = 0;
	__bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_ENABLE;
	
	/* ALARMKEY検出割込み許可（AMFMラジオのみ） */
	#if 0
	if ((Ma_model_dest() >= 2)&&(Ma_model_dest() <= 6))
	#else
	if (Ma_get_alarm() == TRUE)
	#endif
	{
		__bf_P0FSA.P04IE = 1;
		__bf_P0FSA.P04IL = 0;		/* Lレベル */
	}
}

/******************************************************************************
HOLDX → ACC OFF(通常)
*******************************************************************************
	関数名	：Io_xxx_holdx_out
	引  数	：無し
	戻り値	：無し
	機  能	：HOLDX OUT 時のﾚｼﾞｽﾀ設定処理
------------------------------------------------------------------------------
******************************************************************************/
void	Io_sfr_holdx_out(void)
{
#if ( _WDT_ == 1 )
	/* WDT開始 */
	__byte_WDTCR = BIN_00101001;	/*
				       |||||||+--- START   1:WDT START (0の書き込みで停止できない)
				       ||||||+---- OVF     0:暴走検出時ﾘｾｯﾄ禁止
				       |||||+----- USERFLG 0:固定
				       ||||+------ PDNSTOP 0:HOLDX突入時WDTﾘｾｯﾄ 1:HOLDX突入時WDT停止
				       |||+------- SRFLG   0:ﾘｾｯﾄ実行検出ﾌﾗｸﾞ
				       ||+-------- MDSEL   0:ｲﾝﾀﾗﾌﾟﾄﾓｰﾄﾞ 1:ﾘｾｯﾄﾓｰﾄﾞ
				       |+--------- -       0:固定
				       +---------- -       0:固定  */
	/* WDT ｶｳﾝﾀｸﾘｱ */
	__SFR_BITCLR(__WDTCR,0);
#endif

	#if 0 /* HOLDXに入ると、制御しなくてよい */
	__bf_OCR0.RCSTOP = 0;	/* RC発振開始 */
	__bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;	/* ｼｽﾃﾑｸﾛｯｸ選択：RC発振(1MHz) */
	#endif
}
void	Io_port_holdx_out( void )
{
	/* LCD_DO SIOデータ(CMOS)設定 */
	pLCD_DO_FSA = 1;
	pLCD_DO_FSB = 0;
	pLCD_DO_LAT = 0;
	pLCD_DO_DDR = 1;
	/* LCD_CL SIOクロック(CMOS)設定 */
	pLCD_CL_FSA = 1;
	pLCD_CL_FSB = 0;
	pLCD_CL_LAT = 0;
	pLCD_CL_DDR = 1;
}
void	Io_int_holdx_out( void )
{
	/* ﾍﾞｰｽﾀｲﾏ 62.5ms start ACC_DETチャタ用 */
	//__bf_BTCR.RUN = 1;
	__bf_BTCR.IE1 = 1;
	
	/* 時計ﾀｲﾏ割込み禁止 */
	__byte_CKTIMCTRL = BIN_10001110;
	
	/* 10msタイマ割り込み許可★★★ */
	__bf_T1CNT.IE = 1;
	__bf_T1CNT.RUN = 1;
	
	/* ACC_DET割込み停止 */
	__bf_INT23CR.INT3IF = 0;
	__bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
	
	/* ALARMKEY検出割込み禁止 */
	__bf_P0FSA.P04IE = 0;
	__bf_P0FSA.P04IL = 0;
	__bf_P0FSA.P04FLG = 0;
}






/******************************************************************************
ACC ON(POWER OFF) → ACC OFF(通常)
*******************************************************************************
	関数名	：Io_xxx_accoff_in
	引  数	：無し
	戻り値	：無し
	機  能	：POWER OFF IN 時のﾚｼﾞｽﾀ設定処理
			  システムクロックOSC0選択
			  RC発振/OSC1停止
------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
******************************************************************************/
void	Io_sfr_accoff_in( void )
{
	volatile WORD	wait_cnt ;
	
	__bf_OCR0.RCSTOP = 0;	/* RC発振開始 */
	
  	/* RC発振安定  */
	for (wait_cnt = 0; wait_cnt < 0x10; wait_cnt++)
	{
	}
	
	__bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_RCONCHIP;
	
	__bf_OCR0.ENOSC1 = 0;			/* OSC1 Stop */
	
}
void	Io_port_accoff_in( void )
{
	/* I2C_SCK LOW出力設定 */
	pI2C_SCK_FSA = 0;
	pI2C_SCK_FSB = 0;
	pI2C_SCK_LAT = 0;
	pI2C_SCK_DDR = 1;
	/* I2C_SDA LOW出力設定 */
	pI2C_SDA_FSA = 0;
	pI2C_SDA_FSB = 0;
	pI2C_SDA_LAT = 0;
	pI2C_SDA_DDR = 1;
}
void	Io_int_accoff_in( void )
{
	/* ﾍﾞｰｽﾀｲﾏ 62.5ms 開始 ACC_DETチャタ用 */
	//__bf_BTCR.RUN = 1;
	__bf_BTCR.IE1 = 1;
	
	/* 1msﾀｲﾏ停止 */
	__byte_T2CNT0 = BIN_00000000;
	
	/* 10msタイマ割り込み許可★★★ */
	__bf_T1CNT.IE = 1;
	__bf_T1CNT.RUN = 1;
	
	/* ACC_DET割込み停止 */
	__bf_INT23CR.INT3IF = 0;
	__bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
}

/******************************************************************************
ACC OFF(通常) → ACC ON(POWER OFF)
*******************************************************************************
	関数名	：Io_xxx_accoff_out
	引  数	：無し
	戻り値	：無し
	機  能	：POWER OFF IN 時のﾚｼﾞｽﾀ設定処理
			  システムクロックOSC0選択
			  RC発振/OSC1停止
------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
******************************************************************************/
void	Io_sfr_accoff_out( void )
{
	volatile WORD	wait_cnt ;
	
	__bf_OCR0.ENOSC1 = 1;			/* OSC1 Start */

	/* 50ms 以上Wait */
	for (wait_cnt = 0; wait_cnt < 0x0A0; wait_cnt++)
	{
	}
	
	
	__bf_OCR0.SCKSEL = __bfval_OCR0_SCKSEL_OSC1;
	
	__bf_OCR0.RCSTOP = __bfval_OCR0_RCSTOP_STOP;	/* RC発振停止 */
	
}
void	Io_port_accoff_out( void )
{
	/* I2C_SCK HIGH出力設定 */
	pI2C_SCK_FSA = 0;
	pI2C_SCK_FSB = 0;
	pI2C_SCK_LAT = 1;
	pI2C_SCK_DDR = 1;
	/* I2C_SDA HIGH出力設定 */
	pI2C_SDA_FSA = 0;
	pI2C_SDA_FSB = 0;
	pI2C_SDA_LAT = 1;
	pI2C_SDA_DDR = 1;
}
void	Io_int_accoff_out( void )
{
	/* ﾍﾞｰｽﾀｲﾏ 62.5ms 停止 ACC_DETチャタ用 */
	//__bf_BTCR.RUN = 0;
	__bf_BTCR.IE1 = 0;
	
	/* 1msﾀｲﾏ開始 */
	__byte_T2CNT0 = BIN_00000101;
	
	/* 10ms割り込み停止 AccOFF時Alarmキー検出用 */
	__bf_T1CNT.IE = 0;
	__bf_T1CNT.RUN = 0;
	
	/* ACC_DET割込み停止 */
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
	/* 時計タイマ500ms割り込み禁止 */
	__byte_CKTIMCTRL = BIN_10001110;
}

void Io_int_disable_almkey(void)
{
	/* ALARMKEY検出割込み禁止 */
	__bf_P0FSA.P04IE = 0;
	__bf_P0FSA.P04IL = 0;
}

void Io_int_disable_accdet(void)
{
	/* ACC_DET割込み停止 */
	__bf_INT23CR.INT3IF = 0;
	__bf_INT23CR.INT3IE = __bfval_INT23CR_INT3IE_DISABLE;
}
/******************************************************************************
	関数名	：IO_In_acc_det
	引  数	：無し
	戻り値	：HIGH
			   LOW
	機  能	：ACC-DET端子状態返却
------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
******************************************************************************/
BOOL	IO_In_acc_det(void)
{
	fIO_acc = MIO_IN_pACC_DET();

#if 0	/* 反転を使用しないのでコメントアウト */
	if ((pACC_DET_FSA == 0) &&
		(pACC_DET_FSB == 1))
	{
		fIO_acc = ~fIO_acc;
	}
#endif
	return(fIO_acc);
}
/******************************************************************************
	関数名	：IO_In_ill_det
	引  数	：無し
	戻り値	：HIGH
			   LOW
	機  能	：ILL-DET端子状態返却
------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
******************************************************************************/
BOOL	IO_In_ill_det(void)
{
	fIO_ill = MIO_IN_pILL_DET();
	return(fIO_ill);
}
/******************************************************************************
	関数名	：IO_In_acc_det
	引  数	：無し
	戻り値	：HIGH
			   LOW
	機  能	：ALARM_KEY端子状態返却
------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
******************************************************************************/
BOOL	IO_In_alarm_key(void)
{
	fIO_alarm = MIO_IN_pALARM_KEY();
	return(fIO_alarm);
}

/******************************************************************************
	関  数 名	: Io_beep_start
	引     数	: 無し
	戻  り 値	: 無し
	機能概略	: 2kHzのBEEPを鳴動します
    備     考 	: 停止は、 Io_beep_stop をコールしてください
-------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
[20130415yamamoto] 仕様変更 4kHz⇒2kHz
********************************************************************************/
void Io_beep_start( void )
{
	__bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;		/* トグル出力停止 */

	__bf_T0CNT.CKSEL = __bfval_T0CNT_CKSEL_OSC0;
	
	__byte_T0LR = 0x07;								/* MODE3 2kHz */
	__bf_T0PR.PR = 0;
	
	__bf_T0PR.MODE = __bfval_T0PR_MODE_MODE3;
	__bf_T0CNT.RUN = __bfval_T0CNT_RUN_RUN;			/* トグル出力開始 */
}
/******************************************************************************
	関  数 名	: Io_beep_start
	引     数	: 無し
	戻  り 値	: 無し
	機能概略	: 8HzのBEEPを鳴動します,TESTモード用
    備     考 	: 停止は、 Io_beep_stop をコールしてください
-------------------------------------------------------------------------------
[20120926yamamoto] 新規作成
********************************************************************************/
void Io_beep_test_8Hz_start( void )
{
	__bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;		/* トグル出力停止 */

	__bf_T0CNT.CKSEL = __bfval_T0CNT_CKSEL_OSC0;
	
	__byte_T0LR = 0xFF;							/* MODE3 8Hz */
	__byte_T0PR = 0x67;							/* ﾃｽﾄﾓｰﾄﾞ用 */

	__bf_T0CNT.RUN = __bfval_T0CNT_RUN_RUN;			/* トグル出力開始 */
}


/******************************************************************************
	関  数 名	: Io_beep_stop
	引     数	: 無し
	戻  り 値	: 無し
	機能概略	: 4kHzのBEEPを停止します
    備     考 	:
-------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
********************************************************************************/
void Io_beep_stop( void )
{
	__bf_T0CNT.RUN = __bfval_T0CNT_RUN_STOP;		/* トグル出力停止 */
}
/******************************************************************************
	関  数 名	: Io_mincnt_read
	引     数	: 無し
	戻  り 値	: 0-1439(分)
	機能概略	: 日分秒カウンタの分の値を読み出す
    備     考 	: 必ず日分秒カウンタが動作している時に使用してください。
-------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
********************************************************************************/
WORD Io_mincnt_read( void )
{
	BYTE i;
	BYTE old_sec;
	WORD old_min;
//	WORD old_day;
	BYTE new_sec;
	WORD new_min;
//	WORD new_day;
	
	/* 分秒を連続して読み出して一致すれば分をreturn */
	for(i=0;i<2;i++)
	{
		old_sec = __byte_SEC;
		old_min = __word_MINL;
	//	old_day = __word_DAYL;
		new_sec = __byte_SEC;
		new_min = __word_MINL;
	//	new_day = __word_DAYL;
		
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
	関  数 名	: Io_mincnt_write
	引     数	: WORD min：分の値を更新します。
	戻  り 値	: 無し
	機能概略	: 分カウンタの値を更新する
    備     考 	: 
-------------------------------------------------------------------------------
[20120731yamamoto] 新規作成
[20121017yamamoto] 仕様変更対応 時計ﾀｲﾏを停止する様に変更
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
	関   数  名	: Io_wdt_clear
	引       数	: なし
	戻   り  値	: なし
	機 能 概 略	: ウォッチドッグタイマのクリアを行う
    備       考 : 
-------------------------------------------------------------------------------
修正履歴
	番号	日付       担当者		修正内容
-------------------------------------------------------------------------------
  【000】	09/6/18    佐久田		新規作成
********************************************************************************/
void Io_wdt_clear(void)
{
	/* WDT ｶｳﾝﾀｸﾘｱ */
	__SFR_BITCLR(__WDTCR,0);
}

#endif


#undef	_IO_SFR_C_

