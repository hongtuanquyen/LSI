/******************************************************************************
..     NIaü¯                    2007ÓÃÞÙ

		vWFNg¼	:
		t@C¼		: tu_item.h
		@\			: @Tuner itemwb_Ì§²Ù
------------------------------------------------------------------------------
		C³ð	yNúzyVersionzy¼Oz yà¾z
------------------------------------------------------------------------------
[sun051213]	VKì¬
******************************************************************************/
#ifndef	_TU_ITEM_H_
#define	_TU_ITEM_H_

#undef	EXT
#ifdef	_TU_ITEM_C_
#define	EXT
#else
#define	EXT	extern
#endif

/* Itemßèl */
#define		CTU_I_OKEND		0xff			/* ITEMOK®¹ */
#define		CTU_I_NGEND		0xfe			/* ITEMNG®¹ */
#define		CTU_I_NOTEND	0xfd			/* ITEM */

/* ItemJÚl */
#define		CTU_NEXT_M		0xff			/* ÌÒ²ÝÓ°ÄÞðs¤ */
#define		CTU_CONT		0xfe			/* A±Ò²ÝÉßçÈ¢	*/
#define		CTU_NOOPT		0xfd			/* Ò²ÝÉßé */
#define		CTU_OKEND		0xfc
#define		CTU_NGEND		0xfb
/*----------------------------------------------------
   ÖÌßÛÄº°Ù
----------------------------------------------------*/
EXT	void	Tu_main_process(void);
EXT	void	Tu_I_initial(void);
EXT	BYTE	Tu_I_premute(void);
EXT	BYTE	Tu_I_icreset(void);
EXT	BYTE	Tu_I_aftermute(void);
EXT	BYTE	Tu_I_frqset(void);
EXT	BYTE	Tu_I_bndpara(void);
EXT BYTE	Tu_I_pstwait(void);
EXT	BYTE 	Tu_I_pstwrite(void);
EXT	BYTE	Tu_I_updw(void);
EXT	BYTE	Tu_I_man(void);
EXT	BYTE	Tu_I_cwait(void);
EXT	BYTE	Tu_I_seekst(void);
EXT	BYTE 	Tu_I_qinfochk(void);
EXT BYTE	Tu_I_seekend(void);
EXT	BYTE	Tu_I_isrread(void);
EXT	BYTE 	Tu_I_apmst(void);
EXT	BYTE 	Tu_I_apmset(void);
EXT	BYTE 	Tu_I_apmendchk(void);
EXT	BYTE 	Tu_I_apmend(void);
EXT	BYTE	Tu_I_pstup(void);
EXT	BYTE	Tu_I_scanwait(void);
EXT	BYTE 	Tu_I_pstchk(void);

#endif
