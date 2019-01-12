/******************************************************************************
   �N�����I���a����
	�v���W�F�N�g��	: 	2012�NAM/FM���W�I�Ή����f���J��
	�t�@�C����		: 	aud_ext.h
	���W���[����	: 	Audio���W���[��
	�@�\�T��		: 	�O�����W���[�������C���^�[�t�F�[�X�֐���`
						�w�b�_�[�t�@�C��
-------------------------------------------------------------------------------
�C������
	�ԍ�		���t		�S����			�C�����e
	�y000�z		12/07/13	leejh			�V�K�쐬
	�y001�z		12/09/11	leejh			AUD_BEEP_STATUS(�ύX)
											 ��CAUD_BEEP_STA_ALARM_M_NCONT(�ǉ�)
 											 ��CAUD_BEEP_STA_ALARM_M_CONT(�ǉ�)
 											 ��CAUD_BEEP_STA_MULTIPLE_M_NCONT(�ǉ�)
 											 ��CAUD_BEEP_STA_MULTIPLE_M_CONT(�ǉ�)
	�y002�z		12/09/25	leejh			AUD_AUDIO_POWER_MODE(�ǉ�)
********************************************************************************/
#ifndef		_AUD_EXT_H_
#define		_AUD_EXT_H_

#include	"../model.h"

#undef		EXT
#ifdef		_AUD_EXT_C_
#define		EXT
#else
#define		EXT extern
#endif

/*---------------------------------------------------------------------------*
 *	�O�����W���[�������萔��`											 	 *
 *---------------------------------------------------------------------------*/
/* ������萔��` */
enum AUD_AUDIO_INFO_CTRL_DEF {
	CAUD_STP_DOWN		,		/* �X�e�b�vDOWN			*/
	CAUD_STP_UP			,		/* �X�e�b�vUP			*/
	CAUD_STP_CENTER		,		/* �X�e�b�vCENTER		*/
	CAUD_STP_MAX		,		/* �X�e�b�vMAX			*/
	CAUD_STP_MIN		,		/* �X�e�b�vMIN			*/
	CAUD_STP_DEF		,		/* �o�׎��ݒ�			*/
	CAUD_STP_NOP = 0x0F			/* non operation		*/
};

/* �߂�l�`���萔��` */
enum AUD_RET_VAL_DEF {
	CAUD_HEX			,		/* HEX�`���Œʒm */
	CAUD_BCD					/* BCD�`���Œʒm */
};

/* MUTE���W���[��ID�萔��` */
enum AUD_MUTE_MODULE_ID_DEF{ 
	CAUD_MDL_AUDIO	= 0x01	,	/*	1:AUDIO MODULE		 */
	CAUD_MDL_AUX			,	/*	2:AUX MODULE		 */
	CAUD_MDL_MAIN			,	/*	3:MAIN MODULE		 */
	CAUD_MDL_POWER			,	/*	4:POWER MODULE		 */
	CAUD_MDL_TUNER			,	/*	5:TUNER MODULE		 */
	CAUD_MDL_BAND				/*	6:Band�`�F���W		 */
};

/* MUTE MID�萔��` */
enum AUD_MUTE_MID_DEF {
	/* �ʏ�MUTE */
	CAUD_MUTE_AUD_NR_MUTE	  = ( 0x00 | CAUD_MDL_AUDIO		),
	CAUD_MUTE_AUX_NR_MUTE	  = ( 0x00 | CAUD_MDL_AUX		),
	CAUD_MUTE_MAIN_NR_MUTE	  = ( 0x00 | CAUD_MDL_MAIN		),
	CAUD_MUTE_TUN_NR_MUTE	  = ( 0x00 | CAUD_MDL_TUNER		),
	CAUD_MUTE_BANDCHG_NR_MUTE = ( 0x00 | CAUD_MDL_BAND		),
	
	/* �V�X�e��MUTE */
	CAUD_MUTE_MAIN_DT_MUTE	  = ( 0x20 | CAUD_MDL_MAIN		),
	CAUD_MUTE_PWR_DT_MUTE	  = ( 0x20 | CAUD_MDL_POWER		),
	CAUD_MUTE_AUD_DT_MUTE	  = ( 0x20 | CAUD_MDL_AUDIO		),
	
	/* POWER MUTE */
	CAUD_MUTE_PWR_DA_MUTE	  = ( 0x40 | CAUD_MDL_POWER		)
};

/* Volume�萔��` */
enum AUD_AUDIO_VOL_DEF {
	CAUD_VOL_MAX =	31,
	CAUD_VOL_CNT =	13,
	CAUD_VOL_MIN =	 0,
	CAUD_VOL_INI =	13
};

/* Bass�萔��` */
enum AUD_AUDIO_BAS_DEF {
	CAUD_BAS_MAX =	10,
	CAUD_BAS_CNT =	5,
	CAUD_BAS_MIN =	0,
	CAUD_BAS_INI =	5
};

/* Treble�萔��` */
enum AUD_AUDIO_TRE_DEF {
	CAUD_TRE_MAX =	10,
	CAUD_TRE_CNT =	 5,
	CAUD_TRE_MIN =	 0,
	CAUD_TRE_INI =	 5
};

/* Balance�萔��` */
enum AUD_AUDIO_BAL_DEF {
	CAUD_BAL_MAX =	10,
	CAUD_BAL_CNT =	 5,
	CAUD_BAL_MIN =	 0,
	CAUD_BAL_INI =	 5
};

/* Fader�萔��` */
enum AUD_AUDIO_FAD_DEF {
	CAUD_FAD_MAX =	10,
	CAUD_FAD_CNT =	 5,
	CAUD_FAD_MIN =	 0,
	CAUD_FAD_INI =	 5
};

/* BEEP����v���^�C�v�萔��` */
enum AUD_REQ_BEEP_DEF {
	CAUD_REQ_BEEP_NOP		=0x00	,
	CAUD_REQ_BEEP_LONGKEY			,		/* �L�[������	*/
	CAUD_REQ_BEEP_ALARM				,		/* �A���[��		*/
	CAUD_REQ_BEEP_MULTIPLE			,		/* �A��			*/
	CAUD_REQ_BEEP_MAX
};

/* BEEP�����Ԓ萔��` */
enum AUD_BEEP_STATUS {
	CAUD_BEEP_STA_OFF		=0x00	,	/* OFF						*/
	
	/*---------Mute���䖳��-------*/
	CAUD_BEEP_STA_LONGKEY_M_NCONT	,	/* �L�[������				*/
	CAUD_BEEP_STA_ALARM_M_NCONT		,	/* �A���[��					*/
	CAUD_BEEP_STA_MULTIPLE_M_NCONT	,	/* �A��						*/
	
	/*---------Mute���䂠��-------*/
	CAUD_BEEP_STA_LONGKEY_M_CONT	,	/* �L�[������				*/
	CAUD_BEEP_STA_ALARM_M_CONT		,	/* �A���[��					*/
	CAUD_BEEP_STA_MULTIPLE_M_CONT		/* �A��						*/
};

/* MUTE��� */
enum AUD_MUTE_STATUS {
	CAUD_MUTE_ON_END	,			/* MUTE ON��������			*/
	CAUD_MUTE_OFF_END	,			/* MUTE OFF��������			*/
	CAUD_MUTE_ON_BUSY	,			/* MUTE ON������			*/
	CAUD_MUTE_OFF_BUSY				/* MUTE OFF������			*/
};

/* �����d�����[�h */
enum AUD_AUDIO_POWER_MODE {
 	CAUD_PWR_OFF	,				/* Power Off				*/
 	CAUD_PWR_ONSEQ	,				/* Power On Seq				*/
 	CAUD_PWR_ON		,				/* Power On					*/
 	CAUD_PWR_OFFSEQ					/* Power Off Seq			*/
};

/*---------------------------------------------------------------------------*
 *	�O�����W���[�������v���g�^�C�v�錾									 	 *
 *---------------------------------------------------------------------------*/
/* E-VOL����֐� */
EXT void Aud_set_vol_step( BYTE );
EXT void Aud_set_bas_step( BYTE );
EXT void Aud_set_tre_step( BYTE );
EXT void Aud_set_bal_step( BYTE );
EXT void Aud_set_fad_step( BYTE );

EXT BYTE Aud_get_vol_step( void );
EXT BYTE Aud_get_bas_step( void );
EXT BYTE Aud_get_tre_step( void );
EXT BYTE Aud_get_bal_step( void );
EXT BYTE Aud_get_fad_step( void );

/* �~���[�g����֐� */
EXT void Aud_set_prg_mute( BYTE, BYTE );

/* BEEP����֐� */
EXT void Aud_set_beep( BYTE );
EXT void Aud_cancel_beep( void );
EXT BYTE Aud_get_beep_type( void );

EXT void Aud_event_1ms( void );
EXT void Aud_event_5ms( void );
EXT void Aud_event_10ms( void );

BYTE Aud_incr( BYTE, BYTE );
BYTE Aud_decr( BYTE, BYTE );
BYTE Aud_add( BYTE, BYTE, BYTE );
BYTE Aud_sub( BYTE, BYTE );

EXT BYTE Aud_get_mute_status( void );
EXT BYTE Aud_get_power_status( void );

#if _TEST_MODE_
EXT void Aud_test_func( BYTE, BYTE *, BYTE );
EXT void Aud_secret_loud_key( void );
#endif
#endif
