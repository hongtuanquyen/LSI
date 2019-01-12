/******************************************************************************
	クラリオン殿向け
	プロジェクト名	：2012年AMFM_RADIOモデル
	ファイル名		: aux_key.h
    モジュール名	: AUX
	機	能			: AUX KEY処理ヘッダーファイル
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
#ifndef	_AUX_KEY_H_
#define	_AUX_KEY_H_

#include    "../key/key_def.h"

#undef	EXT
#ifdef	_AUX_KEY_C_
#define	EXT
#else
#define	EXT	extern
#endif


/*----------------------------------------------------
  	関数プロトタイプ
----------------------------------------------------*/
EXT KEY_PROC_TBL * Aux_key_func_addr_get(void);
EXT KEY_CHG_TBL * Aux_keyCodeTable_addr_get(void);
EXT BYTE Aux_ModeID_get(void);

#endif
