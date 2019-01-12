/******************************************************************************
	�N�����I���i���j�a����		2007 C3X03 �ذ��[��]

		�v���W�F�N�g��	: '07�N�t���b�v�_�E���@MP3���f���J��
		�t�@�C����		: pw_func_ext.h
		�@�\			: Power Module �O���Q�Ɗ֐��Q�֐��pͯ�ް�̧��
------------------------------------------------------------------------------
�C������
�y�ԍ��z	���t		�S����	�C�����e
�yNKC-0000�z'05/12/08	���n	�V�K�쐬
******************************************************************************/
#ifndef	_PW_FUNC_H_
#define	_PW_FUNC_H_

#undef	EXT
#ifdef	_PW_FUNC_C_
#define	EXT
#else
#define	EXT	extern
#endif

#include	"../power/pw_def.h"

/*----------------------------------------------------
   �֐����ĺ��
----------------------------------------------------*/
/*	Power Module 1ms ������ُ����֐��F1ms�ް����ϰ���ď���					*/
EXT	void	Pw_1ms_timer(void);

/*	Power Module 2ms ������ُ����֐��F1ms�ް����ϰ���ď���					*/
EXT	void	Pw_2ms_timer(void);

/*	Power Module 4ms ������ُ����֐��F1ms�ް����ϰ���ď���					*/
EXT	void	Pw_4ms_timer(void);

/*	Power Module 5ms ������ُ����֐��F5ms�ް����ϰ���ď���				*/
EXT	void	Pw_5ms_timer(void);

/*	Power Module 10ms ������ُ����֐��F10ms�ް����ϰ���ď���				*/
EXT	void	Pw_10ms_timer(void);

/* AccOFF�� 62.5ms�C���^�[�o�������֐� */
EXT	void	Pw_62ms_cycle(void);

/*	ACC�X�e�[�^�X ��Ԏ擾�֐��F�d������X�e�[�^�X��Ԃ��B		*/
EXT	BYTE	Pw_ACC_status_get(void);

/*	ACC Port ��Ԏ擾�֐��FACC�߰Ă��Ď���Hi/Low����ݸތ�̏�Ԃ�Ԃ��B		*/
EXT	BYTE	Pw_ACC_DET_get(void);

/*	Power on/off���Ď�����Ԃ�Ԃ��B				*/
EXT	BYTE	Pw_Power_status_get(void);

/*	Power Key��Ԃ��擾����		 */
EXT BYTE	Pw_Power_Key_get(void);

/*	PowerOff�����ǂ����̕\���`�F�b�N	*/
EXT BYTE	Pw_PowerOff_disp_chk(void);

EXT	void 	Pw_power_key(void);
EXT	void 	Pw_power_event(BYTE, BYTE);

/* HoldX��Ԏ��A���荞�ݔ����ɂ��AAccOFF�ʏ��Ԃɂ������̊��荞�ݏ��� */
EXT void	Pw_acc_off_nor_req(BYTE mode);

EXT	BOOL	Pw_ret_illdet(void);
EXT	BOOL	Pw_ret_almkey(void);
EXT	void	Pw_almkey_clear(void);

/* AccOFF���A�u�A���[�����������v OR �u�A���[���������[�h�ɓ���v�̏ꍇ�APower�֒m�点 */
EXT	void	Pw_acc_wakeup_req(BYTE onoff);

/* �e�X�g���[�h�@�\ */
EXT	void	Pw_test_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length);

#if _POWER_DIAG_
/* DIAG���o�@�\ */
EXT	void Pw_DIAG_check(BYTE mode);
EXT	void Pw_DIAG_clear( void );
#endif	/* _POWER_DIAG_ */

/*----------------------------------------------------
   �׸� ��`
----------------------------------------------------*/
/*	Power Module Flag Memory area					*/
/*	EXT	_bram	BYTE	Power_flag;					*/
/*	EXT	_bit	fpw_yobi8	_atbit(key_flag, 7);	*/
/*--------------------------------------------------*/
/*	�����A�N�Z�X�ϐ���`							*/
/*--------------------------------------------------*/
/*EXT	_near	BYTE	pw_speed_input;				*/
/*--------------------------------------------------*/
/*	�ϐ���`										*/
/*--------------------------------------------------*/
/*EXT	BYTE	pw_data_aria;						*/
/*--------------------------------------------------*/

#endif
