/*************************************************************************
	クラリオン殿向け 
	プロジェクト名	：2012年AMFM_RADIOﾓﾃﾞﾙ
	ファイル名		：sbm_sys.h
	モジュール名	：SUBMODE
	機   能			：Sub systemメッセージ受付処理
--------------------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
--------------------------------------------------------------------------
			2012/7/23	0.1			lrj		新規
*************************************************************************/
#ifndef		_SBM_SYS_H_
#define		_SBM_SYS_H_

#include	"../model.h"

#undef	EXT
#ifdef	_SBM_SYS_C_
#define	EXT   
#else
#define	EXT	extern
#endif

EXT	void Sbm_sys_message( BYTE );
EXT void Sbm_100ms_cnt(void);
EXT void Sbm_msg_modechg(void);

#endif
