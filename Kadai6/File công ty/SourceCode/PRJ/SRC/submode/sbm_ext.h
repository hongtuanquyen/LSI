/*************************************************************************
	クラリオン殿向け 
	プロジェクト名	：2012年AMFM_RADIOﾓﾃﾞﾙ
	ファイル名		：sbm_ext.h
	モジュール名	：SUBモード
	機   能			：外部ｲﾝﾀﾌｪｰｽヘッダﾌｧｲﾙ
--------------------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
--------------------------------------------------------------------------
			2012/7/25		0.1		lrj		新規
*************************************************************************/
#ifndef		_SBM_EXT_H_
#define		_SBM_EXT_H_

#include	"../model.h"


#include	"../disp/lcd_ext.h"
#include	"../key/key_func_ext.h"
#include	"../key/key_def.h"
#include	"../testmode/TEST_inc.h"

#include	"../submode/sbm_def.h"


/*-----------------------共通--------------------*/
#undef	EXT
#ifdef	_SBM_EXT_C_
#define	EXT   
#else
#define	EXT	extern
#endif


EXT BYTE Sbm_current_mode(void);
EXT BYTE Sbm_mode_check(void);
EXT void Sbm_mode_cancel(BYTE);
EXT KEY_CHG_TBL *Sbm_keyCodeTable_addr_get(void);
EXT KEY_PROC_TBL * Sbm_key_func_addr_get(void);
EXT void Sbm_key_volupon(void);
EXT void Sbm_key_voluplong(void);
EXT void Sbm_key_volupoff(void);
EXT void Sbm_key_voldwon(void);
EXT void Sbm_key_voldwlong(void);
EXT void Sbm_key_voldwoff(void);
EXT BYTE Vol_lcddata_set(void);
EXT BYTE Vol_mode_check(void);
EXT void Vol_mode_cancel(void);
EXT void Sbm_mode_initial(void);
EXT BYTE Sbm_indicator_set(void);
EXT void Sbm_BeepOn_Power(void);
EXT BYTE Sct_mode_check(void);
EXT void Sct_mode_cancel(void);
EXT void Sct_key_test(BYTE);
EXT void Sct_key_pwr_on(void);
EXT void Sct_key_Ver_dispoff(void);
EXT void Sbm_key_tisc_on(void);



/*---------------------時計調整ﾓｰﾄﾞ--------------*/
#undef	EXT
#ifdef	_CLOCK_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif
EXT void Clk_time_main(void);
EXT void Clk_mode_initial(void);
EXT void Clk_lcddata_set( TDP_CLK * );
EXT BYTE Clk_indicator_set(void);
EXT BYTE Clk_adj_check(void);
EXT void Clk_adj_start(void);
EXT void Clk_adj_cancel(void);
EXT void Clk_adj_hour_up(void);
EXT void Clk_adj_min_up(void);
EXT void Clk_adj_tim_rst(void);
EXT void Clk_adj_set(void);
EXT void Clk_key_up_off(void);
EXT void Clk_key_up_long(void);
EXT void Clk_key_dw_off(void);
EXT void Clk_key_dw_long(void);
EXT void Clk_key_as_off(void);
EXT void Clk_key_md_off(void);
EXT void Clk_key_pwr_long(void);
EXT void Clk_key_voluplong(void);
EXT void Clk_key_volupoff(void);
EXT void Clk_key_voldwlong(void);
EXT void Clk_key_voldwoff(void);
#if _TEST_ALARM_ON_
EXT void Clk_alarm_test(void);
#endif


/*---------------------AUDIO調整ﾓｰﾄﾞ-------------------*/
#undef	EXT
#ifdef	_SEF_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif
EXT void Sef_lcddata_set(TDP_SEL *);
EXT void Sef_mode_start(void);
EXT void Sef_mode_chg(void);
EXT BYTE Sef_mode_check(void);
EXT void Sef_mode_cancel(void);
EXT void Sef_audio_updw(BYTE);
EXT void Sef_key_pwr_on(void);
EXT void Sef_key_pwr_long(void);
EXT void Sef_key_up_long(void);
EXT void Sef_key_up_off(void);
EXT void Sef_key_dw_long(void);
EXT void Sef_key_dw_off(void);
EXT void Sef_key_alm_on(void);
EXT void Sef_key_alm_long(void);
EXT void Sef_key_md_off(void);
EXT void Sef_mode_initial(void);



/*---------------------ALARM調整ﾓｰﾄﾞ-------------------*/
#undef	EXT
#ifdef	_ALM_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif
EXT void Alm_lcddata_set(TDP_CLK *);
EXT BOOL Alm_bell_indicator_set(void);
EXT BYTE Alm_indicator_set(void);
EXT void Alm_mode_start(void);
EXT BYTE Alm_mode_check(void);
EXT void Alm_mode_cancel(void);
EXT void Alm_mode_hour_up(void);
EXT void Alm_mode_min_up(void);
EXT void Alm_mode_set(void);
EXT void Alm_onoff_set(void);
EXT BOOL Alm_beepon_get(void);
EXT void Alm_key_pwr_on(void);
EXT void Alm_key_up_off(void);
EXT void Alm_key_up_long(void);
EXT void Alm_key_dw_off(void);
EXT void Alm_key_dw_long(void);
EXT void Alm_beep_key_stop(void);
EXT void Alm_key_tisc_on(void);
EXT void Alm_key_volupon(void);
EXT void Alm_key_voluplong(void);
EXT void Alm_key_volupoff(void);
EXT void Alm_key_voldwon(void);
EXT void Alm_key_voldwlong(void);
EXT void Alm_key_voldwoff(void);
EXT void Alm_beep_on_stop(void);
EXT void Alm_mode_initial(void);


#endif

