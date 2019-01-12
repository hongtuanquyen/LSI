/***************************************************************
	NIaü¯ 
	vWFNg¼	F2012NAMFM_RADIOÓÃÞÙ
	t@C¼		Fsef_mode.c
	W[¼	FSUB[h(Audio²®)
	@   \			FAudio²®
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/23		0.1		lrj		VK
****************************************************************/
#define	_SEF_MODE_C_

#include	"../model.h"

#include	"../comm/common.h"
#include	"../disp/lcd_ext.h"
#include	"../key/key_func_ext.h"
#include	"../main/ma_ext.h"
#include	"../audio/Aud_inc.h"

#include	"sbm_def.h"
#include	"sbm_ext.h"
#include	"sbm_sys.h"
#include	"sbm_keytbl.h"

/******************************************************************************
		Ö¼  FSef_lcddata_set
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[h\¦ÃÞ°Àì¬
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void Sef_lcddata_set( TDP_SEL *auddisp )
{
	auddisp->mode = sbm_aud_mode;
	switch(sbm_aud_mode)
	{
		case CSBM_AUD_BAS:
			auddisp->value = Aud_get_bas_step();
		break;
		case CSBM_AUD_TRE:
			auddisp->value = Aud_get_tre_step();
		break;
		case CSBM_AUD_BAL:
			auddisp->value = Aud_get_bal_step();
		break;
		default:
		break;
	}
	
}
/******************************************************************************
		Ö¼  FSef_mode_start
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hüé
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void Sef_mode_start(void)
{
	Vol_mode_cancel();
	sbm_aud_mode = CSBM_AUD_BAS;
	sbm_aud_tim = CSBM_AUD_7S;
	Key_nop();
}
/******************************************************************************
		Ö¼  FSef_mode_start
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hØÖ
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void Sef_mode_chg(void)
{
	if( sbm_aud_mode == CSBM_AUD_BAS )
	{
		sbm_aud_mode = CSBM_AUD_TRE;
		sbm_aud_tim = CSBM_AUD_7S;
	}
	else if( sbm_aud_mode == CSBM_AUD_TRE )
	{
		if (Ma_model_dest() == CMA_MODEL_AM_J)
		{
			sbm_aud_mode = CSBM_AUD_OFF;
			sbm_aud_tim = 0;
		}
		else
		{
			sbm_aud_mode = CSBM_AUD_BAL;
			sbm_aud_tim = CSBM_AUD_7S;
		}
	}
	else if( sbm_aud_mode == CSBM_AUD_BAL )
	{
		sbm_aud_mode = CSBM_AUD_OFF;
		sbm_aud_tim = 0;
	}
	else
	{
		;
	}
	Key_nop();
}
/******************************************************************************
		Ö¼  FSef_mode_start
		ø  	F³µ
		ßèl	F	CSBM_AUD_OFF = 0,
					CSBM_AUD_BAS,
					CSBM_AUD_TRE,
					CSBM_AUD_BAL
		@  \	FAudio²®[h©`FbN
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
BYTE Sef_mode_check(void)
{
	return(sbm_aud_mode);
}
/******************************************************************************
		Ö¼  FSef_mode_start
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hð
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void Sef_mode_cancel(void)
{
	sbm_aud_mode = CSBM_AUD_OFF;
	sbm_aud_tim = 0;
}
/******************************************************************************
		Ö¼  FSef_audio_updw
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hAAudio²®lÏX
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void Sef_audio_updw(BYTE req)
{
	sbm_aud_tim = CSBM_AUD_7S;
	
	switch(sbm_aud_mode)
	{
		case CSBM_AUD_BAS:
			Aud_set_bas_step(req);			/* BASS²® */
		break;
		case CSBM_AUD_TRE:
			Aud_set_tre_step(req);			/* TREBLE²® */
		break;
		case CSBM_AUD_BAL:
			Aud_set_bal_step(req);			/* BALANCE²® */
		break;
		default:
		break;
	}
}
/******************************************************************************
		Ö¼  FSef_key_power_long
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hAPOWER·°LONG
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void Sef_key_pwr_long(void)
{
	if (Ma_get_clock() == TRUE )
	{
		Sef_mode_cancel();
		Clk_adj_start();
		Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	}
	Key_nop();
}
/******************************************************************************
		Ö¼  FSef_key_up_off
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hAUP·°OFF
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void Sef_key_up_long(void)
{
	Sef_audio_updw(UP);
	Key_long_time200();
}
void Sef_key_up_off(void)
{
	Sef_audio_updw(UP);
	Key_nop();
}
/******************************************************************************
		Ö¼  FSef_key_dw_off
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hADOWN·°OFF
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void Sef_key_dw_long(void)
{
	Sef_audio_updw(DW);
	Key_long_time200();
}
void Sef_key_dw_off(void)
{
	Sef_audio_updw(DW);
	Key_nop();
}
/******************************************************************************
		Ö¼  FSef_key_alm_long
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hAALARM·°LONG
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/24		0.1		lrj		VK
******************************************************************************/
void	Sef_key_alm_on(void)
{
	if (Ma_get_alarm() == TRUE )
	{
		Key_long_time2000();
	}
	else
	{
		Key_nop();
	}
}
void Sef_key_alm_long(void)
{
	Sef_mode_cancel();
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Alm_mode_start();
}

void Sef_key_md_off(void)
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
/******************************************************************************
		Ö¼  FSef_mode_initial
		ø  	F³µ
		ßèl	F³µ
		@  \	FAudio²®[hÏú»
----------------------------------------------------------------
C³ð	yNúzyVersionzy¼Oz yà¾z
----------------------------------------------------------------
			2012/7/30		0.1		lrj		VK
******************************************************************************/
void Sef_mode_initial(void)
{
	sbm_aud_tim = 0;
	sbm_aud_mode = CSBM_AUD_OFF;
}

#if 0	/*dlµÈ¢½ßAí*/	/*_TEST_MODE_*/
void SEL_TestAudio_Edit(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length)
{
	Sef_mode_cancel();
	#if 0
	switch(RcvCmd)
	{
		case CTEST_MODE_DIR_VOLUME:
		case CTEST_MODE_BT_VOLUME_SET:
		break;
		case CTEST_MODE_RCH_MAX:
		case CTEST_MODE_LCH_MAX:
		case CTEST_MODE_BAL_CENTER:
			sbm_snd_mode = CSBM_SND_BAL;
			sbm_snd_tim = CSBM_SND_10S;		/* ÛÀ²Ï10b */
			Vol_mode_cancel();
		break;
		case CTEST_MODE_FFRONT_MAX:
		case CTEST_MODE_FREAR_MAX:
		case CTEST_MODE_FAD_CENTER:
			sbm_snd_mode = CSBM_SND_FAD;
			sbm_snd_tim = CSBM_SND_10S;		/* ÛÀ²Ï10b */
			Vol_mode_cancel();
		break;
		case CTEST_MODE_BASS_MAX:
		case CTEST_MODE_BASS_MIN:
			sbm_snd_mode = CSBM_SND_BAS;
			sbm_snd_bas  = CSBM_SND_BAS_G;
			fsbm_bas_adjust = ON;
			fsbm_mid_adjust = ON;
			fsbm_tre_adjust = ON;
			sbm_snd_tim = CSBM_SND_10S;		/* ÛÀ²Ï10b */
			Vol_mode_cancel();
		break;
		case CTEST_MODE_TREBLE_MAX:
		case CTEST_MODE_TREBLE_MIN:
			sbm_snd_mode = CSBM_SND_TRE;
			sbm_snd_bas  = CSBM_SND_TRE_G;
			fsbm_bas_adjust = ON;
			fsbm_mid_adjust = ON;
			fsbm_tre_adjust = ON;
			sbm_snd_tim = CSBM_SND_10S;		/* ÛÀ²Ï10b */
			Vol_mode_cancel();
		break;
		case CTEST_MODE_BASTRE_CENTER:
			sbm_snd_mode = CSBM_SND_BAS;
			sbm_snd_bas  = CSBM_SND_BAS_G;
			fsbm_bas_adjust = ON;
			fsbm_mid_adjust = ON;
			fsbm_tre_adjust = ON;
			sbm_snd_tim = CSBM_SND_10S;		/* ÛÀ²Ï10b */
			Vol_mode_cancel();
		break;
		case CTEST_MODE_MBEX:
			sbm_snd_mode = CSBM_SND_MABEX;
			sbm_snd_tim = CSBM_SND_10S;		/* ÛÀ²Ï10b */
			Vol_mode_cancel();
		break;
		case CTEST_MODE_BEQ:
			if(RcvBuf[0] == 0x00)
			{
				Lcd_1shot_disp(CLCD_FLAT,CLCD_1SHOT_2S);
				Aud_edit_flat();
			}
			else
			{
				sbm_snd_mode = CSBM_SND_ZEH;
				sbm_snd_tim = CSBM_SND_10S;		/* ÛÀ²Ï10b */
			}
			Vol_mode_cancel();
		break;
		case CTEST_MODE_SUBWOOFER:
			sbm_snd_mode = CSBM_SND_SUBLPF;
			sbm_snd_tim = CSBM_SND_10S;		/* ÛÀ²Ï10b */
			Vol_mode_cancel();
		break;
		default:
		break;
	
	}
	AUD_Test_Func(RcvCmd,RcvBuf,Length);
	#endif
}

#endif	/*_TEST_MODE_*/


#undef _CLOCK_MODE_C_
