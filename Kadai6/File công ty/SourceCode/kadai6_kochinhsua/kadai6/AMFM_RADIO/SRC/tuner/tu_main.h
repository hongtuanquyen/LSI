/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: tu_main.h
		�@�\			: �@Tuner MainӼޭ�كw�b�_̧��
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051125]	�V�K�쐬
[sun060915] SD臒lýĔŎd�l�����ύX�Ή�
[zhang070313] clarion �Ή��C��
[matu120903] S-meter���� Local�l�ǉ� 
[yamamoto120905] ���Ʊ�n���AM����ގ��g����M�͈͏���C��
[matu120926] �e�X�g���[�h�Ή�
[yamamoto130305] ST�`���^�����O���ԕύX
******************************************************************************/
#ifndef	_TU_MAIN_H_
#define	_TU_MAIN_H_

#include	"../comm/common.h"
#include	"tu_def.h"

#undef	EXT
#ifdef	_TU_MAIN_C_
#define	EXT
#else
#define	EXT	extern
#endif

/*----------------------------------------------------
   �֐����ĺ��
----------------------------------------------------*/
EXT	void	Tu_main(void);
EXT	void	Tu_initial(void);
EXT	void	Tu_aftermuteset(void);			/* AfterMute ��ď��� */
/*----------------------------------------------------
   �׸� ��`
   [zhang070308]CLARION�d�l�ύX�Ή�
----------------------------------------------------*/
EXT	TCOM_FLG	tu_flag;		/*Tuner*/
#define		ftu_mute		tu_flag.bit.bit7		/* Tuner Mute Request  1:mute on  0:mute off */
#define		ftu_write		tu_flag.bit.bit6		/* Tuner �������ݗv�� 1: on 0: off */
#define		ftu_updw		tu_flag.bit.bit5		/* Up/DownFlag  1: Up 0�FDown */
#define		ftu_stero		tu_flag.bit.bit4		/* ��ڵ�m��Flag  1: Stero On  0: Stero Off */
#define		ftu_stero_b		tu_flag.bit.bit3		/* ��ڵ����ݸޗp�ޯ�Flag */
#define		ftu_lw			tu_flag.bit.bit2		/* LW Band Flag 1: LW  0: MW */
#define		ftu_isr			tu_flag.bit.bit1		/* ISR ���[�hON�F1/OFF:0	Tuner/HD Radio��ISR�𓝈�Ǘ����邽�߁A
														ISR��Ɨ����[�h�Ƃ��Ēǉ��A100809�A�x			*/
#define		ftu_ifcnt		tu_flag.bit.bit0		/* Tuner IF ���Ē��׸� 	1: ���Ē�  0:���ďI�� */

EXT	TCOM_FLG	tu_flag1;		/*Tuner*/
#define		ftu_stchk		tu_flag1.bit.bit7		/* Stero����ݸ�����10ms�׸�  1: 10ms������ݸ�  0: 10ms�o�߂��Ă��Ȃ� */
#define		ftu_seek		tu_flag1.bit.bit6		/* Tuner Seek On Flag 1: off 0: on */
#define		ftu_sttest		tu_flag1.bit.bit5		/* Tuner ST�\�� Flag 1: ST�\���� 0: ���\���� */
#if	(_MODEL_EEPROM_ == 1)
#define		ftu_weepreq		tu_flag1.bit.bit4		/* Tuner Data Save Request	100811�A�x	*/
#endif

/*----------------------------------------------------
   �����A�N�Z�X�ϐ���`
----------------------------------------------------*/
EXT	BYTE	tu_m_mode;		/* Tuner Main Mode */
EXT	BYTE	tu_m_seq;		/* Tuner Main sequence */
EXT	BYTE	tu_i_seq;		/* Tuner Item sequence */
EXT	BYTE	tu_aftermute;	/* Tuner AfterMute��� [10ms Base] */
EXT	BYTE	tu_bandno;		/* band��� */
EXT	BYTE	tu_pwrmode;		/* Tuner PowerӰ�� */
#define		CTU_PWROFF		0
#define		CTU_FUNCON		1

EXT	TTU_CH	tu_ch;			/* �J�����g���g���G���A */
EXT	TTU_CH	tu_isrch;			/* ISR���g���G���A */
EXT	BYTE	tu_pno;			/* ���Ď��g��PresetNo */
EXT	BYTE	tu_wrtpno;	/* WRITE���[�h�����g��PresetNo */
EXT	BYTE	tu_bndmode;		/* BandӰ�ޏ�� */
EXT	TTU_INIT tu_coef;		/* Tuning�W���p�G���A */
EXT	BYTE	tu_stcnt;		/* ST�M������ݸ޶��� */
EXT	BYTE	tu_stdistim;	/* ST�M���֎~��� */
#define		CTU_ST_ANTON		1500/50
#define		CTU_ST_PLLCHG		500/50
#define		CTU_STON_CNT	100		/* ST ON����ݸ޶���   100��*10ms */
#define		CTU_STOFF_CNT	50		/* ST OFF����ݸ޶���   50��*10ms */

EXT	BYTE	tu_1ms_timer;		/* 1ms������� */
EXT	BYTE	tu_5ms_timer;		/* 5ms������� */
EXT	BYTE	tu_10ms_timer;		/* 10ms������� */
EXT	BYTE	tu_100ms_timer;		/* 100ms������� */

EXT	BYTE	tu_test_dxlo;		/* DX/LO�ؑ֕ϐ� [matu120926] */
EXT	BYTE	tu_qinfochk_num;	/* Q-info�m�F�񐔐ݒ�ϐ� [matu121105] */

/*----------------------------------------------------
   �ϐ���`
----------------------------------------------------*/
EXT	TTU_BAND	tu_mem[CTU_BAND];	/* ����ނ�׽Ď��g��/Preset���g���G���A */
EXT	BYTE		tu_as_cnt;			/* �I�[�g�X�g�A���Ɍ������ǐ� */
EXT	BYTE		tu_updw_stus;		/* Tuner Up/Down��� */

/* APM��{����` */
#define		CTU_ASMAX		6			/* APM���oMAX�ǐ� */


/* �����޳����������߂�l */
#define		CTU_UPDW_OK		0
#define		CTU_UPDW_END	1
#define		CTU_UPDW_NOT	2

#define		CTU_PSTSCAN		0x05			/* PS��Tu_I_pstscan ��main seq�ԍ�*/


EXT	WORD		tu_dispfreq;		/* �\�����g�� */
EXT	LONG		tu_ifcnt;			/* IF���Ēl */
EXT	BYTE		tu_seekng_cnt;	/* SEEK���NG�� */

EXT	BYTE		tu_para[2][CTU_PARA_LEN];		/*	EEPROM����擾���������f�[�^��ۑ�����A100811�A�x	*/
EXT	TTU_CHKCOEF	tu_chkcef[2];					/*	20db��S-Meter���x���ȂǁA�ǃ`�F�b�N�p�f�[�^�A100811�A�x	*/
EXT	BYTE		tu_dx[2];						/*	SEEK��S-Meter臒l�A100811�A�x	*/
EXT	BYTE		tu_lo[2];						/*	SEEK��S-Meter臒l LOCAl [matu120903] */
/*----------------------------------------------------
   ROMð��ْ�`
----------------------------------------------------*/
/*----------------------------------------------------
   BAND���ݒ�ð���
[zhang070305]CLARION�d�l�ύX�Ή�
----------------------------------------------------*/
#ifdef _TU_MAIN_C_
const	BYTE	TU_BAND_INFO[CTU_DEST][CTU_BAND][2] =
{
/* ���{���� AM���W�I */
	{
		CTU_AM,	CTU_BAND0,				/* AM */
		0xFF,	0xFF,					/* �Ȃ� */
		0xFF,	0xFF					/* �Ȃ� */
	},
/* ���{���� AM/FM���W�I */
	{
		CTU_AM,	CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* �k�Č��� */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* ����Č��� */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* �I�Z�A�j�A���� */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* �A�W�A���� */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	},
/* �I�Z�A�j�A���� */
	{
		CTU_AM, CTU_BAND0,				/* AM */
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2				/* fm2 */
	}
#if	(CTU_E_TEST == 0x01)
/* E���� */
	}
		CTU_FM,	CTU_BAND1,				/* fm1 */
		CTU_FM, CTU_BAND2,				/* fm2 */
		CTU_FM, CTU_BAND3,				/* fm3 */
		CTU_AM, CTU_BAND0				/* AM */
	}
#endif
};
#else
extern	const	BYTE	TU_BAND_INFO[CTU_DEST][CTU_BAND][2];
#endif
/*----------------------------------------------------
   Preset�����ݒ�ð���
[zhang070301]CLARION�d�l�ύX�Ή�
----------------------------------------------------*/
#ifdef _TU_MAIN_C_
const WORD	 TU_PRESET_TBL[CTU_DEST][CTU_BAND][CTU_PRESET]= {
/* ���{���� AM���W�I */
/*AM*/
	{{0x0001, 0x0009, 0x0035, 0x0062, 0x0078, 0x007b},
/*  */
	 {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/*  */
	 {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},},
/* ���{���� AM/FM���W�I */
/*AM*/
	{{0x0001, 0x0009, 0x0035, 0x0062, 0x0078, 0x007b},
/*FM1*/
	 {0x0000, 0x0014, 0x0046, 0x0078, 0x008c, 0x0000},
/*FM2*/
	 {0x0000, 0x0014, 0x0046, 0x0078, 0x008c, 0x0000},},
/* �k�Č��� */
/*AM*/
	{{0x0000, 0x0007, 0x002f, 0x0057, 0x006c, 0x0076},
/*FM1*/
	 {0x0001, 0x000c, 0x0034, 0x005c, 0x0065, 0x0065},
/*FM2*/
	 {0x0001, 0x000c, 0x0034, 0x005c, 0x0065, 0x0065},},
/* ����Č��� */
/*AM*/
	{{0x0000, 0x000e, 0x005e, 0x00ae, 0x00d8, 0x00ec},
/*FM1*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},
/*FM2*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},},
/* �I�Z�A�j�A���� */
/*AM*/
	{{0x0001, 0x0009, 0x0035, 0x0062, 0x0078, 0x007b},
/*FM1*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},
/*FM2*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},},
/* �A�W�A���� */
/*AM*/
	{{0x0000, 0x0008, 0x0034, 0x0061, 0x0077, 0x007a},
/*FM1*/
	 {0x0008, 0x0034, 0x00d4, 0x0174, 0x0198, 0x0198},
/*FM2*/
	 {0x0008, 0x0034, 0x00d4, 0x0174, 0x0198, 0x0198},},
/* ��O�n����� */
/*AM*/
	{{0x0009, 0x0051, 0x01DD, 0x0372, 0x0438, 0x0453},
/*FM1*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},
/*FM2*/
	 {0x0004, 0x001a, 0x006a, 0x00ba, 0x00cc, 0x00cc},},

#if	(CTU_E_TEST == 0x01)
/*E Dest*/
/*FM1*/
	,{{0x00, 0x08, 0xd4, 0x14c,0x198, 0x19A},
/*FM2*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/*FM3*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/*AM*/
	{0x33, 0x5f, 0x8c, 0x03, 0x15, 0x2a},}
#endif
};
#else
extern	const WORD TU_PRESET_TBL[CTU_DEST][CTU_BAND][CTU_PRESET];
#endif

/*----------------------------------------------------
   Tunning�W����`ð���
[sun060915] SD臒lýĔŎd�l�����ύX�Ή�
[zhang070301]CLARION�d�l�ύX�Ή�
	max_ch, min_ch, step,   min_n, 	if_min, 	if_max, 	if_wait, sd_dx, sd_loc, wait_edge, sd_wait, fref, min_freq, c_wait
----------------------------------------------------*/
#ifdef _TU_MAIN_C_
const TTU_DEST	 TU_COEF_TBL[CTU_DEST]= {
/* ���{���� AM���W�I */
	{0x00,	0x00,					/* FM/AM LW Band�Ȃ� */
/* FM_MW */
	{7600,	0x008c,	0x00,	100,	500/5,	50/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{522,	0x007b,	0x00,	9,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* ���{���� AM/FM���W�I */
	{0x00,	0x00,					/* FM/AM LW Band�Ȃ� */
/* FM_MW */
	{7600,	0x008c,	0x00,	100,	500/5,	50/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{522,	0x007b,	0x00,	9,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},
	
/* �k�Č��� */
	{0x00,	0x00,					/* FM/AM LW Band�Ȃ� */
/* FM_MW */
	{8770,	0x0065,	0x00,	200,	500/5,	50/5},                     
/* FM_LW */
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */
	{530,	0x0076,	0x00,	10,		500/5,	50/5},
/* AM_LW */
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* ����Č��� */
	{0x00,	0x0000,					/* FM/AM LW Band�Ȃ� */
/* FM_MW */
	{8750,	0x00cd,	0x00,	100,	500/5,	50/5},                     
/* FM_LW */
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */
	{530,	0x00ec,	0x00,	5,		500/5,	50/5},
/* AM_LW */
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* �I�Z�A�j�A���� */
	{0x00,	0x00,					/* FM/AM LW Band�Ȃ� */
/* FM_MW */
	{8750,	0x00cd,	0x00,	100,	500/5,	30/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{522,	0x0084,	0x00,	9,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* �A�W�A���� */
	{0x00,	0x00,					/* FM/AM LW Band�Ȃ� */
/* FM_MW */
	{8750,	0x019a,	0x00,	50,		500/5,	30/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{531,	0x007a,	0x00,	9,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

/* ��O�n��d���� */
	{0x00,	0x00,					/* FM/AM LW Band�Ȃ� */
/* FM_MW */
	{8750,	0x00cd,	0x00,	100,	500/5,	30/5},                     
/* FM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{522,	0x0453,	0x00,	1,		500/5,	50/5},
/* AM_LW */                         
	{0x00,	0x0000,	0x00,	0x00,	0x00,	0x00}},

#if	(CTU_E_TEST == 0x01)
/* E Dest */
	,{0x02,	0x00,					/* FM/AM LW Band�Ȃ� */
/* FM_MW */
	{8750,	0x19A,	0x00,	50,		500/5,	50/5},                     
/* FM_LW */                         
	{0x00,	0x00,	0x00,	0x00,	0x00,	0x00},
/* AM_MW */                         
	{531,	0xa3,	0x2b,	9,		500/5,	50/5},
/* AM_LW */                         
	{153,	0x2a,	0x00,	3,		500/5,	50/5}}
#endif
};
#else
extern const TTU_DEST	 TU_COEF_TBL[CTU_DEST];
#endif

#ifdef _TU_MAIN_C_
const WORD	 TU_ISR_TBL[3]= 
{
/* min_freq	min_ch	max_ch */
	1620,	0x007a,	0x007b,
};
#else
extern	const WORD TU_ISR_TBL[3];
#endif

/*	U����FM	*/
#ifdef _TU_MAIN_C_
const	BYTE TU_PARA_FM_U_TBL[] = {
	0x20 , // TUNER0
	0x00 , // TUNER1
	0x40 , // TUNER2
	0x85 , // RADIO
	0x05 , // SOFTMUTE0
	0x44 , // SOFTMUTE1
	0x90 , // SOFTMUTE_FM
	0x03 , // HIGHFCUT0
	0x58 , // HIGHFCUT1
	0x00 , // HIGHFCUT2
	0x07 , // STEREO0
	0x50 , // STEREO1
	0x05 , // STEREO2
	0x16 , // CONTROL
	0x40 , // LEVEL_OFFSET
	0x04  // AM_LNA
};
#else
extern	const BYTE TU_PARA_FM_U_TBL[];
#endif

/*	J����FM	*/
#ifdef _TU_MAIN_C_
const	BYTE TU_PARA_FM_J_TBL[] = {
	0x20 , // TUNER0
	0x00 , // TUNER1
	0x40 , // TUNER2
	0x81 , // RADIO
	0x05 , // SOFTMUTE0
	0x49 , // SOFTMUTE1
	0x90 , // SOFTMUTE_FM
	0x03 , // HIGHFCUT0
	0x50 , // HIGHFCUT1
	0x00 , // HIGHFCUT2
	0x07 , // STEREO0
	0x50 , // STEREO1
	0x05 , // STEREO2
	0x16 , // CONTROL
	0x40 , // LEVEL_OFFSET
	0x04   // AM_LNA
};
#else
extern	const BYTE TU_PARA_FM_J_TBL[];
#endif

/*	AM���W�I(����)����MW	*/
#ifdef _TU_MAIN_C_
const	BYTE TU_PARA_MW_AM_TBL[] = {
	0x00 , // TUNER0
	0x00 , // TUNER1
	0xC0 , // TUNER2
	0x81 , // RADIO
	0x06 , // SOFTMUTE0
	0x50 , // SOFTMUTE1
	0x0A , // SOFTMUTE_AM
	0x06 , // HIGHFCUT0
	0x48 , // HIGHFCUT1
	0x00 , // HIGHFCUT2
	0x00 , // STEREO0
	0x00 , // STEREO1
	0x00 , // STEREO2
	0x14 , // CONTROL
	0x40 , // LEVEL_OFFSET
	0x04   // AM_LNA
};
#else
extern	const BYTE TU_PARA_MW_AM_TBL[];
#endif

/*	AMFM���W�I����MW	*/
#ifdef _TU_MAIN_C_
const	BYTE TU_PARA_MW_AMFM_TBL[] = {
	0x00 , // TUNER0
	0x00 , // TUNER1
	0xC0 , // TUNER2
	0xA5 , // RADIO
	0x06 , // SOFTMUTE0
	0x50 , // SOFTMUTE1
	0x0A , // SOFTMUTE_AM
	0x06 , // HIGHFCUT0
	0x48 , // HIGHFCUT1
	0x00 , // HIGHFCUT2
	0x00 , // STEREO0
	0x00 , // STEREO1
	0x00 , // STEREO2
	0x14 , // CONTROL
	0x40 , // LEVEL_OFFSET
	0x04   // AM_LNA
};
#else
extern	const BYTE TU_PARA_MW_AMFM_TBL[];
#endif


#ifdef _TU_MAIN_C_
const BYTE*	 TU_PARA_TBL[CTU_DEST][2]= 
{
/*JAPAN Dest AM*/
	TU_PARA_FM_J_TBL,TU_PARA_MW_AM_TBL
/*JAPAN Dest AM/FM*/
	,TU_PARA_FM_J_TBL,TU_PARA_MW_AMFM_TBL
/*U Dest �k��*/
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
/*B Dest �����*/
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
/* ��ޱ���� �I�Z�A�j�A*/
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
/* ��ޱ���� */
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
/* ��O�n����� */
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
#if	(CTU_E_TEST == 0x01)
/*Europe Dest*/
	,TU_PARA_FM_U_TBL,TU_PARA_MW_AMFM_TBL
#endif
};
#else
extern	const BYTE* TU_PARA_TBL[CTU_DEST][2];
#endif

#ifdef _TU_MAIN_C_
const	TTU_CHKCOEF		TU_INI_CHKCOEF[2] = 
{
	{	0x67,	0xA6,	0xff,	0x18,	0x05,	0x03,	0x14,	0x0F,	0x0F,	0x05,	0x05	},
	{	0x92,	0xC9,	0x3C,	0x24,	0x05,	0x02,	0x1e,	0x0F,	0x0F,	0xFF,	0xFF	}
};	/* [matu120903] */

#define		TU_AM_IF_DETAIL_THIRD_DEST	0x00	/* 16OTP�d�l�ύX ��O�n��d���� IFDetail�l */
#else
extern	const	TTU_CHKCOEF		TU_INI_CHKCOEF[2];
#endif

#endif
