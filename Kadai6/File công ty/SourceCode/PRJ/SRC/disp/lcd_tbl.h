/***************************************************************
   �N�����I���a����        2007  C3X03�V���[�Y
   �v���W�F�N�g��   �F'07�N�t���b�v���f���J��
   �t�@�C����      �Flcd_tbl.h
   ���W���[����   �FLCD(�\��)
   �@   �\         �F�\���e�[�u����`ͯ��̧��
----------------------------------------------------------------
�C������   �y�N�����z�yVersion�z�y���O�z �y�����z
----------------------------------------------------------------
         2005/11/28      0.1      ��      �V�K�쐬
[060719jyo]USBӰ�ޕ�����ǉ�
****************************************************************/
#ifndef      _LCD_TBL_H_
#define      _LCD_TBL_H_

#undef   EXT
#ifdef   _LCD_MAIN_C_
#define   EXT   
#else
#define   EXT   extern
#endif


#ifdef   _LCD_MODE_C_

/* VER */
#define MAIN_VER      0
const BYTE CLCD_SRC_TBL[][6] = {
                                 {" AU   "},
                                    };
/*----------------���v������---------------------*/
#define      ADJ         3
#define      CT         2
#define      AM         1
#define      PM         0
const BYTE CLCD_CLK_TBL[][10] = {
                                 {" PM       "},         /* PM                */
                                 {" AM       "},         /* AM                */   
                                 {" CT       "},         /* CT                */
                                 {" CT  ---- "}         /* ADJUST             */
                                    };
const BYTE CLCD_ADJCLK_TBL[][10] = {
                                 {" PM       "},         /* PM                */
                                 {" AM       "}         /* AM                */   
                                    };
/*----------------AUX------------------------*/
#define         STR_AUX         "AU   "

/*-----------------SELӰ��ð���------------------*/


const BYTE CLCD_VOL_TBL[] = {"Vo   "};

const BYTE CLCD_BAS_TBL[] = {"BA   "};
const BYTE CLCD_TRE_TBL[] = {"TR   "};
const BYTE CLCD_BAL_TBL[] = {"BL   "};
enum
{
   VOL=0,
   BAS,
   TRE,
   BAL_C,
   BAL
};


const BYTE CLCD_SEL_TBL[][5] = {
                                 {"Uo   "},
                                 {"BA   "},      /* BAS   dB */
                                 {"TR   "},      /* TRE   dB */
                                 {"BL 0 "},
                                 {"BL   "},
                                    };
#endif

#endif
