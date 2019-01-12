/******************************************************************************
   �N�����I���a����
	�v���W�F�N�g��	: 	2012�NAM/FM���W�I�Ή����f���J��
	�t�@�C����		: 	aud_dev_func.c
	���W���[����	: 	Audio���W���[��
	�@�\�T��		: 	E-VOL�f�o�C�X�h���C�o����
-------------------------------------------------------------------------------
�C������
	�ԍ�		���t		�S����		�֐���			�C�����e
	�y000�z		12/07/13	leejh						�V�K�쐬
	�y001�z		12/09/25	leejh		Aud_encode_volume_gain(�ύX)
										Aud_encode_fader(�ύX)
										Aud_encode_loudness(�ύX)
  	�y002�z		12/11/20	leejh		Aud_get_tone_index(�ǉ�)
										Aud_encode_bass_gain(�ύX)
										Aud_encode_treble_gain(�ύX)
										Aud_data_send_req(�ύX)
										Aud_set_volume(�ύX)
  	�y003�z		13/03/05	leejh		Aud_encode_loudness(�ύX)
  	�y004�z		13/03/21	leejh		�}�N����`(�ύX)
********************************************************************************/
#define		_AUD_DEV_FUNC_C_

#include	<string.h>
#include	"../model.h"

#include	"../comm/common.h"
#include	"../io/IO_inc.h"

#include	"aud_ext.h"
#include	"aud_sys.h"
#include	"aud_def.h"
#include	"aud_main.h"
#include	"aud_dev_func.h"
#include	"aud_mute_func.h"
#include	"aud_beep_func.h"

/*---------------------------------------------------------------------------*
 *	�\���̒�`															 	 *
 *---------------------------------------------------------------------------*/

/* fader��� */
typedef struct {
	BYTE			left_front;				/* left(1ch) front encode data	 [V13]2ch�ɕύX	*/
	BYTE			right_front;			/* right(2ch) front encode data	 [V13]1ch�ɕύX	*/
	BYTE			left_rear;				/* left(1ch) rear encode data	 [V13]2ch�ɕύX	*/
	BYTE			right_rear;				/* right(2ch) front encode data	 [V13]1ch�ɕύX	*/
} TAUD_FADER_INFO;

/* �f�[�^�G���R�[�h���	 */
typedef struct {
	BYTE			instruction;						/* ����					*/
	TAUD_FADER_INFO	fader;								/* fader�G���R�[�h���	*/
	BYTE			length;								/* �]���f�[�^��			*/
	BYTE			index;								/* �]���f�[�^�����݈ʒu	*/
	BYTE			buff[CAUD_SET_AUDIO_DATA_LENGHT];	/* �]���o�b�t�@			*/
} TAUD_ENCODE_INFO;

/*---------------------------------------------------------------------------*
 *	�����ϐ���`														 	 *
 *---------------------------------------------------------------------------*/
static TAUD_AUDIO_INFO		*aud_list_info;		/* AUDIO��񂪊i�[������o�b�t�@�ւ̃|�C���^	*/
static TAUD_MUTE_FLAG		*aud_list_mute;		/* MUTE��񂪊i�[������o�b�t�@�ւ̃|�C���^	*/
static TAUD_ENCODE_INFO		aud_encode_info;	/* ���M�f�[�^�G���R�[�h���						*/

static BYTE					aud_prev_bas;		/* �ȑO Bass Step	*/
static BYTE					aud_prev_tre;		/* �ȑO Treble Step	*/

/*---------------------------------------------------------------------------*
 *	�萔��`															 	 *
 *---------------------------------------------------------------------------*/
#define CAUD_BMT_CUT_FLG			0x80
#define CAUD_FADER_GAIN_MUTE		0xFF

/*---------------------------------------------------------------------------*
 *	�}�N����`															 	 *
 *---------------------------------------------------------------------------*/
#define MAUD_FADER_1CH_FRONT			aud_encode_info.fader.right_front
#define MAUD_FADER_2CH_FRONT			aud_encode_info.fader.left_front
#define MAUD_FADER_1CH_REAR				aud_encode_info.fader.right_rear
#define MAUD_FADER_2CH_REAR				aud_encode_info.fader.left_rear

#define	MAUD_SET_ENCODE_DATA(data)								\
		aud_encode_info.buff[aud_encode_info.index] = data;		\
		aud_encode_info.index++

/*---------------------------------------------------------------------------*
 *	�v���g�^�C�v��`													 	 *
 *---------------------------------------------------------------------------*/
static void Aud_send_data( void );
static BYTE Aud_get_volume_step( void );
static BYTE Aud_get_tone_index( BYTE );

static void Aud_encode_pack_header( BYTE );
static void Aud_encode_initial_setup1( void );
static void Aud_encode_initial_setup2( void );
static void Aud_encode_input_selector( void );
static void Aud_encode_input_gain( void );
static void Aud_encode_volume_gain( void );
static void Aud_encode_fader( BYTE );
static void Aud_encode_bass_gain( void );
static void Aud_encode_treble_gain( void );
static void Aud_encode_loudness( void );
static void Aud_encode_system_reset( void );

static void Aud_data_send_req( BYTE, BYTE, BYTE );
static void Aud_set_volume( void );
static void Aud_set_bass( void );
static void Aud_set_treble( void );
static void Aud_set_smute_on( void );
static void Aud_set_smute_off( void );
static void Aud_set_audio_data( void );
static void Aud_set_system_reset( void );
static void Aud_set_init_data( void );

/******************************************************************************
	�֐���		�FAud_send_data
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FI2C BUS �f�[�^�]��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
static void Aud_send_data( void )
{
	BYTE retry_cnt;
	BYTE state;

	/* �R���p�C��Warning���� */
	retry_cnt	= 0;
	state		= OK;

	while ( retry_cnt < 3 )
	{
		/* 1�o�C�g�f�[�^�o�� */
		state = IO_EVOL_Send( &aud_encode_info.buff[0], aud_encode_info.length );

		/* �ʐMOK�̏ꍇ�͋����I�� */
		if ( state == OK )
		{
			break;
		}
		retry_cnt++;	/* NG�̏ꍇ��3�񃊃g���C */
	}
}

/******************************************************************************
	�֐���		�FAud_get_volume_step
	����		�F����
	�߂�l		�FBYTE	vol_step	:volume step�l
	�@�\�T��	�F���ݗL����volume step�l���Z�o���Ēʒm����
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
static BYTE Aud_get_volume_step( void )
{
	BYTE vol_step;

	vol_step = 0;

	/* key mute, �O��mute���l������volume step���Z�o����	 */
	/* 	vol step - (key mute����step + �O��mute����step)	 */
	vol_step = Aud_sub( aud_list_info->vol_step,
					 ( aud_list_info->pre_kvol + aud_list_info->pre_evol ));
	
	return ( vol_step );
}

/******************************************************************************
	�֐���		�FAud_get_tone_index
	����		�FBYTE	vol_step	:VOL STEP�l
	�߂�l		�FBYTE	tone_index	:VOL STEP�ɂ��ATone�e�[�u���C���f�b�N�X
	�@�\�T��	�F�g�[���e�[�u���p�C���f�b�N�X���擾����
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/11/20	leejh		�V�K�쐬
******************************************************************************/
#define		CAUD_VOL_STEP_IDX_1_MIN			(19)
#define		CAUD_VOL_STEP_IDX_1_MAX			(20)

#define		CAUD_VOL_STEP_IDX_2_MIN			(21)
#define		CAUD_VOL_STEP_IDX_2_MAX			(22)

#define		CAUD_VOL_STEP_IDX_3_MIN			(23)
#define		CAUD_VOL_STEP_IDX_3_MAX			(24)

#define		CAUD_VOL_STEP_IDX_4_MIN			(25)
#define		CAUD_VOL_STEP_IDX_4_MAX			(27)

#define		CAUD_VOL_STEP_IDX_5_MIN			(28)
#define		CAUD_VOL_STEP_IDX_5_MAX			(31)

static BYTE Aud_get_tone_index(BYTE vol_step)
{
	/* ���[�J���ϐ��錾 */
	BYTE tone_index;
	
	/* ���[�J���ϐ������� */
	tone_index = 0;
	
	/* Index 1 */
	if (( vol_step >= CAUD_VOL_STEP_IDX_1_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_1_MAX ))
	{
		tone_index = 1;
	}
	/* Index 2 */
	else if (( vol_step >= CAUD_VOL_STEP_IDX_2_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_2_MAX ))
	{
		tone_index = 2;
	}
	/* Index 3 */
	else if (( vol_step >= CAUD_VOL_STEP_IDX_3_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_3_MAX ))
	{
		tone_index = 3;
	}
	/* Index 4 */
	else if (( vol_step >= CAUD_VOL_STEP_IDX_4_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_4_MAX ))
	{
		tone_index = 4;
	}
	/* Index 5 */
	else if (( vol_step >= CAUD_VOL_STEP_IDX_5_MIN ) &&
		( vol_step <= CAUD_VOL_STEP_IDX_5_MAX ))
	{
		tone_index = 5;
	}
	/* Index 0 */
	else
	{
		;
	}
	
	return ( tone_index );
}

#if 1 /*_EVOL_BD37503FV_*/
/******************************************************************************
	�֐���		�FAud_get_BD37503FV_slave_address
	����		�FBYTE sel_adrs�FSelect Address
	�߂�l		�F�Ȃ�
	�@�\�T��	�FSlave address��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
static void Aud_encode_pack_header( BYTE sel_adrs )
{
	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( CAUD_DEVICE_ADRS );
	MAUD_SET_ENCODE_DATA( sel_adrs );
}

/******************************************************************************
	�֐���		�FAud_encode_initial_setup1
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FInitial Setup1��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
#define		CAUD_INIT_SETUP1		0x00		/* Initial Set */

static void Aud_encode_initial_setup1( void )
{
	BYTE ebuf;

	/* Initial Setup1 */
	ebuf = CAUD_INIT_SETUP1;

	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	�֐���		�FAud_encode_initial_setup2
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FInitial Setup2��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
#define		CAUD_INIT_SETUP2		0x00		/* Initial Set */

static void Aud_encode_initial_setup2( void )
{
	BYTE ebuf;

	/* Initial Setup2 */
	ebuf = CAUD_INIT_SETUP2;

	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	�֐���		�FAud_encode_input_selector
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FInput Selector��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
 ******************************************************************************/
static void Aud_encode_input_selector( void )
{
	BYTE src;
	BYTE ebuf;
	
	/* current mode���擾���� */
	src = aud_list_info->curt_src;

	/* �擾�������[�h���e�[�u���O */
	if ( src >= CAUD_MODE_MAX )
	{
		src = CAUD_MODE_TUNER_AM;
	}

	ebuf = aud_input_sel_tbl[src];

	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	�֐���		�FAud_encode_input_gain
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FInput gain��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
static void Aud_encode_input_gain( void )
{
	BYTE src;
	BYTE ebuf;
	
	/* current mode���擾���� */
	src = aud_list_info->curt_src;

	/* �擾�������[�h���e�[�u���O */
	if ( src >= CAUD_MODE_MAX )
	{
		src = CAUD_MODE_TUNER_AM;
	}
	
	ebuf = aud_input_gain_tbl[src];
	
	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	�֐���		�FAud_encode_volume_gain
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�Fvolume gain��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
 ******************************************************************************/
#define CAUD_VOLUME_GAIN_MUTE 0xFF

static void Aud_encode_volume_gain( void )
{
	BYTE vol_step;
	BYTE ebuf;
	
	/* mute���l������volume step�l���擾���� */
	vol_step = Aud_get_volume_step();

	/* VOL STEP�̍ő�l�𒴉� */
	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_INI;
	}
	
	/* volume��ݒ� */
	ebuf = aud_vol_gain_tbl[vol_step];

	/* Power Off�A���[���͖������ɂ��� */
	if ( aud_list_mute->data_mute == ON )
	{
		ebuf = (BYTE) CAUD_VOLUME_GAIN_MUTE;
	}
	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	�֐���		�FAud_encode_fader
	����		�FBYTE get_data�F	CAUD_ENC_FADER_ALL
									CAUD_ENC_FADER_1CH_FRONT
									CAUD_ENC_FADER_2CH_FRONT
									CAUD_ENC_FADER_1CH_REAR
									CAUD_ENC_FADER_2CH_REAR
	�߂�l		�F�Ȃ�
	�@�\�T��	�FFader Xch Front/Rear��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
	�y001�z		13/03/22	leejh		�d�l�ύX(LRch���])
 ******************************************************************************/
enum
{
	CAUD_ENC_FADER_ALL		= 0x00	,	/* Fader CH	All		*/
	CAUD_ENC_FADER_1CH_FRONT		,	/* Fader 1CH Front	*/
	CAUD_ENC_FADER_2CH_FRONT		,	/* Fader 2CH Front	*/
	CAUD_ENC_FADER_1CH_REAR			,	/* Fader 1CH Rear	*/
	CAUD_ENC_FADER_2CH_REAR				/* Fader 2CH Rear	*/
};

enum
{
	LF = 0x00	,	/* LEFT FRONT	*/
	RF			,	/* RIGHT FRONT	*/
	LR			,	/* LEFT REAR	*/
	RR				/* RIGHT REAR	*/
};

static void Aud_encode_fader( BYTE enc_data )
{
	BYTE bal_step;
	BYTE fad_step;
	BYTE vol_step;
	BYTE fad_att;
	
	BYTE balfad_step[4];
	BYTE balfad_att[4];
	BYTE balfad_enc[4];
	
	/* �ϐ��̏������ifail safe�j */
	MAUD_FADER_1CH_FRONT	= \
	MAUD_FADER_2CH_FRONT	= \
	MAUD_FADER_1CH_REAR		= \
	MAUD_FADER_2CH_REAR		= CAUD_FADER_GAIN_MUTE;

	bal_step = aud_list_info->bal_step;
	fad_step = aud_list_info->fad_step;
	
	vol_step = Aud_get_volume_step();

	if ( bal_step > CAUD_BAL_MAX )
	{
		bal_step = CAUD_BAL_CNT;
	}

	if ( fad_step > CAUD_FAD_MAX )
	{
		fad_step = CAUD_FAD_CNT;
	}

	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_MIN;
	}
	
	/* VOL 0�AVOL MUTE�APower Off�A���[����fail safe��-���𑗐M */
	if (( vol_step > CAUD_VOL_MIN ) && ( aud_list_mute->data_mute == OFF ))
	{
		/* BALANCE step���G���R�[�h�p��step�ɕϊ����� */
		balfad_step[LF] = aud_balfad_step_tbl[bal_step].right;	/* LEFT  FRONT	*/
		balfad_step[RF] = aud_balfad_step_tbl[bal_step].left;	/* RIGHT FRONT	*/
		balfad_step[LR] = aud_balfad_step_tbl[bal_step].right;	/* LEFT  REAR	*/
		balfad_step[RR] = aud_balfad_step_tbl[bal_step].left;	/* RIGHT REAR	*/

		/* FADER step���G���R�[�h�p��step�ɕϊ����� */
		balfad_step[LF] += aud_balfad_step_tbl[fad_step].left;	/* LEFT  FRONT	*/
		balfad_step[RF] += aud_balfad_step_tbl[fad_step].left;	/* RIGHT FRONT	*/
		balfad_step[LR] += aud_balfad_step_tbl[fad_step].right;	/* LEFT  REAR	*/
		balfad_step[RR] += aud_balfad_step_tbl[fad_step].right;	/* RIGHT REAR	*/

		/*�����ɂ��ABalFad ATT�擾 */
		balfad_att[LF] = aud_balfad_tbl[balfad_step[LF]] & 0x7F;
		balfad_att[RF] = aud_balfad_tbl[balfad_step[RF]] & 0x7F;
		balfad_att[LR] = aud_balfad_tbl[balfad_step[LR]] & 0x7F;
		balfad_att[RR] = aud_balfad_tbl[balfad_step[RR]] & 0x7F;
		
		/* Volume Step�ɂ��AFad ATT�擾 */
		fad_att = aud_fad_att_tbl[vol_step] & 0x7F;

		/* �G���R�[�h�l���i�[ */
		balfad_enc[LF] = balfad_att[LF] + fad_att;
		balfad_enc[RF] = balfad_att[RF] + fad_att;
		balfad_enc[LR] = balfad_att[LR] + fad_att;
		balfad_enc[RR] = balfad_att[RR] + fad_att;
		
		if ( balfad_enc[RF] <= 0x3F )
		{
			MAUD_FADER_1CH_FRONT = balfad_enc[RF] | 0x80;
		}
		
		if ( balfad_enc[LF] <= 0x3F )
		{
			MAUD_FADER_2CH_FRONT = balfad_enc[LF] | 0x80;
		}
		
		if ( balfad_enc[RR] <= 0x3F )
		{
#if 1		/* �{���f����Rear CH��-��dB */
			MAUD_FADER_1CH_REAR = CAUD_FADER_GAIN_MUTE;
#else
			MAUD_FADER_1CH_REAR  = balfad_enc[RR] | 0x80;
#endif
		}
		
		if ( balfad_enc[LR] <= 0x3F )
		{
#if 1		/* �{���f����Rear CH��-��dB */
			MAUD_FADER_2CH_REAR = CAUD_FADER_GAIN_MUTE;
#else
			MAUD_FADER_2CH_REAR  = balfad_enc[LR] | 0x80;
#endif
		}
	}
	
	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	switch ( enc_data )
	{
		case CAUD_ENC_FADER_1CH_FRONT:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_1CH_FRONT );
			break;

		case CAUD_ENC_FADER_2CH_FRONT:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_2CH_FRONT );
			break;

		case CAUD_ENC_FADER_1CH_REAR:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_1CH_REAR );
			break;

		case CAUD_ENC_FADER_2CH_REAR:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_2CH_REAR );
			break;

		case CAUD_ENC_FADER_ALL:
		default:
			MAUD_SET_ENCODE_DATA( MAUD_FADER_1CH_FRONT );
			MAUD_SET_ENCODE_DATA( MAUD_FADER_2CH_FRONT );
			MAUD_SET_ENCODE_DATA( MAUD_FADER_1CH_REAR );
			MAUD_SET_ENCODE_DATA( MAUD_FADER_2CH_REAR );
			break;
	}
}

/******************************************************************************
	�֐���		�FAud_encode_bass_gain
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FBass gain��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
   	�y001�z		12/11/20	leejh		VOL STEP�ɂ��A�g�[���G���R�[�h�����ǉ�
 ******************************************************************************/
static void Aud_encode_bass_gain( void )
{
	BYTE bas_step;
	BYTE ebuf;
	BYTE vol_step;
	BYTE tone_index;
		
	/* ���[�J���ϐ������� */
	tone_index = 0;
	
	vol_step = aud_list_info->vol_step;		/* vol step	 */
	bas_step = aud_list_info->bas_step;		/* bass step */

	/* VOL STEP�̍ő�l�𒴉� */
	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_INI;
	}
	
	/* �g�[���C���f�b�N�X���擾���� */
	tone_index = Aud_get_tone_index(vol_step);
	
	/* bass step�l���͈͂𒴂��Ă���ꍇ */
	if ( bas_step > CAUD_BAS_MAX )
	{
		bas_step = CAUD_BAS_INI;
	}

	/* �f�[�^�e�[�u������l�𒊏o���� */
	ebuf = aud_bas_tre_gain_tbl[tone_index][bas_step];
	
	/* cut�t���O�ݒ�										 */
	/* -2dB�ȉ�����0dB��ݒ肷��ꍇcut�t���O���Z�b�g����	 */
	if (( ebuf == 0x00 ) && (( aud_prev_bas & CAUD_BMT_CUT_FLG ) == 0x80 ))
	{
		ebuf |= (BYTE) CAUD_BMT_CUT_FLG;
	}

	aud_prev_bas = ebuf;

	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( ebuf );
}
/******************************************************************************
	�֐���		�FAud_encode_treble_gain
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�Ftreble gain��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
  	�y001�z		12/11/20	leejh		VOL STEP�ɂ��A�g�[���G���R�[�h�����ǉ�
 ******************************************************************************/
static void Aud_encode_treble_gain( void )
{
	BYTE tre_step;
	BYTE ebuf;
	BYTE vol_step;
	BYTE tone_index;
	
	/* ���[�J���ϐ������� */
	tone_index = 0;
	
	vol_step = aud_list_info->vol_step;		/* vol step	 */
	tre_step = aud_list_info->tre_step;		/* treble step */

	/* VOL STEP�̍ő�l�𒴉� */
	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_INI;
	}
	
	/* �g�[���C���f�b�N�X���擾���� */
	tone_index = Aud_get_tone_index(vol_step);
	
	/* treble step�l���͈͂𒴂��Ă���ꍇ */
	if ( tre_step > CAUD_TRE_MAX )
	{
		tre_step = CAUD_TRE_INI;
	}

	/* �f�[�^�e�[�u������l�𒊏o���� */
	ebuf = aud_bas_tre_gain_tbl[tone_index][tre_step];
	
	/* cut�t���O�ݒ�										 */
	/* -2dB�ȉ�����0dB��ݒ肷��ꍇcut�t���O���Z�b�g����	 */
	if (( ebuf == 0x00 ) && (( aud_prev_tre & CAUD_BMT_CUT_FLG ) == 0x80 ))
	{
		ebuf |= (BYTE) CAUD_BMT_CUT_FLG;
	}

	aud_prev_tre = ebuf;

	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	�֐���		�FAud_encode_loudness
	����		�F����
	�߂�l		�F����
	�@�\�T��	�Floudness gain��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
	�y001�z		12/09/25	leejh		LOUD�f�f(�e�X�g�A�B���L�[)�@�\�Ή�
	�y002�z		13/03/05	leejh		LOUD ON���[�h�ǉ�
 ******************************************************************************/
#define		CAUD_LOUDNESS_GAIN_0DB		0x00
	
static void Aud_encode_loudness( void )
{
	BYTE vol_step;
	BYTE ebuf;
	BYTE src;
	
#if _TEST_MODE_
	BYTE loud_onoff;
#endif
	loud_onoff = OFF;		/* Fail Safe */
	
	/* mute���l������volume step�l���擾���� */
	vol_step = Aud_get_volume_step();

	/* VOL STEP�̍ő�l�𒴉� */
	if ( vol_step > CAUD_VOL_MAX )
	{
		vol_step = CAUD_VOL_INI;
	}
	
	/* current mode���擾���� */
	src = aud_list_info->curt_src;
	
#if _TEST_MODE_
	/* LOUD�f�f(�e�X�g)���D�� */
	if ( Aud_diag_loud.prio_diag == CAUD_PRIO_TEST )
	{
		switch( Aud_diag_loud.test_loud )
		{
			/* �e�X�g���[�hLOUD ON */
			case CAUD_LOUD_STA_ON:
				loud_onoff = ON;
				break;

			/* �e�X�g���[�hLOUD OFF */
			case CAUD_LOUD_STA_OFF:
				break;

			/* ���̑� */
			default:
				break;
		}
	}
	/* LOUD�f�f(�B���L�[)���D�� */
	else if ( Aud_diag_loud.prio_diag == CAUD_PRIO_SECRET )
	{
		switch( Aud_diag_loud.secret_loud )
		{
			/* �B���L�[LOUD ON */
			case CAUD_LOUD_STA_ON:
				loud_onoff = ON;
				break;

			/* �B���L�[LOUD OFF */
			case CAUD_LOUD_STA_OFF:
				break;

			/* ���̑� */
			default:
				break;
		}
	}
	/* ���̑�(�ʏ�) */
	else
	{
		/* FM���[�h LOUD ON */
		if ( (src == CAUD_MODE_TUNER_FM) || (src == CAUD_MODE_AUX1) )
		{
			loud_onoff = ON;		
		}
	}
	
	if ( loud_onoff == ON )		/* LOUD ON�G���R�[�h�@*/
	{
		/* Loudness Gain�擾 */
		ebuf = aud_loudness_gain_tbl[vol_step];
	}
	else						/* LOUD OFF�G���R�[�h�@*/
	{
		/* ���̑� Loudness Gain 0dB */
		ebuf = (BYTE) CAUD_LOUDNESS_GAIN_0DB;
	}
#else
	if ( src == CAUD_MODE_TUNER_FM )
	{
		/* Loudness Gain�擾 */
		ebuf = aud_loudness_gain_tbl[vol_step];
	}
	else
	{
		/* ���̑� Loudness Gain 0dB */
		ebuf = (BYTE) CAUD_LOUDNESS_GAIN_0DB;
	}
#endif
	
	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( ebuf );
}

/******************************************************************************
	�֐���		�FAud_encode_system_reset
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FSystem Reset��ݒ肷��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
 ******************************************************************************/
/* -------------System Reset (SA FE[D7~D0])------------------- */
#define	CAUD_SYSTEM_RESET				0x81

static void Aud_encode_system_reset( void )
{
	/* �G���R�[�h�����f�[�^��]���o�b�t�@�ɏ����� */
	MAUD_SET_ENCODE_DATA( CAUD_SYSTEM_RESET );
}

/******************************************************************************	
	Item���M�F�eItem���Ƃɑ��M���s���B
	���쑗�M�F�L�[����ɂ��A�K�v��Item���M���s���B
	���̑��F���̑��A�{�V�X�e���ɂĕK�v��Item���M���s��
	���eItem(Blk)�̃T�C�Y���l�����A���M�v���ݒ���s���Ă��������B
	  �ڍׂ́uBD35703FV�f�[�^�V�[�g�v���Q�Ƃ��Ă��������B
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
enum
{
	CAUD_SEND_TYPE_NOP		= 0x00	,		/* NOP				*/
	/* Item���M */
	CAUD_SEND_TYPE_INITAL_SETUP1	,		/* Initial Setup1	*/
	CAUD_SEND_TYPE_INITAL_SETUP2	,		/* Initial Setup2	*/
	CAUD_SEND_TYPE_INPUT_SELECTOR	,		/* Input Selector	*/
	CAUD_SEND_TYPE_INPUT_GAIN		,		/* Input Gain		*/
	CAUD_SEND_TYPE_VOLUME_GAIN		,		/* Volume Gain		*/
	CAUD_SEND_TYPE_FADER_1CHF		,		/* Fader 1CH Front	*/
	CAUD_SEND_TYPE_FADER_2CHF		,		/* Fader 2CH Front	*/
	CAUD_SEND_TYPE_FADER_1CHR		,		/* Fader 1CH Rear	*/
	CAUD_SEND_TYPE_FADER_2CHR		,		/* Fader 2CH Rear	*/
	CAUD_SEND_TYPE_BASS_GAIN		,		/* Bass Gain		*/
	CAUD_SEND_TYPE_TREBLE_GAIN		,		/* Treble Gain		*/
	CAUD_SEND_TYPE_LOUDNESS_GAIN	,		/* Loudness Gain	*/
	CAUD_SEND_TYPE_SYSTEM_RESET		,		/* System Reset		*/
	/* ���쑗�M*/
	CAUD_SEND_TYPE_VOLUME_CONTROL	,		/* Volume Control	*/
	CAUD_SEND_TYPE_BASS_CONTROL		,		/* Bass	Control		*/
	CAUD_SEND_TYPE_TREBLE_CONTROL	,		/* Treble Control	*/
	CAUD_SEND_TYPE_BALANCE_CONTROL	,		/* Balance Control	*/
	CAUD_SEND_TYPE_FADER_CONTROL	,		/* Fader Control	*/
	/* ���̑� */
	CAUD_SEND_TYPE_SOFTMUTE_ON		,		/* Softmute On		*/
	CAUD_SEND_TYPE_SOFTMUTE_OFF		,		/* Softmute Off		*/
	CAUD_SEND_TYPE_ALL_AUDIO		,		/* All				*/
	CAUD_SEND_TYPE_INIT_AUDIO		,		/* Initial			*/
	CAUD_SEND_TYPE_MAX						/* MAX				*/
};

/******************************************************************************
	�֐���		�FAud_data_send_req
	����		�FBYTE	saddr	�F	Select Address
				  BYTE	type	�F	���M�^�C�v
				  BYTE	len		�F	���M����(MAX�FCAUD_SET_AUDIO_DATA_LENGHT)
	�߂�l		�F�Ȃ�
	�@�\�T��	�FAudio�f�[�^���M��v������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
	�y000�z		12/11/20	leejh		VOL/BAL/FAD�������ABAS/TRE�G���R�[�h
******************************************************************************/
static void Aud_data_send_req( BYTE saddr, BYTE type, BYTE len )
{
	BYTE send_req;
	
	send_req = ON;		/* �f�t�H���g�͑��M ON		*/

	if ( len > 2 )		/* �Œ�3Blk����󂯕t����	*/
	{
		aud_encode_info.index	= 0;		/* Index�N���A(Fail Safe)	*/
		aud_encode_info.length	= len;		/* �]������ݒ肷��		*/

		Aud_encode_pack_header( saddr );	/* Slave, Select addr(2Blk)	*/

		switch ( type )		/* �v���ɂ��AItem�G���R�[�h */
		{
			/*-------------�eItem���M-------------*/
			case CAUD_SEND_TYPE_INITAL_SETUP1:	/* Initial Setup1(1Blk)	*/
				Aud_encode_initial_setup1();
				break;
				
			case CAUD_SEND_TYPE_INITAL_SETUP2:	/* Initial Setup2(1Blk)	*/
				Aud_encode_initial_setup2();
				break;
				
			case CAUD_SEND_TYPE_INPUT_SELECTOR:	/* Input Selector(1Blk)	*/
				Aud_encode_input_selector();
				break;
				
			case CAUD_SEND_TYPE_INPUT_GAIN:		/* Input Gain(1Blk)		*/
				Aud_encode_input_gain();
				break;
				
			case CAUD_SEND_TYPE_VOLUME_GAIN:	/* Volume Gain(1Blk)	*/
				Aud_encode_volume_gain();
				break;
				
			case CAUD_SEND_TYPE_FADER_1CHF:		/* Fader 1CH Front(1Blk)*/
				Aud_encode_fader( CAUD_ENC_FADER_1CH_FRONT );	
				break;

			case CAUD_SEND_TYPE_FADER_2CHF:		/* Fader 2CH Front(1Blk)*/
				Aud_encode_fader( CAUD_ENC_FADER_2CH_FRONT );
				break;

			case CAUD_SEND_TYPE_FADER_1CHR:		/* Fader 1CH Rear(1Blk)	*/
				Aud_encode_fader( CAUD_ENC_FADER_1CH_REAR );
				break;

			case CAUD_SEND_TYPE_FADER_2CHR:		/* Fader 2CH Rear(1Blk)	*/
				Aud_encode_fader( CAUD_ENC_FADER_2CH_REAR );
				break;
						
			case CAUD_SEND_TYPE_BASS_GAIN:		/* Bass Gain(1Blk)		*/
				Aud_encode_bass_gain();
				break;
				
			case CAUD_SEND_TYPE_TREBLE_GAIN:	/* Treble Gain(1Blk)	*/
				Aud_encode_treble_gain();		
				break;
				
			case CAUD_SEND_TYPE_LOUDNESS_GAIN:	/* Loudness Gain(1Blk)	*/
				Aud_encode_loudness();
				break;
			
			case CAUD_SEND_TYPE_SYSTEM_RESET:
				Aud_encode_system_reset();		/* System Reset(1Blk)	*/
				break;
				
			/*-------------���쑗�M-------------*/
			case CAUD_SEND_TYPE_VOLUME_CONTROL:	/* Volume Control		*/
			case CAUD_SEND_TYPE_BALANCE_CONTROL:/* Balance Control		*/
			case CAUD_SEND_TYPE_FADER_CONTROL:	/* Fader Control		*/
				Aud_encode_volume_gain();				/* 1Blk	*/
				Aud_encode_fader( CAUD_ENC_FADER_ALL );	/* 4Blk	*/
				Aud_encode_bass_gain();					/* 1Blk	*/
				Aud_encode_treble_gain();				/* 1Blk	*/
				Aud_encode_loudness();					/* 1Blk	*/
				break;

			case CAUD_SEND_TYPE_BASS_CONTROL:	/* Bass Control			*/
				Aud_encode_bass_gain();			/* 1Blk	*/
				break;

			case CAUD_SEND_TYPE_TREBLE_CONTROL:	/* Treble Control		*/
				Aud_encode_treble_gain();		/* 1Blk	*/
				break;

			/*-------------���̑�-------------*/
			/* ���E-VOL�V���[�Y�Ή�(Softmute) */
			case CAUD_SEND_TYPE_SOFTMUTE_ON:	/* Softmute On			*/
			case CAUD_SEND_TYPE_SOFTMUTE_OFF:	/* Softmute Off			*/
				Aud_encode_input_gain();		/* 1Blk	*/
				break;
				
			case CAUD_SEND_TYPE_ALL_AUDIO:				/* All Send		*/
				Aud_encode_input_selector();			/* 1Blk			*/
				Aud_encode_input_gain();				/* 1Blk			*/
				Aud_encode_volume_gain();				/* 1Blk			*/
				Aud_encode_fader( CAUD_ENC_FADER_ALL );	/* 4Blk			*/
				Aud_encode_bass_gain();					/* 1Blk			*/
				Aud_encode_treble_gain();				/* 1Blk			*/
				Aud_encode_loudness();					/* 1Blk			*/
				break;

			case CAUD_SEND_TYPE_INIT_AUDIO:		/* Initial Send(12Blk)	*/
				/* �����l�e�[�u���̒l�������Ă���						*/
				/* buff��Slave Adress�ASelect address���l�����ď������ނ��� */
				romcopy(( BYTE * ) & aud_encode_info.buff[2],
						( BYTE * ) & aud_EVOL_init_data_tbl[0], sizeof ( aud_EVOL_init_data_tbl ));
				break;
				
			default:	/* Exception */
				send_req = OFF;					/* ���M���Ȃ�		*/
				break;
		}
	}
	else
	{
		send_req = OFF;		/* ���M���Ȃ� */
	}
	
	if ( send_req == ON )	/* ���M�v������? */
	{
		Aud_send_data();	/* �G���R�[�h�����f�[�^��]������ */
	}
}

/******************************************************************************
	�֐���		�FAud_set_volume
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�Fvolume,balance,fader��������audio�f�[�^�]������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
	�y001�z		12/11/20	leejh		VOL/BAL/FAD�������ABAS/TRE�G���R�[�h
 *										Item(Loudness Gain)�ʑ��M�폜
******************************************************************************/
static void Aud_set_volume( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Vol~Loud(8Blk)= 10Blk */
	Aud_data_send_req( CAUD_SADR_VOLUME_GAIN, CAUD_SEND_TYPE_VOLUME_CONTROL, 10 );
}

/******************************************************************************
	�֐���		�FAud_set_bass
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�Fbass��������audio�f�[�^�]������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
static void Aud_set_bass( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Bass(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_BASS_GAIN,  CAUD_SEND_TYPE_BASS_CONTROL, 3 );
}

/******************************************************************************
	�֐���		�FAud_set_treble
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�Ftreble��������audio�f�[�^�]������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
static void Aud_set_treble( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Treb(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_TREBLE_GAIN,  CAUD_SEND_TYPE_TREBLE_CONTROL, 3 );
}

/******************************************************************************
	�֐���		�FAud_set_smute_on
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�Fsoft mute on��]������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬(��ʃV���[�Y�p)
******************************************************************************/
static void Aud_set_smute_on( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Input(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_INPUT_GAIN,  CAUD_SEND_TYPE_SOFTMUTE_ON, 3 );
}

/******************************************************************************
	�֐���		�FAud_set_smute_off
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�Fsoft mute off��]������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬(��ʃV���[�Y�p)
******************************************************************************/
static void Aud_set_smute_off( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+InputGain(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_INPUT_GAIN,  CAUD_SEND_TYPE_SOFTMUTE_OFF, 3 );
}

/******************************************************************************
	�֐���		�FAud_set_audio_data
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�F�I�[�f�B�I�f�[�^�]��
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
 ******************************************************************************/
static void Aud_set_audio_data( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+InputSel~Loud(10Blk)= 12Blk */
	Aud_data_send_req( CAUD_SADR_INPUT_SELECTOR,  CAUD_SEND_TYPE_ALL_AUDIO, 12 );
}

/******************************************************************************
	�֐���		�FAud_set_system_reset
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FBD37503FV system reset�f�[�^��]������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
 ******************************************************************************/
static void Aud_set_system_reset( void )
{
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+SysRst(1Blk)= 3Blk */
	Aud_data_send_req( CAUD_SADR_SYSTEM_RESET,  CAUD_SEND_TYPE_SYSTEM_RESET, 3 );
}

/******************************************************************************
	�֐���		�FAud_set_init_data
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FBD37503FV�����ݒ�f�[�^��]������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
 ******************************************************************************/
static void Aud_set_init_data( void )
{
	BYTE len;
	
	/* �e�[�u���̒����擾(Slave Address+Select Address(2Blk)�𑫂� */
	len =  sizeof( aud_EVOL_init_data_tbl ) + 2;
	
	/* SlaveAddr(1Blk)+SelAddr(1Blk)+Init1~Loud(12Blk)= 14Blk */
	Aud_data_send_req( CAUD_SADR_INITIAL_SETUP1,  CAUD_SEND_TYPE_INIT_AUDIO, len );
}
#endif /* _EVOL_BD37503FV_ */

/******************************************************************************
	�֐���		�FAud_ctrl_ASP
	����		�FBYTE cmd_adrs	:	�R�}���hID
	�߂�l		�F�Ȃ�
	�@�\�T��	�FASP(Audio Sound Processor)���䃁�C������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
typedef void (*CAUD_ASP_FUNC)( void );
const CAUD_ASP_FUNC aud_ASP_func_tbl[] = {
	(void *)0				,
	Aud_set_system_reset	,		/* CAUD_ASP_SYS_RESET			: ASP�V�X�e�����Z�b�g	 */
	Aud_set_init_data		,		/* CAUD_ASP_INIT_AUDIO			: ASP�f�[�^�����ݒ�		 */
	Aud_set_volume			,		/* CAUD_ASP_VOL					: volume����			 */
	Aud_set_bass			,		/* CAUD_ASP_BAS					: bass����				 */
	Aud_set_treble			,		/* CAUD_ASP_TRE					: treble����			 */
	Aud_set_volume			,		/* CAUD_ASP_BAL					: balance����			 */
	Aud_set_volume			,		/* CAUD_ASP_FAD					: fader����				 */
	Aud_set_smute_on		,		/* CAUD_ASP_SMUTE_ON			: soft mute on			 */
	Aud_set_smute_off		,		/* CAUD_ASP_SMUTE_OFF			: soft mute off			 */
	Aud_set_audio_data		,		/* CAUD_ASP_SRC_CHG				: ���[�h�؊���			 */
	Aud_set_audio_data				/* CAUD_ASP_ALL_AUDIO			: �S�I�[�f�B�I�f�[�^�ݒ� */
};

void Aud_ctrl_ASP( BYTE instruction )
{
	if ( instruction < CAUD_ASP_INSTRUCTION_END )
	{
		/* ASP�ւ̖��߂�ۑ����� */
		aud_encode_info.instruction = instruction;

		if ( aud_ASP_func_tbl[instruction] )
		{
			/* �f�[�^�G���R�[�h�E�]���������J�n���� */
			(*( aud_ASP_func_tbl[instruction] ))();
		}
	}
}

/******************************************************************************
	�֐���		�FAud_init_dev_func
	����		�F�Ȃ�
	�߂�l		�F�Ȃ�
	�@�\�T��	�FE-VOL�f�o�C�X����u���b�N����������
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
******************************************************************************/
void Aud_init_dev_func( void )
{
	/* AUDIO��񂪊i�[����Ă���̈�̃A�h���X���擾���� */
	aud_list_info = Aud_get_audio_info_list();
	aud_list_mute = Aud_get_mute_state_list();
	
	/* encode��񏉊��� */
	memset((BYTE *) &aud_encode_info, (BYTE) 0, (BYTE) sizeof( aud_encode_info ));
}

/******************************************************************************
	�֐���		�FAud_send_initial
	����		�F����
	�߂�l		�F����
	�@�\�T��	�FE-VOL�������f�[�^�𑗐M
------------------------------------------------------------------------------
	�C������
	 �ԍ�		���t		�S����		�C�����e
	�y000�z		12/07/13	leejh		�V�K�쐬
 ******************************************************************************/
void Aud_send_initial( void )
{
	/* BD37503FV system reset */
	Aud_set_system_reset();

	/* BD37503FV initial */
	Aud_set_init_data();
}

#undef _AUD_DEV_FUNC_C_

