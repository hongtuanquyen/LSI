/********************************************************************************
FIlE NAME	:pf_frmdef.h
DESCRIPTION	:�S�̐���i�t���[���A�h���X�j���J�w�b�_
REMARK		:
DATE		:2015/05/15
COPYRIGHT	:Panasonic Co.,Ltd.
*********************************************************************************/
#ifndef	_PF_FRMDEF_H_
#define	_PF_FRMDEF_H_

/************************************
	�}�N����` / �񋓌^				
*************************************/
/*=====	�t���[���A�h���X	=====*/
#define		WIDTH			800		/*	��		*/
#define		VGA_WIDTH       640     /*	��(VGA�g�k�\���p)	*/
#define		HEIGHT			480		/*	����	*/
#define		BTN_HEIGHT		138		/*	�{�^���p����	*//*FrameBuff�z�u�ύX @02012_8p[2013/10/26]*/

/* G3_SI@021_004h[2013/11/25] start ---- */
#if	!defined( GFX3D_MAKE_SW ) && !defined( MXE_MAKE_SW )	/* 3D or MXE�t�s�ȊO start ---- *//*FrameBuff�z�u�ύX @02012_8p[2013/10/26] start ---- */

#if defined (_G3MODE_S) || defined (_G3MODE_P)	/* SDRAM�񓋍ڃ{�[�h�� *//* G3_SI@025_004h[2014/02/04] */
#define	FRAME1_ADDR		(0x544D2000)		/* �t���[���o�b�t�@��eDRAM�Ƃ��Đݒ� */
#else/* _G3MODE_S / _G3MODE_P */
#define	FRAME1_ADDR		(0x58000000)		/* Audio2DEC@01091_2w[13/12/03] *//* ��L���b�V���A�N�Z�X�ϐ���eDRAM�ցA�t���[���o�b�t�@��SDRAM�ֈړ��B�ύX�O=0x544D2000 */
#endif

#define	FRAME2_ADDR		(FRAME1_ADDR + (WIDTH*BTN_HEIGHT))			/* GFX2D��QAC�Ή� @02022_8p[2014/03/18] start ---- */
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
#define	D3FRM2_ADDR		FRAME5_ADDR									/* GFX2D��QAC�Ή� @02022_8p[2014/03/18] end ---- */

#ifdef _UACS_IPOD_FUNCTION		/* Gerda3 Audio @01033_6f[2013/09/9] start ---- */
/* iPod Artwork �ϊ��t���[���A�h���X���T�C�Y���� */
#define	CONVFRM_ADDR		(0x58323E80)
#define	CONVFRM_WIDTH		(128)
#define	CONVFRM_HEIGHT		(128)
#endif/* _UACS_IPOD_FUNCTION */	/* Gerda3 Audio @01033_6f[2013/09/9] end ---- */


#if	UA_USE_PLP_ARTWORK			/* Gerda3 Audio @01121_6f[2014/2/10] start ---- */
/* �����Đ� Artwork �ϊ��t���[���A�h���X���T�C�Y���� */
#if defined (_G3MODE_S) || defined (_G3MODE_P)	/* SDRAM�񓋍ڃ{�[�h�� *//* G3_SI@025_004h[2014/02/04] */
#define	PLP_CONVFRM_ADDR	(0x54735E80)
#else/* _G3MODE_S / _G3MODE_P �łȂ� */
#define	PLP_CONVFRM_ADDR	(0x583D1E80)
#endif/* _G3MODE_S / _G3MODE_P */

#define	PLP_CONVFRM_WIDTH	(640)
#define	PLP_CONVFRM_HEIGHT	(640)
#define	PLP0_CONVFRM_ADDR	(PLP_CONVFRM_ADDR)
#define	PLP1_CONVFRM_ADDR	(PLP_CONVFRM_ADDR + PLP_CONVFRM_WIDTH*PLP_CONVFRM_HEIGHT*2)
#endif/* UA_USE_PLP_ARTWORK */	/* Gerda3 Audio @01121_6f[2014/2/10]end ----*/
															/* 3D or MXE�t�s�ȊO end ---- *//*FrameBuff�z�u�ύX @02012_8p[2013/10/26] end ---- */

#else	/* !(GFX3D_MAKE_SW) && !(MXE_MAKE_SW) */			/* 3D or MXE�t�s�� start ---- *//* G3_Common@011_004h[2013/11/15] start ---- */

#define	FRAME1_ADDR		0x58000000		/* Audio2DEC@01090_2w[13/12/03] *//* 3D�`��̃��[�N�̈�Əd�Ȃ�Ȃ��悤�ύX�B�ύX�O=0x581C0000 */
#define	FRAME2_ADDR		(FRAME1_ADDR + (WIDTH*BTN_HEIGHT))			/* GFX2D��QAC�Ή� @02022_8p[2014/03/18] start ---- */
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
#define	D3FRM2_ADDR		FRAME5_ADDR									/* GFX2D��QAC�Ή� @02022_8p[2014/03/18] end ---- */

#ifdef _UACS_IPOD_FUNCTION		/* Gerda3 Audio @01033_6f[2013/09/9] start ---- */
/* iPod Artwork �ϊ��t���[���A�h���X���T�C�Y���� */
#define	CONVFRM_ADDR		(0x58323E80)
#define	CONVFRM_WIDTH		(128)
#define	CONVFRM_HEIGHT		(128)
#endif/* _UACS_IPOD_FUNCTION */	/* Gerda3 Audio @01033_6f[2013/09/9] end ---- */

#if	UA_USE_PLP_ARTWORK			/* Gerda3 Audio @01121_6f[2014/2/10] start ---- */
/* �����Đ� Artwork �ϊ��t���[���A�h���X���T�C�Y���� */
#define	PLP_CONVFRM_ADDR	(0x583D1E80)
#define	PLP_CONVFRM_WIDTH	(640)
#define	PLP_CONVFRM_HEIGHT	(640)
#define	PLP0_CONVFRM_ADDR	(PLP_CONVFRM_ADDR)
#define	PLP1_CONVFRM_ADDR	(PLP_CONVFRM_ADDR + PLP_CONVFRM_WIDTH*PLP_CONVFRM_HEIGHT*2)
#endif/*UA_USE_PLP_ARTWORK*/	/* Gerda3 Audio @01121_6f[2014/2/10] end ---- */

#endif	/* !(GFX3D_MAKE_SW) && !(MXE_MAKE_SW) */			/* 3D or MXE�t�s�� end ---- *//* G3_Common@011_004h[2013/11/15] end ---- */

#endif	/* _PF_FRMDEF_H_ */
