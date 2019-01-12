/******************************************************************************
   クラリオン殿向け
	プロジェクト名	: 	2012年AM/FMラジオ対応モデル開発
	ファイル名		: 	aud_dev_func.h
	モジュール名	: 	Audioモジュール
	機能概略		: 	E-VOLデバイスドライバ処理のヘッダーファイル
-------------------------------------------------------------------------------
修正履歴
	番号		日付		担当者		関数名			修正内容
	【000】		12/07/13	leejh						新規作成
  	【000】		12/11/20	leejh		aud_bas_tre_gain_tbl(変更)
											.....VOL STEPによる、トーンテーブル
********************************************************************************/

#ifndef		_AUD_DEV_FUNC_H_
#define		_AUD_DEV_FUNC_H_

#include	"../model.h"


#undef		EXT
#ifdef		_AUD_DEV_FUNC_C_
#define		EXT
#else
#define		EXT extern
#endif

/*---------------------------------------------------------------------------*
 *	BAL/FAD構造体定義														 *
 *---------------------------------------------------------------------------*/
typedef struct {
	BYTE	left;		/* LEFT /REAR	 */
	BYTE	right;		/* RIGHT/FRONT	 */
} TAUD_ADEV_BALFAD_STEP;

#ifdef _AUD_DEV_FUNC_C_
#if 1	/* _EVOL_BD37503FV_ */
	/*----------------------------------------------------------------------*
	 *	ROHM(BD37503FV)														*
	 *----------------------------------------------------------------------*/
	#define	CAUD_SET_AUDIO_DATA_LENGHT		15		/* 転送データ最大長		*/
	#define	CAUD_DEVICE_ADRS				0x80	/* Slave address		*/
	
	/* Select address */
	#define	CAUD_SADR_INITIAL_SETUP1		0x01	/* INITIAL_SETUP1		*/
	#define	CAUD_SADR_INITIAL_SETUP2		0x03	/* INITIAL_SETUP2		*/
	#define	CAUD_SADR_INPUT_SELECTOR		0x05	/* INPUT_SELECTOR		*/
	#define	CAUD_SADR_INPUT_GAIN			0x06	/* INPUT_GAIN			*/
	#define	CAUD_SADR_VOLUME_GAIN			0x20	/* VOLUME_GAIN			*/
	#define	CAUD_SADR_FADER_1CH_FRONT		0x28	/* FADER_1CH_FRONT		*/
	#define	CAUD_SADR_FADER_2CH_FRONT		0x29	/* FADER_2CH_FRONT		*/
	#define	CAUD_SADR_FADER_1CH_REAR		0x2A	/* FADER_1CH_REAR		*/
	#define	CAUD_SADR_FADER_2CH_REAR		0x2B	/* FADER_2CH_REAR		*/
	#define	CAUD_SADR_BASS_GAIN				0x51	/* BASS_GAIN			*/
	#define	CAUD_SADR_TREBLE_GAIN			0x57	/* TREBLE_GAIN			*/
	#define	CAUD_SADR_LOUDNESS_GAIN			0x75	/* LOUDNESS GAIN		*/
	#define	CAUD_SADR_SYSTEM_RESET			0xFE	/* SYSTEM_RESET			*/
	
	/* -------------BD37503FVデータテーブル---------------- */
	/* BD37503FV初期設定送信データテーブル */
	const BYTE aud_EVOL_init_data_tbl[] =
	{
		0xA0,		/*	0:INITIAL SETUP1					*/
		0x00,		/*	1:INITIAL SETUP2					*/
		0x00,		/*	2:INPUT SELECTOR					*/
		#if 1	/* 社内不具合No.125 */
		0x00,		/*	3:INPUT GAIN						*/
		#else
		0x0B,		/*	3:INPUT GAIN						*/
		#endif
		0x81,		/*	4:VOLUME GAIN						*/
		0x9E,		/*	5:FADER 1CH FRONT					*/
		0x9E,		/*	6:FADER 2CH FRONT					*/
		0xFF,		/*  7:FADER 1CH REAR					*/
		0xFF,		/*	8:FADER 2CH REAR					*/
		0x00,		/*	9:BASS GAIN							*/
		0x00,		/*	10:TREBLE GAIN						*/
		0x00		/*	11:LOUDNESS GAIN					*/
	};
		
	/* -----------Input Selector (SA 05[D3～D0])----------- */
	const BYTE aud_input_sel_tbl[] =
	{
		0x00,			/* CAUD_MODE_TUNER_AM				*/
		0x01,			/* CAUD_MODE_TUNER_FM				*/
		0x04			/* CAUD_MODE_AUX1					*/
	};
		
	/* -------------Input Gain (SA 06[D4～D0])------------- */
	const BYTE aud_input_gain_tbl[] =
	{
		#if 1	/* V03対応 */
		0x00,			/* CAUD_MODE_TUNER_AM				*/
		0x00,			/* CAUD_MODE_TUNER_FM				*/
		0x01			/* CAUD_MODE_AUX1					*/
		#else
		0x0B,			/* CAUD_MODE_TUNER_AM				*/
		0x0B,			/* CAUD_MODE_TUNER_FM				*/
		0x0A			/* CAUD_MODE_AUX1					*/
		#endif
	};
		
	/* ----------Volume Fader Gain(SA 20[D7～D0])---------- */
	const BYTE aud_vol_gain_tbl[] =
	{
		0xFF,0xA4,0x9C,0x94,0x8E,0x88,0x84,0x81,0x81,0x81,	/*	VOL  0～VOL  9 */
		0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,	/*	VOL 10～VOL 19 */
		0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,	/*	VOL 20～VOL 29 */
		0x80,0x80											/*	VOL 30～VOL 31 */
	};
	/* -------------Select address (SA 51,57)-------------- */		
	/* -------------Bass/Treble Gain[D4～D0]--------------- */
	const BYTE aud_bas_tre_gain_tbl[6][11] =
	{
		/*  -5  -4,  -3   -2   -1    0   +1   +2   +3   +4   +5 */
		{ 0x8A,0x88,0x86,0x84,0x82,0x00,0x02,0x04,0x06,0x08,0x0A },	/*	VOL  0～VOL  18 */
		{ 0x8A,0x88,0x86,0x84,0x82,0x00,0x02,0x04,0x06,0x08,0x08 },	/*	VOL 19～VOL  20 */
		{ 0x8A,0x88,0x86,0x84,0x82,0x00,0x02,0x04,0x06,0x06,0x06 },	/*	VOL 21～VOL  22 */
		{ 0x8A,0x88,0x86,0x84,0x82,0x00,0x02,0x04,0x04,0x04,0x04 },	/*	VOL 23～VOL  24 */
		{ 0x8A,0x88,0x86,0x84,0x82,0x00,0x02,0x02,0x02,0x02,0x02 },	/*	VOL 25～VOL  27 */
		{ 0x8A,0x88,0x86,0x84,0x82,0x00,0x00,0x00,0x00,0x00,0x00 }	/*	VOL 28～VOL  31 */
	};
	
	const TAUD_ADEV_BALFAD_STEP aud_balfad_step_tbl[] = 
	{
	/*	  RIGHT, LEFT 		 		 */
	/*	  FRONT, REAR		  [STEP] */
		{ 0x05 , 0x00 },	/*	0	 */
		{ 0x04 , 0x00 },	/*	1	 */
		{ 0x03 , 0x00 },	/*	2	 */
		{ 0x02 , 0x00 },	/*	3	 */
		{ 0x01 , 0x00 },	/*	4	 */
		{ 0x00 , 0x00 },	/*	5	 */
		{ 0x00 , 0x01 },	/*	6	 */
		{ 0x00 , 0x02 },	/*	7	 */
		{ 0x00 , 0x03 },	/*	8	 */
		{ 0x00 , 0x04 },	/*	9	 */
		{ 0x00 , 0x05 }		/*	10	 */
	};

	const BYTE aud_fad_att_tbl[] = 
	{
		0xFF,0xAB,0xAB,0xAB,0xAB,0xAB,0xAB,0xAA,0xA8,0xA6,	/* VOL  0～VOL  9 */
		0xA4,0xA2,0xA0,0x9E,0x9C,0x9A,0x98,0x96,0x94,0x92,	/* VOL 10～VOL 19 */
		0x90,0x8E,0x8C,0x8A,0x88,0x86,0x84,0x83,0x82,0x81,	/* VOL 20～VOL 29 */
		0x81,0x80											/* VOL 30～VOL 31 */
	};
	
	/* -Volume/Fader Volume Attenuation (SA 28～2B[D7～D0])- */
	const BYTE aud_balfad_tbl[] = 
	{
				/* [dB]	[offset]	 */
		0x80,	/*  0	0			 */
		0x84,	/* -4	1			 */
		0x88,	/* -8	2			 */
		0x8C,	/* -12	3			 */
		0x94,	/* -20	4			 */
		0xFF,	/* -∞	5			 */
		0xFF,	/* -∞	6			 */
		0xFF,	/* -∞	7			 */
		0xFF,	/* -∞	8			 */
		0xFF,	/* -∞	9			 */
		0xFF	/* -∞	10			 */
	};
	
	/* -------------Loudness Gain (SA 75[D4～D0])------------------- */
	const BYTE aud_loudness_gain_tbl[] = 
	{
		0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,			/*	VOL  0～VOL  9 */
		0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,			/*	VOL 10～VOL 19 */
		0x0A,0x0A,0x08,0x08,0x06,0x06,0x04,0x04,0x02,0x02,			/*	VOL 20～VOL 29 */
		0x00,0x00													/*	VOL 30～VOL 31 */
	};
#endif	/* _EVOL_BD37503FV_ */
#endif	/* _AUD_DEV_FUNC_C_ */
/*---------------------------------------------------------------------------*
 *	E-VOLデバイスドライバブロックプロトタイプ定義						 	 *
 *---------------------------------------------------------------------------*/
void Aud_ctrl_ASP( BYTE );
void Aud_init_dev_func( void );
void Aud_send_initial( void );
#endif
