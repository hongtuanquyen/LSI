/******************************************************************************
	�N�����I���i���j�a����		2007 C3X03 �ذ��[��]

		�v���W�F�N�g��	: '07�N�t���b�v�_�E���@MP3���f���J��
		�t�@�C����		: pw_sys.h
		�@�\			: �@�d������Ӽޭ�٥System Message�֐��Q�@ͯ�ް�̧��
------------------------------------------------------------------------------
�C������
�y�ԍ��z	���t		�S����	�C�����e
�yNKC-0000�z'05/12/09	���n	�V�K�쐬
******************************************************************************/
#ifndef	_PW_SYS_H_
#define	_PW_SYS_H_

#undef	EXT
#ifdef	_PW_SYS_C_
#define	EXT   
#else
#define	EXT	extern
#endif

EXT	void	Pw_sys_message(BYTE );
EXT	void	Pw_initial(void);

#endif
