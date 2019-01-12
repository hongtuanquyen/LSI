/***************************************************************
	クラリオン殿向け       
	プロジェクト名	：'12年モデル
	ファイル名		：lcd_send.c
	モジュール名	：LCD(表示)
	機   能			：LCD送信処理、表示データ編集処理ﾌｧｲﾙ
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
#define	_LCD_SEND_C_

#include	<stdio.h>
#include	<string.h> 
#include	<stdlib.h>

#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#include	"../power/pw_func_ext.h"

#include	"lcd_font.h"
#include	"lcd_ext.h"
#include	"lcd_mode.h"
#include	"lcd_main.h"
#include	"lcd_send.h"

/****************************************************************
		関数名：lcd_edit
		引  数：無し
		戻り値：無し
		機  能：表示データ編集
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
void Lcd_edit( void )
{
/* seg1の編集設定 */
	Lcd_setseg_1();

/* seg2の編集設定 */
	Lcd_setseg_2();

/* seg3の編集設定 */
	Lcd_setseg_3();

/* seg4の編集設定 */
	Lcd_setseg_4();

/* seg5の編集設定 */
	Lcd_setseg_5();

}
/****************************************************************
		関数名：lcd_setseg_1
		引  数：①asciiコード ②セグメント種類
		戻り値：テーブルから取得ﾃﾞｰﾀ（tbldatakeep)
		機  能：SEG1のセグメントﾃﾞｰﾀ設定処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
void Lcd_setseg_1( void )
{
	if( lcd_seg_buf[0] == 0 )		/* 0の場合" " */
	{
		return;
	}
		lcd_font1_buf.font_buf[0] = FONT_SEG1_TBL[lcd_seg_buf[0]-0x30][0];
		lcd_font1_buf.font_buf[1] = FONT_SEG1_TBL[lcd_seg_buf[0]-0x30][1];

		lcd_send_buf.record.seg1_a = lcd_font1_buf.font.buf_a;
		lcd_send_buf.record.seg1_b = lcd_font1_buf.font.buf_b;
		lcd_send_buf.record.seg1_c = lcd_font1_buf.font.buf_c;
		lcd_send_buf.record.seg1_d = lcd_font1_buf.font.buf_d;
		lcd_send_buf.record.seg1_e = lcd_font1_buf.font.buf_e;
		lcd_send_buf.record.seg1_f = lcd_font1_buf.font.buf_f;
		lcd_send_buf.record.seg1_g = lcd_font1_buf.font.buf_g;
		lcd_send_buf.record.seg1_h = lcd_font1_buf.font.buf_h;
		lcd_send_buf.record.seg1_i = lcd_font1_buf.font.buf_i;
}
/****************************************************************
		関数名：lcd_setseg_2
		引  数：①asciiコード ②セグメント種類
		戻り値：テーブルから取得ﾃﾞｰﾀ（tbldatakeep)
		機  能：SEG1のセグメントﾃﾞｰﾀ設定処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
void Lcd_setseg_2( void )
{
	if( lcd_seg_buf[1] == 0 )		/* 0の場合" " */
	{
		return;
	}
		lcd_font2_buf.font_buf[0] = FONT_SEG2_TBL[lcd_seg_buf[1]-0x2A][0];
		lcd_font2_buf.font_buf[1] = FONT_SEG2_TBL[lcd_seg_buf[1]-0x2A][1];

		lcd_send_buf.record.seg2_a = lcd_font2_buf.font.buf_a;
		lcd_send_buf.record.seg2_b = lcd_font2_buf.font.buf_b;
		lcd_send_buf.record.seg2_c = lcd_font2_buf.font.buf_c;
		lcd_send_buf.record.seg2_d = lcd_font2_buf.font.buf_d;
		lcd_send_buf.record.seg2_e = lcd_font2_buf.font.buf_e;
		lcd_send_buf.record.seg2_f = lcd_font2_buf.font.buf_f;
		lcd_send_buf.record.seg2_g = lcd_font2_buf.font.buf_g;
		lcd_send_buf.record.seg2_h = lcd_font2_buf.font.buf_h;
}
/****************************************************************
		関数名：lcd_setseg_3
		引  数：①asciiコード ②セグメント種類
		戻り値：テーブルから取得ﾃﾞｰﾀ（tbldatakeep)
		機  能：SEG3のセグメントﾃﾞｰﾀ設定処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
void Lcd_setseg_3( void )
{
	if( lcd_seg_buf[2] == 0 )		/* 0の場合" " */
	{
		return;
	}
		lcd_font3_buf.font_buf[0] = FONT_SEG3_TBL[lcd_seg_buf[2]-0x2A][0];
		lcd_font3_buf.font_buf[1] = FONT_SEG3_TBL[lcd_seg_buf[2]-0x2A][1];

		lcd_send_buf.record.seg3_a = lcd_font3_buf.font.buf_a;
		lcd_send_buf.record.seg3_b = lcd_font3_buf.font.buf_b;
		lcd_send_buf.record.seg3_c = lcd_font3_buf.font.buf_c;
		lcd_send_buf.record.seg3_d = lcd_font3_buf.font.buf_d;
		lcd_send_buf.record.seg3_e = lcd_font3_buf.font.buf_e;
		lcd_send_buf.record.seg3_f = lcd_font3_buf.font.buf_f;
		lcd_send_buf.record.seg3_g = lcd_font3_buf.font.buf_g;
		lcd_send_buf.record.seg3_h = lcd_font3_buf.font.buf_h;
		lcd_send_buf.record.seg3_i = lcd_font3_buf.font.buf_i;
}
/****************************************************************
		関数名：lcd_setseg_4
		引  数：①asciiコード ②セグメント種類
		戻り値：テーブルから取得ﾃﾞｰﾀ（tbldatakeep)
		機  能：SEG4のセグメントﾃﾞｰﾀ設定処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
void Lcd_setseg_4( void )
{
	if( lcd_seg_buf[3] == 0 )		/* 0の場合" " */
	{
		return;
	}
		lcd_font4_buf.font_buf[0] = FONT_SEG4_TBL[lcd_seg_buf[3]-0x30][0];
		lcd_font4_buf.font_buf[1] = FONT_SEG4_TBL[lcd_seg_buf[3]-0x30][1];

		lcd_send_buf.record.seg4_a = lcd_font4_buf.font.buf_a;
		lcd_send_buf.record.seg4_b = lcd_font4_buf.font.buf_b;
		lcd_send_buf.record.seg4_c = lcd_font4_buf.font.buf_c;
		lcd_send_buf.record.seg4_d = lcd_font4_buf.font.buf_d;
		lcd_send_buf.record.seg4_e = lcd_font4_buf.font.buf_e;
		lcd_send_buf.record.seg4_f = lcd_font4_buf.font.buf_f;
		lcd_send_buf.record.seg4_g = lcd_font4_buf.font.buf_g;
}
/****************************************************************
		関数名：lcd_setseg_5
		引  数：①asciiコード ②セグメント種類
		戻り値：テーブルから取得ﾃﾞｰﾀ（tbldatakeep)
		機  能：SEG5のセグメントﾃﾞｰﾀ設定処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
void Lcd_setseg_5( void )
{
	if( lcd_seg_buf[4] == 0 )		/* 0の場合" " */
	{
		return;
	}
		lcd_font5_buf.font_buf[0] = FONT_SEG5_TBL[lcd_seg_buf[4]-0x30][0];
		lcd_font5_buf.font_buf[1] = FONT_SEG5_TBL[lcd_seg_buf[4]-0x30][1];
		
		lcd_send_buf.record.seg5_a = lcd_font5_buf.font.buf_a;
		lcd_send_buf.record.seg5_b = lcd_font5_buf.font.buf_b;
		lcd_send_buf.record.seg5_c = lcd_font5_buf.font.buf_c;
		lcd_send_buf.record.seg5_d = lcd_font5_buf.font.buf_d;
		lcd_send_buf.record.seg5_e = lcd_font5_buf.font.buf_e;
		lcd_send_buf.record.seg5_f = lcd_font5_buf.font.buf_f;
		lcd_send_buf.record.seg5_g = lcd_font5_buf.font.buf_g;

}
void Lcd_test_edit(void)
{
	BYTE com_b;
	BYTE seg_b1;
	BYTE seg_b2;
	BYTE seg_b3;
	
	BYTE com_1;
	BYTE com_2;
	BYTE com_3;
	BYTE t_s1;
	BYTE t_s2;
	BYTE t_s3;
	BYTE t_s4;
	BYTE t_s5;
	BYTE t_s6;
	BYTE t_s7;
	BYTE t_s8;
	BYTE t_s9;
	BYTE t_s10;
	BYTE t_s11;
	BYTE t_s12;
	BYTE t_s13;
	BYTE t_s14;
	BYTE t_s15;
	BYTE t_s16;
	BYTE t_s17;
	BYTE t_s18;
	
	com_b  = lcd_test_buf[0];
	seg_b1 = lcd_test_buf[1];
	seg_b2 = lcd_test_buf[2];
	seg_b3 = lcd_test_buf[3];
	
	com_1 = com_b & 0x04;
	com_2 = com_b & 0x02;
	com_3 = com_b & 0x01;
	t_s1  = seg_b1 & 0x80;
	t_s2  = seg_b1 & 0x40;
	t_s3  = seg_b1 & 0x20;
	t_s4  = seg_b1 & 0x10;
	t_s5  = seg_b1 & 0x08;
	t_s6  = seg_b1 & 0x04;
	t_s7  = seg_b1 & 0x02;
	t_s8  = seg_b1 & 0x01;
	t_s9  = seg_b2 & 0x80;
	t_s10 = seg_b2 & 0x40; 
	t_s11 = seg_b2 & 0x20; 
	t_s12 = seg_b2 & 0x10; 
	t_s13 = seg_b2 & 0x08; 
	t_s14 = seg_b2 & 0x04; 
	t_s15 = seg_b2 & 0x02; 
	t_s16 = seg_b2 & 0x01; 
	t_s17 = seg_b3 & 0x80;
	t_s18 = seg_b3 & 0x40;
	
	
	
	
	#if 1
	/* S1×COM */
	if ((t_s1 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d1 = 1;
	}
	if ((t_s1 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d2 = 1;
	}
	if ((t_s1 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d3 = 1;
	}
	
	/* S2×COM */
	if ((t_s2 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d4 = 1;
	}
	if ((t_s2 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d5 = 1;
	}
	if ((t_s2 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d6 = 1;
	}

	/* S3×COM */
	if ((t_s3 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d7 = 1;
	}
	if ((t_s3 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d8 = 1;
	}
	if ((t_s3 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d9 = 1;
	}

	/* S4×COM */
	if ((t_s4 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d10 = 1;
	}
	if ((t_s4 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d11 = 1;
	}
	if ((t_s4 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d12 = 1;
	}

	/* S5×COM */
	if ((t_s5 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d13 = 1;
	}
	if ((t_s5 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d14 = 1;
	}
	if ((t_s5 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d15 = 1;
	}

	/* S6×COM */
	/* DT16 */
	if ((t_s6 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d16 = 1;
	}
	/* DT17 */
	if ((t_s6 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d17 = 1;
	}
	/* DT18 */
	if ((t_s6 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d18 = 1;
	}

	/* S7×COM */
	/* DT19 */
	if ((t_s7 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d19 = 1;
	}
	/* DT20 */
	if ((t_s7 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d20 = 1;
	}
	/* DT21 */
	if ((t_s7 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d21 = 1;
	}

	/* S8×COM */
	/* DT22 */
	if ((t_s8 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d22 = 1;
	}
	
	/* DT23 */
	if ((t_s8 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d23 = 1;
	}
	/* DT24 */
	if ((t_s8 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d24 = 1;
	}

	/* S9×COM */
	/* DT25 */
	if ((t_s9 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d25 = 1;
	}
	/* DT26 */
	if ((t_s9 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d26 = 1;
	}
	/* DT27 */
	if ((t_s9 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d27 = 1;
	}

	/* S10×COM */
	/* DT28 */
	if ((t_s10 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d28 = 1;
	}
	/* DT29 */
	if ((t_s10 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d29 = 1;
	}
	/* DT30 */
	if ((t_s10 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d30 = 1;
	}

	/* S11×COM */
	/* DT31 */
	if ((t_s11 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d31 = 1;
	}
	/* DT32 */
	if ((t_s11 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d32 = 1;
	}
	/* DT33 */
	if ((t_s11 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d33 = 1;
	}

	/* S12×COM */
	/* DT34 */
	if ((t_s12 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d34 = 1;
	}
	/* DT35 */
	if ((t_s12 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d35 = 1;
	}
	/* DT36 */
	if ((t_s12 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d36 = 1;
	}

	/* S13×COM */
	/* DT37 */
	if ((t_s13 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d37 = 1;
	}
	/* DT38 */
	if ((t_s13 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d38 = 1;
	}
	/* DT39 */
	if ((t_s13 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d39 = 1;
	}

	/* S14×COM */
	/* DT40 */
	if ((t_s14 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d40 = 1;
	}
	/* DT41 */
	if ((t_s14 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d41 = 1;
	}
	/* DT42 */
	if ((t_s14 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d42 = 1;
	}

	/* S15×COM */
	/* DT43 */
	if ((t_s15 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d43 = 1;
	}
	/* DT44 */
	if ((t_s15 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d44 = 1;
	}
	/* DT45 */
	if ((t_s15 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d45 = 1;
	}

	/* S16×COM */
	/* DT46 */
	if ((t_s16 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d46 = 1;
	}
	/* DT47 */
	if ((t_s16 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d47 = 1;
	}
	/* DT48 */
	if ((t_s16 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d48 = 1;
	}

	/* S17×COM */
	/* DT49 */
	if ((t_s17 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d49 = 1;
	}
	/* DT50 */
	if ((t_s17 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d50 = 1;
	}
	/* DT51 */
	if ((t_s17 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d51 = 1;
	}

	/* S18×COM */
	/* DT52 */
	if ((t_s18 != 0x00)&&(com_1!=0x00))
	{
		lcd_send_buf.record_test.d52 = 1;
	}
	/* DT53 */
	if ((t_s18 != 0x00)&&(com_2!=0x00))
	{
		lcd_send_buf.record_test.d53 = 1;
	}
	/* DT54 */
	if ((t_s18 != 0x00)&&(com_3!=0x00))
	{
		lcd_send_buf.record_test.d54 = 1;
	}
	
	#else
	/* S1×COM */
	/* DT1 */
	if (((seg_b1 & 0x80) == 0x80)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d1 = 1;
	}
	/* DT2 */
	if (((seg_b1 & 0x80) == 0x80)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d2 = 1;
	}
	/* DT3 */
	if (((seg_b1 & 0x80) == 0x80)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d3 = 1;
	}
	
	/* S2×COM */
	/* DT4 */
	if (((seg_b1 & 0x40) == 0x40)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d4 = 1;
	}
	/* DT5 */
	if (((seg_b1 & 0x40) == 0x40)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d5 = 1;
	}
	/* DT6 */
	if (((seg_b1 & 0x40) == 0x40)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d6 = 1;
	}

	/* S3×COM */
	/* DT7 */
	if (((seg_b1 & 0x20) == 0x20)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d7 = 1;
	}
	/* DT8 */
	if (((seg_b1 & 0x20) == 0x20)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d8 = 1;
	}
	/* DT9 */
	if (((seg_b1 & 0x20) == 0x20)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d9 = 1;
	}

	/* S4×COM */
	/* DT10 */
	if (((seg_b2 & 0x10) == 0x10)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d10 = 1;
	}
	/* DT11 */
	if (((seg_b2 & 0x10) == 0x10)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d11 = 1;
	}
	/* DT12 */
	if (((seg_b2 & 0x10) == 0x10)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d12 = 1;
	}

	/* S5×COM */
	/* DT13 */
	if (((seg_b2 & 0x08) == 0x08)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d13 = 1;
	}
	/* DT14 */
	if (((seg_b2 & 0x08) == 0x08)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d14 = 1;
	}
	/* DT15 */
	if (((seg_b2 & 0x08) == 0x08)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d15 = 1;
	}

	/* S6×COM */
	/* DT16 */
	if (((seg_b2 & 0x04) == 0x04)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d16 = 1;
	}
	/* DT17 */
	if (((seg_b3 & 0x04) == 0x04)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d17 = 1;
	}
	/* DT18 */
	if (((seg_b3 & 0x02) == 0x02)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d18 = 1;
	}

	/* S7×COM */
	/* DT19 */
	if (((seg_b1 & 0x02) == 0x02)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d19 = 1;
	}
	/* DT20 */
	if (((seg_b1 & 0x01) == 0x01)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d20 = 1;
	}
	/* DT21 */
	if (((seg_b1 & 0x01) == 0x01)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d21 = 1;
	}

	/* S8×COM */
	/* DT22 */
	if (((seg_b1 & 0x01) == 0x01)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d22 = 1;
	}
	
	/* DT23 */
	if (((seg_b1 & 0x08) == 0x08)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d23 = 1;
	}
	/* DT24 */
	if (((seg_b1 & 0x04) == 0x04)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d24 = 1;
	}

	/* S9×COM */
	/* DT25 */
	if (((seg_b1 & 0x02) == 0x02)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d25 = 1;
	}
	/* DT26 */
	if (((seg_b1 & 0x01) == 0x01)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d26 = 1;
	}
	/* DT27 */
	if (((seg_b2 & 0x80) == 0x80)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d27 = 1;
	}

	/* S10×COM */
	/* DT28 */
	if (((seg_b2 & 0x40) == 0x40)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d28 = 1;
	}
	/* DT29 */
	if (((seg_b2 & 0x20) == 0x20)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d29 = 1;
	}
	/* DT30 */
	if (((seg_b2 & 0x10) == 0x10)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d30 = 1;
	}

	/* S11×COM */
	/* DT31 */
	if (((seg_b2 & 0x80) == 0x80)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d31 = 1;
	}
	/* DT32 */
	if (((seg_b2 & 0x40) == 0x40)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d32 = 1;
	}
	/* DT33 */
	if (((seg_b2 & 0x20) == 0x20)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d33 = 1;
	}

	/* S12×COM */
	/* DT34 */
	if (((seg_b2 & 0x10) == 0x10)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d34 = 1;
	}
	/* DT35 */
	if (((seg_b3 & 0x80) == 0x80)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d35 = 1;
	}
	/* DT36 */
	if (((seg_b3 & 0x40) == 0x40)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d36 = 1;
	}

	/* S13×COM */
	/* DT37 */
	if (((seg_b1 & 0x80) == 0x80)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d37 = 1;
	}
	/* DT38 */
	if (((seg_b1 & 0x40) == 0x40)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d38 = 1;
	}
	/* DT39 */
	if (((seg_b1 & 0x20) == 0x20)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d39 = 1;
	}

	/* S14×COM */
	/* DT40 */
	if (((seg_b1 & 0x10) == 0x10)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d40 = 1;
	}
	/* DT41 */
	if (((seg_b1 & 0x08) == 0x08)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d41 = 1;
	}
	/* DT42 */
	if (((seg_b1 & 0x04) == 0x04)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d42 = 1;
	}

	/* S15×COM */
	/* DT43 */
	if (((seg_b1 & 0x02) == 0x02)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d43 = 1;
	}
	/* DT44 */
	if (((seg_b1 & 0x01) == 0x01)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d44 = 1;
	}
	/* DT45 */
	if (((seg_b2 & 0x80) == 0x80)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d45 = 1;
	}

	/* S16×COM */
	/* DT46 */
	if (((seg_b2 & 0x40) == 0x40)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d46 = 1;
	}
	/* DT47 */
	if (((seg_b2 & 0x20) == 0x20)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d47 = 1;
	}
	/* DT48 */
	if (((seg_b2 & 0x10) == 0x10)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d48 = 1;
	}

	/* S17×COM */
	/* DT49 */
	if (((seg_b2 & 0x08) == 0x08)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d49 = 1;
	}
	/* DT50 */
	if (((seg_b2 & 0x04) == 0x04)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d50 = 1;
	}
	/* DT51 */
	if (((seg_b2 & 0x02) == 0x02)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d51 = 1;
	}

	/* S18×COM */
	/* DT52 */
	if (((seg_b2 & 0x01) == 0x01)&&((com_b&0x04) == 0x04))
	{
		lcd_send_buf.record_test.d52 = 1;
	}
	/* DT53 */
	if (((seg_b3 & 0x80) == 0x80)&&((com_b&0x02) == 0x02))
	{
		lcd_send_buf.record_test.d53 = 1;
	}
	/* DT54 */
	if (((seg_b3 & 0x40) == 0x40)&&((com_b&0x01) == 0x01))
	{
		lcd_send_buf.record_test.d54 = 1;
	}
	#endif
}

/****************************************************************
		関数名：lcd_send
		引  数：無し
		戻り値：無し
		機  能：LCD送信処理
----------------------------------------------------------------
修正履歴	【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
			2012/07/30		0.1		ha		新規作成
****************************************************************/
#define		CLCD_DRIVER_ADR		0x42
void Lcd_send( void )
{
	BYTE i;

	
	MIO_OUT_pLCDCE( LOW );		/* LCD CE    "L" */
	for( i=1; i<4; i++ )
	{
		IO_lcd_ccb(CLCD_DRIVER_ADR);	/* CCBアドレス送信 */
		
		MIO_OUT_pLCDCE( HIGH );				/* LCD CE  "H" */
		switch(i)
		{
			case 1:
				IO_lcd_send( lcd_send_buf.buf_byte,SENDBYTE );		/* 1レコード */
				break;
			case 2:
				IO_lcd_send( &lcd_send_buf.buf_byte[RECORD2],SENDBYTE );		/* 2レコード */
				break;
			case 3:
				IO_lcd_send( &lcd_send_buf.buf_byte[RECORD3],SENDBYTE );		/* 3レコード */
				break;
			default:
				break;
		}
		MIO_OUT_pLCDCE( LOW );		/* LCD CE    "L" */
	}
	flcd_send = OFF;
	/* flcd_accoff_send = OFF; */
	flcd_blank = OFF;				/* ﾌﾞﾗﾝｸ送信要求ﾌﾗｸﾞクリア */
}

#undef	_LCD_SEND_C_
