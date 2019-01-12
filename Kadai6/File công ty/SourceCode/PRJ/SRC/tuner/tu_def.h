/******************************************************************************
..     �N�����I���a����                    2007����

      �v���W�F�N�g��   :
      �t�@�C����      : tu_def.h
      �@�\         : �@TunerӼޭ�ٌ^�A�萔��`ͯ��̧��
------------------------------------------------------------------------------
      �C������   �y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051125]   �V�K�쐬
[sun051220] TTU_BAND��`н�C���A׽ē�����7�Ǖ��ł����A6�Ǖ�������`���Ă��Ȃ�����
[zhang070313] clarion �Ή��C��
[matu120903] S-meter���� Local�l�ǉ� 
[matu120926] �e�X�g���[�h�Ή�
******************************************************************************/
#ifndef      _TU_DEF_H_
#define      _TU_DEF_H_

#include   "../model.h"

/*----------------------------------------------------
   �萔��`
----------------------------------------------------*/

#define      CTU_E_TEST         0x00         /*   TEF6606�����l��E�����e�X�g�p   */

#if   (CTU_E_TEST == 0x01)
#define      CTU_DEST   8            /* Tuner�d���� */
#else
#define      CTU_DEST   7            /* Tuner�d���� */
#endif
#define      CTU_BAND   3            /* Band 3Band */
#define      CTU_PRESET   6            /*Preset 6�Ǖ�*/

#define      CTU_MW      0            /*MW Band*/
#define      CTU_LW      1            /*LW Band*/

#define      CTU_FMLW   1            /* FM LW Band���� */
#define      CTU_AMLW   2            /* AM LW Band���� */

#define      CTU_DEST_JPN_A      0      /* ���{�d���� AM���W�I */
#define      CTU_DEST_JPN_FA      1      /* ���{�d���� AM/FM���W�I */
#define      CTU_DEST_USA      2      /* �k��/����Ďd���� */
#define      CTU_DEST_CSA      3      /* ����Ďd���� */
#define      CTU_DEST_OCA      4      /* �I�Z�A�j�A�d���� */
#define      CTU_DEST_AAM      5      /* �A�W�A/�A�t���J/�����d���� */
#define      CTU_DEST_THR      6      /* ��O�n��d���� */
#define      CTU_DEST_MAX      7      /* MAX */

#if   (CTU_E_TEST == 0x01)
#define      CTU_DEST_E   7
#endif

#define      CTU_DOWN   0
#define      CTU_UP      1
#define      CTU_PREMUTE         50/5+1            /* Tuner PreMute 50ms timer */
#define      CTU_SEEKMUTE      100/5+1            /* Tuner Seek PreMute 100ms timer */
#define      CTU_PWRWAIT         800/5+1            /* PowerOn��PLL��Ă܂ł̈���800mstimer */
#define      CTU_SCANWAIT_TIM   5000/100         /* SCAN��M���� 5s timer */
#define      CTU_PSTWAIT_TIM      10000/100         /* Preset �������ݑ҂����� 10s timer */

#define      CTU_AFTER100   11         /* AfterMute100mstimer:100/10+1 */
#define      CTU_AFTER250   250/10+1   /* AfterMute100mstimer:250/10+1 */
#define      CTU_AFTER300   31         /* AfterMute300mstimer:300/10+1 */
#define      CTU_AFTER500   500/10+1         /* AfterMute500mstimer */
#define      CTU_AFTER1000   101         /* AfterMute100mstimer:1000/10+1 */
#define      CTU_AFTER1500   1500/10+1         /* AfterMute1500mstimer */


#define      CTU_NOTEDGE         0
#define      CTU_BNDEDGE         1               /* Tuner Up/Down BandEdge */
#define      CTU_LMEDGE         2               /* Tuner Up/Down LW/MW Edge */

/* TESTMODE�Ή� *//* [matu120926] */
/* LO/DX�ؑ֕ϐ��g�p */
#define      CTU_TEST_NO         0
#define      CTU_TEST_LOCAL      1               /* DX/LO �ؑ� LOCAL�ݒ�v�� */
#define      CTU_TEST_DX         2               /* DX/LO �ؑ� DX�ݒ�v�� */
/* Q-INFO�`�F�b�N�� */
#define      CTU_SEEK_NUM      5
#define      CTU_TEST_NUM      100
/*��TESTMODE�Ή�*//* [matu120926] */

#define      CTU_I2C_STOP      0
#define      CTU_I2C_IDLE      1
#define      CTU_I2C_BUSY      2
#define      CTU_I2C_ERR         3

#define      CTU_PARA_TUNER0         0x00
#define      CTU_PARA_TUNER1         0x01
#define      CTU_PARA_TUNER2         0x02
#define      CTU_PARA_RADIO         0x03
#define      CTU_PARA_SOFTMUTE0      0x04
#define      CTU_PARA_SOFTMUTE1      0x05
#define      CTU_PARA_SOFTMUTE_AM   0x06
#define      CTU_PARA_HIGHFCUT0      0x07
#define      CTU_PARA_HIGHFCUT1      0x08
#define      CTU_PARA_HIGHFCUT2      0x09
#define      CTU_PARA_STEREO0      0x0A
#define      CTU_PARA_STEREO1      0x0B
#define      CTU_PARA_STEREO2      0x0C
#define      CTU_PARA_CONTROL      0x0D
#define      CTU_PARA_LEVEL_OFFSET   0x0E
#define      CTU_PARA_AM_LNA         0x0F
#define      CTU_PARA_LEN         0x10

#if (_MODEL_EEPROM_ == 1)
#define      CTU_EEPWR_PST         1

#define      CTU_EEPOFS_PREMEM         0x20   /*   Tuner��T�ԃ������f�[�^�ۑ��̈�   */
#if 0
#define      CTU_EEPADR_PSTLEN         36      /* 2byte * 6pch * 3band */
#else
#define      CTU_EEPADR_PSTLEN         36      /* 16BYTE * 3�s */
#endif

#define      CTU_EEPADR_PARA            0x60         /*   Tuner�e�d���������R�}���h�ۑ��̈�J�n�A�h���X   */

#define      CTU_EEPOFS_CHKCOEF         (CTU_PARA_LEN * (2) * CTU_DEST_MAX)
#define      CTU_EEPOFS_FM20DB         CTU_EEPOFS_CHKCOEF
#define      CTU_EEPOFS_FM40DB         (CTU_EEPOFS_FM20DB + 1)
#define      CTU_EEPOFS_AM30DB         (CTU_EEPOFS_FM20DB + 9)
#define      CTU_EEPOFS_AM50DB         (CTU_EEPOFS_AM30DB + 1)

/*****   EEPROM ���ڽ�l ******/
#define      CTU_EEP_ADR_FMSMETER   0x00         //FM SҰ�(DSP-PJ���s�d�l����̱��ڽ)
#define      CTU_EEP_ADR_FMSMETER20DB 0x00         //FM 20dB����SҰ�(DSP-PJ���s�d�l����̱��ڽ)
#define      CTU_EEP_ADR_FMSMETER40DB 0x01         //FM 40dB����SҰ�(DSP-PJ���s�d�l����̱��ڽ)

#define      CTU_EEP_ADR_AMSMETER   0x02         //AM SҰ�(DSP-PJ���s�d�l����̱��ڽ)
#define      CTU_EEP_ADR_AMSMETER20DB 0x02         //AM 30dB����SҰ�(DSP-PJ���s�d�l����̱��ڽ)
#define      CTU_EEP_ADR_AMSMETER40DB 0x03         //AM 50dB����SҰ�(DSP-PJ���s�d�l����̱��ڽ)

#define      CTU_EEP_ADR_AMSMLOLV   0x06         /* [matu120903] */
#define      CTU_EEP_ADR_AMSMDXLV   0x07
#define      CTU_EEP_ADR_FMSMDXLV   0x09

#define      CTU_EEP_ADR_FMWAM      0x0B
#define      CTU_EEP_ADR_FMUSN      0x0C
/* EEPROM�e�[�u���擪�o�b�t�@ */
#define      CTU_EEP_ADR_Q_INF1      0x00         /* �A���C�����g�ADX�AWAN�AUSN�ݒ� */
#define      CTU_EEP_ADR_PARA_FMU   0x10         /* FM USA�p�����[�^�ݒ� */
#define      CTU_EEP_ADR_PARA_FMJ   0x20         /* FM JAPAN�p�����[�^�ݒ� */
#define      CTU_EEP_ADR_PARA_AM      0x30         /* AM JAPAN/USA�p�����[�^�ݒ� */
#define      CTU_EEP_ADR_Q_INF2      0x40         /* IF-COUNT�AWAIT TIMER�ݒ� */
#define      CTU_EEP_ADR_Q_INF3      0x50         /* S-meter�Anoise�ݒ� */
/* Check Sam */
#define      CTU_EEP_ADR_Q_INF1_CHK      BIN_00000001
#define      CTU_EEP_ADR_PARA_FM_CHK      BIN_00000010
#define      CTU_EEP_ADR_PARA_AM_CHK      BIN_00000100
#define      CTU_EEP_ADR_Q_INF2_CHK      BIN_00001000
#define      CTU_EEP_ADR_Q_INF3_CHK      BIN_00010000

#define      CTU_EEP_ADR_AM_IFROUGH   0x43
#define      CTU_EEP_ADR_AM_IFDETAIL   0x44
#define      CTU_EEP_ADR_FM_IFROUGH   0x45
#define      CTU_EEP_ADR_FM_IFDETAIL   0x46

#define      CTU_EEP_ADR_FM_Q_WAIT   0x47
#define      CTU_EEP_ADR_AM_Q_WAIT   0x48

#define      CTU_EEP_ADR_AM_PERC_LV   0x51
#define      CTU_EEP_ADR_FM_PERC_LV   0x52
#define      CTU_EEP_ADR_FM_PERC_NS   0x53

#endif
/*----------------------------------------------------
   �^��`
----------------------------------------------------*/
typedef union {
   WORD   word;
   struct
   {
      BYTE   byte0;
      BYTE   byte1;
   }byte;
}WORD_BYTE;

/* Tuning�W����` */
typedef  struct {
   WORD   min_freq;         /* �ŏ����g�� FM�P��10kHz  WB/AM�P��kHz */
   WORD   max_ch;            /* Max����ِ� */
   WORD   min_ch;            /* Min����� */
   BYTE   step;            /* �ï�� �P�ʁFkHz */
   BYTE   wait_edge;         /* Wait���[����޴���] �P�ʁF5ms */
   BYTE   c_wait;            /* �A�������޳�WAIT��� */
   }TTU_INIT;

/* �d��������Tuning�W����` */
typedef struct {
   BYTE      mode;
   BYTE      dummy;
   TTU_INIT   fm;
   TTU_INIT   fm_lw;
   TTU_INIT   am;
   TTU_INIT   am_lw;
   }TTU_DEST;

/* ���g����{����` */
typedef struct {
   WORD   freq;            /* ���g�� */
   }TTU_CH;

/* Band���̊�{����` */
/* [sun051220]��`н�C���A׽ē�����7�Ǖ��ł����A6�Ǖ�������`���Ă��Ȃ����� */
typedef struct {
   TTU_CH   pset[CTU_PRESET+1];   /* 0: ׽Ď��g���G���A�A 1�`6�FPreset�G���A      0: last frequency area, 1 to 6: Preset area */
   BYTE   pstno;            /* Band׽�PresetNo */
   BYTE   bno;            /* Band No */
   BYTE   band;            /* FM/AM��� */
   }TTU_BAND;

/* Controlð��ْ�` */
typedef  struct{
   BYTE (*func)(void);
   BYTE ok_seq;
   BYTE ng_seq;
   }TTU_CONTTBL;

/* �ʐM��{����` */
typedef union {
   BYTE   subaddr;
   struct
   {
      BYTE   addr:4;
      BYTE   dumy:1;
      BYTE   mode:3;
   }bit;
}TRAS_MSA;

typedef struct {   
   BYTE txbuf[16];      /*���Mbuffer         */
   BYTE rxbuf[5];      /*��Mbuffer         */
   TRAS_MSA msa;         /*I2C���M��MSA         */
   BYTE length;      /*read/write�f�[�^��   */
   BYTE status;      /*I2C���            */
   }TTU_I2C;

/*   Average S-Meter Check��   */
typedef struct {
   BYTE   Status;      //   ���������ð���ް�
   BYTE   Smeter;      //   �d�E���x�ް�
   BYTE   Mnoise:4;   //   ����߽ɲ���ް�
   BYTE   Rnoise:4;   //   �א�ɲ���ް�
   BYTE   IFval:5;
   BYTE   IFneg:1;
   BYTE   IFsts:2;   //   IFCOUNTER Status
   BYTE   Bwidth;      //   IF�ш敝�ް�
} TTU_QINFO,*PTTU_QINFO;

typedef struct {
   BYTE   cnt;            //   �v���� 1:�v������
   BYTE   data;            //   �ݐ��ް�
   BYTE   rem;            //   �v�Z��]
} TTU_QI_CALC;

typedef union {
   WORD   word;
   BYTE   byte[2];
} CHG_WORD;

typedef   struct {
   BYTE   sm_20db;
   BYTE   sm_40db;
   BYTE   sm_lodb;   /* [matu120903] */
   BYTE   sm_dxdb;
   BYTE   if_rough;
   BYTE   if_detail;
   BYTE   wait_tm;
   BYTE   prec_sm;
   BYTE   prec_noise;
   BYTE   usn_max;
   BYTE   wam_max;
} TTU_CHKCOEF;

/*   Average S-Meter Check��   */
   
/*----------------------------------------------------
   MainӰ�ޒ�`

����TTU_CONTTBL   * _rom TU_SeqTBL[]�̏��ԂƘA�����Ă��܂��A
�ύX���ɕK���m�F��
----------------------------------------------------*/
enum
{
   CTU_NOP   = 0,         /* 0 */
   CTU_M_PWRON,         /* 1 */
   CTU_M_LSTCAL,         /* 2 */
   CTU_M_BANDCHG,         /* 3 */
   CTU_M_MAN,            /* 4 */
   CTU_M_CMAN,            /* 5 */
   CTU_M_SEEK,            /* 6 */
   CTU_M_PSTRD,         /* 7 */
   CTU_M_PSTWT,         /* 8 */
   CTU_M_AS,            /* 9 */
   CTU_M_SCAN,            /* a */
   CTU_M_ISRRD,         /* b */
   CTU_M_MAX            /* c */
};

/*----------------------------------------------------
   Main���[�h���V�[�P���X��`
----------------------------------------------------*/
#define      CTU_SCANWAIT_SEQ      0x05         /* SCAN5s��M��*/
#endif
