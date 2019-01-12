/********************************************************************************/
/*		tu_test.c																*/
/*																				*/
/*		�v���W�F�N�g���F�N�����I���a���� 2011���f��								*/
/*		�t�@�C�����Ftu_test.c													*/
/*		���W���[�����FTuner														*/
/*		�@�\�F																	*/
/*																				*/
/*																				*/
/*																				*/
/*			�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z			*/
/*------------------------------------------------------------------------------*/
/*						2010.07.06		0.0			�x		�V�K�쐬			*/
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
	Tu_Diag_Idle,			/* ��~��� 				 */
	Tu_Diag_Smeter,			/* FM SҰ��v��				 */
	Tu_Diag_Smeter,			/* AM SҰ��v��				 */
	Tu_Diag_Smeter,			/* FM SҰ���d������(20dB) */
	Tu_Diag_Smeter,			/* AM SҰ���d������(30dB) */
	Tu_Diag_Smeter,			/* FM ���ٕϊ��l�擾		 */
	Tu_Diag_Smeter,			/* AM ���ٕϊ��l�擾		 */
};

enum
{
	CTU_DIAG_NOP,				
	CTU_DIAG_FMSMETER_REQ,		//FM SҰ��v��
	CTU_DIAG_AMSMETER_REQ,		//AM SҰ��v��
	CTU_DIAG_FMLST_REQ,			//FM SҰ���d������(20dB)
	CTU_DIAG_AMLST_REQ,			//AM SҰ���d������(20dB)
	CTU_DIAG_FMLVCNV_REQ,		//FM SҰ����ٕϊ��l�擾
	CTU_DIAG_AMLVCNV_REQ,		//AM SҰ����ٕϊ��l�擾
};

/***	���M���Œ�f�[�^	***/
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

/***	ATOMIC�n�R�}���h	***/
#define		CTU_TEST_ATM_FMSM		0
#define		CTU_TEST_ATM_AMSM		1
#define		CTU_TEST_ATM_FM20DB		4
#define		CTU_TEST_ATM_AM30DB		5
#define		CTU_TEST_ATM_FM40DB		6
#define		CTU_TEST_ATM_AM50DB		7

/***	ATMIC�n�V�[�P���X		***/
#define		CTU_SMETER_CHECK	0x01
#define		CTU_SMEEP_WRITE		0x02
#define		CTU_DATA_REPLY		0x03

static	BYTE	tu_diag_req;
static	BYTE	tu_diag_seq;
static	BYTE	tu_diag_cnt;

static	BYTE	tu_sm_chk_ok;
#if 0	/*���g�p�̂��߃R�����g�A�E�g*/
BYTE	tu_test_AM1db;
BYTE	tu_test_AM_20db;
BYTE	tu_test_AM_40db;

BYTE	tu_test_FM1db;
BYTE	tu_test_FM_20db;
BYTE	tu_test_FM_40db;
#endif
/******************************************************************************
.. 		�֐���  �FTu_cmd_func
		��  ��	�FRcvCmd,
				  *RcvBuf
				  Lenth
		�߂�l	�F����
		�@  �\	�F�e�X�g���[�h��t����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120926] �V�K�쐬
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
			Tu_intset_pchtbl(destno);					/* PCH�e�[�u���ݒ� */
			Tu_SaveEepData(CTU_EEPWR_PST,tu_eepdata);	/* EEPROM�������݃f�[�^�ݒ� */
			EEP_Initial_para_Req(tu_eepdata,0xFF);			/* EEPROM�������ݗv�� */
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
.. 		�֐���  �FTu_test_dirpch
		��  ��	�F*RcvBuf
		�߂�l	�F����
		�@  �\	�F�_�C���N�gPCH�ǂݍ��ݏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120926] �V�K�쐬
******************************************************************************/
void	Tu_test_dirpch(BYTE *RcvBuf)
{
	if ((RcvBuf[0] > 0) && (RcvBuf[0] <= CTU_PRESET))
	{
		Tu_job_clear();
		Tu_pno_set(RcvBuf[0]);
		Tu_presetcall();
		tu_m_mode = CTU_M_PSTRD;					/* Preset readӰ�޾�� */
		Lcd_disp_base(CMA_TUNER);
	}
}

/******************************************************************************
.. 		�֐���  �FTu_test_dirnch
		��  ��	�F*RcvBuf
		�߂�l	�F����
		�@  �\	�F�_�C���N�g���g���ǂݍ��ݏ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120926] �V�K�쐬
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
	
	if (bandmd == CTU_FM)		/* FM STEP�� */
	{
		step = (step/10);				/* step 10kHz�ɕϊ� */
	}
		
	min_freq = tu_coef.min_freq;
	max_freq = ((tu_coef.max_ch - tu_coef.min_ch) * step) + min_freq;
	
	/* ��p���l�ϊ� */
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
		Lcd_1shot_key_cancel(CLCD_1SHOT_OFF);		/* ���ׂĎ����\����ݾ� */	
		Lcd_disp_base(CMA_TUNER);
	}
}

/******************************************************************************
.. 		�֐���  �FTu_test_dxlochg
		��  ��	�F*RcvBuf
		�߂�l	�F����
		�@  �\	�FDX/LO�ؑ֏���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120926] �V�K�쐬
******************************************************************************/
void	Tu_test_dxlochg(BYTE *RcvBuf)
{
	switch(RcvBuf[0])
	{
		case 1:
			tu_test_dxlo = CTU_TEST_LOCAL;					/* LOCAL�ݒ�v�� */
			break;
			
		case 0:
			tu_test_dxlo = CTU_TEST_DX;						/* DX�ݒ�v�� */
			break;
			
		default:
			break;
	}
}

/******************************************************************************
.. 		�֐���  �FTu_test_stareply
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTuner�X�e�[�^�X�ԑ�����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu121105] �V�K�쐬
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

	Tu_dispfreqset();					/* ���g���\���p�ϊ� */
	iRet = Cm_lib_wHEXtoBCD(tu_dispfreq,bcddata);	/* HEX��BCD�ϊ� */
	
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
.. 		�֐���  �FTu_test_atomicget
		��  ��	�Fdestno
		�߂�l	�F����
		�@  �\	�FATOMIC�f�[�^�擾����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu120926] �V�K�쐬
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
				/*	��������	*/
				Tu_Diag_NgSet();
				break;
		}
	}
	else
	{
		/*	��������	*/
		Tu_Diag_NgSet();
	}
}

/******************************************************************************
.. 		�֐���  �FTu_test_invalidchk
		��  ��	: atomic_req
		�߂�l	�F����
		�@  �\	�F�����R�}���h���菈��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu121101] �V�K�쐬
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
.. 		�֐���  �FTu_Diag_Smeter
		��  ��	:
		�߂�l	�F����
		�@  �\	�F�����R�}���h���菈��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu121101] �V�K�쐬
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
.. 		�֐���  �FTu_Smeter_Check
		��  ��	:
		�߂�l	�F����
		�@  �\	�F�����R�}���h���菈��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu121101] �V�K�쐬
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
		Tu_data_read();/* SD�l��荞�� */
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
			tu_qinfochk_num = CTU_SEEK_NUM;		/* �����l�ݒ� */
		}
	}
}

/******************************************************************************
.. 		�֐���  �FTu_SmEep_Write
		��  ��	:
		�߂�l	�F����
		�@  �\	�F�����R�}���h���菈��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu121101] �V�K�쐬
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
					/*	��������	*/
					Tu_Diag_NgSet();
					break;
				
			}
			/* �����ђl��ݒ肵�܂�(�����l�͑S�ı��ڽ0x00�`0x0e�͈̔�) */
			calsum	= 0;					/* 16Byte SUM�v�Z				  */
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
		/*	EEPROM�ǂݏo�����s��	*/
		Tu_Diag_NgSet();
	}
}

/******************************************************************************
.. 		�֐���  �FTu_Data_Reply
		��  ��	:
		�߂�l	�F����
		�@  �\	�F�����R�}���h���菈��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matu121101] �V�K�쐬
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
			/*	��������	*/
			Tu_Diag_NgSet();
			break;
	}
}

WORD	Tu_Chg_WORD( BYTE * Byte_Data )
{
	CHG_WORD  tmp;						/* �ϊ��p�\���̒�`               */

	tmp.byte[0] = *Byte_Data;			/* BYTE��WORD�ɕϊ�            */
	tmp.byte[1] = *(Byte_Data+1);
	return(tmp.word);
}


void	Tu_Add_WORD( BYTE * Byte_Data , WORD Add_Data)
{
	CHG_WORD  tmp;						/* �ϊ��p�\���̒�`               */
	
	tmp.byte[0] = *Byte_Data;			/* BYTE��WORD�ɕϊ�            */
	tmp.byte[1] = *(Byte_Data+1);
	tmp.word += Add_Data;				/* �ް������Z����                 */
	*Byte_Data = tmp.byte[0];			/* ���Z�����ް���߂�             */
	*(Byte_Data+1)= tmp.byte[1];
}

BYTE	Tu_Level_db_Chg( BYTE d_Level20 , BYTE d_Level40 )
{
	BYTE	temp = 0x00U;
	
	//--------------------------
	// 1dB�������LEVEL�l���v�Z 
	//--------------------------
	if( d_Level40 < d_Level20 ){							// 20db Level�l�̕����������ꍇ�A
		return(0x01U);										// �v�Z���ʂ́u�P�v�ɂ���
	}
	
	//====================================================================
	// �v�Z���@ 
	//--------------------------------------------------------------------
	// 2008.10.06-1 �R�����g�ǉ�
	// FM/AM�Ōv�Z�����Ⴄ
	// 1)FM��
	//   �d�l��̎� ��  ((( 40dB��S-METER�l �| 20dB��S-METER�l )�~�P�O) �� (40dB �| 20dB)) 
	//   �����ȒP�� ��  ((( 40dB��S-METER�l �| 20dB��S-METER�l )�~�P�O) �� (20))
	//   ��������   ��  ((( 40dB��S-METER�l �| 20dB��S-METER�l )�~�P) �� (2))
	//   �ŏI�I��   ��  ((( 40dB��S-METER�l �| 20dB��S-METER�l )�� 2)
	//   �ł悭�Ȃ�B
	// 2)AM��
	//   �d�l��̎� ��  ((( 50dB��S-METER�l �| 30dB��S-METER�l )�~�P�O) �� (50dB �| 30dB)) 
	//   �����ȒP�� ��  ((( 50dB��S-METER�l �| 30dB��S-METER�l )�~�P�O) �� (20))
	//   ��������   ��  ((( 50dB��S-METER�l �| 30dB��S-METER�l )�~�P) �� (2))
	//   �ŏI�I��   ��  ((( 50dB��S-METER�l �| 30dB��S-METER�l )�� 2)
	//   �ł悭�Ȃ�B
	// ����
	//   FM/AM���ɁA���L�̌v�Z�͋��ʂŖ��Ȃ�
	//====================================================================
	// 2008.09.09-1 �v�Z����ύX
	temp = (BYTE)((d_Level40 - d_Level20) / 2U );
//	temp = ((d_Level40 - d_Level20) / ( 40U - 20U ));
	
	if( 0x00U == temp ){
		temp = 0x01U;
	}
	return(temp);
}


BYTE	Tu_FMLeveldB_Conv(BYTE level)
{
	WORD	temp = 0x00U;											// 2008.08.21-3 db�ϊ����A�I�[�o�[�t���[����������\��������̂ŁA�ϐ���WORD�ɕύX
	BYTE	result_db = 0x00U;
	
	//========================================================================
	// Level�l��S-Meter�֕ϊ� 
	//------------------------------------------------------------------------
	// ( ��MLEVEL��20dB��S-METER�l�̍��~10 �� �`(1db�������Level�l) �{ 20dB 
	//========================================================================
	
	//--------------------------------
	// ��MLEVEL��20dB��S-METER�l�̍� 
	//--------------------------------
	if( level < tu_chkcef[CTU_FM].sm_20db ){							// 20db�l�̕��������ꍇ
		temp = (tu_chkcef[CTU_FM].sm_20db - level);
	}else{															// Level�l�̕��������ꍇ
		temp = (level - tu_chkcef[CTU_FM].sm_20db);
	}
	
	// 2008.09.09-1 10�{����悤�ɏC��
	temp = (WORD)(temp * 10);										// temp�̈�͍ő�ł�255�܂ł����ݒ肳��Ȃ��B�Ȃ̂ŁA10�{���Ă����Ȃ�
	
	if( 0x00U != tu_level_FM1db ){								// 1dB�������LEVEL�l�Ŋ���Z�B���ꂪ0�ɂȂ邱�Ƃ͂Ȃ����A
		temp = (temp / tu_level_FM1db);							// MISRA�Ŏw�E����Ȃ����߂ɁA�K�[�h�����ǉ�
	}
	// <��>tu_level_FM1db��0�ɐݒ肳��邱�Ƃ��Ȃ����A������A0���ݒ肳��Ă���ꍇ�Adb�l�́A20�Œ�Őݒ肳���
	
	// 2008.08.21-3 �I�[�o�[�t���[����������\�������邽�߁A�Ή�
	// 2008.09.10-1 Level�l��20db�����Ⴂ�ꍇ�́A�v�Z���ʂ���|20����悤�ɏC��
	if( level < tu_chkcef[CTU_FM].sm_20db ){							// 20db�l�̕��������ꍇ
		if( temp > 20U ){											// ���ɒl��20�ȉ��̏ꍇ
			temp = 0U;												// �l��0�ɐݒ�(�v�Z�����Ⴄ�ƌv�Z���ʂ�0xFF�ɂȂ����Ⴄ�B�B)
		}else{
			temp = (WORD)(20U - temp);							// 20db����v�Z���ʂ�����
		}
	}else{
		temp = (WORD)(20U + temp);								// 20db���{����
	}
	
	if( 0x00FF < temp ){											// db�ϊ��������ʁA255�ȏ�̏ꍇ
		result_db = 0xFFU;											// 255db�Ƃ���
	}else{
		result_db = (BYTE)temp;									// ���̑��́A�v�Z���ʂ����̂܂ܐݒ�
	}
	
	return( result_db );
}
BYTE	Tu_AMLeveldB_Conv(BYTE level)
{
	WORD	temp = 0x00U;											// 2008.08.21-3 db�ϊ����A�I�[�o�[�t���[����������\��������̂ŁA�ϐ���WORD�ɕύX
	BYTE	result_db = 0x00U;
	
	//========================================================================
	// Level�l��S-Meter�֕ϊ� 
	//------------------------------------------------------------------------
	// 2008.10.06-1 �v�Z���ύX
	// ( ��MLEVEL��30dB��S-METER�l�̍��~10 �� �`(1db�������Level�l) �{ 30dB 
	//========================================================================
	
	//--------------------------------
	// ��MLEVEL��20dB��S-METER�l�̍� 
	//--------------------------------
	if( level < tu_chkcef[CTU_AM].sm_20db ){							// 20db�l�̕��������ꍇ
		temp = (tu_chkcef[CTU_AM].sm_20db - level);
	}else{															// Level�l�̕��������ꍇ
		temp = (level - tu_chkcef[CTU_AM].sm_20db);
	}
	
	// 2008.09.09-1 10�{����悤�ɏC��
	temp = (WORD)(temp * 10);										// temp�̈�͍ő�ł�255�܂ł����ݒ肳��Ȃ��B�Ȃ̂ŁA10�{���Ă����Ȃ�
	
	if( 0 != tu_level_AM1db ){									// 1dB�������LEVEL�l�Ŋ���Z�B���ꂪ0�ɂȂ邱�Ƃ͂Ȃ����A
		temp = (temp / tu_level_AM1db);							// MISRA�Ŏw�E����Ȃ����߂ɁA�K�[�h�����ǉ�
	}
	// <��>tu_level_AM1db��0�ɐݒ肳��邱�Ƃ��Ȃ����A������A0���ݒ肳��Ă���ꍇ�Adb�l�́A20�Œ�Őݒ肳���
	
	// 2008.08.21-3 �I�[�o�[�t���[����������\�������邽�߁A�Ή�
	// 2008.09.10-1 Level�l��20db�����Ⴂ�ꍇ�́A�v�Z���ʂ���|20����悤�ɏC��
	// 2008.10.06-1 ������20db��30db�ɕύX
	if( level < tu_chkcef[CTU_AM].sm_20db ){							// 30db�l�̕��������ꍇ
		if( temp > 30U ){											// ���ɒl��30�ȉ��̏ꍇ
			temp = 0U;												// �l��0�ɐݒ�(�v�Z�����Ⴄ�ƌv�Z���ʂ�0xFF�ɂȂ����Ⴄ�B�B)
		}else{
			temp = (WORD)(30U - temp);							// 30db����v�Z���ʂ�����
		}
	}else{
		temp = (WORD)(30U + temp);								// 30db���{����
	}
	
	if( 0x00FF < temp ){											// db�ϊ��������ʁA255�ȏ�̏ꍇ
		result_db = 0xFFU;											// 255db�Ƃ���
	}else{
		result_db = (BYTE)temp;									// ���̑��́A�v�Z���ʂ����̂܂ܐݒ�
	}
	
	return( result_db );
}

BYTE	Tu_Average_QInfo( BYTE *rxbuf )
{
	BYTE		work;
	BYTE		chknum;
	TTU_QINFO	*Qinfo;
	
	Qinfo = (PTTU_QINFO)rxbuf;
	/*	QRS�r�b�g(bit7,bit6)�̊m�F				*/
	if ( (Qinfo->Status & BIN_11000000) != BIN_01000000 )
	{	/* QRS��01�łȂ��Ȃ�		*/
		return(2);								/* �����I��(NG)				*/
	}
	
	/* ============================================================================	*/
	/*	Q-Info�擾 ���`�F�b�N�����όv�Z											*/
	/*	  Q-Info���`�F�b�N��pick up1��`�F�b�N���̂������l���g�p(single model)	*/
	/* ============================================================================	*/
#if 1
	/* ==== Q-Info ���� ===========================================================	*/
	if( tu_test_mode == 0xFF )
	{
		if( tu_sme_cnt == 0 )
		{
			work = Tu_QInfo_Chk(0,Qinfo);				/* PI check�� 1�񔻒�				*/
		}
		else
		{
			work = Tu_QInfo_Chk(1,Qinfo);				/* PI check�� 1�񔻒�				*/
		}
		if(work != OK)
		{				/* �������l����m�f��				*/
			return(NG);						/* �����I��(NG)						*/
		}
		chknum = CTU_SEEK_NUM;
	}
	else
	{
		chknum = CTU_TEST_NUM;
	}
#endif
	/* ----- S-Meter ���όv�Z ----------------------------------------------------- */
	Tu_QinfoAvg(tu_qinfochk_num,&tu_sme_cnt,&tu_sme_ave[0],Qinfo->Smeter);

	/* ----- IF-Offset ���όv�Z --------------------------------------------------- */
	Tu_QinfoAvg(tu_qinfochk_num,&tu_if_cnt,&tu_if_ave[0],Qinfo->IFval);

	/* ----- �א�ɲ�� ���όv�Z ---------------------------------------------------- */
	Tu_QinfoAvg(tu_qinfochk_num,&tu_usn_cnt,&tu_usn_ave[0],Qinfo->Rnoise);

	/* ----- ����߽ɲ�� ���όv�Z -------------------------------------------------- */
	Tu_QinfoAvg(tu_qinfochk_num,&tu_wam_cnt,&tu_wam_ave[0],Qinfo->Mnoise);

	/* ===== ����̕��ϒl���p�ӂł��������f =======================================	*/
	if( (tu_sme_cnt  != 1)						/* S-Meter�� ������ 				*/
	  ||(tu_if_cnt != 1)						/* IF-Offset ������ 				*/
	  ||(tu_usn_cnt	 != 1)						/* �א�  ɲ�ޏ����� 				*/
	  ||(tu_wam_cnt	 != 1)						/* ����߽ɲ�ޏ����� 				*/
	)
	{
		return(2);						/* �����I��(OK)						*/
	}

	/* ============================================================================	*/
	/*	Q-Info���σ`�F�b�N															*/
	/* ============================================================================	*/

	/* ----- Q-Info����̓��͐ݒ� -------------------------------------------------	*/
	Qinfo->Smeter = tu_sme_ave[0];					/* 									*/
	Qinfo->IFval  = tu_if_ave[0];
	Qinfo->Rnoise = tu_usn_ave[0];
	Qinfo->Mnoise = tu_wam_ave[0];
	
	/* ==== Q-Info ���� ===========================================================	*/
	work = Tu_QInfo_Chk(2,Qinfo);				/* PI check�� ���ϔ���				*/
	if(work != OK)
	{				/* �������l����m�f��				*/
		return(NG);						/* �����I��(NG)						*/
	}
	else
	{									/* �������l����n�j��				*/
		return(OK);						/* �����I��(OK)						*/
	}


}

BYTE	Tu_QInfo_Chk(BYTE chk_tm, TTU_QINFO *pQInfo)
{
	BYTE	ret;
	
	/* ===== S-Meter ���� =============================================	*/
	ret = Tu_QInfo_SmeterChk(chk_tm,pQInfo->Smeter);
	if( tu_test_mode != 0xFF )
	{
		return (OK);
	}
	if(ret == FALSE) {							/* �������l����m�f��	*/
		return( NG );			//2007.04.24
	}

	/* ===== IF-Offset ���� =========================================== */
	ret = Tu_QInfo_IFChk(chk_tm,pQInfo->IFval);
	if(ret == NG) {								/* �������l����m�f��	*/
		return( NG );				//2007.04.24
	}

	if(Tu_band_chk() != CTU_AM)
	{
		/* ===== �א�ɲ�� ���� ============================================ */
		ret = Tu_CheckFM_USN(chk_tm,pQInfo->Rnoise);
		if(ret == TRUE) {						/* �������l����m�f��	*/
			return( NG );			//2007.04.24
		}
	
		/* ===== ����߽ɲ�� ���� ==========================================	*/
		ret = Tu_CheckFM_WAM(chk_tm,pQInfo->Mnoise);
		if(ret == TRUE) {							/* �������l����m�f��	*/
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
	
	
	// 2008.09.25-1 �I�[�o�[�t���[���������Ȃ��悤�ɑΉ�
	//              ���Z���A�I�[�o�[�t���[���������Ȃ��悤�ɕϐ���WORD�ɃL���X�g����B
	//----------------------
	// �p�����[�^�ɂ�蕪�� 
	//----------------------
	if(Tu_band_chk() == CTU_AM)		/*	BAND��AM	*/
	{
		temp_Nowdb = Tu_AMLeveldB_Conv( data );				// ��MLEVEL��db�ɕϊ�
		r = Tu_dxlo_chk(CTU_AM);	/* [matu120903] */
		if( Item < 2 )
		{
			r = (r * 10)/tu_chkcef[CTU_AM].prec_sm;
		}
	}
	else		/*	FM	*/
	{
		temp_Nowdb = Tu_FMLeveldB_Conv( data );				// ��MLEVEL��db�ɕϊ�
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
	
	// 2008.08.25-1 EEPROM����擾����臒l�Ŕ��肷��
	
	if(Tu_band_chk() == CTU_AM)		/*	BAND��AM	*/
	{
		if( Item == 2 )
		{
			work = tu_chkcef[CTU_AM].if_detail;					//�ڍ׃`�F�b�N �f�[�^������
		}
		else
		{
			work = tu_chkcef[CTU_AM].if_rough;					//���t�`�F�b�N �f�[�^������
		}
	
	}
	else
	{
		if( Item == 2 )
		{
			work = tu_chkcef[CTU_FM].if_detail;			//�ڍ׃`�F�b�N �f�[�^������
		}
		else
		{
			work = tu_chkcef[CTU_FM].if_rough;			//���t�`�F�b�N �f�[�^������
		}
	}

	if(data > work)
	{
		r = NG;									// +�������傫�� = NG
	}
	else
	{
												// �Ƃ������Ƃ́AOK�B
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
	BYTE			TotalNum,			//	(i)���ω�
	TTU_QI_CALC*	pqinf,				//	(o)�ݐ��ް�
	BYTE			value				//	(i)�����ް�
)
{
	BYTE		work[2];
	WORD		usTemp	=	0;

	/* ===== ����f�[�^ =======================================================	*/
	if ( 0 == pqinf->cnt ) {
		if ( TotalNum < 255 ) {				//	�������
			pqinf->cnt	=	(BYTE)(TotalNum + 1);	//	ٰ�݋��p�ׂ̈ɉ��ް��␳
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

	/* ===== �w��񐔂܂ł̃f�[�^��荞�� =====================================	*/
	if ( 1 != pqinf->cnt ) {
		usTemp	=	value;					//	WORD�^�ϐ��ɑ��
	/* ----- �w��񐔖��� ----------------------------------------------------- */
		Tu_Add_WORD(&work[0],usTemp);			//	�ް����Z
		(pqinf->cnt)--;
		if( 1 == pqinf->cnt ) {				//	�w��񐔼ެ�Ď�
			usTemp		=	Tu_Chg_WORD(&work[0]);
		}
		else {
			pqinf->data	=	work[0];		//	�P���Ȍv�Z�r���̊i�[
			pqinf->rem	=	work[1];		//	���Ɨ]����i�[���Ă���킯�ł͂Ȃ�
			return( FALSE );				//	���ϒl�܂�
		}
	/* ----- �w��񐔈ȍ~ ----------------------------------------------------- */
	}
	else {
		/* �i�O��܂ł̕��σf�[�^�j�~�i�w��񐔁|�P�j
			 �{�i�O��̕��όv�Z�Ŕ��������]��j�{�i����v�������l�j 			*/
		usTemp	=	(WORD)((work[0] * (TotalNum-1)) + work[1] + value);
	}

	/* ===== ���ϒl�Z�o =======================================================	*/
	/* �i�����܂ł̉��Z�f�[�^�j���i�w��񐔁j 									*/
	if ( 0 == TotalNum ) {						//	���ꂪ0����������̪�پ��
		work[0]	=	(BYTE)(usTemp);
		work[1]	=	0;							//	�]���ް������Ƃ���
	}
	else {
		work[0]	=	(BYTE)(usTemp/TotalNum);
		work[1]	=	(BYTE)(usTemp%TotalNum);	//	�]���ް��i�[
	}

	pqinf->data	=	work[0];
	pqinf->rem	=	work[1];

	return( TRUE );							//	���ϒlOK
}

/******************************************************************
	BYTE TotalNum,			//	���ω�
	BYTE *pNum,			//	�擾��	1�޲�
	BYTE *pData,			//	���ϒl		2�޲�
	BYTE value				//	����擾�����ް�
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

