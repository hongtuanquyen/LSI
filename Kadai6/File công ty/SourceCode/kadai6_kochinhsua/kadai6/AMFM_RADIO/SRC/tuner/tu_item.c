#include	"../model.h"

#if	_MODEL_RADIO_

#define		_TU_ITEM_C_

#include		"../comm/common.h"
#include		"../audio/AUD_inc.h"
#include		"../main/MA_inc.h"
#include		"../disp/LCD_inc.h"
#include		"../submode/sbm_ext.h"

#include	"tu_def.h"
#include	"tu_main.h"
#include	"tu_ext.h"
#include	"tu_item.h"
#include	"tu_conttbl.h"
#include	"tu_io.h"
#include	"tu_test.h"
#if		(_MODEL_EEPROM_ == 1)	/*	EEPROM���珉���p�����[�^���擾�Ή��A100811�A�x	*/
#include	"tu_eeprom.h"
#endif
#include	"tu_event.h"

/************************************************************************/
/* �֐��� :void	Tu_main_process(void)									*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	Tuner������۰ُ���											*/
/* �ڍ�   :																*/
/************************************************************************/
void	Tu_main_process(void)
{
	const	TTU_CONTTBL		*m_mode;
	const	TTU_CONTTBL		*seq_tbl;
	BYTE	ret;
    BYTE 	(*func)(void) ;

	m_mode = (const	TTU_CONTTBL*)TU_SeqTBL[tu_m_mode -1];

	seq_tbl = (const	TTU_CONTTBL*)(m_mode+tu_m_seq);
	func = (seq_tbl->func);
	if (func == 0)
	{
		return;
	}
	ret = (*func)();
	if (ret == CTU_I_OKEND)
	{
		if (seq_tbl->ok_seq == CTU_NEXT_M)		/* ����Ҳݏ������� */
		{
			tu_m_seq = 0x00;					/* Ҳݼ��ݽ�ر */
			Tu_I_initial();						/* Item���������� */
		}
		else if ((seq_tbl->ok_seq == CTU_OKEND) ||
			  (seq_tbl->ok_seq == CTU_NGEND))
		{
			Tu_job_clear();						/* �������� */
		}
		else
		{
			tu_m_seq = seq_tbl->ok_seq;			/* ����Item�־�� */
			Tu_I_initial();						/* Item���������� */
		}
	}
	else if (ret == CTU_I_NGEND)
	{
		if(seq_tbl->ng_seq == CTU_NEXT_M)
		{
			tu_m_seq = 0x00;					/* Ҳݼ��ݽ�ر */
			Tu_I_initial();						/* Item���������� */
		}
		else if((seq_tbl->ng_seq == CTU_OKEND) ||
			  (seq_tbl->ng_seq == CTU_NGEND))
		{
			Tu_job_clear();						/* �������� */
		}
		else
		{
			tu_m_seq = seq_tbl->ng_seq;			/* ����Item�־�� */
			Tu_I_initial();						/* Item���������� */
		}
	}
	else
	{
	}
}

/************************************************************************/
/* �֐��� :void	Tu_I_initial(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	Item�����p�ϐ�����������									*/
/* �ڍ�   :																*/
/************************************************************************/
void	Tu_I_initial(void)
{
	tu_i_seq = 0;
	tu_5ms_timer = 0;
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_premute(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER PreMute Wait����										*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_premute(void)
{
	switch(tu_i_seq)
	{
		case	0:
			if((tu_m_mode == CTU_M_SEEK)
			|| (tu_m_mode == CTU_M_AS))
			{
				tu_5ms_timer = 30/5;	/* �L�[������beep���҂� */
			}
			else
			{
				tu_5ms_timer = 0;
			}
			tu_i_seq ++;
			break;
		case	1:
			if (tu_5ms_timer == TIMER_OUT)
			{
				Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);	/* TunerMute�v��ON */
				if(Aud_get_mute_status() != CAUD_MUTE_ON_END)
				{
					if((tu_m_mode == CTU_M_SEEK)
					|| (tu_m_mode == CTU_M_AS)
					|| (tu_m_mode == CTU_M_SCAN))
					{
						tu_5ms_timer = CTU_SEEKMUTE;	/* Front Mute timer set */
					}
					else
					{
						tu_5ms_timer = CTU_PREMUTE;		/* Front Mute timer set */
					}
				}
				else
				{
					tu_5ms_timer = 0;
				}
				ftu_mute = ON;								/* Mute Flag��� */
				tu_i_seq ++;
			}
			break;
		case	2:
			if (tu_5ms_timer == TIMER_OUT)
			{
				tu_i_seq = 0;					/* Item Seq Clear */
				return(CTU_I_OKEND);				/* PreMuteOK */
			}
			break;
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);							/* �����p����������Ҳ݂ɖ߂� */
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_icreset(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER IC RESET����											*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_icreset(void)
{
	switch(tu_i_seq)
	{
		case	0:
			Tu_lastcall();
			tu_i_seq ++;
			break;
		case	1:
			tu_i_seq ++;
			tu_5ms_timer = 10/5;
			Tu_frq_send_set();
			break;
		case	2:
#if 0
		if(tu_5ms_timer == TIMER_OUT )
			{
				tu_ic_data.txbuf[15] = 0x14;
				#if 0
				tu_ic_data.msa.subaddr = 0x1F;		/*	�ۑ�Ǘ��\8�Ԗ��񓚂��A0x0F��0x1F�ɕύX�A100816�A�x	*/
				#else
				tu_ic_data.msa.subaddr = 0x0F;		/* OIL#54�Ή� */
				#endif
				tu_ic_data.length = 1;
				tu_i_seq ++;
				Tu_data_write();
			}
			break;
		case	3:
#endif
		return(CTU_I_OKEND);
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_aftermute(void)									*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER AfterMute�ݒ菈��										*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_aftermute(void)
{
	#if 0	/* audio���W���[��250ms�҂��Ă�̂ŁA�폜 */
	Tu_aftermute(CTU_AFTER250);		/* AfterMute 250ms */
	#else
	Tu_aftermute(0);		/* AfterMute 250ms */
	#endif
	return(CTU_I_OKEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_frqset(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER ���g���ݒ菈��											*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_frqset(void)
{
	switch(tu_i_seq)
	{
		case	0:
			tu_i_seq ++;
			tu_5ms_timer = 10/5;
			Tu_frq_send_set();
			break;
		case	1:
			if(tu_5ms_timer == TIMER_OUT )
			{
				tu_i_seq ++;
				Tu_end_send_set();
			}
			break;
		case	2:
			return(CTU_I_OKEND);
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}


/************************************************************************/
/* �֐��� :BYTE	Tu_I_bndpara(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �o���h�p�����[�^�ݒ菈��								*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_bndpara(void)
{
	switch(tu_i_seq)
	{
		case	0:
			tu_i_seq ++;
			tu_5ms_timer = 10/5;
			Tu_para_send_set();
			break;
		case	1:
			if(tu_5ms_timer == TIMER_OUT )
			{
				tu_i_seq ++;
				tu_5ms_timer = 10/5;
				Tu_frq_send_set();
			}
			break;
		case	2:
#if 0
		if(tu_5ms_timer == TIMER_OUT )
			{
				tu_ic_data.txbuf[15] = 0x00;
				#if 0
				tu_ic_data.msa.subaddr = 0x1F;		/*	�ۑ�Ǘ��\8�Ԗ��񓚂��A0x0F��0x1F�ɕύX�A100816�A�x	*/
				#else
				tu_ic_data.msa.subaddr = 0x0F;		/* OIL#54�Ή� */
				#endif
				tu_ic_data.length = 1;
				tu_i_seq ++;
				Tu_data_write();
			}
			break;
		case	3:
#endif
			Tu_set_prechno();
			return(CTU_I_OKEND);
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_pstwait(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �v���Z�b�g�������ݑ҂�����							*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_pstwait(void)
{
	switch (tu_i_seq)
	{
		case	0:
			tu_100ms_timer = CTU_PSTWAIT_TIM;				/* �v���Z�b�g�������ݑ҂� */
			tu_i_seq++;
			break;
		case	1:
			if(ftu_write == ON)
			{
				Tu_pno_set(tu_wrtpno);
				tu_wrtpno = Tu_pch_updw(tu_wrtpno,CTU_UP);
				return(CTU_I_OKEND);
			}
			if(tu_100ms_timer == TIMER_OUT)
			{
				Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
				Lcd_disp_base(CMA_TUNER);
				return(CTU_I_NGEND);
			}
			break;
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_pstwrite(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �v���Z�b�g�������ݏ���								*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_pstwrite(void)
{
	#if 0	/* �d�l���������߁A�폜 */
	Aud_set_prg_mute(CAUD_MUTE_TUN_NR_MUTE,ON);	/* [070525] MUTE�����C�� TunerMute�v��ON */
	ftu_mute = ON;								/* [070525] MUTE�����C�� Mute Flag��� */
	Tu_aftermute(CTU_AFTER500);
	#endif
	Tu_lastwrite();
	Tu_stdis_timset(CTU_ST_PLLCHG);		/*500msST���o�֎~ */
	#if	(_MODEL_EEPROM_ == 1)
	tu_eepdata[0]	= (BYTE)(tu_ch.freq & 0xFF);
	tu_eepdata[1]	= (BYTE)((tu_ch.freq >> 8) & 0xFF);
	#if 0
	tu_eepreq.EepAddr	= (CTU_EEPOFS_PREMEM + (tu_bandno * 12) + (tu_pno -1) * 2);
	#else
	tu_eepreq.EepAddr	= (CTU_EEPOFS_PREMEM + (tu_bandno * 16) + (tu_pno -1) * 2);
	#endif
	tu_eepreq.Length	= 2;
	tu_eepreq.BufAddr	= tu_eepdata;
	ftu_weepreq			= ON;
	tu_eep_cnt			= 0;
	#endif
	return(CTU_I_OKEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_updw(void)											*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER ���g���A�b�v�_�E������								*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_updw(void)
{
#if	(CTU_E_TEST == 0x01)
	Tu_ch_updw();
	if((Tu_band_chk() == CTU_FM)&&( tu_m_mode == CTU_M_SEEK ))
	{
		Tu_ch_updw();
	}
	return(CTU_I_OKEND);
#else
	Tu_ch_updw();
	if ((tu_m_mode == CTU_M_MAN))
	{
		if(Ma_current_mode() == CMA_ISR)
		{
			Lcd_disp_base(CMA_ISR);
		}
		else
		{
			Lcd_disp_base(CMA_TUNER);
		}
	}
#if 0
	iRet = Tu_updw_typechk();
	if (iRet == CTU_UPDW_NOT)
	{
		Tu_ch_updw();
		iRet = Tu_updw_typechk();
	}
	if (iRet == CTU_UPDW_END)
	{
		return(CTU_I_NGEND);
	}
	else
	{
		return(CTU_I_OKEND);
	}
#else
	return(CTU_I_OKEND);
#endif
#endif
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_man(void)											*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �}�j���A����											*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_man(void)
{
	Tu_pno_set(0);
	Tu_set_prechno();
	Tu_lastwrite();
	Sbm_mode_cancel(CMA_TUNER);
	return(CTU_I_OKEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_cwait(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �w��^�C���҂�����									*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_cwait(void)
{
	switch (tu_i_seq)
	{
		case	0:
			if ((tu_updw_stus == CTU_BNDEDGE)
			|| (tu_m_mode == CTU_M_SCAN))
			{
				tu_5ms_timer = tu_coef.wait_edge;
			}
			else
			{
				tu_5ms_timer = tu_coef.c_wait;
			}
			tu_i_seq++;
			break;
		case	1:
			if (tu_5ms_timer == TIMER_OUT)
			{
				return(CTU_I_OKEND);
			}
			break;
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}
/************************************************************************/
/* �֐��� :BYTE	Tu_I_seekst(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �V�[�N�J�n����										*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_seekst(void)
{
//	Tu_lastcall();
	Tu_pno_set(0);				/* PresetNo Clear */
	return(CTU_I_OKEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_qinfochk(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER SEEK�ǒ�~�����`�F�b�N����							*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_qinfochk(void)
{
	BYTE ret;
	
	ret = NG;
	switch (tu_i_seq)
	{
		case 0:
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
			if ((tu_updw_stus == CTU_BNDEDGE) ||
				(tu_updw_stus == CTU_LMEDGE))		/*	Buglist#24�Ή��A�o���h�G�b�W�҂��A20100712	*/
			{
				tu_5ms_timer = tu_coef.wait_edge;
			}
			tu_i_seq ++;
			break;
			
		case	1:
			if( tu_5ms_timer != 0 )
			{
				break;
			}
			if(Tu_band_chk() == CTU_AM)
			{
				tu_1ms_timer = tu_chkcef[CTU_AM].wait_tm;		/*	EEPROM���珉���p�����[�^���擾�Ή��A100811�A�x	*/
			}
			else
			{
				tu_1ms_timer = tu_chkcef[CTU_FM].wait_tm;		/*	EEPROM���珉���p�����[�^���擾�Ή��A100811�A�x	*/
			}
			tu_i_seq++;
			break;
		case	2:
			if (tu_1ms_timer == TIMER_OUT)
			{
				tu_ic_data.msa.subaddr = 0xe0;
				tu_ic_data.length = 5;
				Tu_data_read();/* SD�l��荞�� */
				ret = Tu_Average_QInfo(tu_ic_data.rxbuf);
				if(ret == OK)
				{
					return(CTU_I_OKEND);
				}
				else if( ret == NG )
				{
					return(CTU_I_NGEND);
				}
				else
				{
					tu_i_seq --;
				}
			}
			break;
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}

#if 0
/************************************************************************/
/* �֐��� :BYTE	Tu_I_icreset(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER IC RESET����											*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_sdchk(void)
{
	BYTE	band;

	band = Tu_band_chk();
	switch (tu_i_seq)
	{
		case	0:
			tu_1ms_timer = 10/1;
			tu_i_seq++;
			break;
		case	1:
			if (tu_1ms_timer == TIMER_OUT)
			{
				tu_ic_data.msa.subaddr = 0xe0;
				tu_ic_data.length = 5;
				Tu_data_read();/* SD�l��荞�� */
				if((tu_ic_data.rxbuf[0]&0xC0) == 0x40)
				{
#if 0			/*	S-Meter�l�����`�F�b�N�A20100603�A�x	*/
					if (tu_ic_data.rxbuf[1] > tu_coef.sd_dx)
#endif				/*	EEPROM���珉���p�����[�^���擾�Ή��A100811�A�x	*/
					if (tu_ic_data.rxbuf[1] > (tu_dx[band] * 10 / tu_chkcef[band].prec_sm))
					{
						return(CTU_I_OKEND);
					}
					else
					{
						return(CTU_I_NGEND);
					}
				}
			}
			break;
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_icreset(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER IC RESET����											*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_ifchk(void)
{
	BYTE	ifcnt;
	BYTE	band;
	BYTE	usn;
	BYTE	wam;
	
	usn = 0;
	wam = 0;
	band = Tu_band_chk();
	
#if _TUNER_BND_WB_
	if(Tu_band_chk() == CTU_WB)
	{
		return(CTU_I_OKEND);
	}
#endif
	switch (tu_i_seq)
	{
		case	0:
			tu_10ms_timer = 160/10;
			tu_seekng_cnt = 0;
			tu_i_seq++;
			break;
		case	1:
			if (tu_1ms_timer == TIMER_OUT)
			{
				tu_1ms_timer = 10/1;
				tu_ic_data.msa.subaddr = 0xe0;
				tu_ic_data.length = 5;
				Tu_data_read();/* SD�l��荞�� */
				if((tu_ic_data.rxbuf[3]&0xC0) == 0xC0)
				{
					ifcnt = (tu_ic_data.rxbuf[3]&0x1F);
					if( band == CTU_FM )
					{
						usn = tu_ic_data.rxbuf[2]>>4;
						wam = tu_ic_data.rxbuf[2]&0x0F;
					}
					if((ifcnt <= tu_chkcef[band].if_detail)
#if 0			/*	S-Meter�l�����`�F�b�N�A20100603�A�x	*/
						&&(tu_ic_data.rxbuf[1] > tu_coef.sd_loc)
#endif			/*	EEPROM���珉���p�����[�^���擾�Ή��A100811�A�x	*/
						&&(tu_ic_data.rxbuf[1] > tu_dx[band])
						&&(usn <= tu_chkcef[band].usn_max)
						&&(wam <= tu_chkcef[band].wam_max))
					{
						if(tu_10ms_timer == 0)
						{
							return(CTU_I_OKEND);
						}
					}
					else
					{
						tu_seekng_cnt ++;
						if(tu_seekng_cnt >= 2)
						{
							return(CTU_I_NGEND);
						}
					}
				}
			}
			break;
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}
#endif
/************************************************************************/
/* �֐��� :BYTE	Tu_I_seekend(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �V�[�N�I������										*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE	Tu_I_seekend(void)
{
	Tu_set_prechno();
	Tu_lastwrite();						/* ׽ď����� */
	Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */
	Lcd_disp_base(CMA_TUNER);
	return(CTU_I_OKEND);
}
/******************************************************************************
	��   ��  ��	: Tu_I_isrread
	��       ��	: ����
	��   ��  �l	: ����
	�@ �\ �T ��	: ISR �ǂݏo������
    ��       �l :
-------------------------------------------------------------------------------
[zhang070312] �V�K�쐬
********************************************************************************/
BYTE	Tu_I_isrread(void)
{
	Tu_lastcall();
	return(CTU_I_OKEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_apmst(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �I�[�g�X�g�A�J�n����									*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_apmst(void)
{
	Tu_as_initial();					/* APM�ر�ر */
	Tu_updw_set(TRUE);					/* ���ߕ������ */
	tu_pno = 0x01;
	return(CTU_I_OKEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_apmset(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �I�[�g�X�g�A�L�Ǐ������ޏ���							*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_apmset(void)
{
	tu_as_cnt ++;
	tu_mem[tu_bandno].pset[tu_as_cnt] = tu_ch;

	if(tu_pno < CTU_ASMAX)
	{
		tu_pno ++;
	}
	return(CTU_I_OKEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_apmendchk(void)									*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �I�[�g�X�g�A�I���`�F�b�N����							*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_apmendchk(void)
{
	TTU_CH	ch;

	if(tu_as_cnt >= CTU_ASMAX)
	{
		return(CTU_I_OKEND);
	}
	if (tu_updw_stus == CTU_BNDEDGE)
	{
		return(CTU_I_OKEND);
	}
	return(CTU_I_NGEND);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_I_apmend(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	TUNER �I�[�g�X�g�A�I������									*/
/* �ڍ�   :	�߂�l�F�������											*/
/* 					CTU_I_OKEND �� ����OK����							*/
/*					CTU_I_NGEND �� ����NG����							*/
/*					CTU_I_NOTEND �� ������								*/
/************************************************************************/
BYTE 	Tu_I_apmend(void)
{
	BYTE	icnt;
	
	if(tu_as_cnt == 0)
	{
		Tu_lastcall();
		tu_m_mode = CTU_M_LSTCAL;
		Lcd_disp_base(CMA_TUNER);
		return(CTU_I_NGEND);			/* PresetScan�֑J�ڂ��Ȃ� */
	}
	else
	{
		tu_pno = 1;			/* �i�[��MIN PresetSET */
		Tu_presetcall();
		tu_m_mode = CTU_M_PSTRD;
		Lcd_disp_base(CMA_TUNER);
		#if	(_MODEL_EEPROM_ == 1)
		for (icnt = 0; icnt< tu_as_cnt ; icnt++)
		{
			tu_eepdata[icnt*2] = (BYTE)(tu_mem[tu_bandno].pset[icnt+1].freq & 0xFF);
			tu_eepdata[icnt*2 + 1] = (BYTE)((tu_mem[tu_bandno].pset[icnt+1].freq>>8) & 0xFF);
		}
		tu_eepreq.EepAddr	= CTU_EEPOFS_PREMEM + (tu_bandno * 16);
		tu_eepreq.Length	= tu_as_cnt * 2;
		tu_eepreq.BufAddr	= tu_eepdata;
		ftu_weepreq			= ON;
		tu_eep_cnt			= 0;
		#endif
		return(CTU_I_OKEND);
	}
}
/******************************************************************************
.. 		�֐���  �FTu_I_pstup
		��  ��	�F����
		�߂�l	�F�������
				�FCTU_I_OKEND �� ����OK����
				�FCTU_I_NGEND �� ����NG����
				�FCTU_I_NOTEND �� ������
		�@  �\	�F����Preset�ԍ���ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051228] �V�K�쐬
[sun060105] �P��ýĊ�[н1 PresetCall���Ă��Ȃ�]
[sun060821] Preset��ق���O��List�ر����悤�ɏC��
			PresetNo��Ă��邾����PLL�ݒ肵�Ȃ��ƁA�޺��ނ�OneMain����Ă��܂�
[zhang070313]������ύX
[zhang070314]bandno �؊���ύX
******************************************************************************/
BYTE	Tu_I_pstup(void)
{
	tu_pno ++;
	if (tu_pno > 6)
	{
		tu_pno = 0x01;			/* Preset1�ɖ߂� */
		if(Tu_band_chk() == CTU_FM)
		{
			tu_bandno++;
			if(Tu_band_chk() == CTU_AM)
			{
				tu_bandno = 0;
			}
		}
		else
		{
			tu_bandno++;
			if (tu_bandno >= CTU_BAND)
			{
				tu_bandno = Tu_fmbandmax() + 1;
			}
		}
	}
	Tu_pno_set(tu_pno);
	/* Preset���g���ǂݏo�� */			/* [070625zhang] �w�E20���C�� */
	tu_ch = tu_mem[tu_bandno].pset[tu_pno];		/* Preset���g���AREG�AListID */
	Tu_ch_lm_set(tu_ch.freq);					/* LW/MW�׸޾�� */
//	Tu_presetcall();
	return(CTU_I_OKEND);
}

/******************************************************************************
.. 		�֐���  �FTu_I_pstscan
		��  ��	�F����
		�߂�l	�F�������
				�FCTU_I_OKEND �� ����OK����
				�FCTU_I_NGEND �� ����NG����
				�FCTU_I_NOTEND �� ������
		�@  �\	�F�����ؾ��Scan ����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matsu120713] �V�K�쐬
******************************************************************************/
BYTE	Tu_I_scanwait(void)
{
	switch (tu_i_seq)
	{
		case	0:
			Tu_set_prechno();
			#if 0	/* audio���W���[��250ms�҂��Ă�̂ŁA�폜 */
			Tu_aftermute(CTU_AFTER250);		/* AfterMute 250ms */
			#else
			Tu_aftermute(0);		/* AfterMute 250ms */
			#endif
			Tu_stdis_timset(CTU_ST_PLLCHG);		/*500msST���o�֎~ */
			tu_100ms_timer = CTU_SCANWAIT_TIM;	/* Tuner Scan��� */
			tu_i_seq++;
			break;
		case	1:
			if (tu_100ms_timer == TIMER_OUT)
			{
				return(CTU_I_OKEND);
			}
			break;
		default:
			tu_i_seq = 0;
			break;
	}
	return(CTU_I_NOTEND);
}

/******************************************************************************
.. 		�֐���  �FTu_I_pstchk
		��  ��	�F����
		�߂�l	�F�������
				�FCTU_I_OKEND �� ����OK����
				�FCTU_I_NGEND �� ����NG����
				�FCTU_I_NOTEND �� ������
		�@  �\	�F���g������PresetNo����������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matsu120713] �V�K�쐬
******************************************************************************/
BYTE	Tu_I_pstchk(void)
{
	Tu_set_prechno();				
	return(CTU_I_OKEND);
}

#undef  _TU_ITEM_C_

#endif
