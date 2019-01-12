/******************************************************************************
   �N�����I���i���j�a����      

      �v���W�F�N�g��   : AMFM���W�I
      �t�@�C����      : pw_main.c
      �@�\         : �d�����[�h�J�ڏ���
------------------------------------------------------------------------------
�C������
[chin20120725] ����
******************************************************************************/
#define   _PW_MAIN_C_

#include   "../comm/common.h"
#include   "../key/key_func_ext.h"
#include   "../io/IO_inc.h"
#include   "../main/ma_ext.h"
#include   "../audio/aud_ext.h"
#include   "../disp/lcd_ext.h"
#include   "../submode/sbm_ext.h"

#include   "pw_def.h"
#include   "pw_func_ext.h"
#include   "pw_main.h"

/*-----------------------------------------------------------------------
    �֐��錾
------------------------------------------------------------------------*/
static   void   Pw_set_item(void);
static   void   Pw_set_targetmode(void);
#if _POWER_TEST_COLDS_
static   BYTE   Pw_ColdS(void);
#endif
static   BYTE   Pw_HoldX(void);
static   BYTE   Pw_HoldX_In(void);
static   BYTE   Pw_HoldX_Out(void);
static   BYTE   Pw_AccOff(void);
static   BYTE   Pw_AccOff_In(void);
static   BYTE   Pw_AccOff_Out(void);
static   BYTE   Pw_PwrOff(void);
static   BYTE   Pw_PwrOn_Out(void);
static   BYTE   Pw_PwrOn_In(void);
static   BYTE   Pw_PwrOn(void);
static   BYTE   Pw_holdX_in_check(void);

/*-----------------------------------------------------------------------
   ITEM�ݒ�ð���
------------------------------------------------------------------------*/

const BYTE tpw_item_tbl[4][4] = {
/* ����Ӱ��   HOLDX��        ACCOFF��            POWER OFF          Power ON��      */   /* �ڕWӰ��  Tu tren xuong la target mode v� tu trai sang la curmode*/
   {   CPW_I_HOLDX,      CPW_I_HOLDX_IN,      CPW_I_ACCOFF_IN,   CPW_I_PWRON_OUT   },   /* HOLDX */
   {   CPW_I_HOLDX_OUT,   CPW_I_ACCOFF,      CPW_I_ACCOFF_IN,   CPW_I_PWRON_OUT   },   /* ACCOFF */
   {   CPW_I_HOLDX_OUT,   CPW_I_ACCOFF_OUT,   CPW_I_PWROFF,      CPW_I_PWRON_OUT   },   /* PowerOff */
   {   CPW_I_HOLDX_OUT,   CPW_I_ACCOFF_OUT,   CPW_I_PWRON_IN,      CPW_I_PWRON      }   /* PowerOn */
};

/*-----------------------------------------------------------------------
   �d������ITEM ð���
�������ӁF tpw_item_tbl�Őݒ肳���l�ƘA�����遚��
------------------------------------------------------------------------*/
typedef struct{
   BYTE   item;
   BYTE   (*func)(void);
}TPW_ITEM;

const  TPW_ITEM tpw_item[] =
{
      CPW_I_HOLDX,      Pw_HoldX
   ,   CPW_I_HOLDX_IN,      Pw_HoldX_In
   ,   CPW_I_HOLDX_OUT,   Pw_HoldX_Out
   ,   CPW_I_ACCOFF,      Pw_AccOff
   ,   CPW_I_ACCOFF_IN,   Pw_AccOff_In
   ,   CPW_I_ACCOFF_OUT,   Pw_AccOff_Out
   ,   CPW_I_PWROFF,      Pw_PwrOff
   ,   CPW_I_PWRON_OUT,   Pw_PwrOn_Out
   ,   CPW_I_PWRON_IN,      Pw_PwrOn_In
   ,   CPW_I_PWRON,      Pw_PwrOn
      #if _POWER_TEST_COLDS_
   ,   CPW_I_COLDS,      Pw_ColdS         /* Cold Start���� */
      #endif
};

/******************************************************************************
   �� �� ��   �Fpw_main
   ��   ��      �F����
   �� �� �l   �F����
   �@ �\ �T ��   �F�d������Ӽޭ�� ��ԊĎ�Ҳݏ���
------------------------------------------------------------------------------
�C������
�y�ԍ��z   ���t      �S����      �C�����e
[sun060401]�V�K�쐬
******************************************************************************/
void   Pw_main(void)
{
   const   TPW_ITEM *pwitem;
   BYTE    (*pwfunc)(void);
   BYTE   work;

   do
   {
      #if _WDT_
      Io_wdt_clear();
      #endif
      
      /* ���ޯ�Ӱ�ސݒ菈�� */
      Pw_set_targetmode();
      /* ����ITEM�ݒ菈�� */
      Pw_set_item();
      /* ITEM�����ďo���� */
      pwitem = (const TPW_ITEM*)(tpw_item + pw_item);
      pwfunc = (pwitem->func);
      work = (*pwfunc)();
   }while(work == CPW_SEQCONT);  //CPW_SEQCONT = 1
   
   #if _POWER_DIAG_
   Pw_DIAG_check(CPW_DIAG_MD_MAIN);
   #endif   /* _POWER_DIAG_ */
}
/******************************************************************************
   �� �� ��   �FPw_set_targetmode
   ��   ��      �F����
   �� �� �l   �F����
   �@ �\ �T ��   �F�d���ڕW��Ԑݒ菈��
            �FBATT/ACC/PANEL�APower���Ȃǂ̏�Ԃ����킹�ēd���ڕW�����
------------------------------------------------------------------------------
�C�������F
[sun070321] �ύX
[sun070323] ACC OFF��ILL-DET������΁AHALT����o��悤�ɕύX
[sun100616] ILL ON��CTRL�[�q��HIGH�ɂ��Ȃ��AKEY ILL/BlinkLED�̂ݐ���̂��߁A
         CPW_DISPOFF�ɂ��Ȃ��悤�ɕύX
******************************************************************************/
static   void   Pw_set_targetmode(void)
{
   BYTE   tarmode;
   
   #if 1
   if (fpw_wakeup == TRUE)
   {
      tarmode = CPW_PWRON;
   }
   else
   if ((fpw_acc == ON)||(Alm_mode_check() == TRUE)) // check 2 TH l� ACCON hoac ACCOFF nhung dang dieu chinh ALARM
   {
      if (fpw_power_key == ON)
      {
         tarmode = CPW_PWRON;      /* ���ޯ�Ӱ�ނ�PowerOn�� */
      }
      else
      {
         tarmode = CPW_PWROFF;      /* ���ޯ�Ӱ�ނ�POWER OFF�� */
      }
   }
   else
   {
      #if _HOLDX_
      if (( fpw_acc_nor == OFF)
#if 0 /* �g�p���Ȃ����߁A�폜 */
        &&( pw_accwait_timer == TIMER_OUT)
#endif
        &&( fpw_accdet_active == OFF)
        &&( fpw_almdet_active == OFF))
      {
         tarmode = CPW_HOLDX;      /* ���ޯ�Ӱ�ނ�HOLDX�� */
      }
      else
      #endif
      {
         tarmode = CPW_ACCOFF;      /* ���ޯ�Ӱ�ނ�AccOFF���̒ʏ탂�[�h�� */
      }
   }
   #else
   tarmode = CPW_ACCOFF;
   #endif
   
   /* ���ޯ�Ӱ����ݼނ��� */
   #if 1
   if ((tarmode != pw_tarmode) &&      /* ���ޯ�Ӱ�ޕω����� */
           (fpw_itemchg_enb == ON))      /* ITEM�����҂��� */
   #else
   if (tarmode != pw_tarmode)      /* ���ޯ�Ӱ�ޕω����� */
   #endif
   {
      pw_tarmode = tarmode;
      pw_power_seq = CPW_SEQEND;
   }
   
   #if _WDT_TEST_
   while(1)
   {
      ;
   }
   #endif
}
/******************************************************************************
   �� �� ��   �FPw_set_item
   ��   ��      �F����
   �� �� �l   �F����
   �@ �\ �T ��   �F�d�����sITEM����NO��ď���
------------------------------------------------------------------------------
�C�������F
[sun060401]�V�K�쐬
******************************************************************************/
static   void   Pw_set_item(void)
{
   BYTE   item;

   #if _POWER_TEST_COLDS_
   if(/*ColdStart*/)
   {
      pw_item = CPW_COLDS;
   }
   else
   #endif
   /* BATT OFF�ȊO�̏ꍇ��ITEM��ݼދ������ITEM�ݒ�� */
   if (pw_power_seq == CPW_SEQEND)
   {
      fpw_itemchg_enb = OFF;
      pw_item = tpw_item_tbl[pw_tarmode][pw_curmode];
      pw_power_seq = CPW_SEQST; // CPW_SEQST = 1
   }
   
}


#if _POWER_TEST_COLDS_
/*------------------------------------------------------------------------------
..       �֐���  �FPw_ColdS
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FPw_ColdS���̏���
------------------------------------------------------------------------------
[chin20120803]�V�K�쐬
-----------------------------------------------------------------------------*/
static   BYTE   Pw_ColdS(void)
{
   //�V�[�P���X���K�v���ǂ���
}
#endif

/*------------------------------------------------------------------------------
..       �֐���  �FPw_HoldX
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FHoldXӰ�ޒ�����
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
static   BYTE Pw_HoldX(void)
{
   fpw_acc_nor = ON;
   pw_power_seq = CPW_SEQEND;
   fpw_itemchg_enb = ON;
   return(CPW_SEQCONT);
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_HoldX_In
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FHoldX���[�h�ɑJ�ڏ���
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
/* ACC OFF(�ʏ�) �� HOLDX */
static   BYTE Pw_HoldX_In(void)
{
   #if 1
   switch(pw_power_seq)
   {
      case 1:
         Ma_set_Msg( CMA_POWER, CMSG_HOLDX_IN );
         pw_power_seq ++;
         break;
      case 2:
         pw_curmode = CPW_HOLDX;
         pw_power_seq = CPW_SEQEND;
         fpw_almdet_active = OFF;
         if (IO_In_acc_det() == HIGH)
         {
            Io_port_holdx_in();
            Io_int_holdx_in();
            Io_sfr_holdx_in();
         }
         else
         {
            fpw_acc_nor = ON;
            fpw_accdet_active = ON;
         }
         fpw_itemchg_enb = ON;
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
   #else
   Ma_set_Msg( CMA_POWER, CMSG_HOLDX_IN );
   Pw_almkey_clear();
   pw_curmode = CPW_HOLDX;
   pw_power_seq = CPW_SEQEND;
   Io_int_holdx_in();
   Io_sfr_holdx_in();
   #endif
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_HoldX_Out
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FHoldX���[�h����ʂ��鏈���iPowerOff�ɑJ�ځj
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
/* HOLDX �� ACC OFF(�ʏ�) */
static   BYTE Pw_HoldX_Out(void)
{
   #if 0
   switch(pw_power_seq)
   {
      case 1:
         pw_power_seq ++;
         //Io_int_holdx_out();
         //Io_sfr_holdx_out();
         break;
      case 2:
         Ma_set_Msg( CMA_POWER, CMSG_HOLDX_OUT );
         pw_curmode = CPW_ACCOFF;
         pw_power_seq = CPW_SEQEND;
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
   #else
   #if 0 /* ���荞�ݏ����Ŏ��{���邽�߁A�폜 */
   Io_int_holdx_out();
   Io_sfr_holdx_out();
   Io_port_holdx_out();
   #endif
   
   Ma_set_Msg( CMA_POWER, CMSG_HOLDX_OUT );
   pw_curmode = CPW_ACCOFF;
   pw_power_seq = CPW_SEQEND;
   fpw_itemchg_enb = ON;
   return(CPW_RETMAIN);
   #endif
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_AccOff
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAccOff���̒ʏ��ԏ���
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
static   BYTE Pw_AccOff(void)
{
   fpw_itemchg_enb = ON;
   if ((fpw_accdet_active == ON)||(fpw_almdet_active == ON))
   {
      return (CPW_RETMAIN);
   }
   
   #if 1
   if (Pw_holdX_in_check() == TRUE )
   {
      fpw_acc_nor = OFF;      /* holdX�ֈڍs�v�� */
   }
   #endif
   return (CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_AccOff_In
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAccOff���̒ʏ��ԂɑJ�ڏ���
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
/* ACC ON(POWER OFF) �� ACC OFF(�ʏ�) */
static   BYTE Pw_AccOff_In(void)
{
   switch(pw_power_seq)
   {
      case 1:
         Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_OFFREQ );
         pw_seq_timer = (30+1);  // Qu�t phim 30ms, giam dan khi ham Ma_tim_int_1ms xay ra(do timer interrupt), no se goi ham Pw_1ms_timer de tru 1 don vi.
         pw_power_seq ++;
         break;
      case 2:
         if (pw_seq_timer == 0)
         {
            MIO_OUT_pSYS_ON(LOW);
            IO_SetIICPort(OFF);
            pw_curmode = CPW_ACCOFF;
            pw_power_seq = CPW_SEQEND;
            Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_OFF );
            Key_nop();
            Key_check_start();
            Io_port_accoff_in();
            Io_int_accoff_in();
            Io_sfr_accoff_in();
         }
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_AccOff_Out
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FAccOff���̒ʏ��Ԃ���ʂ��鏈���iHoldX���[�h�ɑJ�ځj
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
/* ACC OFF(�ʏ�) �� ACC ON(POWER OFF) */
static   BYTE Pw_AccOff_Out(void)
{
   switch(pw_power_seq)
   {
      case 1:
         Io_int_accoff_out();
         Io_sfr_accoff_out();
         pw_power_seq ++;
         break;
      case 2:
         MIO_OUT_pSYS_ON(HIGH);            /* SYS_ON */
         IO_SetIICPort(ON);
         Ma_set_Msg( CMA_POWER, CMSG_MAINCNT_ON );
         pw_power_seq = CPW_SEQEND;
         pw_curmode = CPW_PWROFF; // Khi ACC_OFF chuyen sang ACC ON, ko can biet Power ON hay OFF, no van chuyen ve mode Power OFF. Khi wa h�m Lcd_mode() trong 
                                  // file lcd_mode.c, no se kiem tra current mode, khi nay la POWER OFF, no se chi hien thi thoi gian.
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);  // CPW_RETMAIN = 0
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_PwrOff
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FPowerOFF���[�h������
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
static   BYTE Pw_PwrOff(void)
{
   #if 0
   accoff_check
   if ()   /* AccOff�� */
   {
      if�I(/* �A���[��BEEP����||�A���[��������) */)
      {
         
      }
   }
   #endif
   
   fpw_itemchg_enb = ON;
   pw_power_seq = CPW_SEQEND;
   Key_check_start();  // trong file key_func_ext.c
   return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_PwrOn_Out
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FPowerON���[�h����ʂ��钆�����iPowerOff�ɑJ�ځj
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
static   BYTE Pw_PwrOn_Out(void)
{
   switch(pw_power_seq)
   {
      case   1:
         #if 0
         if (Aud_get_beep_type() == CAUD_BEEP_STA_OFF)
         #endif
         {
            fpw_itemchg_enb = OFF;
            fpw_mute = ON;
            Aud_set_prg_mute(CAUD_MUTE_PWR_DT_MUTE,ON);
            pw_seq_timer = (10+1);
            Key_check_stop();
            pw_power_seq++;
         }
         break;
      case   2:
         if (pw_seq_timer == 0x00)
         {
            Ma_set_Msg( CMA_POWER, CMSG_PWROFF );
            Key_check_start();
            MIO_OUT_pSTBY(LOW);
            pw_curmode = CPW_PWROFF;   // Xet qua trinh chuyen tu PWRON(ACCON)->ACCOFF, tuc la tarmode = ACCOFF. Nhung vi curmode ko fai la ACCOFF(tarmode
                                       // ma minh muon nham toi) ma la PWROFF, nen khi nay tarmode va curmode khac nhau, dan den la sau khi quay lai Pw_main()
                                       // ta lai xet toi qua trinh chuyen tu PWROFF -> ACCOFF.Mot lan nua no se nhay sang ham Pw_AccOff_In().
            pw_power_seq = CPW_SEQEND;
            fpw_itemchg_enb = ON;
         }
         break;
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_PwrOn_In
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FPowerON���[�h�ɑJ�ڏ���
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
���l�F   PowerON���m�肵�Ă���A50ms��\���X�V���邽�߁A�\���̃C���^�[�t�F�[�X���K�v
      
-----------------------------------------------------------------------------*/
static   BYTE Pw_PwrOn_In(void)
{
   switch(pw_power_seq)
   {
      case   1:
         Ma_set_Msg( CMA_POWER, CMSG_PWRON );
         Key_check_stop();
         pw_seq_timer = (50+1);
         pw_power_seq++;
         break;
      case   2:
         if (pw_seq_timer == 0)
         {
            MIO_OUT_pSTBY(HIGH);
            #if _POWER_DIAG_
            Pw_DIAG_clear();
            #endif   /* _POWER_DIAG_ */
            if (fpw_wakeup == TRUE)
            {
               Aud_set_beep(CAUD_REQ_BEEP_ALARM);
            }
            Key_check_start();
            Aud_set_prg_mute(CAUD_MUTE_PWR_DA_MUTE,OFF);
            Aud_set_prg_mute(CAUD_MUTE_PWR_DT_MUTE,OFF);
            pw_power_seq = CPW_SEQEND;
            pw_curmode = CPW_PWRON;
         }
         break;
      case   3:
      
      default:
         pw_power_seq = CPW_SEQEND;
         break;
   }
   return(CPW_RETMAIN);
}

/*------------------------------------------------------------------------------
..       �֐���  �FPw_PwrOn
      ��  ��   �F����
      �߂�l   �F����
      �@  �\   �FPowerON������
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
-----------------------------------------------------------------------------*/
static   BYTE Pw_PwrOn(void)
{
   pw_power_seq = CPW_SEQEND;
   Key_check_start();
   fpw_itemchg_enb = ON;
   return(CPW_RETMAIN);
}

/******************************************************************************
   �֐���   �Fpw_initial
   ��  ��   �F����
   �߂�l   �F����
   �@  �\   �F�d������Ӽޭ�ُ���������
------------------------------------------------------------------------------
[chin20120725]�V�K�쐬
���l�F   �V�X�e�����b�Z�[�W�͔��s���܂���B
      HoldX�����ԂɂȂ�܂ŁAACC_DET�֎~�i�t���O���A���Ԃ�҂��j
      IO���W���[���̃C���^�[�t�F�[�X���Ăяo���iColdStart���̊֐��j
******************************************************************************/
void   Pw_initial(void)
{
   /* ACCOFF���[�h�ɂ��Ă���*/
   pw_curmode = CPW_ACCOFF;      /* AccOffӰ�ނ������   Bat dau tu che do AccOff  */
   
   #if 0
   Io_int_initial_set();
   #endif
   pw_power_seq = CPW_SEQEND;      // CPW_SEQEND = 0 
   fpw_acc_nor = ON;            // fpw_acc_nor trong file pw_main.h
   fpw_accdet_active = ON;      /* Acc�`���^�����O���s�� */
   Lcd_accoff_send_req();   // trong file lcd_ext.c, duoc su dung trong file lcd_ext.c
   #if 0 /* �g�p���Ȃ����߁A�폜  */
   pw_accwait_timer = CPW_ACCWATI_TIM;      /* ACC����ݸޒ�HALT�֑J�ڂ��Ȃ��悤��   Khong the chuyen sang HALT trong khi chat voi ACC*/
   #endif
   fpw_itemchg_enb = ON;  // trong file pw_main.c, h�m Pw_targetmode  
   
   pw_AccOn_4mscnt = 0;
   pw_accon_chat_cnt = 0;
   pw_accoff_chat_cnt = 0;
   pw_almkey_cnt = 0;
}

/******************************************************************************
   �֐���   �FPw_holdX_in_check
   ��  ��   �F����
   �߂�l   �FTRUE ��   HOLDX�ɑJ��
         �FFALSE��   HOLDX�ɑJ�ڂ��Ȃ�
   �@  �\   �Faccoff��Ԃ���HOLDX�ɍs�����ǂ����̃`�F�b�N����
------------------------------------------------------------------------------
[chin20120827]�V�K�쐬
******************************************************************************/
static   BYTE   Pw_holdX_in_check(void)
{
   BYTE ret;
   
   ret = TRUE;
   
   #if 0
   if( Alm_mode_check() == TRUE )
   {
      ret = FALSE;         /* �A���[���������ł͂Ȃ� */
   }
   #endif
   
   if (Lcd_accoff_send_end() == TRUE )
   {
      ret = FALSE;         /* �\���X�V�ς� */
   }
   return ret;
}
#undef  _PW_MAIN_C_
