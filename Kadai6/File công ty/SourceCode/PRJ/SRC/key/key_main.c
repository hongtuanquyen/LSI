/******************************************************************************
   �N�����I���a����
   �v���W�F�N�g��   : 2012�NAMFM_RADIO���f��
   �t�@�C����      : key_main.c
   �@   �\         : �L�[���̓��C������
                 �L�[���W���[�����[�J���T�u�֐�
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
#define   _KEY_MAIN_C_

#include   "string.h"

#include   "../comm/common.h"
#include   "key_def.h"
#include   "key_func_ext.h"
#include   "key_sys.h"
#include   "key_main.h"

#include   "../io/IO_inc.h"
#include   "../main/ma_ext.h"
#include   "../power/pw_func_ext.h"
#include   "../tuner/tu_key.h"
#include   "../aux1/aux_key.h"
#include   "../off/off_key.h"
#include   "../disp/lcd_ext.h"
#include   "../submode/sbm_ext.h"
#include   "../submode/sbm_def.h"
#include   "../testmode/TEST_inc.h"

/*   ----------------------------------------*/
/*   �֐��v���g�^�C�v�錾�i���[�J���֐��j   */
/*   ----------------------------------------*/

/*   �k�b�c�L�[�̓��͔��菈��            */
static   void   Key_lcd_keyin_check(void);

/*   �`�k�`�q�l�L�[�̓��͔��菈��            */
static   void   Key_alarmkey_check(void);

/*   ���̓L�[�R�[�h�̊m����s���B         */
static   void   Key_CodeCheck_func(void);

/*   ���̓L�[�ɑΉ����������̋N�����s���B   */
static   void   Key_code_process(void);

/*   LCD Key�̉�����Ă���L�[�R�[�h���`�F�b�N����B*/
static   BYTE   Key_lcd_key_data_check(void);

/*   ���̓L�[�ɑ΂��铮�쏈�����N������B   */
static   BYTE    Key_code_sub_func(void);

/*   Key Data ��M����                  */
static   BYTE   Key_DataRecive( void );

static void Key_status_check(void);
static BYTE   Key_parameter_get(void);

#if (CKEY_TEST_KEY == YES)
static BYTE   Key_code_search(BYTE , BYTE   *);
#endif
static void Key_test_key_chk(void);

static void   key_mem_keyjet(BYTE keyjet);

/******************************************************************************
   �֐���      �FKey_main
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FKey���W���[���E���C������
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
void   Key_main(void)
{
   Key_lcd_keyin_check();         /*   LCD Key���̓`�F�b�N����   Check xem co yeu cau tu LCD driver ko. Neu co, check xem key data nhan duoc gom nhung key gi      */
   Key_alarmkey_check();          /*   Luu y key ALARM la key rieng. Check xem che do ALARM co bi error, neu ko thi check xem key co duoc nhan chua.
                                       check fkey_alarm_key.*/
   Key_test_key_chk();            /*   ????    ko hieu */
   Key_CodeCheck_func();         /*   Key �m�菈��               Kiem tra xem co phai 2 key tro len dc nhan hay ko(CKEY_ID_MULTIPLEX). 
                                                               Neu dung, ko nhan tat ca key(key_new_key = CKEY_ID_KEY_OFF)*/
   Key_status_check();            /*  Key��ԃ`�F�b�N����       Key state check processing        */
   Key_code_process();            /*  Key���򏈗�                  */
}

/******************************************************************************
   �֐���   �FKey_initial
   ��  ��   �F����
   �߂�l   �F����
   �@  �\   �FKey���W���[���ϐ�����������
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
void   Key_initial(void)
{
   /*   LCD�L�[�N���A */
   key_lcd_input = CKEY_ID_KEY_OFF;   /* LCD Key Code Memory area   CKEY_ID_KEY_OFF = 0*/
   key_lcd_input_Timer = CKEY_TIME_UP; /* CKEY_TIME_UP */

   /* �L�[��ԃ`�F�b�N�p�ϐ��N���A   Key status check variable clear */
   key_old_key = 0;   /* �O��m�� Key Code */
   key_new_key = 0;   /* ����m�� Key Code */
   key_now_key = 0;
   key_long_timer = 0xff;   /* Long Key �m��p�^�C�}�J�E���^     Long key confirmation timer counter*/
   
   key_before_key = 0;
   key_test_code = 0;
   key_test_sta = CKEY_TSET_NOP;  // CKEY_TSET_NOP = 0
}

/******************************************************************************
   �֐���   �FKey_lcd_keyin_check
   ��  ��   �F����
   �߂�l   �FKey�R�[�h
   �@  �\   �FLCD���͏���
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
static   void   Key_lcd_keyin_check(void)
{
   if (key_lcd_input_Timer == CKEY_TIME_UP)   // CKEY_TIME_UP = 0
   {
      key_lcd_input_Timer = CKEY_IN_TIME50ms;
      
      if(MIO_IN_pLCDDI() == LOW)   /*   LCD Key on ?  MIO_IN_pLCDDI() trong file IO_ext.h, Kiem tra truc tiep Port 1 pin 0, xem file LC75857E tr18
                                        Neu la LOW tuc la nhan duoc yeu cau gui key data tu LCD driver*/
      {
         if(Key_DataRecive() !=NG )  // Neu viec nhan ko gap error, nhan du 30bit key data
         {
            key_lcd_input = Key_lcd_key_data_check(); //key_lcd_input = CKEY_ID_KEY_OFF(ko co key duoc nhan) hoac input_code[ 0 ](chi co 1 key duoc nhan)
                                                      //                 hoac CKEY_ID_MULTIPLEX(nhieu key duoc nhan)
                                                      //                 hoac CKEY_ID_LCD_TEST1 / TEST2/ TEST3 
         }
         if (key_test_sta == CKEY_TSET_INPUT)
         {
            key_test_sta = CKEY_TSET_REPLY;
            memcpy(key_t_data,key_lcd_driver_data,sizeof(key_t_data)); 
         }
      }
      else  // chua c� yeu cau gui key data tu LCD driver
      {
         key_lcd_input = CKEY_ID_KEY_OFF;
         if (key_test_sta == CKEY_TSET_INPUT)
         {
            key_test_sta = CKEY_TSET_REPLY;
            memset(key_t_data,0x00,sizeof(key_t_data));
         }
      }
   }
}

/******************************************************************************
   �֐���   �FKey_alarmkey_check
   ��  ��   �F����
   �߂�l   �FKey �R�[�h
   �@  �\   �FAlarmKey�ǂݍ��ݏ���
------------------------------------------------------------------------------
[chin20120817]   �V�K�쐬
******************************************************************************/
void   Key_alarmkey_check(void)
{
   if (Ma_model_dest()== CMA_MODEL_AM_J)
   {
      fkey_alarm_key = CKEY_ID_KEY_OFF;
   }
   else
   {
      fkey_alarm_key = Pw_ret_almkey();
   }
}
/******************************************************************************
   �֐���   �FKey_CodeCheck_func
   ��  ��   �F����
   �߂�l   �FKey �R�[�h
   �@  �\   �F���̓L�[�R�[�h�̊m�菈��
   �R�����g�FLCD�L�[�R�[�h������ɃL�[�m����s���B
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
static   void   Key_CodeCheck_func(void)
{
   BYTE   iKeyCnt;
   BYTE   iKeyCode;
   
   if (key_in_disable_timer != CKEY_TIME_UP ) // CKEY_TIME_UP = 0,  duoc gan moi trong key_check_stop(), va ham key_check_stop() duoc goi trong Pw_power_key()
   {
      fkey_alldis = ON;      // all key disabled
      fkey_nop = ON;
   }
   fkey_prio_key = ON;
   if (key_lcd_input == CKEY_ID_LCD_PWR)
   {
      key_new_key = key_lcd_input;
   }
   else
   {
      fkey_prio_key = OFF;
      fkey_alldis = OFF;
      iKeyCnt = 0;
      iKeyCode = CKEY_ID_KEY_OFF;
      
      if (fkey_alarm_key == ON)
      {
         iKeyCode = CKEY_ID_LCD_ALARM;
         iKeyCnt++;
      }
      if (key_lcd_input != CKEY_ID_KEY_OFF)
      {
         iKeyCode = key_lcd_input;
         iKeyCnt++;
      }
      if ((iKeyCnt > 1) || (iKeyCode == CKEY_ID_MULTIPLEX))
      {
         key_new_key = CKEY_ID_KEY_OFF;
         fkey_nop = ON;                  /* ���d�L�[�����ꂽ */
      }
      else if (iKeyCnt == 1)               /* ��L�[ */
      {
         key_new_key = iKeyCode;            /* �L�[�m�� */
      }
      else                           /* �L�[������ĂȂ� */
      {
         key_new_key = CKEY_ID_KEY_OFF;      /* key off �m�� */
         fkey_nop = OFF;
      }
   }
   
   if ( fkey_alldis == ON )                  /* ALL Key�֎~��    ALL Key In Prohibited */
   {
      key_new_key = CKEY_ID_KEY_OFF;            /* �L�[OFF�Ƃ���     Set key to OFF*/
   }
   else if ((fkey_nop == ON) && (fkey_prio_key == OFF))
   {
      key_new_key = CKEY_ID_KEY_OFF;            /* �L�[OFF�Ƃ���     Set key to OFF*/
   }
   else
   {
      ;
   }
}
/******************************************************************************
   �֐���   �FKey_lcd_key_data_check
   ��  ��   �F����
   �߂�l   �FKey�R�[�h
   �@  �\   �FLCD���̓f�[�^�`�F�b�N�����B
   �R�����g�FLCD�L�[�̉�����Ă���L�[�R�[�h���`�F�b�N����B
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
static   BYTE   Key_lcd_key_data_check(void)
{
   BYTE   key_code_cnt;         /*   Key Driver Data Total �J�E���^    Key Driver Data Total Counter   */
   BYTE   return_cd;            /*   �߂�l�i�[�ϐ�   Bien so chua gia tri tra ve */
   BYTE   key_driver_data;      /*   Key Driver Data Byte  �C���f�b�N�X   Key Driver Data Byte index*/
   BYTE   key_d_bit_cnt;        /*  Key Driver Data Bit �C���f�b�N�X   Key Driver Data Bit index*/
   BYTE   loop_cnt1;            /*   Loop �J�E���^         Loop Counter         */
   BYTE   loop_cnt2;            /*   Loop �J�E���^         Loop Counter         */
   BYTE   input_code[3];         /*  Test��3���d�L�[�Ή��̂��߁A�o�b�t�@�g�� De ma dap ung duoc nhieu key cua Test, mo rong buffer*/
   BYTE   dest;
   
   dest = Ma_model_dest();
   input_code [0] = CKEY_ID_KEY_OFF;   /*   Key code Save aria  CKEY_ID_KEY_OFF = 0  */
   input_code [1] = CKEY_ID_KEY_OFF;   /*   Key code Save aria   */
   input_code [2] = CKEY_ID_KEY_OFF;   /*   Key code Save aria   */
   
   key_code_cnt = 0;
   key_d_bit_cnt = 0;
   key_lcd_driver_data[3] &= 0x03f; // 0x03f = 0x3f = 00111111, ro rang byte thu 4 bo 2 bit cuoi. nen nho la gui data theo LSB. Xem tkey_code_tbl[] trong key_main.h
  
   /*   Chuc nang vong lap for loop_cnt1: quet 4 byte(32bit) duoc gui tu LCD qua VDK
        Chuc nang vong lap for loop_cnt2: quet tung bit cua 1 byte
          Neu bit do & 0x01 = 0x01, tuc bit giu chuc nang cua 1 key nao do (Xem tkey_code_tbl[] hoac tkey_am_code_tbl[])
          Sau do no se dua vao mang input_code. Tong so bit key bit la 3
   */
   for (loop_cnt1 = 0;loop_cnt1 < CKEY_LCD_DATA_BYTE;loop_cnt1++) // CKEY_LCD_DATA_BYTE = 4;
   {
      key_driver_data = key_lcd_driver_data[loop_cnt1];
      for (loop_cnt2 = 0;loop_cnt2 < 8;loop_cnt2++)
      {
         if ((key_driver_data & 0x01) == 0x01)
         {
            key_code_cnt++;
            if (key_code_cnt > 3)   /* 3Key �� 4Key�ȏ�Ȃ� Key�����ɕύX */
            {
               break;
            }
            if (dest == CMA_MODEL_AM_J)  // chi co AM
            {
               input_code[ key_code_cnt-1 ] = tkey_am_code_tbl[key_d_bit_cnt]; // tkey_am_code_tbl trong file key_main.h
            }
            else
            {
               input_code[ key_code_cnt-1 ] = tkey_code_tbl[key_d_bit_cnt];
            }
         }
         key_driver_data = (key_driver_data >> 1);
         key_d_bit_cnt++;
      }
   }

   if (key_code_cnt == 0 )
   {
      return_cd = CKEY_ID_KEY_OFF; 
   }
   else if (key_code_cnt == 1 )
   {
      return_cd = input_code[ 0 ];
   }
   #if (CKEY_TEST_KEY == YES) // CKEY_TEST_KEY = 1, trong file key_def.h
   else if (key_code_cnt == 2 )
   {
      if (Key_code_search(CKEY_ID_LCD_PWR, input_code) == TRUE)
      {
         key_before_key = CKEY_ID_KEY_OFF;
         return_cd = CKEY_ID_LCD_PWR;
      }
      else
      {
         return_cd = Key_code_search(CKEY_ID_LCD_CH, input_code);
         if (return_cd == TRUE)
         {
            if (Key_code_search(CKEY_ID_LCD_RIGHT, input_code) == TRUE)
            {
               key_before_key = CKEY_ID_LCD_TEST1;
               return_cd = key_before_key;
            }
            else if (Key_code_search(CKEY_ID_LCD_LEFT, input_code) == TRUE)
            {
               key_before_key = CKEY_ID_LCD_TEST2;
               return_cd = key_before_key;
            }
            else if (Key_code_search(CKEY_ID_LCD_VOLUP, input_code) == TRUE)
            {
               key_before_key = CKEY_ID_LCD_TEST3;
               return_cd = key_before_key;
            }
            else
            {
               return_cd = CKEY_ID_MULTIPLEX;
               fkey_nop = ON;
               key_before_key = CKEY_ID_KEY_OFF;
            }
         }
         else
         {
            return_cd = CKEY_ID_MULTIPLEX;
            fkey_nop = ON;
            key_before_key = CKEY_ID_KEY_OFF;
         }
      }
   }
   else if (key_code_cnt == 3 )
   {
      if (( key_before_key >= CKEY_ID_LCD_TEST1 )&&( key_before_key <= CKEY_ID_LCD_TEST3 ))  
      {
         return_cd = Key_code_search(CKEY_ID_LCD_PWR, input_code);
         if (return_cd == TRUE)
         {
            return_cd = key_before_key + 3;         /* �B���L�[�m��   Confirm hidden key  */
            fkey_nop = OFF;
         }
      }
      else
      {
         key_before_key = CKEY_ID_KEY_OFF;
         return_cd = CKEY_ID_MULTIPLEX;
         fkey_nop = ON;
      }
   }
   #endif
   else
   {
      return_cd = CKEY_ID_MULTIPLEX;
      fkey_nop = ON;
   }
   return ( return_cd );
}

/******************************************************************************
   �֐���   �FKey_code_search
   ��  ��   �Fcode �� �T�[�`���̃L�[�R�[�h
         �F*input_key �� LCD�L�[�R�[�h�i�[�o�b�t�@�擪�A�h���X
   �߂�l   �FTRUE �� �w��L�[�R�[�h�ALCD�L�[�R�[�h�o�b�t�@�Ɋi�[����Ă���
         �FFALSE �� �w��L�[�R�[�h�A�i�[����Ă��Ȃ�
   �@  �\   �F�w�跰LCD�L�[�R�[�h�i�[�o�b�t�@�ɂ��邩�ǂ����T�[�`����
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
#if (CKEY_TEST_KEY == YES)
static BYTE   Key_code_search(BYTE   code, BYTE   *input_key)
{
   BYTE   iCnt;
   BYTE   iRet;

   iRet = FALSE;
   for (iCnt = 0; iCnt < 3; iCnt ++)
   {
      if ((*input_key) == code)
      {
         iRet = TRUE;
         break;
      }
      input_key++;
   }
   return(iRet);
}
#endif
/******************************************************************************
   �֐���   �FKey_parameter_get
   ��  ��   �F����
   �߂�l   �FOK �˃f�[�^�擾����
         �FNG �˃f�[�^�擾���s
   �@  �\   �F�L�[�e�p�����[�^�擾����
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
static BYTE   Key_parameter_get(void)
{
   BYTE   work;
   BYTE   rtn_cd;

   key_com_stat = Sbm_current_mode();

   if((Sbm_mode_check() != OFF)&&(Sbm_mode_check() != CSBM_VOL)) // v� la submode nen key_mode_stat se bang gia tri trc do. Neu dang o TUNER ma chinh alarm th� 
   {                                                             // key_mode_stat se bang cua TUNER.
      KcodeT_p = (const KEY_CHG_TBL *)Sbm_keyCodeTable_addr_get(); // tuy la submode nao(ALARM, CLOCK,SEF...) thi se anh xa den keytable cua submode do
      ProceT_p = (const KEY_PROC_TBL *)Sbm_key_func_addr_get();
      rtn_cd = Key_code_sub_func();   /* �T�u���[�h�L�[�T�[�`�A�L���L�[�����OK�ɂȂ�  Submode key search, OK if it is valid key*/
      if (rtn_cd == OK)
      {
         return(rtn_cd);
      }
   }

   if (Pw_Power_status_get() == OFF)
   {
      KcodeT_p = (const KEY_CHG_TBL *)Off_keyCodeTable_addr_get();
      ProceT_p = (const KEY_PROC_TBL *)Off_key_func_addr_get();
      key_mode_stat = 0x00;
      rtn_cd = Key_code_sub_func();
      return(rtn_cd);
   }

   /* ���C�����[�h�T�[�` */
   rtn_cd = OK;
   work = Ma_current_mode();
   switch( work )
   {
      case   CMA_TUNER:      /*   TUNER���[�h   */
      case   CMA_ISR:
         KcodeT_p = (const KEY_CHG_TBL *)Tu_keyCodeTable_addr_get();
         ProceT_p = (const KEY_PROC_TBL *)Tu_key_func_addr_get();
         key_mode_stat = Tu_ModeID_get();
         break;
      
      case   CMA_AUX:      /*   AUX���[�h */
         KcodeT_p = (const KEY_CHG_TBL *)Aux_keyCodeTable_addr_get();
         ProceT_p = (const KEY_PROC_TBL *)Aux_key_func_addr_get();
         key_mode_stat = Aux_ModeID_get();  // 0x00
         break;

      default:
         KcodeT_p = (const KEY_CHG_TBL *)Aux_keyCodeTable_addr_get();
         ProceT_p = (const KEY_PROC_TBL *)Aux_key_func_addr_get();
         key_mode_stat = Aux_ModeID_get();
         break;
   }

      rtn_cd = Key_code_sub_func();
   
   return(rtn_cd);
}
/******************************************************************************
   �֐���   �FKey_status_check
   ��  ��   �F����
   �߂�l   �F����
   �@  �\   �F�L�[��Ԕ��f����
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
static void Key_status_check(void)
{
   BYTE   ext_mute;

   /* �L�[��Ԕ��f���� */
   key_status = CKEY_KEYNOP;            /* Key �ω����� */

   if (key_old_key != key_new_key)   // Truong hop nay = FALSE(key_old_key == key_new_key) khi key duoc nhan giu lau. 
   {
      if (key_old_key == CKEY_ID_KEY_OFF)    // truong hop nhan key moi. VD: truoc do nhan key PWR , sau do nhan key CH. 
                                             // Trong TH nay, key_long_timer se duoc gan moi(1s,2s tuy module)
      {
         key_status = CKEY_KEYON;         /* Key On */
         key_now_key = key_new_key;         /* New Key On */
         key_old_key = key_new_key;
      }
      else     // Truong hop ko nhan tiep key da nhan duoc do. VD: luc dau nhan key PWR, tha ra va ko nhan nua, toi lan key_main() tiep theo no se zo else n�y
      {        // VD: Nhan key PWR de tu PWROFF->ON:
               //   O lan nhan mess CMSG_MAIN dau tien, doan if phia tren se duoc thuc hien
               //   O lan nhan mess tiep theo(khi nay da release key PWR), doan else se duoc thuc hien, khi nay co fpw_power_key se tu OFF->ON
         key_status = CKEY_KEYOFF;         /* Key Off */
         key_now_key = key_old_key;         /* Old Key Off */
         key_old_key = CKEY_ID_KEY_OFF;      /* Key Old Off�ݒ� */
         fkey_prio_key = OFF;            /* �D��L�[OFF */
      }
      key_long_timer = 0xff;               /* key long timer�����ݒ� */
   }
   else if (key_long_timer == TIMER_OUT)     // Truong hop nhan giu key. 
                                             // Neu nhan giu key ma ko release, key_long_timer se giam tu tu cho den khi = TIMER_OUT.
   {
      key_status = CKEY_KEYLONG;            /* Key ������ */
      key_now_key = key_old_key;            /* Old Key Off.  */
      key_long_timer = 0xff;
   }
   #if 0
   else if ((key_long_timer != 0xff) &&
         (fkey_prio_key == FALSE))
   {
      /* �O��MUTE���D��L�[�ȊO�L�[OFF�Ƃ��� */
      ext_mute = Sbm_current_mode();            /* EXT_MUTE �`�F�b�N */
      if ((ext_mute & CSBM_EMUTE) == CSBM_EMUTE)
      {
         key_status = CKEY_KEYOFF;         /* Key Off */
         key_now_key = key_old_key;         /* Old Key Off */
         key_old_key = CKEY_ID_KEY_OFF;      /* Key Old Off�ݒ� */
         fkey_nop = ON;                  /* �L�[���� */
      }
   }
   #endif
   else
   {
      ;
   }
}

/******************************************************************************
   �֐���   �FKey_code_process
   ��  ��   �F����
   �߂�l   �FKey �R�[�h
   �@  �\   �F���̓L�[�ɑ΂��郁�C�����[�h���̓��쏈�����N������B
   �R�����g�FLCD�L�[������Ă���L�[�R�[�h�����e�[�u���A�h���X��
           �擾���A���������s����B
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
static   void   Key_code_process(void)
{
   BYTE   work;

   if (key_status == CKEY_KEYNOP)         /* �L�[�ω������A���̂܂ܖ߂� */
   {
      return;
   }

   work = Key_parameter_get();

   if (work == NG)                  /* �p�����[�^�擾�ł��Ȃ� */
   {
      return;
   }
   key_modenb &= key_mode_stat;
   key_comenb &= key_com_stat;

   if ((key_modenb != key_mode_stat)||  // VD: dang trong mode TUNER SEEK => key_mode_stat = 0x02, neu nhan key VOLUP/VOLDOWN thi key_modenb = 0x01.(Xem file tu_keytbl.h)
      (key_comenb != key_com_stat))     //     khi n�y key_modenb = key_modenb & key_mode_stat = 0x00 => thoa dieu kien.
   {
      Key_nop();            /* �L�[���� */
   }
   else if ( Key_ProcFunc != 0 )
   {
      Key_ProcFunc();      /*   Key off Key����֐��p   */
   }
   else
   {
      Key_nop();            /* �L�[���� */
   }
}

/******************************************************************************
   �֐���   �FKey_code_sub_func
   ��  ��   �F����
   �߂�l   �FKey �R�[�h
   �@  �\   �F���̓L�[�ɑ΂��铮�쏈�����N������B
   �R�����g�F�m��L�[�̃��[�h���̓����R�[�h���珈���֐��A�h���X�擾����B
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
static   BYTE    Key_code_sub_func(void)
{
   BYTE   Key_code;
   BYTE   cnt;

   Key_code = CKEY_ID_INVALID;

   /* Local�L�[�ϊ� */
   for ( cnt = 0; cnt < CKEY_BYTE_MAX; cnt++ )
   {
      if ( KcodeT_p->com_key_code == CKEY_ID_INVALID )
      {
         return(NG);
      }
      else if (KcodeT_p->com_key_code == key_now_key)
      {
         Key_code = KcodeT_p->local_key_code;
         break;
      }
      else
      {
         ;
      }
      KcodeT_p++;
   }

   key_modenb   = 0;
   key_comenb      = 0;
   Key_ProcFunc   = 0;

   for (cnt = 0; cnt < CKEY_BYTE_MAX; cnt++)
   {
      if (ProceT_p->code == Key_code)
      {
         if (key_status == CKEY_KEYON)
         {
            key_comenb    = ProceT_p->comenb_on;
            key_modenb   = ProceT_p->modenb_on;
            Key_ProcFunc= ProceT_p->key_on;
         }
         else if (key_status == CKEY_KEYOFF)
         {
            key_comenb    = ProceT_p->comenb_off;
            key_modenb   = ProceT_p->modenb_off;
            Key_ProcFunc= ProceT_p->key_off;
         }
         else
         {
            key_comenb    = ProceT_p->comenb_long;
            key_modenb   = ProceT_p->modenb_long;
            Key_ProcFunc= ProceT_p->key_long;
         }
         return(OK);
      }
      else if ( ProceT_p->code == CKEY_ID_INVALID )
      {
         break;
      }
      else
      {
         ;
      }
      ProceT_p++;
   }
   return ( NG );
}

/******************************************************************************
   �֐���   �FKey_DataRecive
   ��  ��   �F����
   �߂�l   �F����
   �@  �\   �FKey Data ��M����
------------------------------------------------------------------------------
���t      �S����      �C�����e
2012/07/26   ���C��      �V�K�쐬
******************************************************************************/
static   BYTE   Key_DataRecive( void )
{
   BYTE   mode;
   memset(&key_lcd_driver_data[0],0x00,sizeof(key_lcd_driver_data));
   
   MIO_OUT_pLCDCE( LOW );      /*   LCD CE    "L"   */
   IO_lcd_ccb( 0x43);      /*   CCB�A�h���X���M   */
   MIO_OUT_pLCDCE( HIGH );      /*   LCD CE  "H"      */
   mode = IO_lcd_receive( &key_lcd_driver_data[0] );  // nhan data tu LCD driver va dua vao 4 phan tu(4 byte) cua key_lcd_driver_data[] 
   MIO_OUT_pLCDCE( LOW );      /*   LCD CE    "L"   */
   return(mode);
}

#define      TEST_KEY_POWER      0x0080
#define      TEST_KEY_AS         0x0040
#define      TEST_KEY_MODE      0x0020
#define      TEST_KEY_VUP      0x0010
#define      TEST_KEY_CH         0x0008
#define      TEST_KEY_VDW      0x0004
#define      TEST_KEY_UP         0x0002
#define      TEST_KEY_DW         0x0001
#define      TEST_KEY_TISC      0x8000
#define      TEST_KEY_ALARM      0x4000

void   key_mem_keyjet(BYTE keyjet)
{
   switch(keyjet)
   {
      case CKEY_ID_LCD_PWR:
         key_test_code |= TEST_KEY_POWER;
         break;
      
      case CKEY_ID_LCD_AS:
         key_test_code |= TEST_KEY_AS;
         break;
      
      case CKEY_ID_LCD_SRC:
         key_test_code |= TEST_KEY_MODE;
         break;
      
      case CKEY_ID_LCD_VOLUP:
         key_test_code |= TEST_KEY_VUP;
         break;
      
      case CKEY_ID_LCD_CH:
         key_test_code |= TEST_KEY_CH;
         break;
      
      case CKEY_ID_LCD_VOLDW:
         key_test_code |= TEST_KEY_VDW;
         break;
      
      case CKEY_ID_LCD_RIGHT:
         key_test_code |= TEST_KEY_UP;
         break;
      
      case CKEY_ID_LCD_TI_SCAN:
         key_test_code |= TEST_KEY_TISC;
         break;
      
      case CKEY_ID_LCD_LEFT:
         key_test_code |= TEST_KEY_DW;
         break;
      
      case CKEY_ID_LCD_ALARM:
         key_test_code |= TEST_KEY_ALARM;
         break;
      
      default :
         break;
   }
}

void   Key_test_key_chk(void)
{
   BYTE   key_driver_data;
   BYTE   key_d_bit_cnt;
   BYTE   loop_cnt1;            /*   Loop �J�E���^                  */
   BYTE   loop_cnt2;            /*   Loop �J�E���^                  */
   BYTE   test_jet;
   BYTE   test_rpy_bur[4];
   
   key_t_data[3] &= 0x03f;
   key_d_bit_cnt = 0;
   key_driver_data = 0;
   
   if (key_test_sta == CKEY_TSET_REPLY)
   {
      key_test_sta = CKEY_TSET_NOP;
      
      key_test_code = 0x0000;
      
      /*---- �p�l���L�[ ----*/
      for (loop_cnt1 = 0;loop_cnt1 < CKEY_LCD_DATA_BYTE;loop_cnt1++)
      {
         key_driver_data = key_t_data[loop_cnt1];
         for (loop_cnt2 = 0;loop_cnt2 < 8;loop_cnt2++)
         {
            if ((key_driver_data & 0x01) == 0x01)
            {
               test_jet = tkey_code_tbl[key_d_bit_cnt];
               key_mem_keyjet(test_jet);
            }
            key_driver_data = (key_driver_data >> 1);
            key_d_bit_cnt++;
         }
      }
      
      /*---- Alarm�L�[ ----*/
      if (fkey_alarm_key == TRUE )
      {
         key_mem_keyjet(CKEY_ID_LCD_ALARM);
      }
      
      test_rpy_bur[0] = 0xFB;
      test_rpy_bur[1] = 0x03;
      test_rpy_bur[2] = (BYTE)(key_test_code);
      test_rpy_bur[3] = (BYTE)(key_test_code>>8);
      
      Test_data_reply(CTEST_MODE_KEY_TEST,0x2C,test_rpy_bur,4);
   }
}


#undef _KEY_MAIN_C_
