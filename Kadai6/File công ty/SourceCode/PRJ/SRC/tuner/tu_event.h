/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: tu_event.h
		�@�\			: ?Tuner event�w�b�_̧��
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[sun051215]	�V�K�쐬
[matu120903] S-meter���� Local�l�ǉ� 
******************************************************************************/
#ifndef	_TU_EVENT_H_
#define	_TU_EVENT_H_

#undef	EXT
#ifdef	_TU_EVENT_C_
#define	EXT
#else
#define	EXT	extern
#endif

/*----------------------------------------------------
   �֐����ĺ��
----------------------------------------------------*/
EXT		void	Tu_e_func_on(void);
EXT		void	Tu_e_func_off(void);
EXT		void	Tu_e_md_off(void);
EXT		void	Tu_e_bndoff(void);
EXT		void	Tu_e_updw_on(BYTE );
EXT		void	Tu_e_updw_long(BYTE );
EXT		void	Tu_e_updw_off(BYTE );
EXT		void	Tu_e_presetlong(BYTE );
EXT		void	Tu_e_presetoff(BYTE );
EXT		void	Tu_isron(void);
EXT		void	Tu_isroff(void);
EXT		void	Tu_e_as_on(void);
EXT		void	Tu_e_as_long(void);
EXT		void	Tu_e_pchupdw_on(BYTE );
EXT		void	Tu_e_pchupdw_long(void);
EXT		void	Tu_e_pchupdw_off(BYTE );
EXT		void	Tu_e_stteston(void);
EXT		void	Tu_e_sttestlong(void);
EXT		void	Tu_e_scanupdw_on(BYTE );
EXT		void	Tu_e_seek_updw(BYTE ud);
EXT		BYTE	Tu_pch_updw(BYTE ,BYTE );
EXT		BYTE	Tu_pch_up(BYTE );
EXT		BYTE	Tu_pch_down(BYTE );
EXT		void	Tu_updw_set(BYTE );
EXT		void	Tu_job_clear(void);				/* Ҳ�Ӱ�ޓ���֌W�ر���� */
EXT		void	Tu_as_initial(void);			/* AS���������� */
EXT		BYTE	Tu_fmbandmax(void);
EXT		void	Tu_BndChange(void);				/* BandChange */
EXT		void	Tu_presetcall(void);
EXT		BYTE	Tu_dxlo_chk(BYTE band);	/* [matu120903] */
EXT		void	Tu_intset_pchtbl(BYTE destno);	/* [matu120926] */
EXT		void	Tu_isrcall(void);
EXT		void	Tu_lastcall(void);
EXT		void	Tu_lastwrite(void);				/* ���Ď��g����׽Ă֏������� */
EXT		void	Tu_set_prechno(void);			/* PCH�����E�ݒ菈�� */
EXT		BYTE	Tu_chk_presetfreq(void);		/* PCH�������� */
EXT		void	Tu_presetcall_nolast(void);		/* PCH�ďo�E���X�g���ݒ菈�� */
EXT		void	Tu_pno_set(BYTE);				/* ����PresetNo��� */
EXT		void	Tu_aftermute(BYTE);				/* AfterMute ��ď��� */
EXT		void	Tu_ch_updw(void);
EXT		void 	Tu_ch_lm_set(WORD );
EXT		BYTE	Tu_dest_chk(void);
EXT		void 	Tu_coef_read(void);
EXT		void	Tu_ch_up(void);
EXT		void	Tu_ch_down(void);
EXT		void	Tu_dispfreqset(void);
EXT		void	Tu_steroclr(void);
EXT		void	Tu_sterochat(void);
EXT		void	Tu_stdis_timset(BYTE );
EXT		void	Tu_band_freq_set(void);
EXT		void	Tu_band_para_set(void);
EXT		void	Tu_para_send_set(void);
EXT		void	Tu_frq_send_set(void);
EXT		void	Tu_end_send_set(void);
EXT		void	Tu_beep_mute_set(BYTE on_off);
#endif
