/******************************************************************************
   �N�����I���a����
	�v���W�F�N�g��	: 	2012�NAM/FM���W�I�Ή����f���J��
	�t�@�C����		: 	aud_main.h
	���W���[����	: 	Audio���W���[��
	�@�\�T��		: 	���C�������̃w�b�_�[�t�@�C��
-------------------------------------------------------------------------------
�C������
	�ԍ�		���t		�S����		�֐���			�C�����e
	�y000�z		12/07/13	leejh						�V�K�쐬
********************************************************************************/
#ifndef		_AUD_MAIN_H_
#define		_AUD_MAIN_H_

#include	"../model.h"

#undef		EXT
#ifdef		_AUD_MAIN_C_
#define		EXT
#else
#define		EXT extern
#endif

/*---------------------------------------------------------------------------*
 *	�\���̒�`															 	 *
 *---------------------------------------------------------------------------*/
typedef struct {
	BYTE	zero_mute	:1;	/* volume zero mute		0: mute off	1: mute on	 */
	BYTE	reserved	:7;	/* reserved										 */
} TAUD_EVOL_MUTE;

EXT TAUD_EVOL_MUTE		aud_mute_evol;		/* EVOL����mute���				 */

/*---------------------------------------------------------------------------*
 *	E-VOL����u���b�N�v���g�^�C�v��`									 	 *
 *---------------------------------------------------------------------------*/
void Aud_msg_mode_change( void );
void Aud_msg_maincnt_on( void );
void Aud_msg_maincnt_off_req( void );
void Aud_msg_maincnt_off( void );
void Aud_msg_power_on( void );
void Aud_msg_power_off( void );

void Aud_edit_volume( BYTE );
void Aud_edit_bass( BYTE );
void Aud_edit_treble( BYTE );
void Aud_edit_balance( BYTE );
void Aud_edit_fader( BYTE );

TAUD_AUDIO_INFO *Aud_get_audio_info_list( void );
BYTE Aud_get_audio_info( BYTE );
BYTE Aud_get_power_mode( void );
void Aud_evol_main( void );

void Aud_set_timer( BYTE , BYTE , BYTE );
BYTE Aud_get_timer( BYTE , BYTE );
void Aud_dec_timer( TAUD_TIMER_INFO * );

void Aud_msg_timer( BYTE );
void Aud_msg_initial( void );
void Aud_msg_main( void );

#if _TEST_MODE_
void Aud_secret_loud_off( void );
void Aud_test_edit_data( BYTE, BYTE *, BYTE );
#endif
#endif
