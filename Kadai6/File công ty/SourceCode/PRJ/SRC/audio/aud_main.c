/******************************************************************************
   �N�����I���a����
   �v���W�F�N�g��   :    2012�NAM/FM���W�I�Ή����f���J��
   �t�@�C����      :    aud_main.c
   ���W���[����   :    Audio���W���[��
   �@�\�T��      :    ���C������
-------------------------------------------------------------------------------
�C������
   �ԍ�      ���t      �S����      �֐���         �C�����e
   �y000�z      12/07/13   leejh                  �V�K�쐬
   �y001�z      12/09/25   leejh      Aud_msg_maincnt_off_req(�ύX)
                              Aud_msg_power_off(�ύX)
                               Aud_get_power_mode(�ύX)
                              Aud_init_evol_func(�ύX)
                              Aud_secret_loud_off(�ǉ�)
                              Aud_test_edit_data(�ǉ�)
                              Aud_test_edit_vol(�ǉ�)
                              Aud_test_edit_tone(�ǉ�)
                              Aud_test_edit_sp(�ǉ�)
                              Aud_test_edit_loud(�ǉ�)
                              Aud_test_edit_mute(�ǉ�)
                              Aud_test_edit_beep(�ǉ�)
   �y002�z      12/11/07   leejh      Aud_msg_maincnt_off(�ύX)
                              Aud_msg_power_off(�ύX)
********************************************************************************/
#define      _AUD_MAIN_C_

#include   <string.h>
#include   "../model.h"

#include   "../comm/common.h"
#include   "../main/MA_inc.h"
#include   "../tuner/TU_inc.h"
#include   "../power/PW_inc.h"
#include   "../io/IO_inc.h"
#include   "../submode/SBM_inc.h"
#include   "../testmode/TEST_inc.h"

#include   "aud_ext.h"
#include   "aud_sys.h"
#include   "aud_def.h"
#include   "aud_main.h"
#include   "aud_mute_func.h"
#include   "aud_dev_func.h"
#include   "aud_beep_func.h"

#define      _TEST_NON_MUTE_      0

/*---------------------------------------------------------------------------*
 *   �����萔��`                                               *
 *---------------------------------------------------------------------------*/
#define   AUD_MODE_MAX   ( CMA_MODE_MAX + 1 )   /* ���[�h MAX   */

#define CAUD_EVOL_SEQ_START   0x01   /* Mute�����V�[�P���X�J�n   */
#define CAUD_EVOL_SEQ_OFF   0xFF   /* Mute�����V�[�P���X�I��   */

/* MAIN MODE -> AUDIO�T�E���h�\�[�X�ϊ��e�[�u�� */
const TAUD_MODE_CONV_DEF aud_mid_tbl[] =
{
   /* MainModuleID   , AudioModuleID                        */
   { CMA_TUNER      , CAUD_MODE_TUNER_AM   },   /* TUNER MODE   */
   { CMA_AUX      , CAUD_MODE_AUX1      },   /* AUX1 MODE   */
   { CMA_ISR      , CAUD_MODE_TUNER_AM   }   /* ISR MODE      */
};

/*---------------------------------------------------------------------------*
 *   �\���̒�`                                                  *
 *---------------------------------------------------------------------------*/
/* �T�E���h�\�[�X����AUDIO���Ǘ����X�g */
typedef struct {
   BYTE      volume[AUD_MODE_MAX];   /* VOL�X�e�b�v�Ǘ����X�g         */
} TAUD_AUDIO_STEP_LIST;

/* �C�x���g���Ǘ� */
typedef struct {
   BYTE      mcnt_on      :1;   /* 0: main cnt on�C�x���g   0: OFF 1: ON   */
   BYTE      adjust      :1;   /* 1: audio����            0: OFF 1: ON   */
   BYTE      band_chg   :1;   /* 2: TUBER BAND�؊���      0: OFF 1: ON   */
   BYTE      mode_chg   :1;   /* 3: ���[�h�`�F���WEVENT   0: OFF 1: ON   */
   BYTE      reserved   :4;   /* reserved                           */
} TAUD_EVENT;

/* �V�[�P���X���Ǘ� */
typedef struct {
   TAUD_EVENT   event;            /* EVOL�C�x���g�t���O       */
   BYTE      seq_id;            /* �����V�[�P���X�J�E���^    */
   BYTE      maincnt_seq_id;      /* MainCnt_On�����l���M�V�[�P���X�J�E���^ */
} TAUD_SEQ_MODE;

/*---------------------------------------------------------------------------*
 *   �ϐ���`                                                  *
 *---------------------------------------------------------------------------*/
static TAUD_AUDIO_INFO      Aud_audio_info;                  /* AUDIO���      TAUD_AUDIO_INFO trong file aud_def.h      */
static TAUD_AUDIO_STEP_LIST   Aud_saved_list;                  /* �T�E���h�\�[�X����AUDIO���    */
static BYTE               Aud_01ms_timer[CAUD_C01MS_MAX];      /* 1ms�^�C�}�J�E���^          */
static BYTE               Aud_05ms_timer[CAUD_C05MS_MAX];      /* 5ms�^�C�}�J�E���^          */
static BYTE               Aud_10ms_timer[CAUD_C10MS_MAX];      /* 10ms�^�C�}�J�E���^          */
static BYTE               Aud_25ms_timer[CAUD_C25MS_MAX];      /* 25ms�^�C�}�J�E���^          */
static BYTE               Aud_1sec_timer[CAUD_C1SEC_MAX];      /* 1sec�^�C�}�J�E���^          */
static TAUD_TIMER_INFO      Aud_01ms_info;                  /* 1ms�^�C�}���             */
static TAUD_TIMER_INFO      Aud_05ms_info;                  /* 5ms�^�C�}���             */
static TAUD_TIMER_INFO      Aud_10ms_info;                  /* 10ms�^�C�}���             */
static TAUD_TIMER_INFO      Aud_25ms_info;                  /* 25ms�^�C�}���             */
static TAUD_TIMER_INFO      Aud_1sec_info;                  /* 1sec�^�C�}���             */
static TAUD_SEQ_MODE      aud_mode_evol;                  /* EVOL�����V�[�P���X�Ď����    */

/*---------------------------------------------------------------------------*
 *   �v���g�^�C�v��`                                            *
 *---------------------------------------------------------------------------*/
static BYTE Aud_mode_MainToAudio( BYTE );
static void Aud_update_src_info( void );
static void Aud_save_audio_info( void );
static void Aud_read_audio_info( void );
static void Aud_event_maincnt_on( void );
static void Aud_event_mode_change( void );
static void Aud_event_band_change( void );
static void Aud_event_ajust_audio( void );

static void Aud_chk_BAND_change( void );
static void Aud_chk_audio_event( void );

static void Aud_audio_event( void );
static void Aud_chk_audio_mute( void );

static void Aud_init_volume( void );
static void Aud_init_bass( void );
static void Aud_init_treble( void );
static void Aud_init_balance( void );
static void Aud_init_fader( void );

static void Aud_init_audio_list( void );
static void Aud_init_evol_func( void );

#if _TEST_MODE_
static void Aud_test_edit_vol( BYTE, BYTE * );
static void Aud_test_edit_tone( BYTE );
static void Aud_test_edit_sp( BYTE );
static void Aud_test_edit_loud( BYTE, BYTE * );
static void Aud_test_edit_mute( BYTE, BYTE * );
static void Aud_test_edit_beep( BYTE * );
#endif

/******************************************************************************
   �֐���      �FAud_mode_MainToAudio
   ����      �FBYTE main_id 
   �߂�l      �FBYTE mode_id
   �@�\�T��   �FMain���W���[�����Ǘ����郂�[�hID��AUDIO�Ǘ���ID�֕ϊ�����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static BYTE Aud_mode_MainToAudio( BYTE main_id )
{
   BYTE   i;
   BYTE   mode_id;
   BYTE   max_cnt;

   mode_id = CAUD_MODE_TUNER_AM;   /* Fail Safe */
   max_cnt = sizeof( aud_mid_tbl ) / sizeof( TAUD_MODE_CONV_DEF );

   for ( i = 0; i < max_cnt; i++ )
   {
      /* ��v����ID���������� */
      if ( main_id == aud_mid_tbl[i].mid )
      {
         mode_id = aud_mid_tbl[i].aid;

         /* TUNER���[�h���̏ꍇ�͎�M�o���h�𒲂ׂ� */
         if ( aud_mid_tbl[i].mid == CMA_TUNER )
         {
            /* FM��M���̏ꍇ��FM�ɕϊ� */
            if ( Tu_band_chk() != CTU_AM )
            {
               mode_id = CAUD_MODE_TUNER_FM;
            }
         }
         break;
      }
   }
   
   return( mode_id );
}

/******************************************************************************
   �֐���      �FAud_update_sound_src_info
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �F�T�E���h�\�[�X�̏����X�V����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_update_src_info( void )
{
   BYTE   curt_mode;

   /* �J�����g���[�h���擾 */
   curt_mode = Ma_current_mode();

   /* ���[�h�����X�V���� */
   Aud_audio_info.prev_mode = Aud_audio_info.curt_mode;
   Aud_audio_info.curt_mode = curt_mode;

   /* �T�E���h�\�[�X�����X�V���� */
   Aud_audio_info.prev_src  = Aud_audio_info.curt_src;
   Aud_audio_info.curt_src  = Aud_mode_MainToAudio( curt_mode );
}

/******************************************************************************
   �֐���      �FAud_save_audio_info
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �F�T�E���h�\�[�X����AUDIO����ޔ�����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_save_audio_info( void )
{
   BYTE   sund_src;

   /* �؊����O�̃T�E���h�\�[�X */
   sund_src = Aud_audio_info.prev_src;

   /* AUDIO����ޔ����� */
#if 1   /* Volume Step���� */
   Aud_saved_list.volume[CAUD_MODE_TUNER_AM] = Aud_audio_info.vol_step;
#else   /* Volume Step�Ɨ� */
   Aud_saved_list.volume[sund_src] = Aud_audio_info.vol_step;
#endif
}

/******************************************************************************
   �֐���      �FAud_read_audio_info
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �Faudio���Ǐo������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_read_audio_info( void )
{
   BYTE   sund_src;

   /* �؊�����(����)�̃T�E���h�\�[�X */
   sund_src  = Aud_audio_info.curt_src;

#if 1   /* Volume Step���� */
   Aud_audio_info.vol_step = Aud_saved_list.volume[CAUD_MODE_TUNER_AM];
#else   /* Volume Step�Ɨ� */
   Aud_audio_info.vol_step = Aud_saved_list.volume[sund_src];
#endif
}

/******************************************************************************
   �֐���      �FAud_msg_mode_change
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �F���[�h�؊�������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_msg_mode_change( void )
{
   aud_mode_evol.event.mode_chg = ON;
}

/******************************************************************************
   �֐���      �FAud_event_mcnt_on
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FMAIN CONT ON����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_msg_maincnt_on( void )
{
   /* 100ms�^�C�}��ݒ肷�� */
   Aud_set_timer( CAUD_CNT_25MS, CAUD_C25MS_MCNT_ON, ((100/25) + 1));

   /* set event main cont on */
   aud_mode_evol.event.mcnt_on      = ON;
   aud_mode_evol.maincnt_seq_id   = CAUD_EVOL_SEQ_START;
   
   /* ���M�֎~ */
   Aud_dis_audio  = ON;
}

/******************************************************************************
   �֐���      �FAud_msg_maincnt_off_req
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FMAIN CONT OFF����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_msg_maincnt_off_req( void )
{   
   Aud_set_stop_beep();            /* BEEP��~      */
   
   Aud_power_mode = CAUD_PWR_OFFSEQ;   /* PWR OFF REQ   */
   Aud_dis_audio = ON;               /* ���M�֎~      */
   
#if _TEST_MODE_
   /* LOUD�f�f(�e�X�g�A�B���L�[)�ϐ��N���A */
   memset(&Aud_diag_loud, 0, sizeof(Aud_diag_loud));
#endif
}

/******************************************************************************
   �֐���      �FAud_msg_maincnt_off
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FMAIN CONT OFF����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
   �y001�z      12/11/07   leejh      LOUD�f�f�\���̃N���A�����ǉ�
******************************************************************************/
void Aud_msg_maincnt_off( void )
{
   Aud_power_mode = CAUD_PWR_OFF;      /* PWR OFF   */
   Aud_dis_audio = ON;               /* ���M�֎~   */
}

/******************************************************************************
   �֐���      �FAud_event_pwr_on
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FPOWER ON����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_msg_power_on( void )
{
   /* power off�O�̓��̓\�[�X�Ƒ��Ⴊ���邩���m�F����    */
   if ( Ma_current_mode() != Aud_audio_info.curt_mode )
   {
      /* mode change���b�Z�[�W�������Əꍇ�Ɠ����悤�ɂ��� */
      Aud_msg_mode_change();
   }
}

/******************************************************************************
   �֐���      �FAud_event_pwr_off
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FMAIN CONT ON����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
    �y001�z      12/09/25   leejh      LOUD�f�f(�B���L�[)�N���A�����ǉ�
     �y002�z      12/11/07   leejh      LOUD OFF����������Ȃ��s��Ή�
******************************************************************************/
void Aud_msg_power_off( void )
{
   Aud_set_stop_beep();            /* BEEP��~   */
   
#if _TEST_MODE_
   /* LOUD�f�f(�B���L�[)���� */
   if( Aud_diag_loud.secret_loud != CAUD_LOUD_STA_UNKNOWN )
   {
      /* LOUD�f�f(�e�X�g)���� */
      if(( Aud_diag_loud.test_loud == CAUD_LOUD_STA_OFF) ||
         ( Aud_diag_loud.test_loud == CAUD_LOUD_STA_ON ))
      {
         /* LOUD�f�f(�e�X�g)�D��ɂ��� */
         Aud_diag_loud.prio_diag = CAUD_PRIO_TEST;
      }
      else
      {
         /* UNKNOWN�ݒ� */
         Aud_diag_loud.prio_diag = CAUD_PRIO_UNKNOWN;
      }
      
      /* LOUD �s��(�B���L�[)�N���A */
      Aud_diag_loud.secret_loud = CAUD_LOUD_STA_UNKNOWN;
   }
#endif
}

/******************************************************************************
   �֐���      �FAud_event_maincnt_on
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �FEVOL MAINCNT ON�C�x���g����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_event_maincnt_on( void )
{
   switch ( aud_mode_evol.maincnt_seq_id )
   {
      case 1:
         if ( Aud_get_timer( CAUD_CNT_25MS, CAUD_C25MS_MCNT_ON ) == 0 )
         {
            /* E-VOL�����l�𑗐M */
            Aud_send_initial();

            aud_mode_evol.maincnt_seq_id++;
            /* �A�h�o���X�X�C�b�`20ms�҂� */
            Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_ASP_WAIT, ((20/5) + 1));
         }
         break;
         
      case 2:
         if ( Aud_get_timer( CAUD_CNT_05MS, CAUD_C05MS_ASP_WAIT ) == 0 )
         {
            /* power on�������(power on�m��) */
            Aud_power_mode = CAUD_PWR_ON;
            Aud_dis_audio = OFF;
            
            /* data mute on(�O�̂���) */
            aud_mute_state.data_mute = ON;
            
            /* audio�S�f�[�^���M */
            Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
      
            aud_mode_evol.event.mcnt_on = OFF;
            aud_mode_evol.maincnt_seq_id = CAUD_EVOL_SEQ_OFF;
         }
         break;
         
      default:
         break;
   }
}

/******************************************************************************
   �֐���      �FAud_event_mode_change
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �F���[�h�؊�������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y002�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_event_mode_change( void )
{
   /* Mute On�������Ă���A���͐ؑւ��s�� */
   if( Aud_is_on_mute() == TRUE )
   {
      /* �T�E���h�\�[�X�����X�V���� */
      Aud_update_src_info();
   
      /* �؊����O��AUDIO����ޔ����� */
      Aud_save_audio_info();
   
      /* �؊������AUDIO����Ǐo�� */
      Aud_read_audio_info();
      
      Aud_ctrl_ASP( CAUD_ASP_SRC_CHG );
      
      /* BAND�؊����C�x���g�N���A */
      aud_mode_evol.event.band_chg = OFF;
      
      /* ModeChange EVENT�N���A */
      aud_mode_evol.event.mode_chg = OFF;
   }
}

/******************************************************************************
   �֐���      �FAud_event_band_change
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �F
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_event_band_change( void )
{
   if (( Aud_get_timer( CAUD_CNT_10MS, CAUD_C10MS_BAND_CHG_WAIT ) == 0 )
   && ( Aud_is_on_mute() == TRUE ))
   {
      /* �T�E���h�\�[�X�����X�V���� */
      Aud_update_src_info();

      /* �؊����O��AUDIO����ޔ����� */
      Aud_save_audio_info();

      /* �؊������AUDIO����Ǐo�� */
      Aud_read_audio_info();
      
      /* �\�[�X�ؑւ�E-VOL�f�[�^���M */
      Aud_ctrl_ASP( CAUD_ASP_SRC_CHG );

      /* BAND�؊����C�x���g�N���A */
      aud_mode_evol.event.band_chg = OFF;
   }
}

/******************************************************************************
   �֐���      �FAud_event_ajust_audio
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FE-VOL��AUDIO�f�[�^�̑��M�ASRS WOW�f�o�C�X�𐧌䂷��
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_event_ajust_audio( void )
{
   /* �d�q�{�����[���f�o�C�X���� */
   Aud_ctrl_ASP( Aud_audio_info.asp_cmd );

   /* �C�x���g�N���A */
   Aud_audio_info.asp_cmd      = 0xFF;
   aud_mode_evol.event.adjust   = OFF;
}

/******************************************************************************
   �֐���      �FAud_edit_volume
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �Fvolume��������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_edit_volume( BYTE direction )
{
   BYTE   step;
   BYTE   pre_step;
   
   /* ���݂�step�l��ǂ݂��� */
   step = Aud_audio_info.vol_step;

   switch ( direction )
   {
      case CAUD_STP_UP:
         step = Aud_incr( step, CAUD_VOL_MAX );
         break;

      case CAUD_STP_DOWN:
         step = Aud_decr( step, CAUD_VOL_MIN );
         break;

      case CAUD_STP_CENTER:
         step = CAUD_VOL_CNT;
         break;

      case CAUD_STP_MAX:
         step = CAUD_VOL_MAX;
         break;

      case CAUD_STP_MIN:
         step = CAUD_VOL_MIN;
         break;

      case CAUD_STP_DEF:
         step = CAUD_VOL_INI;
         break;

      default:
         break;
   }

   /* volume step�ω����� */
   if ( Aud_audio_info.vol_step != step )
   {
      /* ��Step���ȑOStep�Ɋi�[ */
      pre_step = Aud_audio_info.vol_step;

      /* volume step���X�V���� */
      Aud_audio_info.vol_step = step;

      /* volume�����̗v����EVOL�֏o�� */
      Aud_audio_info.asp_cmd = CAUD_ASP_VOL;
      aud_mode_evol.event.adjust = ON;
      
      /* Vol Step 1��0�A0��1�͑��M�v�����Ȃ� */
      if ((( pre_step > CAUD_VOL_MIN ) && ( step == CAUD_VOL_MIN )) ||
         (( pre_step == CAUD_VOL_MIN ) && ( step > CAUD_VOL_MIN )))
      {
         Aud_audio_info.asp_cmd = 0xFF;
         aud_mode_evol.event.adjust = OFF;
      }
   }   
}

/******************************************************************************
   �֐���      �FAud_edit_bass
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �FBASS��������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_edit_bass( BYTE direction )
{
   BYTE   step;

   step = Aud_audio_info.bas_step;

   switch ( direction )
   {
      case CAUD_STP_UP:
         step = Aud_incr( Aud_audio_info.bas_step, CAUD_BAS_MAX );
         break;

      case CAUD_STP_DOWN:
         step = Aud_decr( Aud_audio_info.bas_step, CAUD_BAS_MIN );
         break;

      case CAUD_STP_CENTER:
         step = CAUD_BAS_CNT;
         break;

      case CAUD_STP_MAX:
         step = CAUD_BAS_MAX;
         break;

      case CAUD_STP_MIN:
         step = CAUD_BAS_MIN;
         break;

      case CAUD_STP_DEF:
         step = CAUD_BAS_INI;
         break;

      default:
         break;
   }

   if (Aud_audio_info.bas_step != step)
   {
      /* bass step���X�V���� */
      Aud_audio_info.bas_step = step;
      
      /* bass�����̗v����EVOL�֏o�� */
      Aud_audio_info.asp_cmd = CAUD_ASP_BAS;
      aud_mode_evol.event.adjust   = ON;

   }
}

/******************************************************************************
   �֐���      �FAud_edit_treble
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �FTREBLE��������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_edit_treble( BYTE direction )
{
   BYTE step;

   step = Aud_audio_info.tre_step;

   switch ( direction )
   {
      case CAUD_STP_UP:
         step = Aud_incr( Aud_audio_info.tre_step, CAUD_TRE_MAX );
         break;

      case CAUD_STP_DOWN:
         step = Aud_decr( Aud_audio_info.tre_step, CAUD_TRE_MIN );
         break;

      case CAUD_STP_CENTER:
         step = CAUD_TRE_CNT;
         break;

      case CAUD_STP_MAX:
         step = CAUD_TRE_MAX;
         break;

      case CAUD_STP_MIN:
         step = CAUD_TRE_MIN;
         break;

      case CAUD_STP_DEF:
         step = CAUD_TRE_INI;
         break;

      default:
         break;
   }

   if (Aud_audio_info.tre_step != step)
   {
      /* treble step���X�V���� */
      Aud_audio_info.tre_step = step;
      
      /* treble�����̗v����EVOL�֏o�� */
      Aud_audio_info.asp_cmd = CAUD_ASP_TRE;
      aud_mode_evol.event.adjust = ON;
   }
}

/******************************************************************************
   �֐���      �FAud_edit_balance
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �FBALANCE��������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_edit_balance( BYTE direction )
{
   BYTE step;

   step = Aud_audio_info.bal_step;

   switch ( direction )
   {
      case CAUD_STP_UP:
         /* STEP�����Z���� */
         step = Aud_incr( Aud_audio_info.bal_step, CAUD_BAL_MAX );
         break;

      case CAUD_STP_DOWN:
         /* STEP�����Z���� */
         step = Aud_decr( Aud_audio_info.bal_step, CAUD_BAL_MIN );
         break;

      case CAUD_STP_CENTER:
         step = CAUD_BAL_CNT;
         break;

      case CAUD_STP_MAX:
         step = CAUD_BAL_MAX;
         break;

      case CAUD_STP_MIN:
         step = CAUD_BAL_MIN;
         break;

      case CAUD_STP_DEF:
         step = CAUD_BAL_INI;
         break;

      default:
         break;
   }

   if (Aud_audio_info.bal_step != step)
   {
      /* balance step���X�V���� */
      Aud_audio_info.bal_step = step;
      
      /* balance�����̗v����EVOL�֏o�� */
      Aud_audio_info.asp_cmd = CAUD_ASP_BAL;
      aud_mode_evol.event.adjust = ON;
   }
}

/******************************************************************************
   �֐���      �FAud_edit_fader
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �FFADER��������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_edit_fader( BYTE direction )
{
   BYTE   step;

   step = Aud_audio_info.fad_step;

   switch ( direction )
   {
      case CAUD_STP_UP:
         step = Aud_incr( Aud_audio_info.fad_step, CAUD_FAD_MAX );
         break;

      case CAUD_STP_DOWN:
         step = Aud_decr( Aud_audio_info.fad_step, CAUD_FAD_MIN );
         break;

      case CAUD_STP_CENTER:
         step = CAUD_FAD_CNT;
         break;

      case CAUD_STP_MAX:
         step = CAUD_FAD_MAX;
         break;

      case CAUD_STP_MIN:
         step = CAUD_FAD_MIN;
         break;

      case CAUD_STP_DEF:
         step = CAUD_FAD_INI;
         break;

      default:
         break;
   }

   if (Aud_audio_info.fad_step != step)
   {
      /* fader step���X�V���� */
      Aud_audio_info.fad_step = step;
      
      /* fader�����̗v����EVOL�֏o�� */
      Aud_audio_info.asp_cmd = CAUD_ASP_FAD;
      aud_mode_evol.event.adjust = ON;
   }
}

/******************************************************************************
   �֐���      �FAud_get_audio_info_list
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FAUDIOI��񃊃X�g���擾����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
TAUD_AUDIO_INFO *Aud_get_audio_info_list( void )
{
   return( &Aud_audio_info );
}

/******************************************************************************
   �֐���      �FAud_get_taudio_info
   ����      �FBYTE   ainf_typ   CAUD_ADJ_VOL�FVolume Step
                           CAUD_ADJ_BAS�FBass Step
                            CAUD_ADJ_TRE�FTreble Step
                           CAUD_ADJ_BAL�FBalance Ste)
                           CAUD_ADJ_FAD�FFader Step
   �߂�l      �FBYTE   aud_info   : �e�I�[�f�B�I���
   �@�\�T��   �F�C�ӂ̃I�[�f�B�I����ʒm����֐�
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
 ******************************************************************************/
BYTE Aud_get_audio_info( BYTE ainf_typ )
{
   BYTE aud_info;

   /* �ϐ������� */
   aud_info = 0xFF;

   switch ( ainf_typ )
   {
      /* volume step */
      case CAUD_ADJ_VOL:
         aud_info = Aud_audio_info.vol_step;  // duoc khai bao trong file aud_main.c, gia tri vol_step duoc khoi tao lan dau = NULL;
         break;

      /* bass step */
      case CAUD_ADJ_BAS:
         aud_info = Aud_audio_info.bas_step;
         break;

      /* treble step */
      case CAUD_ADJ_TRE:
         aud_info = Aud_audio_info.tre_step;
         break;

      /* balance step */
      case CAUD_ADJ_BAL:
         aud_info = Aud_audio_info.bal_step;
         break;

      /* fader step */
      case CAUD_ADJ_FAD:
         aud_info = Aud_audio_info.fad_step;
         break;

      default:
         break;
   }

   return ( aud_info );
}

/******************************************************************************
   �֐���      �FAud_get_power_mode
   ����      �F����
   �߂�l      �FBYTE   CAUD_PWR_OFF   : Power Off
                  CAUD_PWR_ONSEQ   : Power On Seq
                  CAUD_PWR_ON      : Power On
                  CAUD_PWR_OFFSEQ   : Power Off Seq
   �@�\�T��   �FAudio Power���[�h���擾����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
 ******************************************************************************/
BYTE Aud_get_power_mode( void )
{
   return ( Aud_power_mode );
}

/******************************************************************************
   �֐���      �FAud_chk_BAND_change
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FE-VOL�C�x���g�Ď�����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_chk_BAND_change( void )
{
   BYTE   curt_band;
   BYTE   curt_mode;

   curt_mode = Ma_current_mode();
   
   /* TUNER mode */
   if (curt_mode == CMA_TUNER)
   {
      curt_band = Tu_band_chk();

      /* BAND change */
      if (curt_band != Aud_audio_info.curt_band )
      {
         /* BAND���X�V */
         Aud_audio_info.curt_band = curt_band;

         /* �o���h�ؑւ̃C���^�[�o���^�C��(�C���^�[�o�����Ԃ��K�v���g�p) */
         Aud_set_timer( CAUD_CNT_10MS, CAUD_C10MS_BAND_CHG_WAIT, 0 );
         
         aud_mode_evol.event.band_chg = ON;
      }
   }
}

/******************************************************************************
   �֐���      �FAud_chk_audio_event
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FE-VOL�C�x���g�Ď�����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_chk_audio_event( void )
{
   Aud_chk_BAND_change();
}

/******************************************************************************
   �֐���      �FAud_audio_event
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FE-VOL�C�x���g����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_audio_event( void )
{
   /* MAINCNT ON�C�x���g���o */
   if ( aud_mode_evol.event.mcnt_on == ON )
   {
      Aud_event_maincnt_on();
   }
   /* ���[�h�؂�ւ��C�x���g���o */
   else if( aud_mode_evol.event.mode_chg == ON )
   {
      Aud_event_mode_change();
   }
   /* TUNER BAND�؊����C�x���g���o */
   else if ( aud_mode_evol.event.band_chg == ON )
   {
      Aud_event_band_change();
   }
   /* AUDIO�����C�x���g���o */
   else if ( aud_mode_evol.event.adjust == ON )
   {
      Aud_event_ajust_audio();
   }
   else   /* QAC�΍� */
   {
      ;
   }
}

/******************************************************************************
   �֐���      �FAud_chk_audio_mute
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FE-VOL���䃁�C������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_chk_audio_mute( void )
{
   BYTE   vol_step;

   /* volume step�擾 */
   vol_step = Aud_sub( Aud_audio_info.vol_step, Aud_audio_info.pre_kvol );

   if( vol_step == 0 )
   {
      /* mute on�v�� */
      Aud_set_prg_mute( CAUD_MUTE_AUD_NR_MUTE, ON );

      aud_mute_evol.zero_mute = ON;

   }
   else
   {
      aud_mute_evol.zero_mute = OFF;

      /* mute off�v�� */
      Aud_set_prg_mute( CAUD_MUTE_AUD_NR_MUTE, OFF );
   }
}

/******************************************************************************
   �֐���      �FAud_evol_main
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FE-VOL���䃁�C������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_evol_main( void )
{
   /* EVOL�C�x���g�Ď����� */
   Aud_chk_audio_event();
   
   /* EVOL�C�x���g���� */
   Aud_audio_event();
   
   #if (_TEST_NON_MUTE_ == 0)   /* �e�X�g��VOL�l0���AMUTE�[�q����s�v */
   /* mute���䏈�� */
   Aud_chk_audio_mute();
   #endif
}

/******************************************************************************
   �֐���      �FAud_set_timer
   ����      �FBYTE tmr_id   : �^�C�}��ʁi1/5/10/25/1000�j
            �FBYTE cnt_id   : �^�C�}�J�E���^id
            �FBYTE cnt      : �J�E���^�i��255�ȉ��̒l���w�肷�鎖�I�j
   �߂�l      �F����
   �@�\�T��   �F�^�C�}�J�E���^��ݒ肷�鏈��
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      06/04/03   ISHIHARA   �V�K�쐬
******************************************************************************/
void Aud_set_timer( BYTE tmr_id, BYTE cnt_id, BYTE cnt )
{
   switch( tmr_id ) {
      /* 1ms�^�C�} */
      case CAUD_CNT_01MS:
         if (Aud_01ms_info.max > cnt_id )
         {
            Aud_01ms_info.cntr[cnt_id] =cnt;
         }
      break;

      /* 5ms�^�C�} */
      case CAUD_CNT_05MS:
         if (Aud_05ms_info.max > cnt_id )
         {
            Aud_05ms_info.cntr[cnt_id] =cnt;
         }
      break;

      /* 10ms�^�C�} */
      case CAUD_CNT_10MS:
         if (Aud_10ms_info.max > cnt_id )
         {
            Aud_10ms_info.cntr[cnt_id] =cnt;
         }
      break;

      /* 25ms�^�C�} */
      case CAUD_CNT_25MS:
         if (Aud_25ms_info.max > cnt_id )
         {
            Aud_25ms_info.cntr[cnt_id] =cnt;
         }
      break;

      /* 1000ms�^�C�} */
      case CAUD_CNT_1SEC:
         if (Aud_1sec_info.max > cnt_id )
         {
            Aud_1sec_info.cntr[cnt_id] =cnt;
         }
      break;

      /* �����w�� */
      default:
      break;
   }
}

/******************************************************************************
   �֐���      �FAud_get_timer
   ����      �FBYTE tmr_typ   : �^�C�}��ʁi1/5/10/25/1000�j
            �FBYTE tmr_id   : �^�C�}�J�E���^id
   �߂�l      �FBYTE tmr      : �J�E���g���̃^�C�}�J�E���^
   �@�\�T��   �F�^�C�}�J�E���^�擾����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      06/04/03   ISHIHARA   �V�K�쐬
******************************************************************************/
BYTE Aud_get_timer( BYTE tmr_id, BYTE cnt_id )
{
   BYTE   tmr;

   /* �ϐ������� */
   tmr =0xff;

   switch( tmr_id ) {
      /* 1ms�^�C�} */
      case CAUD_CNT_01MS:
         if (Aud_01ms_info.max > cnt_id )
         {
            tmr = Aud_01ms_info.cntr[cnt_id];
         }
      break;

      /* 5ms�^�C�} */
      case CAUD_CNT_05MS:
         if (Aud_05ms_info.max > cnt_id )
         {
            tmr = Aud_05ms_info.cntr[cnt_id];
         }
      break;

      /* 10ms�^�C�} */
      case CAUD_CNT_10MS:
         if (Aud_10ms_info.max > cnt_id )
         {
            tmr = Aud_10ms_info.cntr[cnt_id];
         }
      break;

      /* 25ms�^�C�} */
      case CAUD_CNT_25MS:
         if (Aud_25ms_info.max > cnt_id )
         {
            tmr = Aud_25ms_info.cntr[cnt_id];
         }
      break;

      /* 1000ms�^�C�} */
      case CAUD_CNT_1SEC:
         if (Aud_1sec_info.max > cnt_id )
         {
            tmr = Aud_1sec_info.cntr[cnt_id];
         }
      break;

      /* �����w�� */
      default:
      break;
   }
   return( tmr );
}

/******************************************************************************
   �֐���      �FAud_dec_timer
   ����      �FTAUD_TIMER_INFO   *tmr_info   :�J�E���g��������^�C�}���
   �߂�l      �F����
   �@�\�T��   �F�^�C�}���Z�����i�e�^�C�}���ʁj
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      06/04/03   ISHIHARA   �V�K�쐬
******************************************************************************/
void Aud_dec_timer( TAUD_TIMER_INFO *tmr_info )
{
   BYTE   i;

   /* �G���g������Ă���J�E���^���A�J�E���^���f�N�������g���� */
   for(i= 0; i< tmr_info->max; i++) {
      /* �^�C�}��"0"�łȂ���Ό��Z���� */
      if ( tmr_info->cntr[i] != 0 )
      {
         tmr_info->cntr[i]--;
      }
   }
}

/******************************************************************************
   �֐���      �FAud_msg_timer
   ����      �FBYTE timer_msg   : �^�C�}���b�Z�[�W
   �߂�l      �F�Ȃ�
   �@�\�T��   �FEVOL�^�C�}����
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_msg_timer( BYTE timer_msg )
{
   switch ( timer_msg )
   {
      case CAUD_EVT_TIMER_001M:
         /* 1ms�^�C�}�J�E���g���� */
         Aud_dec_timer(&Aud_01ms_info);
         break;

      case CAUD_EVT_TIMER_005M:
         /* 5ms�^�C�}�J�E���g���� */
         Aud_dec_timer(&Aud_05ms_info);
         break;

      case CAUD_EVT_TIMER_010M:
         /* 10ms�^�C�}�J�E���g���� */
         Aud_dec_timer(&Aud_10ms_info);
         break;

      case CAUD_EVT_TIMER_025M:
         /* 25ms�^�C�}�J�E���g���� */
         Aud_dec_timer(&Aud_25ms_info);
         break;

      case CAUD_EVT_TIMER_001S:
         /* 1000ms�^�C�}�J�E���g���� */
         Aud_dec_timer(&Aud_1sec_info);
         break;

      default:
         break;
   }
}

/******************************************************************************
   �֐���      �FAud_init_volume
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �Fvolume step�l������������֐�
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_init_volume( void )
{
   Aud_audio_info.vol_step = CAUD_VOL_INI;
   
   memset(&Aud_saved_list.volume[0], CAUD_VOL_INI, AUD_MODE_MAX );
}

/******************************************************************************
   �֐���      �FAud_init_bass
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �Fbass step�l������������֐�
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_init_bass( void )
{
   Aud_audio_info.bas_step = CAUD_BAS_INI;
}

/******************************************************************************
   �֐���      �FAud_init_treble
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �Ftreble step�l������������֐�
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_init_treble( void )
{
   Aud_audio_info.tre_step = CAUD_TRE_INI;
}

/******************************************************************************
   �֐���      �FAud_init_balance
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �Fbalance step�l������������֐�
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_init_balance( void )
{
   Aud_audio_info.bal_step = CAUD_BAL_INI;
}

/******************************************************************************
   �֐���      �FAud_init_fader
   ����      �F�Ȃ�
   �߂�l      �F�Ȃ�
   �@�\�T��   �Ffader step�l������������֐�
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_init_fader( void )
{
   Aud_audio_info.fad_step = CAUD_FAD_INI;
}

/******************************************************************************
   �֐���      �FAud_init_audio_list
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FAUDIO��񃊃X�g�̏����ݒ菈��
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
static void Aud_init_audio_list( void )
{
   /* AUDIO�N���A */
   Aud_init_volume();
   Aud_init_bass();
   Aud_init_treble();
   Aud_init_balance();
   Aud_init_fader();
   
   /* ���[�h�N���A*/
   Aud_audio_info.curt_src      = CAUD_MODE_TUNER_AM;
   Aud_audio_info.prev_src      = CAUD_MODE_TUNER_AM;
   Aud_audio_info.curt_band   = Tu_band_chk();
   
   /* �L�[MUTE�n�N���A */
   Aud_audio_info.pre_kvol      = CAUD_VOL_MIN;
   Aud_audio_info.pre_evol      = CAUD_VOL_MIN;
   
   /* ASP�N���A */
   Aud_audio_info.asp_cmd      = 0xFF;
}

/******************************************************************************
   �֐���      �FAud_init_evol_func
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FE-VOL����u���b�N����������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
   �y001�z      12/09/25   leejh      LOUD�f�f(�e�X�g�A�B���L�[)�\���̏�����
                               ���� Power Off��ԏ����ǉ�
******************************************************************************/
static void Aud_init_evol_func( void )
{
   /* �����ϐ��̏����� */
   memset(&Aud_audio_info, 0, sizeof(Aud_audio_info));
   memset(Aud_01ms_timer, 0, sizeof(Aud_01ms_timer));
   memset(Aud_05ms_timer, 0, sizeof(Aud_05ms_timer));
   memset(Aud_10ms_timer, 0, sizeof(Aud_10ms_timer));
   memset(Aud_25ms_timer, 0, sizeof(Aud_25ms_timer));
   memset(Aud_1sec_timer, 0, sizeof(Aud_1sec_timer));
   memset(&Aud_saved_list, 0, sizeof(Aud_saved_list));
   memset(&aud_mode_evol, 0, sizeof(aud_mode_evol));
   
#if _TEST_MODE_
   /* LOUD�f�f(�e�X�g�A�B���L�[)�ϐ��N���A */
   memset(&Aud_diag_loud, 0, sizeof(Aud_diag_loud));
#endif
   
   /* 1ms�^�C�}���̏����� */
   Aud_01ms_info.max  = sizeof(Aud_01ms_timer);
   Aud_01ms_info.cntr = Aud_01ms_timer;

   /* 5ms�^�C�}���̏����� */
   Aud_05ms_info.max  = sizeof(Aud_05ms_timer);
   Aud_05ms_info.cntr = Aud_05ms_timer;

   /* 10ms�^�C�}���̏����� */
   Aud_10ms_info.max  = sizeof(Aud_10ms_timer);
   Aud_10ms_info.cntr = Aud_10ms_timer;

   /* 25ms�^�C�}���̏����� */
   Aud_25ms_info.max  = sizeof(Aud_25ms_timer);
   Aud_25ms_info.cntr = Aud_25ms_timer;

   /* 1sec�^�C�}���̏����� */
   Aud_1sec_info.max  = sizeof(Aud_1sec_timer);
   Aud_1sec_info.cntr = Aud_1sec_timer;

   /* AUDIO��񃊃X�g�̏����ݒ菈��   */
   Aud_init_audio_list();

   /* MainCnt On�O�ɑ��M���Ă����Ȃ�   */
   Aud_dis_audio = ON;
   
   /* ���� Power Off��� */
   Aud_power_mode = CAUD_PWR_OFF;
}

/******************************************************************************
   �֐���      �FAud_msg_initial
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FAUDIO���䃂�W���[������������
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_msg_initial( void )
{
   /* E-VOL����u���b�N������ */
   Aud_init_evol_func();

   /* MUTE����u���b�N������ */
   Aud_init_mute_func();

   /* AUDIO�f�o�C�X�h���C�o���������� */
   Aud_init_dev_func();
   
   /* Beep ������ */
   Aud_init_beep_func();
}

/******************************************************************************
   �֐���      �FAud_msg_main
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �F�I�[�f�B�I���䃁�C�����[�v
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/07/13   leejh      �V�K�쐬
******************************************************************************/
void Aud_msg_main( void )
{
   /* MUTE����u���b�N���C�����[�v */
   Aud_mute_main();

   /* E-VOL����u���b�N���C�����[�v */
   Aud_evol_main();
   
   /* Beep���䃁�C�����[�v */
   Aud_beep_main();
}

#if _TEST_MODE_
/******************************************************************************
   �֐���      �FAud_secret_edit_loud
   ����      �F����
   �߂�l      �F����
   �@�\�T��   �FAudio�B���L�[����(Loud Off)
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
******************************************************************************/
void Aud_secret_loud_off( void )
{
   /* �B���L�[Loud Off�ݒ� */
   Aud_diag_loud.secret_loud = CAUD_LOUD_STA_OFF;
   
   /* �B���L�[�D�� */
   Aud_diag_loud.prio_diag = CAUD_PRIO_SECRET;
   
   /* ALL DATA���M�v�� */
   Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
}

/******************************************************************************
   �֐���      �FAud_test_edit_data
   ����      �FBYTE RcvCmd   => ��M�R�}���h
              BYTE *RcvBuf   => ��M�o�b�t�@
              BYTE Length   => ��M����
   �߂�l      �F����
   �@�\�T��   �FAudio�e�X�g���[�h(Data Edit)
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
******************************************************************************/
void Aud_test_edit_data( BYTE RcvCmd, BYTE *RcvBuf, BYTE Length )
{
   switch ( RcvCmd )
   {
      /* Volume�R�}���h */
      case CTEST_MODE_AUD_DIR_VOL:
         Aud_test_edit_vol( RcvCmd, RcvBuf );
         break;
      
      /* Tone�R�}���h */
      case CTEST_MODE_AUD_BAS_MAX:
      case CTEST_MODE_AUD_BAS_MIN:
      case CTEST_MODE_AUD_TRE_MAX:
      case CTEST_MODE_AUD_TRE_MIN:
      case CTEST_MODE_AUD_BSTR_CEN:
         Aud_test_edit_tone( RcvCmd );
         break;
      
      /* Speaker�R�}���h */
      case CTEST_MODE_AUD_BAL_RMAX:
      case CTEST_MODE_AUD_BAL_LMAX:   
      case CTEST_MODE_AUD_BAL_CEN:   
         Aud_test_edit_sp( RcvCmd );
         break;
      
      /* Loudness�R�}���h */
      case CTEST_MODE_AUD_LOUD_ONOFF:
         Aud_test_edit_loud( RcvCmd, RcvBuf );
         break;
      
      /* Mute�R�}���h */
      case CTEST_MODE_AUD_MUTE_ONOFF:
         Aud_test_edit_mute( RcvCmd, RcvBuf );
         break;
      
      /* Beep�R�}���h */
      case CTEST_MODE_OTHER_BEEP_ONOFF:
         Aud_test_edit_beep( RcvBuf );
         break;
         
      default:
         break;
   }
}

/******************************************************************************
   �֐���      �FAud_test_edit_vol
   ����      �FBYTE RcvCmd   => ��M�R�}���h
              BYTE *RcvBuf   => ��M�o�b�t�@
   �߂�l      �F����
   �@�\�T��   �FAudio�e�X�g���[�h(Volume Edit)
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
******************************************************************************/
static void Aud_test_edit_vol( BYTE RcvCmd, BYTE *RcvBuf )
{
   BYTE cur_vol_step;
   BYTE tar_vol_step;
   BYTE snd_flg;
   
   /* Fail Safe */
   snd_flg = ON;
   
   switch ( RcvCmd )
   {
      case CTEST_MODE_AUD_DIR_VOL:               /* Direct Volume   */
         if ( RcvBuf[0] <= CAUD_VOL_MAX )         /* �͈̓`�F�b�N      */
         {
            /* ����Step�A�^�[�Q�b�gStep�擾 */
            cur_vol_step = Aud_audio_info.vol_step;
            tar_vol_step = RcvBuf[0];
            
            /* Volume mute���͑��M�֎~ */
            if ((( cur_vol_step > CAUD_VOL_MIN ) && ( tar_vol_step == CAUD_VOL_MIN )) ||
               (( cur_vol_step == CAUD_VOL_MIN ) && ( tar_vol_step > CAUD_VOL_MIN )) )
            {
               snd_flg = OFF;
            }
            
            /* �^�[�Q�b�g��Volume Step�i�[ */
            Aud_audio_info.vol_step = tar_vol_step;
         }
         else
         {
            /* �͈͒��߂͑��M�֎~ */
            snd_flg = OFF;
         }
         break;
         
      default:   /* Exception */
         snd_flg = OFF;
         break;
   }
   
   /* ���M���̂� */
   if ( snd_flg == ON )
   {
      Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
   }
}

/******************************************************************************
   �֐���      �FAud_test_edit_tone
   ����      �FBYTE RcvCmd   => ��M�R�}���h
   �߂�l      �F����
   �@�\�T��   �FAudio�e�X�g���[�h(Tone Edit)
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
******************************************************************************/
static void Aud_test_edit_tone( BYTE RcvCmd )
{
   BYTE tar_bas_step;
   BYTE tar_tre_step;
   BYTE cur_bas_step;
   BYTE cur_tre_step;
   BYTE snd_flg;
   BOOL cnt_det;
   
   /* Fail Safe */
   cnt_det = FALSE;
   snd_flg = ON;
   
   /* Step�擾 */
   cur_bas_step = Aud_audio_info.bas_step;
   cur_tre_step = Aud_audio_info.tre_step;
   
   switch ( RcvCmd )
   {
      case CTEST_MODE_AUD_BAS_MAX:      /* Bass Max */
         tar_bas_step = CAUD_BAS_MAX;
         tar_tre_step = cur_tre_step;
         
         /* �Z���^�[�����̓Z���^�[�𑗐M���Ă��� */
         if ( cur_bas_step < CAUD_BAS_CNT )
         {
            cur_bas_step = CAUD_BAS_CNT;
            cnt_det = TRUE;
         }
         break;
         
      case CTEST_MODE_AUD_BAS_MIN:      /* Bass Min */
         tar_bas_step = CAUD_BAS_MIN;
         tar_tre_step = cur_tre_step;
         
         /* �Z���^�[���߂̓Z���^�[�𑗐M���Ă��� */
         if ( cur_bas_step > CAUD_BAS_CNT )
         {
            cur_bas_step = CAUD_BAS_CNT;
            cnt_det = TRUE;
         }
         break;
         
      case CTEST_MODE_AUD_TRE_MAX:      /* Treble Max */
         tar_bas_step = cur_bas_step;
         tar_tre_step = CAUD_TRE_MAX;
         
         /* �Z���^�[�����̓Z���^�[�𑗐M���Ă��� */
         if ( cur_tre_step < CAUD_TRE_CNT )
         {
            cur_tre_step = CAUD_TRE_CNT;
            cnt_det = TRUE;
         }
         break;
         
      case CTEST_MODE_AUD_TRE_MIN:      /* Treble Min */
         tar_bas_step = cur_bas_step;
         tar_tre_step = CAUD_TRE_MIN;
         
         /* �Z���^�[���߂̓Z���^�[�𑗐M���Ă��� */
         if ( cur_tre_step > CAUD_TRE_CNT )
         {
            cur_tre_step = CAUD_TRE_CNT;
            cnt_det = TRUE;
         }
         break;
         
      case CTEST_MODE_AUD_BSTR_CEN:      /* Bas/Tre Center */
         tar_bas_step = CAUD_BAS_CNT;
         tar_tre_step = CAUD_TRE_CNT;
         break;
         
      default:                     /* Exception */
         snd_flg = OFF;
         break;
   }
   
   /* ���M���̂� */
   if ( snd_flg == ON )
   {
      if( cnt_det == TRUE )
      {
         /* 0dB�o�R���M */
         Aud_audio_info.bas_step = cur_bas_step;
         Aud_audio_info.tre_step = cur_tre_step;

         Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
      }
      
      /* �^�[�Q�b�g���M */
      Aud_audio_info.bas_step = tar_bas_step;   
      Aud_audio_info.tre_step = tar_tre_step;

      Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
   }
}

/******************************************************************************
   �֐���      �FAud_test_edit_sp
   ����      �FBYTE RcvCmd   => ��M�R�}���h
   �߂�l      �F����
   �@�\�T��   �FAudio�e�X�g���[�h(Speaker Edit)
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
******************************************************************************/
static void Aud_test_edit_sp( BYTE RcvCmd )
{
   BYTE tar_bal_step;
   BYTE snd_flg;
   
   /* Fail Safe */
   snd_flg = ON;

   switch ( RcvCmd )
   {
      case CTEST_MODE_AUD_BAL_RMAX:         /* Balance R Max */
         tar_bal_step = CAUD_BAL_MAX;
         break;
         
      case CTEST_MODE_AUD_BAL_LMAX:         /* Balance L Max    */
         tar_bal_step = CAUD_BAL_MIN;
         break;
         
      case CTEST_MODE_AUD_BAL_CEN:         /* Balance Center   */
         tar_bal_step = CAUD_BAL_CNT;
         break;

      default:                        /* Exception      */
         snd_flg = OFF;
         break;
   }
   
   /* ���M���̂� */
   if ( snd_flg == ON )
   {
      /* �^�[�Q�b�g���M */
      Aud_audio_info.bal_step = tar_bal_step;

      Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
   }
}

/******************************************************************************
   �֐���      �FAud_test_edit_loud
   ����      �FBYTE RcvCmd   => ��M�R�}���h
              BYTE *RcvBuf   => ��M�o�b�t�@
   �߂�l      �F����
   �@�\�T��   �FAudio�e�X�g���[�h(Loudness Edit)
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
******************************************************************************/
static void Aud_test_edit_loud( BYTE RcvCmd, BYTE *RcvBuf )
{
   BYTE snd_flg;

   /* Fail Safe */
   snd_flg = ON;
   
   switch ( RcvCmd )
   {
      case CTEST_MODE_AUD_LOUD_ONOFF:
         if ( RcvBuf[0] == ON )         /* Loud On   */
         {
            Aud_diag_loud.test_loud = CAUD_LOUD_STA_ON;
         }
         else if(RcvBuf[0] == OFF)      /* Loud Off   */
         {
            Aud_diag_loud.test_loud = CAUD_LOUD_STA_OFF;
         }
         else
         {
            snd_flg = OFF;
         }
         break;
         
      default:   /* Exception */
         snd_flg = OFF;
         break;
   }
   
   /* ���M���̂� */
   if ( snd_flg == ON )
   {
      /* �e�X�g�D�� */
      Aud_diag_loud.prio_diag = CAUD_PRIO_TEST;
      
      Aud_ctrl_ASP( CAUD_ASP_ALL_AUDIO );
   }
}

/******************************************************************************
   �֐���      �FAud_test_edit_mute
   ����      �FBYTE RcvCmd   => ��M�R�}���h
              BYTE *RcvBuf   => ��M�o�b�t�@
   �߂�l      �F����
   �@�\�T��   �FAudio�e�X�g���[�h(MUTE Edit)
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
******************************************************************************/
static void Aud_test_edit_mute( BYTE RcvCmd, BYTE *RcvBuf )
{
   switch ( RcvCmd )
   {
      case CTEST_MODE_AUD_MUTE_ONOFF:
         if ( RcvBuf[0] == ON )         /* SysMute On   */
         {
            MIO_OUT_pSYS_MUTE( LOW );
         }
         else if ( RcvBuf[0] == OFF )   /* SysMute Off   */
         {
            MIO_OUT_pSYS_MUTE( HIGH );
         }
         else   /* QAC�Ή� */
         {
            ;
         }
         break;
      
      default:   /* Exception */
         break;
   }
}

/******************************************************************************
   �֐���      �FAud_test_edit_beep
   ����      �FBYTE *RcvBuf   => ��M�o�b�t�@
   �߂�l      �F����
   �@�\�T��   �FAudio�e�X�g���[�h(BEEP Edit)
------------------------------------------------------------------------------
   �C������
    �ԍ�      ���t      �S����      �C�����e
   �y000�z      12/09/25   leejh      �V�K�쐬
******************************************************************************/
static void Aud_test_edit_beep( BYTE *RcvBuf )
{
   switch ( RcvBuf[0] )
   {
      case 0x20:      /* Beep   */
         if ( RcvBuf[1] == ON )            /* Beep On         */
         {
            Io_beep_start();            /* BEEP�J�n         */
         }
         else if ( RcvBuf[1] == OFF )      /* Beep Off         */
         {
            Io_beep_stop();               /* BEEP��~         */
         }
         else   /* QAC�Ή� */
         {
            ;
         }
         break;
      
      case 0x21:      /* Alarm */
         if ( RcvBuf[1] == ON )            /* Alarm On         */
         {
            
#if _POWER_DIAG_
            Aud_set_beep(CAUD_REQ_BEEP_ALARM);
#else
            MIO_OUT_pALARM( HIGH );         /* Alarm�[�q High   */
            Io_beep_start();            /* BEEP�J�n         */
#endif
         }
         else if ( RcvBuf[1] == OFF )      /* Alarm Off      */
         {         
#if _POWER_DIAG_
            Aud_cancel_beep();
#else
            MIO_OUT_pALARM( LOW );         /* Alarm�[�q Low   */
            Io_beep_stop();               /* BEEP��~         */
#endif
         }
         else   /* QAC�Ή� */
         {
            ;
         }
         break;
         
      default:   /* Exception */
         break;
   }
}
#endif
#undef _AUD_MAIN_C_

