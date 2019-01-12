/******************************************************************************
..     ƒNƒ‰ƒŠƒIƒ““aŒü‚¯                    2007ÓÃŞÙ

		ƒvƒƒWƒFƒNƒg–¼	:
		ƒtƒ@ƒCƒ‹–¼		: ma_sys.c
		‹@”\			: Main¼½ÃÑÒ¯¾°¼Şó•tˆ—ˆ—
------------------------------------------------------------------------------
		C³—š—ğ	y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051125]	V‹Kì¬
[sun060712] USBÓ°ÄŞ‘Î‰
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
.. 		ŠÖ”–¼  FMa_sys_message
		ˆø  ”	FSystemÒ¯¾°¼Ş
		–ß‚è’l	F–³‚µ
		‹@  ”\	FMainSystemÒ¯¾°¼Şó•tˆ—
		C³—š—ğ	y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051129] V‹Kì¬
[sun070410] ModePowerOFFˆ—’Ç‰Á
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
.. 		ŠÖ”–¼  FMa_main
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FMainÓ¼Ş­°ÙÒ²İˆ—
		C³—š—ğ	y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051129] V‹Kì¬
******************************************************************************/
static void Ma_main(void)
{
	Ma_mode_main();					/* MainÓ°ÄŞØ‘ÖŠÄ‹ˆ— */
	Ma_tim_main();					/* ¼½ÃÑÍŞ°½À²Ï¶³İÄˆ— */
}
/******************************************************************************
.. 		ŠÖ”–¼  FMa_10ms_timer
		ˆø  ”	F–³‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	FMainÓ¼Ş­°Ù10msŠ„‚İÀ²Ïˆ—
		C³—š—ğ	y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun060106] V‹Kì¬
******************************************************************************/
void Ma_10ms_timer(void)
{
	if (ma_mutetimer > TIMER_OUT)
	{
		ma_mutetimer--;
	}
}

/******************************************************************************
.. 		ŠÖ”–¼  FMa_initial
		ˆø  ”	F‚È‚µ
		–ß‚è’l	F–³‚µ
		‹@  ”\	F‰Šúİ’è“Ç‚İ‚İ
				F“à•”•Ï”‰Šúİ’è
		C³—š—ğ	y”NŒ“úzyVersionzy–¼‘Oz yà–¾z
------------------------------------------------------------------------------
[sun051128] ‚Æ‚è‚ ‚¦‚¸İ’è‚µ‚Ä’u‚­
[sun060621] HEX–ˆ‚ÉdŒü‚¯İ’è‚·‚é‚æ‚¤‚É•ÏX
[sun090403] MainLastÓ°ÄŞ‚ÍTuner‚É‰Šúİ’è
[sun100529] ‰Šúİ’èd—l‘Î‰
[sun100610] ‰Šúİ’èd—lPort•\‚Æ“¯‚¶‚É•ÏX
******************************************************************************/
static void Ma_initial(void)
{
	/*------------------------------
	   MainÓ°ÄŞ‰Šúİ’è
	------------------------------*/
	memset(&ma_mode_cont[0],CMA_NOCONN,sizeof(ma_mode_cont));
	ma_mode_cont[CMA_ID_TU] = CMA_TUNER;
	ma_mode_cont[CMA_ID_AUX1] = CMA_AUX;
	
	if ((ma_ini_data.fac1[0] == 0x5A)&&(ma_ini_data.fac1[1] == 0xA5)&&
		(ma_ini_data.fac2[0] == 0x5A)&&(ma_ini_data.fac2[1] == 0xA5)&&
		(ma_ini_data.area >= 0x00)&&(ma_ini_data.area <= 0x07))
	{
		if ((ma_ini_data.area == 0x00))		/* dŒü‚¯NG */
		{
			ma_ini_data.ini_status = CMA_INIT_STA_AREA_NG;
		}
		else								/* dŒü‚¯OK */
		{
			/* Œv‚ÆƒAƒ‰[ƒ€”ÍˆÍƒ`ƒFƒbƒN */
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
	else	/* ‰Šú‰»ˆ—s‚¤ */
	{
		ma_dest = 0;
		ma_ini_data.ini_status = CMA_INIT_STA_ALL_NG;
	}
	
	switch(ma_ini_data.ini_status)
	{
		case CMA_INIT_STA_ALL_OK:
			if ((ma_dest >= 2)&&(ma_dest <= 7))
			{
				/* ILL_DET “ü—Í */
				MIO_CHG_pILL_DET(IN);
	
				/* AlarmKeyŒŸo‹–‰Â */
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
				/* ILL_DET “ü—Í */
				MIO_CHG_pILL_DET(IN);
				
				/* AlarmKeyŒŸo‹–‰Â */
				Io_alarmkey_enable();
			}
			Ma_mode_initialset();
			break;
		
		case CMA_INIT_STA_ALL_NG:
			/* ‰Šú‰»ˆ— */
				/* 5AA5 00 FF FF */
				/* AM—Ìˆæ F‘S0xFF */
				/* FM1—ÌˆæF‘S0xFF */
				/* FM2—ÌˆæF‘S0xFF */
				/* 5AA5 */
			EEP_Initial_Req();
			break;
		
		case CMA_INIT_STA_AREA_NG:
		default:
			break;
	}
	
	
	/*------------------------------
	   ÍŞ°½À²Ï¶³İÄ—p•Ï”‰Šúİ’è
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
