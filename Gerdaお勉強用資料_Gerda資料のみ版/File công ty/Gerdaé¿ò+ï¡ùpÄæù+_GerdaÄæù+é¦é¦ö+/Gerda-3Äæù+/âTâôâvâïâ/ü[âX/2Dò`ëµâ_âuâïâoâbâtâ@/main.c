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

#define FRAME_ID_DRAW_0 FRAME_ID_0
#define FRAME_ID_DRAW_1 FRAME_ID_1

#define FRAME_BGCOLOR 0xff000000    // mau den
#define LAYER_BGCOLOR 0xff000000    
#define SUBLAYER_BGCOLOR 0xff000000

#define CLIP_XL 0
#define CLIP_YL 0
#define CLIP_XR WIDTH-1           // WIDTH  = 800 
#define CLIP_YR HEIGHT-1          // HEIGHT = 480

#define SQU_COLOR 0xff0000ff    // mau xanh duong

void main_task(VP_INT exinf)
{
	unsigned char i = 0;
	ULONG id_drw;
	
	define_frame(FRAME_ID_DRAW_0, FRAME_ADD_0, WIDTH, GD_FMT_RGB565); // Xem muc 3.2.6.2 drw_set_frame(), o luu y thu 3 noi len moi lien quan giua frame type va Width.
	define_frame(FRAME_ID_DRAW_1, FRAME_ADD_1, WIDTH, GD_FMT_RGB565); // O day type 16 bit, Width = 800 => 800 la boi so cua 16 (800/16 = 50)
	
	while(1)
	{
		if((i%2) == 0)
			id_drw = FRAME_ID_DRAW_0;
		else
			id_drw = FRAME_ID_DRAW_1;
		
		drw_set_frame(id_drw);
		drw_set_lcofs(0, 0);
		drw_set_clip(CLIP_XL, CLIP_YL, CLIP_XR, CLIP_YR);
		drw_set_color(GD_COLOR0, FRAME_BGCOLOR);     
		drw_set_alpha(GD_ALPH_IGNORE, 0); // ???
		drw_clear(CLIP_XL, CLIP_YL, CLIP_XR, CLIP_YR);  // tao frame hnh chu nhat co mau den 
		
		drw_set_dsource(GTTX_TOPMENU_TEXADR, GTTX_TOPMENU_WIDTH, GTTX_TOPMENU_TYPE); // truoc khi muon lay texture, ta phai tro dia chi cua vùng source chua texture do
		drw_set_texture(0, 0, GTTX_TOPMENU_WIDTH, GTTX_TOPMENU_HEIGHT);
		drw_set_alpha(GD_ALPH_IGNORE, 0);
		drw_set_drawmode(GD_OUT_FRM);
		drw_set_textmode(GD_MSK_ALL, GD_TXT_SHADE);
		drw_prect(CLIP_XL, CLIP_YL, 0<<4, 0<<4, CLIP_XR, CLIP_YR, (GTTX_TOPMENU_WIDTH-1)<<4, (GTTX_TOPMENU_HEIGHT-1)<<4); // vi day la HCN dc tham chieu nen ko can goi hàm API drw_set_color truoc.
		                                                                                                                  // (GTTX_TOPMENU_WIDTH-1)<<4 co the do ben trong hàm drw_prect, no lay 4 bit cuoi lam so nhi phan. 
																														  // VD: GTTX_TOPMENU_WIDTH-1 = 4095 = 0000111111111111, neu ko dich trai 4 bit thì ta co vi tri la 000011111111 thay vi 111111111111.
		        /*  0         0      0     0      799       479           799<<4                    479<<4         */ 
		drw_set_color(GD_COLOR0, SQU_COLOR);   // set mau cho HCN dinh ve la mau xanh
		drw_set_alpha(GD_ALPH_IGNORE, 0);
		drw_set_drawmode(GD_OUT_FRM);
		drw_rect(0, 210, 100+i*7+50, 270);
		
		drw_wait_drawend();
		
		// display
//		dsp_display(GD_VO_0, GD_OFF);
//		dsp_layer(GD_LAYER00, GD_RGB_LAYER0, GD_LYR_DISP_OFF);
		dsp_sublayer(GD_SUBLAYER00, 0, GD_LYR_DISP_OFF);
		
		dsp_layer_bgcolor(GD_VO_0, LAYER_BGCOLOR);
		dsp_sublayer_bgcolor(GD_VO_0, SUBLAYER_BGCOLOR);
		
		dsp_sublayer_win(GD_SUBLAYER00, WIDTH, HEIGHT, 0, 0, 0, 0);  // Tai sao tham so thu 2 va 3 bang chieu rong va chieu cao cua frame. Xem hàm 3.3.2.15
		dsp_sublayer(GD_SUBLAYER00, id_drw, GD_LYR_DISP_ON);
		dsp_layer(GD_LAYER00, GD_RGB_LAYER0, GD_LYR_DISP_ON);
		
		dsp_display(GD_VO_0, GD_ON);	//@‰Šú‰»‚Ì‚Æ‚«‚É1‰ñON
		
		dsp_wait_vblank(GD_VO_0, 0);
		dsp_wait_vblank(GD_VO_0, 1);
		
		i++;
		if(i >= 60)
			i = 0;
	}
}