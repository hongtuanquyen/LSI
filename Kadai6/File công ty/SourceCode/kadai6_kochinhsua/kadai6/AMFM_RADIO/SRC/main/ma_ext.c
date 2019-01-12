/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: main.c
		�@�\			: SystemҲ�ٰ�ߏ���
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051125]	�V�K�쐬
******************************************************************************/
#define	_MA_EXT_C_

#include	<string.h>
#include	"../model.h"
#include	"../comm/common.h"
#include	"../io/IO_inc.h"
#if (_MODEL_EEPROM_ == YES)
#include	"../power/pw_main.h"
#include	"../eeprom/eeprom_ext.h"
#endif
#if _TEST_MODE_
#include	"../power/pw_func_ext.h"
#include	"../testmode/TEST_inc.h"
#if _MODEL_RADIO_
#include	"../tuner/tu_ext.h"
#endif
#endif
#include	"main.h"
#include	"ma_ext.h"
#include	"ma_mode.h"
#include	"ma_tim.h"


#if _TEST_MODE_
/******************************************************************************
.. 		�֐���  �FMa_Test_Func
		��  ��	�FRcvCmd �� Test�����
				�FRcvBuf �� ��������Ұ��i�[�ޯ̧���ڽ
				�FLength �� ���Ұ��ް���
		�߂�l	�F�Ȃ�
		�@  �\	�F���ꂽMain���[�h��TunerӰ�ނł��邩�ǂ����ʒm����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun070822] �V�K�쐬
[sun100929] PowerOff��Ӱ�ޑJ�ڗv���󂯕t���Ȃ��悤�ɕύX
******************************************************************************/
void	Ma_Test_Func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Lenth)
{
	switch(RcvCmd)
	{
		case	CTEST_MODE_POWERON:
			if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_OK)||
				(Ma_Chk_EEP_Initial() == CMA_INIT_STA_CKAL_NG))
			{
				Pw_power_event(0xff,ON);
			}
			break;
		case	CTEST_MODE_POWEROFF:
			Pw_power_event(0xff,OFF);
			break;
		case	CTEST_MODE_SRC:
			if (Pw_Power_status_get() == OFF)
			{
				return;
			}
			switch(*RcvBuf)
			{
				#if _MODEL_RADIO_
				/* EXT1 = 07  EXT2 = 11 �� AM  EXT2 = 01 ��FM */
				case CTEST_MODE_SRC_TUNER_EXTR1:
					if (*(RcvBuf+1) == CTEST_MODE_SRC_TUNER_AM_EXTR2)
					{
						 Tu_bnd_set(0);
						Ma_mode_event(CMA_ID_TU, CMA_TUNER, CMA_MODEON);
					}
					else if (*(RcvBuf+1) == CTEST_MODE_SRC_TUNER_FM_EXTR2)
					{
						 Tu_bnd_set(1);
						Ma_mode_event(CMA_ID_TU, CMA_TUNER, CMA_MODEON);
					}
					else
					{
						/*������*/
					}
					break;
				#endif
				case	CTEST_MODE_SRC_AUX_EXTR1:
					if (( Ma_model_dest() >= CMA_MODEL_AMFM_J )
					  &&( Ma_model_dest() <= CMA_MODEL_AMFM_X ))
					{
						Ma_mode_event(CMA_ID_AUX1, CMA_AUX, CMA_MODEON);
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
#endif

/******************************************************************************
.. 		�֐���  �FMa_set_Msg
		��  ��	�Fmode��ү���ޔ��s��Ӽޭ��ID
				�FMsg�˔��s�\��ү����ID
		�߂�l	�F����
		�@  �\	�Fү���ޓo�^����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051207] �V�K�쐬
******************************************************************************/
void	Ma_set_Msg(BYTE mode,BYTE Msg)
{
	Ma_message_set(mode,Msg);
}
/******************************************************************************
.. 		�֐���  �FMa_key_src
		��  ��	�F����
		�߂�l	�F����
		�@  �\	�Fsource ������
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051215] �V�K�쐬
[sun060725] DM�֌W�����R�����g
[sun070704] MODE�ؑ֍Œ��ɿ���������ɕύX
******************************************************************************/
void Ma_key_src(void)
{
	if (fma_mdchg == OFF)		/* [sun070704] */
	{
		Ma_source_chg();
	}
}
/******************************************************************************
.. 		�֐���  �FMa_mode_event
		��  ��	�Fmode_id �� �����ޯ̧ID
				�Fmode �� Ҳ�Ӱ�ޒl
				�F
		�߂�l	�F����
		�@  �\	�FӼޭ��On/OFF�ʒm����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060118] �V�K�쐬
******************************************************************************/
void Ma_mode_event(BYTE mode_id, BYTE mode, BYTE event)
{
	if ((mode_id > CMA_ID_MAX) ||
		(mode > CMA_MODE_MAX) ||
		(event >CMA_EVENT_MAX ))
	{
		return;					/* �w��s�� */
	}
	Ma_mode_event_chk(mode_id, mode, event);
}
/******************************************************************************
.. 		�֐���  �FMa_target_mode
		��  ��	�F����
		�߂�l	�F���݂�MainMode
		�@  �\	�F���ޯ�Ҳ�Ӱ�ޒʒm����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun060605] �V�K�쐬
******************************************************************************/
BYTE  Ma_target_mode(void)
{
	return(ma_mode[CMA_TARGET_MD].cond.mode);
}
/******************************************************************************
.. 		�֐���  �FMa_current_mode
		��  ��	�F����
		�߂�l	�F���݂�MainMode
		�@  �\	�F����Ҳ�Ӱ�ޒʒm����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051207] �V�K�쐬
******************************************************************************/
BYTE  Ma_current_mode(void)
{
	return(ma_mode[CMA_CUR_MD].cond.mode);
}
/******************************************************************************
.. 		�֐���  �FMa_model_dest
		��  ��	�F�Ȃ�
		�߂�l	�FModelType�F
				�FCMA_MODEL_U �� USA����
				�FCMA_MODEL_T �� K����
				�FCMA_MODEL_J �� Japan����
				�FCMA_MODEL_E �� ���B����
		�@  �\	�FModelType�ʒm����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051128] �V�K�쐬
******************************************************************************/
BYTE 	Ma_model_dest(void)
{
	return( ma_dest );
}


/******************************************************************************
.. 		�֐���  �FMa_ini_data_set
		��  ��	�Fdata��EEPROM�̏����l�o�b�t�@�A�h���X
		�߂�l	�F����
		�@  �\	�F�����l���i�[����
------------------------------------------------------------------------------
[chin120918] �V�K�쐬
******************************************************************************/
void	Ma_ini_data_set(BYTE *data)
{
	memcpy((BYTE*)&ma_ini_data,data,5);				/* FactoryCode1�A�d�����A���v�A�A���[�� */
	memcpy(&ma_ini_data.fac2[0],(data+10),2);		/* FactoryCode2 */
	memcpy(ma_part_num,data+5,sizeof(ma_part_num));	/* ���i�ԍ� */
	ma_ini_data.ini_status = CMA_INIT_STA_ALL_NG;	/* �����l���s�ɂ��Ă��� */
}

/******************************************************************************
.. 		�֐���  �FMa_part_num_get
		��  ��	�Fn_buf�����i�ԍ��i�[�o�b�t�@�A�h���X
		�߂�l	�F����
		�@  �\	�F���i�ԍ���n��
------------------------------------------------------------------------------
[chin120918] �V�K�쐬
******************************************************************************/
void	Ma_part_num_get(BYTE * n_buf)
{
	BYTE i;
	BYTE te;
	
	for (i = 0;i<sizeof(ma_part_num);i++)
	{
		te = ma_part_num[i];
		n_buf[i*2] = (BYTE)((te>>4)&0x0F);
		n_buf[i*2+1] = (BYTE)(te&0x0F);
	}
}

/******************************************************************************
.. 		�֐���  �FMa_get_clock
		��  ��	�F����
		�߂�l	�FTRUE�����v�L
				  FALSE�����v��
		�@  �\	�F���v�L�����̌��ʂ�ʒm����
------------------------------------------------------------------------------
[chin120918] �V�K�쐬
******************************************************************************/
BYTE 	Ma_get_clock(void)
{
	if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_NG)||
		(Ma_Chk_EEP_Initial() == CMA_INIT_STA_AREA_NG))
	{
		return FALSE;
	}
	return( ma_ini_data.clock );
}

/******************************************************************************
.. 		�֐���  �FMa_get_alarm
		��  ��	�F����
		�߂�l	�FTRUE���A���[���L
				  FALSE���A���[����
		�@  �\	�F�A���[���L�����̌��ʂ�ʒm����
------------------------------------------------------------------------------
[chin120918] �V�K�쐬
******************************************************************************/
BYTE 	Ma_get_alarm(void)
{
	if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_NG)||
		(Ma_Chk_EEP_Initial() == CMA_INIT_STA_AREA_NG))
	{
		return FALSE;
	}
	
	#if 0
	if (ma_ini_data.clock == TRUE)
	{
		return ( ma_ini_data.alarm );
	}
	#else
	if ((ma_dest >= 2)&&
	    (ma_dest <= 7)&&
	    (ma_ini_data.clock == 0x01)&&
	    (ma_ini_data.alarm == 0x01))
	{
		return( TRUE );
	}
	#endif
	else
	{
		return( FALSE );
	}
}

BYTE Ma_is_mode_chg(void)
{
	if(( fma_mdchg == ON ) || ( fma_mute == ON ))
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}
/******************************************************************************
.. 		�֐���  �FMa_get_alarm
		��  ��	�F����
		�߂�l	�FCMA_INIT_STA_ALL_NG �������lNG
				  CMA_INIT_STA_AREA_NG���d����NG
				  CMA_INIT_STA_CKAL_NG�����v�A�A���[��NG
				  CMA_INIT_STA_ALL_OK �������lOK
		�@  �\	�F�����l�̃`�F�b�N���ʂ�ʒm����
------------------------------------------------------------------------------
[chin120918] �V�K�쐬
******************************************************************************/
BYTE 	Ma_Chk_EEP_Initial(void)
{
	#if 1
	return ma_ini_data.ini_status;
	#else
	if (ma_ini_data.ini_status == CMA_INIT_STA_ALL_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	#endif
}

#if 0
/* _MODEL_EEPROM_ */
void Ma_SaveEepData(BYTE *BufAddr)
{
	*BufAddr++ = fpw_power_key;
	*BufAddr++ = ma_mode[CMA_TARGET_MD].id;
	*BufAddr++ = ma_mode[CMA_TARGET_MD].cond.mode;
	*BufAddr++ = ma_mode[CMA_LAST_MD].id;
	*BufAddr++ = ma_mode[CMA_LAST_MD].cond.mode;
}
/* ******************************************************************************
 	�֐���  �FMa_eeprom_datachk
	��  ��	�Fdata �� EEPROM�ް�
			�Fstatus �� �ް����
	�߂�l	�FOK  �� �ް�����
			�FNG  �� �ް��ُ�
	�@  �\	�FEEPROM�Ɋi�[���Ă����ް��͈���������
	�C������	�y�N�����z�y���O�z �y�����z
------------------------------------------------------------------------------
[chin090326]�V�K�쐬
[sun090403]������Ӱ��EEPROM�֋L�����Ȃ�
**********************************************************************************/
BYTE	Ma_eeprom_datachk(WORD		data,	BYTE	eep_id)
{
	BYTE	iRet;
	iRet = FALSE;
	if (eep_id == CEEP_MN_LSTMODE)
	{
		if ((data == 0x0001)||(data == 0x0102)||(data == 0x0304))
		{
			iRet = TRUE;
		}
	}
	return iRet;
}
/*******************************************************************************
 	�֐���  �FMa_eeprom_initial
	��  ��	�F�Ȃ�
	�߂�l	�F�Ȃ�
	�@  �\	�FEEPROM�Ɋi�[���Ă����ް������l�����ݏ���
			flcd_dimmer	:
			flcd_scroll	:
			flcd_ssp	:
	�C������	�y�N�����z�y���O�z �y�����z
------------------------------------------------------------------------------
[chin090326]�V�K�쐬
**********************************************************************************/
void Ma_eeprom_initial(void)
{
	WORD lstmode;
	lstmode = (WORD)CMA_ID_TU;
	lstmode = lstmode << 8;
	lstmode |= (WORD)CMA_TUNER;
	eeprom_data_writereq(CEEP_MN_LSTMODE,lstmode);
}

/******************************************************************************
.. 		�֐���  �FMa_mode_eeprom_set
		��  ��	�F�Ȃ�
		�߂�l	�F����
		�@  �\	�FACC/DCP/BATT�@OFF��MainӰ�ނ�EEPROM�֊i�[����
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun090411] Ma_mode_pwroff����ړ�
******************************************************************************/
void	Ma_eeprom_data_set(void)
{
	WORD	mode;
	
	mode = (WORD)ma_mode[CMA_TARGET_MD].id;
	mode <<= 8;
	mode |= (WORD)ma_mode[CMA_TARGET_MD].cond.mode;
	eeprom_data_writereq(CEEP_MN_LSTMODE,mode);
}

#endif

#undef	_MA_EXT_C_
