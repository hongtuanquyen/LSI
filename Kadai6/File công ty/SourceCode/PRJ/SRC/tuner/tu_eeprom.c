/********************************************************************************/
/*		tu_eeprom.c																*/
/*																				*/
/*		�v���W�F�N�g���FTuner													*/
/*		�t�@�C�����F															*/
/*		���W���[�����F															*/
/*		�@�\�F			EEPROM�֌W����											*/
/*																				*/
/*																				*/
/*																				*/
/*			�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z			*/
/*------------------------------------------------------------------------------*/
/*						2010.08.06		0.0			�x		�V�K�쐬			*/
/*[20120629chin]	U�����d�l�ύX U�����d�l�ύX step:200��100kHz				*/
/********************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_

#if	(_MODEL_EEPROM_ == 1)

#define	_TU_EEPROM_C_

#include	<string.h>
#include	"../comm/common.h"
#include	"../eeprom/eeprom_ext.h"
#include	"../main/ma_ext.h"
#include	"tu_def.h"
#include	"tu_event.h"
#include	"tu_main.h"
#include	"tu_eeprom.h"
#include	"tu_ext.h"

/********************************************************************************/
/*	�֐���  �FTu_CheckInvalidData												*/
/*	��  ��	�Fcheckbuf:�`�F�b�N�p�f�[�^�擪�A�h���X								*/
/*			  maxfm:�ő�FM���g��												*/
/*			  maxam:�ő�AM���g��												*/
/*	�߂�l	�FTRUE:�L���f�[�^/FALSE�F�����f�[�^									*/
/*	�@  �\	�FEEPROM����擾������T�ԃ������f�[�^�L�����`�F�b�N				*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.06		0.0			�x		�V�K�쐬					*/
/*				2012.11.28		0.0		yamamoto	QAC�΍�						*/
/********************************************************************************/
BYTE	Tu_CheckInvalidData(BYTE *checkbuf,WORD maxfm,WORD maxam)
{
	BYTE	*pCheck;
	BYTE	i;
	WORD	freq_tst;
	
	pCheck = checkbuf;
	for(i=0;i< CTU_PRESET ;i++)
	{
		if( (WORD)((*(pCheck+1) * 0x100) + (*pCheck)) > maxam )
		{
			return FALSE;
		}
		pCheck += 2;
	}
	
	pCheck = checkbuf + 0x10;
	for(i=0;i< CTU_PRESET ;i++)
	{
		if( (WORD)((*(pCheck+1) * 0x100) + (*pCheck)) > maxfm )
		{
			return FALSE;
		}
		pCheck += 2;
	}
	
	pCheck = checkbuf + 0x20;
	for(i=0;i< CTU_PRESET;i++)
	{
		if( (WORD)((*(pCheck+1) * 0x100) + (*pCheck)) > maxfm )
		{
			return FALSE;
		}
		pCheck += 2;
	}
	return TRUE;
}

/********************************************************************************/
/*	�֐���  �FTu_ReadEepData													*/
/*	��  ��	�Fsavebuf:�ǂݍ��݃f�[�^�擪�A�h���X								*/
/*			  dest:�d����														*/
/*	�߂�l	�FTRUE:�ǂݍ��ݐ������L��/FALSE�F�ǂݍ��ݎ��s�܂��͖���			*/
/*	�@  �\	�FEEPROM�����T�ԗp�f�[�^�擾�ƗL�����`�F�b�N						*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.06		0.0			�x		�V�K�쐬					*/
/********************************************************************************/
BYTE	Tu_ReadEepData(BYTE *savebuf,BYTE dest)
{
	sEEP_PARA	tu_read;
	BYTE	ret;
	WORD	maxfreq_fm;
	WORD	maxfreq_am;
	
	ret = FALSE;
	#if 0
	if( Ma_Chk_EEP_Initial() == TRUE )		/*	EEPROM�������ς݃`�F�b�N�A���s�ꍇ�f�t�H�[���g�l���g��	*/
	#endif
	{
		if ((Ma_model_dest() >= CMA_MODEL_AMFM_J)&&(Ma_model_dest() <= CMA_MODEL_AMFM_X))
		{
			maxfreq_fm = TU_COEF_TBL[dest].fm.max_ch;
		}
		else
		{
			maxfreq_fm = 0x00;
		}
		maxfreq_am = TU_COEF_TBL[dest].am.max_ch;
#if 0
		while(!EEP_CheckIDLE())		/*	�ǂݍ��݉\���`�F�b�N	*/
		{
			;
		}
#endif
		tu_read.EepAddr	= CEEP_ADDR_TUNER;
#if 0
		tu_read.EepAddr	= 0x7A5F;
#endif
		tu_read.Length	= CEEP_LENG_TUNER;
		tu_read.BufAddr	= savebuf;
		if(EEP_ReadReq(tu_read) == TRUE)
		{
			if(Tu_CheckInvalidData(savebuf,maxfreq_fm,maxfreq_am) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return ret;
}


/********************************************************************************/
/*	�֐���  �FTu_CheckInvalidData												*/
/*	��  ��	�Fchkbuf:�`�F�b�N�p�f�[�^�擪�A�h���X								*/
/*			  destno:�d����														*/
/*	�߂�l	�FTRUE:�L���f�[�^/FALSE�F�����f�[�^									*/
/*	�@  �\	�FEEPROM����擾���������ݒ��臒l�f�[�^�L�����`�F�b�N				*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			�x		�V�K�쐬					*/
/********************************************************************************/
BYTE	Tu_CheckInvalidPara(BYTE *chkbuf,BYTE destno)
{
	BYTE	*p;
	BYTE	i;
	BYTE	checksum;
	BYTE	ret;
	BYTE	cnt;
	
	checksum = 0;
	p = chkbuf;
	ret = 0;
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16 )
	{
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_Q_INF1_CHK;
		}
	}
	if(( destno == CTU_DEST_JPN_A )
	|| ( destno == CTU_DEST_JPN_FA ))
	{
		p = (chkbuf + CTU_EEP_ADR_PARA_FMJ);
	}
	else
	{
		p = (chkbuf + CTU_EEP_ADR_PARA_FMU);
	}
	
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16)
	{
		checksum = 0;
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_PARA_FM_CHK;
		}
	}
	
	p = (chkbuf + CTU_EEP_ADR_PARA_AM);
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16)
	{
		checksum = 0;
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_PARA_AM_CHK;
		}
	}
	
	p = (chkbuf + CTU_EEP_ADR_Q_INF2);
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16)
	{
		checksum = 0;
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_Q_INF2_CHK;
		}
	}
	
	p = (chkbuf + CTU_EEP_ADR_Q_INF3);
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16)
	{
		checksum = 0;
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_Q_INF3_CHK;
		}
	}
	
	return ret;
}

/********************************************************************************/
/*	�֐���  �FTu_ReadEepPara													*/
/*	��  ��	�Freadbuf:�ǂݍ��݃f�[�^�擪�A�h���X								*/
/*			  dest:�d����														*/
/*	�߂�l	�FTRUE:�ǂݍ��ݐ������L��/FALSE�F�ǂݍ��ݎ��s�܂��͖���			*/
/*	�@  �\	�FEEPROM���珉���ݒ��臒l�f�[�^�擾�ƗL�����`�F�b�N				*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			�x		�V�K�쐬					*/
/*				2012.11.28		0.0		yamamoto	QAC�΍�						*/
/********************************************************************************/
BYTE	Tu_ReadEepPara(BYTE *readbuf,BYTE dest)
{
	sEEP_PARA	tu_read;
	BYTE	ret;
	
	ret = FALSE;
	tu_read.EepAddr	= CEEP_ADDR_ATOMIC;
	tu_read.Length	= 96;		/*	6*16	*/
	tu_read.BufAddr	= readbuf;
	if(EEP_ReadReq(tu_read) == TRUE)
	{
		ret = Tu_CheckInvalidPara(readbuf,dest);
	}
	return ret;
}
#if 0 /* ���g�p�̂��߃R�����g�A�E�g */
/********************************************************************************/
/*	�֐���  �FTu_EepCmpValid													*/
/*	��  ��	�Fbuf:�`�F�b�N�f�[�^�擪�A�h���X									*/
/*			  cmpdata:�`�F�b�N�f�[�^											*/
/*			  len:����															*/
/*	�߂�l	�FTRUE:�L��/FALSE�F����												*/
/*	�@  �\	�F�w��o�b�t�@���e�͎w��BYTE���e�Ɠ����̃f�[�^���邩�`�F�b�N		*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			�x		�V�K�쐬					*/
/********************************************************************************/
BYTE	Tu_EepCmpValid(BYTE *buf,BYTE cmpdata,BYTE len)
{
	BYTE	i;
	for( i=0;i<len;i++ )
	{
		if( *(buf+i) == cmpdata )
		{
			return FALSE;
		}
	}
	return TRUE;
}
#endif
/********************************************************************************/
/*	�֐���  �FTu_ReadEepRq														*/
/*	��  ��	�Frdbuf:�ǂݍ��݃f�[�^�擪�A�h���X									*/
/*			  offset:�I�t�Z�b�g													*/
/*			  rdlen:����														*/
/*	�߂�l	�FTRUE:�ǂݍ��ݐ���/FALSE�F�ǂݍ��ݎ��s								*/
/*	�@  �\	�FEEPROM����w��I�t�Z�b�g�ƒ����Ńf�[�^�ǂݏo������				*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			�x		�V�K�쐬					*/
/********************************************************************************/
BYTE	Tu_ReadEepRq(BYTE *rdbuf,BYTE offset,BYTE rdlen)
{
	sEEP_PARA	tu_read;
	BYTE	ret;
	
	ret = FALSE;
	tu_read.EepAddr	= (BYTE)(CTU_EEPADR_PARA + offset);
	tu_read.Length	= rdlen;
	tu_read.BufAddr	= rdbuf;
	if(EEP_CheckIDLE() == TRUE)		/*	�������݉\���`�F�b�N	*/
	{
		if(EEP_ReadReq(tu_read) == TRUE)
		{
			ret = TRUE;
		}
	}
	
	return ret;
}

/********************************************************************************/
/*	�֐���  �FTu_WriteEepRq														*/
/*	��  ��	�Fwrbuf:�������݃f�[�^�擪�A�h���X									*/
/*			  offset:�I�t�Z�b�g													*/
/*			  wrlen:����														*/
/*	�߂�l	�FTRUE:�������݉\/FALSE�F�������ݎ��s								*/
/*	�@  �\	�FEEPROM�Ɏw��I�t�Z�b�g�ƒ����Ńf�[�^�������ޏ���					*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			�x		�V�K�쐬					*/
/********************************************************************************/
BYTE	Tu_WriteEepRq(BYTE *wrbuf,BYTE offset,BYTE wrlen)
{
	sEEP_PARA	tu_write;
	BYTE	ret;
	
	ret = FALSE;
	tu_write.EepAddr	= (BYTE)(CTU_EEPADR_PARA + offset);
	tu_write.Length		= wrlen;
	tu_write.BufAddr	= wrbuf;
	if(EEP_CheckIDLE() == TRUE)		/*	�������݉\���`�F�b�N	*/
	{
		EEP_WriteReq(tu_write);
		ret = TRUE;
	}
	
	return ret;
}

/********************************************************************************/
/*	�֐���  �FTu_WriteEepData													*/
/*	��  ��	�F�Ȃ�																*/
/*	�߂�l	�F�Ȃ�																*/
/*	�@  �\	�FEEPROM�֎w����e���������ݏ���									*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			�x		�V�K�쐬					*/
/********************************************************************************/
void	Tu_WriteEepData(void)
{
	if(EEP_CheckIDLE() == TRUE)		/*	�������݉\���`�F�b�N	*/
	{
		EEP_WriteReq(tu_eepreq);
		ftu_weepreq = OFF;
	}
	else
	{
		tu_eep_cnt ++;
		if( tu_eep_cnt > 3 )
		{
			ftu_weepreq = OFF;
		}
	}
}


/********************************************************************************/
/*	�֐���  �FTu_SaveEepData													*/
/*	��  ��	�F����																*/
/*	�߂�l	�F����																*/
/*	�@  �\	�FEEPROM�փf�[�^�ۑ�����											*/
/*	���ӎ���:																	*/
/*	�C������	�y�N�����z	�yVersion�z	�y���O�z 	�y�����z					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.06		0.0			�x		�V�K�쐬					*/
/********************************************************************************/
void	Tu_SaveEepData(BYTE status,BYTE *BufAddr)
{
	BYTE	*p;
	BYTE	iLoop1,iLoop2;
	
	p = BufAddr;
	if( status == CTU_EEPWR_PST )
	{
		for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
		{
			for (iLoop2 = 1; iLoop2 < (CTU_PRESET+1); iLoop2 ++)
			{
				*p = (BYTE)(tu_mem[iLoop1].pset[iLoop2].freq & 0xFF);
				p++;
				*p = (BYTE)((tu_mem[iLoop1].pset[iLoop2].freq >> 8)&0xFF);
				p++;
			}
		}
	}
}

#undef	_TU_EEPROM_C_

#endif
#endif
