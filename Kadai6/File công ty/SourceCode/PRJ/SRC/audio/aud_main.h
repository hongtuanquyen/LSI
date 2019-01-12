/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		: 	aud_main.h
	モジュール名	: 	Audioモジュール
	機能概略		: 	メイン処理のヘッダーファイル
-------------------------------------------------------------------------------
修正履歴
	番号		日付		担当者		関数名			修正内容
	【000】		12/07/13	leejh						新規作成
********************************************************************************/
#ifndef		_AUD_MAIN_H_
#define		_AUD_MAIN_H_

#include	"../model.h"

#undef		EXT
#ifdef		_AUD_MAIN_C_
#define		EXT
#else
#define		EXT extern
#endif

/*---------------------------------------------------------------------------*
 *	構造体定義															 	 *
 *---------------------------------------------------------------------------*/
typedef struct {
	BYTE	zero_mute	:1;	/* volume zero mute		0: mute off	1: mute on	 */
	BYTE	reserved	:7;	/* reserved										 */
} TAUD_EVOL_MUTE;

EXT TAUD_EVOL_MUTE		aud_mute_evol;		/* EVOL処理mute情報				 */

/*---------------------------------------------------------------------------*
 *	E-VOL制御ブロックプロトタイプ定義									 	 *
 *---------------------------------------------------------------------------*/
void Aud_msg_mode_change( void );
void Aud_msg_maincnt_on( void );
void Aud_msg_maincnt_off_req( void );
void Aud_msg_maincnt_off( void );
void Aud_msg_power_on( void );
void Aud_msg_power_off( void );

void Aud_edit_volume( BYTE );
void Aud_edit_bass( BYTE );
void Aud_edit_treble( BYTE );
void Aud_edit_balance( BYTE );
void Aud_edit_fader( BYTE );

TAUD_AUDIO_INFO *Aud_get_audio_info_list( void );
BYTE Aud_get_audio_info( BYTE );
BYTE Aud_get_power_mode( void );
void Aud_evol_main( void );

void Aud_set_timer( BYTE , BYTE , BYTE );
BYTE Aud_get_timer( BYTE , BYTE );
void Aud_dec_timer( TAUD_TIMER_INFO * );

void Aud_msg_timer( BYTE );
void Aud_msg_initial( void );
void Aud_msg_main( void );

#if _TEST_MODE_
void Aud_secret_loud_off( void );
void Aud_test_edit_data( BYTE, BYTE *, BYTE );
#endif
#endif
