/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_item.h
		機能			: ①Tuner itemヘッダﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051213]	新規作成
******************************************************************************/
#ifndef	_TU_ITEM_H_
#define	_TU_ITEM_H_

#undef	EXT
#ifdef	_TU_ITEM_C_
#define	EXT
#else
#define	EXT	extern
#endif

/* Item処理戻り値 */
#define		CTU_I_OKEND		0xff			/* ITEM処理OK完了 */
#define		CTU_I_NGEND		0xfe			/* ITEM処理NG完了 */
#define		CTU_I_NOTEND	0xfd			/* ITEM処理中 */

/* Item処理遷移値 */
#define		CTU_NEXT_M		0xff			/* 次のﾒｲﾝﾓｰﾄﾞ処理を行う */
#define		CTU_CONT		0xfe			/* 連続ﾒｲﾝに戻らない	*/
#define		CTU_NOOPT		0xfd			/* ﾒｲﾝに戻る */
#define		CTU_OKEND		0xfc
#define		CTU_NGEND		0xfb
/*----------------------------------------------------
   関数ﾌﾟﾛﾄｺｰﾙ
----------------------------------------------------*/
EXT	void	Tu_main_process(void);
EXT	void	Tu_I_initial(void);
EXT	BYTE	Tu_I_premute(void);
EXT	BYTE	Tu_I_icreset(void);
EXT	BYTE	Tu_I_aftermute(void);
EXT	BYTE	Tu_I_frqset(void);
EXT	BYTE	Tu_I_bndpara(void);
EXT BYTE	Tu_I_pstwait(void);
EXT	BYTE 	Tu_I_pstwrite(void);
EXT	BYTE	Tu_I_updw(void);
EXT	BYTE	Tu_I_man(void);
EXT	BYTE	Tu_I_cwait(void);
EXT	BYTE	Tu_I_seekst(void);
EXT	BYTE 	Tu_I_qinfochk(void);
EXT BYTE	Tu_I_seekend(void);
EXT	BYTE	Tu_I_isrread(void);
EXT	BYTE 	Tu_I_apmst(void);
EXT	BYTE 	Tu_I_apmset(void);
EXT	BYTE 	Tu_I_apmendchk(void);
EXT	BYTE 	Tu_I_apmend(void);
EXT	BYTE	Tu_I_pstup(void);
EXT	BYTE	Tu_I_scanwait(void);
EXT	BYTE 	Tu_I_pstchk(void);

#endif
