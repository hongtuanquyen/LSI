/********************************************************************************
FIlE NAME	:pf_frmdef.h
DESCRIPTION	:全体制御（フレームアドレス）公開ヘッダ
REMARK		:
DATE		:2015/05/15
COPYRIGHT	:Panasonic Co.,Ltd.
*********************************************************************************/
#ifndef	_PF_FRMDEF_H_
#define	_PF_FRMDEF_H_

/************************************
	マクロ定義 / 列挙型				
*************************************/
/*=====	フレームアドレス	=====*/
#define		WIDTH			800		/*	幅		*/
#define		VGA_WIDTH       640     /*	幅(VGA拡縮表示用)	*/
#define		HEIGHT			480		/*	高さ	*/
#define		BTN_HEIGHT		138		/*	ボタン用高さ	*//*FrameBuff配置変更 @02012_8p[2013/10/26]*/

/* G3_SI@021_004h[2013/11/25] start ---- */
#if	!defined( GFX3D_MAKE_SW ) && !defined( MXE_MAKE_SW )	/* 3D or MXE輻輳以外 start ---- *//*FrameBuff配置変更 @02012_8p[2013/10/26] start ---- */

#if defined (_G3MODE_S) || defined (_G3MODE_P)	/* SDRAM非搭載ボード時 *//* G3_SI@025_004h[2014/02/04] */
#define	FRAME1_ADDR		(0x544D2000)		/* フレームバッファをeDRAMとして設定 */
#else/* _G3MODE_S / _G3MODE_P */
#define	FRAME1_ADDR		(0x58000000)		/* Audio2DEC@01091_2w[13/12/03] *//* 非キャッシュアクセス変数をeDRAMへ、フレームバッファをSDRAMへ移動。変更前=0x544D2000 */
#endif

#define	FRAME2_ADDR		(FRAME1_ADDR + (WIDTH*BTN_HEIGHT))			/* GFX2DのQAC対応 @02022_8p[2014/03/18] start ---- */
#define	FRAME3_ADDR		(FRAME1_ADDR + ((WIDTH*BTN_HEIGHT)*2))
#define	FRAME4_ADDR		(FRAME3_ADDR + ((WIDTH*HEIGHT)*2))
#define	FRAME5_ADDR		(FRAME3_ADDR + (((WIDTH*HEIGHT)*2)*2))
#define	FRAME6_ADDR		(FRAME3_ADDR + (((WIDTH*HEIGHT)*2)*3))

#define	HUDFRM1_ADDR	FRAME3_ADDR
#define	HUDFRM2_ADDR	(FRAME3_ADDR + (WIDTH*HEIGHT))

#define	ICONFRM1_ADDR	FRAME5_ADDR
#define	ICONFRM2_ADDR	(FRAME5_ADDR + (WIDTH*HEIGHT))

#define	MLFRM1_ADDR		FRAME3_ADDR
#define	MLFRM2_ADDR		FRAME5_ADDR 

#define	D3FRM1_ADDR		FRAME3_ADDR
#define	D3FRM2_ADDR		FRAME5_ADDR									/* GFX2DのQAC対応 @02022_8p[2014/03/18] end ---- */

#ifdef _UACS_IPOD_FUNCTION		/* Gerda3 Audio @01033_6f[2013/09/9] start ---- */
/* iPod Artwork 変換フレームアドレス＆サイズ制限 */
#define	CONVFRM_ADDR		(0x58323E80)
#define	CONVFRM_WIDTH		(128)
#define	CONVFRM_HEIGHT		(128)
#endif/* _UACS_IPOD_FUNCTION */	/* Gerda3 Audio @01033_6f[2013/09/9] end ---- */


#if	UA_USE_PLP_ARTWORK			/* Gerda3 Audio @01121_6f[2014/2/10] start ---- */
/* 平文再生 Artwork 変換フレームアドレス＆サイズ制限 */
#if defined (_G3MODE_S) || defined (_G3MODE_P)	/* SDRAM非搭載ボード時 *//* G3_SI@025_004h[2014/02/04] */
#define	PLP_CONVFRM_ADDR	(0x54735E80)
#else/* _G3MODE_S / _G3MODE_P でない */
#define	PLP_CONVFRM_ADDR	(0x583D1E80)
#endif/* _G3MODE_S / _G3MODE_P */

#define	PLP_CONVFRM_WIDTH	(640)
#define	PLP_CONVFRM_HEIGHT	(640)
#define	PLP0_CONVFRM_ADDR	(PLP_CONVFRM_ADDR)
#define	PLP1_CONVFRM_ADDR	(PLP_CONVFRM_ADDR + PLP_CONVFRM_WIDTH*PLP_CONVFRM_HEIGHT*2)
#endif/* UA_USE_PLP_ARTWORK */	/* Gerda3 Audio @01121_6f[2014/2/10]end ----*/
															/* 3D or MXE輻輳以外 end ---- *//*FrameBuff配置変更 @02012_8p[2013/10/26] end ---- */

#else	/* !(GFX3D_MAKE_SW) && !(MXE_MAKE_SW) */			/* 3D or MXE輻輳時 start ---- *//* G3_Common@011_004h[2013/11/15] start ---- */

#define	FRAME1_ADDR		0x58000000		/* Audio2DEC@01090_2w[13/12/03] *//* 3D描画のワーク領域と重ならないよう変更。変更前=0x581C0000 */
#define	FRAME2_ADDR		(FRAME1_ADDR + (WIDTH*BTN_HEIGHT))			/* GFX2DのQAC対応 @02022_8p[2014/03/18] start ---- */
#define	FRAME3_ADDR		(FRAME1_ADDR + ((WIDTH*BTN_HEIGHT)*2))
#define	FRAME4_ADDR		(FRAME3_ADDR + ((WIDTH*HEIGHT)*2))
#define	FRAME5_ADDR		(FRAME3_ADDR + (((WIDTH*HEIGHT)*2)*2))
#define	FRAME6_ADDR		(FRAME3_ADDR + (((WIDTH*HEIGHT)*2)*3))

#define	HUDFRM1_ADDR	FRAME3_ADDR
#define	HUDFRM2_ADDR	(FRAME3_ADDR + (WIDTH*HEIGHT))

#define	ICONFRM1_ADDR	FRAME5_ADDR
#define	ICONFRM2_ADDR	(FRAME5_ADDR + (WIDTH*HEIGHT))

#define	MLFRM1_ADDR		FRAME3_ADDR
#define	MLFRM2_ADDR		FRAME5_ADDR 

#define	D3FRM1_ADDR		FRAME3_ADDR
#define	D3FRM2_ADDR		FRAME5_ADDR									/* GFX2DのQAC対応 @02022_8p[2014/03/18] end ---- */

#ifdef _UACS_IPOD_FUNCTION		/* Gerda3 Audio @01033_6f[2013/09/9] start ---- */
/* iPod Artwork 変換フレームアドレス＆サイズ制限 */
#define	CONVFRM_ADDR		(0x58323E80)
#define	CONVFRM_WIDTH		(128)
#define	CONVFRM_HEIGHT		(128)
#endif/* _UACS_IPOD_FUNCTION */	/* Gerda3 Audio @01033_6f[2013/09/9] end ---- */

#if	UA_USE_PLP_ARTWORK			/* Gerda3 Audio @01121_6f[2014/2/10] start ---- */
/* 平文再生 Artwork 変換フレームアドレス＆サイズ制限 */
#define	PLP_CONVFRM_ADDR	(0x583D1E80)
#define	PLP_CONVFRM_WIDTH	(640)
#define	PLP_CONVFRM_HEIGHT	(640)
#define	PLP0_CONVFRM_ADDR	(PLP_CONVFRM_ADDR)
#define	PLP1_CONVFRM_ADDR	(PLP_CONVFRM_ADDR + PLP_CONVFRM_WIDTH*PLP_CONVFRM_HEIGHT*2)
#endif/*UA_USE_PLP_ARTWORK*/	/* Gerda3 Audio @01121_6f[2014/2/10] end ---- */

#endif	/* !(GFX3D_MAKE_SW) && !(MXE_MAKE_SW) */			/* 3D or MXE輻輳時 end ---- *//* G3_Common@011_004h[2013/11/15] end ---- */

#endif	/* _PF_FRMDEF_H_ */
