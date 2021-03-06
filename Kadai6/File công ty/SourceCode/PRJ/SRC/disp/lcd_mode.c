/***************************************************************
   クラリオン殿向け       
   プロジェクト名   ：'12年モデル
   ファイル名      ：lcd_mode.c
   モジュール名   ：LCD(表示)
   機   能         ：ﾓｰﾄﾞ分岐処理関数ファイル
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
#define      _LCD_MODE_C_

#include    <stdio.h>
#include   <string.h>
#include   <stdlib.h>

#include   "../model.h"

#include   "../comm/common.h"
#include   "../audio/aud_ext.h"
#include   "../audio/aud_dev_func.h"
#include   "../main/ma_ext.h"
#include   "../tuner/TU_inc.h"
#include   "../submode/SBM_inc.h"
#include   "../testmode/TEST_inc.h"
#include   "../power/pw_def.h"
#include   "../power/pw_func_ext.h"   /* QAC対策 */
#include   "lcd_main.h"
#include   "lcd_ext.h"
#include   "lcd_mode.h"
#include   "lcd_send.h"
#include   "lcd_sys.h"
#include   "lcd_tbl.h"


/* ローカル関数 */
static void Lcd_main_mode(void);
static void Lcd_off_mode(void);
static void Lcd_snd_mode(void);
static void Lcd_1shotdisp_mode(void);
static void Lcd_tu_base(TDP_TUNER *);
static void Lcd_tu_base_frq(BYTE band,BYTE bandno,BYTE *freq,BYTE preno);
static void Lcd_tu_indicator(void );
static void Lcd_st_indicator(void );
static void Lcd_clkalm_indicator(void);
static void   Lcd_blank_disp(void);

static void Lcd_tuner_mode(void);

static void Lcd_aux_mode(void);
static void Lcd_vol_mode(void);
static void Lcd_isr_mode(void);
static void Lcd_model_version(void);
/****************************************************************
      関 数 名：Lcd_mode
      引    数：無し
      戻 り 値：無し
      機    能：ﾓｰﾄﾞ分岐処理
      コメント：表示優先度の順
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
void Lcd_mode(void)
{ 
   /* Cach kiem tra de hien thi mode se theo thu tu sau: 
      1. Check xem code area dung hay ko
      2. Check ACCOFF 
      3. (ACCON)Check POWERON OFF
      4. (ACCON v� POWERON)Check Version
      5. (ACCON v� POWERON)Check Submode
      6. Check Main Mode*/
   if(Ma_model_dest() == 0x00 )  // trong file ma_ext.c, h瀘 tra ve gia tri bien ma_dest. 0x00 tuc la AERA bi error
   {
      Lcd_area_disp();   // trong file lcd_mode.c, thiet lap lcd_seg_buf[0] => [3] l� "AREA", lcd_seg_buf[4] = NULL. 
   }
   #if 0 
   else if (Pw_ACC_DET_get() == OFF)  // trong file pw_func_ext.c , tra ve gia tri fpw_acc. Neu fpw_acc = ON thi tra ve ON, nguoc lai OFF.
   {
      Lcd_off_mode();               /* PWR OFF    trong file lcd_mode.c*/
   }
   #endif
   else if(Pw_PowerOff_disp_chk() == TRUE) //  trong file pw_func_ext.c, Xet dang PWRON hay OFF, h瀘 tra ve FALSE neu pw_curmode va pw_tarmode deu bang CPW_PWRON. 
   {
      Lcd_off_mode();               /* PWR OFF.  Trong khi PWROFF thi LCD se chi hien thi thoi gian thoi. Neu thoi gian bi error thi ko hien thi gi het*/
   }
   /* Tu else if phia duoi la danh cho POWERON (ACCON)*/
   else if (Sct_mode_check() == CSBM_SCT_VER) //Sct_mode_check trong file sbm_ext.c. CSBM_SCT_VER=1 trong file sbm_def.h
   {
      Lcd_model_version();     /* LCD se hien thi "U-162"*/
   }
   #if 0   /* 未使用 */
   else if((lcd_src_mode > 0)&&(lcd_src_mode < CLCD_SRC_MAX))
   {
      Lcd_srcchg_mode();            /* ｿｰｽ遷移一時表示 */
   }
   #endif
   else if((Sbm_mode_check() != CSBM_OFF)&&(Sbm_mode_check() !=CSBM_TES))  // check xem co dang o submode khong
   {
      Lcd_st_indicator();      // Ham kiem tra xem co dang trong mode tuner FM ko, neu la FM check xem station FM do co dich vu phat am thanh stereo ko,
                               // neu co thi indicator "st" se s疣g
      if(Vol_mode_check() != OFF)
      {
         Lcd_vol_mode();   // Hien thi so luong VOLUMN dc dieu chinh. VD lcd hien thi tu seg1 -> seg5(ko tinh colon) la:
                           // TH1: vol = 22 => "UO:22 "   TH2: vol = 09 => "UO: 9 " (ko hien thi ki tu '0')
      }
      else if(Sef_mode_check() !=  OFF)
      {
         Lcd_snd_mode();               /* SOUNDﾓｰﾄﾞ 
                                          VD hien thi BAS: 
                                             TH1: "BA:-2 "  TH2: "BA:+2 "  TH3: "BA: 0 "
                                          VD hien thi TRE: 
                                             TH1: "TR:-2 "  TH2: "TR:+2 "  TH3: "TR: 0 "
                                          VD hien thi BAL: 
                                             TH1: "BL:   "  TH2: "BL: R "  TH3: "BA: L "*/
      }
      else if((Clk_adj_check() == ON)||(Alm_mode_check() == ON)||(Alm_beepon_get() == TRUE))
      {
         Lcd_clock_mode();  // Hien thi gia tri dong ho
      }
      else
      {
         ;
      }
   }
   else
   {
      Lcd_st_indicator();
      Lcd_main_mode();            /* 各ﾒｲﾝﾓｰﾄﾞ処�  danh cho main mode�
                                   Khong tinh truong hop hien thi CLOCK khi qua 5s
                                      TUNER mode: FM hien thi tu seg1->4 va dot,  AM hien thi tu seg1->3
                                      AUX mode: Hien thi chuoi "AU   "
                                      ISR mode: Hien tan so*/
   }
}
/****************************************************************
      関数名：Lcd_main_mode
      引  数：無し
      戻り値：無し
      機  能：各ﾒｲﾝﾓｰﾄﾞ表示処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
static void Lcd_main_mode(void)
{
   BYTE ma_mode;
   ma_mode = Ma_current_mode();
   switch(ma_mode)
   {
      case CMA_TUNER:
         Lcd_tuner_mode();                  /* TUNERﾓｰﾄﾞ */
         break;
      
      case CMA_AUX:
         Lcd_aux_mode();                     /* AUXﾓｰﾄﾞ　 */
         break;
         
      case CMA_ISR:                        /* ISRモード */
         Lcd_isr_mode();
         break;
      
      default:
         break;
   }
}
/****************************************************************
      関数名：Lcd_off_mode
      引  数：無し
      戻り値：無し
      機  能：OFFﾓｰﾄﾞ表示処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
static void Lcd_off_mode(void)
{
   #if 1
   Lcd_clock_mode();         /* Clock display in OFF mode */
   #else
   if (Ma_get_clock() == TRUE)
   {
      Lcd_clock_mode();         /* OFFﾓｰﾄﾞ中時計表示 */
   }
   else
   {
      Lcd_blank_disp();
   }
   #endif
}

#if 0   /* 未使用 */
/****************************************************************
      関数名：Lcd_srcchg_mode
      引  数：無し
      戻り値：無し
      機  能：ｿｰｽ遷移一時表示処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
static void Lcd_srcchg_mode(void)
{
   //lcd_8moji_romcpy(lcd_seg_buf,(BYTE *)CLCD_SRC_TBL[lcd_src_mode-1]);
   lcd_moji_romcpy(lcd_seg_buf,(BYTE *)CLCD_SRC_TBL[lcd_src_mode-1]);
}
#endif

/****************************************************************
      関数名：Lcd_vol_mode
      引  数：無し
      戻り値：無し
      機  能：Vol調整表示処理             Vol adjustment display processing
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
static void Lcd_vol_mode(void)
{
   BYTE   value;
   
   value = Vol_lcddata_set();  // sbm_ext.c
   lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[VOL]);  //CLCD_SEL_TBL trong file lcd_tbl.h, VOL=0 trong file lcd_tbl.h. 
                                                             // 5 seg cua LCD se hien thi "Uo   "
   lcd_send_buf.record1.colon = ON;
   Lcd_hextoascii(value,&lcd_seg_buf[SELVOL]);      /* HEXtoASCII    SELVOL = 2*/
   Lcd_nodisp(&lcd_seg_buf[SELVOL],1);                  /* 上位が0なら、表示しない  ko hien thi k� tu '0' neu gia tri vol chi co o hang don vi*/
   
}
/****************************************************************
      関数名：Lcd_snd_mode
      引  数：無し
      戻り値：無し
      機  能：Audio調整表示処理     Audio adjustment display processing
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
static void Lcd_snd_mode(void)
{
   TDP_SEL seldisp;   /* TDP_SEL trong file lcd_ext.h gom 2 gia tri value va mode*/
   BYTE value;
   
   memset( &seldisp, 0, sizeof(seldisp));         /* ｸﾘｱ */
   Sef_lcddata_set( &seldisp );               /* AUDIOﾃﾞｰﾀ情報取得  trong  sef_mode.c. Set value cho seldisp*/
   lcd_send_buf.record1.colon = ON;
   switch(seldisp.mode)
   {
      /*seldisp.value(BAS,TRE,BAL) max = 10 min = 0  (Xem aud_ext.h) 
           SELBAS = SELTRE = SELBAL = 2
           lcd_seg_buf[3] chua ki tu '0' -> '4'
           lcd_seg_buf[2] chua ki tu '-' hoac '+' */
      case CSBM_AUD_BAS:
         lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAS]);
         if(seldisp.value < CAUD_BAS_CNT)    // CAUD_BAS_CNT = 5. 
         {
            value = (CAUD_BAS_CNT - seldisp.value);
            Lcd_hextoascii(value,&lcd_seg_buf[SELBAS]);      /* HEXtoASCII   SELBAS = 2. trong file lcd_mode.h. */
            Lcd_nodisp(&lcd_seg_buf[SELBAS],1);            /* 上位が0なら、表示しない */
            lcd_seg_buf[SELBAS] = '-';                     /* "-" */
         }
         else if(seldisp.value > CAUD_BAS_CNT)
         {
            value = (seldisp.value - CAUD_BAS_CNT);
            Lcd_hextoascii(value,&lcd_seg_buf[SELBAS]);      /* HEXtoASCII  */
            Lcd_nodisp(&lcd_seg_buf[SELBAS],1);            /* 上位が0なら、表示しない */
            lcd_seg_buf[SELBAS] = '+';
         }
         else
         {
            lcd_seg_buf[SELBAS+1] = '0';
         }
         break;
      case CSBM_AUD_TRE:
         lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[TRE]);
         if(seldisp.value < CAUD_TRE_CNT)  // CAUD_TRE_CNT = 5
         {
            value = (CAUD_TRE_CNT - seldisp.value);
            Lcd_hextoascii(value,&lcd_seg_buf[SELTRE]);      /* HEXtoASCII */
            Lcd_nodisp(&lcd_seg_buf[SELTRE],1);            /* 上位が0なら、表示しない */
            lcd_seg_buf[SELTRE] = '-';                     /* "-" */
            
         }
         else if(seldisp.value > CAUD_TRE_CNT)
         {
            value = (seldisp.value - CAUD_TRE_CNT);
            Lcd_hextoascii(value,&lcd_seg_buf[SELTRE]);      /* HEXtoASCII */
            Lcd_nodisp(&lcd_seg_buf[SELTRE],1);            /* 上位が0なら、表示しない */
            lcd_seg_buf[SELTRE] = '+';                     /* "+" */
         }
         else
         {
            lcd_seg_buf[SELTRE+1] = '0';
         }
         break;
      case CSBM_AUD_BAL:
         if(seldisp.value == CAUD_BAL_CNT)   // CAUD_BAL_CNT = 5
         {
            lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAL_C]);
         }
         else
         {
            lcd_moji_romcpy(lcd_seg_buf,(BYTE *)&CLCD_SEL_TBL[BAL]);
            if(seldisp.value > CAUD_BAL_CNT)
            {
               
               seldisp.value -= CAUD_BAL_CNT;
               Lcd_hextoascii( seldisp.value, &lcd_seg_buf[SELBAL] );      /* HEX to ASCII */
               Lcd_nodisp( &lcd_seg_buf[SELBAL],1 );                  /* 上位が0場合、表示しない */
               lcd_seg_buf[SELBAL] = 'R';
            }
            else
            {
            
               seldisp.value = CAUD_BAL_CNT - seldisp.value;
               Lcd_hextoascii( seldisp.value, &lcd_seg_buf[SELBAL] );      /* HEX to ASCII */
               Lcd_nodisp( &lcd_seg_buf[SELBAL],1 );                  /* 上位が0場合、表示しない */
               lcd_seg_buf[SELBAL] = 'L';
            }
         }
         break;
      default:
         break;
   }
}

/****************************************************************
      関数名：Lcd_tuner_mode
      引  数：無し
      戻り値：無し
      機  能：TUNERﾓｰﾄﾞ表示処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/30      0.1      ha      新規作成
****************************************************************/
static void Lcd_tuner_mode(void)
{
   static TDP_TUNER tudisp; // trong file lcd_ext.h
   BYTE dmode;
   BYTE ind;
   
   memset(&tudisp,0,sizeof(tudisp));            /* ﾊﾞｯﾌｧｸﾘｱ */
   
   if (Ma_get_clock() == FALSE)   // tra ve FALSE neu status la  CMA_INIT_STA_ALL_NG hay CMA_INIT_STA_AREA_NG, nguoc lai tra ve ma_ini_data.clock
   {
      dmode = CLCD_BASE_TUNER;
   }
   else
   {
      if(lcd_base_mode == CLCD_BASE_TUNER)
      {
         dmode = CLCD_BASE_TUNER;
      }
      else
      {
         dmode = CLCD_BASE_CLOCK;
      }
   }
   
   Tu_lcddata_set(&tudisp,dmode);
   /*
       Neu tudisp.mode == CLCD_DISPMODE co nghia nguoi dung ko thao tac cac key va LCD chi don gian la hien thi. 
       Neu tudisp.mode != CLCD_DISPMODE co nghia nguoi dung dang thao tac key(UP/DOWN, SEEK, SCAN...).
   */
   if((tudisp.mode != CLCD_DISPMODE) && (tudisp.mode < CLCD_TU_MAX))      /* 特殊状態、内部ﾓｰﾄﾞ */
   {
      switch(tudisp.mode)         /* 表示ﾓｰﾄﾞ */
      {
         case CLCD_TU_SEEK:         /* SEEK中 */
            Lcd_tu_base(&tudisp);
            //Lcd_tu_base_frq(tudisp.band,tudisp.bandno,&(tudisp.frq[0]),tudisp.preno);
            Lcd_disp_1shot_base(CMA_TUNER);         /* SEEK後ﾍﾞｰｽ5秒表示要求    base 5 seconds display request after SEEK */
            break;
   
         case CLCD_TU_SCAN:         /* ﾌﾟﾘｾｯﾄｽｷｬﾝ中   Dang trong luc  Preset scanning*/
            Lcd_tu_base(&tudisp);
            break;
            
         case CLCD_TU_SCANWAIT:
            Lcd_tu_base(&tudisp);
            Lcd_blink_set(CLCD_BLK_FREQ,BLK_500MS);      /* 周波数点滅処理  Xu li Frequency blinking*/
            break;
         
         case CLCD_TU_PSTWT:  // preset write
            Lcd_tu_base(&tudisp);
            Lcd_blink_set(CLCD_BLK_PSNO,BLK_500MS);      /* プリセットNo点滅処理 */
            break;
         
         case CLCD_TU_AS:         /* SEEK中 */
            Lcd_tu_base(&tudisp);
            Lcd_disp_1shot_base(CMA_TUNER);         /* SEEK後ﾍﾞｰｽ5秒表示要求 */
            break;
   
         default:
            break;
      }
   }
   else                        /* DISPﾓｰﾄﾞ */
   {
      if(Clk_adj_check() == ON)
      {
         dmode = CLCD_BASE_CLOCK;      /* 時計調整表示設定 */
      }
      switch(dmode)
      {
         case CLCD_BASE_TUNER:
            Lcd_tu_base(&tudisp);
            break;
         case CLCD_BASE_CLOCK:
            Lcd_clock_mode();
            break;
         default:
            break;
      }
   }
}
/****************************************************************
      関数名：Lcd_tu_base
      引  数：無し
      戻り値：無し
      機  能：ベース表示処理(TUNER)
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
static void Lcd_tu_base(TDP_TUNER *add)
{
   #if 0
   BYTE tu_ind;
   tu_ind = Tu_indicator_set();
   if((tu_ind & CLCD_TU_IND_S1) != 0)            /* "1"    */
   {
      lcd_send_buf.record1.s1 = ON;
   }
   #endif
   Lcd_tu_indicator();
   Lcd_tu_base_frq(add->band,add->bandno,&(add->frq[0]),add->preno);
}
/****************************************************************
      関数名：Lcd_tu_base_frq
      引  数：無し
      戻り値：無し
      機  能：ベース表示処理(TUNER周波数)
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
static void Lcd_tu_base_frq(BYTE band,BYTE bandno,BYTE *freq,BYTE preno)
{
   BYTE dest;
   
   switch(band)
   {
      case CTU_FM:
/* 周波数表示 */
         lcd_seg_buf[FRQ] = (((*(freq+1))>>4) & 0x0F) + 0x30;            // FRQ = 0, trong file lcd_mode.h
         lcd_send_buf.record1.dot = ON;               /* "."点灯 */
         //Lcd_nodisp(&lcd_seg_buf[FRQ],1);               /* 0表示しない */
         lcd_seg_buf[FRQ+1] = ((*(freq+1)) & 0x0F) + 0x30;
         lcd_seg_buf[FRQ+2] = (((*(freq+2)) >> 4) & 0x0F) + 0x30;
         if (Ma_model_dest() == CMA_MODEL_AMFM_A )
         {
            lcd_seg_buf[FRQ+3] = ((*(freq+2)) & 0x0F) + 0x30;
         }
         else
         {
            Lcd_nodisp(&lcd_seg_buf[FRQ+3],1);                        /* 0表示しない */
         }
         break;
      case CTU_AM:
/* 周波数表示 */
         #if 0
         lcd_seg_buf[FRQ] = (((*(freq+1)) >> 4) & 0x0F) + 0x30;
         Lcd_nodisp(&lcd_seg_buf[FRQ],1);                        /* 0表示しない */
         lcd_seg_buf[FRQ+1] = ((*(freq+1)) & 0x0F) + 0x30;
         lcd_seg_buf[FRQ+2] = (((*(freq+2)) >> 4) & 0x0F) + 0x30;
         lcd_seg_buf[FRQ+3] = ((*(freq+2)) & 0x0F) + 0x30;
         #else
         lcd_seg_buf[FRQ] = ((*(freq+1)) & 0x0F) + 0x30;
         lcd_seg_buf[FRQ+1] = (((*(freq+2))>>4) & 0x0F) + 0x30;
         lcd_seg_buf[FRQ+2] = ((*(freq+2)) & 0x0F) + 0x30;
         #endif
         break;
      default:
         break;
   }
/* ﾌﾟﾘｾｯﾄﾅﾝﾊﾞｰ表示   Preset number display   */
   if((preno > 0) && (preno <= 7))            /* 範囲以外処理しない  */
   {
      lcd_seg_buf[PRENO] = preno + 0x30;
   }
}

void Lcd_tu_indicator(void )
{   
   BYTE ind;
   ind = Tu_indicator_set();
   if((ind & CLCD_TU_IND_S1) != 0)            /* "1"    */
   {
      lcd_send_buf.record1.s1 = ON;
   }
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
   #if 0   /* STは別点灯処理に仕様変更 */
   if((ind & CLCD_TU_IND_ST) != 0)         /* "ST"    */
   {
      lcd_send_buf.record1.st = ON;
   }
   #endif
}

/****************************************************************
      関数名：Lcd_st_indicator
      引  数：無し
      戻り値：無し
      機  能：STインジケータ有無判定処理                              ST Indicator Presence / Absence Determination Process
            仕様変更により追加
            時計表示中でもSTインジケータを点灯させるため           To light the ST indicator even while the clock is displayed
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2013/04/02      1.0      山本   新規作成
****************************************************************/
static void Lcd_st_indicator(void )
{   
   BYTE ind;
   if( (Ma_current_mode()    == CMA_TUNER   )   &&
      (Tu_band_chk()       == CTU_FM      )   )
   {
      ind = Tu_indicator_set();
      if((ind & CLCD_TU_IND_ST) != 0)         /* "ST"    */
      {
         lcd_send_buf.record1.st = ON;
      }
   }
}

/****************************************************************
      関数名：Lcd_isr_mode
      引  数：無し
      戻り値：無し
      機  能：ISRﾓｰﾄﾞ表示処理(交通情報)
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/30      0.1      ha      新規作成
****************************************************************/
static void Lcd_isr_mode(void)
{
   BYTE dmode;
   BYTE dest;
   BYTE ind;
   TDP_ISR isrdisp;  //  TDP_ISR trong file lcd_ext.h
   BYTE tu_isr_ind;

   if (Ma_get_clock() == FALSE)
   {
      dmode = CLCD_BASE_ISR;
   }
   else
   {
      if(lcd_base_mode == CLCD_BASE_ISR)
      {
         dmode = CLCD_BASE_ISR;
      }
      else
      {
         dmode = CLCD_BASE_CLOCK;
      }
   }
   Tu_isr_lcddata_set(&isrdisp,dmode);  // gan tan so va mode ISR cho isrdisp
   tu_isr_ind = Tu_isr_indicator_set();
   if((tu_isr_ind & CLCD_TU_IND_TI) != 0)            /* "1"    */
   {
      lcd_send_buf.record1.ti = ON;
   }
   switch(dmode)
   {
      case CLCD_BASE_ISR:
            if((tu_isr_ind & CLCD_TU_IND_S1) != 0)            /* "1"    */
            {
               lcd_send_buf.record1.s1 = ON;
            }
            lcd_send_buf.record1.am = ON;
            #if 1
            lcd_seg_buf[FRQ] = (isrdisp.frq[1] & 0x0F) + 0x30;
            lcd_seg_buf[FRQ+1] = ((isrdisp.frq[2] >> 4) & 0x0F) + 0x30;
            lcd_seg_buf[FRQ+2] = (isrdisp.frq[2] & 0x0F) + 0x30;
            #else
            lcd_seg_buf[FRQ+1] = ((isrdisp.frq[1] >> 4) & 0x0F) + 0x30;
            Lcd_nodisp(&lcd_seg_buf[5],1);                        /* 0表示しない */
            lcd_seg_buf[FRQ+2] = (isrdisp.frq[1] & 0x0F) + 0x30;
            lcd_seg_buf[FRQ+3] = ((isrdisp.frq[2] >> 4) & 0x0F) + 0x30;
            lcd_seg_buf[FRQ+4] = (isrdisp.frq[2] & 0x0F) + 0x30;
            #endif
         break;
      
      case CLCD_BASE_CLOCK:
         Lcd_clock_mode();            /* 時計表示 */
         break;
   
      default:
         break;
   }
}
/****************************************************************
      関数名：Lcd_model_version
      引  数：無し
      戻り値：無し
      機  能：機種名表示         Model name display
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2007/4/23      0.1      何      新規作成
         2010/6/9      0.2      xu      
****************************************************************/
static void Lcd_model_version(void)
{
   lcd_seg_buf[0] = 'U';
   lcd_seg_buf[1] = '-';
   memcpy(&lcd_seg_buf[2],_VERSION_,2);  // VERSION = "16", trong file model.h
   lcd_seg_buf[4] = Ma_model_dest()+0x30;   // Ma_model_dest trong file ma_ext.c, tra ve gia tri ma_dest
}
/****************************************************************
      関数名：Lcd_aux_mode
      引  数：無し
      戻り値：無し
      機  能：AUXﾓｰﾄﾞ表示処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/27      0.1      ha      新規作成
****************************************************************/
static void Lcd_aux_mode(void)
{
   BYTE dmode;
   
   if (Ma_get_clock() == FALSE)
   {
      dmode = CLCD_BASE_AUX;
   }
   else
   {
      if(lcd_base_mode == CLCD_BASE_AUX)
      {
         dmode = CLCD_BASE_AUX;
      }
      else
      {
         dmode = CLCD_BASE_CLOCK;      /* ベース表示は時計 */
      }
   }
   switch(dmode)
   {
      case CLCD_BASE_AUX:
         memcpy(lcd_seg_buf,STR_AUX,6);  // trong file lcd_tbl.h, STR_AUX = "AU   "
         break;
      case CLCD_BASE_CLOCK:
         Lcd_clock_mode();            /* 時計表示 */
         break;
      default:
         break;
   }
}

/****************************************************************
      関数名：Lcd_clock_mode
      引  数：mode_id→各ﾒｲﾝﾓｰﾄﾞ,CLCD_OFF_COLCK→OFFﾓｰﾄﾞ
      戻り値：無し
      機  能：時計優先表示ﾓｰﾄﾞ処理
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/07/30      0.1      ha      新規作成
****************************************************************/
void Lcd_clock_mode(void)
{
   static TDP_CLK clkdisp;  // TDP_CLK trong file lcd_ext.h, clkdisp co 4 phan tu BYTE trong struct TDP_CLK: hour, min, adjust, mode.
   
   memset(&clkdisp,0,sizeof(clkdisp));      /* ｸﾘｱ  Clear */
   if (Ma_get_clock() == FALSE) /* Ma_get_clock trong file ma_ext.c. H瀘 se tra ve gia tri FALSE khi status la CMA_INIT_STA_ALL_NG va
                                   CMA_INIT_STA_AREA_NG, nguoc lai tra ve ma_ini_data.clock*/ 
   {
      Lcd_blank_disp();         // trong file lcd_mode.c. Noi dung h瀘: Lcd_nodisp( &lcd_seg_buf[0],5 );
      return;                   // Khi PWROFF, vi CLOCK bi error nen LCD se hien thi ki tu cua mode truoc khi PWROFF cho den khi PWRON lai.
   }
   
   Clk_lcddata_set(&clkdisp);   // trong file clock_mode.c.  Thiet lap gia tri data hien thi dong ho trong Alarm
   
   /* 調整ﾓｰﾄﾞ中 */
   if(clkdisp.mode != CSBM_CLK_NORMAL)  /*CSBM_CLK_NORMAL duoc dinh nghia trong file sbm_def.h.  
                                          CSBM_CLK_NORMAL:  Clock dang chay binh thuong, CSBM_CLK_ADJUST: Dang trong qua trinh dieu chinh thoi gian 
                                          Neu thoi gian dang trong qua trinh dieu chinh, thi se xet tiep xem dang dieu chinh thoi gian cua Alarm hay thoi gian cua Dong ho
                                          Neu dieu chinh thoi gian cua Alarm thi thoi gian Alarm nhap nhay, con ko thi thoi gian Dong ho nhap nhay*/
   {
      if (Alm_mode_check() != OFF)    // co phai dang dieu chinh thoi gian cua Alarm ko. 
      {
         Lcd_blink_set(CLCD_BLK_ALARM,BLK_500MS);   /* 時計点滅 */
      }
      else
      {
         Lcd_blink_set(CLCD_BLK_CLOCK,BLK_500MS);   /* 時計点滅 */
      }
   }
   lcd_send_buf.record1.colon = ON;                        /* ":"点灯 */
   Lcd_hextoascii( clkdisp.hour, &lcd_seg_buf[0] );  /* trong file lcd_main.c. Vi du, clkdisp.hour = 12(12 gio),vi o day so "12" la decimal va doi ra ascii ko co y nghia,
                                                        sau khi thuc hien h瀘 hextoascii thi ta co: lcd_seg_buf[0]=49(HEX:0x31), lcd_seg_buf[1]=50(HEX:0x32).*/
   Lcd_nodisp( &lcd_seg_buf[0],1 );                  /* Neu ta co hour=08 (8 gio), thi so "0"(lcd_seg_buf[0]) se ko can thiet phai hien thi*/
   Lcd_hextoascii( clkdisp.min, &lcd_seg_buf[2] );      /* lcd_seg_buf[2]: ph�t o hang chuc va lcd_seg_buf[3]: phut o hang don vi  */
   if ((Alm_mode_check() == ON))
   {
      lcd_seg_buf[4] = 'A';               /* seg5に'A'を表示する    Display 'A' in seg 5  Neu dang trong mode Alarm, th� 
                                             chu 'A' se xuat hien ke ben thoi gian.(ca 2 truong hop nhap nhay va ko nhap nhay)
                                             VD: 5:20A(Binh thuong)=> 5:20A ->  :  A-> 5:20A (Nhap nhay) 
                                                12:00A(Binh thuong)=>12:00A ->  :  A->12:00A (Nhap nhay) */
   }
   Lcd_clkalm_indicator(); // Ham se thuc hien nen chon am hay pm tuy vao thoi gian dong ho
}

void Lcd_clkalm_indicator(void)
{
   BYTE ind;
   if (Alm_mode_check() == ON)
   {
      ind = Alm_indicator_set();  // tra ve indicator la PM hay AM
   }
   else
   {
      ind = Clk_indicator_set();  // tra ve indicator la PM hay AM
   }
   if((ind & CLCD_CL_IND_AM) != 0 )      /* "AM" */
   {
      lcd_send_buf.record1.am = ON;
   }
   if((ind & CLCD_CL_IND_PM) != 0 )      /* "PM" */
   {
      lcd_send_buf.record1.pm = ON;
   }
}
void   Lcd_blank_disp(void)
{
   Lcd_nodisp( &lcd_seg_buf[0],5 ); // ko hien thi ca 5 seg, tuc la seg ko s疣g chu ko fai hien thi so 0
}

void   Lcd_area_disp(void)
{
   lcd_seg_buf[0] = 'A';
   lcd_seg_buf[1] = 'R';
   lcd_seg_buf[2] = 'E';
   lcd_seg_buf[3] = 'A';
   Lcd_nodisp( &lcd_seg_buf[4],1 );  // dc khoi tao trong file lcd_main.c. seg thu 5 ko hien thi.
}

#if 0 /* 使用しないため、削除 */
/****************************************************************
      関数名：Lcd_allseq_on
      引  数：無し
      戻り値：無し
      機  能：全点灯
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/08/20      0.1      ha      新規作成
****************************************************************/
void   Lcd_allseq_on(void)
{
   memset(lcd_send_buf.buf_byte,0xff,sizeof(lcd_send_buf.buf_byte));   /* 送信バッファ0ｸﾘｱ */
   
   lcd_send_buf.record.ctrl_1 = 0x00;   /* 0000ｺﾝﾄﾛｰﾙﾃﾞｰﾀ */
   lcd_send_buf.record.ctrl_2 = 0x09;   /* 10010000 */
   lcd_send_buf.record.ctrl_3 = 0x12;   /* 01001000 */      /* 1/3 Duty */

   lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;   /* 2ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
   lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;   /* 3ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
   
   memset(lcd_seg_buf,0xff,sizeof(lcd_seg_buf));
   
   flcd_send = ON;
}

/****************************************************************
      関数名：Lcd_allseq_off
      引  数：無し
      戻り値：無し
      機  能：全消灯
----------------------------------------------------------------
修正履歴   【年月日】【Version】【名前】 【説明】
----------------------------------------------------------------
         2012/08/20      0.1      ha      新規作成
****************************************************************/
void   Lcd_allseq_off(void)
{
   memset(lcd_send_buf.buf_byte,0x00,sizeof(lcd_send_buf.buf_byte));   /* 送信バッファ0ｸﾘｱ */
   
   lcd_send_buf.record.ctrl_1 = 0x00;   /* 0000ｺﾝﾄﾛｰﾙﾃﾞｰﾀ */
   lcd_send_buf.record.ctrl_2 = 0x09;   /* 10010000 */
   lcd_send_buf.record.ctrl_3 = 0x12;   /* 01001000 */      /* 1/3 Duty */

   lcd_send_buf.buf_byte[RECORD2+SENDBYTE-1] = 0x80;   /* 2ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
   lcd_send_buf.buf_byte[RECORD3+SENDBYTE-1] = 0x40;   /* 3ﾚｺｰﾄﾞ目固定ﾃﾞｰﾀ */
   
   memset(lcd_seg_buf,0x00,sizeof(lcd_seg_buf));
   
   flcd_send = ON;
}
#endif

#undef      _LCD_MODE_C_
