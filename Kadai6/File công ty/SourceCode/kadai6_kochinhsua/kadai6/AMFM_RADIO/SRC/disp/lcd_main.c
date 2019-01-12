/***************************************************************
	クラリオン殿向け   
	プロジェクト名	：'07年モデル開発
	ファイル名		：lcd_main.c
	モジュール名	：LCD(表示)
	機   能			：LCDメイン処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
#define	_LCD_MAIN_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../power/pw_func_ext.h"
#include	"../main/ma_ext.h"
#include	"../submode/sbm_ext.h"
#include	"../submode/sbm_def.h"
#include	"../io/IO_inc.h"
#include	"../tuner/TU_inc.h"
#include	"../audio/aud_ext.h"


#include	"lcd_main.h"
#include	"lcd_send.h"
#include	"lcd_mode.h"
#include	"lcd_ext.h"
#include	"lcd_sys.h"
#include	"lcd_font.h"


#include	<stdio.h>
#include	<string.h> 
#include	<stdlib.h>
/****************************************************************
		関数名：Lcd_main
		引  数：無し
		戻り値：無し
		機  能：メイン処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_main(void)
{
	if(flcd_send == ON)							/* 送信要求があれば、送信 */
	{
		Lcd_send();
	}
	else if ((Pw_ACC_DET_get() == OFF)&&(Alm_mode_check()== OFF))
	{
		if (flcd_accoff_send == TRUE)
		{
			Lcd_ram_initial();							/* 送信ﾊﾞｯﾌｧｸﾘｱ */
			if(Ma_model_dest() == 0x00 )
			{
				Lcd_area_disp();
			}
			else
			{
				Lcd_clock_mode();
			}
			Lcd_indictrol();
			Lcd_edit();							/* LCDｾｸﾞﾒﾝﾄ編集 */
			flcd_accoff_send = OFF;
		}
	}
	else if (lcd_edit_tim == 0)
	{
		lcd_edit_tim = CLCD_ACCON_EDIT_TIM;					/* 50msﾀｲﾏ再設定 */
		Lcd_ram_initial();							/* 送信ﾊﾞｯﾌｧｸﾘｱ */
		if (flcd_test_mode == OFF)
		{
			Lcd_mode();								/* ﾓｰﾄﾞ分岐処理 */
			if(flcd_blank != ON)					/* DISP OFF中点滅、ｲﾝｼﾞｹｰﾀ、ｾｸﾞﾒﾝﾄ編集処理抜け */
			{
				Lcd_indictrol();					/* ｲﾝｼﾞｹｰﾀ処理 */
				Lcd_blink();						/* 点滅処理 */
				Lcd_edit();							/* LCDｾｸﾞﾒﾝﾄ編集 */
			}
		}
		else
		{
			Lcd_test_edit();
		}
	}
	else
	{
		/*処理なし*/
	}
}
/****************************************************************
		関数名：Lcd_initial
		引  数：無し
		戻り値：無し
		機  能：表示ﾓｼﾞｭｰﾙの初期化
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_initial(void)
{
	lcd_tim_pwr_wait = 0xFF;
	lcd_edit_tim = CLCD_ACCON_EDIT_TIM;		/* 編集周期ﾀｲﾏ初期化 */
	flcd_test_mode   = OFF;
	Lcd_ram_initial();					/* 送信ﾊﾞｯﾌｧなどのｸﾘｱ */
}
/****************************************************************
		関数名：Lcd_ram_initial
		引  数：無し
		戻り値：無し
		機  能：RAMｸﾘｱ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
・1/3デューティ
・キースキャン禁止(KSC:1)
・キースキャン許可(KSC:0)
****************************************************************/
void Lcd_ram_initial(void)
{
	/* 送信ﾊﾞｯﾌｧ初期化 */
	memset(lcd_send_buf.buf_byte,0,sizeof(lcd_send_buf.buf_byte));	/* 送信バッファ0ｸﾘｱ */
/*---------------------------------------------------------------------------*/
/*0 SP KC0 KC1*/
/*KC2 KSC K0 K1 P0 P1 P2 SC*/
/*DR DT FC0 FC1 FC2 OC 0 0*/
	lcd_send_buf.record.ctrl_1 = 0x00;	/* 0000ｺﾝﾄﾛｰﾙﾃﾞｰﾀ */
	#if 1
	if ((Pw_ACC_DET_get() == OFF)&&(Alm_mode_check() == FALSE)&&(Alm_beepon_get() == FALSE))
	#else
	//if (Pw_ACC_status_get() == OFF)
	if((Pw_ACC_status_get() == OFF)&&(Alm_mode_check() == FALSE))
	#endif
	{
		lcd_send_buf.record.ctrl_2 = 0x0B;	/* 11010000 *//* キースキャン禁止 */
	}
	else
	{
		lcd_send_buf.record.ctrl_2 = 0x09;	/* 10010000 *//* キースキャン許可 */
	}
	lcd_send_buf.record.ctrl_3 = 0x12;	/* 01001000 */		/* 1/3 Duty */
	
	lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;	/* 2ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
	lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;	/* 3ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */

	/* 文字ﾊﾞｯﾌｧｸﾘｱ */
	memset(lcd_seg_buf,0,sizeof(lcd_seg_buf));
	/* 点滅ﾓｰﾄﾞｸﾘｱ */
	lcd_blk_mode = 0;									/* 点滅ﾓｰﾄﾞｸﾘｱ,時計調整中ｸﾘｱしない */
	
	flcd_send = ON;
}
/****************************************************************
		関数名：Lcd_pwr_on
		引  数：無し
		戻り値：無し
		機  能：LCDドライバーPwrOn(ACC ON)処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2006/3/13		0.1		徐		新規作成
****************************************************************/
void Lcd_driver_send( void )
{
	/* LCDﾄﾞﾗｲﾊﾞ送信 */
	Lcd_ram_initial();
	flcd_blank = ON;									/* ﾌﾞﾗﾝｸ送信 */
	Lcd_send();
	
}

/****************************************************************
		関数名：Lcd_indictrol
		引  数：無し
		戻り値：無し
		機  能：インジケータ設定処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_indictrol( void )
{
	BYTE ind;
	
	#if 0
	if ((Clk_adj_check() == ON)||(lcd_base_mode == OFF )||(Pw_Power_status_get() != ON))
	{
		ind = Clk_indicator_set();
		if((ind & CLCD_CL_IND_AM) != 0 )		/* "AM" */
		{
			lcd_send_buf.record1.am = ON;
		}
		if((ind & CLCD_CL_IND_PM) != 0 )		/* "PM" */
		{
			lcd_send_buf.record1.pm = ON;
		}
	}
	else if (Alm_mode_check() == ON)
	{
		ind = Alm_indicator_set();
		if((ind & CLCD_CL_IND_AM) != 0 )		/* "AM" */
		{
			lcd_send_buf.record1.am = ON;
		}
		if((ind & CLCD_CL_IND_PM) != 0 )		/* "PM" */
		{
			lcd_send_buf.record1.pm = ON;
		}
	}
	else if((Sbm_mode_check() != CSBM_VOL )&&(Sbm_mode_check() != CSBM_SEF ))
	{
		BYTE ma_mode;
		/* 主なﾓｰﾄﾞｲﾝｼﾞｹｰﾀ */
		ma_mode = Ma_current_mode();
		switch(ma_mode)
		{
			/* TUNERモード */
			case CMA_TUNER:
				ind = Tu_indicator_set();
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
				if((ind & CLCD_TU_IND_ST) != 0)			/* "ST"    */
				{
					lcd_send_buf.record1.st = ON;
				}
				#if 0
				if((ind & CLCD_TU_IND_S1) != 0)			/* "1"    */
				{
					lcd_send_buf.record1.s1 = ON;
				}
				#endif
				break;
				
			case CMA_ISR:
				ind = Tu_isr_indicator_set();
				if((ind & CLCD_TU_IND_AM) != 0)			/* "AM"   */
				{
					lcd_send_buf.record1.am = ON;
				}
				#if 0
				if((ind & CLCD_TU_IND_TI) != 0)			/* "・)))"   */
				{
					lcd_send_buf.record1.ti = ON;
				}
				if((ind & CLCD_TU_IND_S1) != 0)				/* "1"    */
				{
					lcd_send_buf.record1.s1 = ON;
				}
				#endif
				break;
				
			case CMA_AUX:	/* AUXはない */
			default:
				break;
		}
	}
	else
	{
		;
	}
	
	#endif
	
	if(Alm_bell_indicator_set() == ON )		/* 「アラーム記号」 */
	{
		lcd_send_buf.record1.al = ON;
	}
	if(Ma_current_mode() == CMA_ISR )		/* 「アラーム記号」 */
	{
		lcd_send_buf.record1.ti = ON;
	}
	
}
/****************************************************************
		関数名：Lcd_blink
		引  数：無し
		戻り値：無し
		機  能：文字点滅処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
void Lcd_blink(void)
{
	if( lcd_blk_mode == OFF ) 
	{
		lcd_tim_blk = 0;							/* 点滅ﾀｲﾏ解除 */
		return;
	}
	
	switch(lcd_blk_type)
	{
		case BLK_500MS:										/* 500ms周期 */
			if(lcd_tim_blk == 0)
			{
				lcd_tim_blk = CLCD_TIMECNT_1S;				/* ｽﾀｰﾄﾀｲﾏ再設定 */
				flcd_blk_clr = OFF;
			}
			else if(lcd_tim_blk >= CLCD_TIMECNT_500MS)
			{
				return;
			}
			else
			{
				;
			}
			break;
		case BLK_1S:										/* 1s周期 */
			if(lcd_tim_blk == 0)
			{
				lcd_tim_blk = CLCD_TIMECNT_2S;				/* ｽﾀｰﾄﾀｲﾏ再設定 */
				flcd_blk_clr = OFF;
			}
			else if(lcd_tim_blk >= CLCD_TIMECNT_1S)
			{
				return;
			}
			else
			{
				;
			}
			break;
		default:
			break;
	}
		
	if(((lcd_blk_type == BLK_1S) && (lcd_tim_blk < CLCD_TIMECNT_1S))
	|| ((lcd_blk_type == BLK_500MS) && (lcd_tim_blk < CLCD_TIMECNT_500MS)))
	{
		if(flcd_blk_clr == OFF)						/* 調整中消灯させない */
		{
			switch(lcd_blk_mode)
			{
				case CLCD_BLK_FREQ:
					lcd_send_buf.record1.s1 = OFF;
					lcd_send_buf.record1.dot = OFF;
					memset(&lcd_seg_buf[0],0,5);
					break;
				
				case CLCD_BLK_CLOCK:
					lcd_send_buf.record1.colon = OFF;
					lcd_send_buf.record1.am = OFF;
					lcd_send_buf.record1.pm = OFF;
					memset(&lcd_seg_buf[0],0,4);
					break;
				
				case CLCD_BLK_ALARM:
					lcd_send_buf.record1.al = OFF;
					lcd_send_buf.record1.colon = OFF;
					lcd_send_buf.record1.am = OFF;
					lcd_send_buf.record1.pm = OFF;
					memset(&lcd_seg_buf[0],0,4);
					break;
				case CLCD_BLK_PSNO:	
					memset(&lcd_seg_buf[4],0,1);
					break;
				
				default:
					break;
			}
		}
	}
}
/****************************************************************
		関数名：Lcd_blink_set
		引  数：無し
		戻り値：無し
		機  能：文字点滅設定
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2006/3/13		0.1		徐		新規作成
****************************************************************/
void Lcd_blink_set(BYTE blkmode, BYTE blktype)
{
	lcd_blk_mode = blkmode;			/* 点滅ﾓｰﾄﾞ */
	lcd_blk_type = blktype;			/* 点滅周期 */
}
/****************************************************************
		関数名：Lcd_25ms_cnt
		引  数：無し
		戻り値：無し
		機  能：25msﾀｲﾏｶｳﾝﾄ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_25ms_cnt(void)
{
	/* MAIN_CNT待ちﾀｲﾏ */
	if((lcd_tim_pwr_wait > TIMER_OUT) && (lcd_tim_pwr_wait != 0xFF))
	{
		lcd_tim_pwr_wait--;
	}
}

/****************************************************************
		関数名：Lcd_50ms_cnt
		引  数：無し
		戻り値：無し
		機  能：50msﾀｲﾏｶｳﾝﾄ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_50ms_cnt(void)
{
	/* 点滅ﾀｲﾏｶｳﾝﾄ */
	if(lcd_tim_blk > TIMER_OUT)
	{
		lcd_tim_blk--;
	}
	/* 一時消灯ﾀｲﾏｶｳﾝﾄ */
	if(lcd_tim_1shot_blk > TIMER_OUT)
	{
		lcd_tim_1shot_blk--;
		if(lcd_tim_1shot_blk == TIMER_OUT)
		{
			lcd_1shot_blk = OFF;
		}
	}
}
/****************************************************************
		関数名：Lcd_100ms_cnt
		引  数：無し
		戻り値：無し
		機  能：100msﾀｲﾏｶｳﾝﾄ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_100ms_cnt(void)
{
	/* ｿｰｽﾁｪﾝｼﾞ一時表示ﾀｲﾏｶｳﾝﾄ */
	if(lcd_src_tim > TIMER_OUT)							/* 2秒表示ﾀｲﾏ */
	{
		lcd_src_tim--;
		if(lcd_src_tim == TIMER_OUT)
		{
			lcd_src_mode = OFF;
			Lcd_disp_1shot_base(Ma_current_mode());		/* 2秒ﾍﾞｰｽ表示 */
		}
	}
	/* 一時表示ﾀｲﾏｶｳﾝﾄ */
	if(lcd_tim_1shot > TIMER_OUT)
	{
		lcd_tim_1shot--;
		if(lcd_tim_1shot == TIMER_OUT)
		{
				lcd_1shot_mode = OFF;
		}
	}
	/* ベース表示ﾀｲﾏｶｳﾝﾄ */
	if (Ma_get_clock() == TRUE )
	{
		if(lcd_tim_base > TIMER_OUT)
		{
			lcd_tim_base--;
			if(lcd_tim_base == TIMER_OUT)
			{
				lcd_base_mode = OFF;
			}
		}
	}
}


/*------------lcd_mode.cから移動----------------------*/
/****************************************************************
		関数名：lcd_moji_romcpy
		引  数：無し
		戻り値：無し
		機  能：HEXtoASCII処理
				ASCIIﾃﾞｰﾀ入れるRAMは2バイト
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void lcd_moji_romcpy(BYTE *savebuf, BYTE *addfrom)
{
	const BYTE *add;
	
	add =  (const BYTE *)addfrom;
	*savebuf =  *add;
	*(savebuf+1) = *(add+1);
	*(savebuf+2) = *(add+2);
	*(savebuf+3) = *(add+3);
	*(savebuf+4) = *(add+4);
	*(savebuf+5) = *(add+5);
}

/****************************************************************
		関数名：lcd_hextoascii
		引  数：無し
		戻り値：無し
		機  能：HEXtoASCII処理
				ASCIIﾃﾞｰﾀ入れるRAMは2バイト
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_hextoascii( BYTE hex, BYTE *ascii )
{
	*ascii = ( hex/10 ) + 0x30;
	*(ascii+1) = ( hex%10 ) +0x30;
}
/****************************************************************
		関数名：lcd_nodisp
		引  数：dataﾎﾟｲﾝﾀ、dataのsize
		戻り値：無し
		機  能：0の場合、表示しない
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/27		0.1		ha		新規作成
****************************************************************/
void Lcd_nodisp( BYTE *data, BYTE size )
{
	if( *data == 0x30 )
	{
		*data = 0;
	}
	data ++;
	size --;
	while(size)
	{
		if( *(data - 1) == 0)
		{
			if( *data == 0x30 )
			{
				*data = 0;
			}
		}
		data ++;
		size --;
	}
}
#undef	_LCD_MAIN_C_
