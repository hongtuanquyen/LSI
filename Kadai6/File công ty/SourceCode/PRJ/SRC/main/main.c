/******************************************************************************
..     クラリオン殿向け   danh cho cong ty Clarion                 2007ﾓﾃﾞﾙ

      プロジェクト名   :
      ファイル名      : main.c
      機能         : Systemﾒｲﾝﾙｰﾌﾟ処理   Xu li vong lap System main
------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]   新規作成
******************************************************************************/
#define   _MAIN_C_

#include    <stdio.h>
#include    <string.h>
#include   "../model.h"
#include   "../comm/common.h"
#include    "../audio/aud_sys.h"
#include    "../disp/lcd_sys.h"
#include    "../power/PW_inc.h"
#include    "../key/key_sys.h"
#include   "../io/IO_inc.h"
#include    "../tuner/tu_sys.h"
#include   "../submode/sbm_sys.h"
#include   "../testmode/test_sys.h"
#include    "../aux1/aux_sys.h"
#if    _MODEL_EEPROM_
#include    "../eeprom/eeprom_ext.h"
#include    "../eeprom/eeprom_sys.h"   /* QAC対策 */
#endif

#include   "main.h"
#include   "ma_ext.h"
#include   "ma_sys.h"

/*----------------------------------------------------
   関数プロトタイプ
----------------------------------------------------*/
static void   Ma_message_call(BYTE  msg);
static void   Ma_message_process(void);
static void   Ma_message_clear(void);
static void   Ma_cur_message_get(void);

/*----------------------------------------------------
   ｼｽﾃﾑｺﾏﾝﾄﾞ受付処理登録ﾃｰﾌﾞﾙ
----------------------------------------------------*/
typedef   struct{
   void   (*func)(BYTE);
}TMA_SYSTBL;

const TMA_SYSTBL tma_sys_tbl[] =
{
   Key_sys_message,
   Test_sys_message,
   Sbm_sys_message,
   Tu_sys_message,
   Aux_sys_message,
   Aud_sys_message,
   Lcd_sys_message
};

const TMA_SYSTBL tma_sys_ini_ng_tbl[] =
{
   Key_sys_message,
   Test_sys_message,
   Aud_sys_message,
   Lcd_sys_message
};

/******************************************************************************
..       関数名  ：main 
      引  数   ：なし                   Doi so: Khong
      戻り値   ：無し                   Returned value: Khong
      機  能   ：SystemMainﾙｰﾌﾟ処理、ﾀｲﾏｶｳﾝﾄ処理      Xu li vong lap System Main     Xu li Timer Counter
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128] 新規作成
[sakurada060729] testVerでﾘｾｯﾄ回路停止（ _RESET_DISABLE_ ）
[sun070313] _RESET_DISABLE_定義削除変更
[sun090325] EEPROM RESETでMainﾙｰﾌﾟから抜けるように変更
[yamamoto20120727] ﾘｾｯﾄ処理削除
[yamamoto20121112] WDTｽﾀｰﾄ処理削除、Io_sfr_initial_set内で行う
******************************************************************************/
int   main(void)
{
   __DisableInterrupt();

   Io_sfr_initial_set();            /* ﾚｼﾞｽﾀ初期設定 WDTｽﾀｰﾄ   Xem IO_sfr.c v� sfr884200.h trong folder phan mem SanyoX16*/
   
   Ma_message_clear();               /* ﾒｯｾｰｼﾞﾊﾞｯﾌｧ初期化    Khoi tao message buffer */
   Ma_message_call(CMSG_INITIAL);      /* 初期化ﾒｯｾｰｼﾞ発行       ban hanh message khoi tao. CMSG_INITIAL nam trong common.h*/
   
   /*-EEPROM対応-----------------------------*/
   MIO_OUT_pSYS_ON(LOW);            /* SYS_ON端子制御 */
   Io_sfr_accoff_in();               /* 12M発振停止、RC発振開始 */
   /*----------------------------------------*/
   
   ma_cur_msg = CMSG_MAIN;            /* ｶﾚﾝﾄﾒｯｾｰｼﾞMainﾙｰﾌﾟに初期化 */

   __EnableInterrupt( 0 ) ;

   while(1)
   {
      Ma_message_process();               /* Mainﾙｰﾌﾟﾒｯｾｰｼﾞ発行 */

   }
   #if 0      /* レビュー指摘事項 */
   __Reset();
   #endif
}

/******************************************************************************
..       関数名  ：Ma_message_process
      引  数   ：なし
      戻り値   ：無し
      機  能   ：Systemﾒｯｾｰｼﾞ発行制御処理
            ：ﾒｯｾｰｼﾞﾊﾞｯﾌｧを確認し、発行されていないﾒｯｾｰｼﾞあれば発行、
            ：なければ、ﾒｲﾝﾙｰﾌﾟﾒｯｾｰｼﾞを発行
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
******************************************************************************/
static void   Ma_message_process(void)
{
   void   (*sys_func)(BYTE);
   BYTE   iLoop;
   BYTE   iSize;
   BYTE   ini_sta;

   Pw_sys_message(ma_cur_msg);         /* 電源ﾓｼﾞｭｰﾙ */
   Ma_sys_message(ma_cur_msg);         /* Mainﾓｼﾞｭｰﾙ */
   #if (_MODEL_EEPROM_ == YES)
   Eeprom_sys_message(ma_cur_msg);      /* BATT OFFでEEPROMのみ動作可能  chi thuc hien khi BATT OFF, neu trong EEPROM device co data duoc luu san trong do
                                                                        thi lenh n炸 se thuc hien viec doc data tu EEPROM device.*/
   #endif
   Ma_cur_message_get();            /* 発行ｺﾏﾝﾄﾞ再ｾｯﾄ */
   
   // Ma_Chk_EEP_Initial trong file ma_ext.c
   if ((Ma_Chk_EEP_Initial() == CMA_INIT_STA_ALL_OK)||
      (Ma_Chk_EEP_Initial() == CMA_INIT_STA_CKAL_NG))  
   {
      ini_sta = TRUE;
   }
   else
   {
      ini_sta = FALSE;
   }
   
   /* メインループ */
   if (ini_sta == TRUE)
   {
      iSize =  sizeof(tma_sys_tbl)/sizeof(TMA_SYSTBL);
      for (iLoop = 0; iLoop < iSize; iLoop ++)
      {
         sys_func = tma_sys_tbl[iLoop].func;
         
         (*sys_func)( ma_cur_msg );
      }
   }
   else  
   {
      iSize =  sizeof(tma_sys_ini_ng_tbl)/sizeof(TMA_SYSTBL);
      for (iLoop = 0; iLoop < iSize; iLoop ++)
      {
         sys_func = tma_sys_ini_ng_tbl[iLoop].func;
         (*sys_func)( ma_cur_msg );
      }
   }
}
/******************************************************************************
..       関数名  ：Ma_message_set
      引  数   ：なし
      戻り値   ：無し
      機  能   ：Systemﾒｯｾｰｼﾞ登録処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
[sun051213] ﾒｯｾｰｼﾞ登録後、ﾎﾟｲﾝﾀｲﾝｸﾘﾒﾝﾄするように変更
******************************************************************************/
static void   Ma_cur_message_get(void)
{
   if (ma_msg_outptr != ma_msg_inptr)
   {
      ma_cur_msg = ma_sysmsg[ma_msg_outptr];
      ma_msg_outptr ++;
      if( ma_msg_outptr >= CMA_MSGMAX )   // CMA_MSGMAX trong main.h
      {
         ma_msg_outptr = 0;
      }
   }
   else
   {
      ma_cur_msg = CMSG_MAIN;
   }
}

/******************************************************************************
..       関数名  ：Ma_message_clear
      引  数   ：なし
      戻り値   ：無し
      機  能   ：Systemﾒｯｾｰｼﾞｸﾘｱ処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
******************************************************************************/
static void   Ma_message_clear(void)
{
   memset(ma_sysmsg,0x00,sizeof(ma_sysmsg));      /* ﾒｯｾｰｼﾞ格納ﾊﾞｯﾌｧ0ｸﾘｱ   arrray ma_sysmsg chua toi da 50 phan tu.*/
   ma_msg_inptr = 0;                        /* ﾒｯｾｰｼﾞ格納ﾎﾟｲﾝﾀｸﾘｱ  Clear con tro chua message */
   ma_msg_outptr = 0;                        /* ﾒｯｾｰｼﾞ発行ﾎﾟｲﾝﾀｸﾘｱ  Clear con tro ban hanh message */
}

/******************************************************************************
..       関数名  ：Ma_message_call
      引  数   ：なし
      戻り値   ：無し
      機  能   ：初期ﾒｯｾｰｼﾞ通知処理    Xu li viec thong bao message khoi tao 
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128] 新規作成
[sun090401] 初期化EEPROM必ず先頭にｺｰﾙされること
******************************************************************************/
static void   Ma_message_call(BYTE  msg)
{
   #if    _MODEL_EEPROM_
   Eeprom_sys_message( msg );
   #endif
   Ma_sys_message( msg );            /* 初期設定読み込みあるので、必ず一番最初でｺｰﾙ */
   Pw_sys_message( msg );
   Test_sys_message( msg );
   Key_sys_message( msg );
   Sbm_sys_message( msg );
   Tu_sys_message( msg );
   Aux_sys_message( msg);
   Aud_sys_message( msg );
   Lcd_sys_message( msg );
}

/******************************************************************************
..       関数名  ：Ma_message_set
      引  数   ：なし
      戻り値   ：無し
      機  能   ：Systemﾒｯｾｰｼﾞ登録処理
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051212] 新規作成
[sun051213] ﾒｯｾｰｼﾞ登録後、ﾎﾟｲﾝﾀｲﾝｸﾘﾒﾝﾄするように変更
******************************************************************************/
void   Ma_message_set(BYTE mode, BYTE  msg)
{
   /* HOLDXINであれば、他のﾒｯｾｰｼﾞ全部ｸﾘｱ   If it is HOLDXIN, clear all other messages */
   if ((mode == CMA_POWER) && ( msg == CMSG_HOLDX_IN))
   {
      Ma_message_clear();
   }
   ma_sysmsg[ma_msg_inptr] = msg;

   ma_msg_inptr ++ ;

   if (ma_msg_inptr >= CMA_MSGMAX)
   {
      ma_msg_inptr = 0;
   }
}

#undef      _MAIN_C_
