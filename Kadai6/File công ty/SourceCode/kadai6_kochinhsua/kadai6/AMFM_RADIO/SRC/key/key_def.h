/******************************************************************************
	�N�����I���a����
	�v���W�F�N�g��	: 2012�NAMFM_RADIO���f��
	�t�@�C����		: key_def.h
	�@  �\			: Key���W���[�� �f�[�^�^�A�萔��`�w�b�_�t�@�C��
------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
******************************************************************************/
#ifndef		_KEY_DEF_H_
#define		_KEY_DEF_H_


#define		CKEY_TEST_KEY		1			/* ýđ��d�����o�L�薳�� */

/* -------------------------------------------------------- */
/*	���� Key�R�[�h 											*/
/* -------------------------------------------------------- */
#define	CKEY_ID_KEY_OFF		0	/*	�L�[����		*/
#define	CKEY_ID_LCD_PWR		1	/*	PWR	 			*/
#define	CKEY_ID_LCD_AS		2	/*	AS			 	*/
#define	CKEY_ID_LCD_SRC		3	/*	MODE		 	*/
#define	CKEY_ID_LCD_VOLUP	4	/*	VOLUP			*/
#define	CKEY_ID_LCD_CH		5	/*	CH				*/
#define	CKEY_ID_LCD_VOLDW	6	/*	VOLDW			*/
#define	CKEY_ID_LCD_RIGHT	7	/*	UP				*/
#define	CKEY_ID_LCD_ALARM	8	/*	ALARM			*/
#define	CKEY_ID_LCD_LEFT	9	/*	DOWN			*/
#define	CKEY_ID_LCD_TI_SCAN	10	/*	TI/SCAN			*/

#define	CKEY_ID_LCD_TEST1	200	/*	CH�{UP�{DW  �L�[��3�d�L�[�R�[�h  */
#define	CKEY_ID_LCD_TEST2	201	/*	CH�{UP�{VUP �L�[��3�d�L�[�R�[�h  */
#define	CKEY_ID_LCD_TEST3	202	/*	CH�{UP�{VDW �L�[��3�d�L�[�R�[�h  */

#define	CKEY_ID_LCD_TEST1_P		203	/*	CH�{UP�{DW  �L�[��3�d�L�[�R�[�h  */
#define	CKEY_ID_LCD_TEST2_P		204	/*	CH�{UP�{VUP �L�[��3�d�L�[�R�[�h  */
#define	CKEY_ID_LCD_TEST3_P		205	/*	CH�{UP�{VDW �L�[��3�d�L�[�R�[�h  */

#define	CKEY_ID_MULTIPLEX	254	/*	���d�L�[						*/

#define	CKEY_ID_INVALID		255	/*	�����L�[						*/

/* ------------------------------------------------------------ */
/*	Key Timer	 												*/
#define	CKEY_LONG_WAIT_TIME200		8		/*	200/25	->	4		*/
#define	CKEY_LONG_WAIT_TIME500		20		/*	500/25	->	10		*/
#define	CKEY_LONG_WAIT_TIME1000		40		/*	1000/25	->	20		*/
#define	CKEY_LONG_WAIT_TIME2000		80		/*	2000/25	->	40		*/
#define	CKEY_LONG_WAIT_TIME2000_ACCOFF	200	/*	2000/10	->	20		*/
#define	CKEY_TIME_UP				0		/*	Timer�^�C���A�b�v	*/
#define	CKEY_IN_TIMER_MAX			0xFE	/*	Timer MAX			*/
#define	CKEY_TIMER_STOP				0xFF	/*	Timer STOP			*/

#define	CKEY_IN_TIME50ms		10		/*	50/5	->	10		*/

/*	LCD Key input 											*/
#define	CKEY_LCD_DATA_BYTE			4	/*	KD1-KD30->4Byte	*/
/* -------------------------------------------------------- */
#define	CKEY_BYTE_MAX	0xFF	/*	��������BYTE�f�[�^�ő�l	*/
#define	CKEY_WORD_MAX	0xFFFF	/*	��������2BYTE�f�[�^�ő�l	*/
/* -------------------------------------------------------- */
/*	�^��`	*/
/* Key ���쏈���o�^�e�[�u���^ */
#define	KEY_PROC_TBL_SIZE	16	/*	�e�[�u���T�C�Y 16Byte	*/
typedef struct{
	BYTE	code;			/*	�L�[�R�[�h							*/
	BYTE	comenb_on;		/*	�L�[ON�������ʃ��[�h�L���ݒ�			*/
	BYTE	modenb_on;		/*	�L�[ON�������W���[�����L���ݒ�			*/
	void	(* key_on)();	/*	�L�[ON�����A�h���X						*/
	BYTE	comenb_long;	/*	�L�[�������������ʃ��[�h�L���ݒ�		*/
	BYTE	modenb_long;	/*	�L�[�������������W���[�����L���ݒ�		*/
	void	(* key_long)();	/*	�L�[�����������A�h���X					*/
	BYTE	comenb_off;		/*	�L�[OFF�������ʃ��[�h�L���ݒ�			*/
	BYTE	modenb_off;		/*	�L�[OFF�������W���[�����L���ݒ�			*/
	void	(* key_off)();	/*	�L�[OFF�����A�h���X					*/
}KEY_PROC_TBL;
/* Key ���쏈���o�^�e�[�u���^ */
#define	KEY_CHG_TBL_SIZE	2	/*	�e�[�u���T�C�Y 2Byte				*/
typedef struct{
	BYTE	com_key_code;	/*���ʃL�[�R�[�h*/
	BYTE	local_key_code;	/*�e���W���[�����[�J���L�[�R�[�h*/
}KEY_CHG_TBL;

#endif