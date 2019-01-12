/******************************************************************************
	クラリオン（株）殿向け		2007 C3X03 ｼﾘｰｽﾞ[仮]

		プロジェクト名	: '07年フリップダウン　MP3モデル開発
		ファイル名		: eeprom_main.c
		機能			: 電源モード遷移処理
------------------------------------------------------------------------------
修正履歴
[sun070228] 改造
******************************************************************************/
#include	"../model.h"
#if 	_MODEL_EEPROM_
#define	_EEPROM_MAIN_C_

#include	<string.h>
#include	"../comm/common.h"
#include	"../main/ma_ext.h"
#include	"../io/IO_inc.h"

#include	"eeprom_main.h"
#include	"eeprom_ext.h"

static BYTE	EEP_Drv_AckPolling(void);

#define		CEEP_MODE_W		0xa0
#define		CEEP_MODE_R		0xa1

#define		CIO_IIC_NACK	1
#define		CIO_IIC_ACK		0

/*****************************************************************************************
	関数名		：BYTE	EEP_Drv_AckPolling(void)	
	引数		：なし
	リターン	：なし
	機能概要	：EEPROM Acknowledgeのポーリング処理
				：�StartBit送信
				：�書込みConditionﾃﾞｰﾀ送信
				：�Ackﾁｪｯｸ
				：MAX　6ms間ACK　OKまで待つ
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
*****************************************************************************************/
BYTE	EEP_Drv_AckPolling(void)
{
	BYTE	p_ret;
	BYTE	iCnt;
	
	iCnt = 10;
	p_ret = NG;
	while(iCnt > 0)
	{
		IO_EEP_SendStart();					/*スタートコンディション送信*/
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
	関数名		：BYTE	EEP_Drv_Read(void)	
	引数		：なし
	リターン	：なし
	機能概要	：EEPROMﾄﾞﾗｲﾊﾞMain処理
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
*****************************************************************************************/
BYTE	EEP_Drv_Read(void)
{
	BYTE	r_ret;
	
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
	関数名		：BYTE	EEP_Drv_Write(void)
	引数		：なし
	リターン	：なし
	機能概要	：EEPROMﾄﾞﾗｲﾊﾞMain処理
-----------------------------------------------------------------------------------------
[sun100806] 新規作成
*****************************************************************************************/
BYTE	EEP_Drv_Write(void)
{
	BYTE i;
	BYTE w_ret;
	
	/* start conditon */
	IO_EEP_SendStart();
	
	/* device address */
	IO_EEP_WriteByte(0xa0);
	
	/* WORD ADDRESS(00) */
	w_ret = IO_EEP_WriteByte(sEEP_DRVWork.SlaveAddr);
	
	#if 1		/* 16BYTE書込み可能、16BYTE以内なら一括で */
	/* SantaFeによって、1BYTEか16BYTEしか書込みしない、Length判断する必要はない */
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
		sEEP_DRVWork.Length -= 8;		/* length -8 */
		sEEP_DRVWork.SlaveAddr += 8;		/* slaveaddress + 8 */
		sEEP_DRVWork.DataAddr += 8;		/* address + 8 */
	}
	#endif
	
	/* IO_EEP_SendStop */
	IO_EEP_SendStop();
	
	bEEP_Drv_Timer = 11;
	
	return w_ret;
}


/******************************************************************************
.. 		関数名  ：eeprom_initial
		引  数	：なしﾞ
		戻り値	：無し
		機  能	：EEPROM 初期読込み処理
------------------------------------------------------------------------------
******************************************************************************/
void	eeprom_initial(void)
{
	#if 1
	IO_EEP_Port_Reset();
	#endif
	EEP_reset();
	
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
.. 		関数名  ：eeprom_main
		引  数	：なしﾞ
		戻り値	：無し
		機  能	：EEPROM Systemﾒｯｾｰｼﾞ受付処理
------------------------------------------------------------------------------
[sun100806] 新規作成
******************************************************************************/
void	eeprom_main(void )
{
	
	BYTE	status;
	
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
#endif	/* _MODEL_EEPROM_ */
