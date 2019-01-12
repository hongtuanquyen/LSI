/***************************************************************
	�N�����I���a����   
	�v���W�F�N�g��	�F'12�N���f���J��
	�t�@�C����		�Flcd_ext.h
	���W���[����	�FLCD(�\��)
	�@   �\			�F�C���^�t�F�[�X��`ͯ��̧��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/30		0.1		ha		�V�K�쐬
****************************************************************/
#ifndef		_LCD_EXT_H_
#define		_LCD_EXT_H_

#include	"../disp/lcd_main.h"

#undef	EXT
#ifdef	_LCD_EXT_C_
#define	EXT   
#else
#define	EXT	extern
#endif

/*-----------------�O���Q�Ɨp��������--------------*/

/*----------------�萔��`-------------------------*/
/* �\���x����ϒ�` */
/* 10ms���� */
#define		CLCD_EDITWAIT_50MS			5		/* 50ms */
#define		CLCD_EDITWAIT_100MS			10		/* 100ms */
#define		CLCD_EDITWAIT_120MS			12		/* 120ms */
#define		CLCD_EDITWAIT_150MS			15		/* 150ms */
#define		CLCD_EDITWAIT_200MS			20		/* 200ms */

/* ���v�\���ؑ� */
#define		CLCD_CLK_CHG		2

/* �ꎞ�\����` */
#define		CLCD_1SHOT_1S		10			/* 1000/100 */
#define		CLCD_1SHOT_2S		21			/* 2000/100 */
#define		CLCD_1SHOT_3S		30			/* 3000/100 */
#define		CLCD_1SHOT_5S		50			/* 5000/100 */
#define		CLCD_1SHOT_10S		100			/* 10000/100 */
/*[070228he]--------------------start----------------------------*/
#define		CLCD_1SHOT_4S		40			/* 4000/100 */
#define		CLCD_1SHOT_7S		70			/* 7000/100 */
/*------------------------------end------------------------------*/

enum
{
	CLCD_OFF = 0,
	CLCD_VOL,
	CLCD_BAS,		/* 1: BASS (SEL) 	*/
	CLCD_TRE,		/* 2: TREBLE (SEL) 	*/
	CLCD_BAL,		/* 3: BALANCE(SEL)  */
	CLCD_FAD,		/* 4: FADER (SEL)	*/
	CLCD_1SHOT_MAX
};


/* ��۰ٗv�� */
#define		CLCD_SCR_2SON		1		/* 2�b�����Ă��罸۰ٽ��� */
#define		CLCD_SCR_ON			2		/* �ꎞ��������۰� */
#define		CLCD_SCR_2ON		3		/* �ꎞ�������A2��۰� */
#define		CLCD_SCR_SRC		4		/* ����J�ڽ�۰� */
#define		CLCD_SCR_DEMO		5		/* DEMO��۰� */


/* �ݼ޹����` */
/* TUNER���[�h */
#define		CLCD_TU_IND_FM				0x01
#define		CLCD_TU_IND_AM				0x02
#define		CLCD_TU_IND_ONE				0x04
#define		CLCD_TU_IND_TWO				0x08
#define		CLCD_TU_IND_ST				0x10
#define		CLCD_TU_IND_TI				0x20
#define		CLCD_TU_IND_S1				0x40

/* ���v���[�h */
#define		CLCD_CL_IND_AM				0x01
#define		CLCD_CL_IND_PM				0x02
#define		CLCD_CL_IND_AL				0x04

/* AUX���[�h */
#define		CLCD_AU_IND_AU				0x01
/*-----------------�T�uӰ��---------------------*/
/* TEST�ް� */
typedef struct
{
	BYTE mode;					/* �\��Ӱ�� 		*/
	BYTE value;					/* �l 				*/
}TDP_TES;
/* AUDIO�ް� */
 typedef struct
{
	BYTE mode;					/* SEL�\�����[�h 	*/
	BYTE value;					/* value 			*/
}TDP_SEL;

enum
{
	CLCD_SEL_BASG,
	CLCD_SEL_BASF,
	CLCD_SEL_BASQ
};

enum
{
	CLCD_SEL_TREG,
	CLCD_SEL_TREF
};

enum
{
	CLCD_SEL_ZEHOFF,
	CLCD_SEL_ZEHB,
	CLCD_SEL_ZEHIM,
	CLCD_SEL_ZEHEX,
	CLCD_SEL_ZEHCS
};

/* ���Ѻ��۰� */
typedef struct
{
	BYTE	mode;				/* SYSTEM�\�����[�h */
	BYTE	value;				/* value 			*/
}TDP_SYM;
/* ���v�ް� */
typedef struct
{
	BYTE hour;					/* �u���v���  				*/
	BYTE min;					/* �u���v���  				*/
	BYTE adjust;					/* 0:������ 1�F�����ς� */
	BYTE mode;					
}TDP_CLK;

#define		CLCD_CLK_NOMAL		0			/* �m�[�}�� */
#define		CLCD_CLK_HOUR		1			/* �������� */
#define		CLCD_CLK_MIN		2			/* �������� */
/* �T�uӰ�ނ̕\��Ӱ�ނ̓T�uӰ�ނŒ�`���܂� */

/*-------------------------Ҳ�Ӱ��-------------------------*/
/* tuner�ް���� */
typedef struct
{
 	BYTE mode;		   			/* �\��Ӱ�� 							*/
	BYTE *add;		   			/* ps�ް����߲�� 						*/
	BYTE length;				/* PS�ް����޲Đ� 						*/
	BYTE band;		  			/* AM/FM 								*/
	BYTE bandno;		   		/* BAND�ԍ� 							*/
 	BYTE base;					/* �x�[�X�\�� 1�����g�� 2��PS 			*/
 	BYTE frq[3];		   		/* ���g�� 								*/
 	BYTE preno;		   			/* �v���Z�b�gNO. 						*/
	BYTE pty;		   			/* PTY�ԍ��f�[�^ 0�`34  HEX 			*/
	BYTE test;					/* RDS�e�X�g���f 1��RDS�� 2���ʏ풆		*/

}TDP_TUNER;

#define		CLCD_TUBASE_FRQ			1
#define		CLCD_TUBASE_PS			2

typedef struct
{
	BYTE band;		  			/* AM/FM 								*/
 	BYTE frq[3];		   		/* ���g�� 								*/
}TDP_ISR;
/* [100701xu]DIR���[�h��ǉ� */
typedef struct
{
	BYTE num[4];
	BYTE mode;		   			/* �\��Ӱ�� 							*/
}TDP_DIR;
/* [100701xu]TUNER TEST���[�h��ǉ� */
typedef struct
{
	BYTE band;
	BYTE mode;		   			/* �\��Ӱ�� 							*/
	BYTE value;		   			/* �\��DB�� 							*/
}TDP_TUTEST;
enum
{
	CLCD_TUTEST_LITTLE,
	CLCD_TUTEST_LITING,
	CLCD_TUTEST_MIDLE,
	CLCD_TUTEST_MIDING,
	CLCD_TUTEST_CHECK,
	CLCD_TUTEST_CHKING,
};



/*--------------�\��Ӱ�ޒ�`--------------------------*/
/* Tuner */
#define		CLCD_DISPMODE		0		/* DISP MODE */
enum
{
	CLCD_TU_SCAN = 1,			/* 1: SCAN 				*/
	CLCD_TU_SCANWAIT,			/* 2: SCAN��M��		*/
	CLCD_TU_SEEK,				/* 3: SEEK�� 			*/
	CLCD_TU_AS,					/* 4: AS�� 			*/
	CLCD_TU_PSTWT,				/* 5: �v���Z�b�g������	*/
	CLCD_TU_MAX
};

/*----------------------�O���Q�Ɨp�����܂�------------------*/




/*----------------------Ӽޭ�ٓ�����`��������----------------*/
/* ��ϒ萔 */
#define		CLCD_BASE_5s					50			/* 5�b�A5000/100 */
#define		CLCD_BASE_2s					21			/* [100904]2�b�A2000/100->2.1�b */
#define		CLCD_BASE_10s					100			/* 10�b�A10000/100 */

#define		CLCD_SCRLL_2S					42			/* 2�b�A2000/50 */	/*[101026xu]USB/MP3 FOL���O�\���C��*/
#define		CLCD_SCRLL_700MS				14			/* 700ms, 700/50 */
#define		CLCD_SCRLL_200MS				4			/* 200ms, 200/50 */
#define		CLCD_SCRLL_300MS				6			/* 300ms, 300/50 */
#define		CLCD_SCRLL_350MS				7			/* 350ms, 350/50 */
#define		CLCD_SCRLL_500MS				10			/* 500ms, 500/50 */
#define		CLCD_SRC_SCRLL_50MS				2			/* 50ms, 50/25 */

#define		CLCD_BASEWAIT_TIM				20			/* 200ms,200/10 */

/* �x�[�X�\��Ӱ�� */
enum
{
	CLCD_BASE_TUNER = 1,
	CLCD_BASE_ISR,
	CLCD_BASE_AUX,
	CLCD_BASE_CLOCK,
	CLCD_BASE_MAX
};


#define		CLCD_1SHOT_OFF		0xFF			/* �ꎞ�\��������ݾ� */
#define		CLCD_SSP_CANCEL_TIM			60			/* 30�b�A30000/500 */

/*-------------���-----------------------*/
EXT BYTE lcd_tim_1shot;				/* �ꎞ�\����� */
EXT BYTE lcd_tim_scr;				/* ��۰���� */
EXT BYTE lcd_tim_1shot_blk;			/* �ꎞ������� */
EXT BYTE lcd_tim_base;				/* �x�[�X�\����� */
EXT BYTE lcd_base_wait_tim;			/* �ް��\���҂���� */


EXT BYTE lcd_ftitl_req;
/*-----------Ӱ��-------------------*/
EXT BYTE lcd_1shot_mode;			/* �ꎞ�\��Ӱ�� */
EXT BYTE lcd_scr_cnt;				/* ��۰ٶ��� */
EXT BYTE lcd_1shot_blk;				/* �ꎞ����Ӱ�� */
EXT BYTE lcd_base_mode;				/* �x�[�XӰ�� */ 
EXT BYTE lcd_test_buf[4];

/*----------------------------------------------------------------
		�֐���������
-----------------------------------------------------------------*/

EXT void Lcd_1shot_disp(BYTE, BYTE);		/* �ꎞ�\���v�� */
EXT void Lcd_1shot_cancel(BYTE);			/* �ꎞ�\����ݾي֐� */
EXT void Lcd_1shot_key_cancel(BYTE);		/* �ꎞ�\���A������\���AOPEN/CLOSE�\��,SUB��ݾ�(�L�[�����p) */
EXT BOOL Lcd_dispkey_1shotcancel(void);
EXT void Lcd_disp_base(BYTE);
EXT void Lcd_disp_1shot_base(BYTE);			/* �ް��\���v���C�eӼޭ�ٓ�����Ԃ��ς�鎞(�L�[����ȊO) */
EXT void Lcd_base_scrll(BYTE);				/* mp3/ipod�G���[��ԉ����A�Ȃ��ς�������A�ް�����۰ٗv�� */
EXT void Lcd_10ms_cnt(void);								/* 10ms�����ݏ��� */
EXT void Lcd_blktim_set(void);								/* �_����ύĐݒ� */
EXT BOOL Lcd_1shot_check(void);
EXT BOOL Lcd_1shotsrc_cancel(void);
EXT void Lcd_base_disp_cancel(void);


EXT	void Lcd_send_req(void);

EXT void Lcd_accoff_send_req(void);
EXT BOOL Lcd_accoff_send_end(void);
EXT void Lcd_testmode_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Length);




#endif
