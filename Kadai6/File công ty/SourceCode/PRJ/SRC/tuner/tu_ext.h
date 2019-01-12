/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_ext.h
		機能			: ①Tuner 外部IF ﾍｯﾀﾞﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212]	新規作成
******************************************************************************/
#ifndef		_TU_EXT_H_
#define		_TU_EXT_H_

#include	"../disp/lcd_ext.h"

#undef		EXT
#ifdef		_TU_EXT_C_
#define		EXT
#else
#define		EXT		extern
#endif

/*-------------------------------------
		 関数宣言
-------------------------------------*/
EXT	BYTE Tu_band_chk(void);
EXT	BYTE Tu_bandno_chk(void);
EXT	BYTE Tu_pstno_chk(void);
EXT	void Tu_1ms_timer(void);
EXT	void Tu_5ms_timer(void);
EXT	BYTE Tu_indicator_set(void);
EXT	BYTE Tu_isr_indicator_set(void);
EXT	BYTE Tu_mainmd_check(void);
EXT	BYTE Tu_st_check(void);
EXT	void Tu_lcddata_set(TDP_TUNER * , BYTE);
EXT	void Tu_isr_lcddata_set(TDP_ISR * , BYTE);
EXT	void Tu_amband_set(void);
EXT	void Tu_isrmd_on(void);
EXT	void Tu_isrmd_off(void);
EXT	void Tu_bnd_set(BYTE bandno);		/*	テストモードの直接AM/FM1選択を対応、100826、岳	*/
EXT	void Tu_beep_mute(BYTE on_off);
/* 他ファイル関数 */
EXT	void Tu_SaveEepData(BYTE status,BYTE *BufAddr);	/* tu_eeprom.c */
EXT	void Tu_key_isr_on(void);						/* tu_key.c */
EXT	void Tu_cmd_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Lenth);	/* tu_test.c */

/*-------------------------------------
		 BAND 情報定義
-------------------------------------*/
#define		CTU_FM		0 		/* FM BAND中 */
#define		CTU_AM		1	 	/* AM BAND中 */

#define		CTU_BAND_AM		0 		/* AM BAND中 */
#define		CTU_BAND_FM1	1	 	/* FM1 BAND中 */
#define		CTU_BAND_FM2	2 		/* FM2 BAND中 */

#define		CTU_BAND0	0		/* BandNo無し */
#define		CTU_BAND1	1		/* BandNo 1 */
#define		CTU_BAND2	2		/* BandNo 2 */
#define		CTU_BAND3	3		/* BandNo 3 */


#endif
