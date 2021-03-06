/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: ma_sys.c
		機能			: Mainｼｽﾃﾑﾒｯｾｰｼﾞ受付処理処理
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]	新規作成
[sun060712] USBﾓｰﾄﾞ対応
******************************************************************************/
#define		_MA_SYS_C_

#include	"string.h"
#include	"../model.h"
#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#include	"../power/pw_main.h"

#if (_MODEL_EEPROM_ == YES)
#include	"../eeprom/eeprom_ext.h"
#endif

#include	"ma_mode.h"
#include	"ma_sys.h"
#include	"ma_tim.h"
#include	"ma_ext.h"

static void Ma_initial(void);
static void Ma_main(void);
static void Ma_mode_initialset(void);

/******************************************************************************
.. 		関数名  ：Ma_sys_message
		引  数	：Systemﾒｯｾｰｼﾞ
		戻り値	：無し
		機  能	：MainSystemﾒｯｾｰｼﾞ受付処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051129] 新規作成
[sun070410] ModePowerOFF処理追加
******************************************************************************/
void	Ma_sys_message(BYTE	msg)
{
	switch(msg)
	{
		case	CMSG_MAIN:
			Ma_main();
			break;
		case	CMSG_INITIAL:
			Ma_initial();
			break;
		case	CMSG_PWROFF:
		case	CMSG_MAINCNT_OFFREQ:
		case	CMSG_MAINCNT_OFF:
			Ma_mode_pwroff();
			break;
		default:
			break;
	}
}

/******************************************************************************
.. 		関数名  ：Ma_main
		引  数	：無し
		戻り値	：無し
		機  能	：Mainﾓｼﾞｭｰﾙﾒｲﾝ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051129] 新規作成
******************************************************************************/
static void Ma_main(void)
{
	Ma_mode_main();					/* Mainﾓｰﾄﾞ切替監視処理 */
	Ma_tim_main();					/* ｼｽﾃﾑﾍﾞｰｽﾀｲﾏｶｳﾝﾄ処理 */
}
/******************************************************************************
.. 		関数名  ：Ma_10ms_timer
		引  数	：無し
		戻り値	：無し
		機  能	：Mainﾓｼﾞｭｰﾙ10ms割込みﾀｲﾏ処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun060106] 新規作成
******************************************************************************/
void Ma_10ms_timer(void)
{
	if (ma_mutetimer > TIMER_OUT)
	{
		ma_mutetimer--;
	}
}

/******************************************************************************
.. 		関数名  ：Ma_initial
		引  数	：なし
		戻り値	：無し
		機  能	：初期設定読み込み
				：内部変数初期設定
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128] とりあえず設定して置く
[sun060621] HEX毎に仕向け設定するように変更
[sun090403] MainLastﾓｰﾄﾞはTunerに初期設定
[sun100529] 初期設定仕様対応
[sun100610] 初期設定仕様Port表と同じに変更
******************************************************************************/
static void Ma_initial(void)
{
	/*------------------------------
	   Mainﾓｰﾄﾞ初期設定
	------------------------------*/
	memset(&ma_mode_cont[0],CMA_NOCONN,sizeof(ma_mode_cont));
	ma_mode_cont[CMA_ID_TU] = CMA_TUNER;
	ma_mode_cont[CMA_ID_AUX1] = CMA_AUX;
	
	if ((ma_ini_data.fac1[0] == 0x5A)&&(ma_ini_data.fac1[1] == 0xA5)&&
		(ma_ini_data.fac2[0] == 0x5A)&&(ma_ini_data.fac2[1] == 0xA5)&&
		(ma_ini_data.area >= 0x00)&&(ma_ini_data.area <= 0x07))
	{
		if ((ma_ini_data.area == 0x00))		/* 仕向けNG */
		{
			ma_ini_data.ini_status = CMA_INIT_STA_AREA_NG;
		}
		else								/* 仕向けOK */
		{
			/* 時計とアラーム範囲チェック */
			if ((ma_ini_data.clock >= 0x00)&&(ma_ini_data.clock <= 0x01)&&
			    (ma_ini_data.alarm >= 0x00)&&(ma_ini_data.alarm <= 0x01))
			{
				ma_ini_data.ini_status = CMA_INIT_STA_ALL_OK;
			}
			else
			{
				ma_ini_data.ini_status = CMA_INIT_STA_CKAL_NG;
			}
		}
		ma_dest = 0x05;
	}
	else	/* 初期化処理行う */
	{
		ma_dest = 0;
		ma_ini_data.ini_status = CMA_INIT_STA_ALL_NG;
	}
	
	switch(ma_ini_data.ini_status)
	{
		case CMA_INIT_STA_ALL_OK:
			if ((ma_dest >= 2)&&(ma_dest <= 7))
			{
				/* ILL_DET 入力 */
				MIO_CHG_pILL_DET(IN);
	
				/* AlarmKey検出許可 */
				if((ma_ini_data.clock == 0x01)&&(ma_ini_data.alarm == 0x01))
				{
					Io_alarmkey_enable();
				}
			}
			Ma_mode_initialset();
			break;
		case CMA_INIT_STA_CKAL_NG:
			ma_ini_data.clock = 0x01;
			ma_ini_data.alarm = 0x01;
			/*  */
			if ((ma_dest >= 2)&&(ma_dest <= 7))
			{
				/* ILL_DET 入力 */
				MIO_CHG_pILL_DET(IN);
				
				/* AlarmKey検出許可 */
				Io_alarmkey_enable();
			}
			Ma_mode_initialset();
			break;
		
		case CMA_INIT_STA_ALL_NG:
			/* 初期化処理 */
				/* 5AA5 00 FF FF */
				/* AM領域 ：全0xFF */
				/* FM1領域：全0xFF */
				/* FM2領域：全0xFF */
				/* 5AA5 */
			EEP_Initial_Req();
			break;
		
		case CMA_INIT_STA_AREA_NG:
		default:
			break;
	}
	
	
	/*------------------------------
	   ﾍﾞｰｽﾀｲﾏｶｳﾝﾄ用変数初期設定
	------------------------------*/
	ma_tim_4ms_cnt = CTIM_4MS_CNT;
	ma_tim_5ms_cnt = CTIM_5MS_CNT;
	ma_tim_10ms_cnt = CTIM_10MS_CNT - 1;
	ma_tim_25ms_cnt = CTIM_25MS_CNT + 2;
	ma_tim_50ms_cnt = CTIM_50MS_CNT;
	ma_tim_100ms_cnt = CTIM_100MS_CNT + 1;
	ma_tim_500ms_cnt = CTIM_500MS_CNT + 3;
	ma_tim_1s_cnt = CTIM_1S_CNT - 1;
}

static void Ma_mode_initialset(void)
{
	fpw_power_key = OFF;
	ma_mode[CMA_TARGET_MD].id = CMA_ID_TU;
	ma_mode[CMA_TARGET_MD].cond.mode = CMA_TUNER;
	Ma_modetbl_srh(&ma_mode[CMA_TARGET_MD]);
	ma_mode[CMA_CUR_MD] = ma_mode[CMA_TARGET_MD];
	ma_mode[CMA_LAST_MD].id = CMA_ID_TU;
	ma_mode[CMA_LAST_MD].cond.mode = CMA_TUNER;
	Ma_modetbl_srh(&ma_mode[CMA_LAST_MD]);
}
#undef		_MA_SYS_C_
