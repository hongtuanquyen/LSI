/*************************************************************************
	�N�����I���a���� 
	�v���W�F�N�g��	�F2012�NAMFM_RADIO����
	�t�@�C����		�Fsbm_def.h
	���W���[����	�FSUB���[�h
	�@   �\			�FSUBӰ�ޒ�`�t�@�C��
--------------------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
--------------------------------------------------------------------------
			2007/3/17		0.1		��		�V�K
*************************************************************************/
#ifndef		_SBM_DEF_H_
#define		_SBM_DEF_H_

#include	"../model.h"

#undef	EXT
#ifdef	_SBM_EXT_C_
#define	EXT   
#else
#define	EXT	extern
#endif


/*---------------�׸ޒ�`-----------------------*/
EXT	TCOM_FLG 	sbm_flag;
#define		fsbm_vol_mode		sbm_flag.bit.bit7		/* vol���[�h�׸� */
#define		fsbm_ver_disp		sbm_flag.bit.bit6		/* �o�[�W�����\�����[�h�׸� */
#define		fsbm_alm_off_req	sbm_flag.bit.bit5		/* alm���[�hAM/PM�׸� */
#define		fsbm_alm_onoff		sbm_flag.bit.bit4		/* alm���[�hON/OFF�׸� */
//#define		fsbm_alm_setted		sbm_flag.bit.bit3		/* alm�N�������׸� */
#define		fsbm_alm_beepon		sbm_flag.bit.bit2		/* alm�N�������׸� */
#define		fsbm_beep_en		sbm_flag.bit.bit1		/* alm�N�������׸� */
	
	
	
	
	
	
	
enum
{
	CSBM_OFF,		/* SUB���[�h�ł͂Ȃ�	*/
	CSBM_BEP,		/* ALM BEEP�� */
	CSBM_VOL,		/* VOL�������[�h������ 		*/
	CSBM_VER,		/* VOL�������[�h������ 		*/
	CSBM_SEF,		/* AUDIO�������[�h������ 	*/
	CSBM_CLK,		/* CLOCK�������[�h������ 		*/
	CSBM_ALM,		/* ALARM�������[�h������ 		*/
	CSBM_TES		/* TESE���[�h������ 	*/
};

/*--------------SUBӰ�ޗL��bit��`----------------*/
#define		CSBM_BIT_PWR		0x20
#define		CSBM_BIT_VOL		0x10
#define		CSBM_BIT_BEP		0x08
#define		CSBM_BIT_ALM		0x04
#define		CSBM_BIT_CLK		0x02
#define		CSBM_BIT_AUD		0x01



/*---------------------------VOLUME����Ӱ��----------------------*/
#define		CSBM_VOL_7S			70			/* 100ms���� */
EXT	BYTE	sbm_vol_tim;


/*---------------�B���L�[�iSECRET KEY�jӰ��----------------------*/
enum{
	CSBM_SCT_OFF,
	CSBM_SCT_VER,
	CSBM_SCT_BEEP,
	CSBM_SCT_LOUDOFF
};

EXT BYTE sbm_sct_mode;						/* TES���[�h */



/*---------------------------AUDIO����Ӱ��----------------------*/
#undef	EXT
#ifdef	_SEF_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif


#define		CSBM_AUD_7S			70			/* 100ms���� */


enum		/* �������� */
{
	CSBM_AUD_OFF = 0,
	CSBM_AUD_BAS,
	CSBM_AUD_TRE,
	CSBM_AUD_BAL
};

EXT BYTE sbm_aud_mode;				/* AUDIO����Ӱ�� */
EXT BYTE sbm_aud_tim;				/* AUDIO����Ӱ�ޕێ���� */



/*---------------------------CLOCK����Ӱ��----------------------*/

#undef	EXT
#ifdef	_CLOCK_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif


#define		CSBM_CLK_15S			150			/* 100ms���� */

typedef	struct{
	BYTE	min;
	BYTE	hour;
}TSBM_CLK;

EXT	BYTE	sbm_clk_tim;					/* ���v�������[�h�ێ���� */
EXT	BYTE	sbm_clk_mode;					/* ���vӰ�� */
#define		CSBM_CLK_NORMAL		0			/* ���v�����ς� */
#define		CSBM_CLK_ADJUST		1			/* ���v�����̒����� */

#define		CSBM_CLK_AM			0			/* ���v�̌ߑO�� */
#define		CSBM_CLK_PM			1			/* ���v�̌ߌ� */

EXT		TSBM_CLK	sbm_clk_adj;			/* ���v�����ϐ��ر */
EXT		TSBM_CLK	sbm_clk_bak;			/* ���v�����ϐ��ر */
#define		CSBM_CLK_1MIN	60
#define		CSBM_CLK_1H		60
#define		CSBM_CLK_1DAY	24

EXT	WORD	sbm_beepon_tim;					/* ���v�������[�h�ێ���� */
#define		CSBM_BEEPON_180S	180000/100	/* 180S */


/*---------------------------ALARM����Ӱ��----------------------*/
#undef	EXT
#ifdef	_ALM_MODE_C_
#define	EXT   
#else
#define	EXT	extern
#endif

#define		CSBM_ALM_15S			150			/* 100ms���� */

typedef	struct{
	BYTE	min;
	BYTE	hour;
}TSBM_ALM;

EXT	BYTE	sbm_alm_tim;					/* ALARM�������[�h�ێ���� */
EXT	BYTE	sbm_alm_mode;					/* ALARM����Ӱ�� */
#define		CSBM_ALM_NORMAL		0			/* ALARM�����ς� */
#define		CSBM_ALM_ADJUST		1			/* ALARM�����̒����� */
EXT		TSBM_ALM	sbm_alm_adj;			/* ALARM�����ϐ��ر */
EXT		TSBM_ALM	sbm_alm_bak;			/* ALARM�����ϐ��ر */




#endif

