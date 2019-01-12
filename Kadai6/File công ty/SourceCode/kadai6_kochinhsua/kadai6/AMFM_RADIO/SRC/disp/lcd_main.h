/***************************************************************
	�N�����I���a����       
	�v���W�F�N�g��	�F'07�N���f���J��
	�t�@�C����		�Flcd_main.h
	���W���[����	�FLCD(�\��)
	�@   �\			�F�\�����W���[�����C����`ͯ��̧��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2006/3/8		0.1		��		�V�K�쐬
[070302he]	�ݼ޹���_�Őݒ�ύX
			LEVEL�ݒ�폜
****************************************************************/
#ifndef		_LCD_MAIN_H_
#define		_LCD_MAIN_H_

#undef	EXT
#ifdef	_LCD_MAIN_C_
#define	EXT   
#else
#define	EXT	extern
#endif


/*-----------------�׸ޒ�`----------------------*/
EXT	TCOM_FLG	lcd_flag1;
#define		flcd_send			lcd_flag1.bit.bit0		/* ���M�v���׸� 0=���M�v���Ȃ� 1=���M�v�� */
#define		flcd_blank			lcd_flag1.bit.bit1		/* ���ݸ���M�v�� */

EXT	TCOM_FLG	lcd_flag2;

EXT	TCOM_FLG	lcd_flag3;
#define	flcd_blk_clr		lcd_flag3.bit.bit0		/* �_�Ÿر�v�� 1= �v������,0=�v������ */

EXT	TCOM_FLG	lcd_flag4;
#define	flcd_test_mode		lcd_flag4.bit.bit0		/* �e�X�g���[�h�p�@1=�e�X�g���[�h�@0=�@�e�X�g���[�h�ȊO */

EXT	TCOM_FLG	lcd_flag5;
#define	flcd_accoff_send	lcd_flag5.bit.bit0		/* accoff���̍X�V�v�� */

/*---------------�萔��`-----------------------*/
#define		CLCD_TIMECNT_2S			40			/* �_�����2000/50 */
#define		CLCD_TIMECNT_1S			20			/* �_�����1000/50 */
#define		CLCD_TIMECNT_500MS		10			/* �_�����500/50 */

#define		CLCD_ACCON_EDIT_TIM		5



/* �����_�� */
enum
{
	CLCD_NONE = 0,
	CLCD_BLK_FREQ,
	CLCD_BLK_CLOCK,
	CLCD_BLK_ALARM,
	CLCD_BLK_PSNO,
	CLCD_BLK_MAX
};
/*---------------�ϐ���`----------------------*/
#define		BLK_500MS				0
#define		BLK_1S					1
EXT BYTE lcd_tim_blk;				/* �_����� */
EXT BYTE lcd_tim_blkcolon;			/* �_����� */
EXT BYTE lcd_tim_pwr_wait;			/* PWR_CNT_ON 500ms�҂���� */
EXT BYTE lcd_ind_blk;				/* �ݼ޹���_��Ӱ�� */
EXT BYTE lcd_blk_mode;				/* �_��Ӱ�� */
EXT BYTE lcd_blk_type;				/* �_����ώ�� */
EXT BYTE lcd_edit_tim;				/* �ҏW��� */
EXT BYTE lcd_send_tim;				/* ���M��� */

/*-----------------------------------------------
		�֐���������
-----------------------------------------------*/
EXT void Lcd_main(void);
EXT void Lcd_initial(void);
EXT void Lcd_ram_initial(void);
EXT void Lcd_driver_send( void );
EXT void Lcd_indictrol( void );
EXT void Lcd_blink(void);
EXT void Lcd_blink_set(BYTE, BYTE);
EXT void Lcd_25ms_cnt(void);
EXT void Lcd_50ms_cnt(void);
EXT void Lcd_100ms_cnt(void);
EXT void lcd_moji_romcpy(BYTE *savebuf, BYTE *addfrom);
EXT void Lcd_hextoascii( BYTE, BYTE * );
EXT void Lcd_nodisp( BYTE *, BYTE);


#endif
