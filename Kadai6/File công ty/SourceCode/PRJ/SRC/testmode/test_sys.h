/******************************************************************************
	�N�����I���i���j�a����		

		�v���W�F�N�g��	: AMFM���W�I
		�t�@�C����		: test_sys.h
		�@�\			: test���[�h��I/F��񋟂���
------------------------------------------------------------------------------
******************************************************************************/
#include	"../model.h"

#if 	_TEST_MODE_

#ifndef	_TEST_SYS_H_
#define	_TEST_SYS_H_

#undef	EXT
#ifdef	_TEST_SYS_C_
#define	EXT
#else
#define	EXT	extern
#endif

EXT void	Test_sys_message( BYTE msg );



#endif	/*	_TEST_SYS_H_	*/

#endif	/*	_TEST_MODE_	*/
