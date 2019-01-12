/***********************************************************************
	�N�����I���a���� 
	�v���W�F�N�g��	�F2008�NLOW2L/LOW3����
	�t�@�C����		�Ftes_mode.c
	���W���[����	�FSUB���[�h(ADJ����Ӱ��)
	�@   �\			�FADJ��������
------------------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------
			2007/4/16		0.1		��		�V�K
***********************************************************************/
#include	"../model.h"

#if _TEST_MODE_

#define		_TEST_MAIN_C_


#include    <string.h>
#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#include	"../key/key_func_ext.h"
#include	"../key/key_def.h"
#include	"../disp/lcd_ext.h"
#include	"../main/ma_ext.h"
#include	"../audio/aud_ext.h"
#include	"../audio/aud_def.h"
#include	"../power/pw_func_ext.h"
#include	"../tuner/TU_inc.h"
#if	_MODEL_EEPROM_
#include	"../eeprom/eeprom_ext.h"
#endif

#include	"TEST_inc.h"

#define		_INT_TAISAKU_	0

#define		test_dec_len			test_cmd_buf[0]				/* ��M�f�[�^���� */
#define		test_dec_dl				test_cmd_buf[4]				/* ��M�f�[�^�� */
#define		test_dec_mg				test_cmd_buf[5]				/* ��M�R�}���h��MG */
#define		test_dec_ln				test_cmd_buf[7]				/* ��M�R�}���h��LN */
#define		test_dec_cmd			test_cmd_buf[11]			/* ��M�R�}���h */
#define		test_dec_para			test_cmd_buf[12]			/* ��M�R�}���h��PARA */
#define		test_dec_para_adr		(&test_cmd_buf[12])			/* ��M�f�[�^�̃p�����[�^�A�h���X */
#define		test_dec_size			(test_dec_dl-8)				/* ��M�f�[�^�̃p�����[�^�̃����O�X */

static void test_cmd_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length);
static void test_clk_beep_set(BYTE* para);
#if 0
static void test_atomic_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length);
#endif

enum{
	CTEST_STS_STOP,
	CTEST_STS_RCV,
	CTEST_STS_DEC,
	CTEST_STS_RESULT,
	CTEST_STS_SND
};
static		BYTE test_status;

#define		CTEST_RCV_MAX		32
#define		CTEST_SND_MAX		37


/*--------------------------------- */
/* ��M�֌W                         */
/*--------------------------------- */
static		BYTE TestRcvBuf[3][CTEST_RCV_MAX];
static		BYTE TestRcvCnt;
static		BYTE TestRcvCmdCnt;
static		BYTE TestRcvLen;
static		BYTE TestRcvTime;

static		BYTE test_cmd_buf[CTEST_RCV_MAX];

/*--------------------------------- */
/* ���M�֌W                         */
/*--------------------------------- */
static		BYTE TestSndBuf[CTEST_SND_MAX];
static		BYTE TestSndCnt;
static		BYTE TestSndLen;



typedef	 struct{
	BYTE	test_len;			/* */
	BYTE	test_dl;			/* */
	BYTE	test_mg;			/* */
	BYTE	test_tl;			/* */
	BYTE	test_ln;			/* */
	BYTE	test_ls1;			/* */
	BYTE	test_ls2;			/* */
	BYTE	test_st;			/* */
	BYTE	test_cmd;			/* */
	BYTE	ret;				/* �ԐM�K�v�� */
	BYTE	command;			/* */
	void	(*Test_func)( BYTE ,BYTE *,BYTE);
}TEST_MODE_COND;


static TEST_MODE_COND test_cur_cmd;

static const TEST_MODE_COND	test_md_20_tbl[] ={
/*		LEN		DL		MG		TL		LN		LS1		LS2		ST		CMD		RET		COMMAND							,test_func					*/
	{	0x0B,	0x08,	0x20,	0xE1,	0x00,	0x00,	0x00,	0x01,	0xFF,	0x00,	CTEST_MODE_TEST_ON				,test_cmd_func			},
	{	0x0B,	0x08,	0x20,	0xE1,	0x00,	0x00,	0x00,	0x01,	0x01,	0x00,	CTEST_MODE_POWERON				,Ma_Test_Func				},
	{	0x0B,	0x08,	0x20,	0xE1,	0x00,	0x00,	0x00,	0x01,	0x02,	0x00,	CTEST_MODE_POWEROFF				,Ma_Test_Func				},
	{	0x0D,	0x0A,	0x20,	0xE1,	0x00,	0x00,	0x00,	0x01,	0x00,	0x00,	CTEST_MODE_SRC					,Ma_Test_Func				},
	{	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	CTEST_MODE_MAX					,NULL						}
};

static const TEST_MODE_COND	test_md_30_tbl[] ={
	{	0x0C,	0x09,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0xF4,	0x00,	CTEST_MODE_AUD_DIR_VOL			,Aud_test_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0xE4,	0x00,	CTEST_MODE_AUD_BAL_RMAX			,Aud_test_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0xE5,	0x00,	CTEST_MODE_AUD_BAL_LMAX			,Aud_test_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0xE3,	0x00,	CTEST_MODE_AUD_BAL_CEN			,Aud_test_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0xE9,	0x00,	CTEST_MODE_AUD_BAS_MAX			,Aud_test_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0xEA,	0x00,	CTEST_MODE_AUD_BAS_MIN			,Aud_test_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0xEB,	0x00,	CTEST_MODE_AUD_TRE_MAX			,Aud_test_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0xEC,	0x00,	CTEST_MODE_AUD_TRE_MIN			,Aud_test_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x00,	0x00,	0x00,	0x01,	0xEF,	0x00,	CTEST_MODE_AUD_BSTR_CEN			,Aud_test_func			},
	{	0x0C,	0x09,	0x30,	0xE1,	0x02,	0x00,	0x00,	0x01,	0x1A,	0x00,	CTEST_MODE_AUD_LOUD_ONOFF		,Aud_test_func			},

	{	0x0C,	0x09,	0x30,	0xE1,	0x00,	0x00,	0x00,	0x01,	0x16,	0x00,	CTEST_MODE_TUNER_DIR_PCH		,Tu_cmd_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x00,	0x00,	0x00,	0x01,	0x05,	0x00,	CTEST_MODE_TUNER_SEEKUP			,Tu_cmd_func			},
	{	0x0B,	0x08,	0x30,	0xE1,	0x00,	0x00,	0x00,	0x01,	0x06,	0x00,	CTEST_MODE_TUNER_SEEKDW			,Tu_cmd_func			},
	{	0x0E,	0x0B,	0x30,	0xE1,	0x00,	0x00,	0x00,	0x01,	0x40,	0x00,	CTEST_MODE_TUNER_DIR_NCH		,Tu_cmd_func			},
	{	0x0C,	0x09,	0x30,	0xE1,	0x00,	0x00,	0x00,	0x01,	0x86,	0x00,	CTEST_MODE_TUNER_LO_DX			,Tu_cmd_func			},
	{	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	CTEST_MODE_MAX					,NULL					}
};

static const TEST_MODE_COND	test_md_60_tbl[] ={
	{	0x0C,	0x09,	0x60,	0xE1,	0x00,	0x00,	0x00,	0x01,	0xE6,	0x00,	CTEST_MODE_AUD_MUTE_ONOFF		,Aud_test_func			},
	{	0x0D,	0x0A,	0x60,	0xE1,	0x00,	0x00,	0x00,	0x01,	0xEC,	0x01,	CTEST_MODE_ATMIC_IC				,Tu_cmd_func			},
	{	0x1F,	0x1C,	0x60,	0xE1,	0xE8,	0x00,	0x00,	0x01,	0xD0,	0x01,	CTEST_MODE_ATMIC_EEP			,EEP_Test_func			},

	{	0x0C,	0x09,	0x60,	0xE1,	0x00,	0x00,	0x00,	0x01,	0xFA,	0x01,	CTEST_MODE_OTHER_GET_PORT		,test_cmd_func			},
	{	0x0D,	0x0A,	0x60,	0xE1,	0x0F,	0x00,	0x00,	0x01,	0xFB,	0x00,	CTEST_MODE_OTHER_BEEP_ONOFF		,Aud_test_func			},
	{	0x0C,	0x09,	0x60,	0xE1,	0x00,	0x00,	0x00,	0x01,	0xF8,	0x00,	CTEST_MODE_OTHER_STBY_ONOFF		,Pw_test_func			},
	{	0x0C,	0x09,	0x60,	0xE1,	0x00,	0x00,	0x00,	0x01,	0xE0,	0x00,	CTEST_MODE_OTHER_CLK_OUTPUT		,test_cmd_func			},
	{	0x0B,	0x08,	0x60,	0xE1,	0x39,	0x00,	0x00,	0x01,	0xF4,	0x00,	CTEST_MODE_TUNER_INI_PCH		,Tu_cmd_func			},

	{	0x0F,	0x0C,	0x60,	0xE1,	0xB1,	0x01,	0x00,	0x01,	0xF1,	0x00,	CTEST_MODE_LCD_TEST				,Lcd_testmode_func		},
	{	0x0C,	0x09,	0x60,	0xE1,	0x00,	0x00,	0x00,	0x01,	0xFB,	0x01,	CTEST_MODE_KEY_TEST				,test_cmd_func			},
	{	0x0B,	0x08,	0x60,	0xE1,	0x00,	0x00,	0x00,	0x00,	0xEE,	0x01,	CTEST_MODE_OTHER_GET_VER		,test_cmd_func			},

	{	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	CTEST_MODE_MAX					,NULL						}
};

#if _INT_TAISAKU_
BYTE tst_rx_sta;
BYTE tst_rx_sta_bak;
#endif
static void test_sts_rcv( void );
static void test_sts_dec( void );
static void test_sts_result( void );
static void test_sts_snd( void );
static void test_chk_sum(void);

static void test_rcv_reset(void);
static void test_snd_reset(void);
static void test_chk_sum(void);
static BYTE test_revdata_chk( TEST_MODE_COND );
static BYTE test_cmd_start_chk(void );

/******************************************************************************
		�֐���  �FTest_Mode_Int_Rcv()
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTESTMODE �f�[�^�擾����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[chin20120904]�V�K�쐬
******************************************************************************/
void Test_Mode_Int_Rcv(void)
{
	BYTE rev_data;
	
	rev_data = IO_Test_Get_RxData();
	
	if (TestRcvCnt == 0)						/* 1BYTE�ڎ�M�F����M���i�[���Ă��� */
	{
		if (rev_data != 0x00)
		{
			TestRcvLen = rev_data;
			TestRcvBuf[TestRcvCmdCnt][TestRcvCnt] = rev_data;
			TestRcvCnt ++;
		}
		/* ��M�^�C�}�[�A�E�g�J�n�ݒ� */
		TestRcvTime = (100/10)+1;
	}
	else if (TestRcvCnt >= TestRcvLen)		/* ��M���� */
	{
		TestRcvBuf[TestRcvCmdCnt][TestRcvCnt] = rev_data;
		/* ��M�o�b�t�@�J�E���g */
		memcpy(test_cmd_buf,&TestRcvBuf[TestRcvCmdCnt],TestRcvLen+1);
		memset(&TestRcvBuf[TestRcvCmdCnt],0x00,sizeof(TestRcvBuf[TestRcvCmdCnt]));
		TestRcvCnt = 0;
		TestRcvLen = 0;
		if (++TestRcvCmdCnt >=3)
		{
			TestRcvCmdCnt = 0;
		}
		
		/* ��M�^�C�}�N���A */
		TestRcvTime = 0;
		
		/* UART��M���� */
		ftest_rcv_ok = OK;
		
	
	}
	else									/* ����1BYTE��M */
	{
		TestRcvBuf[TestRcvCmdCnt][TestRcvCnt] = rev_data;
		TestRcvCnt ++;
	}
}
/******************************************************************************
		�֐���  �FTest_Mode_Int_Snd
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTESTMODE �������݃f�[�^����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[chin20120904]�V�K�쐬
******************************************************************************/
void Test_Mode_Int_Snd(void)
{
	if((ftest_sending == TRUE)&&( TestSndLen!= 0x00))
	{
		if(TestSndCnt < TestSndLen)
		{
			IO_Test_TxNext(TestSndBuf[TestSndCnt]);
			TestSndCnt ++;
		}
		else
		{
			test_snd_reset();
		}
	}
}

/******************************************************************************
		�֐���  �FTest_Mode_Initial
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTESTMODE ����������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[chin20120904]�V�K�쐬
******************************************************************************/
void Test_Mode_Initial(void)
{
	/* �o�b�t�@�N���A */
	memset(TestRcvBuf,0x00,sizeof(TestRcvBuf));
	memset(TestSndBuf,0x00,sizeof(TestSndBuf));
	memset(test_cmd_buf,0x00,sizeof(test_cmd_buf));
	memset(&test_cur_cmd,0x00,sizeof(test_cur_cmd));
	
	test_status = CTEST_STS_STOP;
	
	/* ��M�p�ϐ� */
	TestRcvCmdCnt = 0;
	TestRcvCnt = 0;
	TestRcvLen = 0;
	TestRcvTime = 0;
	ftest_rcv_ok = NG;
	ftset_started = NG;
	ftest_result = NG;
	
	/* ���M�p�ϐ� */
	TestSndCnt = 0;
	ftest_sending = NG;
	
#if _INT_TAISAKU_
	tst_rx_sta = 0;
	tst_rx_sta_bak = 0;
#endif
}

static void test_sts_rcv( void )
{
	if ( ftest_rcv_ok == TRUE )
	{
		if (ftset_started == OK )		/* TestMode�� */
		{
			test_status = CTEST_STS_DEC;
			ftest_rcv_ok = NG;
		}
		else
		{
			#if 0
			if ((test_cmd_buf[5] == 0x20)&&(test_cmd_buf[11] == 0xFF))		/* test start command */
			#else
			if (test_cmd_start_chk() == TRUE)
			#endif
			{
				ftset_started = OK;		/* test mode�J�n */
				ftest_rcv_ok = NG;		/* �Ď�M�� */
			}
			else
			{
				/* �R�}���h�󂯕t���Ȃ��A��M�҂���Ԃɖ߂� */
				test_rcv_reset();
				ftest_rcv_ok = NG;		/* �Ď�M�� */
			}
		}
	}
}

static void test_sts_dec( void )
{
	BYTE t_loop;
	BYTE t_tbl_size;
	BYTE test_and;
	const TEST_MODE_COND *p_tbl;
	void (*dec_func)(BYTE, BYTE*, BYTE );
	
	memset(&test_cur_cmd,0x00,sizeof(test_cur_cmd));
	
	switch (test_dec_mg)
	{
		case 0x20:
			p_tbl = test_md_20_tbl;
			t_tbl_size = sizeof(test_md_20_tbl)/sizeof(TEST_MODE_COND);
			break;
		case 0x30:
			p_tbl = test_md_30_tbl;
			t_tbl_size = sizeof(test_md_30_tbl)/sizeof(TEST_MODE_COND);
			break;
		case 0x60:
			p_tbl = test_md_60_tbl;
			t_tbl_size = sizeof(test_md_60_tbl)/sizeof(TEST_MODE_COND);
			break;
		default:
			/* �G���[���� */
			test_rcv_reset();
			test_status = CTEST_STS_RCV;
			return;
	}
	
	for (t_loop = 0; t_loop < (t_tbl_size); t_loop++)
	{
		if ((test_dec_cmd == p_tbl[t_loop].test_cmd)&&(test_dec_ln == p_tbl[t_loop].test_ln))
		{
			#if 0
			dec_func = p_tbl[t_loop].Test_func;
			(*dec_func)(p_tbl[t_loop].command, test_dec_para_adr, test_dec_size);
			#endif
			test_cur_cmd = p_tbl[t_loop];
			break;
		}
		
		if (p_tbl[t_loop].test_len == 0xFF)
		{
			test_status = CTEST_STS_RCV;
			test_rcv_reset();
			return ;
		}
	}
	
	if (test_revdata_chk(test_cur_cmd) == TRUE)						/* ��M�f�[�^���������`�F�b�N */
	{
		dec_func = p_tbl[t_loop].Test_func;
		(*dec_func)(p_tbl[t_loop].command, test_dec_para_adr, test_dec_size);
		
		if(p_tbl[t_loop].ret == 0x00 )			/* �ԐM�v������H */
		{
			test_status = CTEST_STS_RCV;
			test_rcv_reset();
		}
		else
		{
			test_and = test_cmd_buf[13];
			test_and = test_and&0xF0;
			if ((test_cur_cmd.command == CTEST_MODE_ATMIC_EEP)&&(test_and == 0x00 ))			/* EEPROM�����݃R�}���h */
			{
				test_status = CTEST_STS_RCV;
				test_rcv_reset();
			}
			else													/* �ȊO�̃R�}���h */
			{
				test_status = CTEST_STS_RESULT;
				TestRcvTime = (1000/10)+1;
			}
		}
	}
	else
	{
		test_status = CTEST_STS_RCV;
		test_rcv_reset();
	}
}


static void test_sts_result( void )
{
	BYTE buf_pt;
	buf_pt = 0x10;
	
	if (ftest_result == OK)
	{
	/*  TestSndBuf[0] = 0x0F; */		/* ���f�[�^�� */
		TestSndBuf[1] = 0xC0;			/* �Œ�F0xC0 */
		TestSndBuf[2] = 0x01;			/* �Œ�F0x01 */
		TestSndBuf[3] = 0x00;			/* �Œ�F0x00 */
		TestSndBuf[4] = 0x00;			/* �Œ�F0x00 */
		TestSndBuf[5] = 0x00;			/* �Œ�F0x00 */
		TestSndBuf[6] = 0x00;			/* �Œ�F0x00 */
		TestSndBuf[7] = 0x00;			/* �Œ�F0x00 */
		TestSndBuf[8] = 0x0F;			/* �Œ�F0x0F */
	/*  TestSndBuf[9] = 0x0F; */		/* DL */
		TestSndBuf[10] = 0x80;			/* �Œ�F0x80 */
		TestSndBuf[12] = 0xE1;			/* �Œ�F0xE1 */
		TestSndBuf[13] = 0x00;			/* �Œ�F0x00 */
		TestSndBuf[14] = 0x00;			/* �Œ�F0x00 */
		TestSndBuf[15] = 0x01;			/* �Œ�F0x01 */
		
		switch (TestReplyCmd)
		{
			case CTEST_MODE_ATMIC_EEP:
				TestSndBuf[0] = 0x11+TestReplyLen;			/* LEN */
				TestSndBuf[9] = 0x08+TestReplyLen;			/* DL */
				TestSndBuf[11] = 0xE8;						/* TL */
				TestSndBuf[buf_pt++] = TestReplyPara;	/* DT */
				memcpy(&TestSndBuf[buf_pt],TestReplyBuf,TestReplyLen);
				break;
			case CTEST_MODE_ATMIC_IC:
				TestSndBuf[0] = 0x11+TestReplyLen;			/* LEN */
				TestSndBuf[9] = 0x08+TestReplyLen;			/* DL */
				TestSndBuf[11] = 0x00;						/* TL */
				TestSndBuf[buf_pt++] = TestReplyPara;		/* DT */
				memcpy(&TestSndBuf[buf_pt],TestReplyBuf,TestReplyLen);
				break;
			case CTEST_MODE_OTHER_GET_PORT:					/* ���̓|�[�g��Ԏ擾 */
				TestSndBuf[0] = 0x11+TestReplyLen;			/* LEN */
				TestSndBuf[9] = 0x08+TestReplyLen;			/* DL */
				TestSndBuf[11] = 0xAF;						/* TL */
				TestSndBuf[buf_pt++] = TestReplyPara;		/* DT */
				memcpy(&TestSndBuf[buf_pt],TestReplyBuf,TestReplyLen);
				break;
			case CTEST_MODE_KEY_TEST:						/* KEY�e�X�g */
			case CTEST_MODE_TUNER_GET_STA:					/* radio��Ԏ擾 */
				TestSndBuf[0] = 0x11+TestReplyLen;			/* LEN */
				TestSndBuf[9] = 0x08+TestReplyLen;			/* DL */
				TestSndBuf[11] = 0xAF;						/* TL */
				TestSndBuf[buf_pt++] = TestReplyPara;		/* DT */
				memcpy(&TestSndBuf[buf_pt],TestReplyBuf,TestReplyLen);
				break;
			case CTEST_MODE_OTHER_GET_VER:					/* Verson�擾 */
				TestSndBuf[0] = 0x11+TestReplyLen;			/* LEN */
				TestSndBuf[9] = 0x08+TestReplyLen;			/* DL */
				TestSndBuf[11] = 0x00;						/* TL */
				TestSndBuf[buf_pt++] = TestReplyPara;		/* DT */
				memcpy(&TestSndBuf[buf_pt],TestReplyBuf,TestReplyLen);
				break;
			default :
				break;
			
		}
		test_chk_sum();
		test_status = CTEST_STS_SND;
		TestSndLen = TestSndBuf[0]+1;
		TestSndCnt = 0;
		IO_Test_TxStart(TestSndBuf[TestSndCnt++]);
		ftest_sending = OK;
		ftest_result = NG;
	}
}

static void test_sts_snd( void )
{
	if (ftest_sending == NG)
	{	
		test_status = CTEST_STS_RCV;
	}
}

static void test_chk_sum(void)
{
	BYTE iCt;
	BYTE chksum;
	
	chksum = 0;
	for (iCt = 9;iCt<TestSndBuf[0];iCt ++ )
	{
		chksum += TestSndBuf[iCt];
	}
	TestSndBuf[iCt] = chksum;
}
/******************************************************************************
		�֐���  �FTEST_Mode_main
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FTESTMODE ���C������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
			2010/08/24		0.1		��		�V�K
******************************************************************************/
void Test_Mode_main(void)
{
	BYTE i;
	BYTE user_data4;
	BYTE ret;
	BYTE iRet;

/*-----------------------*/	
	if(Pw_ACC_status_get() != TRUE)			/*	BATT/ACC ON		*/
	{
		test_status = CTEST_STS_RCV;
		test_rcv_reset();
		ftest_rcv_ok = NG;
		return;
	}
	
#if _INT_TAISAKU_
	if (tst_rx_sta_bak == 0 )
	{
		tst_rx_sta = pUART_RX_IN;
		
		if (tst_rx_sta == HIGH )
		{
			IO_Test_SetUartPort(ON);
			tst_rx_sta_bak = 1;
		}
	}
#endif
	
	switch(test_status)
	{
		case CTEST_STS_RCV:
			test_sts_rcv();
			break;
			
		case CTEST_STS_DEC:
			test_sts_dec();
			break;
			
		case CTEST_STS_RESULT:
			test_sts_result();
			break;
			
		case CTEST_STS_SND:
			test_sts_snd();
			break;
			
		default:
			test_status = CTEST_STS_RCV;
			test_rcv_reset();
			ftest_rcv_ok = NG;
			break;
	}
}

void Test_10ms_timer(void)
{
	if (TestRcvTime > 0)
	{
		TestRcvTime --;
		if (TestRcvTime == 0)
		{
			/* ��M������~�A��M�f�[�^�p�� */
			test_status = CTEST_STS_RCV;
			ftest_rcv_ok = NG;
			test_rcv_reset();
			test_snd_reset();
		}
	}
}


static void test_rcv_reset(void)
{
	/* ��M�o�b�t�@�N���A */
	memset(test_cmd_buf,0x00,sizeof(test_cmd_buf));
	
	/* �֌W�ϐ��N���A */
	TestRcvCnt = 0;
	TestRcvLen = 0;
	TestRcvTime = 0;
}
static void test_snd_reset(void)
{
	IO_Test_TxEnd();
	
	memset(TestSndBuf,0xFF,sizeof(TestSndBuf));
	TestSndCnt = 0x00;
	TestSndLen = 0x00;
	ftest_sending = FALSE;
	TestRcvTime = 0;
}

static BYTE test_cmd_start_chk(void )
{
	BYTE ret_s;
	
	ret_s = FALSE;
	test_cur_cmd = test_md_20_tbl[0];
	
	/* test_revdata_chk()�̒��ɁALN�̓`�F�b�N���ĂȂ�����ASTART�R�}���h�͂����Ŋm�F���� */
	if ((test_cmd_buf[5]  == test_cur_cmd.test_mg)&&
		(test_cmd_buf[7]  == test_cur_cmd.test_ln)&&
		(test_cmd_buf[11] == test_cur_cmd.test_cmd))
	{
		ret_s = test_revdata_chk(test_cur_cmd);
	}
	return ret_s;
}


static BYTE test_revdata_chk( TEST_MODE_COND mode_cond)
{
	if (test_cmd_buf[0] > mode_cond.test_len)					/* ���f�[�^�� */
	{
		return FALSE;
	}
	if (test_cmd_buf[1] != 0x14)					/* �Œ�l0x14 */
	{
		return FALSE;
	}
	if (test_cmd_buf[2] != 0x00)					/* �Œ�l0x00 */
	{
		return FALSE;
	}
	if (test_cmd_buf[3] != 0x00)					/* �Œ�l0x00 */
	{
		return FALSE;
	}
	if (test_cmd_buf[4] > mode_cond.test_dl)					/* DL�� */
	{
		return FALSE;
	}
	
	if (test_cmd_buf[6] != mode_cond.test_tl)					/* TL */
	{
		return FALSE;
	}
	
	#if 0
	if (test_cmd_buf[7] != mode_cond.test_ln)					/* LN */
	{
		return FALSE;
	}
	#endif
	
	if (test_cmd_buf[8] != mode_cond.test_ls1)					/* LS1 */
	{
		return FALSE;
	}
	if (test_cmd_buf[9] != mode_cond.test_ls2)					/* LS2 */
	{
		return FALSE;
	}
	if (test_cmd_buf[10] != mode_cond.test_st)					/* ST */
	{
		return FALSE;
	}
	return TRUE;
}

static void test_cmd_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length)
{
	BYTE rpy_buf[11];
	BYTE ver_b[2];
	BYTE ver;

	switch (RcvCmd)
	{
		case CTEST_MODE_OTHER_GET_PORT:
			rpy_buf[0] = 0xFA;
			rpy_buf[1] = IO_In_acc_det();
			rpy_buf[2] = MIO_IN_pILL_DET();
			rpy_buf[3] = MIO_IN_pALARM_KEY();
			#if (_POWER_DIAG_ == 0)
			Test_data_reply(RcvCmd,0x2C,rpy_buf,4);		/* �ԐM�K�v */
			#else
			rpy_buf[4] = MIO_IN_pDIAG();
			Test_data_reply(RcvCmd,0x2C,rpy_buf,5);		/* �ԐM�K�v */
			#endif
			break;
		
		case CTEST_MODE_OTHER_CLK_OUTPUT:
			/* ���v�������[�h */
			test_clk_beep_set(RcvBuf);					/* �ԐM�s�v */
			break;
		
		case CTEST_MODE_OTHER_GET_VER:
			Ma_part_num_get(rpy_buf);
			memcpy(ver_b,_VERSION_,2);
			ver = (BYTE)((ver_b[0]-0x30)<<4) + (BYTE)(ver_b[1]-0x30);
			rpy_buf[10] = ver;
			Test_data_reply(RcvCmd,0x29,rpy_buf,11);	/* �ԐM�K�v */
			break;
		
		case CTEST_MODE_KEY_TEST:
			/* KEYTEST�R�}���h��Tuner��Ԏ擾�R�}���h�̓R�}���h�������Ȃ̂ŁA�p�����[�^�Ŕ��f���� */
			if (*RcvBuf == 0x22)
			{
				Key_testmode_func(CTEST_MODE_KEY_TEST,RcvBuf,Length);
			}
			else if (*RcvBuf == 0x23)
			{
				Tu_cmd_func(CTEST_MODE_TUNER_GET_STA,RcvBuf,Length);
			}
			else
			{
				test_rcv_reset();
				ftest_rcv_ok = NG;
				test_status = CTEST_STS_RCV;		/* �Ď�M�� */
			}
			break;
		
		default :
			test_rcv_reset();
			ftest_rcv_ok = NG;
			test_status = CTEST_STS_RCV;		/* �Ď�M�� */
			break;
	}
	
	return;
}

static void test_clk_beep_set(BYTE *para)
{
	if (para[0] == 0x01)
	{
		Io_beep_test_8Hz_start();
	}
}

void Test_maincnt_off(void )
{
	Test_Mode_Initial();
	IO_Test_SetUartPort(OFF);
}
void Test_maincnt_on(void )
{
#if (_INT_TAISAKU_ == 0)
	IO_Test_SetUartPort(ON);
#endif
	#if 0
	IO_Test_RxStart();
	#endif
	test_status = CTEST_STS_RCV;
	
}


#undef		_TEST_MAIN_C_

#endif		/* _TEST_MODE_	100829��	*/

