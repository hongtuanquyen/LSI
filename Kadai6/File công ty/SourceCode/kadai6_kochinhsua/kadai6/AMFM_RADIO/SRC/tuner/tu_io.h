#ifndef		_TU_IO_H_
#define		_TU_IO_H_

#undef	EXT
#ifdef	_TU_IO_C_
#define	EXT   
#else
#define	EXT	extern
#endif

#include		"tu_def.h"

/*----------------------------------------------------*/
/*   関数プロトタイプ								  */
/*----------------------------------------------------*/
EXT	BYTE	Tu_data_write(void);
EXT	BYTE	Tu_data_read(void);

/*----------------------------------------------------*/
/*   定数 定義										  */
/*----------------------------------------------------*/
#define	TTU_WRITE_SLAVE		0xC0
#define	TTU_READ_SLAVE		0xC1
/*----------------------------------------------------*/
/*   変数 定義										  */
/*----------------------------------------------------*/
EXT	TTU_I2C	tu_ic_data;		/* 通信基本情報エリア */

#endif
