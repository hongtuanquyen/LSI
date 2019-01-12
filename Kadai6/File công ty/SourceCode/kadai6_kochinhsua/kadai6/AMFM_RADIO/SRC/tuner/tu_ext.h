/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: tu_ext.h
		�@�\			: �@Tuner �O��IF ͯ��̧��
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051212]	�V�K�쐬
******************************************************************************/
#ifndef		_TU_EXT_H_
#define		_TU_EXT_H_

#include	"../disp/lcd_ext.h"

#undef		EXT
#ifdef		_TU_EXT_C_
#define		EXT
#else
#define		EXT		extern
#endif

/*-------------------------------------
		 �֐��錾
-------------------------------------*/
EXT	BYTE Tu_band_chk(void);
EXT	BYTE Tu_bandno_chk(void);
EXT	BYTE Tu_pstno_chk(void);
EXT	void Tu_1ms_timer(void);
EXT	void Tu_5ms_timer(void);
EXT	BYTE Tu_indicator_set(void);
EXT	BYTE Tu_isr_indicator_set(void);
EXT	BYTE Tu_mainmd_check(void);
EXT	BYTE Tu_st_check(void);
EXT	void Tu_lcddata_set(TDP_TUNER * , BYTE);
EXT	void Tu_isr_lcddata_set(TDP_ISR * , BYTE);
EXT	void Tu_amband_set(void);
EXT	void Tu_isrmd_on(void);
EXT	void Tu_isrmd_off(void);
EXT	void Tu_bnd_set(BYTE bandno);		/*	�e�X�g���[�h�̒���AM/FM1�I����Ή��A100826�A�x	*/
EXT	void Tu_beep_mute(BYTE on_off);
/* ���t�@�C���֐� */
EXT	void Tu_SaveEepData(BYTE status,BYTE *BufAddr);	/* tu_eeprom.c */
EXT	void Tu_key_isr_on(void);						/* tu_key.c */
EXT	void Tu_cmd_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Lenth);	/* tu_test.c */

/*-------------------------------------
		 BAND ����`
-------------------------------------*/
#define		CTU_FM		0 		/* FM BAND�� */
#define		CTU_AM		1	 	/* AM BAND�� */

#define		CTU_BAND_AM		0 		/* AM BAND�� */
#define		CTU_BAND_FM1	1	 	/* FM1 BAND�� */
#define		CTU_BAND_FM2	2 		/* FM2 BAND�� */

#define		CTU_BAND0	0		/* BandNo���� */
#define		CTU_BAND1	1		/* BandNo 1 */
#define		CTU_BAND2	2		/* BandNo 2 */
#define		CTU_BAND3	3		/* BandNo 3 */


#endif
