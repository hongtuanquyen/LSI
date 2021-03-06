/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		:	aud_sys.h
	モジュール名	:	Audioモジュール
	機 能 概 略		:	システムから発行されたメッセージ処理のヘッダーファイル
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		関数名		修正内容
	【000】		12/07/13	leejh					新規作成
******************************************************************************/
#ifndef		_AUD_SYS_H_
#define		_AUD_SYS_H_

#include	"../model.h"

#undef		EXT
#ifdef		_AUD_SYS_C_
#define		EXT
#else
#define		EXT extern
#endif

/*----------------------------------------------------
   外部IF関数 
----------------------------------------------------*/
EXT void Aud_sys_message( BYTE );	/* システムメッセージ処理関数 */

#endif
