/***************************************************************
	クラリオン殿向け       
	プロジェクト名	：'12年モデル
	ファイル名		：lcd_mode.c
	モジュール名	：LCD(表示)
	機   能			：ﾓｰﾄﾞ分岐処理関数ファイル
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
#define		_LCD_MODE_C_

#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#include	"../model.h"

#include	"../comm/common.h"
#include	"../audio/aud_ext.h"
#include	"../audio/aud_dev_func.h"
#include	"../main/ma_ext.h"
#include	"../tuner/TU_inc.h"
#include	"../submode/SBM_inc.h"
#include	"../testmode/TEST_inc.h"
#include	"../power/pw_def.h"
#include	"../power/pw_func_ext.h"	/* QAC対策 */
#include	"lcd_main.h"
#include	"lcd_ext.h"
#include	"lcd_mode.h"
#include	"lcd_send.h"
#include	"lcd_sys.h"
#include	"lcd_tbl.h"


/* ローカル関数 */
static void Lcd_main_mode(void);
static void Lcd_off_mode(void);
static void Lcd_snd_mode(void);
static void Lcd_1shotdisp_mode(void);
static void Lcd_tu_base(TDP_TUNER *);
static void Lcd_tu_base_frq(BYTE band,BYTE bandno,BYTE *freq,BYTE preno);
static void Lcd_tu_indicator(void );
static void Lcd_st_indicator(void );
static void Lcd_clkalm_indicator(void);
static void	Lcd_blank_disp(void);

static void Lcd_tuner_mode(void);

static void Lcd_aux_mode(void);
static void Lcd_vol_mode(void);
static void Lcd_isr_mode(void);
static void Lcd_model_version(void);
/****************************************************************
		関 数 名：Lcd_mode
		引    数：無し
		戻 り 値：無し
		機    能：ﾓｰﾄﾞ分岐処理
		コメント：表示優先度の順
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_mode(void)
{ 
	if(Ma_model_dest() == 0x00 )
	{
		Lcd_area_disp();
	}
	#if 0 
	else if (Pw_ACC_DET_get() == OFF)
	{
		Lcd_off_mode();					/* PWR OFF */
	}
	#endif
	else if(Pw_PowerOff_disp_chk() == TRUE)
	{
		Lcd_off_mode();					/* PWR OFF */
	}
	else if (Sct_mode_check() == CSBM_SCT_VER)
	{
		Lcd_model_version();
	}
	#if 0	/* 未使用 */
	else if((lcd_src_mode > 0)&&(lcd_src_mode < CLCD_SRC_MAX))
	{
		Lcd_srcchg_mode();				/* ｿｰｽ遷移一時表示 */
	}
	#endif
	else if((Sbm_mode_check() != CSBM_OFF)&&(Sbm_mode_check() !=CSBM_TES))
	{
		Lcd_st_indicator();
		if(Vol_mode_check() != OFF)
		{
			Lcd_vol_mode();
		}
		else if(Sef_mode_check() !=  OFF)
		{
			Lcd_snd_mode();					/* SOUNDﾓｰﾄﾞ */
		}
		else if((Clk_adj_check() == ON)||(Alm_mode_check() == ON)||(Alm_beepon_get() == TRUE))
		{
			Lcd_clock_mode();
		}
		else
		{
			;
		}
	}
	else
	{
		Lcd_st_indicator();
		Lcd_main_mode();				/* 各ﾒｲﾝﾓｰﾄﾞ処理 */
	}
}
/****************************************************************
		関数名：Lcd_main_mode
		引  数：無し
		戻り値：無し
		機  能：各ﾒｲﾝﾓｰﾄﾞ表示処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
static void Lcd_main_mode(void)
{
	BYTE ma_mode;
	ma_mode = Ma_current_mode();
	switch(ma_mode)
	{
		case CMA_TUNER:
			Lcd_tuner_mode();						/* TUNERﾓｰﾄﾞ */
			break;
		
		case CMA_AUX:
			Lcd_aux_mode();							/* AUXﾓｰﾄﾞ　 */
			break;
			
		case CMA_ISR:								/* ISRモード */
			Lcd_isr_mode();
			break;
		
		default:
			break;
	}
}
/****************************************************************
		関数名：Lcd_off_mode
		引  数：無し
		戻り値：無し
		機  能：OFFﾓｰﾄﾞ表示処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
static void Lcd_off_mode(void)
{
	#if 1
	Lcd_clock_mode();			/* OFFﾓｰﾄﾞ中時計表示 */
	#else
	if (Ma_get_clock() == TRUE)
	{
		Lcd_clock_mode();			/* OFFﾓｰﾄﾞ中時計表示 */
	}
	else
	{
		Lcd_blank_disp();
	}
	#endif
}

#if 0	/* 未使用 */
/****************************************************************
		関数名：Lcd_srcchg_mode
		引  数：無し
		戻り値：無し
		機  能：ｿｰｽ遷移一時表示処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
static void Lcd_srcchg_mode(void)
{
	//lcd_8moji_romcpy(lcd_seg_buf,(BYTE *)CLCD_SRC_TBL[lcd_src_mode-1]);
	lcd_moji_romcpy(lcd_seg_buf,(BYTE *)CLCD_SRC_TBL[lcd_src_mode-1]);
}
#endif

/****************************************************************
		関数名：Lcd_vol_mode
		引  数：無し
		戻り値：無し
		機  能：Vol調整表示処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
static void Lcd_vol_mode(void)
{
	BYTE	value;
	
	value = Vol_lcddata_set();
	lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[VOL]);
	lcd_send_buf.record1.colon = ON;
	Lcd_hextoascii(value,&lcd_seg_buf[SELVOL]);		/* HEXtoASCII */
	Lcd_nodisp(&lcd_seg_buf[SELVOL],1);						/* 上位が0なら、表示しない */
	
}
/****************************************************************
		関数名：Lcd_snd_mode
		引  数：無し
		戻り値：無し
		機  能：Audio調整表示処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
static void Lcd_snd_mode(void)
{
	TDP_SEL seldisp;
	BYTE value;
	
	memset( &seldisp, 0, sizeof(seldisp));			/* ｸﾘｱ */
	Sef_lcddata_set( &seldisp );					/* AUDIOﾃﾞｰﾀ情報取得 */
	lcd_send_buf.record1.colon = ON;
	switch(seldisp.mode)
	{
		case CSBM_AUD_BAS:
			lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAS]);
			if(seldisp.value < CAUD_BAS_CNT)
			{
				value = (CAUD_BAS_CNT - seldisp.value);
				Lcd_hextoascii(value,&lcd_seg_buf[SELBAS]);		/* HEXtoASCII */
				Lcd_nodisp(&lcd_seg_buf[SELBAS],1);				/* 上位が0なら、表示しない */
				lcd_seg_buf[SELBAS] = '-';							/* "-" */
			}
			else if(seldisp.value > CAUD_BAS_CNT)
			{
				value = (seldisp.value - CAUD_BAS_CNT);
				Lcd_hextoascii(value,&lcd_seg_buf[SELBAS]);		/* HEXtoASCII */
				Lcd_nodisp(&lcd_seg_buf[SELBAS],1);				/* 上位が0なら、表示しない */
				lcd_seg_buf[SELBAS] = '+';
			}
			else
			{
				lcd_seg_buf[SELBAS+1] = '0';
			}
			break;
		case CSBM_AUD_TRE:
			lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[TRE]);
			if(seldisp.value < CAUD_TRE_CNT)
			{
				value = (CAUD_TRE_CNT - seldisp.value);
				Lcd_hextoascii(value,&lcd_seg_buf[SELTRE]);		/* HEXtoASCII */
				Lcd_nodisp(&lcd_seg_buf[SELTRE],1);				/* 上位が0なら、表示しない */
				lcd_seg_buf[SELTRE] = '-';							/* "-" */
				
			}
			else if(seldisp.value > CAUD_TRE_CNT)
			{
				value = (seldisp.value - CAUD_TRE_CNT);
				Lcd_hextoascii(value,&lcd_seg_buf[SELTRE]);		/* HEXtoASCII */
				Lcd_nodisp(&lcd_seg_buf[SELTRE],1);				/* 上位が0なら、表示しない */
				lcd_seg_buf[SELTRE] = '+';							/* "+" */
			}
			else
			{
				lcd_seg_buf[SELTRE+1] = '0';
			}
			break;
		case CSBM_AUD_BAL:
			if(seldisp.value == CAUD_BAL_CNT)
			{
				lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAL_C]);
			}
			else
			{
				lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAL]);
				if(seldisp.value > CAUD_BAL_CNT)
				{
					
					seldisp.value -= CAUD_BAL_CNT;
					Lcd_hextoascii( seldisp.value, &lcd_seg_buf[SELBAL] );		/* HEX to ASCII */
					Lcd_nodisp( &lcd_seg_buf[SELBAL],1 );						/* 上位が0場合、表示しない */
					lcd_seg_buf[SELBAL] = 'R';
				}
				else
				{
				
					seldisp.value = CAUD_BAL_CNT - seldisp.value;
					Lcd_hextoascii( seldisp.value, &lcd_seg_buf[SELBAL] );		/* HEX to ASCII */
					Lcd_nodisp( &lcd_seg_buf[SELBAL],1 );						/* 上位が0場合、表示しない */
					lcd_seg_buf[SELBAL] = 'L';
				}
			}
			break;
		default:
			break;
	}
}

/****************************************************************
		関数名：Lcd_tuner_mode
		引  数：無し
		戻り値：無し
		機  能：TUNERﾓｰﾄﾞ表示処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
static void Lcd_tuner_mode(void)
{
	static TDP_TUNER tudisp;
	BYTE dmode;
	BYTE ind;
	
	memset(&tudisp,0,sizeof(tudisp));				/* ﾊﾞｯﾌｧｸﾘｱ */
	
	if (Ma_get_clock() == FALSE)
	{
		dmode = CLCD_BASE_TUNER;
	}
	else
	{
		if(lcd_base_mode == CLCD_BASE_TUNER)
		{
			dmode = CLCD_BASE_TUNER;
		}
		else
		{
			dmode = CLCD_BASE_CLOCK;
		}
	}
	
	Tu_lcddata_set(&tudisp,dmode);
	if((tudisp.mode != CLCD_DISPMODE) && (tudisp.mode < CLCD_TU_MAX))		/* 特殊状態、内部ﾓｰﾄﾞ */
	{
		switch(tudisp.mode)			/* 表示ﾓｰﾄﾞ */
		{
			case CLCD_TU_SEEK:			/* SEEK中 */
				Lcd_tu_base(&tudisp);
				//Lcd_tu_base_frq(tudisp.band,tudisp.bandno,&(tudisp.frq[0]),tudisp.preno);
				Lcd_disp_1shot_base(CMA_TUNER);			/* SEEK後ﾍﾞｰｽ5秒表示要求 */
				break;
	
			case CLCD_TU_SCAN:			/* ﾌﾟﾘｾｯﾄｽｷｬﾝ中 */
				Lcd_tu_base(&tudisp);
				break;
				
			case CLCD_TU_SCANWAIT:
				Lcd_tu_base(&tudisp);
				Lcd_blink_set(CLCD_BLK_FREQ,BLK_500MS);		/* 周波数点滅処理 */
				break;
			
			case CLCD_TU_PSTWT:
				Lcd_tu_base(&tudisp);
				Lcd_blink_set(CLCD_BLK_PSNO,BLK_500MS);		/* プリセットNo点滅処理 */
				break;
			
			case CLCD_TU_AS:			/* SEEK中 */
				Lcd_tu_base(&tudisp);
				Lcd_disp_1shot_base(CMA_TUNER);			/* SEEK後ﾍﾞｰｽ5秒表示要求 */
				break;
	
			default:
				break;
		}
	}
	else								/* DISPﾓｰﾄﾞ */
	{
		if(Clk_adj_check() == ON)
		{
			dmode = CLCD_BASE_CLOCK;		/* 時計調整表示設定 */
		}
		switch(dmode)
		{
			case CLCD_BASE_TUNER:
				Lcd_tu_base(&tudisp);
				break;
			case CLCD_BASE_CLOCK:
				Lcd_clock_mode();
				break;
			default:
				break;
		}
	}
}
/****************************************************************
		関数名：Lcd_tu_base
		引  数：無し
		戻り値：無し
		機  能：ベース表示処理(TUNER)
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
static void Lcd_tu_base(TDP_TUNER *add)
{
	#if 0
	BYTE tu_ind;
	tu_ind = Tu_indicator_set();
	if((tu_ind & CLCD_TU_IND_S1) != 0)				/* "1"    */
	{
		lcd_send_buf.record1.s1 = ON;
	}
	#endif
	Lcd_tu_indicator();
	Lcd_tu_base_frq(add->band,add->bandno,&(add->frq[0]),add->preno);
}
/****************************************************************
		関数名：Lcd_tu_base_frq
		引  数：無し
		戻り値：無し
		機  能：ベース表示処理(TUNER周波数)
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
static void Lcd_tu_base_frq(BYTE band,BYTE bandno,BYTE *freq,BYTE preno)
{
	BYTE dest;
	
	switch(band)
	{
		case CTU_FM:
/* 周波数表示 */
			lcd_seg_buf[FRQ] = (((*(freq+1))>>4) & 0x0F) + 0x30;
			lcd_send_buf.record1.dot = ON;					/* "."点灯 */
			//Lcd_nodisp(&lcd_seg_buf[FRQ],1);					/* 0表示しない */
			lcd_seg_buf[FRQ+1] = ((*(freq+1)) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+2] = (((*(freq+2)) >> 4) & 0x0F) + 0x30;
			if (Ma_model_dest() == CMA_MODEL_AMFM_A )
			{
				lcd_seg_buf[FRQ+3] = ((*(freq+2)) & 0x0F) + 0x30;
			}
			else
			{
				Lcd_nodisp(&lcd_seg_buf[FRQ+3],1);								/* 0表示しない */
			}
			break;
		case CTU_AM:
/* 周波数表示 */
			#if 0
			lcd_seg_buf[FRQ] = (((*(freq+1)) >> 4) & 0x0F) + 0x30;
			Lcd_nodisp(&lcd_seg_buf[FRQ],1);								/* 0表示しない */
			lcd_seg_buf[FRQ+1] = ((*(freq+1)) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+2] = (((*(freq+2)) >> 4) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+3] = ((*(freq+2)) & 0x0F) + 0x30;
			#else
			lcd_seg_buf[FRQ] = ((*(freq+1)) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+1] = (((*(freq+2))>>4) & 0x0F) + 0x30;
			lcd_seg_buf[FRQ+2] = ((*(freq+2)) & 0x0F) + 0x30;
			#endif
			break;
		default:
			break;
	}
/* ﾌﾟﾘｾｯﾄﾅﾝﾊﾞｰ表示 */
	if((preno > 0) && (preno <= 7))				/* 範囲以外処理しない */
	{
		lcd_seg_buf[PRENO] = preno + 0x30;
	}
}

void Lcd_tu_indicator(void )
{	
	BYTE ind;
	ind = Tu_indicator_set();
	if((ind & CLCD_TU_IND_S1) != 0)				/* "1"    */
	{
		lcd_send_buf.record1.s1 = ON;
	}
	if((ind & CLCD_TU_IND_FM) != 0)			/* "FM"   */
	{
		lcd_send_buf.record1.fm = ON;
	}
	if((ind & CLCD_TU_IND_AM) != 0)			/* "AM"   */
	{
		lcd_send_buf.record1.am = ON;
	}
	if((ind & CLCD_TU_IND_ONE) != 0)			/* "1"     */
	{
		lcd_send_buf.record1.one = ON;
	}
	if((ind & CLCD_TU_IND_TWO) != 0)			/* "2"     */
	{
		lcd_send_buf.record1.two = ON;
	}
	#if 0	/* STは別点灯処理に仕様変更 */
	if((ind & CLCD_TU_IND_ST) != 0)			/* "ST"    */
	{
		lcd_send_buf.record1.st = ON;
	}
	#endif
}

/****************************************************************
		関数名：Lcd_st_indicator
		引  数：無し
		戻り値：無し
		機  能：STインジケータ有無判定処理
				仕様変更により追加
				時計表示中でもSTインジケータを点灯させるため
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2013/04/02		1.0		山本	新規作成
****************************************************************/
static void Lcd_st_indicator(void )
{	
	BYTE ind;
	if( (Ma_current_mode() 	== CMA_TUNER	)	&&
		(Tu_band_chk() 		== CTU_FM		)	)
	{
		ind = Tu_indicator_set();
		if((ind & CLCD_TU_IND_ST) != 0)			/* "ST"    */
		{
			lcd_send_buf.record1.st = ON;
		}
	}
}

/****************************************************************
		関数名：Lcd_isr_mode
		引  数：無し
		戻り値：無し
		機  能：ISRﾓｰﾄﾞ表示処理(交通情報)
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
static void Lcd_isr_mode(void)
{
	BYTE dmode;
	BYTE dest;
	BYTE ind;
	TDP_ISR isrdisp;
	BYTE tu_isr_ind;

	if (Ma_get_clock() == FALSE)
	{
		dmode = CLCD_BASE_ISR;
	}
	else
	{
		if(lcd_base_mode == CLCD_BASE_ISR)
		{
			dmode = CLCD_BASE_ISR;
		}
		else
		{
			dmode = CLCD_BASE_CLOCK;
		}
	}
	Tu_isr_lcddata_set(&isrdisp,dmode);
	tu_isr_ind = Tu_isr_indicator_set();
	if((tu_isr_ind & CLCD_TU_IND_TI) != 0)				/* "1"    */
	{
		lcd_send_buf.record1.ti = ON;
	}
	switch(dmode)
	{
		case CLCD_BASE_ISR:
				if((tu_isr_ind & CLCD_TU_IND_S1) != 0)				/* "1"    */
				{
					lcd_send_buf.record1.s1 = ON;
				}
				lcd_send_buf.record1.am = ON;
				#if 1
				lcd_seg_buf[FRQ] = (isrdisp.frq[1] & 0x0F) + 0x30;
				lcd_seg_buf[FRQ+1] = ((isrdisp.frq[2] >> 4) & 0x0F) + 0x30;
				lcd_seg_buf[FRQ+2] = (isrdisp.frq[2] & 0x0F) + 0x30;
				#else
				lcd_seg_buf[FRQ+1] = ((isrdisp.frq[1] >> 4) & 0x0F) + 0x30;
				Lcd_nodisp(&lcd_seg_buf[5],1);								/* 0表示しない */
				lcd_seg_buf[FRQ+2] = (isrdisp.frq[1] & 0x0F) + 0x30;
				lcd_seg_buf[FRQ+3] = ((isrdisp.frq[2] >> 4) & 0x0F) + 0x30;
				lcd_seg_buf[FRQ+4] = (isrdisp.frq[2] & 0x0F) + 0x30;
				#endif
			break;
		
		case CLCD_BASE_CLOCK:
			Lcd_clock_mode();				/* 時計表示 */
			break;
	
		default:
			break;
	}
}
/****************************************************************
		関数名：Lcd_model_version
		引  数：無し
		戻り値：無し
		機  能：機種名表示
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2007/4/23		0.1		何		新規作成
			2010/6/9		0.2		xu		
****************************************************************/
static void Lcd_model_version(void)
{
	lcd_seg_buf[0] = 'U';
	lcd_seg_buf[1] = '-';
	memcpy(&lcd_seg_buf[2],_VERSION_,2);
	lcd_seg_buf[4] = Ma_model_dest()+0x30;
}
/****************************************************************
		関数名：Lcd_aux_mode
		引  数：無し
		戻り値：無し
		機  能：AUXﾓｰﾄﾞ表示処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
static void Lcd_aux_mode(void)
{
	BYTE dmode;
	
	if (Ma_get_clock() == FALSE)
	{
		dmode = CLCD_BASE_AUX;
	}
	else
	{
		if(lcd_base_mode == CLCD_BASE_AUX)
		{
			dmode = CLCD_BASE_AUX;
		}
		else
		{
			dmode = CLCD_BASE_CLOCK;		/* ベース表示は時計 */
		}
	}
	switch(dmode)
	{
		case CLCD_BASE_AUX:
			memcpy(lcd_seg_buf,STR_AUX,6);
			break;
		case CLCD_BASE_CLOCK:
			Lcd_clock_mode();				/* 時計表示 */
			break;
		default:
			break;
	}
}

/****************************************************************
		関数名：Lcd_clock_mode
		引  数：mode_id→各ﾒｲﾝﾓｰﾄﾞ,CLCD_OFF_COLCK→OFFﾓｰﾄﾞ
		戻り値：無し
		機  能：時計優先表示ﾓｰﾄﾞ処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
void Lcd_clock_mode(void)
{
	static TDP_CLK clkdisp;
	
	memset(&clkdisp,0,sizeof(clkdisp));		/* ｸﾘｱ */
	if (Ma_get_clock() == FALSE)
	{
		Lcd_blank_disp();
		return;
	}
	
	Clk_lcddata_set(&clkdisp);
	
	/* 調整ﾓｰﾄﾞ中 */
	if(clkdisp.mode != CSBM_CLK_NORMAL)
	{
		if (Alm_mode_check() != OFF)
		{
			Lcd_blink_set(CLCD_BLK_ALARM,BLK_500MS);	/* 時計点滅 */
		}
		else
		{
			Lcd_blink_set(CLCD_BLK_CLOCK,BLK_500MS);	/* 時計点滅 */
		}
	}
	lcd_send_buf.record1.colon = ON;								/* ":"点灯 */
	Lcd_hextoascii( clkdisp.hour, &lcd_seg_buf[0] );
	Lcd_nodisp( &lcd_seg_buf[0],1 );						/* 0なら表示しない */
	Lcd_hextoascii( clkdisp.min, &lcd_seg_buf[2] );		/* 分ﾃﾞｰﾀ HEX to ASCII */
	if ((Alm_mode_check() == ON))
	{
		lcd_seg_buf[4] = 'A';					/* seg5に'A'を表示する */
	}
	Lcd_clkalm_indicator();
}

void Lcd_clkalm_indicator(void)
{
	BYTE ind;
	if (Alm_mode_check() == ON)
	{
		ind = Alm_indicator_set();
	}
	else
	{
		ind = Clk_indicator_set();
	}
	if((ind & CLCD_CL_IND_AM) != 0 )		/* "AM" */
	{
		lcd_send_buf.record1.am = ON;
	}
	if((ind & CLCD_CL_IND_PM) != 0 )		/* "PM" */
	{
		lcd_send_buf.record1.pm = ON;
	}
}
void	Lcd_blank_disp(void)
{
	Lcd_nodisp( &lcd_seg_buf[0],5 );
}

void	Lcd_area_disp(void)
{
	lcd_seg_buf[0] = 'A';
	lcd_seg_buf[1] = 'R';
	lcd_seg_buf[2] = 'E';
	lcd_seg_buf[3] = 'A';
	Lcd_nodisp( &lcd_seg_buf[4],1 );
}

#if 0 /* 使用しないため、削除 */
/****************************************************************
		関数名：Lcd_allseq_on
		引  数：無し
		戻り値：無し
		機  能：全点灯
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/08/20		0.1		ha		新規作成
****************************************************************/
void	Lcd_allseq_on(void)
{
	memset(lcd_send_buf.buf_byte,0xff,sizeof(lcd_send_buf.buf_byte));	/* 送信バッファ0ｸﾘｱ */
	
	lcd_send_buf.record.ctrl_1 = 0x00;	/* 0000ｺﾝﾄﾛｰﾙﾃﾞｰﾀ */
	lcd_send_buf.record.ctrl_2 = 0x09;	/* 10010000 */
	lcd_send_buf.record.ctrl_3 = 0x12;	/* 01001000 */		/* 1/3 Duty */

	lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;	/* 2ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
	lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;	/* 3ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
	
	memset(lcd_seg_buf,0xff,sizeof(lcd_seg_buf));
	
	flcd_send = ON;
}

/****************************************************************
		関数名：Lcd_allseq_off
		引  数：無し
		戻り値：無し
		機  能：全消灯
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/08/20		0.1		ha		新規作成
****************************************************************/
void	Lcd_allseq_off(void)
{
	memset(lcd_send_buf.buf_byte,0x00,sizeof(lcd_send_buf.buf_byte));	/* 送信バッファ0ｸﾘｱ */
	
	lcd_send_buf.record.ctrl_1 = 0x00;	/* 0000ｺﾝﾄﾛｰﾙﾃﾞｰﾀ */
	lcd_send_buf.record.ctrl_2 = 0x09;	/* 10010000 */
	lcd_send_buf.record.ctrl_3 = 0x12;	/* 01001000 */		/* 1/3 Duty */

	lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;	/* 2ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
	lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;	/* 3ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
	
	memset(lcd_seg_buf,0x00,sizeof(lcd_seg_buf));
	
	flcd_send = ON;
}
#endif

#undef		_LCD_MODE_C_
