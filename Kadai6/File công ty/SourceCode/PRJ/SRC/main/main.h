/******************************************************************************
..     �N�����I���a����                    2007����

      �v���W�F�N�g��   : 
      �t�@�C����      : ma_main.h
      �@�\         : �@Ҳ�Ӽޭ��ͯ��̧��
------------------------------------------------------------------------------
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051128]   �V�K�쐬
******************************************************************************/
#ifndef      _MAIN_H_
#define      _MAIN_H_

#undef   EXT
#ifdef   _MAIN_C_
#define   EXT       
#else
#define   EXT   extern
#endif


/*----------------------------------------------------
   �֐��v���g�^�C�v
----------------------------------------------------*/
void   Ma_message_set(BYTE, BYTE );

/*----------------------------------------------------
   �ϐ���`
----------------------------------------------------*/
#define      CMA_MSGTBL_END   0xff
#define      CMA_MSGMAX      50
EXT   BYTE   ma_sysmsg[CMA_MSGMAX];         /* ү���ފi�[�ޯ̧   Bo dem luu tru message*/
EXT   BYTE   ma_msg_inptr;               /* ү���ފi�[�߲��       Con tro luu tru message*/
EXT   BYTE   ma_msg_outptr;               /* ү���ޔ��s�߲��       Con tro phat hanh message*/
EXT   BYTE   ma_msg_seq;                  /* ү���ޔ��s�������ݽ     Trinh tu xu li viec phat hanh message*/
EXT   BYTE   ma_cur_msg;                  /* ���s���̼���ү����    System message trong qua trinh ph�t h�nh */
#endif
