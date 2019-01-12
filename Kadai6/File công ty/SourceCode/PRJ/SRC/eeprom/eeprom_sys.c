/******************************************************************************
	クラリオン（株）殿向け		2007 C3X03 ｼﾘｰｽﾞ[仮]

		プロジェクト名	: '07年フリップダウン　MP3モデル開発
		ファイル名		: eeprom_sys.c
------------------------------------------------------------------------------
修正履歴
******************************************************************************/
#include	"../model.h"
#if 	_MODEL_EEPROM_
#define		_EEPROM_SYS_C_

#include	"../comm/common.h"
#include	"../io/IO_ext.h"
#include	"eeprom_main.h"
#include	"eeprom_ext.h"
#include	"eeprom_sys.h"

/******************************************************************************
.. 		関数名  ：Eeprom_sys_message
		引  数	：Systemﾒｯｾｰｼﾞ
		戻り値	：無し
		機  能	：EEPROM Systemﾒｯｾｰｼﾞ受付処理
------------------------------------------------------------------------------
修正履歴
******************************************************************************/
void	Eeprom_sys_message(BYTE	msg)
{
	switch(msg)
	{
		case	CMSG_MAIN:
			eeprom_main();
			break;
		case	CMSG_MAINCNT_ON:
			/* 実測値：*/
			/* SYS_ONがHIGHになってから、EEPROMの電源がHIGHになる（eepromのPin8）まで300ns */
			/* SYS_ONがHIGHになってから、CMSG_MAINCNT_ONがくるまで90μm */
			/* よって、ここでリセットをかけていい */
			IO_EEP_Port_Reset();
			break;
		case	CMSG_INITIAL:
			eeprom_initial();
			break;
		default:
			break;
	}
}


#undef  _EEPROM_SYS_C_
#endif	/* _MODEL_EEPROM_ */
