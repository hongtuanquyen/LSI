/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		:	aud_sys.c
	モジュール名	:	Audioモジュール
	機 能 概 略		:	システムから発行されたメッセージ処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		関数名		修正内容
	【000】		2005.12.09	曽田			-		新規作成
******************************************************************************/
#define		_AUD_SYS_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../main//MA_inc.h"

#include	"aud_ext.h"
#include	"aud_sys.h"
#include	"aud_def.h"
#include	"aud_main.h"

/******************************************************************************
	関数名		：	Aud_sys_message
	引数		：	BYTE	sys_msg:	システムコマンド
	戻り値		：	無し
	機能概略	：	システムコマンド受信メイン処理
------------------------------------------------------------------------------
	修正履歴
	 番号		日付		担当者		修正内容
	【000】		12/07/13	leejh		新規作成
******************************************************************************/
void Aud_sys_message( BYTE sys_msg )
{
	switch ( sys_msg )
	{
		/* E-VOLメイン処理 */
		case CMSG_MAIN:
			Aud_msg_main();
			break;

		/* 初期化処理 */
		case CMSG_INITIAL:
			Aud_msg_initial();
			break;

		/* MAINCNT ON電源処理 */
		case CMSG_MAINCNT_ON:
			Aud_msg_maincnt_on();
			break;

		/* POWER ON電源処理 */
		case CMSG_PWRON:
			Aud_msg_power_on();
			break;

		/* モードチェンジ処理 */
		case CMSG_MODECHG:
			Aud_msg_mode_change();
			break;

		/* POWER OFF電源処理 */
		case CMSG_PWROFF:
			Aud_msg_power_off();
			break;

		/* MAINCNT OFFREQ電源処理 */
		case CMSG_MAINCNT_OFFREQ:
			Aud_msg_maincnt_off_req();
			break;

		/* MAINCNT OFF電源処理 */
		case CMSG_MAINCNT_OFF:
			Aud_msg_maincnt_off();
			break;

		/* タイマ処理 */
		case CMSG_25MS_TIMER:
			Aud_msg_timer( CAUD_EVT_TIMER_025M );
			break;
			
		default:
			break;
	}
}

#undef _AUD_SYS_C_

