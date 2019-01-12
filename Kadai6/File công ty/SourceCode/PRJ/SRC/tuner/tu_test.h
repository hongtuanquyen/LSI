#ifndef	_TU_TEST_H
#define	_TU_TEST_H

#undef	EXT
#ifdef	_TU_TEST_C
#define	EXT
#else
#define	EXT	extern
#endif

EXT	BYTE	tu_test_mode;
EXT	BYTE	tu_test_value;
EXT	BYTE	tu_test_seq;

EXT	BYTE	tu_sme_cnt;
EXT	BYTE	tu_if_cnt;
EXT	BYTE	tu_usn_cnt;
EXT	BYTE	tu_wam_cnt;

EXT	BYTE	tu_sme_ave[2];
EXT	BYTE	tu_if_ave[2];
EXT	BYTE	tu_usn_ave[2];
EXT	BYTE	tu_wam_ave[2];

EXT	BYTE	tu_level_FM1db;
EXT	BYTE	tu_level_AM1db;

/* [matu120926] テストモード対応 */
EXT	void	Tu_cmd_func(BYTE RcvCmd,BYTE *RcvBuf,BYTE Lenth);
EXT	void	Tu_test_dirpch(BYTE *RcvBuf);
EXT	void	Tu_test_dirnch(BYTE *RcvBuf);
EXT	void	Tu_test_dxlochg(BYTE *RcvBuf);
EXT	void	Tu_test_stareply(void);
EXT	void	Tu_test_atomicget(BYTE *RcvBuf,BYTE destno);
EXT	BYTE	Tu_test_invalidchk(BYTE atomic_req);
EXT	void	Tu_Smeter_Check(void);
EXT	void	Tu_SmEep_Write(void);
EXT	void	Tu_Data_Reply(void);

EXT	WORD	Tu_Chg_WORD( BYTE * Byte_Data );
EXT	void	Tu_Add_WORD( BYTE * Byte_Data , WORD Add_Data);
EXT	BYTE	Tu_Level_db_Chg( BYTE d_Level20 , BYTE d_Level40 );
EXT	BYTE	Tu_FMLeveldB_Conv(BYTE level);
EXT	BYTE	Tu_AMLeveldB_Conv(BYTE level);
EXT	BYTE	Tu_Average_QInfo( BYTE *rxbuf );
EXT	BYTE	Tu_QInfo_Chk(BYTE chk_tm, TTU_QINFO *pQInfo);
EXT	BYTE	Tu_QInfo_SmeterChk(BYTE Item,BYTE data);
EXT	BYTE	Tu_QInfo_IFChk(BYTE Item, BYTE data);
EXT	BYTE	Tu_CheckFM_USN(BYTE Item, BYTE data);
EXT	BYTE	Tu_CheckFM_WAM(BYTE Item, BYTE data);
EXT	BYTE	Tu_at_QinfoAvg(BYTE TotalNum,TTU_QI_CALC* pqinf,BYTE value);
EXT	BYTE	Tu_QinfoAvg(BYTE TotalNum, BYTE *pNum, BYTE *pData, BYTE value);

EXT	void	Tu_Test_Main(void);
EXT	void	Tu_5ms_test(void);


#endif
