/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: common.h
		機能			: System共通ヘッダﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]	新規作成
[sun051128]	モデル定義追加
[isi060803 2行]	iPod 2行表示対応
[isi060810 USB]	USB Text Buffer追加
[so060814 SIRIUS]変数定義追加、構造体内容を追加
[tog070316 USB]	USB Text Bufferを各Name毎の定義に変更
******************************************************************************/
#include	"../model.h"

#ifndef		_COMMON_H_
#define		_COMMON_H_

/*----------------------------------------------------
   型定義
----------------------------------------------------*/
typedef	unsigned char	BYTE;
typedef	unsigned short	WORD;
typedef	unsigned long	LONG;
typedef	unsigned char	BOOL;
typedef	unsigned char	UCHAR;
typedef unsigned long	DWORD;

typedef	signed char		sBYTE;
typedef	signed short	sWORD;
typedef	signed long		sLONG;


typedef	union
{	
	BYTE	byte;
	struct
	{
		BYTE	bit0:1;
		BYTE	bit1:1;
		BYTE	bit2:1;
		BYTE	bit3:1;
		BYTE	bit4:1;
		BYTE	bit5:1;
		BYTE	bit6:1;
		BYTE	bit7:1;
	}bit;
}TCOM_FLG;

/*----------------------------------------------------
   共通定義
----------------------------------------------------*/
#define		TRUE	1
#define		FALSE	0

#define		ON		1
#define		OFF		0

#define		HIGH	1
#define		LOW		0

#define		YES		1
#define		NO		0

#define		OK		1
#define		NG		0

#define		OPEN	1
#define		CLOSE	0

#define		IN		0
#define		OUT		1

#define		EI		1
#define		DI		0

#define		UP		1
#define		DW		0

#define		TIMER_OUT	0

#define		CHANGE		2

/*----------------------------------------------------
   Systemﾒｯｾｰｼﾞ定義
----------------------------------------------------*/
enum
{
	CMSG_MAIN,
	CMSG_INITIAL,
	CMSG_MAINCNT_ON,
	CMSG_PWRON,
	CMSG_MODECHG,
	CMSG_PWROFF,
	CMSG_MAINCNT_OFFREQ,
	CMSG_MAINCNT_OFF,
	CMSG_HOLDX_IN,
	CMSG_HOLDX_OUT,

	CMSG_10MS_TIMER,
	CMSG_25MS_TIMER,
	CMSG_50MS_TIMER,
	CMSG_100MS_TIMER
};
/*----------------------------------------------------
   BIT位置定義
----------------------------------------------------*/
#if 0
#define     BIT0        (BYTE)(0x01)
#define     BIT1        (BYTE)(0x01) << 1
#define     BIT2        (BYTE)(0x01) << 2
#define     BIT3        (BYTE)(0x01) << 3
#define     BIT4        (BYTE)(0x01) << 4
#define     BIT5        (BYTE)(0x01) << 5
#define     BIT6        (BYTE)(0x01) << 6
#define     BIT7        (BYTE)(0x01) << 7
#endif
/*----------------------------------------------------
   ２進表記のためのマクロ定義
----------------------------------------------------*/
#define     BIN_00000000    (BYTE)0x00
#define     BIN_00000001    (BYTE)0x01
#define     BIN_00000010    (BYTE)0x02
#define     BIN_00000011    (BYTE)0x03
#define     BIN_00000100    (BYTE)0x04
#define     BIN_00000101    (BYTE)0x05
#define     BIN_00000110    (BYTE)0x06
#define     BIN_00000111    (BYTE)0x07
#define     BIN_00001000    (BYTE)0x08
#define     BIN_00001001    (BYTE)0x09
#define     BIN_00001010    (BYTE)0x0a
#define     BIN_00001011    (BYTE)0x0b
#define     BIN_00001100    (BYTE)0x0c
#define     BIN_00001101    (BYTE)0x0d
#define     BIN_00001110    (BYTE)0x0e
#define     BIN_00001111    (BYTE)0x0f

#define     BIN_00010000    (BYTE)0x10
#define     BIN_00010001    (BYTE)0x11
#define     BIN_00010010    (BYTE)0x12
#define     BIN_00010011    (BYTE)0x13
#define     BIN_00010100    (BYTE)0x14
#define     BIN_00010101    (BYTE)0x15
#define     BIN_00010110    (BYTE)0x16
#define     BIN_00010111    (BYTE)0x17
#define     BIN_00011000    (BYTE)0x18
#define     BIN_00011001    (BYTE)0x19
#define     BIN_00011010    (BYTE)0x1a
#define     BIN_00011011    (BYTE)0x1b
#define     BIN_00011100    (BYTE)0x1c
#define     BIN_00011101    (BYTE)0x1d
#define     BIN_00011110    (BYTE)0x1e
#define     BIN_00011111    (BYTE)0x1f

#define     BIN_00100000    (BYTE)0x20
#define     BIN_00100001    (BYTE)0x21
#define     BIN_00100010    (BYTE)0x22
#define     BIN_00100011    (BYTE)0x23
#define     BIN_00100100    (BYTE)0x24
#define     BIN_00100101    (BYTE)0x25
#define     BIN_00100110    (BYTE)0x26
#define     BIN_00100111    (BYTE)0x27
#define     BIN_00101000    (BYTE)0x28
#define     BIN_00101001    (BYTE)0x29
#define     BIN_00101010    (BYTE)0x2a
#define     BIN_00101011    (BYTE)0x2b
#define     BIN_00101100    (BYTE)0x2c
#define     BIN_00101101    (BYTE)0x2d
#define     BIN_00101110    (BYTE)0x2e
#define     BIN_00101111    (BYTE)0x2f

#define     BIN_00110000    (BYTE)0x30
#define     BIN_00110001    (BYTE)0x31
#define     BIN_00110010    (BYTE)0x32
#define     BIN_00110011    (BYTE)0x33
#define     BIN_00110100    (BYTE)0x34
#define     BIN_00110101    (BYTE)0x35
#define     BIN_00110110    (BYTE)0x36
#define     BIN_00110111    (BYTE)0x37
#define     BIN_00111000    (BYTE)0x38
#define     BIN_00111001    (BYTE)0x39
#define     BIN_00111010    (BYTE)0x3a
#define     BIN_00111011    (BYTE)0x3b
#define     BIN_00111100    (BYTE)0x3c
#define     BIN_00111101    (BYTE)0x3d
#define     BIN_00111110    (BYTE)0x3e
#define     BIN_00111111    (BYTE)0x3f

#define     BIN_01000000    (BYTE)0x40
#define     BIN_01000001    (BYTE)0x41
#define     BIN_01000010    (BYTE)0x42
#define     BIN_01000011    (BYTE)0x43
#define     BIN_01000100    (BYTE)0x44
#define     BIN_01000101    (BYTE)0x45
#define     BIN_01000110    (BYTE)0x46
#define     BIN_01000111    (BYTE)0x47
#define     BIN_01001000    (BYTE)0x48
#define     BIN_01001001    (BYTE)0x49
#define     BIN_01001010    (BYTE)0x4a
#define     BIN_01001011    (BYTE)0x4b
#define     BIN_01001100    (BYTE)0x4c
#define     BIN_01001101    (BYTE)0x4d
#define     BIN_01001110    (BYTE)0x4e
#define     BIN_01001111    (BYTE)0x4f

#define     BIN_01010000    (BYTE)0x50
#define     BIN_01010001    (BYTE)0x51
#define     BIN_01010010    (BYTE)0x52
#define     BIN_01010011    (BYTE)0x53
#define     BIN_01010100    (BYTE)0x54
#define     BIN_01010101    (BYTE)0x55
#define     BIN_01010110    (BYTE)0x56
#define     BIN_01010111    (BYTE)0x57
#define     BIN_01011000    (BYTE)0x58
#define     BIN_01011001    (BYTE)0x59
#define     BIN_01011010    (BYTE)0x5a
#define     BIN_01011011    (BYTE)0x5b
#define     BIN_01011100    (BYTE)0x5c
#define     BIN_01011101    (BYTE)0x5d
#define     BIN_01011110    (BYTE)0x5e
#define     BIN_01011111    (BYTE)0x5f

#define     BIN_01100000    (BYTE)0x60
#define     BIN_01100001    (BYTE)0x61
#define     BIN_01100010    (BYTE)0x62
#define     BIN_01100011    (BYTE)0x63
#define     BIN_01100100    (BYTE)0x64
#define     BIN_01100101    (BYTE)0x65
#define     BIN_01100110    (BYTE)0x66
#define     BIN_01100111    (BYTE)0x67
#define     BIN_01101000    (BYTE)0x68
#define     BIN_01101001    (BYTE)0x69
#define     BIN_01101010    (BYTE)0x6a
#define     BIN_01101011    (BYTE)0x6b
#define     BIN_01101100    (BYTE)0x6c
#define     BIN_01101101    (BYTE)0x6d
#define     BIN_01101110    (BYTE)0x6e
#define     BIN_01101111    (BYTE)0x6f

#define     BIN_01110000    (BYTE)0x70
#define     BIN_01110001    (BYTE)0x71
#define     BIN_01110010    (BYTE)0x72
#define     BIN_01110011    (BYTE)0x73
#define     BIN_01110100    (BYTE)0x74
#define     BIN_01110101    (BYTE)0x75
#define     BIN_01110110    (BYTE)0x76
#define     BIN_01110111    (BYTE)0x77
#define     BIN_01111000    (BYTE)0x78
#define     BIN_01111001    (BYTE)0x79
#define     BIN_01111010    (BYTE)0x7a
#define     BIN_01111011    (BYTE)0x7b
#define     BIN_01111100    (BYTE)0x7c
#define     BIN_01111101    (BYTE)0x7d
#define     BIN_01111110    (BYTE)0x7e
#define     BIN_01111111    (BYTE)0x7f

#define     BIN_10000000    (BYTE)0x80
#define     BIN_10000001    (BYTE)0x81
#define     BIN_10000010    (BYTE)0x82
#define     BIN_10000011    (BYTE)0x83
#define     BIN_10000100    (BYTE)0x84
#define     BIN_10000101    (BYTE)0x85
#define     BIN_10000110    (BYTE)0x86
#define     BIN_10000111    (BYTE)0x87
#define     BIN_10001000    (BYTE)0x88
#define     BIN_10001001    (BYTE)0x89
#define     BIN_10001010    (BYTE)0x8a
#define     BIN_10001011    (BYTE)0x8b
#define     BIN_10001100    (BYTE)0x8c
#define     BIN_10001101    (BYTE)0x8d
#define     BIN_10001110    (BYTE)0x8e
#define     BIN_10001111    (BYTE)0x8f

#define     BIN_10010000    (BYTE)0x90
#define     BIN_10010001    (BYTE)0x91
#define     BIN_10010010    (BYTE)0x92
#define     BIN_10010011    (BYTE)0x93
#define     BIN_10010100    (BYTE)0x94
#define     BIN_10010101    (BYTE)0x95
#define     BIN_10010110    (BYTE)0x96
#define     BIN_10010111    (BYTE)0x97
#define     BIN_10011000    (BYTE)0x98
#define     BIN_10011001    (BYTE)0x99
#define     BIN_10011010    (BYTE)0x9a
#define     BIN_10011011    (BYTE)0x9b
#define     BIN_10011100    (BYTE)0x9c
#define     BIN_10011101    (BYTE)0x9d
#define     BIN_10011110    (BYTE)0x9e
#define     BIN_10011111    (BYTE)0x9f

#define     BIN_10100000    (BYTE)0xa0
#define     BIN_10100001    (BYTE)0xa1
#define     BIN_10100010    (BYTE)0xa2
#define     BIN_10100011    (BYTE)0xa3
#define     BIN_10100100    (BYTE)0xa4
#define     BIN_10100101    (BYTE)0xa5
#define     BIN_10100110    (BYTE)0xa6
#define     BIN_10100111    (BYTE)0xa7
#define     BIN_10101000    (BYTE)0xa8
#define     BIN_10101001    (BYTE)0xa9
#define     BIN_10101010    (BYTE)0xaa
#define     BIN_10101011    (BYTE)0xab
#define     BIN_10101100    (BYTE)0xac
#define     BIN_10101101    (BYTE)0xad
#define     BIN_10101110    (BYTE)0xae
#define     BIN_10101111    (BYTE)0xaf

#define     BIN_10110000    (BYTE)0xb0
#define     BIN_10110001    (BYTE)0xb1
#define     BIN_10110010    (BYTE)0xb2
#define     BIN_10110011    (BYTE)0xb3
#define     BIN_10110100    (BYTE)0xb4
#define     BIN_10110101    (BYTE)0xb5
#define     BIN_10110110    (BYTE)0xb6
#define     BIN_10110111    (BYTE)0xb7
#define     BIN_10111000    (BYTE)0xb8
#define     BIN_10111001    (BYTE)0xb9
#define     BIN_10111010    (BYTE)0xba
#define     BIN_10111011    (BYTE)0xbb
#define     BIN_10111100    (BYTE)0xbc
#define     BIN_10111101    (BYTE)0xbd
#define     BIN_10111110    (BYTE)0xbe
#define     BIN_10111111    (BYTE)0xbf

#define     BIN_11000000    (BYTE)0xc0
#define     BIN_11000001    (BYTE)0xc1
#define     BIN_11000010    (BYTE)0xc2
#define     BIN_11000011    (BYTE)0xc3
#define     BIN_11000100    (BYTE)0xc4
#define     BIN_11000101    (BYTE)0xc5
#define     BIN_11000110    (BYTE)0xc6
#define     BIN_11000111    (BYTE)0xc7
#define     BIN_11001000    (BYTE)0xc8
#define     BIN_11001001    (BYTE)0xc9
#define     BIN_11001010    (BYTE)0xca
#define     BIN_11001011    (BYTE)0xcb
#define     BIN_11001100    (BYTE)0xcc
#define     BIN_11001101    (BYTE)0xcd
#define     BIN_11001110    (BYTE)0xce
#define     BIN_11001111    (BYTE)0xcf

#define     BIN_11010000    (BYTE)0xd0
#define     BIN_11010001    (BYTE)0xd1
#define     BIN_11010010    (BYTE)0xd2
#define     BIN_11010011    (BYTE)0xd3
#define     BIN_11010100    (BYTE)0xd4
#define     BIN_11010101    (BYTE)0xd5
#define     BIN_11010110    (BYTE)0xd6
#define     BIN_11010111    (BYTE)0xd7
#define     BIN_11011000    (BYTE)0xd8
#define     BIN_11011001    (BYTE)0xd9
#define     BIN_11011010    (BYTE)0xda
#define     BIN_11011011    (BYTE)0xdb
#define     BIN_11011100    (BYTE)0xdc
#define     BIN_11011101    (BYTE)0xdd
#define     BIN_11011110    (BYTE)0xde
#define     BIN_11011111    (BYTE)0xdf

#define     BIN_11100000    (BYTE)0xe0
#define     BIN_11100001    (BYTE)0xe1
#define     BIN_11100010    (BYTE)0xe2
#define     BIN_11100011    (BYTE)0xe3
#define     BIN_11100100    (BYTE)0xe4
#define     BIN_11100101    (BYTE)0xe5
#define     BIN_11100110    (BYTE)0xe6
#define     BIN_11100111    (BYTE)0xe7
#define     BIN_11101000    (BYTE)0xe8
#define     BIN_11101001    (BYTE)0xe9
#define     BIN_11101010    (BYTE)0xea
#define     BIN_11101011    (BYTE)0xeb
#define     BIN_11101100    (BYTE)0xec
#define     BIN_11101101    (BYTE)0xed
#define     BIN_11101110    (BYTE)0xee
#define     BIN_11101111    (BYTE)0xef

#define     BIN_11110000    (BYTE)0xf0
#define     BIN_11110001    (BYTE)0xf1
#define     BIN_11110010    (BYTE)0xf2
#define     BIN_11110011    (BYTE)0xf3
#define     BIN_11110100    (BYTE)0xf4
#define     BIN_11110101    (BYTE)0xf5
#define     BIN_11110110    (BYTE)0xf6
#define     BIN_11110111    (BYTE)0xf7
#define     BIN_11111000    (BYTE)0xf8
#define     BIN_11111001    (BYTE)0xf9
#define     BIN_11111010    (BYTE)0xfa
#define     BIN_11111011    (BYTE)0xfb
#define     BIN_11111100    (BYTE)0xfc
#define     BIN_11111101    (BYTE)0xfd
#define     BIN_11111110    (BYTE)0xfe
#define     BIN_11111111    (BYTE)0xff

/*----------------------------------------------------
   共通マクロ定義
----------------------------------------------------*/
#define 	MP_X(para1,para2)	((para1) = (para2))


/*----------------------------------------------------
   関数ﾌﾟﾛﾄﾀｲﾌﾟ
----------------------------------------------------*/
#ifdef	_COMMON_C_
void Cm_wait_loop( BYTE cnt  );
BYTE Cm_lib_wHEXtoBCD( WORD hex, BYTE * bcd  );
WORD Cm_lib_bHEXtoBCD( BYTE hex );
WORD Utf8encode_Iso8859( BYTE *str, BYTE *buff, WORD len );
WORD Base64encode( BYTE *str, BYTE *buff, WORD len );
void romcopy(BYTE *, BYTE *, BYTE );
BOOL romcmp(BYTE *, BYTE *, BYTE );
WORD wBcd16ToHex16( WORD bcd );
void wMemClear( BYTE* ps, WORD cnt );
void wMemCopy( BYTE *ps, BYTE *pd, WORD cnt );
void wMemSet( BYTE *ps , BYTE val, WORD cnt );
BOOL wMemCmp( BYTE* ps, BYTE* pd, WORD cnt );
WORD Cm_bcd_add_w( WORD bcd1, WORD bcd2  );
WORD Cm_bcd_sub_w( WORD bcd1, WORD bcd2  );
#else
extern	void Cm_wait_loop( BYTE cnt  );
extern	BYTE Cm_lib_wHEXtoBCD( WORD hex, BYTE * bcd  );
extern	WORD Cm_lib_bHEXtoBCD( BYTE hex );
extern	WORD Utf8encode_Iso8859( BYTE *str, BYTE *buff, WORD len );
extern	WORD Base64encode( BYTE *str, BYTE *buff, WORD len );
extern	void romcopy(BYTE *, BYTE *, BYTE );
extern  BOOL romcmp(BYTE *, BYTE *, BYTE );
extern  WORD wBcd16ToHex16( WORD bcd );
extern void wMemClear( BYTE* ps, WORD cnt );
extern void wMemCopy( BYTE *ps, BYTE *pd, WORD cnt );
extern void wMemSet( BYTE *ps , BYTE val, WORD cnt );
extern BOOL wMemCmp( BYTE* ps, BYTE* pd, WORD cnt );
extern WORD Cm_bcd_add_w( WORD bcd1, WORD bcd2  );
extern WORD Cm_bcd_sub_w( WORD bcd1, WORD bcd2  );
#endif


#endif
