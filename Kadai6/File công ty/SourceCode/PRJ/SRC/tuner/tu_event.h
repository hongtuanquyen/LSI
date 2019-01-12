/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

		プロジェクト名	:
		ファイル名		: tu_event.h
		機能			: ?Tuner eventヘッダﾌｧｲﾙ
------------------------------------------------------------------------------
		修正履歴	【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051215]	新規作成
[matu120903] S-meter判定 Local値追加 
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
   関数ﾌﾟﾛﾄｺｰﾙ
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
EXT		void	Tu_job_clear(void);				/* ﾒｲﾝﾓｰﾄﾞ動作関係ｸﾘｱ処理 */
EXT		void	Tu_as_initial(void);			/* AS初期化処理 */
EXT		BYTE	Tu_fmbandmax(void);
EXT		void	Tu_BndChange(void);				/* BandChange */
EXT		void	Tu_presetcall(void);
EXT		BYTE	Tu_dxlo_chk(BYTE band);	/* [matu120903] */
EXT		void	Tu_intset_pchtbl(BYTE destno);	/* [matu120926] */
EXT		void	Tu_isrcall(void);
EXT		void	Tu_lastcall(void);
EXT		void	Tu_lastwrite(void);				/* ｶﾚﾝﾄ周波数をﾗｽﾄへ書き込み */
EXT		void	Tu_set_prechno(void);			/* PCH検索・設定処理 */
EXT		BYTE	Tu_chk_presetfreq(void);		/* PCH検索処理 */
EXT		void	Tu_presetcall_nolast(void);		/* PCH呼出・ラスト未設定処理 */
EXT		void	Tu_pno_set(BYTE);				/* ｶﾚﾝﾄPresetNoｾｯﾄ */
EXT		void	Tu_aftermute(BYTE);				/* AfterMute ｾｯﾄ処理 */
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
