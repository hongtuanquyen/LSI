/******************************************************************************
	クラリオン殿向け
	プロジェクト名	: 2012年AMFM_RADIOモデル
	ファイル名		: key_def.h
	機  能			: Keyモジュール データ型、定数定義ヘッダファイル
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
#ifndef		_KEY_DEF_H_
#define		_KEY_DEF_H_


#define		CKEY_TEST_KEY		1			/* ﾃｽﾄ多重ｷｰ検出有り無し */

/* -------------------------------------------------------- */
/*	共通 Keyコード 											*/
/* -------------------------------------------------------- */
#define	CKEY_ID_KEY_OFF		0	/*	キー無し		*/
#define	CKEY_ID_LCD_PWR		1	/*	PWR	 			*/
#define	CKEY_ID_LCD_AS		2	/*	AS			 	*/
#define	CKEY_ID_LCD_SRC		3	/*	MODE		 	*/
#define	CKEY_ID_LCD_VOLUP	4	/*	VOLUP			*/
#define	CKEY_ID_LCD_CH		5	/*	CH				*/
#define	CKEY_ID_LCD_VOLDW	6	/*	VOLDW			*/
#define	CKEY_ID_LCD_RIGHT	7	/*	UP				*/
#define	CKEY_ID_LCD_ALARM	8	/*	ALARM			*/
#define	CKEY_ID_LCD_LEFT	9	/*	DOWN			*/
#define	CKEY_ID_LCD_TI_SCAN	10	/*	TI/SCAN			*/

#define	CKEY_ID_LCD_TEST1	200	/*	CH＋UP＋DW  キーの3重キーコード  */
#define	CKEY_ID_LCD_TEST2	201	/*	CH＋UP＋VUP キーの3重キーコード  */
#define	CKEY_ID_LCD_TEST3	202	/*	CH＋UP＋VDW キーの3重キーコード  */

#define	CKEY_ID_LCD_TEST1_P		203	/*	CH＋UP＋DW  キーの3重キーコード  */
#define	CKEY_ID_LCD_TEST2_P		204	/*	CH＋UP＋VUP キーの3重キーコード  */
#define	CKEY_ID_LCD_TEST3_P		205	/*	CH＋UP＋VDW キーの3重キーコード  */

#define	CKEY_ID_MULTIPLEX	254	/*	多重キー						*/

#define	CKEY_ID_INVALID		255	/*	無効キー						*/

/* ------------------------------------------------------------ */
/*	Key Timer	 												*/
#define	CKEY_LONG_WAIT_TIME200		8		/*	200/25	->	4		*/
#define	CKEY_LONG_WAIT_TIME500		20		/*	500/25	->	10		*/
#define	CKEY_LONG_WAIT_TIME1000		40		/*	1000/25	->	20		*/
#define	CKEY_LONG_WAIT_TIME2000		80		/*	2000/25	->	40		*/
#define	CKEY_LONG_WAIT_TIME2000_ACCOFF	200	/*	2000/10	->	20		*/
#define	CKEY_TIME_UP				0		/*	Timerタイムアップ	*/
#define	CKEY_IN_TIMER_MAX			0xFE	/*	Timer MAX			*/
#define	CKEY_TIMER_STOP				0xFF	/*	Timer STOP			*/

#define	CKEY_IN_TIME50ms		10		/*	50/5	->	10		*/

/*	LCD Key input 											*/
#define	CKEY_LCD_DATA_BYTE			4	/*	KD1-KD30->4Byte	*/
/* -------------------------------------------------------- */
#define	CKEY_BYTE_MAX	0xFF	/*	符号無しBYTEデータ最大値	*/
#define	CKEY_WORD_MAX	0xFFFF	/*	符号無し2BYTEデータ最大値	*/
/* -------------------------------------------------------- */
/*	型定義	*/
/* Key 動作処理登録テーブル型 */
#define	KEY_PROC_TBL_SIZE	16	/*	テーブルサイズ 16Byte	*/
typedef struct{
	BYTE	code;			/*	キーコード							*/
	BYTE	comenb_on;		/*	キーON処理共通モード有効設定			*/
	BYTE	modenb_on;		/*	キーON処理モジュール内有効設定			*/
	void	(* key_on)();	/*	キーON処理アドレス						*/
	BYTE	comenb_long;	/*	キー長押し処理共通モード有効設定		*/
	BYTE	modenb_long;	/*	キー長押し処理モジュール内有効設定		*/
	void	(* key_long)();	/*	キー長押し処理アドレス					*/
	BYTE	comenb_off;		/*	キーOFF処理共通モード有効設定			*/
	BYTE	modenb_off;		/*	キーOFF処理モジュール内有効設定			*/
	void	(* key_off)();	/*	キーOFF処理アドレス					*/
}KEY_PROC_TBL;
/* Key 動作処理登録テーブル型 */
#define	KEY_CHG_TBL_SIZE	2	/*	テーブルサイズ 2Byte				*/
typedef struct{
	BYTE	com_key_code;	/*共通キーコード*/
	BYTE	local_key_code;	/*各モジュールローカルキーコード*/
}KEY_CHG_TBL;

#endif