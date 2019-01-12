/***************************************************************
	ƒNƒ‰ƒŠƒIƒ““aŒü‚¯ 
	ƒvƒƒWƒFƒNƒg–¼	F2012”NAMFM_RADIOÓÃÞÙ
	ƒtƒ@ƒCƒ‹–¼		Fclock_mode.c
	ƒ‚ƒWƒ…[ƒ‹–¼	FSUBƒ‚[ƒh(ŽžŒv)
	‹@   ”\			FŽžŒv¶³ÝÄˆ—AŽžŒv’²®ˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		V‹K
****************************************************************/
#define	_CLOCK_MODE_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#include	"../disp/lcd_ext.h"
#include	"../key/key_func_ext.h"
#include	"../main/ma_ext.h"
#include	"../tuner/tu_key.h"
#include	"../audio/Aud_inc.h"

#include	"sbm_def.h"
#include	"sbm_ext.h"
#include	"sbm_sys.h"
#include	"sbm_keytbl.h"

static void Clk_io_data_set(void);
static void Clk_Acc_Sta_Monitor(void);
static BYTE acc_sta;
static BYTE acc_sta_bak;
static BYTE pwr_sta;
static BYTE pwr_sta_bak;

/******************************************************************************
		ŠÖ”–¼  FClk_lcddata_set
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv•\Ž¦ÃÞ°Àì¬ˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		V‹K
******************************************************************************/
void	Clk_lcddata_set( TDP_CLK *clkdisp )
{
	if (Alm_mode_check() != OFF)
	{
		/* alm ‚Ì•\Ž¦ƒf[ƒ^‚ð“n‚· */
		Alm_lcddata_set(clkdisp);
	}
	else
	{
		if (sbm_clk_adj.hour == 0)
		{
			clkdisp->hour = 12;
		}
		else if ((sbm_clk_adj.hour>=13)&&(sbm_clk_adj.hour<=23))
		{
			clkdisp->hour = sbm_clk_adj.hour - 12;
		}
		else
		{
			clkdisp->hour = sbm_clk_adj.hour;
		}
		clkdisp->min = sbm_clk_adj.min;
		
		clkdisp->mode = sbm_clk_mode;
	}
}

BYTE Clk_indicator_set(void)
{
	BYTE	indicator;
	
	indicator = 0;
	
	#if 0
	if (Alm_mode_check() != OFF)
	{
		indicator = Alm_indicator_set();
	}
	else
	#endif
	{
		if ((sbm_clk_adj.hour >= 12)&&(sbm_clk_adj.hour <= 23))
		{
			indicator |= CLCD_CL_IND_PM;
		}
		else
		{
			indicator |= CLCD_CL_IND_AM;
		}
	}
	
	#if 0
	if( fsbm_alm_onoff == ON )
	{
		indicator |= CLCD_CL_IND_AL;
	}
	#endif
	
	return(indicator);
}
/******************************************************************************
		ŠÖ”–¼  FClk_time_main
		ˆø  ”	F‚È‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒvÀ²ÏŽæ“¾ˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		V‹K
******************************************************************************/
void	Clk_time_main(void)
{
	WORD io_min;
	
	if (Ma_get_clock() == FALSE )
	{
		return;
	}
	
	//Pw_acc_wakeup();
	io_min = Io_mincnt_read();
	
	/* IO‚©‚çÀ²Ï‚ðŽæ“¾ */
	sbm_clk_adj.hour = (BYTE)(io_min/60);
	sbm_clk_adj.min = (BYTE)(io_min%60);
	
	if (( sbm_clk_bak.min != sbm_clk_adj.min )
	  ||( sbm_clk_bak.hour != sbm_clk_adj.hour ))
	{
		sbm_clk_bak = sbm_clk_adj;
		if (Pw_ACC_status_get() == OFF)
		{
			/* Lcd_send_req(); */
			Lcd_accoff_send_req();
		}
	}
	
	/* ALARMƒ`ƒFƒbƒNˆ— */
	#if 0
	if  (( fsbm_alm_onoff == ON )
		&&( fsbm_beep_en == ON )
		&&( Alm_mode_check() == OFF )
		&&( sbm_clk_adj.min == sbm_alm_adj.min )
		&&( sbm_clk_adj.hour == sbm_alm_adj.hour ))
	{
		/* BEEP‚·‚é */
		if (fsbm_alm_beepon == OFF)/* ALARM‹N“®ó‘ÔÝ’è */
		{
			fsbm_alm_beepon = ON;
			fsbm_beep_en = OFF;
			Aud_set_beep(CAUD_REQ_BEEP_ALARM);
			/* 180Sƒ^ƒCƒ€Ý’è */
			sbm_beepon_tim = CSBM_BEEPON_180S;
			//if (Pw_ACC_status_get() == OFF)
			{
				Pw_acc_wakeup_req(ON);
			}
		}
	}
	#else
	
	if (Ma_get_alarm() == TRUE)
	{
		if  (( sbm_clk_adj.min == sbm_alm_adj.min )
			&&( sbm_clk_adj.hour == sbm_alm_adj.hour ))
		{
			if  (( fsbm_alm_onoff == ON )
				&&( Alm_mode_check() == OFF )
				&&( Clk_adj_check() == OFF )
				&&( fsbm_alm_beepon == OFF )
				&&( fsbm_beep_en == ON ))
			{
				fsbm_alm_beepon = ON;
				fsbm_beep_en = OFF;
				if (Pw_Power_status_get()  == ON)
				{
					Sbm_mode_cancel(CSBM_OFF);	
					Aud_cancel_beep();
					/* beep req */
					/* PowerON‚¶‚á‚È‚¯‚ê‚ÎAPW‚ªŠÇ—‚·‚é */
					Aud_set_beep(CAUD_REQ_BEEP_ALARM);
				}
				else
				//if (Pw_ACC_DET_get() == OFF)
				{
					Pw_acc_wakeup_req(ON);
				}
			}
		}
		else
		{
			fsbm_beep_en = ON;
		}
	}
	#endif
	
	/*	accó‘ÔŠÄŽ‹ˆ—	*/
	Clk_Acc_Sta_Monitor();
	
	
}

void Clk_Acc_Sta_Monitor(void)
{
	acc_sta = Pw_ACC_DET_get();
	
	if (acc_sta_bak != acc_sta)
	{
		Alm_beep_key_stop();
		acc_sta_bak = acc_sta;
		Lcd_disp_base(Ma_current_mode());
	}
	
	pwr_sta = Pw_Power_status_get();
	
	if (pwr_sta_bak != pwr_sta)
	{
		if (fsbm_alm_off_req == TRUE)
		{
			Alm_mode_cancel();
			fsbm_alm_off_req = FALSE;
		}
		Clk_adj_cancel();
		pwr_sta_bak = pwr_sta;
	}
}
/******************************************************************************
		ŠÖ”–¼  FClk_key_up_off
		ˆø  ”	Fvoid
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒvŽž‰Šú‰»ˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/30		0.1		lrj		V‹K
******************************************************************************/
void Clk_mode_initial(void)
{
	if (Ma_get_clock() == FALSE )
	{
		return;
	}
	sbm_clk_adj.min = 0x00;
	sbm_clk_adj.hour = 0x01;
	Clk_io_data_set();
	//fsbm_clk_ampm = CSBM_CLK_AM;
	sbm_clk_tim = 0;
	sbm_clk_mode = CSBM_CLK_NORMAL;
	
	acc_sta = 0;
	acc_sta_bak = 0;
	pwr_sta = 0;
	pwr_sta_bak = 0;

}


void Clk_io_data_set(void)
{
	WORD io_min;
	
	io_min = (sbm_clk_adj.hour * 60) + sbm_clk_adj.min;
	if (io_min <= 0x59F)	/* 23F59‚Ü‚Å */
	{
		Io_mincnt_write(io_min);
	}
}
/******************************************************************************
		ŠÖ”–¼  FClk_adj_start
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®Ó°ÄÞŽn‚Ü‚éˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		V‹K
******************************************************************************/
void	Clk_adj_start(void)
{
	if (Ma_get_clock() == FALSE )
	{
		Key_nop();
		return;
	}
	
	Vol_mode_cancel();
	Lcd_base_disp_cancel();
	sbm_clk_mode = CSBM_CLK_ADJUST;				/* Žž‚Ì’²®Ó°ÄÞ‚Ö¾¯Ä */
	sbm_clk_tim = CSBM_CLK_15S;
	Lcd_blktim_set();
	Key_nop();
}
/******************************************************************************
		ŠÖ”–¼  FClk_adj_check
		ˆø  ”	F–³‚µ
		–ß‚è’l	FON ËŽžŒv’²®’†
				FOFF Ë ŽžŒv’²®’†‚Å‚Í‚È‚¢
		‹@  ”\	FŽžŒv’²®Ó°ÄÞ’†‚©ƒ`ƒFƒbƒNˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		V‹K
******************************************************************************/
BYTE	Clk_adj_check(void)
{
	if (sbm_clk_mode != CSBM_CLK_NORMAL)
	{
		return(ON);
	}
	else
	{
		return(OFF);
	}
}
/******************************************************************************
		ŠÖ”–¼  FClk_adj_cancel
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®Ó°ÄÞ’†·¬Ý¾Ùˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void	Clk_adj_cancel(void)
{
	sbm_clk_mode = CSBM_CLK_NORMAL;			/* ’²®Ó°ÄÞ·¬Ý¾Ù */
	sbm_clk_tim = 0;
	fsbm_beep_en = OFF;
	#if 0	/* hour‚Æmin‚ð’²®‚·‚é“x‚ÉAŽžÝ’è‚·‚é */
	Io_seccnt_clear();
	Clk_io_data_set();
	#endif
}

/******************************************************************************
		ŠÖ”–¼  FClk_adj_hour_up
		ˆø  ”	FUP/DW
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒvÓ°ÄÞ’†HOUR‚ÌUP’²®
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_adj_hour_up(void)
{
	/* ŽžƒJƒEƒ“ƒg */
	if( sbm_clk_adj.hour >= 23 )
	{
		sbm_clk_adj.hour = 0;
	}
	else
	{
		sbm_clk_adj.hour++;
	}
	#if 0	/* Clk_io_data_setˆ—‚Å•b‚ð“¯Žž‚ÉƒNƒŠƒA‚·‚é */
	Io_seccnt_clear();
	#endif
	Clk_io_data_set();
	sbm_clk_tim = CSBM_CLK_15S;
	Lcd_blktim_set();
}
/******************************************************************************
		ŠÖ”–¼  FClk_adj_min_up
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒvÓ°ÄÞ’†MIN‚ÌUP’²®
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_adj_min_up(void)
{
	sbm_clk_adj.min++;
	if( sbm_clk_adj.min == 60 )
	{
		sbm_clk_adj.min = 0;
	}
	#if 0	/* Clk_io_data_setˆ—‚Å•b‚ð“¯Žž‚ÉƒNƒŠƒA‚·‚é */
	Io_seccnt_clear();
	#endif
	Clk_io_data_set();
	sbm_clk_tim = CSBM_CLK_15S;
	Lcd_blktim_set();
}
/******************************************************************************
		ŠÖ”–¼  FClk_adj_tim_rst
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒvÓ°ÄÞ’†ŽžƒŠƒXƒgˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_adj_tim_rst(void)
{
	#if 0	/* uŽžƒŠƒZƒbƒg‹@”\v‚ªAu•bƒŠƒZƒbƒg‹@”\v‚É‚È‚Á‚½ */
	if( sbm_clk_adj.min >= 30 )
	{
		Clk_adj_hour_up();
	}
	sbm_clk_adj.min = 0;
	
	Clk_io_data_set();
	sbm_clk_tim = CSBM_CLK_15S;
	#else
	
	#if 0	/* •bƒŠƒZƒbƒg‚àMINƒŒƒWƒXƒ^‘ž‚Þ */
	Io_seccnt_clear();
	#else
	Clk_io_data_set();
	#endif
	Lcd_blktim_set();
	sbm_clk_tim = CSBM_CLK_15S;
	#endif
}
/******************************************************************************
		ŠÖ”–¼  FClk_adj_set
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®Ï‚Ýƒf[ƒ^Šm’è
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void	Clk_adj_set(void)
{
	Clk_adj_cancel();
	Key_nop();
}
/******************************************************************************
		ŠÖ”–¼  FClk_key_up_off
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®ƒ‚[ƒhUP·°OFFˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_key_up_off(void)
{
	if(Ma_model_dest() == CMA_MODEL_AM_J)
	{
		Clk_adj_hour_up();
	}
	else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
	{
		Clk_adj_min_up();
	}
	else
	{
		/* No Operation */
	}
	Key_nop();
}
/******************************************************************************
		ŠÖ”–¼  FClk_key_up_long
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®ƒ‚[ƒhUP·°LONGˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_key_up_long(void)
{
	if(Ma_model_dest() == CMA_MODEL_AM_J)
	{
		Clk_adj_hour_up();
		Key_long_time200();
	}
	else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
	{
		Clk_adj_min_up();
		Key_long_time200();
	}
	else
	{
		/* No Operation */
	}
}
/******************************************************************************
		ŠÖ”–¼  FClk_key_dw_off
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®ƒ‚[ƒhDOWN·°OFFˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_key_dw_off(void)
{
	if(Ma_model_dest() == CMA_MODEL_AM_J)
	{
		Clk_adj_min_up();
	}
	else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
	{
		Clk_adj_hour_up();
	}
	else
	{
		/* No Operation */
	}
	Key_nop();
}
/******************************************************************************
		ŠÖ”–¼  FClk_key_dw_long
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®ƒ‚[ƒhDOWN·°LONGˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_key_dw_long(void)
{
	if(Ma_model_dest() == CMA_MODEL_AM_J)
	{
		Clk_adj_min_up();
		Key_long_time200();
	}
	else if((Ma_model_dest() >= CMA_MODEL_AMFM_J) && (Ma_model_dest() <= CMA_MODEL_AMFM_X))
	{
		Clk_adj_hour_up();
		Key_long_time200();
	}
	else
	{
		/* No Operation */
	}
}
/******************************************************************************
		ŠÖ”–¼  FClk_key_as_off
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®ƒ‚[ƒh‚ÌAS·°OFFˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_key_as_off(void)
{
	Clk_adj_tim_rst();
	Clk_adj_cancel();
	Key_nop();
}

/******************************************************************************
		ŠÖ”–¼  FClk_key_as_off
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FŽžŒv’²®ƒ‚[ƒh‚ÌAS·°OFFˆ—
----------------------------------------------------------------
C³—š—ð	y”NŒŽ“úzyVersionzy–¼‘Oz yà–¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		V‹K
******************************************************************************/
void Clk_key_md_off(void)
{
	BYTE mo;
	
	mo = Ma_current_mode();
	if (mo == CMA_AUX)
	{
		Tu_amband_set();
		Ma_key_src();
	}
	else
	{
		Tu_key_md_off();
	}
}

void Clk_key_pwr_long(void)
{
	Clk_adj_set();
	Key_nop();
}

void Clk_key_voluplong(void)
{
	Clk_adj_cancel();
	Sbm_key_voluplong();
}
void Clk_key_volupoff(void)
{
	Clk_adj_cancel();
	Sbm_key_volupoff();
}
void Clk_key_voldwlong(void)
{
	Clk_adj_cancel();
	Sbm_key_voldwlong();
}
void Clk_key_voldwoff(void)
{
	Clk_adj_cancel();
	Sbm_key_voldwoff();
}

#if _TEST_ALARM_ON_
void Clk_alarm_test(void)
{
	fsbm_alm_beepon = ON;
	fsbm_beep_en = OFF;
	if (Pw_Power_status_get()  == ON)
	{
		Aud_cancel_beep();
		/* beep req */
		/* PowerON‚¶‚á‚È‚¯‚ê‚ÎAPW‚ªŠÇ—‚·‚é */
		Aud_set_beep(CAUD_REQ_BEEP_ALARM);
	}
	else
	{
		Pw_acc_wakeup_req(ON);
	}
}
#endif

#undef _CLOCK_MODE_C_
