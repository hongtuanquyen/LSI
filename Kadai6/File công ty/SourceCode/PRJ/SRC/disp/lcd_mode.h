/***************************************************************
   �N�����I���a����    
   �v���W�F�N�g��   �F'12�N���f���J��
   �t�@�C����      �Flcd_mode.h
   ���W���[����   �FLCD(�\��)
   �@   �\         �F�\�����W���[��Ӱ�ޕ����`ͯ��̧��
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/08/01      0.1      ha      �V�K�쐬
****************************************************************/
#ifndef      _LCD_MODE_H_
#define      _LCD_MODE_H_

#undef   EXT
#ifdef   _LCD_MODE_C_
#define   EXT   
#else
#define   EXT   extern
#endif
/* �e�\��Ӱ�ޕ\���ʒu�萔��` */
/* ���Mں��� */
/* �V�K--------start-------- */
#define      RECORD1         0
#define      RECORD2         8
#define      RECORD3         16
#define      SENDBYTE      8
/* �V�K--------end--------- */

/* VER�\�� */
#define      VER                     6
/* SELӰ�� */
#define      SELVOL                  2
#define      SELBAS                  2
#define      SELTRE                  2
#define      SELBAL                  2
#define      SUBVOL                  5
/* ���vӰ�� */
#define      CLKMIN                  5
#define      CLKSEC                  7
/* �޲ڸ�Ӱ�� */
#define      DIRECT                  5
/* TUNERӰ�� */
#define      FRQ                     0
#define      FMFRQ                  0
#define      AMFRQ                  0
#define      PRENO                  4


EXT BYTE lcd_seg_buf[5];         /* �����ޯ̧    Character buffer */

/*-----------------------------------------------
      �֐���������
-----------------------------------------------*/
EXT void Lcd_mode(void);
EXT void Lcd_test_mode(void);
EXT void Lcd_clock_mode(void);
EXT void Lcd_area_disp(void);


#endif
