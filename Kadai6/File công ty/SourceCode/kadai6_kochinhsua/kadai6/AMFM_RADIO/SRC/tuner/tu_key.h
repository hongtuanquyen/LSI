/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_key.h
		機能			: Tunerｷｰ定義ﾍｯﾀﾞﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051214]	新規作成
[zhang070306]isr,entやキー処理関数宣言を追加
[zhang070308]リモコンBandｷｰOn処理関数宣言を追加
******************************************************************************/
#ifndef	_TU_KEY_H_
#define	_TU_KEY_H_

#include	"../key/KEY_inc.h"

#undef	EXT
#ifdef	_TU_KEY_C_
#define	EXT
#else
#define	EXT	extern
#endif


/*----------------------------------------------------
  	ｷｰ処理関数宣言
----------------------------------------------------*/
EXT KEY_PROC_TBL * Tu_key_func_addr_get(void);
EXT KEY_CHG_TBL * Tu_keyCodeTable_addr_get(void);
EXT	BYTE	Tu_ModeID_get(void);

EXT	void	Tu_key_nop(void);
EXT	void	Tu_key_pw_long(void);
EXT	void	Tu_key_pw_off(void);
EXT	void	Tu_key_md_long(void);
EXT	void	Tu_key_md_off(void);
EXT	void	Tu_key_up_on(void);
EXT	void	Tu_key_up_long(void);
EXT	void	Tu_key_up_off(void);
EXT	void	Tu_key_dw_on(void);
EXT	void	Tu_key_dw_long(void);
EXT	void	Tu_key_dw_off(void);
EXT	void	Tu_key_as_on(void);
EXT	void	Tu_key_as_long(void);
EXT	void	Tu_key_ch_on(void);
EXT	void	Tu_key_ch_long(void);
EXT	void	Tu_key_ch_off(void);
EXT	void	Tu_key_tisc_on(void);
EXT	void	Tu_key_ti_long(void);
EXT	void	Tu_key_ti_off(void);
EXT	void	Tu_key_alm_on(void);
EXT	void	Tu_key_alm_long(void);
EXT	void	Tu_key_alm_off(void);
EXT	void	Tu_key_isr_on(void);
EXT	void	Tu_key_isrup_on(void);
EXT	void	Tu_key_isrup_off(void);
EXT	void	Tu_key_isrdw_on(void);
EXT	void	Tu_key_isrdw_off(void);

/*----------------------------------------------------
 Tuner 有効BIT定義
----------------------------------------------------*/
#define		CTU_KYEN_PWRON		0x0001
#define		CTU_KYEN_SEEK		0x0002
#define		CTU_KYEN_AS			0x0004
#define		CTU_KYEN_SCAN		0x0008

#define		CTU_KYEN_ALL		0xffff
#define		CTU_KYDIS_ALL		0x0000

/*----------------------------------------------------
  	Tuner ｷｰｺｰﾄﾞ定義
----------------------------------------------------*/
enum
{
	CTU_KY_NOP	=	0,
	CTU_KY_VOL_UP	,
	CTU_KY_VOL_DW	,
	CTU_KY_PWR		,
	CTU_KY_SRC		,
	CTU_KY_FREQ_UP	,
	CTU_KY_FREQ_DW	,
	CTU_KY_CH		,
	CTU_KY_AS		,
	CTU_KY_ALARM	,
	CTU_KY_TISC		,
	
	CTU_KY_TEST		,
	CTU_KY_STMN		,	/*[zhang070727] ST TEST込みをキー追加*/
	
};



#endif
