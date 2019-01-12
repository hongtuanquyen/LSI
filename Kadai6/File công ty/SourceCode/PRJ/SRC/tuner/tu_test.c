/********************************************************************************/
/*		tu_test.c																*/
/*																				*/
/*		プロジェクト名：クラリオン殿向け 2011モデル								*/
/*		ファイル名：tu_test.c													*/
/*		モジュール名：Tuner														*/
/*		機能：																	*/
/*																				*/
/*																				*/
/*																				*/
/*			修正履歴	【年月日】	【Version】	【名前】 	【説明】			*/
/*------------------------------------------------------------------------------*/
/*						2010.07.06		0.0			岳		新規作成			*/
/********************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_

#define		_TU_TEST_C

#include	<string.h>
#include	"../comm/common.h"
#include	"../main/ma_inc.h"
#include	"../disp/LCD_inc.h"
#include	"../submode/SBM_inc.h"
#include	"../testmode/TEST_inc.h"
#include	"tu_def.h"
#include	"tu_event.h"
#include	"tu_ext.h"
#include	"tu_main.h"
#include	"tu_item.h"
#include	"tu_io.h"
#include	"tu_test.h"
#include	"tu_eeprom.h"

static	void	Tu_Diag_NgSet(void);
static	void	Tu_Diag_Idle(void);
static	void	Tu_Diag_Smeter(void);

static void	(* const tu_diag_mainfunctbl[])( void ) = {
	Tu_Diag_Idle,			/* 停止状態 				 */
	Tu_Diag_Smeter,			/* FM Sﾒｰﾀ要求				 */
	Tu_Diag_Smeter,			/* AM Sﾒｰﾀ要求				 */
	Tu_Diag_Smeter,			/* FM Sﾒｰﾀ基準電圧調整(20dB) */
	Tu_Diag_Smeter,			/* AM Sﾒｰﾀ基準電圧調整(30dB) */
	Tu_Diag_Smeter,			/* FM ﾚﾍﾞﾙ変換値取得		 */
	Tu_Diag_Smeter,			/* AM ﾚﾍﾞﾙ変換値取得		 */
};

enum
{
	CTU_DIAG_NOP,				
	CTU_DIAG_FMSMETER_REQ,		//FM Sﾒｰﾀ要求
	CTU_DIAG_AMSMETER_REQ,		//AM Sﾒｰﾀ要求
	CTU_DIAG_FMLST_REQ,			//FM Sﾒｰﾀ基準電圧調整(20dB)
	CTU_DIAG_AMLST_REQ,			//AM Sﾒｰﾀ基準電圧調整(20dB)
	CTU_DIAG_FMLVCNV_REQ,		//FM Sﾒｰﾀﾚﾍﾞﾙ変換値取得
	CTU_DIAG_AMLVCNV_REQ,		//AM Sﾒｰﾀﾚﾍﾞﾙ変換値取得
};

/***	送信時固定データ	***/
#define		CTU_TEST_SND_STA_DT			0x2c
#define		CTU_TEST_SND_STA_DT1		0xfb
#define		CTU_TEST_SND_STA_DT2		0x23
#define		CTU_TEST_SND_STA_DT3_AM		0x00
#define		CTU_TEST_SND_STA_DT3_FM		0x01
#define		CTU_TEST_SND_STA_LEN		6

#define		CTU_TEST_ATMSND_DT			0x28
#define		CTU_TEST_ATMSND_DB_DT1		0x11
#define		CTU_TEST_ATMSND_SM_DT1		0x00
#define		CTU_TEST_ATMSND_SM_LEN		3
#define		CTU_TEST_ATMSND_20DB_LEN	2
#define		CTU_TEST_ATMSND_40DB_LEN	2

/***	ATOMIC系コマンド	***/
#define		CTU_TEST_ATM_FMSM		0
#define		CTU_TEST_ATM_AMSM		1
#define		CTU_TEST_ATM_FM20DB		4
#define		CTU_TEST_ATM_AM30DB		5
#define		CTU_TEST_ATM_FM40DB		6
#define		CTU_TEST_ATM_AM50DB		7

/***	ATMIC系シーケンス		***/
#define		CTU_SMETER_CHECK	0x01
#define		CTU_SMEEP_WRITE		0x02
#define		CTU_DATA_REPLY		0x03

static	BYTE	tu_diag_req;
static	BYTE	tu_diag_seq;
static	BYTE	tu_diag_cnt;

static	BYTE	tu_sm_chk_ok;
#if 0	/*未使用のためコメントアウト*/
BYTE	tu_test_AM1db;
BYTE	tu_test_AM_20db;
BYTE	tu_test_AM_40db;

BYTE	tu_test_FM1db;
BYTE	tu_test_FM_20db;
BYTE	tu_test_FM_40db;
#endif
/******************************************************************************
.. 		関数名  ：Tu_cmd_func
		引  数	：RcvCmd,
				  *RcvBuf
				  Lenth
		戻り値	：無し
		機  能	：テストモード受付処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120926] 新規作成
******************************************************************************/
void	Tu_cmd_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Lenth)
{
	BYTE destno;
	destno = Tu_dest_chk();

	switch(RcvCmd)
	{
		case	CTEST_MODE_TUNER_DIR_PCH:
			Tu_test_dirpch(RcvBuf);
			break;
			
		case	CTEST_MODE_TUNER_SEEKUP:
			Tu_e_seek_updw(CTU_UP);
			break;
			
		case	CTEST_MODE_TUNER_SEEKDW:
			Tu_e_seek_updw(CTU_DOWN);
			break;
			
		case	CTEST_MODE_TUNER_DIR_NCH:
			Tu_test_dirnch(RcvBuf);
			break;
			
		case	CTEST_MODE_TUNER_LO_DX:
			Tu_test_dxlochg(RcvBuf);
			break;
			
		case	CTEST_MODE_TUNER_INI_PCH:
			Tu_intset_pchtbl(destno);					/* PCHテーブル設定 */
			Tu_SaveEepData(CTU_EEPWR_PST,tu_eepdata);	/* EEPROM書き込みデータ設定 */
			EEP_Initial_para_Req(tu_eepdata,0xFF);			/* EEPROM書き込み要求 */
			break;
			
		case	CTEST_MODE_TUNER_GET_STA:
			Tu_test_stareply();
			break;
			
		case	CTEST_MODE_ATMIC_IC:
			Tu_test_atomicget(RcvBuf,destno);
			break;
			
		default:
			break;
	}
}

/******************************************************************************
.. 		関数名  ：Tu_test_dirpch
		引  数	：*RcvBuf
		戻り値	：無し
		機  能	：ダイレクトPCH読み込み処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120926] 新規作成
******************************************************************************/
void	Tu_test_dirpch(BYTE *RcvBuf)
{
	if ((RcvBuf[0] > 0) && (RcvBuf[0] <= CTU_PRESET))
	{
		Tu_job_clear();
		Tu_pno_set(RcvBuf[0]);
		Tu_presetcall();
		tu_m_mode = CTU_M_PSTRD;					/* Preset readﾓｰﾄﾞｾｯﾄ */
		Lcd_disp_base(CMA_TUNER);
	}
}

/******************************************************************************
.. 		関数名  ：Tu_test_dirnch
		引  数	：*RcvBuf
		戻り値	：無し
		機  能	：ダイレクト周波数読み込み処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120926] 新規作成
******************************************************************************/
void	Tu_test_dirnch(BYTE *RcvBuf)
{
	BYTE	a,b,c,d,e,f;
	BYTE	bandmd;
	WORD	freq;
	WORD	step;
	WORD	min_freq;
	WORD	max_freq;
	
	bandmd = Tu_band_chk();
	step = tu_coef.step;
	
	if (bandmd == CTU_FM)		/* FM STEP数 */
	{
		step = (step/10);				/* step 10kHzに変換 */
	}
		
	min_freq = tu_coef.min_freq;
	max_freq = ((tu_coef.max_ch - tu_coef.min_ch) * step) + min_freq;
	
	/* 常用数値変換 */
	if (bandmd == CTU_AM)
	{
		a = RcvBuf[0] / 0x10;
		b = RcvBuf[0] % 0x10;
		c = RcvBuf[1] / 0x10;
		d = RcvBuf[1] % 0x10;
		#if 0
		freq = (LONG)(((LONG)a * 1000) + (b * 100) + (c * 10) + d);
		#else
		freq = (WORD)(((LONG)a * 1000) + (b * 100) + (c * 10) + d);
		#endif
		
	}
	else
	{
		a = RcvBuf[0] / 0x10;
		b = RcvBuf[0] % 0x10;
		c = RcvBuf[1] / 0x10;
		d = RcvBuf[1] % 0x10;
		e = RcvBuf[2] / 0x10;
		f = RcvBuf[2] % 0x10;
		#if 0
		freq = (LONG)(((LONG)b * 10000) + ((LONG)c * 1000) + (d * 100) + (e * 10) + f);
		#else
		freq = (WORD)(((LONG)b * 10000) + ((LONG)c * 1000) + (d * 100) + (e * 10) + f);
		#endif
	}
	
	if((freq >= min_freq) && ( freq <= max_freq))
	{
		tu_ch.freq = ((freq - min_freq) / step) + tu_coef.min_ch;
		Tu_lastwrite();
		Tu_job_clear();
		tu_m_mode = CTU_M_LSTCAL;
		Sbm_mode_cancel(CMA_TUNER);
		Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* すべて時限表示ｷｬﾝｾﾙ */	
		Lcd_disp_base(CMA_TUNER);
	}
}

/******************************************************************************
.. 		関数名  ：Tu_test_dxlochg
		引  数	：*RcvBuf
		戻り値	：無し
		機  能	：DX/LO切替処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120926] 新規作成
******************************************************************************/
void	Tu_test_dxlochg(BYTE *RcvBuf)
{
	switch(RcvBuf[0])
	{
		case 1:
			tu_test_dxlo = CTU_TEST_LOCAL;					/* LOCAL設定要求 */
			break;
			
		case 0:
			tu_test_dxlo = CTU_TEST_DX;						/* DX設定要求 */
			break;
			
		default:
			break;
	}
}

/******************************************************************************
.. 		関数名  ：Tu_test_stareply
		引  数	：無し
		戻り値	：無し
		機  能	：Tunerステータス返送処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu121105] 新規作成
******************************************************************************/
void	Tu_test_stareply(void)
{
	BYTE	bandmd;
	BYTE	iRet;
	BYTE	bcddata[3];
	BYTE	test_reply[CTU_TEST_SND_STA_LEN];

	bandmd = Tu_band_chk();
	memset( &bcddata[0],0,3);
	memset( &test_reply[0],0,CTU_TEST_SND_STA_LEN);

	Tu_dispfreqset();					/* 周波数表示用変換 */
	iRet = Cm_lib_wHEXtoBCD(tu_dispfreq,bcddata);	/* HEX→BCD変換 */
	
	test_reply[0] = CTU_TEST_SND_STA_DT1;
	test_reply[1] = CTU_TEST_SND_STA_DT2;
	if(bandmd == CTU_FM)
	{
		test_reply[2] = CTU_TEST_SND_STA_DT3_FM;
		test_reply[3] = bcddata[0];
		test_reply[4] = bcddata[1];
		test_reply[5] = bcddata[2];
	}
	else
	{
		test_reply[2] = CTU_TEST_SND_STA_DT3_AM;
		test_reply[3] = bcddata[1];
		test_reply[4] = bcddata[2];
	}
	Test_data_reply(CTEST_MODE_TUNER_GET_STA, CTU_TEST_SND_STA_DT, test_reply, CTU_TEST_SND_STA_LEN);
}

/******************************************************************************
.. 		関数名  ：Tu_test_atomicget
		引  数	：destno
		戻り値	：無し
		機  能	：ATOMICデータ取得処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu120926] 新規作成
******************************************************************************/
void	Tu_test_atomicget(BYTE *RcvBuf,BYTE destno)
{
	BYTE	invalid;
	invalid = Tu_test_invalidchk(RcvBuf[1]);
	
	if(invalid != TRUE)
	{
		if( tu_test_mode == 0xFF )
		{
			tu_test_mode = 0xFE;
		}
		tu_i_seq = 0;
		tu_diag_cnt = 0;
		tu_diag_seq  = CTU_SMETER_CHECK;
		
		tu_sm_chk_ok = NG;
		tu_qinfochk_num = CTU_TEST_NUM;
		
		tu_sme_cnt = 0;
		tu_if_cnt = 0;
		tu_usn_cnt = 0;
		tu_wam_cnt = 0;
		
		tu_sme_ave[0] = 0;
		tu_sme_ave[1] = 0;
		tu_if_ave[0] = 0;
		tu_if_ave[1] = 0;
		tu_usn_ave[0] = 0;
		tu_usn_ave[1] = 0;
		tu_wam_ave[0] = 0;
		tu_wam_ave[1] = 0;
		switch(RcvBuf[1])
		{
			case CTU_TEST_ATM_FMSM:
				tu_diag_req = CTU_DIAG_FMSMETER_REQ;
				break;
				
			case CTU_TEST_ATM_AMSM:
				tu_diag_req = CTU_DIAG_AMSMETER_REQ;
				break;
				
			case CTU_TEST_ATM_FM20DB:
				tu_diag_req = CTU_DIAG_FMLST_REQ;
				break;
				
			case CTU_TEST_ATM_AM30DB:
				tu_diag_req = CTU_DIAG_AMLST_REQ;
				break;
				
			case CTU_TEST_ATM_FM40DB:
				tu_diag_req = CTU_DIAG_FMLVCNV_REQ;
				break;
				
			case CTU_TEST_ATM_AM50DB:
				tu_diag_req = CTU_DIAG_AMLVCNV_REQ;
				break;
				
			default:
				/*	無効操作	*/
				Tu_Diag_NgSet();
				break;
		}
	}
	else
	{
		/*	無効操作	*/
		Tu_Diag_NgSet();
	}
}

/******************************************************************************
.. 		関数名  ：Tu_test_invalidchk
		引  数	: atomic_req
		戻り値	：無し
		機  能	：無効コマンド判定処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu121101] 新規作成
******************************************************************************/
BYTE	Tu_test_invalidchk(BYTE atomic_req)
{
	BYTE	bandmd;
	BYTE	ret;

	bandmd = Tu_band_chk();
	ret = FALSE;

	switch(atomic_req)
	{
		case CTU_TEST_ATM_FMSM:
		case CTU_TEST_ATM_FM20DB:
		case CTU_TEST_ATM_FM40DB:
			if(bandmd == CTU_AM)
			{
				ret = TRUE;
			}
			break;
			
		case CTU_TEST_ATM_AMSM:
		case CTU_TEST_ATM_AM30DB:
		case CTU_TEST_ATM_AM50DB:
			if(bandmd == CTU_FM)
			{
				ret = TRUE;
			}
			break;
			
		default:
			ret = TRUE;
			break;
	}
	return(ret);
}

/******************************************************************************
.. 		関数名  ：Tu_Diag_Smeter
		引  数	:
		戻り値	：無し
		機  能	：無効コマンド判定処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu121101] 新規作成
******************************************************************************/
static	void	Tu_Diag_Smeter(void)
{
	switch(tu_diag_seq)
	{
		case CTU_SMETER_CHECK:
			Tu_Smeter_Check();
			break;
			
		case CTU_SMEEP_WRITE:
			Tu_SmEep_Write();
			break;
			
		case CTU_DATA_REPLY:
			Tu_Data_Reply();
			break;
			
		default:
			Tu_Diag_NgSet();
			break;
	}
}

/******************************************************************************
.. 		関数名  ：Tu_Smeter_Check
		引  数	:
		戻り値	：無し
		機  能	：無効コマンド判定処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu121101] 新規作成
******************************************************************************/
void	Tu_Smeter_Check(void)
{
	#if 0
	if(Tu_I_qinfochk() != CTU_I_NOTEND)
	#else
	if(tu_sm_chk_ok == OK)
	{
		tu_sm_chk_ok = NG;
		tu_ic_data.msa.subaddr = 0xe0;
		tu_ic_data.length = 5;
		Tu_data_read();/* SD値取り込み */
		if ( Tu_Average_QInfo(tu_ic_data.rxbuf) == OK)
		#endif
		{
			tu_diag_seq = CTU_DATA_REPLY;
			switch(tu_diag_req )
			{
				case CTU_DIAG_AMLST_REQ:
					tu_chkcef[CTU_AM].sm_20db = tu_ic_data.rxbuf[1];
					tu_level_AM1db = Tu_Level_db_Chg(tu_chkcef[CTU_AM].sm_20db,tu_chkcef[CTU_AM].sm_40db);
					#if (_MODEL_EEPROM_ == 1)
					tu_diag_seq = CTU_SMEEP_WRITE;
					#endif
					break;
					
				case CTU_DIAG_AMLVCNV_REQ:
					tu_chkcef[CTU_AM].sm_40db = tu_ic_data.rxbuf[1];
					tu_level_AM1db = Tu_Level_db_Chg(tu_chkcef[CTU_AM].sm_20db,tu_chkcef[CTU_AM].sm_40db);
					#if (_MODEL_EEPROM_ == 1)
					tu_diag_seq = CTU_SMEEP_WRITE;
					#endif
					break;
							
				case CTU_DIAG_FMLST_REQ:
					tu_chkcef[CTU_FM].sm_20db = tu_ic_data.rxbuf[1];
					tu_level_FM1db = Tu_Level_db_Chg(tu_chkcef[CTU_FM].sm_20db,tu_chkcef[CTU_FM].sm_40db);
					#if (_MODEL_EEPROM_ == 1)
					tu_diag_seq = CTU_SMEEP_WRITE;
					#endif
					break;
							
				case CTU_DIAG_FMLVCNV_REQ:
					tu_chkcef[CTU_FM].sm_40db = tu_ic_data.rxbuf[1];
					tu_level_FM1db = Tu_Level_db_Chg(tu_chkcef[CTU_FM].sm_20db,tu_chkcef[CTU_FM].sm_40db);
					#if (_MODEL_EEPROM_ == 1)
					tu_diag_seq = CTU_SMEEP_WRITE;
					#endif
					break;
						
				default:
					break;
			}
			tu_qinfochk_num = CTU_SEEK_NUM;		/* 初期値設定 */
		}
	}
}

/******************************************************************************
.. 		関数名  ：Tu_SmEep_Write
		引  数	:
		戻り値	：無し
		機  能	：無効コマンド判定処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu121101] 新規作成
******************************************************************************/
void	Tu_SmEep_Write(void)
{
	BYTE tempbuf[16];
	BYTE calsum;
	BYTE i;

	if( tu_diag_cnt <= 3 )
	{
		#if (_MODEL_EEPROM_ == 1)	/* C12 */
		if(Tu_ReadEepRq(tempbuf,0,16) == TRUE)
		{
			switch(tu_diag_req )
			{
				case CTU_DIAG_AMLST_REQ:
					tempbuf[CTU_EEP_ADR_AMSMETER20DB] = tu_chkcef[CTU_AM].sm_20db;
					break;
					
				case CTU_DIAG_AMLVCNV_REQ:
					tempbuf[CTU_EEP_ADR_AMSMETER40DB] = tu_chkcef[CTU_AM].sm_40db;
					break;
					
				case CTU_DIAG_FMLST_REQ:
					tempbuf[CTU_EEP_ADR_FMSMETER20DB] = tu_chkcef[CTU_FM].sm_20db;
					break;
					
				case CTU_DIAG_FMLVCNV_REQ:
					tempbuf[CTU_EEP_ADR_FMSMETER40DB] = tu_chkcef[CTU_FM].sm_40db;
					break;
					
				default:
					/*	無効操作	*/
					Tu_Diag_NgSet();
					break;
				
			}
			/* ﾁｪｯｸｻﾑ値を設定します(調整値は全てｱﾄﾞﾚｽ0x00～0x0eの範囲) */
			calsum	= 0;					/* 16Byte SUM計算				  */
			for(i=0; i<15; i++)
			{
				calsum	+= tempbuf[i];
			}
			tempbuf[15] = calsum;
			if(Tu_WriteEepRq(tempbuf,0,16) == TRUE)
			{
				tu_diag_seq = CTU_DATA_REPLY;
			}
			else
			{
				tu_diag_cnt ++;
			}
		}
		else
		{
			tu_diag_cnt ++;
		}
		#endif		/* C12 */
	}
	else
	{
		/*	EEPROM読み出し失敗●	*/
		Tu_Diag_NgSet();
	}
}

/******************************************************************************
.. 		関数名  ：Tu_Data_Reply
		引  数	:
		戻り値	：無し
		機  能	：無効コマンド判定処理
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[matu121101] 新規作成
******************************************************************************/
void	Tu_Data_Reply(void)
{
	BYTE	test_reply[3];
	memset( &test_reply[0],0,3);
	
	switch(tu_diag_req )
	{
		case CTU_DIAG_FMSMETER_REQ:
		case CTU_DIAG_AMSMETER_REQ:
			test_reply[0] = CTU_TEST_ATMSND_SM_DT1;
			test_reply[1] = tu_test_value;
			test_reply[2] = tu_ic_data.rxbuf[1];
			Test_data_reply(CTEST_MODE_ATMIC_IC, CTU_TEST_ATMSND_DT, test_reply, CTU_TEST_ATMSND_SM_LEN);
			Tu_Diag_NgSet();
			break;

		case CTU_DIAG_FMLST_REQ:
			test_reply[0] = CTU_TEST_ATMSND_DB_DT1;
			test_reply[1] = tu_chkcef[CTU_FM].sm_20db;
			Test_data_reply(CTEST_MODE_ATMIC_IC, CTU_TEST_ATMSND_DT, test_reply, CTU_TEST_ATMSND_20DB_LEN);
			Tu_Diag_NgSet();
			break;
			
		case CTU_DIAG_AMLST_REQ:
			test_reply[0] = CTU_TEST_ATMSND_DB_DT1;
			test_reply[1] = tu_chkcef[CTU_AM].sm_20db;
			Test_data_reply(CTEST_MODE_ATMIC_IC, CTU_TEST_ATMSND_DT, test_reply, CTU_TEST_ATMSND_20DB_LEN);
			Tu_Diag_NgSet();
			break;
			
		case CTU_DIAG_FMLVCNV_REQ:
			test_reply[0] = CTU_TEST_ATMSND_DB_DT1;
			test_reply[1] = tu_chkcef[CTU_FM].sm_40db;
			Test_data_reply(CTEST_MODE_ATMIC_IC, CTU_TEST_ATMSND_DT, test_reply, CTU_TEST_ATMSND_40DB_LEN);
			Tu_Diag_NgSet();
			break;
			
		case CTU_DIAG_AMLVCNV_REQ:
			test_reply[0] = CTU_TEST_ATMSND_DB_DT1;
			test_reply[1] = tu_chkcef[CTU_AM].sm_40db;
			Test_data_reply(CTEST_MODE_ATMIC_IC, CTU_TEST_ATMSND_DT, test_reply, CTU_TEST_ATMSND_40DB_LEN);
			Tu_Diag_NgSet();
			break;
			
		default:
			/*	無効操作	*/
			Tu_Diag_NgSet();
			break;
	}
}

WORD	Tu_Chg_WORD( BYTE * Byte_Data )
{
	CHG_WORD  tmp;						/* 変換用構造体定義               */

	tmp.byte[0] = *Byte_Data;			/* BYTEをWORDに変換            */
	tmp.byte[1] = *(Byte_Data+1);
	return(tmp.word);
}


void	Tu_Add_WORD( BYTE * Byte_Data , WORD Add_Data)
{
	CHG_WORD  tmp;						/* 変換用構造体定義               */
	
	tmp.byte[0] = *Byte_Data;			/* BYTEをWORDに変換            */
	tmp.byte[1] = *(Byte_Data+1);
	tmp.word += Add_Data;				/* ﾃﾞｰﾀを加算する                 */
	*Byte_Data = tmp.byte[0];			/* 加算したﾃﾞｰﾀを戻す             */
	*(Byte_Data+1)= tmp.byte[1];
}

BYTE	Tu_Level_db_Chg( BYTE d_Level20 , BYTE d_Level40 )
{
	BYTE	temp = 0x00U;
	
	//--------------------------
	// 1dB当たりのLEVEL値を計算 
	//--------------------------
	if( d_Level40 < d_Level20 ){							// 20db Level値の方が小さい場合、
		return(0x01U);										// 計算結果は「１」にする
	}
	
	//====================================================================
	// 計算方法 
	//--------------------------------------------------------------------
	// 2008.10.06-1 コメント追加
	// FM/AMで計算式が違う
	// 1)FM時
	//   仕様上の式 ⇒  ((( 40dBのS-METER値 － 20dBのS-METER値 )×１０) ÷ (40dB － 20dB)) 
	//   少し簡単に ⇒  ((( 40dBのS-METER値 － 20dBのS-METER値 )×１０) ÷ (20))
	//   もう少し   ⇒  ((( 40dBのS-METER値 － 20dBのS-METER値 )×１) ÷ (2))
	//   最終的に   ⇒  ((( 40dBのS-METER値 － 20dBのS-METER値 )÷ 2)
	//   でよくなる。
	// 2)AM時
	//   仕様上の式 ⇒  ((( 50dBのS-METER値 － 30dBのS-METER値 )×１０) ÷ (50dB － 30dB)) 
	//   少し簡単に ⇒  ((( 50dBのS-METER値 － 30dBのS-METER値 )×１０) ÷ (20))
	//   もう少し   ⇒  ((( 50dBのS-METER値 － 30dBのS-METER値 )×１) ÷ (2))
	//   最終的に   ⇒  ((( 50dBのS-METER値 － 30dBのS-METER値 )÷ 2)
	//   でよくなる。
	// 結果
	//   FM/AM共に、下記の計算は共通で問題なし
	//====================================================================
	// 2008.09.09-1 計算式を変更
	temp = (BYTE)((d_Level40 - d_Level20) / 2U );
//	temp = ((d_Level40 - d_Level20) / ( 40U - 20U ));
	
	if( 0x00U == temp ){
		temp = 0x01U;
	}
	return(temp);
}


BYTE	Tu_FMLeveldB_Conv(BYTE level)
{
	WORD	temp = 0x00U;											// 2008.08.21-3 db変換時、オーバーフローが発生する可能性があるので、変数をWORDに変更
	BYTE	result_db = 0x00U;
	
	//========================================================================
	// Level値をS-Meterへ変換 
	//------------------------------------------------------------------------
	// ( 受信LEVELと20dBのS-METER値の差×10 ÷ Ａ(1dbあたりのLevel値) ＋ 20dB 
	//========================================================================
	
	//--------------------------------
	// 受信LEVELと20dBのS-METER値の差 
	//--------------------------------
	if( level < tu_chkcef[CTU_FM].sm_20db ){							// 20db値の方が高い場合
		temp = (tu_chkcef[CTU_FM].sm_20db - level);
	}else{															// Level値の方が高い場合
		temp = (level - tu_chkcef[CTU_FM].sm_20db);
	}
	
	// 2008.09.09-1 10倍するように修正
	temp = (WORD)(temp * 10);										// temp領域は最大でも255までしか設定されない。なので、10倍しても問題なし
	
	if( 0x00U != tu_level_FM1db ){								// 1dB当たりのLEVEL値で割り算。分母が0になることはないが、
		temp = (temp / tu_level_FM1db);							// MISRAで指摘されないために、ガード処理追加
	}
	// <注>tu_level_FM1dbが0に設定されることがないが、万が一、0が設定されている場合、db値は、20固定で設定される
	
	// 2008.08.21-3 オーバーフローが発生する可能性があるため、対応
	// 2008.09.10-1 Level値が20dbよりも低い場合は、計算結果から－20するように修正
	if( level < tu_chkcef[CTU_FM].sm_20db ){							// 20db値の方が高い場合
		if( temp > 20U ){											// 既に値が20以下の場合
			temp = 0U;												// 値を0に設定(計算しちゃうと計算結果が0xFFになっちゃう。。)
		}else{
			temp = (WORD)(20U - temp);							// 20dbから計算結果を引く
		}
	}else{
		temp = (WORD)(20U + temp);								// 20dbを＋する
	}
	
	if( 0x00FF < temp ){											// db変換した結果、255以上の場合
		result_db = 0xFFU;											// 255dbとする
	}else{
		result_db = (BYTE)temp;									// その他は、計算結果をそのまま設定
	}
	
	return( result_db );
}
BYTE	Tu_AMLeveldB_Conv(BYTE level)
{
	WORD	temp = 0x00U;											// 2008.08.21-3 db変換時、オーバーフローが発生する可能性があるので、変数をWORDに変更
	BYTE	result_db = 0x00U;
	
	//========================================================================
	// Level値をS-Meterへ変換 
	//------------------------------------------------------------------------
	// 2008.10.06-1 計算式変更
	// ( 受信LEVELと30dBのS-METER値の差×10 ÷ Ａ(1dbあたりのLevel値) ＋ 30dB 
	//========================================================================
	
	//--------------------------------
	// 受信LEVELと20dBのS-METER値の差 
	//--------------------------------
	if( level < tu_chkcef[CTU_AM].sm_20db ){							// 20db値の方が高い場合
		temp = (tu_chkcef[CTU_AM].sm_20db - level);
	}else{															// Level値の方が高い場合
		temp = (level - tu_chkcef[CTU_AM].sm_20db);
	}
	
	// 2008.09.09-1 10倍するように修正
	temp = (WORD)(temp * 10);										// temp領域は最大でも255までしか設定されない。なので、10倍しても問題なし
	
	if( 0 != tu_level_AM1db ){									// 1dB当たりのLEVEL値で割り算。分母が0になることはないが、
		temp = (temp / tu_level_AM1db);							// MISRAで指摘されないために、ガード処理追加
	}
	// <注>tu_level_AM1dbが0に設定されることがないが、万が一、0が設定されている場合、db値は、20固定で設定される
	
	// 2008.08.21-3 オーバーフローが発生する可能性があるため、対応
	// 2008.09.10-1 Level値が20dbよりも低い場合は、計算結果から－20するように修正
	// 2008.10.06-1 下限を20db⇒30dbに変更
	if( level < tu_chkcef[CTU_AM].sm_20db ){							// 30db値の方が高い場合
		if( temp > 30U ){											// 既に値が30以下の場合
			temp = 0U;												// 値を0に設定(計算しちゃうと計算結果が0xFFになっちゃう。。)
		}else{
			temp = (WORD)(30U - temp);							// 30dbから計算結果を引く
		}
	}else{
		temp = (WORD)(30U + temp);								// 30dbを＋する
	}
	
	if( 0x00FF < temp ){											// db変換した結果、255以上の場合
		result_db = 0xFFU;											// 255dbとする
	}else{
		result_db = (BYTE)temp;									// その他は、計算結果をそのまま設定
	}
	
	return( result_db );
}

BYTE	Tu_Average_QInfo( BYTE *rxbuf )
{
	BYTE		work;
	BYTE		chknum;
	TTU_QINFO	*Qinfo;
	
	Qinfo = (PTTU_QINFO)rxbuf;
	/*	QRSビット(bit7,bit6)の確認				*/
	if ( (Qinfo->Status & BIN_11000000) != BIN_01000000 )
	{	/* QRSが01でないなら		*/
		return(2);								/* 処理終了(NG)				*/
	}
	
	/* ============================================================================	*/
	/*	Q-Info取得 一回チェック＆平均計算											*/
	/*	  Q-Info一回チェックはpick up1回チェック時のしきい値を使用(single model)	*/
	/* ============================================================================	*/
#if 1
	/* ==== Q-Info 判定 ===========================================================	*/
	if( tu_test_mode == 0xFF )
	{
		if( tu_sme_cnt == 0 )
		{
			work = Tu_QInfo_Chk(0,Qinfo);				/* PI check時 1回判定				*/
		}
		else
		{
			work = Tu_QInfo_Chk(1,Qinfo);				/* PI check時 1回判定				*/
		}
		if(work != OK)
		{				/* しきい値判定ＮＧ時				*/
			return(NG);						/* 処理終了(NG)						*/
		}
		chknum = CTU_SEEK_NUM;
	}
	else
	{
		chknum = CTU_TEST_NUM;
	}
#endif
	/* ----- S-Meter 平均計算 ----------------------------------------------------- */
	Tu_QinfoAvg(tu_qinfochk_num,&tu_sme_cnt,&tu_sme_ave[0],Qinfo->Smeter);

	/* ----- IF-Offset 平均計算 --------------------------------------------------- */
	Tu_QinfoAvg(tu_qinfochk_num,&tu_if_cnt,&tu_if_ave[0],Qinfo->IFval);

	/* ----- 隣接ﾉｲｽﾞ 平均計算 ---------------------------------------------------- */
	Tu_QinfoAvg(tu_qinfochk_num,&tu_usn_cnt,&tu_usn_ave[0],Qinfo->Rnoise);

	/* ----- ﾏﾙﾁﾊﾟｽﾉｲｽﾞ 平均計算 -------------------------------------------------- */
	Tu_QinfoAvg(tu_qinfochk_num,&tu_wam_cnt,&tu_wam_ave[0],Qinfo->Mnoise);

	/* ===== 初回の平均値が用意できたか判断 =======================================	*/
	if( (tu_sme_cnt  != 1)						/* S-Meter量 準備中 				*/
	  ||(tu_if_cnt != 1)						/* IF-Offset 準備中 				*/
	  ||(tu_usn_cnt	 != 1)						/* 隣接  ﾉｲｽﾞ準備中 				*/
	  ||(tu_wam_cnt	 != 1)						/* ﾏﾙﾁﾊﾟｽﾉｲｽﾞ準備中 				*/
	)
	{
		return(2);						/* 処理終了(OK)						*/
	}

	/* ============================================================================	*/
	/*	Q-Info平均チェック															*/
	/* ============================================================================	*/

	/* ----- Q-Info判定の入力設定 -------------------------------------------------	*/
	Qinfo->Smeter = tu_sme_ave[0];					/* 									*/
	Qinfo->IFval  = tu_if_ave[0];
	Qinfo->Rnoise = tu_usn_ave[0];
	Qinfo->Mnoise = tu_wam_ave[0];
	
	/* ==== Q-Info 判定 ===========================================================	*/
	work = Tu_QInfo_Chk(2,Qinfo);				/* PI check時 平均判定				*/
	if(work != OK)
	{				/* しきい値判定ＮＧ時				*/
		return(NG);						/* 処理終了(NG)						*/
	}
	else
	{									/* しきい値判定ＯＫ時				*/
		return(OK);						/* 処理終了(OK)						*/
	}


}

BYTE	Tu_QInfo_Chk(BYTE chk_tm, TTU_QINFO *pQInfo)
{
	BYTE	ret;
	
	/* ===== S-Meter 判定 =============================================	*/
	ret = Tu_QInfo_SmeterChk(chk_tm,pQInfo->Smeter);
	if( tu_test_mode != 0xFF )
	{
		return (OK);
	}
	if(ret == FALSE) {							/* しきい値判定ＮＧ時	*/
		return( NG );			//2007.04.24
	}

	/* ===== IF-Offset 判定 =========================================== */
	ret = Tu_QInfo_IFChk(chk_tm,pQInfo->IFval);
	if(ret == NG) {								/* しきい値判定ＮＧ時	*/
		return( NG );				//2007.04.24
	}

	if(Tu_band_chk() != CTU_AM)
	{
		/* ===== 隣接ﾉｲｽﾞ 判定 ============================================ */
		ret = Tu_CheckFM_USN(chk_tm,pQInfo->Rnoise);
		if(ret == TRUE) {						/* しきい値判定ＮＧ時	*/
			return( NG );			//2007.04.24
		}
	
		/* ===== ﾏﾙﾁﾊﾟｽﾉｲｽﾞ 判定 ==========================================	*/
		ret = Tu_CheckFM_WAM(chk_tm,pQInfo->Mnoise);
		if(ret == TRUE) {							/* しきい値判定ＮＧ時	*/
			return( NG );				//2007.04.24
		}
	}

	/* ===== Q-Info OK!	===============================================	*/
	return( OK );					//2007.04.24
}

BYTE	Tu_QInfo_SmeterChk(BYTE Item,BYTE data)
{
	BYTE	r;
	BYTE	temp_Nowdb;
	
	
	// 2008.09.25-1 オーバーフローが発生しないように対応
	//              演算時、オーバーフローが発生しないように変数をWORDにキャストする。
	//----------------------
	// パラメータにより分岐 
	//----------------------
	if(Tu_band_chk() == CTU_AM)		/*	BANDはAM	*/
	{
		temp_Nowdb = Tu_AMLeveldB_Conv( data );				// 受信LEVELをdbに変換
		r = Tu_dxlo_chk(CTU_AM);	/* [matu120903] */
		if( Item < 2 )
		{
			r = (r * 10)/tu_chkcef[CTU_AM].prec_sm;
		}
	}
	else		/*	FM	*/
	{
		temp_Nowdb = Tu_FMLeveldB_Conv( data );				// 受信LEVELをdbに変換
		r = tu_chkcef[CTU_FM].sm_dxdb;
		if( Item < 2 )
		{
			r = (r * 10)/tu_chkcef[CTU_FM].prec_sm;
		}
	}
	if( tu_test_mode != 0xFF )
	{
		tu_test_value = temp_Nowdb;
	}

	if(temp_Nowdb > r){
		return(TRUE);
	}else{
		return(FALSE);
	}
}


BYTE	Tu_QInfo_IFChk(BYTE Item, BYTE data)
{
	BYTE 	r = OK;
	BYTE	work = 0;
	
	// 2008.08.25-1 EEPROMから取得した閾値で判定する
	
	if(Tu_band_chk() == CTU_AM)		/*	BANDはAM	*/
	{
		if( Item == 2 )
		{
			work = tu_chkcef[CTU_AM].if_detail;					//詳細チェック データメモリ
		}
		else
		{
			work = tu_chkcef[CTU_AM].if_rough;					//ラフチェック データメモリ
		}
	
	}
	else
	{
		if( Item == 2 )
		{
			work = tu_chkcef[CTU_FM].if_detail;			//詳細チェック データメモリ
		}
		else
		{
			work = tu_chkcef[CTU_FM].if_rough;			//ラフチェック データメモリ
		}
	}

	if(data > work)
	{
		r = NG;									// +方向より大きい = NG
	}
	else
	{
												// ということは、OK。
	}
	return(r);
}

BYTE	Tu_CheckFM_USN(BYTE Item, BYTE data)
{
	BYTE r;
	
	r = tu_chkcef[CTU_FM].usn_max;
	if( Item < 2 )
	{
		r = (r * tu_chkcef[CTU_FM].prec_noise)/10;
	}
	
	if(data > r) {
		return(TRUE);
	}
	else {
		return(FALSE);
	}
}
BYTE	Tu_CheckFM_WAM(BYTE Item, BYTE data)
{
	BYTE	r;
	
	r = tu_chkcef[CTU_FM].wam_max;
	if( Item < 2 )
	{
		r = (r * tu_chkcef[CTU_FM].prec_noise)/10;
	}

	if(data > r) {
		return(TRUE);
	}
	else {
		return(FALSE);
	}
}


BYTE	Tu_at_QinfoAvg(
	BYTE			TotalNum,			//	(i)平均回数
	TTU_QI_CALC*	pqinf,				//	(o)累積ﾃﾞｰﾀ
	BYTE			value				//	(i)今回ﾃﾞｰﾀ
)
{
	BYTE		work[2];
	WORD		usTemp	=	0;

	/* ===== 初回データ =======================================================	*/
	if ( 0 == pqinf->cnt ) {
		if ( TotalNum < 255 ) {				//	上限ﾁｪｯｸ
			pqinf->cnt	=	(BYTE)(TotalNum + 1);	//	ﾙｰﾁﾝ共用の為に回数ﾃﾞｰﾀ補正
		}
		else {
			pqinf->cnt	=	255;
		}
		pqinf->data		=	0x00;
		pqinf->rem		=	0x00;
		return (TRUE);
	}

	work[0]	=	pqinf->data;
	work[1]	=	pqinf->rem;

	/* ===== 指定回数までのデータ取り込み =====================================	*/
	if ( 1 != pqinf->cnt ) {
		usTemp	=	value;					//	WORD型変数に代入
	/* ----- 指定回数未満 ----------------------------------------------------- */
		Tu_Add_WORD(&work[0],usTemp);			//	ﾃﾞｰﾀ加算
		(pqinf->cnt)--;
		if( 1 == pqinf->cnt ) {				//	指定回数ｼﾞｬｽﾄ時
			usTemp		=	Tu_Chg_WORD(&work[0]);
		}
		else {
			pqinf->data	=	work[0];		//	単純な計算途中の格納
			pqinf->rem	=	work[1];		//	商と余りを格納しているわけではない
			return( FALSE );				//	平均値まだ
		}
	/* ----- 指定回数以降 ----------------------------------------------------- */
	}
	else {
		/* （前回までの平均データ）×（指定回数－１）
			 ＋（前回の平均計算で発生した余り）＋（今回計測した値） 			*/
		usTemp	=	(WORD)((work[0] * (TotalNum-1)) + work[1] + value);
	}

	/* ===== 平均値算出 =======================================================	*/
	/* （ここまでの加算データ）÷（指定回数） 									*/
	if ( 0 == TotalNum ) {						//	分母が0だった時のﾌｪｰﾙｾｰﾌ
		work[0]	=	(BYTE)(usTemp);
		work[1]	=	0;							//	余りﾃﾞｰﾀ無しとする
	}
	else {
		work[0]	=	(BYTE)(usTemp/TotalNum);
		work[1]	=	(BYTE)(usTemp%TotalNum);	//	余りﾃﾞｰﾀ格納
	}

	pqinf->data	=	work[0];
	pqinf->rem	=	work[1];

	return( TRUE );							//	平均値OK
}

/******************************************************************
	BYTE TotalNum,			//	平均回数
	BYTE *pNum,			//	取得回数	1ﾊﾞｲﾄ
	BYTE *pData,			//	平均値		2ﾊﾞｲﾄ
	BYTE value				//	今回取得したﾃﾞｰﾀ
******************************************************************/
BYTE	Tu_QinfoAvg(BYTE TotalNum, BYTE *pNum, BYTE *pData, BYTE value)
{
	BYTE		rtn		=	FALSE;
	TTU_QI_CALC	qinf;

	qinf.cnt	=	*pNum;
	qinf.data	=	*(pData);
	qinf.rem	=	*(pData+1);

	rtn			=	Tu_at_QinfoAvg( TotalNum, &qinf, value );

	*pNum		=	qinf.cnt;
	*(pData)	=	qinf.data;
	*(pData+1)	=	qinf.rem;

	return( rtn );
}

void	Tu_Test_Main(void)
{
	if( tu_diag_req != CTU_DIAG_NOP )
	{
		tu_diag_mainfunctbl[tu_diag_req]();
	}
}

#define		CTU_DIAG_START		0x00

#define		CTU_DIAG_EEP_WRITE	0x01
#define		CTU_DIAG_EEP_READ	0x01

static	void	Tu_Diag_NgSet(void)
{
	tu_test_mode = 0xFF;
	tu_diag_req = CTU_DIAG_NOP;
	tu_diag_seq = 0;
}

static	void	Tu_Diag_Idle(void)
{
	
}

void	Tu_5ms_test(void)
{
	if ((tu_test_mode == 0xFE)&&(tu_diag_req != CTU_DIAG_NOP )&&(tu_diag_seq == 1))
	{
		tu_sm_chk_ok = OK;
	}
}


#undef  _TU_TEST_C_
#endif

