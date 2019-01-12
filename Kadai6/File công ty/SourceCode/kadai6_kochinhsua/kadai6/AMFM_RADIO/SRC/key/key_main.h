/******************************************************************************
	�N�����I���a����
	�v���W�F�N�g��	: 2012�NAMFM_RADIO���f��
	�t�@�C����		: key_main.h
	�@�\			: �L�[���W���[�����C���w�b�_�t�@�C��
------------------------------------------------------------------------------
���t		�S����		�C�����e
2012/07/26	���C��		�V�K�쐬
******************************************************************************/
#ifndef	_KEY_MAIN_H_
#define	_KEY_MAIN_H_

 #undef	EXT
 #ifdef	_KEY_MAIN_C_
 #define	EXT
 #else
 #define	EXT	extern
 #endif

/*----------------------------------------------------
   �֐��v���g�^�C�v�錾
----------------------------------------------------*/
/*	key���W���[�����C������			*/
EXT	void	Key_main(void);
/*	key���W���[���̕ϐ�����������		*/
EXT	void	Key_initial(void);

/*----------------------------------------------------
   �t���O��`
----------------------------------------------------*/
EXT	TCOM_FLG key_flag;	/*Key Module Flag Memory area */
#define		fkey_prio_key		(key_flag.bit.bit0)/* �D��L�[���쒆�t���O */
#define		fkey_alldis			(key_flag.bit.bit1)/* ALL�L�[�֎~�t���O */
#define		fkey_nop			(key_flag.bit.bit2)/* �捞�����t���O ���� 1:ON/�L�� 0:OFF		*/
#define		fkey_alarm_key		(key_flag.bit.bit3)/* alarmKey�������t���O */

/*----------------------------------------------------
   �����A�N�Z�X�ϐ���`
----------------------------------------------------*/
EXT	BYTE	key_long_timer;			/*	Long Key �m��p�^�C�}�J�E���^		*/

/*----------------------------------------------------
   �ϐ���`
----------------------------------------------------*/
EXT	BYTE	key_lcd_input;			/*	LCD Key Code Memory area		*/
EXT	BYTE	key_lcd_driver_data[4];	/* LCD �f�[�^���̓o�b�t�@(32Bit)	*/
EXT	BYTE	key_lcd_input_Timer;	/* LCD �捞�C���^�[�o���^�C�} base-time = 5ms	*/
EXT	BYTE	key_in_disable_timer;	/* (Key disable) 	*/
EXT	BYTE	key_status;				/* Key��� */

EXT	BYTE	key_before_key;			/* �B���L�[�pKey��� */
EXT	WORD	key_test_code;			/* Test�p */
EXT	BYTE	key_test_sta;			/* Test�p */
EXT	BYTE	key_t_data[4];		/* Test�p */
enum{
	CKEY_TSET_NOP,
	CKEY_TSET_INPUT,
	CKEY_TSET_REPLY
};


#define		CKEY_KEYNOP		0		/* Key��ԕω����� */
#define		CKEY_KEYON		1		/* KEY�����ꂽ */
#define		CKEY_KEYOFF		2		/* KEY�����ꂽ */
#define		CKEY_KEYLONG	3		/* KEY�������m�� */

EXT	BYTE	key_now_key;			/*	�J�����g�������L�[�R�[�h			*/
EXT	BYTE	key_old_key;			/*	�O��m��L�[�R�[�h			*/
EXT	BYTE	key_new_key;			/*	����m��L�[�R�[�h			*/

EXT	void 	(*Key_ProcFunc)(void);		/* Key����֐��A�h���X	*/
EXT	BYTE	key_modenb;
EXT	BYTE	key_comenb;
EXT	BYTE	key_mode_stat;
EXT	BYTE	key_com_stat;

EXT	const KEY_PROC_TBL *	ProceT_p;			/*	�����e�[�u���A�h���X�|�C���^		*/
EXT	const KEY_CHG_TBL *	KcodeT_p;			/*	�R�[�h�E�ϊ��e�[�u���A�h���X�|�C���^	*/


/* LCD Driver Key Data  ->  ���� Key Code �ϊ��e�[�u�� */

/*----------------------------------------------------
   �\����
----------------------------------------------------*/
#ifdef _KEY_MAIN_C_
#if 1		/* �L�[�z�u�d�l�ύX */
const	BYTE	tkey_code_tbl[]= {
	CKEY_ID_KEY_OFF,		/*	D01	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D02	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D03	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D04	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D05	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_VOLUP,		/*	D06	�iVOLUP�j				*/
	CKEY_ID_LCD_AS,			/*	D07	�iAS�j					*/
	CKEY_ID_KEY_OFF,		/*	D08	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D09	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D10	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_SRC,		/*	D11	�iMODE�j				*/
	CKEY_ID_LCD_RIGHT,		/*	D12	�iUP�j					*/
	CKEY_ID_KEY_OFF,		/*	D13	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D14	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D15	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_VOLDW,		/*	D16	�iVOLDW�j				*/
	CKEY_ID_LCD_LEFT,		/*	D17	�iDW�j					*/
	CKEY_ID_KEY_OFF,		/*	D18	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D19	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D20	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_PWR,		/*	D21	�iPWR�j					*/
	CKEY_ID_KEY_OFF,		/*	D22	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D23	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D24	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D25	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_TI_SCAN,	/*	D26	�iTI/SCAN�j				*/
	CKEY_ID_LCD_CH,			/*	D27	�iCH�j					*/
	CKEY_ID_KEY_OFF,		/*	D28	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D29	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF			/*	D30	�i�Y���L�[�����j		*/
};
#else		/* �L�[�z�u�d�l�ύX */
const	BYTE	tkey_code_tbl[]= {
	CKEY_ID_KEY_OFF,		/*	D01	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D02	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D03	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D04	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D05	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_PWR,		/*	D06	�iPWR�j					*/
	CKEY_ID_LCD_AS,			/*	D07	�iAS�j					*/
	CKEY_ID_KEY_OFF,		/*	D08	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D09	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D10	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_SRC,		/*	D11	�iMODE�j				*/
	CKEY_ID_LCD_VOLUP,		/*	D12	�iVOLUP�j				*/
	CKEY_ID_KEY_OFF,		/*	D13	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D14	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D15	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_CH,			/*	D16	�iCH�j					*/
	CKEY_ID_LCD_VOLDW,		/*	D17	�iVOLDW�j				*/
	CKEY_ID_KEY_OFF,		/*	D18	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D19	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D20	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_RIGHT,		/*	D21	�iUP�j					*/
	CKEY_ID_KEY_OFF,		/*	D22	�i�Y���L�[�����j	*/
	CKEY_ID_KEY_OFF,		/*	D23	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D24	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D25	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_LEFT,		/*	D26	�iLEFT�j				*/
	CKEY_ID_LCD_TI_SCAN,	/*	D27	�iTI/SCAN�j				*/
	CKEY_ID_KEY_OFF,		/*	D28	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D29	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF			/*	D30	�i�Y���L�[�����j		*/
};
#endif		/* �L�[�z�u�d�l�ύX */
#else
extern const	BYTE	tkey_code_tbl[];
#endif

#ifdef _KEY_MAIN_C_

#if 1		/* �L�[�z�u�d�l�ύX */
const	BYTE	tkey_am_code_tbl[]= {
	CKEY_ID_KEY_OFF,		/*	D01	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D02	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D03	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D04	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D05	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_VOLUP,		/*	D06	�iVOLUP�j				*/
	CKEY_ID_KEY_OFF,		/*	D07	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D08	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D09	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D10	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D11	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_RIGHT,		/*	D12	�iUP�j					*/
	CKEY_ID_KEY_OFF,		/*	D13	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D14	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D15	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_VOLDW,		/*	D16	�iVOLDW�j				*/
	CKEY_ID_LCD_LEFT,		/*	D17	�iDW�j					*/
	CKEY_ID_KEY_OFF,		/*	D18	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D19	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D20	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_PWR,		/*	D21	�iPWR�j					*/
	CKEY_ID_KEY_OFF,		/*	D22	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D23	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D24	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D25	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_TI_SCAN,	/*	D26	�iTI/SCAN�j				*/
	CKEY_ID_LCD_CH,			/*	D27	�iCH�j					*/
	CKEY_ID_KEY_OFF,		/*	D28	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D29	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF			/*	D30	�i�Y���L�[�����j		*/
};
#else		/* �L�[�z�u�d�l�ύX */
const	BYTE	tkey_am_code_tbl[]= {
	CKEY_ID_KEY_OFF,		/*	D01	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D02	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D03	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D04	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D05	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_PWR,		/*	D06	�iPWR�j					*/
	CKEY_ID_KEY_OFF,		/*	D07	�iAS�j					*/
	CKEY_ID_KEY_OFF,		/*	D08	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D09	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D10	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D11	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_VOLUP,		/*	D12	�iVOLUP�j				*/
	CKEY_ID_KEY_OFF,		/*	D13	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D14	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D15	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_CH,			/*	D16	�iCH�j					*/
	CKEY_ID_LCD_VOLDW,		/*	D17	�iVOLDW�j				*/
	CKEY_ID_KEY_OFF,		/*	D18	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D19	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D20	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_RIGHT,		/*	D21	�iUP�j					*/
	CKEY_ID_KEY_OFF,		/*	D22	�i�Y���L�[�����j	*/
	CKEY_ID_KEY_OFF,		/*	D23	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D24	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D25	�i�Y���L�[�����j		*/
	CKEY_ID_LCD_LEFT,		/*	D26	�iLEFT�j				*/
	CKEY_ID_LCD_TI_SCAN,	/*	D27	�iTI/SCAN�j				*/
	CKEY_ID_KEY_OFF,		/*	D28	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF,		/*	D29	�i�Y���L�[�����j		*/
	CKEY_ID_KEY_OFF			/*	D30	�i�Y���L�[�����j		*/
};
#endif		/* �L�[�z�u�d�l�ύX */
#else
extern const	BYTE	tkey_am_code_tbl[];
#endif

#endif