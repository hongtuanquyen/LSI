/***************************************************************
	�N�����I���a����       
	�v���W�F�N�g��	�F'12�N���f���J��
	�t�@�C����		�Flcd_sys.h
	���W���[����	�FLCD(�\��)
	�@   �\			�F��`ͯ��̧��
----------------------------------------------------------------
�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
			2012/07/30		0.1		ha		�V�K�쐬
****************************************************************/
#ifndef		_LCD_SYS_H_
#define		_LCD_SYS_H_

#undef	EXT
#ifdef	_LCD_SYS_C_
#define	EXT   
#else
#define	EXT	extern
#endif


/*---------------����ؑ�----------------------*/
enum
{
	CLCD_SRC_TUNER = 1,
	CLCD_SRC_ISR,
	CLCD_SRC_AUX,
	CLCD_SRC_MAX
};



/*-------------�f���ꎞ�\��-------------------*/
#define		CLCD_OPEN				1
#define		CLCD_CLOSE				2

#define		CLCD_SRC_TIM			20
#define		CLCD_SRCTA_TIM			70			/* 7000/100 */

EXT BYTE lcd_src_mode;					/* �����ݼވꎞ�\��Ӱ�� */
EXT BYTE lcd_src_tim;					/* �����ݼވꎞ�\����� */
EXT BYTE lcd_src_rdsint;

EXT	void Lcd_sys_message( BYTE);
EXT void Lcd_1shot_src(void);

#endif
