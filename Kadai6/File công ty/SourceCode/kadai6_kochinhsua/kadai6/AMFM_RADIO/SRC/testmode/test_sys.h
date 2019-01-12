/******************************************************************************
	クラリオン（株）殿向け		

		プロジェクト名	: AMFMラジオ
		ファイル名		: test_sys.h
		機能			: testモードのI/Fを提供する
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
