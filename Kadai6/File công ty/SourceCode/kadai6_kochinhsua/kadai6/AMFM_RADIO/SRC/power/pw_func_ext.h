/******************************************************************************
	クラリオン（株）殿向け		2007 C3X03 ｼﾘｰｽﾞ[仮]

		プロジェクト名	: '07年フリップダウン　MP3モデル開発
		ファイル名		: pw_func_ext.h
		機能			: Power Module 外部参照関数群関数用ﾍｯﾀﾞｰ･ﾌｧｲﾙ
------------------------------------------------------------------------------
修正履歴
【番号】	日付		担当者	修正内容
【NKC-0000】'05/12/08	仲地	新規作成
******************************************************************************/
#ifndef	_PW_FUNC_H_
#define	_PW_FUNC_H_

#undef	EXT
#ifdef	_PW_FUNC_C_
#define	EXT
#else
#define	EXT	extern
#endif

#include	"../power/pw_def.h"

/*----------------------------------------------------
   関数ﾌﾟﾛﾄｺｰﾙ
----------------------------------------------------*/
/*	Power Module 1ms ｲﾝﾀｰﾊﾞﾙ処理関数：1msﾍﾞｰｽ･ﾀｲﾏｰｶｳﾝﾄ処理					*/
EXT	void	Pw_1ms_timer(void);

/*	Power Module 2ms ｲﾝﾀｰﾊﾞﾙ処理関数：1msﾍﾞｰｽ･ﾀｲﾏｰｶｳﾝﾄ処理					*/
EXT	void	Pw_2ms_timer(void);

/*	Power Module 4ms ｲﾝﾀｰﾊﾞﾙ処理関数：1msﾍﾞｰｽ･ﾀｲﾏｰｶｳﾝﾄ処理					*/
EXT	void	Pw_4ms_timer(void);

/*	Power Module 5ms ｲﾝﾀｰﾊﾞﾙ処理関数：5msﾍﾞｰｽ･ﾀｲﾏｰｶｳﾝﾄ処理				*/
EXT	void	Pw_5ms_timer(void);

/*	Power Module 10ms ｲﾝﾀｰﾊﾞﾙ処理関数：10msﾍﾞｰｽ･ﾀｲﾏｰｶｳﾝﾄ処理				*/
EXT	void	Pw_10ms_timer(void);

/* AccOFF中 62.5msインターバル処理関数 */
EXT	void	Pw_62ms_cycle(void);

/*	ACCステータス 状態取得関数：電源制御ステータスを返す。		*/
EXT	BYTE	Pw_ACC_status_get(void);

/*	ACC Port 状態取得関数：ACCﾎﾟｰﾄを監視しHi/Lowﾁｬﾀﾘﾝｸﾞ後の状態を返す。		*/
EXT	BYTE	Pw_ACC_DET_get(void);

/*	Power on/offを監視し状態を返す。				*/
EXT	BYTE	Pw_Power_status_get(void);

/*	Power Key状態を取得処理		 */
EXT BYTE	Pw_Power_Key_get(void);

/*	PowerOff中かどうかの表示チェック	*/
EXT BYTE	Pw_PowerOff_disp_chk(void);

EXT	void 	Pw_power_key(void);
EXT	void 	Pw_power_event(BYTE, BYTE);

/* HoldX状態時、割り込み発生により、AccOFF通常状態にいく時の割り込み処理 */
EXT void	Pw_acc_off_nor_req(BYTE mode);

EXT	BOOL	Pw_ret_illdet(void);
EXT	BOOL	Pw_ret_almkey(void);
EXT	void	Pw_almkey_clear(void);

/* AccOFF時、「アラーム時刻到着」 OR 「アラーム調整モードに入る」の場合、Powerへ知らせ */
EXT	void	Pw_acc_wakeup_req(BYTE onoff);

/* テストモード機能 */
EXT	void	Pw_test_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length);

#if _POWER_DIAG_
/* DIAG検出機能 */
EXT	void Pw_DIAG_check(BYTE mode);
EXT	void Pw_DIAG_clear( void );
#endif	/* _POWER_DIAG_ */

/*----------------------------------------------------
   ﾌﾗｸﾞ 定義
----------------------------------------------------*/
/*	Power Module Flag Memory area					*/
/*	EXT	_bram	BYTE	Power_flag;					*/
/*	EXT	_bit	fpw_yobi8	_atbit(key_flag, 7);	*/
/*--------------------------------------------------*/
/*	高速アクセス変数定義							*/
/*--------------------------------------------------*/
/*EXT	_near	BYTE	pw_speed_input;				*/
/*--------------------------------------------------*/
/*	変数定義										*/
/*--------------------------------------------------*/
/*EXT	BYTE	pw_data_aria;						*/
/*--------------------------------------------------*/

#endif
