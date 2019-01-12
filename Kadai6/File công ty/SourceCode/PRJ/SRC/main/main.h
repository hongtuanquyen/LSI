/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

      プロジェクト名   : 
      ファイル名      : ma_main.h
      機能         : ①ﾒｲﾝﾓｼﾞｭｰﾙﾍｯﾀﾞﾌｧｲﾙ
------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051128]   新規作成
******************************************************************************/
#ifndef      _MAIN_H_
#define      _MAIN_H_

#undef   EXT
#ifdef   _MAIN_C_
#define   EXT       
#else
#define   EXT   extern
#endif


/*----------------------------------------------------
   関数プロトタイプ
----------------------------------------------------*/
void   Ma_message_set(BYTE, BYTE );

/*----------------------------------------------------
   変数定義
----------------------------------------------------*/
#define      CMA_MSGTBL_END   0xff
#define      CMA_MSGMAX      50
EXT   BYTE   ma_sysmsg[CMA_MSGMAX];         /* ﾒｯｾｰｼﾞ格納ﾊﾞｯﾌｧ   Bo dem luu tru message*/
EXT   BYTE   ma_msg_inptr;               /* ﾒｯｾｰｼﾞ格納ﾎﾟｲﾝﾀ       Con tro luu tru message*/
EXT   BYTE   ma_msg_outptr;               /* ﾒｯｾｰｼﾞ発行ﾎﾟｲﾝﾀ       Con tro phat hanh message*/
EXT   BYTE   ma_msg_seq;                  /* ﾒｯｾｰｼﾞ発行処理ｼｰｹﾝｽ     Trinh tu xu li viec phat hanh message*/
EXT   BYTE   ma_cur_msg;                  /* 発行中のｼｽﾃﾑﾒｯｾｰｼﾞ    System message trong qua trinh ph疸 h瀟h */
#endif
