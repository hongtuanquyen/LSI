/******************************************************************************
	クラリオン（株）殿向け		2007 C3X03 ｼﾘｰｽﾞ[仮]

		プロジェクト名	: '07年フリップダウン　MP3モデル開発
		ファイル名		: eeprom_sys.h
		機能			: ①EEPROM制御ﾓｼﾞｭｰﾙ･System Message関数群　ﾍｯﾀﾞｰ･ﾌｧｲﾙ
------------------------------------------------------------------------------
修正履歴
******************************************************************************/
#include	"../model.h"
#if 	_MODEL_EEPROM_
#ifndef	_EEPROM_SYS_H_
#define	_EEPROM_SYS_H_

#undef	EXT
#ifdef	_EEPROM_SYS_C_
#define	EXT   
#else
#define	EXT	extern
#endif

EXT	void	Eeprom_sys_message(BYTE );

#endif
#endif	/* _MODEL_EEPROM_ */
