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

#define FRAME_ADD_BG FRAME_ADD_0
#define FRAME_ADD_VIDEO_0 FRAME_ADD_1

#define FRAME_ID_BG FRAME_ID_0
#define FRAME_ID_VIDEO_0 FRAME_ID_1

#define FRAME_BGCOLOR 0xff000000
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
#define YUV_HEIGHT_OUT 480


void main_task(VP_INT exinf)
{
	unsigned char i = 0;
	ULONG id_drw;
	
	define_frame(FRAME_ID_BG, FRAME_ADD_BG, WIDTH, GD_FMT_RGB565);
	define_frame(FRAME_ID_VIDEO_0, FRAME_ADD_VIDEO_0, YUV_WIDTH_IN, GD_FMT_YUV16);
	
	drw_set_frame(FRAME_ID_BG);
	drw_set_lcofs(0, 0);
	drw_set_clip(CLIP_XL, CLIP_YL, CLIP_XR, CLIP_YR);
	drw_set_color(GD_COLOR0, 0xff000000);
	drw_set_alpha(GD_ALPH_IGNORE, 0);
	drw_clear(CLIP_XL, CLIP_YL, CLIP_XR, CLIP_YR);
	
	drw_wait_drawend();
	
	dsp_layer_bgcolor(GD_VO_0, LAYER_BGCOLOR);
	dsp_sublayer_bgcolor(GD_VO_0, SUBLAYER_BGCOLOR);
	dsp_sublayer_win(GD_SUBLAYER00, WIDTH, HEIGHT, 0, 0, 0, 0);
	dsp_sublayer(GD_SUBLAYER00, FRAME_ID_BG, GD_LYR_DISP_ON);
	dsp_layer(GD_LAYER00, GD_RGB_LAYER0, GD_LYR_DISP_ON);
	
	dsp_layer(GD_LAYER01, GD_YUV_PATH0, GD_LYR_DISP_OFF);
	
	dsp_display(GD_VO_0, GD_ON);
	dsp_wait_vblank(GD_VO_0, 0);
	dsp_wait_vblank(GD_VO_0, 1);
	
	// video setting
	vi_control_video(GD_YUV_PATH0, FRAME_ID_VIDEO_0, GD_VI_STOP);
	vi_set_cap_mem_bank(GD_YUV_PATH0, 2, 241);    // chieu cao frame hien thi la 241 lines, chon 2 bank thi tu dong he thong se chia bank le tu 1,3,...241, bank chan tu 0,2,...240
	                                              // Xem luu y thu 6 muc 3.4.1.1 ham vi_set_cap_mem_bank(): vi dung vi_scaling_down nen bank height > YUV_HEIGHT_IN
	dsp_wait_vblank(GD_VO_0, 0);
	dsp_wait_vblank(GD_VO_0, 3);	// wait 3sync
	vi_set_cap_ip_conv(GD_YUV_PATH0, GD_VI_IPCV_FULL);
	vi_set_cap_win(GD_YUV_PATH0, GD_VI_NTSC, 746, 241, 94, 14);  // Xem luu y thu 4: 746 ko la boi so cua 4 ???
	vi_select_video(GD_YUV_PATH0, GD_VI_A_CVBS0, NULL);
	vi_scaling_down(GD_YUV_PATH0, GD_ON, YUV_WIDTH_IN, YUV_HEIGHT_IN); // frame size 400x160, hinh bat dc size 746x241 => scale down de bang voi frame size
	
	// input video signal
	vi_control_video(GD_YUV_PATH0, FRAME_ID_VIDEO_0, GD_VI_START);
	dsp_wait_vblank(GD_VO_0, 0);
	dsp_wait_vblank(GD_VO_0, 10);	// wait 10sync
	
	// display
	dsp_layer_win(GD_YUV_PATH0, YUV_WIDTH_IN, YUV_HEIGHT_IN, 0, 0, (WIDTH-YUV_WIDTH_OUT)>>1, (HEIGHT-YUV_HEIGHT_OUT)>>1); // WIDTH-YUV_WIDTH_OUT = 800 - 640 = 160 >> 1 = 80, HEIGHT-YUV_HEIGHT_OUT = 480 - 480 = 0
	dsp_layer_scaling(GD_YUV_PATH0, GD_ON, 0x800*YUV_WIDTH_IN/YUV_WIDTH_OUT, 0x800*YUV_HEIGHT_IN/YUV_HEIGHT_OUT); 
	/*                                     0x800*400/640 = 0x500             0x800*160/480 ~ 0x2AA   
	                                       (tang gap 1.6 lan)                (tang gap 3 lan)               */
	dsp_m_adapt_contrast(GD_YUV_PATH0, GD_ON, 64, 191, 8, 8, 125);
	dsp_m_adapt_block(GD_YUV_PATH0, GD_ON);
	dsp_layer(GD_LAYER01, GD_YUV_PATH0, GD_LYR_DISP_ON);
	
	dsp_wait_vblank(GD_VO_0, 0);
	dsp_wait_vblank(GD_VO_0, 1);
	
	while(1)  // trong ham while nay thi no lam gi ?
	{
		
	}

}