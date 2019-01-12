#include <stdio.h>

#include <itron.h>
#include <kernel.h>
#include <kernel_id.h>

#include "gerda3_dd.h"	// Gerda API defined
#include "pf_frmdef.h"	// frame size and address
#include "texdata.h"	// texture size and sorce type

#define FRAME_ADD_0 (FRAME1_ADDR)
#define FRAME_ADD_1 (FRAME_ADD_0 + (WIDTH * HEIGHT) * 2)	// RGB565 frame: 16bit/pixel(2byte/pixel)
#define FRAME_ADD_2 (FRAME_ADD_1 + (WIDTH * HEIGHT) * 2)
#define FRAME_ADD_3 (FRAME_ADD_2 + (WIDTH * HEIGHT) * 2)
#define FRAME_ADD_4 (FRAME_ADD_3 + (WIDTH * HEIGHT) * 2)
#define FRAME_ADD_5 (FRAME_ADD_4 + (WIDTH * HEIGHT) * 2)
#define FRAME_ADD_6 (FRAME_ADD_5 + (WIDTH * HEIGHT) * 2)
#define FRAME_ADD_7 (FRAME_ADD_6 + (WIDTH * HEIGHT) * 2)
#define FRAME_ADD_8 (FRAME_ADD_7 + (WIDTH * HEIGHT) * 2)

#define FRAME_ID_0 0
#define FRAME_ID_1 1
#define FRAME_ID_2 2
#define FRAME_ID_3 3
#define FRAME_ID_4 4
#define FRAME_ID_5 5
#define FRAME_ID_6 6
#define FRAME_ID_7 7
#define FRAME_ID_8 8

#define FRAME_ID_DRAW_00 FRAME_ID_0
#define FRAME_ID_DRAW_01 FRAME_ID_1

#define FRAME_BGCOLOR 0xffff00ff
//#define FRAME_TCOLOR 0xfffffbff
#define FRAME_TCOLOR 0xffff00ff
#define LAYER_BGCOLOR 0x3ff003ff
#define SUBLAYER_BGCOLOR 0

#define CLIP_XL 0
#define CLIP_YL 0
#define CLIP_XR WIDTH-1
#define CLIP_YR HEIGHT-1

#define YUV_WIDTH_IN 400
#define YUV_HEIGHT_IN 160
#define YUV_WIDTH_OUT 640
#define YUV_HEIGHT_OUT 360


void main_task(VP_INT exinf)
{
	unsigned char i = 0;
	ULONG id_drw;
	POS pos1[4][3] = {{100, 100, 0xff00}, {120, 200, 0xf0f0}, {200, 100, 0xf00f}, {220, 160, 0xf000}};
	POS pos2[4][5] = {{400, 200, 0<<4, 0<<4, 0xffff}, {420, 300, 0, (GTTX_NEKO_HEIGHT-1)<<4, 0xffff},{500, 160, (GTTX_NEKO_WIDTH-1)<<4, 0<<4, 0xffff}, {600, 320, (GTTX_NEKO_WIDTH-1)<<4, (GTTX_NEKO_HEIGHT-1)<<4, 0xffff}};
	
	define_frame(FRAME_ID_DRAW_00, FRAME_ADD_0, WIDTH, GD_FMT_RGB565);
	define_frame(FRAME_ID_DRAW_01, FRAME_ADD_1, WIDTH, GD_FMT_RGB565);
	
	drw_set_frame(FRAME_ID_DRAW_01);
	drw_set_lcofs(0, 0);
	drw_set_clip(CLIP_XL, CLIP_YL, CLIP_XR, CLIP_YR);
	drw_set_color(GD_COLOR0, 0xff000000);
	drw_set_alpha(GD_ALPH_IGNORE, 0);
	drw_clear(CLIP_XL, CLIP_YL, CLIP_XR, CLIP_YR);
	
	drw_wait_drawend();
	
	dsp_layer_bgcolor(GD_VO_0, LAYER_BGCOLOR);
	dsp_sublayer_bgcolor(GD_VO_0, SUBLAYER_BGCOLOR);
	dsp_sublayer_win(GD_SUBLAYER00, WIDTH, HEIGHT, 0, 0, 0, 0);
	dsp_sublayer(GD_SUBLAYER00, FRAME_ID_DRAW_01, GD_LYR_DISP_ON);
	dsp_layer(GD_LAYER00, GD_RGB_LAYER0, GD_LYR_DISP_ON);
	
	dsp_display(GD_VO_0, GD_ON);
	dsp_wait_vblank(GD_VO_0, 0);
	dsp_wait_vblank(GD_VO_0, 1);
	
	
	while(1)
	{
		if((i&0x01) == 0)
			id_drw = FRAME_ID_DRAW_00;
		else
			id_drw = FRAME_ID_DRAW_01;
		
		// draw triangles
		drw_set_frame(id_drw);
		drw_set_lcofs(0, 0);
		drw_set_clip(CLIP_XL, CLIP_YL, CLIP_XR, CLIP_YR);
		drw_set_color(GD_COLOR0, FRAME_BGCOLOR);
		drw_set_alpha(GD_ALPH_IGNORE, 0);
		drw_clear(CLIP_XL, CLIP_YL, CLIP_XR, CLIP_YR);
		drw_set_lcofs(i, 0);
		drw_set_textmode(GD_MSK_ALL, GD_TXT_REFER_OFF | GD_TXT_SHADE | GD_TXT_ANTIALIAS | GD_TXT_ARRAY_STRIP | GD_ART_XYC | GD_ART_ARGB4444);
		drw_set_drawmode(GD_OUT_FRM);
		drw_triangles(4, pos1);
		drw_set_dsource(GTTX_NEKO_TEXADR, GTTX_NEKO_WIDTH, GTTX_NEKO_TYPE);
		drw_set_textmode(GD_MSK_ALL, GD_TXT_REFER_CLAMP | GD_TXT_BILINEAR | GD_TXT_ANTIALIAS | GD_TXT_ARRAY_STRIP | GD_ART_XYUVZ);   // co U,V ko the co C
		drw_set_texture(0, 0, GTTX_NEKO_WIDTH, GTTX_NEKO_HEIGHT);
		drw_set_alpha(GD_ALPH_IGNORE, 0);
		drw_set_drawmode(GD_OUT_FRM);
		drw_triangles(4, pos2);
		
		drw_wait_drawend();
		
		dsp_sublayer_win(GD_SUBLAYER00, WIDTH, HEIGHT, 0, 0, 0, 0);
		dsp_sublayer(GD_SUBLAYER00, id_drw, GD_LYR_DISP_ON);
		
		dsp_wait_vblank(GD_VO_0, 0);
		dsp_wait_vblank(GD_VO_0, 1);
		
		i++;
		if(i >= 60)
			i = 0;
	}
}