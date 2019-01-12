/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: ma_mode.c
		�@�\			: Ҳ�Ӱ�ފĎ�����
						: �@Ҳ�Ӱ�ސؑ֊Ď�����
						: �AӰ�ސؑ�MUTE���䏈��
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun070313]	�ύX
******************************************************************************/
#define	_MA_MODE_C_

#include	"../model.h"
#include	"../comm/common.h"
#include	"../audio/aud_ext.h"
#include	"../power/pw_func_ext.h"
#include	"../tuner/tu_ext.h"

#if _MODEL_EEPROM_
#include	"../eeprom/eeprom_ext.h"
#endif

#include	"main.h"
#include	"ma_ext.h"
#include	"ma_mode.h"

/*-----------------------------------------------------------
          �֐���������
-----------------------------------------------------------*/
static BYTE Ma_next_mode_chg(TMA_MODE, BYTE);
static TMA_MODE Ma_search_next_md(TMA_MODE );
static TMA_MODE	Ma_lastmode_get(void);
static void Ma_mode_chgst(void);
static	void Ma_lastsave(TMA_MODE );

/*-----------------------------------------------------------
         Ҳ�Ӱ�ޏ���ð���
[sun061114] USB IN��Ӱ�ސؑ�/PowerON���Ȃ��悤�Ɏd�l�ύX�Ή�
[sun070410] RDSINT PowerOFF��Ӱ�މ�������悤�ɕύX
			RDS���W���[����PowerOFF��ON�ŋ��������ݏ�ԉ������Ă��邽�߁B
[sun100624] HDRadio�Ƽس�Ӱ�ޒǉ�
[sun100816] TunerMode��HD�@Radio�ڑ�ON��HD�@Radio�ɐ؂�ւ����Ȃ�
			�s��C��
-----------------------------------------------------------*/
#define		CMA_TBLEND	0xff
const	TMA_MODE_COND	ma_mdcond_tbl[] =
{
	/* 								    LAST	ONInt	OffInt	PwrOn	PwrOff	OffKeep */
	{CMA_TUNER,		CMA_PRIO_LV0,		YES,	YES,	NO,		NO,		NO,		YES,},
	{CMA_AUX,		CMA_PRIO_LV0,		YES,	YES,	NO,		NO,		NO,		YES,},
	{CMA_ISR,		CMA_PRIO_LV1,		NO,		YES,	YES,	NO,		NO,		NO,},
	{CMA_TBLEND,	CMA_TBLEND,			NO,		NO,		NO,		NO,		NO,		NO,},
};

/******************************************************************************
.. 		�֐���  �FMa_main
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FҲݺ��۰ُ���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051129] �֐����̂�
[sun060710] PowerOFF��Ӱ����ݼ�ү���ޔ��s���Ȃ��悤�ɕύX
[sun070313]	�ύX
[sun070524] ׽�Ӱ�ދL������悤�ɕύX
******************************************************************************/
void	Ma_mode_main(void)
{
	if (fma_mdchg == ON)
	{
		if(ma_mutetimer == TIMER_OUT)
		{
			fma_mdchg = OFF;
//			ma_last_mode = ma_mode[CMA_CUR_MD].cond.mode;	/* ����Ӱ��׽ĂɋL�� */
			ma_mode[CMA_CUR_MD] = ma_mode[CMA_TARGET_MD];
			
			if (Pw_Power_status_get() == ON)
			{
				Ma_set_Msg(CMA_MODE,CMSG_MODECHG);	/* Ӱ�ސ؂�ւ�ү���ޔ��s */
			}
			ma_mutetimer = CMA_AFTERMUTE;		/* Ӱ�ސ؂�ւ����MUTE��Ͼ�� */
		}
	}
	else if ((ma_mutetimer == TIMER_OUT) &&
			 (fma_mute == ON))
	{
		fma_mute = OFF;
		Aud_set_prg_mute(CAUD_MUTE_MAIN_DT_MUTE,OFF);
	}
	else
	{
		;
	}
}

/******************************************************************************
.. 		�֐���  �FMa_source_chg
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�F������ɂ��Ӱ�ސ؂�ւ�
				�F�@����Ӱ������
				�F  ����Ӱ�ނ�DM��׽�Ӱ�ނɐ؂�ւ�
				�F  ����Ӱ�ނ��D��Ӱ�� ��׽ă��[�h�̐ؑ�
				�F  ����Ӱ�ނ��ʏ�Ӱ�� �ˎ���Ӱ�ނɐؑ�
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060123] �V�K�쐬
[sun060620] �װ�Ӱ�ޒ�TUNER�ɐؑւ���ATUNER�֐ؑւ���悤�ɕύX
[sun070313]	�ύX
[sun070402] ����Ӱ�ނ�LV1�̏ꍇ�́AӰ��OFF��׽�Ӱ�ނ֐؂�ւ�
			ISRӰ�ޒ���src�� �� ISRӰ��OFF���āA׽�Ӱ�ނ֑J��
			RDSINTӰ�ޒ�src���� RDSINTӰ��OFF���āA׽�Ӱ�ނ֑J��
[sun070709] �ŗD��Ӱ��[����TEL�̂�]�ſ���֖ؑ����ɕύX
******************************************************************************/
void Ma_source_chg(void)
{
	TMA_MODE	cur_mode;
	TMA_MODE	next_mode;
	BYTE		iRet;

	cur_mode = ma_mode[CMA_TARGET_MD];

	/* �ؑ֌�Ӱ�޾�� */
	if (cur_mode.cond.prio == CMA_PRIO_LV2)		/* [sun070709] �ŗD��Ӱ�ނſ���֖ؑ��� */
	{
		return;
	}
	else if (cur_mode.cond.prio > CMA_PRIO_LV0)		/* ����Ӱ�ނ͗D��Ӱ��SRC����׽�Ӱ�ނ֑J�� */
	{
		ma_mode_cont[cur_mode.id] = CMA_NOCONN;	/* ����Ӱ��OFF�� */
		next_mode = ma_mode[CMA_LAST_MD];		/* ׽�Ӱ�ނɐ؂�ւ� */
	}
	else
	{
		next_mode = Ma_search_next_md(cur_mode);
		Ma_modetbl_srh(&next_mode);
	}
	iRet = Ma_next_mode_chg(next_mode, CMA_PRIO_NO);

	if (iRet == OK)
	{
		Ma_mode_chgst();
	}
}

/******************************************************************************
.. 		�֐���  �FMa_mode_pwroff
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FPowerOffӰ����������
				�F����Ӱ��PowerOFF�ŉ������邩�������s������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun070410] �V�K�쐬
[sun090403] EEPROM��׽�Ӱ�ޏ����ݗv���ǉ�
[sun090409] ����ē���н�C��
[sun090411] EEPROM�ւ̊i�[��DCP/ACC/BATT�@OFF�ɕύX
******************************************************************************/
void Ma_mode_pwroff(void)
{
	TMA_MODE	cur_mode;
	
	cur_mode = ma_mode[CMA_TARGET_MD];

	/* PowerOFF�ŉ�������Ӱ�ނ����� */
	if(cur_mode.cond.off_keep == NO)
	{
		/* ����Ӱ��OFF����Ĕ��s */
		Ma_mode_event_chk(cur_mode.id,cur_mode.cond.mode,CMA_MODEOFF);
	}
}

/******************************************************************************
.. 		�֐���  �FMa_mode_event_chk
		��  ��	�Fmodeid �� Ӱ��ID�ԍ�
				�Fmode �� Ӱ��
				�Fevent �� ����Ď��
		�߂�l	�F����
		�@  �\	�FӰ�޲������������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060307] �V�K�쐬
[sun070313]	�ύX
[sun070628] USB�L�薳�����f�֐��ɕύX
[sun100812] TunerMode��HDRadio�@ON�ł���΁AHD�@Radio�֑J��
			HD�@Radio��HD�@Radio�@OFF�ł���΁ATuner�֑J��
******************************************************************************/
void Ma_mode_event_chk(BYTE mode_id, BYTE mode, BYTE event)
{
	/* �����ޯ̧�X�V */
	switch(event)
	{
		case	CMA_CONN:
			ma_mode_cont[mode_id] = mode;
			break;
		case	CMA_MODEON:
			ma_mode_cont[mode_id] = mode;
			Ma_mode_on_event(mode_id,mode);
			break;
		case	CMA_MODEOFF:
			ma_mode_cont[mode_id] = CMA_NOCONN;
			Ma_mode_off_event(mode_id,mode);
			break;
		case	CMA_LOAD:
			ma_mode_cont[mode_id] = mode;
			Ma_mode_on_event(mode_id,mode);
			break;
		default:
			break;
	}
}
/******************************************************************************
.. 		�֐���  �FMa_mode_on_event
		��  ��	�Fmodeid �� Ӱ��ID�ԍ�
				�Fmode �� Ӱ��
		�߂�l	�F����
		�@  �\	�FӼޭ��On�ʒm����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060307] �V�K�쐬
[sun060605] н1: On�����ID�������ޯĂɂȂ��Ă��邩�����ǉ�
			�A���װ�ON�ATA ON������Ɖ������t�����Ⴄ�s�
[sun060620] н2�FMP3Ӱ�ނ�Loading��PowerON�ł��Ȃ��s�
			����Ӱ�ނ�PowerOn�������Ă��Ȃ������B
[sun060719]-1 ����Ӱ��ID��EventӰ��ID�������ŕς����Ӱ�ނ̗v������t�����s��C��
            ID��r�ł͂Ȃ�����Ӱ�ނ�eventӰ�ނŔ�r����悤�ɕύX
[sun060913] USB Update��MP3 LOAD��Ӱ�ޑJ�ڂ��Ȃ��悤�ύX
[sun070313]	�ύX
[sun090307] USB���߲�SW�ǉ�
******************************************************************************/
void Ma_mode_on_event(BYTE	mode_id ,BYTE mode)
{
	TMA_MODE	event_mode;
	TMA_MODE	cur_mode;
	BYTE	iRet;

	event_mode.id = mode_id;
	event_mode.cond.mode = mode;
	cur_mode = ma_mode[CMA_TARGET_MD];

	Ma_modetbl_srh(&event_mode);					/* �����Ӱ��ð��ٻ�� */

	iRet = NG;
	if (event_mode.cond.onint != NO)				/* Ӱ��ON�Őؑւ��邩 */
	{
		if (cur_mode.cond.mode != event_mode.cond.mode)		/* [sun060719]-1����Ӱ�ނƓ����� */
		{
			iRet = Ma_next_mode_chg(event_mode, CMA_PRIO_CHK);
		}
	}

	if (event_mode.cond.pwr_on)
	{
		Pw_power_event(mode, ON);
	}

	/* EventMode�֐ؑ֔��� */
	if (iRet == OK)
	{
		Ma_mode_chgst();
	}
}
/******************************************************************************
.. 		�֐���  �FMa_mode_off_event
		��  ��	�Fmode_id �� Ӱ��ID�ԍ�
				�Fmode �� Ӱ��
		�߂�l	�F����
		�@  �\	�FӼޭ��Off�ʒm����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060307] �V�K�쐬
[sun060719]-1 ����Ӱ��ID��EventӰ��ID�������ŕς����Ӱ�ނ̗v������t�����s��C��
            ID��r�ł͂Ȃ�����Ӱ�ނ�eventӰ�ނŔ�r����悤�ɕύX
[sun070313]	�ύX
******************************************************************************/
void Ma_mode_off_event(BYTE	mode_id, BYTE mode)
{
	TMA_MODE	cur_mode;
	TMA_MODE	lst_mode;

	cur_mode = ma_mode[CMA_TARGET_MD];

	if ((cur_mode.cond.mode != mode) ||		/* [sun060719]-1 ����Ӱ�ނƓ����� */
	    (cur_mode.cond.offint == NO))		/* Ӱ��OFF�Őؑւ��邩 */
	{
		return;								/* �X�V�݂̂�SRC�ؑւ��Ȃ� */
	}

	lst_mode = Ma_lastmode_get();
	Ma_next_mode_chg(lst_mode,CMA_PRIO_NO);

	if (cur_mode.cond.pwr_off == YES)
	{
		Pw_power_event(cur_mode.cond.mode,OFF);
	}

	Ma_mode_chgst();
}
/******************************************************************************
.. 		�֐���  �FMa_next_mode_chg
		��  ��	�Fnext_mode �� �ڕWӰ��
				�Fstatus �� �D�����������K�v���ǂ���
		�߂�l	�FOK �� �ڕWӰ����ݼނ���
				�FNG �� �ڕWӰ����ݼނ��Ă��Ȃ�
		�@  �\	�F�ڕWӰ�޾�ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060307] �V�K�쐬
[sun060713] Tuner��TA��M���D������1�ɐݒ�
[sun060802] ��ޭ��w�E�C�� == �� = �ɕύX
			TUNER TA�ȊO�̏ꍇ�����ق�LVO�ɂ��Ă����A���̂܂܂ŕێ�����
[sun060930] TunerӰ�ނ�TA�D�揇�ʂƂȂ��Ă��܂��ƃ\�[�X�������Ȃ��Ȃ�
[sun070313]	�ύX
[sun070402] �D�����ق�LV1�Ŏ���Ӱ�ނւ̑J�ڂ�����������AӰ��OFF�Ƃ���悤��
			�ύX
			ISRӰ�ޒ���CD Loading��ISRӰ��OFF���āAMP3Ӱ�ނ�
			RDSINTӰ�ޒ���CD Loading��RDSINT OFF���āAMP3Ӱ�ނ�
[sun070525] TEL���΂₭�u�f����ƁATEL�ؑւ��Ȃ���CD�߂�̂ŁAReload���Ă��܂�
			���ޯčX�V������A׽Ă��O�����ޯĂ��L������悤�ɕύX
******************************************************************************/
static BYTE	Ma_next_mode_chg(TMA_MODE next_mode, BYTE status)
{
	BYTE	iRet;
	TMA_MODE	cur_mode;

	cur_mode = ma_mode[CMA_TARGET_MD];
	/* Ӱ�ސؑ����� */
	iRet = NG;

	if ((status == CMA_PRIO_NO) ||						/* �D���������� */
		(next_mode.cond.prio >= cur_mode.cond.prio))		/* �ڕWӰ�ނ�����Ӱ�ނ��D�� */
	{
		if (cur_mode.cond.lastsave == ON)
		{
			Ma_lastsave(cur_mode);
		}
		iRet = OK;
	}
	else if (cur_mode.cond.prio == CMA_PRIO_LV1)		/* [sun070402] ����Ӱ��LV1�uISR/RDSINT�v */
	{
		ma_mode_cont[cur_mode.id] = CMA_NOCONN;			/* ����Ӱ��OFF�� */
		iRet = OK;
	}
	else
	{
		if (next_mode.cond.lastsave == ON)
		{
			Ma_lastsave(next_mode);
		}
	}

	if (iRet == OK)
	{
		ma_last_mode = ma_mode[CMA_TARGET_MD].cond.mode;	/* ����Ӱ��׽ĂɋL�� */
		ma_mode[CMA_TARGET_MD] = next_mode;	/* �ڕWӰ�޾�� */
	}
	return(iRet);
}

/******************************************************************************
.. 		�֐���  �FMa_search_next_md
		��  ��	�Fcur_id �� �w��id�̎��̗L��Ӱ�޻��
				�F*next_id �� NEXTӰ�ފi�[�ر
				�F*next_mode_tbl �� NEXTӰ��ð����ް��i�[�ر
		�߂�l	�F
		�@  �\	�F������ɂ�莟�̐ؑ�Ӱ�޻������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060307] �V�K�쐬
******************************************************************************/
static TMA_MODE Ma_search_next_md(TMA_MODE cur_mode)
{
	BYTE	iLoop;
	TMA_MODE	next_mode;

	if (cur_mode.id >= CMA_SRC_MAX)
	{
		next_mode.id = 0;				/* �w��s���ŁA�����擪���*/
		next_mode.cond.mode = ma_mode_cont[0];
		return(next_mode);
	}

	iLoop = cur_mode.id;
	iLoop++;
	while (iLoop != cur_mode.id)
	{
		if (iLoop >= CMA_SRC_MAX)
		{
			iLoop = 0;
		}
		if (ma_mode_cont[iLoop] != CMA_NOCONN)
		{
			break;
		}
		iLoop ++;
	}

	if (iLoop == cur_mode.id)
	{
		iLoop = 0;					/* next���t��Ȃ��A�擪��� */
	}

	next_mode.id = iLoop;
	next_mode.cond.mode = ma_mode_cont[iLoop];
	return(next_mode);
}

/******************************************************************************
.. 		�֐���  �FMa_modetbl_srh
		��  ��	�Fmode �� ����ڕWӰ��
				�F* iTblCnt �� ð��ٔԍ��i�[�ϐ����ڽ
				�F* mode_tbl�� ð����ް��i�[�ϐ��ر
		�߂�l	�FOK �� ��������
				�FNG �� ������Ȃ�����
		�@  �\	�FӰ��ð��ٻ������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060307] �V�K�쐬
******************************************************************************/
void Ma_modetbl_srh( TMA_MODE *mode_tbl )
{
	BYTE	iRet;
	BYTE	iLoop1;
	BYTE	mode;

	mode = mode_tbl->cond.mode;

	iRet = NG;
	/* ����Ӱ�޻�� */
	iLoop1 = 0;
	while (ma_mdcond_tbl[iLoop1].mode != CMA_TBLEND)
	{
		if (ma_mdcond_tbl[iLoop1].mode == mode)
		{
			mode_tbl->cond = ma_mdcond_tbl[iLoop1];
			iRet = OK;
			break;
		}
		iLoop1 ++;
	}
	/* ð��ٻ���ł��Ȃ������ꍇ�́A�擪�ް���ݒ� */
	if (iRet == NG)
	{
		mode_tbl->id = 0;
		mode_tbl->cond = ma_mdcond_tbl[0];
	}
}

/******************************************************************************
.. 		�֐���  �FMa_lastmode_get
		��  ��	�F�Ȃ�
		�߂�l	�F׽�Ӱ��
		�@  �\	�F׽�Ӱ�޻����������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060308] �V�K�쐬
******************************************************************************/
static TMA_MODE	Ma_lastmode_get(void)
{
	TMA_MODE	lastmode;
	/* QAC�΍� */
	lastmode.id = ma_mode[CMA_LAST_MD].id;
	lastmode.cond.mode = ma_mode_cont[lastmode.id];
	Ma_modetbl_srh(&lastmode);
	return(lastmode);
}

/******************************************************************************
.. 		�֐���  �FMa_mode_chgst
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�FӰ�ސֽؑ��ď���
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060308] �V�K�쐬
[sun060509] Ӱ����ݼ�MUTE�����҂��Ԃ�MUTE��ύX�V���Ă��Ȃ��̂ŁA������ϐ؂��܂�
			Ӱ�ސؑւ��Ȃ��Ȃ�s��C��
[sun070822] Tuner�Ԃł̐ؑւ�Mute�����Ȃ��悤�ɏC��
[sun101216] Mode�ؑ֔�������O��BT Fast�Đ�����Request
******************************************************************************/
static void Ma_mode_chgst(void)
{
	if (fma_mdchg == OFF)
	{
		if (Pw_Power_status_get() == ON)
		{
			if (fma_mute == OFF)
			{
				ma_mutetimer = CMA_FRONTMUTE;
			}
			else
			{
				ma_mutetimer = CMA_FRONTMUTE_50;
			}
		}
		else
		{
			ma_mutetimer = 0;
		}
	}

	fma_mdchg = ON;
	fma_mute = ON;
	Aud_set_prg_mute(CAUD_MUTE_MAIN_DT_MUTE,ON);
}

/******************************************************************************
.. 		�֐���  �FMa_lastsave
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�F׽�Ӱ�޾��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060308] �V�K�쐬
******************************************************************************/
static	void Ma_lastsave(TMA_MODE lst_mode)
{
	BYTE	iLoop1;
	BYTE	iLoop2;

	/* ׽�Ӱ�ނƓ��������� */
	if (ma_mode[CMA_LAST_MD].id == lst_mode.id)
	{
		if (ma_mode[CMA_LAST_MD].cond.mode != lst_mode.cond.mode)
		{
			ma_mode[CMA_LAST_MD].cond = lst_mode.cond;
		}
		return;
	}

	/* ����Ӱ��׽Ăɓ����Ă��邩���� */
	for (iLoop1 = CMA_LAST_MD; iLoop1 < CMA_MAX; iLoop1 ++)
	{
		if (lst_mode.id == ma_mode[iLoop1].id)
		{
			break;
		}
	}
	if (iLoop1 >= CMA_MAX)
	{
		iLoop1 = (CMA_MAX - 1);
	}
	/* ׽ē���ւ� */
	for (iLoop2 = iLoop1-1; iLoop2 >= CMA_LAST_MD; iLoop2--,iLoop1--)
	{
		ma_mode[iLoop1] = ma_mode[iLoop2];
	}
	/* ����Ӱ��׽Ăɓ���ւ� */
	ma_mode[CMA_LAST_MD] = lst_mode;
	
}


#undef		_MA_MODE_C_
