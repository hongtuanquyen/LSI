/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	: 
		�t�@�C����		: ma_tim.h
		�@�\			: Ҳ�Ӽޭ���ް���϶��ď���ͯ��̧��
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051128]	�V�K�쐬
******************************************************************************/
#ifndef		_MA_TIM_H_
#define		_MA_TIM_H_

#undef	EXT
#ifdef	_MA_TIM_C_
#define	EXT	    
#else
#define	EXT		extern
#endif

/*----------------------------------------------------
   �֐��v���g�^�C�v
----------------------------------------------------*/
EXT	void	Ma_tim_int_1ms(void);
EXT	void	Ma_tim_main(void);

/*----------------------------------------------------
   �׸ޒ�`
----------------------------------------------------*/
EXT	TCOM_FLG	ma_tim_flag;
#define	fma_tim_25ms	(ma_tim_flag.bit.bit7)		/* 25ms����׸� 1:25ms�o�߁y���:1ms�����݁A�رҲ�:�����z */
#define	fma_tim_2ms		(ma_tim_flag.bit.bit6)		/* 2ms����׸�  1:2ms�o�� */
#define	fma_tim_in1ms	(ma_tim_flag.bit.bit5)		/* 1ms�����ݏ������׸� 1: 1ms�����ݏ����� */
#define	fma_tim_10ms	(ma_tim_flag.bit.bit4)		/* 10ms����׸� 1:10ms�o�߁y��āF1ms�����݁A�ر�FҲݏ����z */
#define	fma_1ms_enb		(ma_tim_flag.bit.bit3)		/* 1ms�֎~�����׸�  1: 1ms����  0�F1ms�֎~ */
/*----------------------------------------------------
   ���������ϐ���`
----------------------------------------------------*/
EXT		BYTE	ma_tim_4ms_cnt;					/* 4ms��϶����y�����݁z */
EXT		BYTE	ma_tim_5ms_cnt;					/* 5ms��϶����y�����݁z */
EXT	 	BYTE	ma_tim_10ms_cnt;				/* 10ms��϶����y�����݁z */
EXT	 	BYTE	ma_tim_25ms_cnt;				/* 25ms��϶����y�����݁z */
EXT	 	BYTE	ma_tim_50ms_cnt;				/* 50ms��϶����yҲ݁z*/
EXT	 	BYTE	ma_tim_100ms_cnt;				/* 100ms��϶����yҲ݁z */
EXT		BYTE	ma_tim_500ms_cnt;				/* 500ms��϶����yҲ݁z */
EXT		BYTE	ma_tim_1s_cnt;					/* 1s��϶����yҲ݁z */
#define		CTIM_4MS_CNT	4
#define		CTIM_5MS_CNT	5
#define		CTIM_10MS_CNT	10
#define		CTIM_25MS_CNT	25
#define		CTIM_50MS_CNT	2
#define		CTIM_100MS_CNT	4
#define		CTIM_500MS_CNT	20
#define		CTIM_1S_CNT		40


#endif
