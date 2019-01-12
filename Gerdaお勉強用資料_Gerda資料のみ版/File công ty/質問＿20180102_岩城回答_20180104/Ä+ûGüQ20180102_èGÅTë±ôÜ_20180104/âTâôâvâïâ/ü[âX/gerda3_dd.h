/******************************************************************
File Name	: gerda3_dd.h
Description : ライブラリ定数宣言/関数プロトタイプ宣言 ヘッダ
Remark		: 履歴 2013/04/18 初版
Date		: 2013/04/18
Copyright	: Panasonic Co. , Ltd.
******************************************************************/
#ifndef		_GERDA3_DD_H
#define		_GERDA3_DD_H

/**********************************************
 *  各種定数宣言
 **********************************************/
#ifndef	FALSE
#define		FALSE			0				/* 偽 */
#endif
#ifndef	TRUE
#define		TRUE			(!FALSE)		/* 真 */
#endif
#ifndef NULL
#define		NULL			0				/* NULL */
#endif

#define		ST_SUCCESS				0		/* 関数が正常終了 */
#define		ST_ERROR				(-1)	/* 関数が異常終了 */
#define		ST_WARNING				1		/* 関数で警告あり */

/* フレームの種別 */
#define		GD_FMT_P1				0x0c
#define		GD_FMT_P4				0x08
#define		GD_FMT_A4				0x0a
#define		GD_FMT_P8				0x00
#define		GD_FMT_A1P7				0x01
#define		GD_FMT_A4P4				0x02
#define		GD_FMT_A8				0x03
#define		GD_FMT_ARGB4444			0x04
#define		GD_FMT_ARGB1555			0x06
#define		GD_FMT_RGB565			0x07
#define		GD_FMT_ARGB8888			0x09
#define		GD_FMT_YUV16			0x0b
#define		GD_FMT_YUV32			0x0d
#define		GD_FMT_YUV32_8			0x0e
#define		GD_FMT_UNDEF			0xFFFFFFFF

/* 描画モード */
#define		GD_OUT_FRM				0x01
#define		GD_OUT_STN				0x02
#define		GD_REF_STN				0x04

#define		GD_ALPH_ARGB			0x00
#define		GD_ALPH_A				0x10
#define		GD_ALPH_RGB				0x02
#define		GD_ALPH_IGNORE			0x01

/* ステンシルへの出力モード */
#define		GD_STN_REPLACE			0x00
#define		GD_STN_CLEAR			0x10
#define		GD_STN_AND				0x20
#define		GD_STN_NAND				0x30
#define		GD_STN_OR				0x40
#define		GD_STN_NOR				0x50
#define		GD_STN_XOR				0x60
#define		GD_STN_XNOR				0x70

/* テクスチャ参照モード設定 */
#define		GD_TXT_REFER_OFF		0x0000	/* テクスチャ参照なし		*/
#define		GD_TXT_REFER_REP		0x0001	/* テクスチャ参照繰り返し	*/
#define		GD_TXT_REFER_CLAMP		0x0002	/* テクスチャ参照クランプ	*/
#define		GD_TXT_BILINEAR			0x0004	/* バイリニアフィルタ有効	*/
#define		GD_TXT_TRANS			0x0008	/* 透過色設定有効			*/
#define		GD_TXT_PERS				0x0010	/* パース補正有効			*/
#define		GD_TXT_SHADE			0x0020	/* グーローシェーディング	*/
#define		GD_TXT_ANTIALIAS		0x0040	/* アンチエイリアス有効		*/
#define		GD_TXT_ARRAY_NORMAL		0x0000	/* 通常モード		*/
#define		GD_TXT_ARRAY_STRIP		0x0100	/* STRIP モード		*/
#define		GD_TXT_ARRAY_FAN		0x0180	/* FAN モード		*/
#define		GD_TXT_RD				0x8000

/* 頂点配列タイプ */
#define		GD_ART_XY				0x0000
#define		GD_ART_XYUV				0x2000
#define		GD_ART_XYUVZ			0x6000
#define		GD_ART_XYC				0x1000
#define		GD_ART_XYUVC			0x3000
#define		GD_ART_XYUVZC			0x7000
#define		GD_ART_ARGB4444			0x0000
#define		GD_ART_ARGB1555			0x0200
#define		GD_ART_RGB565			0x0400

/* マスク定数 */
#define		GD_MSK_ALL				0xffffffff
#define		GD_MSK_REFER			0x0003
#define		GD_MSK_BILINEAR			0x0004
#define		GD_MSK_TRANS			0x0008
#define		GD_MSK_PERS				0x0010
#define		GD_MSK_SHADE			0x0020
#define		GD_MSK_ANTIALIAS		0x0040
#define		GD_MSK_ARRAY_MODE		0x0180
#define		GD_MSK_ARRAY_TYPE		0x7e00

/* 色定数 */
#define		GD_COLOR0				0
#define		GD_COLOR1				1
#define		GD_SCOLOR				2
#define		GD_CCOLOR0				3
#define		GD_CCOLOR1				4
#define		GD_TCOLOR				5

#define		GD_DMTX_PT0				0x082ac4e6,0x3b19f7d5		/* ディザ行列:Bayer型 */
#define		GD_DMTX_PT1				0x6789501a,0x432bfedc		/* ディザ行列:渦巻型  */
#define		GD_DMTX_PT2				0xb469c02e,0x78a53fd1		/* ディザ行列:網点型  */
#define		GD_DMTX_PT3				0xf86125bc,0x9e0743da		/* ディザ行列:魔方陣型 */
#define		GD_DMTX_NONE			0,0							/* ディザ行列:無効化  */

/* 表示パス定数 */
#define		GD_RGB_LAYER0			0x00
#define		GD_RGB_LAYER1			0x01
#define		GD_YUV_PATH0			0x02
#define		GD_YUV_PATH1			0x03
#define		GD_THRGB_PATH			0x04
#define		GD_RGB_LAYER2			0x05

/* 表示レイヤ定数 */
#define		GD_LAYER00				0x00
#define		GD_LAYER01				0x01
#define		GD_LAYER02				0x02
#define		GD_LAYER03				0x03
#define		GD_LAYER04				0x04
#define		GD_LAYER10				0x05
#define		GD_LAYER11				0x06
#define		GD_SUBLAYER00			0x08
#define		GD_SUBLAYER01			0x09
#define		GD_SUBLAYER02			0x0a
#define		GD_SUBLAYER03			0x0b
#define		GD_SUBLAYER10			0x0c
#define		GD_SUBLAYER11			0x0d
#define		GD_SUBLAYER12			0x0e
#define		GD_SUBLAYER13			0x0f
#define		GD_SUBLAYER20			0x10
#define		GD_SUBLAYER21			0x11
#define		GD_SUBLAYER22			0x12
#define		GD_SUBLAYER23			0x13

/* レイヤ制御定数 */
#define		GD_LYR_DISP_OFF			0x0000	/* レイヤ割り当て解除 */
#define		GD_LYR_DISP_ON			0x1000	/* レイヤ表示 ON */
#define		GD_LYR_TRNSC_ON			0x0002	/* レイヤ透過色有効 */
#define		GD_LYR_TRNSL_C			0x0009	/* 半透明処理有効(各ピクセルのα値を無視) */
#define		GD_LYR_TRNSL_P			0x0008	/* 半透明処理有効(各ピクセルのα値を有効) */
#define		GD_LYR_MIRROR_H			0x0200	/* 左右反転処理有効(YUV レイヤのみ) */
#define		GD_LYR_MIRROR_V			0x0100	/* 上下反転処理有効(YUV レイヤのみ) */
#define		GD_LYR_PALETTE0			0x0000	/* P8 表示時にパレット0 参照 */
#define		GD_LYR_PALETTE1			0x4000	/* P8 表示時にパレット1 参照 */

/* FlashROM 制御系 API 定数 */
#define		GD_SFLASH_WRITE			0x1000
#define		FLASH_CS0				((USHORT *)0xA0000000)
#define		FLASH_CS1				((USHORT *)0xB0000000)
#define		FLASH_CS2				((USHORT *)0xC0000000)

/* PINMODEレジスタの値 */
#define		GD_PM_S					5		/* LCD 2ch, Serial Flash,   No SDRAM */
#define		GD_PM_P					4		/* LCD 1ch, Parallel Flash, No SDRAM */
#define		GD_PM_DN				3		/* LCD 1ch, Parallel Flash, SDRAM 16 bit */
#define		GD_PM_DD				2		/* LCD 1ch, Serial Flash,   SDRAM 32 bit */
#define		GD_PM_DL				1		/* LCD 2ch, Serial Flash,   SDRAM 16 bit */
#define		GD_PM_DV				0		/* LCD 2ch, Serial Flash,   No SDRAM */

/* 各APIでonoff引数に指定する値 */
#define		GD_OFF					0
#define		GD_ON					(!GD_OFF)

/* 各APIでonoff引数に指定する値 */
#define		GD_H_ON					3
#define		GD_V_ON					5

/* 拡大ニアレストネイバーオプション */
#define		GD_NEAR					8

/* VOUTチャンネル番号 */
#define		GD_VO_0					0
#define		GD_VO_1					1
#define		GD_VO_NOW				0xffffffff

/* 同期モード */
#define		GD_SYNC_IN				0	/* 内部同期 (デフォルト) */
#define		GD_SYNC_EXT				1	/* 外部同期 */
#define		GD_SYNC_V0				2	/* 入力映像垂直同期（VIN0に同期） */
#define		GD_SYNC_V1				3	/* 入力映像垂直同期（VIN1に同期） */

/* vi_distinct_conf,vi_distinct_fscで指定する値 */
#define		GD_VI_AUTO				0
#define		GD_VI_FIX				1
#define		GD_VI_NOW				0xffffffff

/* dsp_s_formatで指定する出力形式 */
#define		GD_VO_RGB666			0x00
#define		GD_VO_RGB888			0x01
#define		GD_VO_REC656			0x04
#define		GD_VO_YUV422			0x05
#define		GD_VO_YUV444			0x06
#define		GD_VO_FSEL				0x10	/* GPIOの機能選択を自動設定 */

/* vi_set_cap_ip_convで指定する値 */
#define		GD_VI_IPCV_FULL				0x00	/* 両フィールド (デフォルト) */
#define		GD_VI_IPCV_HALF_FLD1		0x05	/* 片フィールド（第1フィールドのみ格納） */
#define		GD_VI_IPCV_HALF_FLD2		0x01	/* 片フィールド（第2フィールドのみ格納） */
#define		GD_VI_IPCV_INTPOL			0x02	/* フィールド補間 */
#define		GD_VI_IPCV_SHIFT_HALFLINE	0x10	/* 0.5 ラインずらし（第1フィールドをずらす） */
#define		GD_VI_IPCV_SHIFT_HALFLINE_2	0x30	/* 0.5 ラインずらし（第2フィールドをずらす） */

#define		GD_VI_NTSC358			0x00
#define		GD_VI_PALBGHI			0x01
#define		GD_VI_PALM				0x02
#define		GD_VI_PALN				0x03
#define		GD_VI_SECAM				0x04
#define		GD_VI_NTSC443			0x05
#define		GD_VI_525P				0x08
#define		GD_VI_625P				0x09
#define		GD_VI_525I				0x0a
#define		GD_VI_625I				0x0b
#define		GD_VI_720P				0x0c
#define		GD_VI_1080I				0x0d
#define		GD_VI_OTHER				0xFFFFFFFF
#define		GD_VI_NTSC				GD_VI_NTSC358
#define		GD_VI_PAL				GD_VI_PALBGHI

/* vi_select_video指定する値 */
#define		GD_VI_NONE				0x00	/* 入力なし(VIN 割込み停止) */
#define		GD_VI_A_CVBS0			0x01	/* アナログコンポジットビデオ */
#define		GD_VI_A_CVBS1			0x02
#define		GD_VI_A_CVBS2			0x03
#define		GD_VI_A_CVBS3			0x04
#define		GD_VI_A_SVIDEO			0x05	/* アナログSVIDEO */
#define		GD_VI_A_YPBPR			0x06	/* アナログコンポーネント */
#define		GD_VI_A_RGB				0x07	/* アナログRGB */
#define		GD_VI_D_RGB0			0x10	/* デジタルRGB（DVIN0） */
#define		GD_VI_D_RGB1			0x11	/* デジタルRGB（DVIN1） */
#define		GD_VI_D_YC0				0x12	/* デジタルYC（DVIN0） */
#define		GD_VI_D_YC1				0x13	/* デジタルYC（DVIN1） */
#define		GD_VI_D_YUV0			0x14	/* デジタルYUV（DVIN0） */
#define		GD_VI_D_YUV1			0x15	/* デジタルYUV（DVIN1） */
#define		GD_VI_D_6010			0x16	/* デジタル601（DVIN0） */
#define		GD_VI_D_6011			0x17	/* デジタル601（DVIN1） */
#define		GD_VI_D_6560			0x18	/* デジタル656（DVIN0） */
#define		GD_VI_D_6561			0x19	/* デジタル656（DVIN1） */

#define		GD_VI_SIZE_WQVGA0		vi_a_wqvga0_regs	/* アナログRGB WQVGA (480×234)	 9.600MHz (水平：15.66068515kHz, 垂直：60.00262511fps) */
#define		GD_VI_SIZE_WQVGA1		vi_a_wqvga1_regs	/* アナログRGB WQVGA (400×234)	 8.000MHz (水平：15.74803150kHz, 垂直：59.87844675fps) */
#define		GD_VI_SIZE_VGA			vi_a_vga_regs		/* アナログRGB VGA   (640×480)	25.175MHz (水平：31.46875000kHz, 垂直：59.94047619fps) */
#define		GD_VI_SIZE_WVGA			vi_a_wvga_regs		/* アナログRGB WVGA  (800×480)	33.000MHz (水平：31.25000000kHz, 垂直：59.52380952fps) */
#define		GD_VI_SIZE_XGA			vi_a_xga_regs		/* アナログRGB XGA  (1024×768)	65.000MHz (水平：48.36309524kHz, 垂直：60.00384025fps) */
#define		GD_VI_SIZE_WXGA_RB		vi_a_wxga_rb_regs	/* アナログRGB WXGA (1280×800)	65.000MHz (水平：48.87218045kHz, 垂直：60.18741435fps) */
#define		GD_VI_SIZE_WXGA			vi_a_wxga_regs		/* アナログRGB WXGA (1280×800)	71.000MHz (水平：49.30555556kHz, 垂直：59.54777241fps) */

#define		GD_VI_SIZE_YC525I		vi_a_525i_regs		/* コンポーネント  525i  (858× 525) 13.50MHz (水平：15.73kHz, 垂直：29.97fps) */
#define		GD_VI_SIZE_YC525P		vi_a_525p_regs		/* コンポーネント  525p  (858× 525) 27.00MHz (水平：31.47kHz, 垂直：59.94fps) */
#define		GD_VI_SIZE_YC625I		vi_a_625i_regs		/* コンポーネント  625i  (864× 625) 13.50MHz (水平：15.63kHz, 垂直：25.00fps) */
#define		GD_VI_SIZE_YC625P		vi_a_625p_regs		/* コンポーネント  625p  (864× 625) 27.00MHz (水平：31.25kHz, 垂直：50.00fps) */
#define		GD_VI_SIZE_YC720P		vi_a_720p_regs		/* コンポーネント  720p (1280× 720) 74.25MHz (水平：44.95504496kHz, 垂直：59.94005994fps) */
#define		GD_VI_SIZE_YC1080I		vi_a_1080i_regs		/* コンポーネント 1080i (1920×1080) 74.25MHz (水平：33.71628372kHz, 垂直：29.97002997fps) */

#define		GD_VI_OPT_8BIT			vi_d_rgb888_regs		/* D_RGB 8bit入力 */

#define		GD_VI_OPT_INTERLACE		vi_d_interlace_regs	/* D_* 入力時インターレス入力 */
#define		GD_VI_OPT_444			vi_d_yuv444_regs	/* D_YUV 4:4:4入力 */

#define		GD_VI_SYNC_DEFAULT		0x00	/* アナログRGB 外部H,V同期 アナログコンポーネント Sync on Y */
#define		GD_VI_SYNC_ONYG			0x40	/* Sync on Y or G */
#define		GD_VI_SYNC_EXTC			0x80	/* 外部コンポジット同期(外部CSync) */
#define		GD_VI_SYNC_EXTHV		0xc0	/* 外部垂直・水平同期(外部H,VSync) */

#define		GD_VI_FSEL				0x100	/* GPIO機能割当て */

/* vi_control_videoで指定する値 */
#define		GD_VI_STOP				0
#define		GD_VI_START				1


/* PWM */
#define		PWM_PORT_000		  0		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_002		  2		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_004		  4		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_008		  8		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_010		 10		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_012		 12		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_014		 14		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_016		 16		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_018		 18		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_020		 20		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_022		 22		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_025		 25		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_027		 27		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_029		 29		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_031		 31		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_033		 33		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_035		 35		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_037		 37		/* PWM_TIMER_3 のとき指定可 */
#define		PWM_PORT_039		 39		/* PWM_TIMER_3 のとき指定可 */
#define		PWM_PORT_041		 41		/* PWM_TIMER_3 のとき指定可 */
#define		PWM_PORT_082		 82		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_084		 84		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_086		 86		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_088		 88		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_090		 90		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_092		 92		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_146		146		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_148		148		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_150		150		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_152		152		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_154		154		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_156		156		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_001		  1		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_003		  3		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_005		  5		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_009		  9		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_011		 11		/* PWM_TIMER_0 のとき指定可 */
#define		PWM_PORT_013		 13		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_015		 15		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_017		 17		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_019		 19		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_021		 21		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_023		 23		/* PWM_TIMER_1 のとき指定可 */
#define		PWM_PORT_026		 26		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_028		 28		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_030		 30		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_032		 32		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_034		 34		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_036		 36		/* PWM_TIMER_2 のとき指定可 */
#define		PWM_PORT_038		 38		/* PWM_TIMER_3 のとき指定可 */
#define		PWM_PORT_040		 40		/* PWM_TIMER_3 のとき指定可 */
#define		PWM_PORT_042		 42		/* PWM_TIMER_3 のとき指定可 */
#define		PWM_PORT_083		 83		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_085		 85		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_087		 87		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_089		 89		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_091		 91		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_093		 93		/* PWM_TIMER_4 のとき指定可 */
#define		PWM_PORT_147		147		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_149		149		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_151		151		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_153		153		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_155		155		/* PWM_TIMER_5 のとき指定可 */
#define		PWM_PORT_157		157		/* PWM_TIMER_5 のとき指定可 */

/* その他 */
#define		GD_WQVGA_ON			1
#define		GD_WVGA_ON			2

#define		THYUV_PORT_000		  0
#define		THYUV_PORT_001		  1
#define		THYUV_PORT_002		  2
#define		THYUV_PORT_010		 10
#define		THYUV_PORT_011		 11
#define		THYUV_PORT_012		 12
#define		THYUV_PORT_017		 17
#define		THYUV_PORT_018		 18
#define		THYUV_PORT_019		 19
#define		THYUV_PORT_021		 21
#define		THYUV_PORT_022		 22
#define		THYUV_PORT_023		 23
#define		THYUV_PORT_029		 29
#define		THYUV_PORT_030		 30
#define		THYUV_PORT_031		 31
#define		THYUV_PORT_170		170
#define		THYUV_PORT_171		171
#define		THYUV_PORT_172		172
#define		THYUV_PORT_NULL		255

#define		THYUV_PORT_SET_0	((THYUV_PORT_002<<16)+(THYUV_PORT_001<<8)+THYUV_PORT_000)
#define		THYUV_PORT_SET_2	((THYUV_PORT_012<<16)+(THYUV_PORT_011<<8)+THYUV_PORT_010)
#define		THYUV_PORT_SET_3	((THYUV_PORT_019<<16)+(THYUV_PORT_018<<8)+THYUV_PORT_017)
#define		THYUV_PORT_SET_4	((THYUV_PORT_023<<16)+(THYUV_PORT_022<<8)+THYUV_PORT_021)
#define		THYUV_PORT_SET_5	((THYUV_PORT_031<<16)+(THYUV_PORT_030<<8)+THYUV_PORT_029)
#define		THYUV_PORT_SET_6	((THYUV_PORT_172<<16)+(THYUV_PORT_171<<8)+THYUV_PORT_170)

/**********************************************
 *  各種型宣言
 **********************************************/
#if 0
typedef unsigned long		ULONG;
typedef unsigned short		USHORT;
typedef unsigned char		UCHAR;
typedef signed long			LONG;
typedef signed short		SHORT;
typedef signed char			CHAR;
#else
#ifndef	ULONG
typedef unsigned long		ULONG;
#endif
#ifndef	USHORT
typedef unsigned short		USHORT;
#endif
#ifndef UCHAR
typedef unsigned char		UCHAR;
#endif
#ifndef LONG
typedef signed long			LONG;
#endif
#ifndef SHORT
typedef signed short		SHORT;
#endif
#ifndef CHAR
typedef signed char			CHAR;
#endif
#endif

typedef LONG				RET_ST;
typedef USHORT				POS;

typedef struct {
	ULONG		size;
	USHORT		width;
	USHORT		height;
	UCHAR		type;
} RL_PARM;

/* 引数がULONGで、戻り値がvoidの関数ポインタ型をDSP_LLC_REGSとする */
typedef void	DSP_LLC_REGS(ULONG);

/* vi_get_status APIの戻り値 */
typedef struct {
	UCHAR		cap;
	UCHAR		field;
	UCHAR		bank;
	UCHAR		type;
	UCHAR		mv;
	UCHAR		hlock;
	UCHAR		vlock;
	UCHAR		dcase;
	UCHAR		fix;
	UCHAR		imode;
} VI_ST;



/**********************************************
 *  各種プロトタイプ宣言
 **********************************************/
RET_ST define_frame(ULONG no, void *fadr, ULONG fw, ULONG type);

void drw_init(void);
void drw_dot(LONG x, LONG y);
void drw_line(LONG x1, LONG y1, LONG x2, LONG y2);
void drw_pline(LONG x1, LONG y1, LONG u1, LONG v1, LONG x2, LONG y2, LONG u2);
void drw_rect(LONG x1, LONG y1, LONG x2, LONG y2);
void drw_prect(LONG x1, LONG y1, LONG u1, LONG v1, LONG x2, LONG y2, LONG u2, LONG v2);
void drw_poly4(LONG x1, LONG y1, LONG x2, LONG y2, LONG x3, LONG y3, LONG x4, LONG y4);
void drw_dots(LONG n, POS pos[]);
void drw_lines(LONG n, POS pos[]);
void drw_triangles(LONG n, POS pos[]);
void drw_wait_drawend(void);
void drw_clear(LONG x1, LONG y1, LONG x2, LONG y2);
void drw_rlprect(void *sadr, LONG x, LONG y);

void drw_rlinfo(RL_PARM *rl, void *sadr);
RET_ST drw_set_frame(ULONG no);
RET_ST drw_set_source(ULONG no);
RET_ST drw_set_dsource(void *sadr, ULONG sw, ULONG type);
RET_ST drw_set_stencil(ULONG no);
RET_ST drw_set_clip(ULONG xl, ULONG yl, ULONG xr, ULONG yr);
RET_ST drw_set_texture(ULONG tu, ULONG tv, ULONG tw, ULONG th);
RET_ST drw_set_color(ULONG type, ULONG color);
void drw_set_drawmode(ULONG mode);
void drw_set_textmode(ULONG mask, ULONG mode);
void drw_set_palette(ULONG *plt, ULONG ofs);
void drw_set_lcofs(LONG lx, LONG ly);
void drw_set_alpha(ULONG cnd, ULONG alpha);
void drw_set_width(ULONG width);
void drw_set_dtmtx(ULONG u, ULONG l);
void drw_save_context(void *adr);
void drw_restore_context(void *adr);

/* 表示系 */
void dsp_layer(ULONG lyno, ULONG path, ULONG mode);
void dsp_layer_win(ULONG path, ULONG sizx, ULONG sizy, ULONG srcx, ULONG srcy, ULONG dstx, ULONG dsty);
void dsp_layer_alpha(ULONG path, ULONG alp);
void dsp_layer_tcolor(ULONG path, ULONG clr);
void dsp_layer_bgcolor(ULONG n, ULONG color);
void dsp_sublayer(ULONG lyno, ULONG fm, ULONG mode);
void dsp_sublayer_win(ULONG lyno, ULONG sizx, ULONG sizy, ULONG srcx, ULONG srcy, ULONG dstx, ULONG dsty);
void dsp_sublayer_alpha(ULONG lyno, ULONG alp0, ULONG alp1);
void dsp_sublayer_tcolor(ULONG lyno, ULONG clr);
void dsp_sublayer_bgcolor(ULONG n, ULONG color);
void dsp_set_palette(ULONG n, ULONG *plt);
void dsp_wait_vblank(ULONG n, SHORT count);

void dsp_init(void);
void dsp_sscpll_init(ULONG, ULONG, ULONG);
RET_ST dsp_display ( ULONG,ULONG );
RET_ST dsp_lcd_timing ( ULONG,USHORT,USHORT,USHORT,USHORT,USHORT,USHORT );
RET_ST dsp_sync_mode ( ULONG,ULONG );
RET_ST dsp_layer_drive ( ULONG,ULONG,ULONG,ULONG,ULONG );
RET_ST dsp_layer_cutoff ( ULONG,ULONG,LONG,LONG,LONG );
RET_ST dsp_layer_scaling ( ULONG,ULONG,ULONG,ULONG );
RET_ST dsp_layer_scale_up ( ULONG,ULONG,ULONG,ULONG,USHORT[],USHORT[] );

/* dsp_s系画質調整 */
RET_ST dsp_s_rgb2yuv ( ULONG,ULONG,ULONG,ULONG,ULONG,ULONG );
RET_ST dsp_s_rgb_gamma ( ULONG,ULONG,UCHAR[],USHORT[],UCHAR[],USHORT[],UCHAR[],USHORT[] );
RET_ST dsp_s_noise ( ULONG,ULONG );
RET_ST dsp_s_format ( ULONG,ULONG );
RET_ST dsp_s_drive ( ULONG,ULONG,ULONG,ULONG );
RET_ST dsp_s_cutoff ( ULONG,LONG,LONG,LONG );
RET_ST dsp_s_brightness ( ULONG,LONG );

/* 映像取込み系 */
RET_ST vi_set_cap_mem_bank ( ULONG, ULONG, ULONG );
RET_ST vi_set_cap_ip_conv ( ULONG, ULONG );
RET_ST vi_set_cap_win ( ULONG,ULONG,ULONG,ULONG,ULONG,ULONG );
RET_ST vi_select_video ( ULONG,ULONG,DSP_LLC_REGS* );
ULONG vi_control_video ( ULONG,ULONG,ULONG );
RET_ST vi_get_status ( ULONG,VI_ST* );
RET_ST vi_distinct_conf ( ULONG,ULONG,ULONG );
ULONG vi_distinct_fsc ( ULONG,ULONG,ULONG,ULONG );
RET_ST vi_wait_vblank ( ULONG );
RET_ST vi_scaling_down ( ULONG,ULONG,ULONG,ULONG );
RET_ST vi_scale_down ( ULONG,ULONG,ULONG,ULONG,USHORT[],USHORT[] );

/* vi系画質調整 */
RET_ST vi_det_area_chk ( ULONG,ULONG );
RET_ST vi_det_area_set ( ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG );
RET_ST vi_yuv_delay ( ULONG,ULONG,LONG,LONG );
RET_ST vi_rgb_delay ( ULONG,LONG,LONG,LONG );
RET_ST vi_cti ( ULONG,ULONG,ULONG,ULONG,ULONG );
RET_ST vi_rgb_drive ( ULONG,ULONG,ULONG,ULONG );
RET_ST vi_rgb_cutoff ( ULONG,LONG,LONG,LONG );
RET_ST vi_yuv_gain ( ULONG,ULONG,ULONG,ULONG,ULONG );
RET_ST vi_yuv_offset ( ULONG,ULONG,LONG,LONG,LONG );
RET_ST vi_yuv2rgb ( ULONG,LONG,LONG,LONG,LONG,LONG,LONG );
RET_ST vi_rgb2yuv ( ULONG,ULONG,ULONG,ULONG,ULONG,ULONG );

/* その他 */
RET_ST dsp_set_pwm(ULONG,ULONG,ULONG,ULONG);
RET_ST dsp_thyuv(ULONG onoff, ULONG sys, ULONG gpio, void (*usr_func)(void));

/* FlashROM */
ULONG flm_conv_adr(ULONG adr);
RET_ST flm_program_bw(ULONG *ptr, ULONG *data, LONG size, LONG unit);
RET_ST flm_program(ULONG *ptr, ULONG *data, LONG size);
RET_ST flm_chip_erase(ULONG *ptr);
RET_ST flm_sector_erase(ULONG *ptr);
void flm_dis_access(void *adr);
void flm_ena_access(void *adr);
void flm_p_cmd_write(USHORT *adr, ULONG data);
ULONG flm_p_cmd_read(USHORT *adr);
USHORT flm_s_cmd_rw(void *adr, ULONG cmd, ULONG n, USHORT data[]);

#define		flm_s_cmd(adr, cmd)						flm_s_cmd_rw((adr), (cmd), 0, NULL)
#define		flm_s_cmd_write(adr, cmd, n, dat)		flm_s_cmd_rw((adr), (cmd)|GD_SFLASH_WRITE, (n), (dat))
#define		flm_s_cmd_write1(adr, cmd, dat)			do {USHORT t = (dat); flm_s_cmd_write((adr), (cmd), 1, &t);} while (0)
#define		flm_s_cmd_write2(adr, cmd, dat1, dat2)	do {USHORT t[2] = {(dat1), (dat2)}; flm_s_cmd_write((adr), (cmd), 2, t);} while (0)
#define		flm_s_cmd_read(adr, cmd, n, dat)		flm_s_cmd_rw((adr), (cmd), (n), (dat))
#define		flm_s_cmd_read1(adr, cmd)				flm_s_cmd_read((adr), (cmd), 1, NULL)

#include	"llc_rom.h"
#include	"gerda3_vpyuv_dd.h"

#endif
