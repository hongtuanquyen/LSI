/*************************************************************************
	�N�����I���a���� 
	�v���W�F�N�g��	�F2012�NAMFM_RADIO����
	�t�@�C����		�Fsbm_sys.h
	���W���[����	�FSUBMODE
	�@   �\			�FSub system���b�Z�[�W��t����
--------------------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
--------------------------------------------------------------------------
			2012/7/23	0.1			lrj		�V�K
*************************************************************************/
#ifndef		_SBM_SYS_H_
#define		_SBM_SYS_H_

#include	"../model.h"

#undef	EXT
#ifdef	_SBM_SYS_C_
#define	EXT   
#else
#define	EXT	extern
#endif

EXT	void Sbm_sys_message( BYTE );
EXT void Sbm_100ms_cnt(void);
EXT void Sbm_msg_modechg(void);

#endif
