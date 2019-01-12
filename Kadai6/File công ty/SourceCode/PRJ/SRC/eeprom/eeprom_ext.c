/******************************************************************************
   クラリオン（株）殿向け      2007 C3X03 ｼﾘｰｽﾞ[仮]

      プロジェクト名   : '07年フリップダウン　MP3モデル開発
      ファイル名      : eeprom_func_ext.c
      機能         : 
------------------------------------------------------------------------------
修正履歴
******************************************************************************/
#include   "../model.h"
#if    _MODEL_EEPROM_
#define   _EEPROM_EXT_C_

#include   <string.h>
#include   "../comm/common.h"
#include   "../io/IO_inc.h"
#include   "../testmode/TEST_inc.h"

#include   "eeprom_main.h"
#include   "eeprom_ext.h"

#if _TEST_MODE_
static BYTE eep_test_buf[19];
#endif

/******************************************************************************
..       関数名  ：Eeprom_1ms_timer
      引  数   ：なし
      戻り値   ：なし
      機  能   ：EEPROM 1ms定期ﾀｲﾏ処理
------------------------------------------------------------------------------
修正履歴
[sun090330] 新規作成
******************************************************************************/
void   Eeprom_1ms_timer(void)
{
   if (bEEP_Drv_Timer > TIMER_OUT)
   {
      bEEP_Drv_Timer--;
   }
}

/******************************************************************************
..       関数名  ：eeprom_data_chk
      引  数   ：data  ⇒ 書込みﾃﾞｰﾀ
            ：map_index ⇒ ﾃﾞｰﾀ格納ﾊﾞｯﾌｧIndex
      戻り値   ：なし
      機  能   ：EEPROM ﾏｯﾌﾟInfo取得処理
------------------------------------------------------------------------------
修正履歴
[sun100805]新規作成
******************************************************************************/
void   EEP_WriteReq(sEEP_PARA   WtReq)
{
   if ((sEEP_DRVWork.Busy == FALSE) &&
      (WtReq.Length <= CEEP_BUFMAX))
   {
      memcpy(bEEP_DataBuf,WtReq.BufAddr,WtReq.Length);
      sEEP_DRVWork.SlaveAddr = WtReq.EepAddr;
      sEEP_DRVWork.Length = WtReq.Length;
      sEEP_DRVWork.DataAddr = bEEP_DataBuf;
      sEEP_DRVWork.Busy = ON;
   }
}

/******************************************************************************
..       関数名  ：eeprom_data_mapread
      引  数   ：map_index ⇒取得ﾏｯﾌﾟデータIndex
      戻り値   ：取得したデータ「WORD単位」
      機  能   ：EEPROM ﾏｯﾌﾟﾃﾞｰﾀ取得処理
------------------------------------------------------------------------------
修正履歴
[sun100805]新規作成
******************************************************************************/
BYTE   EEP_ReadReq(sEEP_PARA   ReadReq)
{
   BYTE   iRet;

   iRet = FALSE;
   if (EEP_CheckIDLE() == TRUE)
   {
      sEEP_DRVWork.SlaveAddr = ReadReq.EepAddr;
      sEEP_DRVWork.DataAddr = ReadReq.BufAddr;
      sEEP_DRVWork.Length = ReadReq.Length;
      if (EEP_Drv_Read() == TRUE)
      {
         iRet = TRUE;
      }
   }
   else
   {
      iRet = FALSE;
   }
   return(iRet);
}

/******************************************************************************
..       関数名  ：BYTE   EEP_CheckIDLE(void)
      引  数   ：なし
      戻り値   ：TRUE　⇒　EEPROM　IDLE中
            ：FALSE　⇒動作中
      機  能   ：EEPROM　IDLE中かどうかﾁｪｯｸ処理
------------------------------------------------------------------------------
修正履歴
[sun100805]新規作成
******************************************************************************/
BYTE   EEP_CheckIDLE(void)
{
   if ((sEEP_DRVWork.Busy == ON) ||
      (bEEP_Drv_Timer != 0x00))
   {
      return(FALSE);
   }
   else
   {
      return(TRUE);
   }
}


/******************************************************************************
..       関数名  ：eeprom_write_ok_chk
      引  数   ：なし
      戻り値   ：OK ⇒ 書込み中
            ：NG ⇒ 書込み完了
      機  能   ：EEPROM Reset要求ﾁｪｯｸ処理
------------------------------------------------------------------------------
修正履歴
[sun090325] 新規作成
[sun090416]   書込み要求全部なくなるまでNGを返すように変更
******************************************************************************/
BOOL   eeprom_write_ok_chk(void)
{
   if ((feep_lastwt == ON) ||
      (bEEP_Drv_Timer != TIMER_OUT) ||
      (sEEP_DRVWork.Busy == ON) ||
      (feep_reset == ON) ||
      (feep_initial_req == NG))
   {
      return(NG);
   }
   else
   {
      return(OK);
   }
}
/******************************************************************************
..       関数名  ：eeprom_adj_reset
      引  数   ：なし
      戻り値   ：なし
      機  能   ：EEPROM Reset処理
------------------------------------------------------------------------------
修正履歴
******************************************************************************/
void   EEP_Initial_Req(void)
{
   #if 0
   feep_initial_req = ON;
   #endif
   
   BYTE isys;
   
   /* FacrotyCode1 5BYTE */
   bEEP_inidata[0] = 0x5a;
   bEEP_inidata[1] = 0xa5;
   bEEP_inidata[2] = 0x00;      /* 仕向け無し   ko co destination */
   bEEP_inidata[3] = 0xFF;      /* 時計有       co dong ho */
   bEEP_inidata[4] = 0xFF;      /* アラーム有    co bao thuc */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_INIDATA; // CEEP_ADDR_INIDATA trong file eeprom_ext.h
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_INIDATA; // CEEP_LENG_INIDATA = 5;
   EEP_Drv_Write(); // nam trong file eeprom_ext.c
   
   for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
   {
      Cm_wait_loop(0xFF);
   }
   
   memset(bEEP_inidata,0xFF,sizeof(bEEP_inidata));
   /* AM領域 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHAM;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_PCHAM;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FM1領域 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHFM1;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_PCHFM1;
   EEP_Drv_Write();

   for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FM2領域 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHFM2;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_PCHFM2;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FacrotyCode2 2BYTE */
   bEEP_inidata[0] = 0x5a;
   bEEP_inidata[1] = 0xa5;
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_FACTCODE2;
   sEEP_DRVWork.DataAddr = &bEEP_inidata[0];
   sEEP_DRVWork.Length = CEEP_LENG_FACTCODE;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
   {
      Cm_wait_loop(0xFF);
   }
}

void   EEP_Initial_para_Req(BYTE *buf,BYTE mode)
{
   BYTE isys;
   
   if (mode != 0xFF)
   {
      /* clockとalarm 2BYTE */
      bEEP_inidata[3] = 0x01;
      bEEP_inidata[4] = 0x01;
      sEEP_DRVWork.SlaveAddr = CEEP_ADDR_CLKALM;
      sEEP_DRVWork.DataAddr = &bEEP_inidata[3];
      sEEP_DRVWork.Length = CEEP_LENG_CLKALM;
      EEP_Drv_Write();   // nam trong file eeprom_main.c
      
      for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
      {
         Cm_wait_loop(0xFF);
      }
   }
   
   /* AM領域 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHAM;
   sEEP_DRVWork.DataAddr = &buf[0];
   sEEP_DRVWork.Length = CEEP_LENG_PCHAM;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FM1領域 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHFM1;
   sEEP_DRVWork.DataAddr = &buf[12];
   sEEP_DRVWork.Length = CEEP_LENG_PCHFM1;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
   {
      Cm_wait_loop(0xFF);
   }
   
   /* FM2領域 12BYTE */
   sEEP_DRVWork.SlaveAddr = CEEP_ADDR_PCHFM2;
   sEEP_DRVWork.DataAddr = &buf[24];
   sEEP_DRVWork.Length = CEEP_LENG_PCHFM2;
   EEP_Drv_Write();
   
   for (isys = 0; isys <40; isys ++)      /* 10ms待つ */
   {
      Cm_wait_loop(0xFF);
   }
}
/*****************************************************************************************
   関数名      ：IO_EEP_WriteData_XBYTE
   引数      ：なし
   リターン   ：OKかNG
   機能概要   ：XBYTE分のデータを書込み
-----------------------------------------------------------------------------------------
[chin20120830] 新規作成
*****************************************************************************************/
BYTE   EEP_WriteData_xBYTE( BYTE *buf, BYTE size )
{
   BYTE ret;
   BYTE iCnt;
   
   ret = NG;
   if (size > 16)
   {
      return ret;
   }
   
   ret = 0x01;
   for (iCnt = 0; iCnt < size; iCnt++)
   {
      ret &= IO_EEP_WriteByte( *buf++);
   }
   return ret;
}

void EEP_Test_func(BYTE cmd,BYTE *addr,BYTE len)
{
   BYTE eep_test;
   sEEP_PARA Req;
   
   if (cmd != CTEST_MODE_ATMIC_EEP)
   {
      return ;
   }
   eep_test = *(addr+1);
   switch(eep_test)
   {
      case 0x08:            /* 1BYTE 書込み */
         Req.EepAddr = *(addr+ 3);
         Req.Length = 0x01;
         Req.BufAddr = addr+4;
         EEP_WriteReq(Req);
         break;
      
      case 0x09:            /* 16BYTE 書込み */
         Req.EepAddr = *(addr+ 3);
         Req.Length = 0x10;
         Req.BufAddr = addr+4;
         EEP_WriteReq(Req);
         break;
      
      case 0x18:            /* 1BYTE 読出し */
         Req.EepAddr = *(addr+ 3);
         Req.Length = 0x01;
         Req.BufAddr = &eep_test_buf[3];
         if (EEP_ReadReq(Req))
         {
            eep_test_buf[0] = 0x02;
            eep_test_buf[1] = 0;
            eep_test_buf[2] = Req.EepAddr;
            Test_data_reply(CTEST_MODE_ATMIC_EEP,0x28,eep_test_buf, 0x04);
         }
         else
         {
            eep_test_buf[0] = 0xFF;
            Test_data_reply(CTEST_MODE_ATMIC_EEP,0x28,eep_test_buf, 0x01);
         }
         break;
      
      case 0x19:            /* 16BYTE 読出し */
         Req.EepAddr = *(addr+ 3);
         Req.Length = 0x10;
         Req.BufAddr = &eep_test_buf[3];
         if (EEP_ReadReq(Req))
         {
            eep_test_buf[0] = 0x04;
            eep_test_buf[1] = 0;
            eep_test_buf[2] = Req.EepAddr;
            Test_data_reply(CTEST_MODE_ATMIC_EEP,0x28,eep_test_buf, 0x13);
         }
         else
         {
            eep_test_buf[0] = 0xFF;
            Test_data_reply(CTEST_MODE_ATMIC_EEP,0x28,eep_test_buf, 0x01);
         }
         break;
      default :
         break;
   }
}
#undef  _EEPROM_EXT_C_
#endif   /* _MODEL_EEPROM_ */
