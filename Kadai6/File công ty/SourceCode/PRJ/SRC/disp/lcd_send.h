/***************************************************************
   ���^�i�V���d�@�a����        2007  C3XXX�V���[�Y
   �v���W�F�N�g��   �F'07�N�t���b�v���f���J��
   �t�@�C����      �Flcd_send.h
   ���W���[����   �FLCD(�\��)
   �@   �\         �F��`ͯ��̧��
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2012/07/30      0.1      ha      �V�K�쐬
****************************************************************/

#ifndef      _LCD_SEND_H_
#define      _LCD_SEND_H_

#undef   EXT
#ifdef   _LCD_SEND_C_
#define   EXT   
#else
#define   EXT   extern
#endif

typedef union
 {
   #if 0
   unsigned char buf_byte[32];
   #endif
   unsigned char buf_byte[24];

   struct         /* ����    Characters */
   {
/* 1ں��� Lan gui thu 1 (Record 1) */
      BYTE         :1;
      BYTE   :      1;
      BYTE   :      1;
      BYTE   :      1;
      BYTE   :      1;
      BYTE   :      1;
      BYTE   :      1;
      BYTE   seg1_b:      1;

      BYTE   seg1_c:      1;
      BYTE   seg1_h:      1;
      BYTE   seg1_i:      1;
      BYTE   seg1_g:      1;
      BYTE   seg1_a:      1;
      BYTE   seg1_f:      1;
      BYTE   seg1_e:      1;
      BYTE   seg1_d:      1;

      BYTE   seg2_c      :1;
      BYTE   seg2_h:      1;
      BYTE   :      1;
      BYTE   seg2_b:      1;
      BYTE   seg2_g:      1;
      BYTE   seg2_a:      1;
      BYTE   seg2_f:      1;
      BYTE   seg2_e:      1;

      BYTE         :1;
      BYTE   seg2_d:      1;
      BYTE   :   1;
      BYTE   :   1;
      BYTE   seg3_c:      1;
      BYTE   seg3_h:      1;
      BYTE   seg3_b:      1;
      BYTE   seg3_i:      1;
      
      BYTE   seg3_g:      1;
      BYTE   seg3_a:      1;
      BYTE   seg3_f:      1;
      BYTE   seg3_e:      1;
      BYTE   :   1;
      BYTE   seg3_d:      1;
      BYTE   seg4_c:      1;
      BYTE   seg4_b:      1;
      
      BYTE   seg4_f:      1;
      BYTE   seg4_g:      1;
      
      BYTE    :   2;
      BYTE   ctrl_1:      4;
      
      BYTE   ctrl_2:      8;
      BYTE   ctrl_3:      8;
      
      
/* 2ں���  Lan gui thu 2 (Record 2)*/      
      BYTE   seg4_a:      1;
      BYTE   seg4_d:      1;
      BYTE   seg4_e:      1;
      BYTE   :   1;
      BYTE   seg5_b:      1;
      BYTE   seg5_c:      1;
      BYTE   seg5_a:      1;
      BYTE   seg5_g:      1;
      
      BYTE   seg5_d:      1;
      BYTE   :   1;
      BYTE   seg5_f:      1;
      BYTE   seg5_e:      1;
      BYTE   :   4;
      
      BYTE   dummy16[4];
      BYTE   reco_2[2];

/* 3���R�[�h */
      BYTE   dummy17[6];
      BYTE   reco_3[2];
      
   }record;
   
   struct         /* �ݼ޹��  Indicator */
   {
/* 1ں��� */
      
      BYTE   fm      :1;      /* "FM"      */
      BYTE   pm      :1;      /* "PM"      */
      BYTE   ti      :1;      /* "�E)))"   */
      BYTE   one      :1;      /* "1"      */
      BYTE   am      :1;      /* "AM"      */
      BYTE   s1      :1;      /* "1"      */
      BYTE   two      :1;      
      BYTE         :1;

      BYTE         :8;
      BYTE         :8;
      
      BYTE   st      :1;      /* "ST"      */
      BYTE         :1;
      BYTE   dot      :1;      /* "."      */
      BYTE   colon  :1;      /* ":"      */
      BYTE         :4;
      
      BYTE         :8;
      BYTE         :8;
      
      BYTE         :3;
      BYTE         :1;
      BYTE         :4;
      
      BYTE         :8;
      
/* 2ں��� */
      BYTE         :8;
      
      BYTE         :1;
      BYTE   al      :1;
      BYTE         :6;
      
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      
/* 3ں��� */
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
      BYTE         :8;
   }record1;
   
   struct{
/* 1ں��� */
      BYTE   d1      :1;
      BYTE   d2      :1;
      BYTE   d3      :1;
      BYTE   d4      :1;
      BYTE   d5      :1;
      BYTE   d6      :1;
      BYTE   d7      :1;
      BYTE   d8      :1;
      
      BYTE   d9      :1;
      BYTE   d10      :1;
      BYTE   d11      :1;
      BYTE   d12      :1;
      BYTE   d13      :1;
      BYTE   d14      :1;
      BYTE   d15      :1;
      BYTE   d16      :1;

      BYTE   d17      :1;
      BYTE   d18      :1;
      BYTE   d19      :1;
      BYTE   d20      :1;
      BYTE   d21      :1;
      BYTE   d22      :1;
      BYTE   d23      :1;
      BYTE   d24      :1;

      BYTE   d25      :1;
      BYTE   d26      :1;
      BYTE   d27      :1;
      BYTE   d28      :1;
      BYTE   d29      :1;
      BYTE   d30      :1;
      BYTE   d31      :1;
      BYTE   d32      :1;

      BYTE   d33      :1;
      BYTE   d34      :1;
      BYTE   d35      :1;
      BYTE   d36      :1;
      BYTE   d37      :1;
      BYTE   d38      :1;
      BYTE   d39      :1;
      BYTE   d40      :1;
      
      BYTE   d41      :1;
      BYTE   d42      :1;
      
      BYTE   td1      :6;
      BYTE   td2[2];
      
/* 2ں��� */
      BYTE   d43      :1;
      BYTE   d44      :1;
      BYTE   d45      :1;
      BYTE   d46      :1;
      BYTE   d47      :1;
      BYTE   d48      :1;
      BYTE   d49      :1;
      BYTE   d50      :1;
      
      BYTE   d51      :1;
      BYTE   d52      :1;
      BYTE   d53      :1;
      BYTE   d54      :1;
      
      BYTE   td3      :4;
      BYTE   td4[6];
      
/* 3ں��� */
      BYTE   td5[8];
   }record_test;
}UPW_RECORD;

EXT UPW_RECORD   lcd_send_buf;      /* ���M�o�b�t�@ */

typedef      union
{
   unsigned char   font_buf[2];
   
   struct{
         BYTE   buf_f      :   1;
         BYTE   buf_a      :   1;
         BYTE   buf_g      :   1;
         BYTE   buf_i      :   1;
         BYTE   buf_h      :   1;
         BYTE   buf_b      :   1;
         BYTE   buf_c      :   1;
         BYTE   buf_0      :   1;

         BYTE   buf_e      :   1;
         BYTE   buf_d      :   1;
         BYTE   buf_1      :   1;
         BYTE   buf_2      :   1;
         BYTE   buf_3      :   1;
         BYTE   buf_4      :   1;
         BYTE   buf_5      :   1;
         BYTE   buf_6      :   1;
         }font;
}UPW_FONT_1;

EXT   UPW_FONT_1   lcd_font1_buf;


typedef      union
{
   unsigned char   font_buf[2];
   
   struct{
         BYTE   buf_f      :   1;
         BYTE   buf_a      :   1;
         BYTE   buf_g      :   1;
         BYTE   buf_b      :   1;
         BYTE   buf_0      :   1;
         BYTE   buf_h      :   1;
         BYTE   buf_c      :   1;
         BYTE   buf_7      :   1;

         BYTE   buf_e      :   1;
         BYTE   buf_1      :   1;
         BYTE   buf_d      :   1;
         BYTE   buf_2      :   1;
         BYTE   buf_3      :   1;
         BYTE   buf_4      :   1;
         BYTE   buf_5      :   1;
         BYTE   buf_6      :   1;
         }font;
}UPW_FONT_2;

EXT   UPW_FONT_2   lcd_font2_buf;

typedef      union
{
   unsigned char   font_buf[2];
   
   struct{
         BYTE   buf_f      :   1;
         BYTE   buf_a      :   1;
         BYTE   buf_g      :   1;
         BYTE   buf_i      :   1;
         BYTE   buf_b      :   1;
         BYTE   buf_h      :   1;
         BYTE   buf_c      :   1;
         BYTE   buf_0      :   1;

         BYTE   buf_e      :   1;
         BYTE   buf_d      :   1;
         BYTE   buf_1      :   1;
         BYTE   buf_2      :   1;
         BYTE   buf_3      :   1;
         BYTE   buf_4      :   1;
         BYTE   buf_5      :   1;
         BYTE   buf_6      :   1;
         }font;
}UPW_FONT_3;

EXT   UPW_FONT_3   lcd_font3_buf;

typedef      union
{
   unsigned char   font_buf[2];
   
   struct{
         BYTE   buf_d      :   1;
         BYTE   buf_a      :   1;
         BYTE   buf_g      :   1;
         BYTE   buf_f      :   1;
         BYTE   buf_b      :   1;
         BYTE   buf_c      :   1;
         BYTE   buf_7      :   1;
         BYTE   buf_8      :   1;

         BYTE   buf_e      :   1;
         BYTE   buf_0      :   1;
         BYTE   buf_1      :   1;
         BYTE   buf_2      :   1;
         BYTE   buf_3      :   1;
         BYTE   buf_4      :   1;
         BYTE   buf_5      :   1;
         BYTE   buf_6      :   1;
         }font;
}UPW_FONT_4;

EXT   UPW_FONT_4   lcd_font4_buf;

typedef      union
{
   unsigned char   font_buf[2];
   
   struct{
         BYTE   buf_f      :   1;
         BYTE   buf_e      :   1;
         BYTE   buf_a      :   1;
         BYTE   buf_g      :   1;
         BYTE   buf_d      :   1;
         BYTE   buf_b      :   1;
         BYTE   buf_c      :   1;
         BYTE   buf_8      :   1;

         BYTE   buf_7      :   1;
         BYTE   buf_0      :   1;
         BYTE   buf_1      :   1;
         BYTE   buf_2      :   1;
         BYTE   buf_3      :   1;
         BYTE   buf_4      :   1;
         BYTE   buf_5      :   1;
         BYTE   buf_6      :   1;
         }font;
}UPW_FONT_5;

EXT   UPW_FONT_5   lcd_font5_buf;
/*---------------------------------------------------
      �֐��֐���������
-----------------------------------------------------*/
EXT void Lcd_edit( void );
EXT void Lcd_setseg_1( void );
EXT void Lcd_setseg_2( void );
EXT void Lcd_setseg_3( void );
EXT void Lcd_setseg_4( void );
EXT void Lcd_setseg_5( void );
EXT void Lcd_test_edit(void);
EXT void Lcd_send( void );

#endif
