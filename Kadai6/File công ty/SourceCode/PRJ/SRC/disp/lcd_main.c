/***************************************************************
   クラリオン殿向け   
   プロジェクト名   ：'07年モデル開発
   ファイル名      ：lcd_main.c
   モジュール名   ：LCD(表示)
   機   能         ：LCDメイン処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
#define   _LCD_MAIN_C_

#include   "../model.h"

#include   "../comm/common.h"
#include   "../power/pw_func_ext.h"
#include   "../main/ma_ext.h"
#include   "../submode/sbm_ext.h"
#include   "../submode/sbm_def.h"
#include   "../io/IO_inc.h"
#include   "../tuner/TU_inc.h"
#include   "../audio/aud_ext.h"


#include   "lcd_main.h"
#include   "lcd_send.h"
#include   "lcd_mode.h"
#include   "lcd_ext.h"
#include   "lcd_sys.h"
#include   "lcd_font.h"


#include   <stdio.h>
#include   <string.h> 
#include   <stdlib.h>
/****************************************************************
      関数名：Lcd_main
      引  数：無し
      戻り値：無し
      機  能：メイン処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_main(void)
{
   if(flcd_send == ON)                     /* 送信要求があれば、送信   flcd_send = ON do trc do da dc khoi tao lan dau trong Lcd_ram_initial, which is in Lcd_initial  */
   {
      Lcd_send();    // trong file lcd_send.c
   }
   else if ((Pw_ACC_DET_get() == OFF)&&(Alm_mode_check()== OFF))  // Pw_ACC_DET_get trong file pw_func_ext.c, h瀘 tra ve gia tri cua fpw_acc.
   {
      if (flcd_accoff_send == TRUE)  // Xem h瀘 Clk_time_main() trong clock_mode.c, thoi gian luon dc cap nhat => flcd_accoff_send lu�n dc gui                                
      {                              // Trong folder 003, file excel DISP, muc 4, so 3. Dong "ACC_OFF chi gui den driver khi co yeu cau" la de cap den flag flcd_accoff_send
                                     // Sau khi co message CMSG_MAINCNT_OFFREQ(chuyen tu ACC_ON -> ACCOFF. ), no se goi ham Lcd_maincnt_offreq, khi nay flcd_accoff_send = TRUE tro lai 
         Lcd_ram_initial();                     /* 送信ﾊﾞｯﾌｧｸﾘｱ */
         if(Ma_model_dest() == 0x00 )
         {
            Lcd_area_disp();
         }
         else
         {
            Lcd_clock_mode();
         }
         Lcd_indictrol();                // trong file lcd_main.c. Xet xem Alarm co duoc enable ko(dung gio thi no se bao thuc), neu co indicator alarm se
                                         // duoc bat s疣g. No cung kiem tra ISR
         Lcd_edit();                     /* LCDｾｸﾞﾒﾝﾄ編集   Edit LCD segments  trong file lcd_send.c, tuc la thiet lap gia tri hien thi moi tren LCD */
         flcd_accoff_send = OFF;
      }
   }
   else if (lcd_edit_tim == 0) /* bien nay bi giam 1 don vi khi qua 10ms. Viec xet xem da qua 10ms hay chua dc thuc hien trong h瀘 Lcd_10ms_cnt trong 
                                  file lcd_ext.c. H瀘 Lcd_10ms_cnt, khi qua 10ms se duoc goi trong ham Ma_tim_int_1ms cua file ma_tim.c. H瀘 Ma_tim_int_1ms
                                  duoc goi khi xay ra ngat timer2 1ms*/
                                  
   {
      lcd_edit_tim = CLCD_ACCON_EDIT_TIM;               /* 50msﾀｲﾏ再設定  CLCD_ACCON_EDIT_TIM = 5, thiet lap lai timer 50ms */
      Lcd_ram_initial();                     /* 送信ﾊﾞｯﾌｧｸﾘｱ   clear buffer chua data gui di va khoi tao lai, sau 50ms chip lai gui cho LCD driver 24 byte data*/
      if (flcd_test_mode == OFF)
      {
         Lcd_mode();                        /* ﾓｰﾄﾞ分岐処�   Xu li phan nhanh che do�  trong file lcd_mode.c*/
         if(flcd_blank != ON)               /* DISP OFF中点滅、ｲﾝｼﾞｹｰﾀ、ｾｸﾞﾒﾝﾄ編集処理抜け    duoc gan lan dau trong h瀘 Lcd_send.*/
         {
            Lcd_indictrol();               /* ｲﾝｼﾞｹｰﾀ処理   trong file lcd_main.c. Xet xem Alarm co duoc enable ko(dung gio thi no se bao thuc), neu co indicator alarm se
                                              duoc bat s疣g. No cung kiem tra ISR*/
            Lcd_blink();                  /* 点滅処理   Xu li nhap nhay */
            Lcd_edit();                     /* LCDｾｸﾞﾒﾝﾄ編集   Chinh sua Lcd segment*/
         }
      }
      else
      {
         Lcd_test_edit();
      }
   }
   else
   {
      /*処理なし*/
   }
}
/****************************************************************
      関数名：Lcd_initial
      引  数：無し
      戻り値：無し
      機  能：表示ﾓｼﾞｭｰﾙの初期化
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_initial(void)
{
   lcd_tim_pwr_wait = 0xFF;   // timer counter cho phep hien thi
   lcd_edit_tim = CLCD_ACCON_EDIT_TIM;      /* 編集周期ﾀｲﾏ初期化   Khoi tao timer chu ki chinh sua, nam trong file lcd_main.h. CLCD_ACCON_EDIT_TIM = 5
                                             chu ki 50ms theo file excel*/
   flcd_test_mode   = OFF;
   Lcd_ram_initial();               /* 送信ﾊﾞｯﾌｧなどのｸﾘｱ */
}
/****************************************************************
      関数名：Lcd_ram_initial
      引  数：無し
      戻り値：無し
      機  能：RAMｸﾘｱ
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
・1/3デューティ
・キースキャン禁止(KSC:1)
・キースキャン許可(KSC:0)
****************************************************************/
void Lcd_ram_initial(void)
{
   /* 送信ﾊﾞｯﾌｧ初期化 */
   memset(lcd_send_buf.buf_byte,0,sizeof(lcd_send_buf.buf_byte));   /* 送信バッファ0ｸﾘｱ */
/*---------------------------------------------------------------------------*/
/*0 SP KC0 KC1*/
/*KC2 KSC K0 K1 P0 P1 P2 SC*/
/*DR DT FC0 FC1 FC2 OC 0 0*/
// Cau hinh control data gui den driver LCD
   lcd_send_buf.record.ctrl_1 = 0x00;   /* 0000ｺﾝﾄﾛｰﾙﾃﾞｰﾀ   0000 control data.   lcd_send_buf trong file lcd_send.h. ctrl1 chi gioi han 4 bit.    */
   #if 1
   if ((Pw_ACC_DET_get() == OFF)&&(Alm_mode_check() == FALSE)&&(Alm_beepon_get() == FALSE))
   #else
   //if (Pw_ACC_status_get() == OFF)
   if((Pw_ACC_status_get() == OFF)&&(Alm_mode_check() == FALSE))
   #endif
   {
      lcd_send_buf.record.ctrl_2 = 0x0B;   /* 11010000  キースキャン禁止  khong cho phep key scan */
   }
   else
   {
      lcd_send_buf.record.ctrl_2 = 0x09;   /* 10010000  キースキャン許可 cho phep key scan */
   }
   lcd_send_buf.record.ctrl_3 = 0x12;   /* 01001000 */      /* 1/3 Duty */
   
   /* Ta ko xet 2 bit DD o lan truyen thu 1 v� n� ban dau da duoc khoi tao bang 0, ta chi xet o lan truyen thu 2 va 3*/
   lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;   /* 2ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ  RECORD2 = 8, SENDBYTE = 8. Trong lan gui thu 2, sau khi gui 42 bit data,22 bits
                                                          cuoi la fixed data.
                                                          do bit thu 43->56 la 0 do memset buf_byte o tren. Do 2 bits DD cuoi la 01, 1 lan gui la 8bit
                                                          nen 8 bit cuoi(bit 56->64) se la 00000001. Mac du ta gan gia tri la 0x80(10000000), nhung do ta chon LSB first
                                                          nen gia tri gui di bat dau tu LSB, tuc la 00000001 */
   lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;   /* 3ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ    RECORD3 = 16.
                                                          Giai thich nhu tren, nhung 2 bits DD bay gio la 10 va la lan gui 64 bits thu 3.*/

   /* 文字ﾊﾞｯﾌｧｸﾘｱ  clear character buffer*/
   memset(lcd_seg_buf,0,sizeof(lcd_seg_buf));  // lcd_seg_buf trong file lcd_mode.h, mang lcd_seg_buf chua 5 phan tu kieu byte
   /* 点滅ﾓｰﾄﾞｸﾘｱ  clear mode blink*/
   lcd_blk_mode = 0;                           /* 点滅ﾓｰﾄﾞｸﾘｱ,時計調整中ｸﾘｱしない   Blink mode clear, ko clear trong khi dieu chinh dong ho */
   
   flcd_send = ON; // lcd_send flag = ON
}
/****************************************************************
      関数名：Lcd_pwr_on
      引  数：無し
      戻り値：無し
      機  能：LCDドライバーPwrOn(ACC ON)処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2006/3/13      0.1      徐      新規作成
****************************************************************/
void Lcd_driver_send( void )
{
   /* LCDﾄﾞﾗｲﾊﾞ送信 */
   Lcd_ram_initial();
   flcd_blank = ON;                           /* ﾌﾞﾗﾝｸ送信 */
   Lcd_send();
   
}

/****************************************************************
      関数名：Lcd_indictrol
      引  数：無し
      戻り値：無し
      機  能：インジケータ設定処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_indictrol( void )
{
   BYTE ind;
   
   #if 0
   if ((Clk_adj_check() == ON)||(lcd_base_mode == OFF )||(Pw_Power_status_get() != ON))
   {
      ind = Clk_indicator_set();
      if((ind & CLCD_CL_IND_AM) != 0 )      /* "AM" */
      {
         lcd_send_buf.record1.am = ON;
      }
      if((ind & CLCD_CL_IND_PM) != 0 )      /* "PM" */
      {
         lcd_send_buf.record1.pm = ON;
      }
   }
   else if (Alm_mode_check() == ON)
   {
      ind = Alm_indicator_set();
      if((ind & CLCD_CL_IND_AM) != 0 )      /* "AM" */
      {
         lcd_send_buf.record1.am = ON;
      }
      if((ind & CLCD_CL_IND_PM) != 0 )      /* "PM" */
      {
         lcd_send_buf.record1.pm = ON;
      }
   }
   else if((Sbm_mode_check() != CSBM_VOL )&&(Sbm_mode_check() != CSBM_SEF ))
   {
      BYTE ma_mode;
      /* 主なﾓｰﾄﾞｲﾝｼﾞｹｰﾀ */
      ma_mode = Ma_current_mode();
      switch(ma_mode)
      {
         /* TUNERモード */
         case CMA_TUNER:
            ind = Tu_indicator_set();
            if((ind & CLCD_TU_IND_FM) != 0)         /* "FM"   */
            {
               lcd_send_buf.record1.fm = ON;
            }
            if((ind & CLCD_TU_IND_AM) != 0)         /* "AM"   */
            {
               lcd_send_buf.record1.am = ON;
            }
            if((ind & CLCD_TU_IND_ONE) != 0)         /* "1"     */
            {
               lcd_send_buf.record1.one = ON;
            }
            if((ind & CLCD_TU_IND_TWO) != 0)         /* "2"     */
            {
               lcd_send_buf.record1.two = ON;
            }
            if((ind & CLCD_TU_IND_ST) != 0)         /* "ST"    */
            {
               lcd_send_buf.record1.st = ON;
            }
            #if 0
            if((ind & CLCD_TU_IND_S1) != 0)         /* "1"    */
            {
               lcd_send_buf.record1.s1 = ON;
            }
            #endif
            break;
            
         case CMA_ISR:
            ind = Tu_isr_indicator_set();
            if((ind & CLCD_TU_IND_AM) != 0)         /* "AM"   */
            {
               lcd_send_buf.record1.am = ON;
            }
            #if 0
            if((ind & CLCD_TU_IND_TI) != 0)         /* "・)))"   */
            {
               lcd_send_buf.record1.ti = ON;
            }
            if((ind & CLCD_TU_IND_S1) != 0)            /* "1"    */
            {
               lcd_send_buf.record1.s1 = ON;
            }
            #endif
            break;
            
         case CMA_AUX:   /* AUXはない */
         default:
            break;
      }
   }
   else
   {
      ;
   }
   
   #endif
   
   if(Alm_bell_indicator_set() == ON )      /* 「アラーム記号」    trong file alm_mode, kiem tra Alarm co duoc bat/enable chua?*/
   {
      lcd_send_buf.record1.al = ON;
   }
   if(Ma_current_mode() == CMA_ISR )      /* 「アラーム記号」 */
   {
      lcd_send_buf.record1.ti = ON;
   }
   
}
/****************************************************************
      関数名：Lcd_blink
      引  数：無し
      戻り値：無し
      機  能：文字点滅処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/30      0.1      ha      新規作成
****************************************************************/
void Lcd_blink(void)
{
   if( lcd_blk_mode == OFF ) 
   {
      lcd_tim_blk = 0;                     /* 点滅ﾀｲﾏ解除 */
      return;
   }
   
   switch(lcd_blk_type)
   {
      case BLK_500MS:                              /* 500ms周期   500 ms period. 500ms s疣g, 500ms tat */
         if(lcd_tim_blk == 0)
         {
            lcd_tim_blk = CLCD_TIMECNT_1S;            /* ｽﾀｰﾄﾀｲﾏ再設定  Thiet lap lai Start timer  */
            flcd_blk_clr = OFF;
         }
         else if(lcd_tim_blk >= CLCD_TIMECNT_500MS)
         {
            return;
         }
         else
         {
            ;
         }
         break;
      case BLK_1S:                              /* 1s周期  1s period, 1s s疣g 1s tat*/
         if(lcd_tim_blk == 0)
         {
            lcd_tim_blk = CLCD_TIMECNT_2S;            /* ｽﾀｰﾄﾀｲﾏ再設定 */
            flcd_blk_clr = OFF;
         }
         else if(lcd_tim_blk >= CLCD_TIMECNT_1S)
         {
            return;
         }
         else
         {
            ;
         }
         break;
      default:
         break;
   }
      
   if(((lcd_blk_type == BLK_1S) && (lcd_tim_blk < CLCD_TIMECNT_1S))
   || ((lcd_blk_type == BLK_500MS) && (lcd_tim_blk < CLCD_TIMECNT_500MS)))
   {
      if(flcd_blk_clr == OFF)                  /* 調整中消灯させない */
      {
         switch(lcd_blk_mode)
         {
            case CLCD_BLK_FREQ:
               lcd_send_buf.record1.s1 = OFF;
               lcd_send_buf.record1.dot = OFF;
               memset(&lcd_seg_buf[0],0,5);
               break;
            
            case CLCD_BLK_CLOCK:
               lcd_send_buf.record1.colon = OFF;
               lcd_send_buf.record1.am = OFF;
               lcd_send_buf.record1.pm = OFF;
               memset(&lcd_seg_buf[0],0,4);
               break;
            
            case CLCD_BLK_ALARM:
               lcd_send_buf.record1.al = OFF;
               lcd_send_buf.record1.colon = OFF;
               lcd_send_buf.record1.am = OFF;
               lcd_send_buf.record1.pm = OFF;
               memset(&lcd_seg_buf[0],0,4);
               break;
            case CLCD_BLK_PSNO:   
               memset(&lcd_seg_buf[4],0,1);
               break;
            
            default:
               break;
         }
      }
   }
}
/****************************************************************
      関数名：Lcd_blink_set
      引  数：無し
      戻り値：無し
      機  能：文字点滅設定
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2006/3/13      0.1      徐      新規作成
****************************************************************/
void Lcd_blink_set(BYTE blkmode, BYTE blktype) // duoc su dung trong file lcd_mode.c
{
   lcd_blk_mode = blkmode;         /* 点滅ﾓｰﾄﾞ   Blinking mode, duoc su dung trong ham Lcd_blink()*/
   lcd_blk_type = blktype;         /* 点滅周期   Blinking cycle duoc su dung trong ham Lcd_blink()*/
}
/****************************************************************
      関数名：Lcd_25ms_cnt
      引  数：無し
      戻り値：無し
      機  能：25msﾀｲﾏｶｳﾝﾄ
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_25ms_cnt(void)
{
   /* MAIN_CNT待ちﾀｲﾏ */
   if((lcd_tim_pwr_wait > TIMER_OUT) && (lcd_tim_pwr_wait != 0xFF))
   {
      lcd_tim_pwr_wait--;
   }
}

/****************************************************************
      関数名：Lcd_50ms_cnt
      引  数：無し
      戻り値：無し
      機  能：50msﾀｲﾏｶｳﾝﾄ
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_50ms_cnt(void)
{
   /* 点滅ﾀｲﾏｶｳﾝﾄ */
   if(lcd_tim_blk > TIMER_OUT)
   {
      lcd_tim_blk--;
   }
   /* 一時消灯ﾀｲﾏｶｳﾝﾄ */
   if(lcd_tim_1shot_blk > TIMER_OUT)
   {
      lcd_tim_1shot_blk--;
      if(lcd_tim_1shot_blk == TIMER_OUT)
      {
         lcd_1shot_blk = OFF;
      }
   }
}
/****************************************************************
      関数名：Lcd_100ms_cnt
      引  数：無し
      戻り値：無し
      機  能：100msﾀｲﾏｶｳﾝﾄ
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_100ms_cnt(void)
{
   /* ｿｰｽﾁｪﾝｼﾞ一時表示ﾀｲﾏｶｳﾝﾄ Source change temporary display timer counter*/ 
   if(lcd_src_tim > TIMER_OUT)                     /* 2秒表示ﾀｲﾏ */
   {
      lcd_src_tim--;
      if(lcd_src_tim == TIMER_OUT)
      {
         lcd_src_mode = OFF;
         Lcd_disp_1shot_base(Ma_current_mode());      /* 2秒ﾍﾞｰｽ表示 */
      }
   }
   /* 一時表示ﾀｲﾏｶｳﾝﾄ   Temporary display timer counter*/
   if(lcd_tim_1shot > TIMER_OUT)
   {
      lcd_tim_1shot--;
      if(lcd_tim_1shot == TIMER_OUT)
      {
            lcd_1shot_mode = OFF;
      }
   }
   /* ベース表示ﾀｲﾏｶｳﾝﾄ Base display timer counter */
   if (Ma_get_clock() == TRUE )
   {
      if(lcd_tim_base > TIMER_OUT)
      {
         lcd_tim_base--;
         if(lcd_tim_base == TIMER_OUT)
         {
            lcd_base_mode = OFF;
         }
      }
   }
}


/*------------lcd_mode.cから移動----------------------*/
/****************************************************************
      関数名：lcd_moji_romcpy
      引  数：無し
      戻り値：無し
      機  能：HEXtoASCII処理                     xu li HEXto ASCII
            ASCIIﾃﾞｰﾀ入れるRAMは2バイト       Ram dua vao data ASCII la 2 bytes
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void lcd_moji_romcpy(BYTE *savebuf, BYTE *addfrom)
{
   const BYTE *add;
   
   add =  (const BYTE *)addfrom;
   *savebuf =  *add;
   *(savebuf+1) = *(add+1);
   *(savebuf+2) = *(add+2);
   *(savebuf+3) = *(add+3);
   *(savebuf+4) = *(add+4);
   *(savebuf+5) = *(add+5); 
   /* Xem file lcd_tbl.h, ta c� CLCD_SRC_TBL[][6], addfrom se la column cua CLCD_SRC_TBL, add se la ki tu hien thi tren LCD. Ta co tong cong 6 ki tu*/
}

/****************************************************************
      関数名：lcd_hextoascii
      引  数：無し
      戻り値：無し
      機  能：HEXtoASCII処理
            ASCIIﾃﾞｰﾀ入れるRAMは2バイト
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_hextoascii( BYTE hex, BYTE *ascii )
{
   *ascii = ( hex/10 ) + 0x30;
   *(ascii+1) = ( hex%10 ) +0x30;
}
/****************************************************************
      関数名：lcd_nodisp
      引  数：dataﾎﾟｲﾝﾀ、dataのsize
      戻り値：無し
      機  能：0の場合、表示しない
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_nodisp( BYTE *data, BYTE size )
{
   // Ham nay se khien cac k� tu '0' cua cac seg khong hien thi.
   // Ta gia su: LCD hien thi:
   // TH1:
   // seg1 seg2 seg3   =>  seg1  seg2  seg3
   // 0    0    1                      1                
   // TH2:
   // seg1 seg2 seg3   =>  seg1  seg2  seg3   (giu nguyen '0')
   // 1    0    1          1     0      1    
   if( *data == 0x30 )  // neu ki tu hien ra la '0' (HEX:0x30) thi se la NULL(HEX: 0x00)
   {
      *data = 0;
   }
   data ++; // dich chuyen con tro len, xet tiep seg tiep theo
   size --; 
   while(size)
   {
      if( *(data - 1) == 0)  // kiem tra xem phan tu trc do co bang NULL chua.                             
      {
         if( *data == 0x30 )
         {
            *data = 0;
         }
      }
      data ++;
      size --;
   }
}
#undef   _LCD_MAIN_C_
