/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: tu_cont_tbl.c
		�@�\			: �@TunerӼޭ�ٺ��۰�ð���̧��
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051213]	�V�K�쐬
[sun061003]�װѻ���ŋǌ��t��Ȃ��ƁA�p������悤�ɕύX
******************************************************************************/
#ifndef		_TU_CONT_H_
#define		_TU_CONT_H_

#include	"tu_item.h"

#ifdef	_TU_ITEM_C_

const	TTU_CONTTBL  tu_m_pwron[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute */
	{	Tu_I_icreset,		2,			2,			},		/* 1:IC ������ */
	{	Tu_I_bndpara,		3,			3,			},		/* 2:BAND�Q���ݒ� */
	{	Tu_I_frqset,		4,			4,			},		/* 3:���g���ݒ� */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 4:AFTERMUTE�ݒ� */
};

const	TTU_CONTTBL  tu_m_lastcal[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:Premute */
	{	Tu_I_frqset,		2,			2,			},		/* 1:���g���ݒ� */
	{	Tu_I_pstchk,		3,			3,			},		/* 2:Preset�ݒ� */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 3:AFTERMUTE�ݒ� */
};

const	TTU_CONTTBL  tu_m_bandchg[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute */
	{	Tu_I_bndpara,		2,			2,			},		/* 1:BAND�Q���ݒ� */
	{	Tu_I_frqset,		3,			3,			},		/* 2:���g���ݒ� */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 3:AFTERMUTE�ݒ� */
};

const	TTU_CONTTBL  tu_m_man[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0: Premute */
	{	Tu_I_updw,			2,			2,			},		/* 1:���g��UP/DOWN1Step */
	{	Tu_I_frqset,		3,			3,			},		/* 2:���g���ݒ� */
	{	Tu_I_man,			4,			4,			},		/* 3:�L�[Off�҂����� */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 4:AFTERMUTE�ݒ� */
};
const	TTU_CONTTBL  tu_m_cman[] =
{
	{	Tu_I_updw,			1,			1,			},		/* 0:���g��UP/DOWN1Step */
	{	Tu_I_frqset,		2,			2,			},		/* 1:���g���ݒ� */
	{	Tu_I_cwait,			0,			0			}		/* 2:�Ԋu�^�C�}�҂� */
};
const	TTU_CONTTBL  tu_m_seek[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute  */
	{	Tu_I_seekst,		2,			2,			},		/* 1:seek start */
	{	Tu_I_updw,			3,			3,			},		/* 2:Step�����޳� */
	{	Tu_I_frqset,		4,			4,			},		/* 3:���g���ݒ� */
	{	Tu_I_qinfochk,		5,			2,			},		/* 4:SeeK��~���� */
	{	Tu_I_seekend,		6,			6,			},		/* 5:Seek�I�� */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 6:AFTERMUTE�ݒ� */
};
const	TTU_CONTTBL  tu_m_pstrd[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:Premute */
	{	Tu_I_frqset,		2,			2,			},		/* 1:���g���ݒ� */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 2:AFTERMUTE�ݒ� */
};

const	TTU_CONTTBL  tu_m_pstwt[] =
{
	{	Tu_I_pstwait,		1,			CTU_NGEND,	},		/* 0:�v���Z�b�g�������ݑ҂�	 */
	{	Tu_I_pstwrite,		CTU_OKEND,	CTU_NGEND	}		/* 1:�v���Z�b�g��������		 */
};

/* [sun061004]�P��ýĊ�[0] */
/* [sun061120]PreMute��ɂ�����悤�ɏC�� */
const	TTU_CONTTBL  tu_m_as[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute */
	{	Tu_I_apmst,			2,			2,			},		/* 1:Apm start*/
	{	Tu_I_frqset,		3,			3,			},		/* 2:���g���ݒ�*/
	{	Tu_I_qinfochk,		4,			5,			},		/* 3:SeeK��~���� */
	{	Tu_I_apmset,		5,			5,			},		/* 4:�L�ǂ��������� */
	{	Tu_I_updw,			6,			6,			},		/* 5:Step�����޳�*/
	{	Tu_I_apmendchk,		7,			2,			},		/* 6:APM�I���`�F�b�N*/
	{	Tu_I_apmend,		CTU_NEXT_M,	CTU_NEXT_M	}		/* 7:Apm end*/
};
/*[zhang070313] clarion �Ή�*/		/*	Preset Scan		*/
const		TTU_CONTTBL	tu_m_scan[] =
{
	{	Tu_I_premute,		1,			1,			},		/* 0:PreMute  */
	{	Tu_I_seekst,		2,			2,			},		/* 1:seek start */
	{	Tu_I_updw,			3,			3,			},		/* 2:Step�����޳� */
	{	Tu_I_frqset,		4,			4,			},		/* 3:���g���ݒ� */
	{	Tu_I_qinfochk,		5,			2,			},		/* 4:SeeK��~���� */
	{	Tu_I_scanwait,		0,			0			}		/* 5:��M��҂����� */
};

/* [zhang070312] ISR READ ��ISR WRITE��ǉ��A�D����Freq(�����)�R�[��*/
const	TTU_CONTTBL  tu_m_isrrd[] =	
{
	{	Tu_I_premute,		1,			1,			},		/* 0: */
	{	Tu_I_isrread,		2,			2,			},		/* 1: */
	{	Tu_I_bndpara,		3,			3,			},		/* 2:BAND�Q���ݒ� */
	{	Tu_I_frqset,		4,			4,			},		/* 3:���g���ݒ� */
	{	Tu_I_aftermute,		CTU_OKEND,	CTU_NGEND	}		/* 4:AFTERMUTE�ݒ� */
};

const	TTU_CONTTBL	* const TU_SeqTBL[] =
{
	tu_m_pwron,
	tu_m_lastcal,
	tu_m_bandchg,
	tu_m_man,
	tu_m_cman,
	tu_m_seek,
	tu_m_pstrd,
	tu_m_pstwt,
	tu_m_as,
	tu_m_scan,
	tu_m_isrrd,
};
#endif

#endif
