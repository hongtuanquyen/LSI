/******************************************************************************
   �N�����I���a����
	�v���W�F�N�g��	: 	2012�NAM/FM���W�I�Ή����f���J��
	�t�@�C����		: 	aud_def.h
	���W���[����	: 	Audio���W���[��
	�@�\�T��		: 	���W���[��������`�̃w�b�_�[�t�@�C��
-------------------------------------------------------------------------------
�C������
	�ԍ�		���t		�S����			�C�����e
	�y000�z		12/07/13	leejh			�V�K�쐬
	�y001�z		12/09/25	leejh			AUD_AUDIO_POWER_MODE(�폜)
											 ��aud_ext.h�Ɉړ�
 											AUD_LOUD_STATUS(�ǉ�)
											AUD_PROI_DIAG(�ǉ�)
 											TAUD_DIAG_LOUD(�ǉ�)
 											Aud_diag_loud(�ǉ�)
********************************************************************************/

#ifndef		_AUD_DEF_H_
#define		_AUD_DEF_H_

#include	"../model.h"

/*---------------------------------------------------------------------------*
 *�����萔��`									 						 	 *
 *---------------------------------------------------------------------------*/
/* ASP(Audio Sound Processor)���䏈���ւ̖��ߒ�`  */
enum AUD_ASP_INSTRUCTION {
	CAUD_ASP_SYS_RESET	=1		,	/* ASP�V�X�e�����Z�b�g					 */
	CAUD_ASP_INIT_AUDIO			,	/* ASP�f�[�^�����ݒ�					 */
	CAUD_ASP_VOL				,	/* volume����							 */
	CAUD_ASP_BAS				,	/* bass����								 */
	CAUD_ASP_TRE				,	/* treble����							 */
	CAUD_ASP_BAL				,	/* balance����							 */
	CAUD_ASP_FAD				,	/* fader����							 */
	CAUD_ASP_SMUTE_ON			,	/* soft mute on							 */
	CAUD_ASP_SMUTE_OFF			,	/* soft mute off						 */
	CAUD_ASP_SRC_CHG			,	/* ���[�h�؊���							 */
	CAUD_ASP_ALL_AUDIO			,	/* �S�I�[�f�B�I�f�[�^�ݒ�				 */
	CAUD_ASP_INSTRUCTION_END
};

/* AUDIO�����v���萔��` */
enum AUD_ADJUST_ID {
	CAUD_ADJ_VOL	= 0x01	,	/*  1:���C�����ʒ���		 */
	CAUD_ADJ_BAS			,	/*  2:BASS����				 */
	CAUD_ADJ_TRE			,	/*  3:TREBLE����			 */
	CAUD_ADJ_BAL			,	/*  4:BALANCE����			 */
	CAUD_ADJ_FAD			,	/*  5:FADER����				 */
	CAUD_ADJ_END				/*	XX:�I�[					*/	
};

/* �^�C�}��� */
enum AUD_AUDIO_TIMER_TYPE_DEF{
	CAUD_CNT_01MS	,		/* 1ms�^�C�}�J�E���^	 */
	CAUD_CNT_05MS	,		/* 5ms�^�C�}�J�E���^	 */
	CAUD_CNT_10MS	,		/* 10ms�^�C�}�J�E���^	 */
	CAUD_CNT_25MS	,		/* 25ms�^�C�}�J�E���^	 */
	CAUD_CNT_1SEC			/* 1000ms�^�C�}�J�E���^	 */
};

/* 1ms�^�C�} */
enum AUD_AUDIO_TIMER_01_DEF{
	CAUD_C01MS_DUMMY	,			/* reserved			*/
	CAUD_C01MS_MAX					/* �Ō�				*/
};

/* 5ms�^�C�} */
enum AUD_AUDIO_TIMER_05_DEF{
	CAUD_C05MS_DUMMY			,	/* reserved				*/
	CAUD_C05MS_ASP_WAIT			,	/* ASP wait�^�C�}		*/
	CAUD_C05MS_PMEUT_TIM		,	/* Premute Wait�^�C�}	*/
	CAUD_C05MS_BEEP_TIM			,	/* BEEP Wait�^�C�}		*/
	CAUD_C05MS_AMUTE_END_TIM	,	/* Mute End�^�C�}		*/
	CAUD_C05MS_MAX					/* �Ō�					*/
};

/* 10ms�^�C�} */
enum AUD_AUDIO_TIMER_10_DEF{
	CAUD_C10MS_DUMMY			,	/* reserved				*/
	CAUD_C10MS_BAND_CHG_WAIT	,	/* BAND�؊����^�C�}		*/
	CAUD_C10MS_BEEP_TIM			,	/* BEEP Wait�^�C�}		*/
	CAUD_C10MS_MAX					/* �Ō�					*/
};

/* 25ms�^�C�} */
enum AUD_AUDIO_TIMER_25_DEF{
	CAUD_C25MS_DUMMY			,	/* reserved				*/
	CAUD_C25MS_MCNT_ON			,	/* MainCnt On Wait�^�C�}*/
	CAUD_C25MS_MAX					/* �Ō�					*/
};

/* 1000ms�^�C�} */
enum AUD_AUDIO_TIMER_1S_DEF{
	CAUD_C1SEC_DUMMY,				/* reserved	*/
	CAUD_C1SEC_MAX					/* �Ō�		*/
};

/* �V�X�e���R�}���hID */
enum AUD_EVENT_ID_DEF{
	CAUD_EVT_MAIN		=0x01	,	/* MAIN�C�x���g				*/
	CAUD_EVT_INITIAL			,	/* INITIAL�C�x���g			*/
	CAUD_EVT_BATT_OFF			,	/* BATT OFF�C�x���g			*/
	CAUD_EVT_PWR_ON				,	/* POWER ON�C�x���g			*/
	CAUD_EVT_PWR_OFF			,	/* POWER OFF�C�x���g		*/
	CAUD_EVT_MCNT_ON			,	/* MAINCNT ON�C�x���g		*/
	CAUD_EVT_MCNT_OFFREQ		,	/* MAINCNT OFF REQ�C�x���g	*/
	CAUD_EVT_MCNT_OFF			,	/* MAINCNT OFF�C�x���g		*/
	CAUD_EVT_MODE_CHG			,	/* MODE CHANGE�C�x���g		*/
	CAUD_EVT_TIMER_001M			,	/* 1ms�^�C�}�C�x���g		*/
	CAUD_EVT_TIMER_005M			,	/* 5ms�^�C�}�C�x���g		*/
	CAUD_EVT_TIMER_010M			,	/* 10ms�^�C�}�C�x���g		*/
	CAUD_EVT_TIMER_025M			,	/* 25ms�^�C�}�C�x���g		*/
	CAUD_EVT_TIMER_050M			,	/* 50ms�^�C�}�C�x���g		*/
	CAUD_EVT_TIMER_100M			,	/* 100ms�^�C�}�C�x���g		*/
	CAUD_EVT_TIMER_125M			,	/* 125ms�^�C�}�C�x���g		*/
	CAUD_EVT_TIMER_500M			,	/* 500ms�^�C�}�C�x���g		*/
	CAUD_EVT_TIMER_001S				/* 1sec�^�C�}�C�x���g		*/
};

/* ���[�hID�萔��` */
enum AUD_MODE_ID_DEF {
	CAUD_MODE_TUNER_AM	= 0	,	/* 0�FTuner AM	*/
	CAUD_MODE_TUNER_FM		,	/* 1�FTuner FM	*/
	CAUD_MODE_AUX1			,	/* 2�FAUX		*/
	CAUD_MODE_MAX				/* Max			*/
};

#if _TEST_MODE_
/* LOUD�X�e�[�^�X�萔��`(�e�X�g�A�B���L�[��p) */
typedef enum {
	CAUD_LOUD_STA_UNKNOWN	= 0	,	/* 0�F�s��		*/
	CAUD_LOUD_STA_OFF			,	/* 1�FLoud Off	*/
	CAUD_LOUD_STA_ON			,	/* 2�FLoud On	*/
	CAUD_LOUD_STA_MAX				/* 3�FMax		*/
} AUD_LOUD_STATUS;

/* LOUD�f�f�D���`(�e�X�g�A�B���L�[��p) */
typedef enum {
	CAUD_PRIO_UNKNOWN		= 0	,	/* 0�F�s��		*/
	CAUD_PRIO_TEST				,	/* 1�F�e�X�g	*/
	CAUD_PRIO_SECRET			,	/* 2�F�B���L�[	*/
	CAUD_PRIO_MAX					/* 3�FMax		*/
} AUD_PROI_DIAG;
#endif

/*---------------------------------------------------------------------------*
 *	�\���̒�`															 	 *
 *---------------------------------------------------------------------------*/
/* AUDIO��񃊃X�g�\���̒�` */
typedef struct {
	BYTE			asp_cmd;		/* AUDIO�f�o�C�X�R�}���h		 */
	BYTE			curt_mode;		/* ���݂̃��[�h					 */
	BYTE			prev_mode;		/* �O��̃��[�h					 */
	BYTE			curt_src;		/* ���݂̃T�E���h�\�[�X			 */
	BYTE			prev_src;		/* �O��̃T�E���h�\�[�X			 */
	BYTE			curt_band;		/* TUNER BAND					 */
	BYTE			vol_step;		/* volume step					 */
	BYTE			bas_step;		/* bass	step					 */
	BYTE			tre_step;		/* treble step					 */
	BYTE			bal_step;		/* balance step					 */
	BYTE			fad_step;		/* fader step					 */
	BYTE			pre_kvol;		/* KEY MUTE�J�n�O��volume step	 */
	BYTE			pre_evol;		/* �O��MUTE�J�n�O��volume step	 */
} TAUD_AUDIO_INFO;

/* ���C�����[�hID�ϊ��p�̍\���̒�` */
typedef struct {
	BYTE	mid;	/* ���C�����W���[������`���Ă��郂�[�hID	 */
	BYTE	aid;	/* E-VOL���W���[�����������[�hID			 */
} TAUD_MODE_CONV_DEF;

/* MUTE���̍\���̒�` */
typedef struct {
	BYTE	sys_mute	:1;		/* sys mute	0: off		1: on		 */
	BYTE	data_mute	:1;		/* data mute0:off		1: on		 */
	BYTE	curt_mute	:1;		/* mute���	0: mute off	1: mute on	 */
	BYTE	onoff_seq	:1;		/* mute����	0: off����	1: on����	 */
	BYTE	reserved	:4;		/* reserved							 */
} TAUD_MUTE_FLAG;

/* �������W���[���\���̋��ʒ�` */
typedef struct {
	BYTE	max;			/* �J�E���^�z��̍ő吔			 */
	BYTE	reserved;		/* padding						 */
	BYTE	*cntr;			/* �J�E���^�z��̐擪�A�h���X	 */
} TAUD_TIMER_INFO;

#if _TEST_MODE_
/* LOUD�f�f�\���̒�` */
typedef struct {
	AUD_LOUD_STATUS		test_loud;			/* �e�X�g				 */
	AUD_LOUD_STATUS		secret_loud;		/* �B���L�[				 */
	AUD_PROI_DIAG		prio_diag;			/* �f�f�D��				 */
} TAUD_DIAG_LOUD;
#endif

/*---------------------------------------------------------------------------*
 *	�ϐ���`															 	 *
 *---------------------------------------------------------------------------*/
TAUD_MUTE_FLAG			aud_mute_state;		/* mute�t���O��`�ϐ�		 */
BYTE					Aud_power_mode;		/* AUDIO�d��mode			 */
BYTE					Aud_dis_audio;		/* ���M�֎~					 */

#if _TEST_MODE_
TAUD_DIAG_LOUD			Aud_diag_loud;		/* LOUD�f�f�Ǘ��ϐ�			 */
#endif

#endif
