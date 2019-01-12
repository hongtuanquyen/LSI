/******************************************************************************
	クラリオン殿向け
	プロジェクト名	: 2012年AMFM_RADIOモデル
	ファイル名		: key_main.c
	機	能			: キー入力メイン処理
					  キーモジュールローカルサブ関数
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
#define	_KEY_MAIN_C_

#include	"string.h"

#include	"../comm/common.h"
#include	"key_def.h"
#include	"key_func_ext.h"
#include	"key_sys.h"
#include	"key_main.h"

#include	"../io/IO_inc.h"
#include	"../main/ma_ext.h"
#include	"../power/pw_func_ext.h"
#include	"../tuner/tu_key.h"
#include	"../aux1/aux_key.h"
#include	"../off/off_key.h"
#include	"../disp/lcd_ext.h"
#include	"../submode/sbm_ext.h"
#include	"../submode/sbm_def.h"
#include	"../testmode/TEST_inc.h"

/*	----------------------------------------*/
/*	関数プロトタイプ宣言（ローカル関数）	*/
/*	----------------------------------------*/

/*	ＬＣＤキーの入力判定処理				*/
static	void	Key_lcd_keyin_check(void);

/*	ＡＬＡＲＭキーの入力判定処理				*/
static	void	Key_alarmkey_check(void);

/*	入力キーコードの確定を行う。			*/
static	void	Key_CodeCheck_func(void);

/*	入力キーに対応した処理の起動を行う。	*/
static	void	Key_code_process(void);

/*	LCD Keyの押されているキーコードをチェックする。*/
static	BYTE	Key_lcd_key_data_check(void);

/*	入力キーに対する動作処理を起動する。	*/
static	BYTE 	Key_code_sub_func(void);

/*	Key Data 受信処理						*/
static	BYTE	Key_DataRecive( void );

static void Key_status_check(void);
static BYTE	Key_parameter_get(void);

#if (CKEY_TEST_KEY == YES)
static BYTE	Key_code_search(BYTE , BYTE	*);
#endif
static void Key_test_key_chk(void);

static void	key_mem_keyjet(BYTE keyjet);

/******************************************************************************
	関数名		：Key_main
	引数		：無し
	戻り値		：無し
	機能概略	：Keyモジュール・メイン処理
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
void	Key_main(void)
{
	Key_lcd_keyin_check();			/*	LCD Key入力チェック処理			*/
	Key_alarmkey_check();
	Key_test_key_chk();
	Key_CodeCheck_func();			/*	Key 確定処理					*/
	Key_status_check();				/*  Key状態チェック処理				*/
	Key_code_process();				/*  Key分岐処理						*/
}

/******************************************************************************
	関数名	：Key_initial
	引  数	：無し
	戻り値	：無し
	機  能	：Keyモジュール変数初期化処理
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
void	Key_initial(void)
{
	/*	LCDキークリア */
	key_lcd_input = CKEY_ID_KEY_OFF;	/* LCD Key Code Memory area */
	key_lcd_input_Timer = CKEY_TIME_UP;

	/* キー状態チェック用変数クリア */
	key_old_key = 0;	/* 前回確定 Key Code */
	key_new_key = 0;	/* 今回確定 Key Code */
	key_now_key = 0;
	key_long_timer = 0xff;	/* Long Key 確定用タイマカウンタ */
	
	key_before_key = 0;
	key_test_code = 0;
	key_test_sta = CKEY_TSET_NOP;
}

/******************************************************************************
	関数名	：Key_lcd_keyin_check
	引  数	：無し
	戻り値	：Keyコード
	機  能	：LCD入力処理
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
static	void	Key_lcd_keyin_check(void)
{
	if (key_lcd_input_Timer == CKEY_TIME_UP)
	{
		key_lcd_input_Timer = CKEY_IN_TIME50ms;
		
		if( MIO_IN_pLCDDI() == LOW)	/*	LCD Key on ?	*/
		{
			if(Key_DataRecive() !=NG )
			{
				key_lcd_input = Key_lcd_key_data_check();
			}
			if (key_test_sta == CKEY_TSET_INPUT)
			{
				key_test_sta = CKEY_TSET_REPLY;
				memcpy(key_t_data,key_lcd_driver_data,sizeof(key_t_data));
			}
		}
		else
		{
			key_lcd_input = CKEY_ID_KEY_OFF;
			if (key_test_sta == CKEY_TSET_INPUT)
			{
				key_test_sta = CKEY_TSET_REPLY;
				memset(key_t_data,0x00,sizeof(key_t_data));
			}
		}
	}
}

/******************************************************************************
	関数名	：Key_alarmkey_check
	引  数	：無し
	戻り値	：Key コード
	機  能	：AlarmKey読み込み処理
------------------------------------------------------------------------------
[chin20120817]	新規作成
******************************************************************************/
void	Key_alarmkey_check(void)
{
	if (Ma_model_dest()== CMA_MODEL_AM_J)
	{
		fkey_alarm_key = CKEY_ID_KEY_OFF;
	}
	else
	{
		fkey_alarm_key = Pw_ret_almkey();
	}
}
/******************************************************************************
	関数名	：Key_CodeCheck_func
	引  数	：無し
	戻り値	：Key コード
	機  能	：入力キーコードの確定処理
	コメント：LCDキーコード情報を基にキー確定を行う。
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
static	void	Key_CodeCheck_func(void)
{
	BYTE	iKeyCnt;
	BYTE	iKeyCode;
	
	if (key_in_disable_timer != CKEY_TIME_UP )
	{
		fkey_alldis = ON;
		fkey_nop = ON;
	}
	fkey_prio_key = ON;
	if (key_lcd_input == CKEY_ID_LCD_PWR)
	{
		key_new_key = key_lcd_input;
	}
	else
	{
		fkey_prio_key = OFF;
		fkey_alldis = OFF;
		iKeyCnt = 0;
		iKeyCode = CKEY_ID_KEY_OFF;
		
		if (fkey_alarm_key == ON)
		{
			iKeyCode = CKEY_ID_LCD_ALARM;
			iKeyCnt++;
		}
		if (key_lcd_input != CKEY_ID_KEY_OFF)
		{
			iKeyCode = key_lcd_input;
			iKeyCnt++;
		}
		if ((iKeyCnt > 1) || (iKeyCode == CKEY_ID_MULTIPLEX))
		{
			key_new_key = CKEY_ID_KEY_OFF;
			fkey_nop = ON;						/* 多重キー押された */
		}
		else if (iKeyCnt == 1)					/* 一つキー */
		{
			key_new_key = iKeyCode;				/* キー確定 */
		}
		else									/* キー押されてない */
		{
			key_new_key = CKEY_ID_KEY_OFF;		/* key off 確定 */
			fkey_nop = OFF;
		}
	}
	
	if ( fkey_alldis == ON )						/* ALL Key禁止中 */
	{
		key_new_key = CKEY_ID_KEY_OFF;				/* キーOFFとする */
	}
	else if ((fkey_nop == ON) && (fkey_prio_key == OFF))
	{
		key_new_key = CKEY_ID_KEY_OFF;				/* キーOFFとする */
	}
	else
	{
		;
	}
}
/******************************************************************************
	関数名	：Key_lcd_key_data_check
	引  数	：無し
	戻り値	：Keyコード
	機  能	：LCD入力データチェック処理。
	コメント：LCDキーの押されているキーコードをチェックする。
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
static	BYTE	Key_lcd_key_data_check(void)
{
	BYTE	key_code_cnt;			/*	Key Driver Data Total カウンタ		*/
	BYTE	return_cd;				/*	戻り値格納変数	*/
	BYTE	key_driver_data;		/*	Key Driver Data Byte  インデックス	*/
	BYTE	key_d_bit_cnt;			/*	Key Driver Data Bit   インデックス	*/
	BYTE	loop_cnt1;				/*	Loop カウンタ						*/
	BYTE	loop_cnt2;				/*	Loop カウンタ						*/
	BYTE	input_code[3];			/*  Testの3つ多重キー対応のため、バッファ拡張 */
	BYTE	dest;
	
	dest = Ma_model_dest();
	input_code [0] = CKEY_ID_KEY_OFF;	/*	Key code Save aria	*/
	input_code [1] = CKEY_ID_KEY_OFF;	/*	Key code Save aria	*/
	input_code [2] = CKEY_ID_KEY_OFF;	/*	Key code Save aria	*/
	
	key_code_cnt = 0;
	key_d_bit_cnt = 0;
	key_lcd_driver_data[3] &= 0x03f;
	for (loop_cnt1 = 0;loop_cnt1 < CKEY_LCD_DATA_BYTE;loop_cnt1++)
	{
		key_driver_data = key_lcd_driver_data[loop_cnt1];
		for (loop_cnt2 = 0;loop_cnt2 < 8;loop_cnt2++)
		{
			if ((key_driver_data & 0x01) == 0x01)
			{
				key_code_cnt++;
				if (key_code_cnt > 3)	/* 3Key ⇒ 4Key以上なら Key無効に変更 */
				{
					break;
				}
				if (dest == CMA_MODEL_AM_J)
				{
					input_code[ key_code_cnt-1 ] = tkey_am_code_tbl[key_d_bit_cnt];
				}
				else
				{
					input_code[ key_code_cnt-1 ] = tkey_code_tbl[key_d_bit_cnt];
				}
			}
			key_driver_data = (key_driver_data >> 1);
			key_d_bit_cnt++;
		}
	}

	if (key_code_cnt == 0 )
	{
		return_cd = CKEY_ID_KEY_OFF;
	}
	else if (key_code_cnt == 1 )
	{
		return_cd = input_code[ 0 ];
	}
	#if (CKEY_TEST_KEY == YES)
	else if (key_code_cnt == 2 )
	{
		if (Key_code_search(CKEY_ID_LCD_PWR, input_code) == TRUE)
		{
			key_before_key = CKEY_ID_KEY_OFF;
			return_cd = CKEY_ID_LCD_PWR;
		}
		else
		{
			return_cd = Key_code_search(CKEY_ID_LCD_CH, input_code);
			if (return_cd == TRUE)
			{
				if (Key_code_search(CKEY_ID_LCD_RIGHT, input_code) == TRUE)
				{
					key_before_key = CKEY_ID_LCD_TEST1;
					return_cd = key_before_key;
				}
				else if (Key_code_search(CKEY_ID_LCD_LEFT, input_code) == TRUE)
				{
					key_before_key = CKEY_ID_LCD_TEST2;
					return_cd = key_before_key;
				}
				else if (Key_code_search(CKEY_ID_LCD_VOLUP, input_code) == TRUE)
				{
					key_before_key = CKEY_ID_LCD_TEST3;
					return_cd = key_before_key;
				}
				else
				{
					return_cd = CKEY_ID_MULTIPLEX;
					fkey_nop = ON;
					key_before_key = CKEY_ID_KEY_OFF;
				}
			}
			else
			{
				return_cd = CKEY_ID_MULTIPLEX;
				fkey_nop = ON;
				key_before_key = CKEY_ID_KEY_OFF;
			}
		}
	}
	else if (key_code_cnt == 3 )
	{
		if (( key_before_key >= CKEY_ID_LCD_TEST1 )&&( key_before_key <= CKEY_ID_LCD_TEST3 ))
		{
			return_cd = Key_code_search(CKEY_ID_LCD_PWR, input_code);
			if (return_cd == TRUE)
			{
				return_cd = key_before_key + 3;			/* 隠しキー確定 */
				fkey_nop = OFF;
			}
		}
		else
		{
			key_before_key = CKEY_ID_KEY_OFF;
			return_cd = CKEY_ID_MULTIPLEX;
			fkey_nop = ON;
		}
	}
	#endif
	else
	{
		return_cd = CKEY_ID_MULTIPLEX;
		fkey_nop = ON;
	}
	return ( return_cd );
}

/******************************************************************************
	関数名	：Key_code_search
	引  数	：code ⇒ サーチ元のキーコード
			：*input_key ⇒ LCDキーコード格納バッファ先頭アドレス
	戻り値	：TRUE ⇒ 指定キーコード、LCDキーコードバッファに格納されている
			：FALSE ⇒ 指定キーコード、格納されていない
	機  能	：指定ｷｰLCDキーコード格納バッファにあるかどうかサーチ処理
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
#if (CKEY_TEST_KEY == YES)
static BYTE	Key_code_search(BYTE	code, BYTE	*input_key)
{
	BYTE	iCnt;
	BYTE	iRet;

	iRet = FALSE;
	for (iCnt = 0; iCnt < 3; iCnt ++)
	{
		if ((*input_key) == code)
		{
			iRet = TRUE;
			break;
		}
		input_key++;
	}
	return(iRet);
}
#endif
/******************************************************************************
	関数名	：Key_parameter_get
	引  数	：無し
	戻り値	：OK ⇒データ取得成功
			：NG ⇒データ取得失敗
	機  能	：キー各パラメータ取得処理
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
static BYTE	Key_parameter_get(void)
{
	BYTE	work;
	BYTE	rtn_cd;

	key_com_stat = Sbm_current_mode();

	if((Sbm_mode_check() != OFF)&&(Sbm_mode_check() != CSBM_VOL))
	{
		KcodeT_p = (const KEY_CHG_TBL *)Sbm_keyCodeTable_addr_get();
		ProceT_p = (const KEY_PROC_TBL *)Sbm_key_func_addr_get();
		rtn_cd = Key_code_sub_func();	/* サブモードキーサーチ、有効キーあればOKになる */
		if (rtn_cd == OK)
		{
			return(rtn_cd);
		}
	}

	if (Pw_Power_status_get() == OFF)
	{
		KcodeT_p = (const KEY_CHG_TBL *)Off_keyCodeTable_addr_get();
		ProceT_p = (const KEY_PROC_TBL *)Off_key_func_addr_get();
		key_mode_stat = 0x00;
		rtn_cd = Key_code_sub_func();
		return(rtn_cd);
	}

	/* メインモードサーチ */
	rtn_cd = OK;
	work = Ma_current_mode();
	switch( work )
	{
		case	CMA_TUNER:		/*	TUNERモード	*/
		case	CMA_ISR:
			KcodeT_p = (const KEY_CHG_TBL *)Tu_keyCodeTable_addr_get();
			ProceT_p = (const KEY_PROC_TBL *)Tu_key_func_addr_get();
			key_mode_stat = Tu_ModeID_get();
			break;
		
		case	CMA_AUX:		/*	AUXモード */
			KcodeT_p = (const KEY_CHG_TBL *)Aux_keyCodeTable_addr_get();
			ProceT_p = (const KEY_PROC_TBL *)Aux_key_func_addr_get();
			key_mode_stat = Aux_ModeID_get();
			break;

		default:
			KcodeT_p = (const KEY_CHG_TBL *)Aux_keyCodeTable_addr_get();
			ProceT_p = (const KEY_PROC_TBL *)Aux_key_func_addr_get();
			key_mode_stat = Aux_ModeID_get();
			break;
	}

		rtn_cd = Key_code_sub_func();
	
	return(rtn_cd);
}
/******************************************************************************
	関数名	：Key_status_check
	引  数	：無し
	戻り値	：無し
	機  能	：キー状態判断処理
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
static void Key_status_check(void)
{
	BYTE	ext_mute;

	/* キー状態判断処理 */
	key_status = CKEY_KEYNOP;				/* Key 変化無し */

	if (key_old_key != key_new_key)
	{
		if (key_old_key == CKEY_ID_KEY_OFF)
		{
			key_status = CKEY_KEYON;			/* Key On */
			key_now_key = key_new_key;			/* New Key On */
			key_old_key = key_new_key;
		}
		else
		{
			key_status = CKEY_KEYOFF;			/* Key Off */
			key_now_key = key_old_key;			/* Old Key Off */
			key_old_key = CKEY_ID_KEY_OFF;		/* Key Old Off設定 */
			fkey_prio_key = OFF;				/* 優先キーOFF */
		}
		key_long_timer = 0xff;					/* key long timer初期設定 */
	}
	else if (key_long_timer == TIMER_OUT)
	{
		key_status = CKEY_KEYLONG;				/* Key 長押し */
		key_now_key = key_old_key;				/* Old Key Off */
		key_long_timer = 0xff;
	}
	#if 0
	else if ((key_long_timer != 0xff) &&
			(fkey_prio_key == FALSE))
	{
		/* 外部MUTE中優先キー以外キーOFFとする */
		ext_mute = Sbm_current_mode();				/* EXT_MUTE チェック */
		if ((ext_mute & CSBM_EMUTE) == CSBM_EMUTE)
		{
			key_status = CKEY_KEYOFF;			/* Key Off */
			key_now_key = key_old_key;			/* Old Key Off */
			key_old_key = CKEY_ID_KEY_OFF;		/* Key Old Off設定 */
			fkey_nop = ON;						/* キー無効 */
		}
	}
	#endif
	else
	{
		;
	}
}

/******************************************************************************
	関数名	：Key_code_process
	引  数	：無し
	戻り値	：Key コード
	機  能	：入力キーに対するメインモード毎の動作処理を起動する。
	コメント：LCDキー押されているキーコード処理テーブルアドレスを
			  取得し、処理を実行する。
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
static	void	Key_code_process(void)
{
	BYTE	work;

	if (key_status == CKEY_KEYNOP)			/* キー変化無し、そのまま戻る */
	{
		return;
	}

	work = Key_parameter_get();

	if (work == NG)						/* パラメータ取得できない */
	{
		return;
	}
	key_modenb &= key_mode_stat;
	key_comenb &= key_com_stat;

	if ((key_modenb != key_mode_stat)||
		(key_comenb != key_com_stat))
	{
		Key_nop();				/* キー無効 */
	}
	else if ( Key_ProcFunc != 0 )
	{
		Key_ProcFunc();		/*	Key off Key動作関数用	*/
	}
	else
	{
		Key_nop();				/* キー無効 */
	}
}

/******************************************************************************
	関数名	：Key_code_sub_func
	引  数	：無し
	戻り値	：Key コード
	機  能	：入力キーに対する動作処理を起動する。
	コメント：確定キーのモード毎の内部コードから処理関数アドレス取得する。
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
static	BYTE 	Key_code_sub_func(void)
{
	BYTE	Key_code;
	BYTE	cnt;

	Key_code = CKEY_ID_INVALID;

	/* Localキー変換 */
	for ( cnt = 0; cnt < CKEY_BYTE_MAX; cnt++ )
	{
		if ( KcodeT_p->com_key_code == CKEY_ID_INVALID )
		{
			return(NG);
		}
		else if (KcodeT_p->com_key_code == key_now_key)
		{
			Key_code = KcodeT_p->local_key_code;
			break;
		}
		else
		{
			;
		}
		KcodeT_p++;
	}

	key_modenb	= 0;
	key_comenb		= 0;
	Key_ProcFunc	= 0;

	for (cnt = 0; cnt < CKEY_BYTE_MAX; cnt++)
	{
		if (ProceT_p->code == Key_code)
		{
			if (key_status == CKEY_KEYON)
			{
				key_comenb 	= ProceT_p->comenb_on;
				key_modenb	= ProceT_p->modenb_on;
				Key_ProcFunc= ProceT_p->key_on;
			}
			else if (key_status == CKEY_KEYOFF)
			{
				key_comenb 	= ProceT_p->comenb_off;
				key_modenb	= ProceT_p->modenb_off;
				Key_ProcFunc= ProceT_p->key_off;
			}
			else
			{
				key_comenb 	= ProceT_p->comenb_long;
				key_modenb	= ProceT_p->modenb_long;
				Key_ProcFunc= ProceT_p->key_long;
			}
			return(OK);
		}
		else if ( ProceT_p->code == CKEY_ID_INVALID )
		{
			break;
		}
		else
		{
			;
		}
		ProceT_p++;
	}
	return ( NG );
}

/******************************************************************************
	関数名	：Key_DataRecive
	引  数	：無し
	戻り値	：無し
	機  能	：Key Data 受信処理
------------------------------------------------------------------------------
日付		担当者		修正内容
2012/07/26	東海林		新規作成
******************************************************************************/
static	BYTE	Key_DataRecive( void )
{
	BYTE	mode;
	memset(&key_lcd_driver_data[0],0x00,sizeof(key_lcd_driver_data));
	
	MIO_OUT_pLCDCE( LOW );		/*	LCD CE    "L"	*/
	IO_lcd_ccb( 0x43);		/*	CCBアドレス送信	*/
	MIO_OUT_pLCDCE( HIGH );		/*	LCD CE  "H"		*/
	mode = IO_lcd_receive( &key_lcd_driver_data[0] );
	MIO_OUT_pLCDCE( LOW );		/*	LCD CE    "L"	*/
	return(mode);
}

#define		TEST_KEY_POWER		0x0080
#define		TEST_KEY_AS			0x0040
#define		TEST_KEY_MODE		0x0020
#define		TEST_KEY_VUP		0x0010
#define		TEST_KEY_CH			0x0008
#define		TEST_KEY_VDW		0x0004
#define		TEST_KEY_UP			0x0002
#define		TEST_KEY_DW			0x0001
#define		TEST_KEY_TISC		0x8000
#define		TEST_KEY_ALARM		0x4000

void	key_mem_keyjet(BYTE keyjet)
{
	switch(keyjet)
	{
		case CKEY_ID_LCD_PWR:
			key_test_code |= TEST_KEY_POWER;
			break;
		
		case CKEY_ID_LCD_AS:
			key_test_code |= TEST_KEY_AS;
			break;
		
		case CKEY_ID_LCD_SRC:
			key_test_code |= TEST_KEY_MODE;
			break;
		
		case CKEY_ID_LCD_VOLUP:
			key_test_code |= TEST_KEY_VUP;
			break;
		
		case CKEY_ID_LCD_CH:
			key_test_code |= TEST_KEY_CH;
			break;
		
		case CKEY_ID_LCD_VOLDW:
			key_test_code |= TEST_KEY_VDW;
			break;
		
		case CKEY_ID_LCD_RIGHT:
			key_test_code |= TEST_KEY_UP;
			break;
		
		case CKEY_ID_LCD_TI_SCAN:
			key_test_code |= TEST_KEY_TISC;
			break;
		
		case CKEY_ID_LCD_LEFT:
			key_test_code |= TEST_KEY_DW;
			break;
		
		case CKEY_ID_LCD_ALARM:
			key_test_code |= TEST_KEY_ALARM;
			break;
		
		default :
			break;
	}
}

void	Key_test_key_chk(void)
{
	BYTE	key_driver_data;
	BYTE	key_d_bit_cnt;
	BYTE	loop_cnt1;				/*	Loop カウンタ						*/
	BYTE	loop_cnt2;				/*	Loop カウンタ						*/
	BYTE	test_jet;
	BYTE	test_rpy_bur[4];
	
	key_t_data[3] &= 0x03f;
	key_d_bit_cnt = 0;
	key_driver_data = 0;
	
	if (key_test_sta == CKEY_TSET_REPLY)
	{
		key_test_sta = CKEY_TSET_NOP;
		
		key_test_code = 0x0000;
		
		/*---- パネルキー ----*/
		for (loop_cnt1 = 0;loop_cnt1 < CKEY_LCD_DATA_BYTE;loop_cnt1++)
		{
			key_driver_data = key_t_data[loop_cnt1];
			for (loop_cnt2 = 0;loop_cnt2 < 8;loop_cnt2++)
			{
				if ((key_driver_data & 0x01) == 0x01)
				{
					test_jet = tkey_code_tbl[key_d_bit_cnt];
					key_mem_keyjet(test_jet);
				}
				key_driver_data = (key_driver_data >> 1);
				key_d_bit_cnt++;
			}
		}
		
		/*---- Alarmキー ----*/
		if (fkey_alarm_key == TRUE )
		{
			key_mem_keyjet(CKEY_ID_LCD_ALARM);
		}
		
		test_rpy_bur[0] = 0xFB;
		test_rpy_bur[1] = 0x03;
		test_rpy_bur[2] = (BYTE)(key_test_code);
		test_rpy_bur[3] = (BYTE)(key_test_code>>8);
		
		Test_data_reply(CTEST_MODE_KEY_TEST,0x2C,test_rpy_bur,4);
	}
}


#undef _KEY_MAIN_C_