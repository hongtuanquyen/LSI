/******************************************************************************
	クラリオン殿向け
	プロジェクト名	: 2012年AMFM_RADIOモデル
	ファイル名		: key_func_ext.h
	機能			: Key Module 外部参照関数群関数用ヘッダーファイル
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
#ifndef	_KEY_FUNC_H_
#define	_KEY_FUNC_H_

#undef	EXT
#ifdef	_KEY_FUNC_C_
#define	EXT
#else
#define	EXT	extern
#endif

/*----------------------------------------------------
   関数
----------------------------------------------------*/
EXT	void	Key_1ms_timer(void);
EXT	void	Key_5ms_timer(void);
EXT	void	Key_check_stop(void);
EXT	void	Key_check_start(void);
EXT	void	Key_long_time200(void);
EXT	void	Key_long_time500(void);
EXT	void	Key_long_time1000(void);
EXT	void	Key_long_time2000(void);
EXT	void	Key_nop(void);
EXT	BYTE	Key_get_priokey(void);		
EXT void	Key_testmode_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length);

EXT	void	Key_10ms_timer_accoff(void);
EXT	void	Key_long_time2000_accoff(void);

#endif