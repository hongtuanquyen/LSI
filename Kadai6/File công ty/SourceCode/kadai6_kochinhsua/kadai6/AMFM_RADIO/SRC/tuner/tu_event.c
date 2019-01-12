/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: tu_event.c
		�@�\			: TUNER ����ď���
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051215]	�V�K�쐬
[sun060710] FUNCOn�������ް��\���v���o���Ȃ��悤�ɕύX
[matu120903] S-meter���� Local�l�ǉ� 
******************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_
#define	_TU_EVENT_C_

#include	<string.h>
#include	"../comm/common.h"
#include	"../key/key_func_ext.h"
#include	"../audio/aud_ext.h"
#include	"../disp/lcd_ext.h"
#include	"../main/ma_ext.h"
#include	"../io/IO_inc.h"
#include	"../submode/sbm_ext.h"
#include	"../power/pw_func_ext.h"
#include	"tu_def.h"
#include	"tu_event.h"
#include	"tu_ext.h"
#include	"tu_main.h"
#include	"tu_key.h"
#include	"tu_io.h"
#include	"tu_item.h"

/* �����֐��v���g�^�C�v�錾 */
static void Tu_mixch_up(void);
static void Tu_mixch_down(void);
/******************************************************************************
	��   ��  ��	: Tu_e_bndon
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӰ��On����
    ��       �l :
-------------------------------------------------------------------------------
[sun051223] �V�K�쐬
[sun060709] ��ڵ���o�֎~���1100ms�ɐݒ菈���ǉ�
[sun060710] �ް��\���v���o���Ȃ��悤�ɕύX
[zhang070323]��ڵ���o�֎~���1100ms�ɐݒ���폜
********************************************************************************/
void	Tu_e_func_on(void)
{
	Tu_job_clear();
	tu_m_mode = CTU_M_PWRON;					/* PowerOnӰ�޾�� */
	Tu_stdis_timset(CTU_ST_ANTON);
	Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);	/* TunerMute�v��ON */
	ftu_mute = ON;								/* Mute Flag��� */
}
/******************************************************************************
	��   ��  ��	: Tu_e_func_off
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӰ��Off����
    ��       �l :
-------------------------------------------------------------------------------
[sun051223] �V�K�쐬
[zhang070308]����100ms->2000ms��NoActive�ɾ��
********************************************************************************/
void	Tu_e_func_off(void)
{
	Tu_lastcall();				/* ׽ēǂݏo�� */
	ftu_sttest = OFF;		/* �u070727���vST�\����ǉ�*/
	Tu_job_clear();
	Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);	/* TunerMute�v��ON */
	ftu_mute = ON;								/* Mute Flag��� */
}

/******************************************************************************
	��   ��  ��	: Tu_e_md_off
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: �����[�h�ModeOff����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬 �V�K�쐬
********************************************************************************/
void  Tu_e_md_off(void)
{
	BYTE chg_req;
	
	if(Ma_current_mode() == CMA_ISR)
	{
		Ma_key_src();
	}
#if 0 /* QAC�΍� */
	else if(Ma_current_mode() == CMA_AUX)
	{
		Tu_amband_set();
		Ma_key_src();
	}
#endif
	else
	{
		Tu_job_clear();
		switch(tu_bandno)
		{
			case	CTU_BAND_AM:
			case	CTU_BAND_FM1:
				Tu_BndChange();
				Tu_lastcall();						/* ׽Čďo */
				tu_m_mode = CTU_M_BANDCHG;			/* BandChangeӰ�޾�� */
				Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
				Lcd_disp_base(CMA_TUNER);			
				Sbm_mode_cancel(CSBM_OFF);
				break;
			default:
				Ma_key_src();
				break;
		}
	}
	Tu_key_nop();						/* ���������� */
}

#if 0	/*	11Model�ŐV�d�l[20100519]Band�����������A20100601�A�x��	*/
/******************************************************************************
	��   ��  ��	: Tu_e_bndon
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��Band��On����
    ��       �l :
-------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[zhang070308]seek,ps,as����,�`���|�j���O���|�h�؊��͂ł���
********************************************************************************/
void	Tu_e_bndon(void)
{
	switch (tu_m_mode)
	{
		case	CTU_M_SCAN:
			Tu_lastwrite();
		case	CTU_M_AS:
		case	CTU_M_SEEK:
			Tu_e_bndoff();
			Tu_key_nop();
//			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
			break;
		default:
			Tu_job_clear();
			Key_long_time1000();
			break;
	}
	Sbm_mode_cancel(CMA_TUNER);
}

/******************************************************************************
	��   ��  ��	: Tu_e_bndlong
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��Band��Long����
    ��       �l :
-------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[zhang070308]T-MODE���|�h�؊���ύX
********************************************************************************/
void	Tu_e_bndlong(void)
{
	Tu_job_clear();
	ftu_seek = ~ftu_seek;				/* seek�׸ދt�] */
	Tu_key_nop();						/* ���������� */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	Lcd_disp_base(CMA_TUNER);
}
#endif
/******************************************************************************
	��   ��  ��	: Tu_e_bndoff
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��Band��Off����
    ��       �l :
-------------------------------------------------------------------------------
[sun051221] �V�K�쐬
********************************************************************************/
void	Tu_e_bndoff(void)
{
	if(tu_m_mode == CTU_M_SCAN)
	{
		Tu_lastwrite();
	}
	else
	{
	
	}
	Tu_job_clear();
	Tu_BndChange();						/* BandChange */
	Tu_lastcall();						/* ׽Čďo */
	tu_m_mode = CTU_M_BANDCHG;			/* BandChangeӰ�޾�� */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	Lcd_disp_base(CMA_TUNER);
	Sbm_mode_cancel(CMA_TUNER);
}

/******************************************************************************
	��   ��  ��	: tu_e_updw_on
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��Tuner Up/DOWN �� ON����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_e_updw_on(BYTE updw)
{
	Tu_updw_set(updw);
	switch (tu_m_mode)
	{
		case	CTU_M_SEEK:
		case	CTU_M_SCAN:
			Tu_lastwrite();
			Tu_job_clear();
			tu_m_mode = CTU_M_LSTCAL;
			Sbm_mode_cancel(CMA_TUNER);
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */	
			Lcd_disp_base(CMA_TUNER);
			Tu_key_nop();				/* ������ */
			break;
		default:
			Tu_job_clear();
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
			Key_long_time500();		/*	500ms�^�C�}�[�ݒ�	*/
			break;
	}
}
/******************************************************************************
	��   ��  ��	: Tu_e_updw_long
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��Tuner Up/DOWN �� Long����
    ��       �l :
-------------------------------------------------------------------------------
********************************************************************************/
void	Tu_e_updw_long(BYTE updw)
{
	Tu_job_clear();
	tu_m_mode = CTU_M_SEEK;
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	Tu_key_nop();
}
/******************************************************************************
	��   ��  ��	: Tu_e_updw_off
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��Tuner Up/DOWN �� Off����
    ��       �l :
-------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[zhang070309]CTU_M_CMAN�I����seek��->CTU_M_CMAN�I���㓮�얳
********************************************************************************/
void	Tu_e_updw_off(BYTE updw)
{
	Tu_job_clear();
	tu_m_mode = CTU_M_MAN;		/* �ƭ�ٱ����޳�Ӱ�޾�� */
	Tu_key_nop();				/* ������ */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	#if 0
	if(Ma_current_mode() == CMA_ISR)
	{
		Lcd_disp_base(CMA_ISR);
	}
	else
	{
		Lcd_disp_base(CMA_TUNER);
	}
	#endif
}

/******************************************************************************
	��   ��  ��	: Tu_e_presetlong
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��Preset��Long����
    ��       �l :
-------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun060106] �P��ýĊ�[н1 ��NOP���Ă��Ȃ�]
********************************************************************************/
void	Tu_e_presetlong(BYTE pstno )
{
	Tu_pno_set(pstno);					/* ����PresetNo��� */
	Tu_job_clear();
	tu_m_mode = CTU_M_PSTWT;			/* Preset WriteӰ�޾�� */
	Tu_key_nop();					/* ���������� */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	Lcd_disp_base(CMA_TUNER);
}
/******************************************************************************
	��   ��  ��	: Tu_e_presetoff
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��Preset��off����
    ��       �l :
-------------------------------------------------------------------------------
[sun051221] �V�K�쐬
********************************************************************************/
void	Tu_e_presetoff(BYTE pstno )
{
	Tu_pno_set(pstno);				/* ����PresetNo��� */
	Tu_job_clear();
	Tu_presetcall();
	tu_m_mode = CTU_M_PSTRD;		/* Preset WriteӰ�޾�� */
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	Lcd_disp_base(CMA_TUNER);
}
/******************************************************************************
	��   ��  ��	: Tu_isron
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��ISR���[�hON����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_isron(void)
{
	Tu_job_clear();
	Ma_mode_event(CMA_ID_TU,CMA_ISR,CMA_MODEON);
}

/******************************************************************************
	��   ��  ��	: Tu_e_isroff
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��ISR���[�hOff����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_isroff(void)
{
	Tu_job_clear();
	Ma_mode_event(CMA_ID_TU,CMA_ISR,CMA_MODEOFF);
}
/******************************************************************************
	��   ��  ��	: Tu_e_ason
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��AS�L�[ ON����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_e_as_on(void)
{
	switch (tu_m_mode)
	{
		case	CTU_M_SEEK:
		case	CTU_M_SCAN:
		case	CTU_M_AS:
			Tu_lastcall();
			Tu_job_clear();
			tu_m_mode = CTU_M_LSTCAL;
			Sbm_mode_cancel(CMA_TUNER);
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */	
			Lcd_disp_base(CMA_TUNER);
			Tu_key_nop();
			break;
		default:
			Key_long_time2000();
			break;
	}
}
/******************************************************************************
	��   ��  ��	: Tu_e_as_long
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��AS�L�[ LONG����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_e_as_long(void)
{
	Tu_job_clear();
	tu_m_mode = CTU_M_AS;
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */	
	Lcd_disp_base(CMA_TUNER);
	Tu_key_nop();
}

/******************************************************************************
	��   ��  ��	: Tu_e_pchupdw_on
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��CH��ON����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_e_pchupdw_on(BYTE updw)
{
	switch (tu_m_mode)
	{
		case	CTU_M_AS:
		case	CTU_M_SCAN:
		case	CTU_M_SEEK:
			Tu_job_clear();
			Tu_lastcall();
			tu_pno = Tu_pch_updw(tu_pno,updw);
			Tu_presetcall();
			tu_m_mode = CTU_M_PSTRD;					/* Preset readӰ�޾�� */
			Sbm_mode_cancel(CMA_TUNER);
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
			Lcd_disp_base(CMA_TUNER);
			Tu_key_nop();
			break;
		case	CTU_M_PSTWT:
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
			Key_long_time500();							/*	500ms�^�C�}�[�ݒ�	*/
			break;
		default:
			Tu_job_clear();
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
			Key_long_time500();							/*	500ms�^�C�}�[�ݒ�	*/
			break;
	}
}
/******************************************************************************
	��   ��  ��	: Tu_e_pchupdw_long
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��CH��LONG����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_e_pchupdw_long(void)
{
	switch (tu_m_mode)
	{
		case	CTU_M_PSTWT:
			ftu_write = ON;							/* �������ݗv��ON */
			break;
		default:
			Tu_job_clear();
			ftu_write = OFF;							/* �������ݗv��OFF */
			tu_m_mode = CTU_M_PSTWT;					/* Preset writeӰ�޾�� */
			break;
	}
	Aud_set_beep(CAUD_REQ_BEEP_LONGKEY);
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	Lcd_disp_base(CMA_TUNER);
	Tu_key_nop();
}
/******************************************************************************
	��   ��  ��	: Tu_e_pchupdw_off
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��CH��OFF����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_e_pchupdw_off(BYTE updw)
{
	switch (tu_m_mode)
	{
		case	CTU_M_PSTWT:
			tu_wrtpno = Tu_pch_updw(tu_wrtpno,updw);
			tu_100ms_timer = CTU_PSTWAIT_TIM;	 		 /* �v���Z�b�g�������ݑ҂� */
			Lcd_blktim_set();
			break;
		default:
			Tu_job_clear();
			tu_pno = Tu_pch_updw(tu_pno,updw);
			Tu_presetcall();
			tu_m_mode = CTU_M_PSTRD;					/* Preset readӰ�޾�� */
			break;
	}
	Sbm_mode_cancel(CMA_TUNER);
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);				/* ���ׂĎ����\����ݾ� */
	Lcd_disp_base(CMA_TUNER);
	Tu_key_nop();
}

/******************************************************************************
	��   ��  ��	: Tu_e_stteston
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��SOUND+P1�� ON����
    ��       �l :
-------------------------------------------------------------------------------
[[zhang070727] �V�K�쐬
********************************************************************************/
void	Tu_e_stteston(void)
{
#if 0 /* QAC�΍� */
/*	Key_long_time5000();
	Sel_mode_cancel(); *//* C12 */
#endif
	Lcd_1shot_key_cancel(CMA_TUNER);
}
/******************************************************************************
	��   ��  ��	: Tu_e_sttestlong
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	:  TunerӼޭ��SOUND+P1�� LONG����
    ��       �l :
-------------------------------------------------------------------------------
[zhang070727] �V�K�쐬
********************************************************************************/
void	Tu_e_sttestlong(void)
{
	if (Tu_band_chk() == CTU_FM)
	{
		ftu_sttest = ~ftu_sttest;
	}
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	Sbm_mode_cancel(CMA_TUNER);
	Tu_key_nop();					/* ���������� */
}
/******************************************************************************
	��   ��  ��	: Tu_e_scanupdw_on
	��       ��	: updw =
	 				CTU_UP:SCAN UP
	 				CTU_DW:SCAN DW
	��   ��  �l	: ����
	�@ �\ �T ��	:  SCAN�L�[ ON����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
void	Tu_e_scanupdw_on(BYTE updw)
{
	if(Sbm_mode_check() != CSBM_SEF)
	{
		switch (tu_m_mode)
		{
			case	CTU_M_PWRON:
			case	CTU_M_BANDCHG:
				break;
			case	CTU_M_SCAN:
				Tu_lastwrite();
				Tu_job_clear();
				tu_m_mode = CTU_M_LSTCAL;
				Sbm_mode_cancel(CMA_TUNER);
				Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
				Lcd_disp_base(CMA_TUNER);
				break;
			default:
				Tu_job_clear();
				Tu_updw_set(updw);
				tu_m_mode = CTU_M_SCAN;
				Sbm_mode_cancel(CMA_TUNER);
				Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
				Lcd_disp_base(CMA_TUNER);
				break;
		}
	}
	Tu_key_nop();
}

void	Tu_e_seek_updw(BYTE ud)
{
	Tu_updw_set(ud);
	switch (tu_m_mode)
	{
		case	CTU_M_AS:
			Tu_lastcall();
		/* QAC�΍� */
			Tu_lastwrite();
			Tu_job_clear();
			tu_m_mode = CTU_M_LSTCAL;
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
			Lcd_disp_base(CMA_TUNER);
			break;
		case	CTU_M_SEEK:
		case	CTU_M_SCAN:
			Tu_lastwrite();
			Tu_job_clear();
			tu_m_mode = CTU_M_LSTCAL;
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
			Lcd_disp_base(CMA_TUNER);
			break;
		default:
			Tu_job_clear();
			tu_m_mode = CTU_M_SEEK;
			Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
			Lcd_disp_base(CMA_TUNER);
			break;
	}
	Sbm_mode_cancel(CMA_TUNER);
}
/*	�e�X�g���[�h�Ή��A100826�A�x��	*/

/******************************************************************************
	��   ��  ��	: Tu_pch_updw
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: TunerӼޭ��pch up/dw���菈��
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
BYTE	Tu_pch_updw(BYTE get_pno, BYTE updw)
{
	BYTE pno;
	
	if (updw == CTU_UP)
	{
		pno = Tu_pch_up(get_pno);
	}
	else
	{
		pno = Tu_pch_down(get_pno);
	}
	return(pno);
}

/******************************************************************************
	��   ��  ��	: Tu_pch_up
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: PCH UP����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
BYTE	Tu_pch_up(BYTE get_pno)
{
	BYTE pno;

	pno = get_pno + 1;

	if (pno > 6)
	{
		pno = 0x01;			/* Preset1�ɖ߂� */
	}
	return(pno);
}
/******************************************************************************
	��   ��  ��	: Tu_pch_down
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: PCH down����
    ��       �l :
-------------------------------------------------------------------------------
[matu120713] �V�K�쐬
********************************************************************************/
BYTE	Tu_pch_down(BYTE get_pno)
{
	BYTE pno;

	pno = get_pno;
	if(pno == 0)
	{
		pno = 0x06;			/* Preset6�ɖ߂� */
	}
	else
	{
		pno --;
		if (pno < 1)
		{
			pno = 0x06;			/* Preset6�ɖ߂� */
		}
	}
	/* QAC�΍� */
	return(pno);

}

/******************************************************************************
	��   ��  ��	: Tu_updw_set
	��       ��	: �����޳ݗv��
				: TRUE �� ���ߕ���
				: FALSE �� �޳ݕ���
	��   ��  �l	: ����
	�@ �\ �T ��	: Tuner�����޳ݕ����ݒ菈��
-------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[0]
********************************************************************************/
void	Tu_updw_set(BYTE updw)
{
	if (updw == CTU_UP)
	{
		ftu_updw = TRUE;
	}
	else
	{
		ftu_updw = FALSE;
	}
}

/******************************************************************************
.. 		�֐���  �FTu_job_clear
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTunerҲݓ���ر����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[0]
******************************************************************************/
void	Tu_job_clear(void)						/* Ҳ�Ӱ�ޓ���֌W�ر���� */
{
	tu_m_mode = CTU_NOP;
	tu_m_seq = 0;
	tu_i_seq = 0;
	tu_10ms_timer = 0;
	tu_5ms_timer = 0;
#if 0 /* QAC�΍� */
//	tu_updw_type = CTU_NORMAL;
#endif
}
/******************************************************************************
.. 		�֐���  �FTu_as_initial
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTuner�����ؾ�ď���������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu20120713] �V�K�쐬
******************************************************************************/
void	Tu_as_initial(void)					/* AS���������� */
{
	tu_as_cnt = 0x00;
	tu_ch.freq = tu_coef.min_ch;		/* �ŏ����g������X�^�[�g */
}
/******************************************************************************
.. 		�֐���  �FTu_fmbandmax
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FFM MAX Band�T�[�`����
				�FAS�X�^�[�g/PS����FM BAND MAX�ԍ��ݒ菈��
				�FTU_BAND_INFO�̏��Ԃ�FM����AM�̏��ɂȂ�̂��O��ł��B
				�FAM��ɂȂ�ꍇ�́A�ύX�K�v�B
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun070319]�V�K�쐬
******************************************************************************/
BYTE Tu_fmbandmax(void)
{
	BYTE	iCnt;
	BYTE	destno;

	destno = Tu_dest_chk();
	
	for (iCnt = 0; iCnt < CTU_BAND; iCnt++)
	{
		if (TU_BAND_INFO[destno][iCnt][0] == CTU_AM)
		{
			break;
		}
	}
	return(iCnt-1);
}

/******************************************************************************
.. 		�֐���  �FTu_BndChange
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTunerBand��ݼޏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[0]
******************************************************************************/
void	Tu_BndChange(void)						/* BandChange */
{
	tu_bandno++;
	if (tu_bandno >= CTU_BAND)
	{
		tu_bandno = 0;
	}
}
/******************************************************************************
.. 		�֐���  �FTu_presetcall
		��  ��	�F����
		�߂�l	�F�������
		�@  �\	�FTUNER Preset���g���ďo����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051223] �V�K�쐬
[sun051226] �P��ýĊ�[н1 ׽Ď��g�������ݘR��]
******************************************************************************/
void	Tu_presetcall(void)
{
	/* Preset���g���ǂݏo�� */
	tu_ch = tu_mem[tu_bandno].pset[tu_pno];		/* Preset���g���AREG�AListID */
	Tu_ch_lm_set(tu_ch.freq);					/* LW/MW�׸޾�� */
	/* ׽Ď��g���ɏ����� */
	tu_mem[tu_bandno].pstno = tu_pno;			/* ׽�PresetNo */
	tu_mem[tu_bandno].pset[0] = tu_ch;			/* ׽Ď��g�� */
}

/******************************************************************************
.. 		�֐���  �FTu_dxlo_chk
		��  ��	�FBYTE band
		�߂�l	:BYTE ret
		�@  �\	�FDX�ALOCAL���菈��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120926] �V�K�쐬
******************************************************************************/
BYTE	Tu_dxlo_chk(BYTE band)
{
	BYTE sm_chk;
	BYTE ill_det;
	
	ill_det = Pw_ret_illdet();
	
	switch(tu_test_dxlo)
	{
		case CTU_TEST_LOCAL:
			sm_chk = tu_chkcef[band].sm_lodb;	/* LOCAL�ݒ� */
			break;
			
		case CTU_TEST_DX:
			sm_chk = tu_chkcef[band].sm_dxdb;	/* DX�ݒ� */
			break;
			
		default:
			if(ill_det == HIGH)						/* ILL HIGH */
			{
				sm_chk = tu_chkcef[band].sm_dxdb;	/* DX�ݒ� */
			}
			else									/* ILL LOW */
			{
				sm_chk = tu_chkcef[band].sm_lodb;	/* LOCAL�ݒ� */
			}
			break;
	}
	return(sm_chk);
}

/******************************************************************************
.. 		�֐���  �FTu_intset_pchtbl
		��  ��	�Fdestno
		�߂�l	:�Ȃ�
		�@  �\	�F�v���Z�b�g�e�[�u�������ݒ菈��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120926] �V�K�쐬
******************************************************************************/
void	Tu_intset_pchtbl(BYTE destno)
{
	BYTE	iLoop1,iLoop2;
	
	for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
	{
		for (iLoop2 = 1; iLoop2 < (CTU_PRESET+1); iLoop2 ++)
		{
			tu_mem[iLoop1].pset[iLoop2].freq = TU_PRESET_TBL[destno][iLoop1][iLoop2-1];
		}
	}
}

/******************************************************************************
.. 		�֐���  �FTu_isrcall
		��  ��	�F����
		�߂�l	�F�������
		�@  �\	�FISR���g���ďo����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[zhang070312] �V�K�쐬
******************************************************************************/
void	Tu_isrcall(void)
{
	/* ISR���g���ǂݏo�� */
	tu_ch = tu_isrch;		/* Preset���g���AREG�AListID */
	Tu_ch_lm_set(tu_ch.freq);					/* LW/MW�׸޾�� */
}

/******************************************************************************
.. 		�֐���  �FTu_lastcall
		��  ��	�F����
		�߂�l	�F�������
		�@  �\	�FTUNER ׽Ď��g���ďo����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[0]
[sun060608] н1�F�݌v�R��AMӰ�ޒ���Ӱ�ނ�׽ĺ�ق��s���ꍇ�́A0��Ă���
******************************************************************************/
void	Tu_lastcall(void)
{
	if(Ma_current_mode() == CMA_ISR)				/* [sakuda101017] */
	{
		tu_ch = tu_isrch;		/* Preset���g���AREG�AListID */
		Tu_ch_lm_set(tu_ch.freq);					/* LW/MW�׸޾�� */
	}
	else
	{
		tu_pno = tu_mem[tu_bandno].pstno;			/* ׽�PresetNo */
		tu_ch = tu_mem[tu_bandno].pset[tu_pno];		/* ׽Ď��g���AREG�AListID */
		Tu_ch_lm_set(tu_ch.freq);					/* LW/MW�׸޾�� */
	}
}
/******************************************************************************
.. 		�֐���  �FTu_lastwrite
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTuner׽ď����ݏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[0]
[sun060608] н1�F�݌v�R��AMӰ�ޒ���Ӱ�ނ�׽ď������݂��Ȃ��悤�ύX
******************************************************************************/
void	Tu_lastwrite(void)						/* ���Ď��g����׽Ă֏������� */
{
	if(Ma_current_mode() == CMA_ISR)				/* [sakuda101017] */
	{
		tu_isrch = tu_ch;
	}
	else
	{
		tu_mem[tu_bandno].pstno = tu_pno;			/* ׽�PresetNo */
		tu_mem[tu_bandno].pset[tu_pno] = tu_ch;		/* ׽Ď��g���AREG�AListID */
		tu_mem[tu_bandno].pset[0] = tu_ch;			/* ׽Ď��g���AREG�AListID */
	}
}

/******************************************************************************
.. 		�֐���  �FTu_set_prechno
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FPreset�ɓo�^����Ă�����g���������B����Ă���΂�Pno�\������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120918] �V�K�쐬
******************************************************************************/
void Tu_set_prechno(void)
{
	BYTE preno;

	preno = Tu_pstno_chk();
	if(preno == 0)				/* [sakuda100528]��ؾ�ēo�^����Ă��Ȃ����̂ݏ��� */
	{
		preno = Tu_chk_presetfreq();
		if(preno != 0)
		{
			Tu_pno_set(preno);				/* ����PresetNo��� */
			Tu_presetcall_nolast();			/*[sakuda100804]*/
		}
	}
}

/******************************************************************************
.. 		�֐���  �FTu_chk_presetfreq
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FPreset�ɓo�^����Ă�����g���������B
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120918] �V�K�쐬
******************************************************************************/
BYTE Tu_chk_presetfreq(void)
{
	TTU_CH prefreq;
	BYTE cnt;
	
	for(cnt = 0; cnt < 6; cnt++)
	{
		prefreq = tu_mem[tu_bandno].pset[cnt+1];		/* ������ނ�Preset���g���擾 */
		if(tu_ch.freq == prefreq.freq)
		{
			return(cnt+1);
		}
	}
	return(0);
}

/******************************************************************************
.. 		�֐���  �FTu_presetcall_nolast
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTUNER �v���Z�b�g���ďo����(׽Ăɂ͓o�^���Ȃ�)
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120918] �V�K�쐬
******************************************************************************/
void	Tu_presetcall_nolast(void)
{
	/* Preset���g���ǂݏo�� */
	tu_ch = tu_mem[tu_bandno].pset[tu_pno];		/* Preset���g���AREG�AListID */
	Tu_ch_lm_set(tu_ch.freq);					/* LW/MW�׸޾�� */
}

/******************************************************************************
.. 		�֐���  �FTu_pno_set
		��  ��	�F�V����PresetNo
		�߂�l	�F����
		�@  �\	�F������ؾ��No��ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[0]
******************************************************************************/
void	Tu_pno_set(BYTE pno)					/* ����PresetNo��� */
{
	tu_pno = pno;
}
/******************************************************************************
.. 		�֐���  �FTu_aftermute
		��  ��	�F���MUTE���
		�߂�l	�F����
		�@  �\	�F���MUTE��Ͼ�ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun060106] �P��ýĊ�[�ύX1 MUTE�׸ޒǉ�]
******************************************************************************/
void	Tu_aftermute(BYTE time)
{
	ftu_mute = OFF;						/* Mute�v��OFF */
	tu_aftermute = time;				/* Mute������Ͼ�� */
}

/******************************************************************************
.. 		�֐���  �FTu_ch_updw
		��  ��	�F����
		�߂�l	�Ftu_updw_stus
		�@  �\	�FTuner���g�������޳ݏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[0]
******************************************************************************/
void	Tu_ch_updw(void)
{
	if( (Ma_model_dest() == CMA_MODEL_AMFM_X)	&&
		((tu_m_mode == CTU_M_AS) 				||
		 (tu_m_mode == CTU_M_SCAN)				||
		 (tu_m_mode == CTU_M_SEEK))				&&
		(Tu_band_chk() == CTU_AM)				 )
	{
		if (ftu_updw == TRUE)
		{
			Tu_mixch_up();
		}
		else
		{
			Tu_mixch_down();
		}
	}
	else
	{
		if (ftu_updw == TRUE)
		{
			Tu_ch_up();
		}
		else
		{
			Tu_ch_down();
		}
	}
}
/******************************************************************************
.. 		�֐���  �FTu_ch_lm_set
		��  ��	�Ffreq �� �w����g��
		�߂�l	�Fftu_lw �� �w����g�� LW/MW���
		�@  �\	�FTuner���g���͈;�ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051225] �V�K�쐬
[sun051226] �P��ýĊ�[0]
******************************************************************************/
void Tu_ch_lm_set(WORD freq)
{
	ftu_lw = CTU_MW;				/* �Ƃ肠���� MW Band */

	Tu_coef_read();

	if (tu_bndmode == CTU_LW)				/* LW ����ޗL�� */
	{
		if (freq < tu_coef.min_ch)				/* �w����g��MW����ލŏ����g����菬���� */
		{
			ftu_lw = CTU_LW;		/* �w����g��LW����� */
			Tu_coef_read();			/* LW BAND���ǂݒ��� */
		}
	}
}

/******************************************************************************
.. 		�֐���  �FTu_dest_chk
		��  ��	�F����
		�߂�l	�FTuner�d����No
		�@  �\	�FTuner�d�����ԍ���������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060109] �V�K�쐬
[zhang070301]�d�����l��ύX
[zhang070305]�d�����̎擾�l��ύX
******************************************************************************/
BYTE	Tu_dest_chk(void)
{
#if	(CTU_E_TEST == 0x01)
	return (CTU_DEST_E);
#else
	BYTE	dest;
	BYTE	iRet;
	
	dest = Ma_model_dest();		/* �d�����̎擾 */
	
	/* ���{�d���� AM���W�I */
	if (dest == CMA_MODEL_AM_J) 	/* AF_CHANGE */
	{
		iRet = CTU_DEST_JPN_A;
	}
	/* ���{�d���� FM/AM���W�I */
	else if (dest == CMA_MODEL_AMFM_J)	/* AF_CHANGE */
	{
		iRet = CTU_DEST_JPN_FA;
	}
	/* �k��/����Ďd���� */
	else if (dest == CMA_MODEL_AMFM_U)			/* AF_CHANGE */
	{
		iRet = CTU_DEST_USA;
	}
	/* ����Ďd���� */
	else if (dest == CMA_MODEL_AMFM_B)			/* AF_CHANGE */
	{
		iRet = CTU_DEST_CSA;
	}
	/* �I�Z�A�j�A�d���� */
	else if (dest == CMA_MODEL_AMFM_T)			/* AF_CHANGE */
	{
		iRet = CTU_DEST_OCA;
	}
	/* �A�W�A/�A�t���J/�����d���� */
	else if (dest == CMA_MODEL_AMFM_A)
	{
		iRet = CTU_DEST_AAM;
	}
	/* ��O�n��d���� */
	else
	{
		iRet = CTU_DEST_THR;
	}
	
	return(iRet);
#endif
}
/******************************************************************************
.. 		�֐���  �FTu_coef_read
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTunerBAND�W����ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060109] �V�K�쐬
******************************************************************************/
void Tu_coef_read(void)
{
	BYTE	bandmd;
	BYTE	destno;

	bandmd = Tu_band_chk();
	destno = Tu_dest_chk();

	tu_bndmode = TU_COEF_TBL[destno].mode;

	if (bandmd == CTU_FM)
	{
		/* FM LWӰ�ނ���Ȃ������o�� */
		if ((tu_bndmode & CTU_FMLW) == CTU_FMLW)
		{
			tu_bndmode = CTU_LW;					/* LW����ޗL�� */
		}
		else
		{
			tu_bndmode = CTU_MW;					/* MW����ނ̂� */
		}

		if (ftu_lw == CTU_MW)
		{
			tu_coef = TU_COEF_TBL[destno].fm;
		}
		else
		{
			tu_coef = TU_COEF_TBL[destno].fm_lw;
		}
	}
	else
	{
		/* AM LWӰ�ނ���Ȃ������o�� */
		if ((tu_bndmode & CTU_AMLW) == CTU_AMLW)
		{
			tu_bndmode = CTU_LW;					/* LW����ޗL�� */
		}
		else
		{
			tu_bndmode = CTU_MW;					/* MW����ނ̂� */
		}

		if (ftu_lw == CTU_MW)
		{
			tu_coef = TU_COEF_TBL[destno].am;
		}
		else
		{
			tu_coef = TU_COEF_TBL[destno].am_lw;
		}
		
		if(Ma_current_mode() == CMA_ISR)
		{
			tu_coef.min_freq = TU_ISR_TBL[0];
			tu_coef.min_ch = TU_ISR_TBL[1];
			tu_coef.max_ch = TU_ISR_TBL[2];
		}
	}
}
/******************************************************************************
.. 		�֐���  �FTu_ch_up
		��  ��	�F����
		�߂�l	�Ftu_updw_stus
		�@  �\	�FTuner���g�����ߏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[н1 MAX Step�擾н]
******************************************************************************/
void	Tu_ch_up(void)
{
	tu_updw_stus = CTU_NOTEDGE;
	if (tu_ch.freq >= tu_coef.max_ch)
	{
		if (tu_bndmode == CTU_LW)				/* LW ����ޗL�� */
		{
			if (ftu_lw == CTU_MW)				/* MW Band */
			{
				tu_updw_stus = CTU_BNDEDGE;		/* ����޴��� */
			}
			else
			{
				tu_updw_stus = CTU_LMEDGE;		/* LW��MW�ִؑ��� */
			}
			ftu_lw = ~ftu_lw;					/* LW Flag���] */
			Tu_coef_read();						/* BAND���ǂݒ��� */
		}
		else
		{
			tu_updw_stus = CTU_BNDEDGE;			/* ����޴��� */
		}
		tu_ch.freq = tu_coef.min_ch;					/* �ŏ����g����� */
	}
	else
	{
		tu_ch.freq ++;
	}
}
/******************************************************************************
.. 		�֐���  �FTu_ch_down
		��  ��	�F����
		�߂�l	�Ftu_updw_stus
		�@  �\	�FTuner���g���޳ݏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051221] �V�K�쐬
[sun051226] �P��ýĊ�[н1 MAX Step�擾н]
******************************************************************************/
void	Tu_ch_down(void)
{
	tu_updw_stus = CTU_NOTEDGE;
	if (tu_ch.freq <= tu_coef.min_ch)
	{
		if (tu_bndmode == CTU_LW)						/* LW ����ޗL�� */
		{
			if (ftu_lw == CTU_LW)				/* MW Band */
			{
				tu_updw_stus = CTU_BNDEDGE;		/* ����޴��� */
			}
			else
			{
				tu_updw_stus = CTU_LMEDGE;		/* LW��MW�ִؑ��� */
			}
			ftu_lw = ~ftu_lw;					/* LW Flag���] */
			Tu_coef_read();						/* BAND���ǂݒ��� */
		}
		else
		{
			tu_updw_stus = CTU_BNDEDGE;			/* ����޴��� */
		}
		tu_ch.freq = tu_coef.max_ch;				/* �ő���g����� */
	}
	else
	{
		tu_ch.freq --;
	}
}

/******************************************************************************
.. 		�֐���  �FTu_dispfreqset
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTuner���g���\���ް��쐬����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051223] �V�K�쐬
[sun051228] �P��ýĊ�
[sun060602] N����EON�AAF�ABS���쒆׽Ď��g����\���ް��ɾ��
[sun060619] н1: LW����BAND�ŁAMW�̎��g�����v�Zн
******************************************************************************/
void	Tu_dispfreqset(void)
{
	BYTE	bandmd;
	WORD	step;
	WORD	min_freq;

	bandmd = Tu_band_chk();
	step = tu_coef.step;
	min_freq = tu_coef.min_freq;

	if (bandmd == CTU_FM)		/* FM STEP�� */
	{
		step = (step/10);				/* step 10kHz�ɕϊ� */
	}	
	tu_dispfreq = ((tu_ch.freq - tu_coef.min_ch) * step) + min_freq;
}

/*------------------------------------------------------------------------------
.. 		�֐���  �FTu_steroclr()
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FTuner��ڵ����OFF�ݒ菈��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060112] �V�K�쐬
-----------------------------------------------------------------------------*/
void Tu_steroclr(void)
{
	ftu_stero = OFF;
	ftu_stero_b = OFF;
	ftu_stchk = OFF;
	tu_stcnt = 0;
}

/*------------------------------------------------------------------------------
.. 		�֐���  �FTu_steroclr()
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FTuner��ڵ����ݸޏ���
				�FON���  �� 50��*10ms
				�FOFF��� ��  2��*10ms
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060112] �V�K�쐬
[sun060619] н1 ST�߰ċɐ��t�ł���
[zhang070226] �C��  
              pFM_STSD����|�[�g    
-----------------------------------------------------------------------------*/
void Tu_sterochat(void)
{
	tu_ic_data.msa.subaddr = 0xe0;
	tu_ic_data.length = 1;
	Tu_data_read();/* SD�l��荞�� */
	if((tu_ic_data.rxbuf[0]&0x10) == 0x10)
	{
		if (ftu_stero_b == LOW)
		{
			tu_stcnt = CTU_STON_CNT;		/* ON����ݸ޶������ */
		}
		ftu_stero_b = HIGH;
	}
	else
	{
		if (ftu_stero_b == HIGH)
		{
			tu_stcnt = CTU_STOFF_CNT;		/* OFF����ݸ޶������ */
		}
		ftu_stero_b = LOW;
	}

	if (tu_stcnt == 0)				/* ����ݸ޶������� */
	{
		ftu_stero = ftu_stero_b;	/* ��ڵ�m�� */
	}
	else
	{
		tu_stcnt --;
	}
}
#if 0 /* ���g�p�̂��߃R�����g�A�E�g */
/******************************************************************************
.. 		�֐���  �FTu_as_samefrqchk
		��  ��	�F����
		�߂�l	�FTRUE �˓������g��APM�ر�Ɋi�[����Ă���
				�FFALSE�˓������g��������Ȃ�����
		�@  �\	�F��������ؾ����؂���Ă��邩����������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051228] �V�K�쐬
[sun060105] �P��ýĊ�[�ύX1 0xff�Ɣ�r���Ȃ�]
******************************************************************************/
BYTE	Tu_as_samefrqchk(void)
{
	BYTE	iCnt;

	for (iCnt = 1; iCnt <= CTU_ASMAX; iCnt++)
	{
		if (tu_mem[tu_bandno].pset[iCnt].freq == tu_ch.freq)
		{
			return(iCnt);				/* �����ǂ���܂��� */
		}
	}
	return(FALSE);						/* �����Ǌi�[����Ă��Ȃ� */
}
#endif
/*------------------------------------------------------------------------------
.. 		�֐���  �FTu_stdis_timset()
		��  ��	�F��ϒl
		�߂�l	�F����
		�@  �\	�FTuner��ڵ�֎~��Ͼ�ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060112] �V�K�쐬
-----------------------------------------------------------------------------*/
void	Tu_stdis_timset(BYTE	tim)
{
	if (tu_stdistim < tim)
	{
		tu_stdistim = tim;
	}
}

/************************************************************************/
/* �֐��� :void	Tu_band_freq_set(void)									*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TunerBAND�Ǝ��g�����M�f�[�^�ݒ菈��							*/
/* �ڍ�   :																*/
/************************************************************************/
void	Tu_band_freq_set(void)
{
	BYTE band;
	BYTE destno;
	WORD_BYTE setfrq;
	
	band = Tu_band_chk();
	destno = Tu_dest_chk();
	Tu_dispfreqset();
	
	if(band == CTU_AM)
	{
		setfrq.word = tu_dispfreq;
	}
	else
	{
		if ((destno == CTU_DEST_USA)
		&&  (tu_dispfreq == 8770))
		{
			tu_dispfreq = 8775;		/* �k�čŏ����g���̏ꍇ���������K�v */
		}
		setfrq.word = (tu_dispfreq/5);
	}
#if 0		/*	EEPROM���珉���p�����[�^���擾�Ή��A100811�A�x	*/
	tu_ic_data.txbuf[0] = (setfrq.byte.byte1 & 0x0f) | TU_PARA_TBL[destno][band][CTU_PARA_TUNER0];
	tu_ic_data.txbuf[1] = setfrq.byte.byte0;
	tu_ic_data.txbuf[2] = TU_PARA_TBL[destno][band][CTU_PARA_TUNER2];
#else
	tu_ic_data.txbuf[0] = (setfrq.byte.byte1 & 0x0f) | tu_para[band][CTU_PARA_TUNER0];
	tu_ic_data.txbuf[1] = setfrq.byte.byte0;
	tu_ic_data.txbuf[2] = tu_para[band][CTU_PARA_TUNER2];
#endif
	Tu_stdis_timset(CTU_ST_PLLCHG);
}
/************************************************************************/
/* �֐��� :void	Tu_band_para_set(void)									*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TunerBAND�֘APARA���M�f�[�^�ݒ菈��							*/
/* �ڍ�   :																*/
/************************************************************************/
void	Tu_band_para_set(void)
{
	BYTE band;
	BYTE destno;
	
	band = Tu_band_chk();
	destno = Tu_dest_chk();
	#if	(CTU_E_TEST == 0x01)
	if ((ftu_lw != CTU_MW)&&(band == CTU_AM))
	{
		memcpy(&tu_ic_data.txbuf[CTU_PARA_RADIO],TU_PARA_LW_E_TBL,13);
		return;
	}
	#endif
	#if 0		/*	EEPROM���珉���p�����[�^���擾�Ή��A100811�A�x	*/
	memcpy(&tu_ic_data.txbuf[CTU_PARA_RADIO],&TU_PARA_TBL[destno][band][CTU_PARA_RADIO],13);
	#else
	memcpy(&tu_ic_data.txbuf[CTU_PARA_RADIO],&tu_para[band][CTU_PARA_RADIO],13);
	#endif
}

/************************************************************************/
/* �֐��� :void	Tu_band_para_set(void)									*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TunerBAND�֘APARA���M����									*/
/* �ڍ�   :																*/
/************************************************************************/
void	Tu_para_send_set(void)
{
	Tu_band_freq_set();
	Tu_band_para_set();
	tu_ic_data.msa.subaddr = 0x20;
	tu_ic_data.length = 16;
	Tu_data_write();
}
/************************************************************************/
/* �֐��� :void	Tu_band_para_set(void)									*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TunerBAND�Ǝ��g�����M����									*/
/* �ڍ�   :																*/
/************************************************************************/
void	Tu_frq_send_set(void)
{
	Tu_band_freq_set();
	tu_ic_data.msa.subaddr = 0x20;
	tu_ic_data.length = 3;
	Tu_data_write();
}

/************************************************************************/
/* �֐��� :void	Tu_band_para_set(void)									*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TunerBAND�Ǝ��g�����M����									*/
/* �ڍ�   :																*/
/************************************************************************/
void	Tu_end_send_set(void)
{
	Tu_band_freq_set();
	tu_ic_data.msa.subaddr = 0xe0;
	tu_ic_data.length = 3;
	Tu_data_write();
}
void	Tu_beep_mute_set(BYTE on_off)
{
	if( on_off == ON )
	{
		Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);	/* TunerMute�v��ON */
		ftu_mute = ON;								/* Mute Flag��� */
	}
	else if(on_off == OFF)
	{
		Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,OFF);	/* TunerMute�v��OFF */
		ftu_mute = OFF;								/* Mute Flag��� */
	}
	else
	{
		;
	}
}

/******************************************************************************
	��   ��  ��	: Tu_mixch_up
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: Tuner�������g���A�b�v����
    ��       �l : ��O�n��d�����d�l�Ή����g���A�b�v����
				  5k/9k�����X�e�b�v�Ŏ��g�����A�b�v����B
				  525kHz�̎��͒�~���Ȃ��B
-------------------------------------------------------------------------------
[yamamoto130314] �V�K�쐬
********************************************************************************/
static void Tu_mixch_up(void)
{
	/* �ϐ��錾 */
	WORD i;
	
	tu_updw_stus = CTU_NOTEDGE;							/* TunerUp/Dw��ԃN���A	*/
	
	if (tu_ch.freq >= tu_coef.max_ch)					/* �J�����g���g�����ő�	*/
	{
		tu_updw_stus = CTU_BNDEDGE;						/* �o���h�G�b�W�ݒ�		*/
		tu_ch.freq = tu_coef.min_ch;					/* �ŏ����g���Z�b�g		*/
	}
	else if ( (tu_ch.freq >= 0) && (tu_ch.freq <= 7) )	/* 525kHz�͒�~���Ȃ�	*/
	{
		tu_ch.freq = 8;									/* 530kHz�ɐݒ�			*/
	}
	else
	{
		for ( i = 1; i <= 5; i++ )
		{
			if ( ((tu_ch.freq + i) % 5 == 3) ||
				 ((tu_ch.freq + i) % 9 == 0)  )
			{
				tu_ch.freq += i;
				return;
			}
		}
	}
}
/******************************************************************************
	��   ��  ��	: Tu_mixch_down
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: Tuner�������g���_�E������
    ��       �l : ��O�n��d�����d�l�Ή����g���_�E������
				  5k/9k�����X�e�b�v�Ŏ��g�����_�E������B
				  525kHz�̎��͒�~���Ȃ��B
-------------------------------------------------------------------------------
[yamamoto130314] �V�K�쐬
********************************************************************************/
static void Tu_mixch_down(void)
{
	/* �ϐ��錾 */
	WORD i;
	
	tu_updw_stus = CTU_NOTEDGE;							/* TunerUp/Dw��ԃN���A	*/
	
	if (tu_ch.freq <= tu_coef.min_ch)					/* �J�����g���g�����ő�	*/
	{
		tu_updw_stus = CTU_BNDEDGE;						/* �o���h�G�b�W�ݒ�		*/
		tu_ch.freq = tu_coef.max_ch;					/* �ő���g���Z�b�g		*/
	}
	else if ( (tu_ch.freq >= 1) && (tu_ch.freq <= 8) )	/* 525kHz�͒�~���Ȃ�	*/
	{
		tu_ch.freq = 0;									/* 522kHz�ɐݒ�			*/
	}
	else
	{
		for ( i = 1; i <= 5; i++ )
		{
			if ( ((tu_ch.freq - i) % 5 == 3) ||
				 ((tu_ch.freq - i) % 9 == 0)  )
			{
				tu_ch.freq -= i;
				return;
			}
		}
	}
}

#undef  _TU_EVENT_C_

#endif