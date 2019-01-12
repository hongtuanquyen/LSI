/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		: 	aud_beep_func.h
	モジュール名	: 	Audioモジュール
	機能概略		: 	ブザー制御処理のヘッダーファイル
-------------------------------------------------------------------------------
修正履歴
	番号		日付		担当者		関数名			修正内容
	【000】		12/07/13	leejh						新規作成
********************************************************************************/
#ifndef		_AUD_BEEP_FUNC_H_
#define		_AUD_BEEP_FUNC_H_

#undef		EXT
#ifdef		_AUD_BEEP_FUNC_C_
#define		EXT
#else
#define		EXT extern
#endif

/*---------------------------------------------------------------------------*
 *	プロトタイプ定義													 	 *
 *---------------------------------------------------------------------------*/
BOOL Aud_is_beep_mute( void );
void Aud_set_stop_beep( void );
BYTE Aud_get_beep_info( void );
void Aud_req_beep( BYTE );
void Aud_beep_main( void );
void Aud_init_beep_func( void );
#endif
