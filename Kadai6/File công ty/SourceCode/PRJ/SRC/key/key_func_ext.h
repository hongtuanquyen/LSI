/******************************************************************************
	�N�����I���a����
	�v���W�F�N�g��	: 2012�NAMFM_RADIO���f��
	�t�@�C����		: key_func_ext.h
	�@�\			: Key Module �O���Q�Ɗ֐��Q�֐��p�w�b�_�[�t�@�C��
------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
******************************************************************************/
#ifndef	_KEY_FUNC_H_
#define	_KEY_FUNC_H_

#undef	EXT
#ifdef	_KEY_FUNC_C_
#define	EXT
#else
#define	EXT	extern
#endif

/*----------------------------------------------------
   �֐�
----------------------------------------------------*/
EXT	void	Key_1ms_timer(void);
EXT	void	Key_5ms_timer(void);
EXT	void	Key_check_stop(void);
EXT	void	Key_check_start(void);
EXT	void	Key_long_time200(void);
EXT	void	Key_long_time500(void);
EXT	void	Key_long_time1000(void);
EXT	void	Key_long_time2000(void);
EXT	void	Key_nop(void);
EXT	BYTE	Key_get_priokey(void);		
EXT void	Key_testmode_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length);

EXT	void	Key_10ms_timer_accoff(void);
EXT	void	Key_long_time2000_accoff(void);

#endif