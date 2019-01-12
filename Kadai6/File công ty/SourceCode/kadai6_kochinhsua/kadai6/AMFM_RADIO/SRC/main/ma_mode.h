/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: ma_mode.h
		�@�\			: �@Ҳ�Ӽޭ��Ӱ�ފĎ�����ͯ��̧��
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051128]	�V�K�쐬
******************************************************************************/
#ifndef		_MA_MODE_H_
#define		_MA_MODE_H_

#include	"../model.h"
#include	"../main/ma_ext.h"

#undef	EXT
#ifdef	_MA_MODE_C_
#define	EXT
#else
#define	EXT	extern
#endif


/*----------------------------------------------------
   Ӱ�ސ؂�ւ�������`ð���
----------------------------------------------------*/
typedef	 struct{
	BYTE	mode;				/* Ҳ�Ӱ�� */
	BYTE	prio;				/* �D�揇�� 0x00���D�斳���A 0x01���D������1�A���������قǗD��x���� */
	BYTE	lastsave:1;			/* ׽�Ӱ�ނɋL�����邩 YES��׽ċL������  NO��׽ċL�����Ȃ� */
	BYTE	onint:1;			/* ������ON��Ӱ�ސؑ�Ӱ�ނ�  YES�ˊ�����ON�Őؑ�  NO�ˊ����݂Őؑւ��Ȃ� */
	BYTE	offint:1;			/* ������OFF��Ӱ�ސؑ�Ӱ�ނ�  YES�ˊ�����OFF�Őؑ�  NO�ˊ����݂Őؑւ��Ȃ� */
	BYTE	pwr_on:1;			/* Ӱ��ON��Power ON�����邩  YES�˂����� NO�˂����Ȃ� */
	BYTE	pwr_off:1;			/* Ӱ��OFF��Power OFF�����邩  YES�˂�����  No�˂����Ȃ� */
	BYTE	off_keep:1;			/* Power Off ��Ӱ�ތp������邩  Yes��PowerOff�ł�MainӰ�ޕێ�  No��PowerOff��Ӱ�މ��� */
}TMA_MODE_COND;

/*----------------------------------------------------
   Ҳ�Ӱ�ޒ�`
----------------------------------------------------*/
typedef	struct{
	BYTE	id;
	TMA_MODE_COND	cond;
}TMA_MODE;

/*----------------------------------------------------
   �֐��v���g�^�C�v
----------------------------------------------------*/
EXT	void	Ma_mode_main(void);
EXT	void	Ma_source_chg(void);
EXT void 	Ma_mode_on_event(BYTE , BYTE );
EXT void 	Ma_mode_off_event(BYTE , BYTE  );
EXT	void 	Ma_mode_event_chk(BYTE, BYTE, BYTE );
EXT	void	Ma_modetbl_srh(TMA_MODE * );
EXT	void	Ma_mode_pwroff(void);

/*----------------------------------------------------
   	�萔��`
----------------------------------------------------*/
#define		CMA_PRIO_LV0		0
#define		CMA_PRIO_LV1		1
#define		CMA_PRIO_LV2		2
#define		CMA_PRIO_LV3		3

#define		CMA_FRONTMUTE		120/10+1
#define		CMA_FRONTMUTE_50	50/10
//#define		CMA_AFTERMUTE		700/10+1
#define		CMA_AFTERMUTE		250/10+1

#define		CMA_PRIO_CHK		1
#define		CMA_PRIO_NO			0

/*----------------------------------------------------
   �׸ޒ�`
----------------------------------------------------*/
EXT	TCOM_FLG	ma_flag;
#define	fma_BlueTooth	(ma_flag.bit.bit3)			/* BlueTooth�L�薳�� 1: ����  0: ���� */
#define	fma_mdchg	(ma_flag.bit.bit2)			/* Mode�ؑ֗v�� 1:�v������  0:�v������ */
#define	fma_mute	(ma_flag.bit.bit1)			/* MainMute�v��  1: Mute�v������  0:Mute�v������ */

/*----------------------------------------------------
   ���������ϐ���`
----------------------------------------------------*/
EXT		BYTE	ma_model;						/* �d�����y�l��`ma_ext.h�z */
EXT 	BYTE	ma_dest;						/* �@�햼�y�l��`ma_ext.h�z */
EXT		BYTE	ma_mutetimer;					/* MasterMute��� [10ms�ް�] */

#define		CMA_MAX		8
EXT		TMA_MODE	ma_mode[CMA_MAX];			/* MainӰ�ޗ����Ǘ��ޯ̧ */
#define		CMA_TARGET_MD		0
#define		CMA_CUR_MD			1
#define		CMA_LAST_MD			2
#define		CMA_MODE_ID			0
#define		CMA_MODE			1

#define		CMA_MD_MAX		CMA_ID_MAX + 1
#define		CMA_SRC_MAX		CMA_ID_SRCMAX + 1
EXT		BYTE	ma_mode_cont[CMA_MD_MAX];		/* Main����ؑ֐����ޯ̧ */
EXT		BYTE	ma_mode_ptr;					/* Main����ؑ֐����ޯ̧�߲�� */

EXT	BYTE	ma_test_mode;				/* ý�Ӱ�� */
EXT	BYTE	ma_last_mode;				/* ׽�Ӱ�� */

#endif
