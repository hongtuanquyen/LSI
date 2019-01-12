/***********************************************************************
	�N�����I���a���� 
	�v���W�F�N�g��	�FAMFM���W�I
	�t�@�C����		�Ftest_ext.c
	���W���[����	�F�e�X�g���[�h
	�@   �\			�FPC�Ƃ̒ʐM����
------------------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------
[chin20120931]	�V�K
***********************************************************************/
#include	"../model.h"

#if 	_TEST_MODE_


#define		_TEST_EXT_C_


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


/******************************************************************************
		�֐���  �FTest_data_reply
		��  ��	�Fcmd�˃R�}���h
				�Fpara�˃p�����[�^
				�Fadr�˕ԐM�o�b�t�@�̃A�h���X
				�Flen�˕ԐM�f�[�^�̃����O�X
		�߂�l	�F	����
		�@  �\	�F�e���W���[�����ԐM�f�[�^��Test���[�h�ɓn��
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[chin20120930]
******************************************************************************/
void Test_data_reply(BYTE cmd,BYTE para,BYTE *adr,BYTE len)
{
	memset( TestReplyBuf,0x00,sizeof(TestReplyBuf));
	ftest_result = OK;
	TestReplyCmd = cmd;
	TestReplyPara = para;
	TestReplyLen = len;
	if(len <= CTEST_REPLY_MAX)
	{
		memcpy( TestReplyBuf,adr,len);
	}
}

#undef		_TEST_EXT_C_

#endif	/* _TEST_MODE_ */
