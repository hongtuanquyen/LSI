/******************************************************************************
   クラリオン殿向け
   プロジェクト名   :    2012年AM/FMラジオ対応モデル開発
   ファイル名      :    aud_beep_func.c
   モジュール名   :    Audioモジュール
   機能概略      :    ブザー制御処理
-------------------------------------------------------------------------------
修正履歴
   番号      日付      担当者      関数名         修正内容
   【000】      2012/10/19   leejh                  新規作成
   【002】      2012/11/07   leejh      Aud_set_stop_beep(変更)
                              TAUD_BEEP_REQ_INFO(変更)
********************************************************************************/
#define      _AUD_BEEP_FUNC_C_

#include   <string.h>
#include   "../model.h"

#include   "../comm/common.h"
#include   "../io/IO_inc.h"
#include   "../power/PW_inc.h"
#include   "../tuner/TU_inc.h"
#include   "../main/MA_inc.h"
#include   "../submode/sbm_ext.h"

#include   "aud_ext.h"
#include   "aud_sys.h"
#include   "aud_def.h"
#include   "aud_main.h"
#include   "aud_mute_func.h"
#include   "aud_beep_func.h"

/*---------------------------------------------------------------------------*
 *   内部モジュール定数定義                                         *
 *---------------------------------------------------------------------------*/
/* 端子制御系 */
#define CAUD_BEEP_50MS_TIM         (( 50 / 5 ) + 1)   /* 50ms Timer(Base 5ms)    */
#define CAUD_BEEP_400MS_TIM         (( 400 / 5 ) + 1)   /* 400ms Timer(Base 5ms) */

/* BEEP制御系 */
#define CAUD_BEEP_30MS_TIM         ( 30 / 5 )      /* 30ms Timer(Base 5ms)    */
#define CAUD_BEEP_70MS_TIM         ( 70 / 5 )      /* 70ms Timer(Base 5ms)    */
#define CAUD_BEEP_510MS_TIM         ( 510 / 10 )   /* 510ms Timer(Base 10ms)*/

/* サイズ情報 */
#define CAUD_SET_BEEP_DATA_SIZE      11            /* BEEPデータサイズ       */

/* カウント情報 */
#define   CAUD_BEEP_ALARM_MAX_CNT      180            /* Alarm 180回          */
#define   CAUD_BEEP_ALARM_END_CNT      (CAUD_BEEP_ALARM_MAX_CNT - 1)

/* INDEX情報 */
#define CAUD_SET_ALARM_ON_IDX      2            /* アラーム On Index    */
#define CAUD_SET_ALARM_OFF_IDX      10            /* アラーム Off Index    */

/*   BEEP制御タイプ定数定義 */
/********* REQ BEEP ON *********/
#define CAUD_BEEP_ON_LONGKEY      0x01         /* Longkey On          */
#define CAUD_BEEP_ON_ALARM         0x02         /* Alarm On             */
#define CAUD_BEEP_ON_MULTIPLE      0x03         /* Multiple On          */
#define CAUD_BEEP_ON_WAIT         0x04         /* Wait On             */

/*********Other BEEP ON**********/

/********* REQ BEEP OFF *********/
#define CAUD_BEEP_OFF_LONGKEY      0x10         /* Longkey Off          */
#define CAUD_BEEP_OFF_ALARM         0x20         /* Alarm Off          */
#define CAUD_BEEP_OFF_MULTIPLE      0x30         /* Multiple Off          */
#define CAUD_BEEP_OFF_WAIT         0x40         /* Wait Off             */

/*********Other BEEP OFF*********/
#define   CAUD_BEEP_OFF_ALARM_START   0xA0         /* Alarm Start          */
#define   CAUD_BEEP_OFF_ALARM_END      0xC0         /* Alarm End          */
#define CAUD_BEEP_OFF_MUTE_ON      0xB0         /* Mute On             */
#define CAUD_BEEP_OFF_MUTE_OFF      0xD0         /* Mute Off             */

enum
{
   CAUD_BEEP_BASE_01MS = 0x01      ,            /* BEEP Time Base 01ms    */
   CAUD_BEEP_BASE_05MS            ,            /* BEEP Time Base 05ms    */
   CAUD_BEEP_BASE_10MS            ,            /* BEEP Time Base 10ms    */
   CAUD_BEEP_BASE_25MS            ,            /* BEEP Time Base 25ms    */
   CAUD_BEEP_BASE_TIMOUT         ,            /* BEEP Time Out       */
   CAUD_BEEP_BASE_05MS_MUTE_END   ,            /* BEEP 05ms + Mute End    */
   CAUD_BEEP_BASE_NOP   = 0xFF                  /* NOP                */
};

/*---------------------------------------------------------------------------*
 *   BEEP制御データテーブル(MAX：CAUD_SET_BEEP_DATA_SIZE)                 *
 *---------------------------------------------------------------------------*/
const BYTE aud_BEEP_longkey_data_tbl[] =   /* Longkey   */
{
   CAUD_BEEP_OFF_MUTE_OFF      ,   /* 0 */
   CAUD_BEEP_ON_LONGKEY      ,   /* 1 */
   CAUD_BEEP_OFF_MUTE_ON         /* 2 */
};

const BYTE aud_BEEP_alarm_data_tbl[] =      /* Alarm   */
{
   CAUD_BEEP_OFF_ALARM_START   ,   /* 0 */
   CAUD_BEEP_OFF_MUTE_OFF      ,   /* 1 */
   CAUD_BEEP_ON_ALARM         ,   /* 2 */
   CAUD_BEEP_OFF_ALARM         ,   /* 3 */
   CAUD_BEEP_ON_ALARM         ,   /* 4 */
   CAUD_BEEP_OFF_ALARM         ,   /* 5 */
   CAUD_BEEP_ON_ALARM         ,   /* 6 */
   CAUD_BEEP_OFF_ALARM         ,   /* 7 */
   CAUD_BEEP_ON_ALARM         ,   /* 8 */
   CAUD_BEEP_OFF_WAIT         ,   /* 9 */
   CAUD_BEEP_OFF_ALARM_END         /*10 */
};

const BYTE aud_BEEP_multiple_data_tbl[] =   /* Multiple   */
{
   CAUD_BEEP_OFF_MUTE_OFF      ,   /* 0 */
   CAUD_BEEP_ON_MULTIPLE         /* 1 */
};

/*---------------------------------------------------------------------------*
 *   BEEP制御ブロック内部構造体定義                                  *
 *---------------------------------------------------------------------------*/
typedef struct
{
   BYTE on_off;                           /* BEEP On and Off   */
   BYTE type;                              /* Type            */
   BYTE data[CAUD_SET_BEEP_DATA_SIZE];            /* Data            */
   BYTE cnt;                              /* Count         */
   BYTE max;                              /* Count Max      */
   BYTE alarm_onoff;                        /* Alarm on and Off */
   BYTE alarm_cnt;                           /* Alarm Count      */
   BYTE alarm_off_req;                        /* Alarm Off Req   */
   BYTE mute_off;                           /* Mute Off Control   */ 
} TAUD_BEEP_REQ_INFO;

/*---------------------------------------------------------------------------*
 *   BEEP制御ブロック内部変数定義                                   *
 *---------------------------------------------------------------------------*/
static   TAUD_BEEP_REQ_INFO   aud_beep_req_info;
static   BYTE            aud_beep_timer_id;
static   TAUD_AUDIO_INFO      *aud_aud_info;

/*---------------------------------------------------------------------------*
 *   プロトタイプ定義                                            *
 *---------------------------------------------------------------------------*/
static void Aud_beep_alarm_cont( BYTE );
static void Aud_beep_mute_cont( BYTE );
static BYTE Aud_set_beep_type( BYTE b_req );
static void Aud_beep_on( void );
static void Aud_beep_off( void );
static void Aud_set_alarm( BYTE );
static void Aud_clear_beep_info( void );
static void Aud_beep_cont ( void );

/******************************************************************************
   関数名      ：Aud_is_beep_mute
   引数      ：無し
   戻り値      ：BOOL      TRUE  : MUTE制御有り
            ：         FALSE : MUTE制御無し
   機能概略   ：BEEP(MUTE制御有り)を確認する
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
BOOL Aud_is_beep_mute( void )
{
   BOOL ret;
   
   ret = FALSE;
   
   /* MUTE制御あり */
   if(( aud_beep_req_info.type >= CAUD_BEEP_STA_LONGKEY_M_CONT) &&
      ( aud_beep_req_info.type <= CAUD_BEEP_STA_MULTIPLE_M_CONT))
   {
      /* TRUEを返す */
      ret = TRUE;
   }
   
   return ( ret );
}

/******************************************************************************
   関数名      ：Aud_beep_alarm_cont
     引数      ：BYTE on_off   ON   : ALARM端子 ON
                        OFF   : ALARM端子 OFF
   戻り値      ：無し
   機能概略   ：ALARM端子制御処理(BEEP制御処理専用)
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
#if (_POWER_DIAG_ == 0)
static void Aud_beep_alarm_cont( BYTE on_off )
{
   /* Alarm端子 ON   */
   if(( on_off == ON ) && ( aud_beep_req_info.alarm_onoff == ON ))
   {
      MIO_OUT_pALARM( HIGH );   
   }
   /* Alarm端子 OFF */
   else if(( on_off == OFF ) && ( aud_beep_req_info.alarm_onoff == ON ))
   {
      MIO_OUT_pALARM( LOW );   
   }
   /* Other */
   else
   {
      ;
   }
}
#endif

/******************************************************************************
   関数名      ：Aud_beep_mute_cont
     引数      ：BYTE on_off   ON   : MUTE端子 ON
                        OFF   : MUTE端子 OFF
   戻り値      ：無し
   機能概略   ：MUTE端子制御処理(BEEP制御処理専用)
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
static void Aud_beep_mute_cont( BYTE on_off )
{
   /* MUTE ON */
   if(( on_off == ON ) && ( aud_beep_req_info.mute_off == ON ))
   {
      MIO_OUT_pSYS_MUTE( LOW );
   }
   /* MUTE OFF */
   else if(( on_off == OFF ) && ( aud_beep_req_info.mute_off == ON ))
   {
      MIO_OUT_pSYS_MUTE( HIGH );   
   }
   /* Other */
   else
   {
      ;
   }
}

/******************************************************************************
   関数名      ：Aud_set_beep_type
   引数      ：BYTE req      CAUD_REQ_BEEP_LONGKEY         : Longkey BEEP
                        CAUD_REQ_BEEP_ALARM            : Alarm BEEP
                          CAUD_REQ_BEEP_MULTIPLE         : Multiple BEEP
   戻り値      ：BYTE b_type   CAUD_BEEP_STA_OFF            : OFF
                        CAUD_BEEP_STA_LONGKEY_M_NCONT   : キー長押し(MUTE制御無し)
                        CAUD_BEEP_STA_ALARM_M_NCONT      : アラーム(MUTE制御無し)
                        CAUD_BEEP_STA_MULTIPLE_M_NCONT   : 連続(MUTE制御無し)
                        CAUD_BEEP_STA_LONGKEY_M_CONT   : キー長押し(MUTE制御有り)
                        CAUD_BEEP_STA_ALARM_M_CONT      : アラーム(MUTE制御有り)
                        CAUD_BEEP_STA_MULTIPLE_M_CONT   : 連続(MUTE制御有り)
   機能概略   ：BEEP種類を設定する
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
static BYTE Aud_set_beep_type( BYTE b_req )
{
   BYTE b_type;
   
   b_type = CAUD_BEEP_STA_OFF;
      
   switch( b_req )
   {
      case CAUD_REQ_BEEP_LONGKEY:
         /* Type Longkey(Mute制御あり) */
         if(( Aud_get_mute_status() == CAUD_MUTE_ON_BUSY ) ||
            ( Aud_get_mute_status() == CAUD_MUTE_ON_END ))
         {
            /* モード切替中 Type Longkey(Mute制御無し) */
            if( Ma_is_mode_chg() == TRUE )
            {
               b_type = CAUD_BEEP_STA_LONGKEY_M_NCONT;
            }
            else
            {
               b_type = CAUD_BEEP_STA_LONGKEY_M_CONT;
            
               aud_beep_req_info.mute_off = ON;
            }
         }
         /* Type Longkey(Mute制御なし) */
         else
         {
            b_type = CAUD_BEEP_STA_LONGKEY_M_NCONT;
         }
         break;
         
      case CAUD_REQ_BEEP_ALARM:
         /* Type Alarm(Mute制御あり) */
         if(( aud_aud_info->vol_step == CAUD_VOL_MIN ) ||
            ( Pw_Power_status_get() == OFF ))
         {
               aud_beep_req_info.mute_off = ON;

               b_type = CAUD_BEEP_STA_ALARM_M_CONT;
         }
         /* Type Alarm(Mute制御なし) */
         else
         {
            /* TUNER SEEK中はType Alarm(Mute制御あり) */
            if(Tu_mainmd_check() == TRUE)
            {   
               /* TunerMute要求ON */
               Tu_beep_mute( ON );
               aud_beep_req_info.mute_off = ON;
               
               b_type = CAUD_BEEP_STA_ALARM_M_CONT;
            }
            else
            {
               b_type = CAUD_BEEP_STA_ALARM_M_NCONT;
            }
         }
         break;
         
      case CAUD_REQ_BEEP_MULTIPLE:
         /* Type Multiple(Mute制御あり) */
         if(( aud_aud_info->vol_step == CAUD_VOL_MIN ) ||
            ( Pw_Power_status_get() == OFF ))
         {
            aud_beep_req_info.mute_off = ON;

            b_type = CAUD_BEEP_STA_MULTIPLE_M_CONT;
         }
         /* Type Multiple(Mute制御なし) */
         else
         {
            b_type = CAUD_BEEP_STA_MULTIPLE_M_NCONT;
         }
         break;
         
      default:
         break;
   }
   
   return ( b_type );
}

/******************************************************************************
   関数名      ：aud_on_beep
   引数      ：無し
   戻り値      ：無し
   機能概略   ：BEEP On処理
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
#define      CAUD_BEEP_TIM_NOP      0x00
#define      CAUD_BEEP_TIM_TYPE1      0x01
#define      CAUD_BEEP_TIM_TYPE2      0x02
#define      CAUD_BEEP_TIM_TYPE3      0x03
#define      CAUD_BEEP_TIM_TYPE4      0x04

static void Aud_beep_on( void )
{   
   BYTE t_val;
   BYTE t_type;
   
   BYTE b_data;
   
   /* Fail Safe */
   t_type = CAUD_BEEP_TIM_NOP;
   t_val = 0;
   
   /* BEPPデータ取得 */
   b_data = aud_beep_req_info.data[aud_beep_req_info.cnt];
   
   /* インターバルタイマを取得 */
   switch ( b_data )
   {
/*******************************REQ BEEP ON********************************/
      case CAUD_BEEP_ON_LONGKEY:         
         t_type   = CAUD_BEEP_TIM_TYPE1;      /* タイプ1               */
         t_val   = CAUD_BEEP_30MS_TIM;      /* 30ms BEEP ON時間設定      */
         break;
         
      case CAUD_BEEP_ON_ALARM:
         t_type   = CAUD_BEEP_TIM_TYPE1;      /* タイプ1               */
         t_val   = CAUD_BEEP_70MS_TIM;      /* 70ms BEEP ON時間設定      */
         break;
         
      case CAUD_BEEP_ON_MULTIPLE:            /* 連続BEEP ON出力         */
         break;

/*******************************REQ BEEP OFF*******************************/
      case CAUD_BEEP_OFF_ALARM:         
         t_type   = CAUD_BEEP_TIM_TYPE1;      /* タイプ1               */
         t_val   = CAUD_BEEP_70MS_TIM;      /* 70ms BEEP OFF時間設定   */
         break;
         
      case CAUD_BEEP_OFF_WAIT:
         t_type   = CAUD_BEEP_TIM_TYPE2;      /* タイプ2               */
         t_val   = CAUD_BEEP_510MS_TIM;      /* 510ms BEEP OFF時間設定   */
         break;

/******************************Other BEEP OFF******************************/
      case CAUD_BEEP_OFF_ALARM_START:
#if (_POWER_DIAG_ == 0)
         Aud_beep_alarm_cont( ON );         /* Alarm端子 ON            */
#endif
         /* Power Off中 */
         if( Pw_Power_status_get() == OFF )
         {
            t_type   = CAUD_BEEP_TIM_TYPE1;   /* タイプ1               */
            t_val   = CAUD_BEEP_400MS_TIM;   /* 400ms BEEP OFF時間設定   */
         }
         /* Power On中 */
         else
         {
            /* ALARM(MUTE制御有り)は50ms */
            if( aud_beep_req_info.type == CAUD_BEEP_STA_ALARM_M_CONT )
            {
               if((Tu_mainmd_check() == TRUE)&&(Aud_get_mute_status() == CAUD_MUTE_OFF_BUSY))
               {
                  t_type   = CAUD_BEEP_TIM_TYPE4;   /* タイプ4            */
               }
               else
               {
                  t_type   = CAUD_BEEP_TIM_TYPE1;   /* タイプ1            */
               }
            }
            /* ALARM(MUTE制御無し)は50ms + MUTE OFF終了 */
            else
            {
               t_type   = CAUD_BEEP_TIM_TYPE4;   /* タイプ4            */
            }
            t_val   = CAUD_BEEP_50MS_TIM;   /* 50ms BEEP OFF時間設定   */
         }
         break;

      case CAUD_BEEP_OFF_ALARM_END:
         /* ALARM終了 */
         if(( aud_beep_req_info.alarm_onoff == ON ) &&
            ( aud_beep_req_info.alarm_cnt >= CAUD_BEEP_ALARM_END_CNT))
         {
            Aud_beep_mute_cont( ON );      /* MUTE ON               */

            t_type   = CAUD_BEEP_TIM_TYPE3;   /* タイプ3               */
         }
         /* ALARM未終了 */
         else
         {
            t_type   = CAUD_BEEP_TIM_TYPE3;   /* タイプ3               */
         }
         break;
         
      case CAUD_BEEP_OFF_MUTE_ON:
         Aud_beep_mute_cont( ON );         /* MUTE ON               */
         
         t_type = CAUD_BEEP_TIM_TYPE3;      /* タイプ3               */
         break;
         
      case CAUD_BEEP_OFF_MUTE_OFF:
         /* 長押しキー(Mute制御無し)、連続(Mute制御無し)はすぐにBEEP出力する */
         if(( aud_beep_req_info.type == CAUD_BEEP_STA_LONGKEY_M_NCONT ) || 
            ( aud_beep_req_info.type == CAUD_BEEP_STA_MULTIPLE_M_NCONT ))
         {
            /* MUTE OFF完了ではないお場合 */
            if( Aud_get_mute_status() != CAUD_MUTE_OFF_END )
            {
               t_type = CAUD_BEEP_TIM_TYPE4;   /* タイプ4               */
            }
            /* その他 */
            else
            {
               t_type = CAUD_BEEP_TIM_TYPE3;   /* タイプ3               */
               t_val   = CAUD_BEEP_50MS_TIM;   /* 50ms   BEEP OFF時間設定   */
            }
         }
         else
         {
            Aud_beep_mute_cont( OFF );      /* MUTE OFF               */
            
            t_val   = CAUD_BEEP_50MS_TIM;   /* 50ms   BEEP OFF時間設定   */
            t_type   = CAUD_BEEP_TIM_TYPE1;   /* タイプ1               */
         }
         break;
      
      default:   /* Exception */
         return;
   }
   
   /* インターバルタイマを設定 */
   switch ( t_type )
   {
      case CAUD_BEEP_TIM_TYPE1:      /* タイプ1：05ms */
         Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_BEEP_TIM, t_val );
         
         aud_beep_timer_id = CAUD_BEEP_BASE_05MS;
         break;
         
      case CAUD_BEEP_TIM_TYPE2:      /* タイプ2：10ms */
         Aud_set_timer( CAUD_CNT_10MS, CAUD_C10MS_BEEP_TIM, t_val );
         
         aud_beep_timer_id = CAUD_BEEP_BASE_10MS;
         break;
      
      case CAUD_BEEP_TIM_TYPE3:      /* タイプ3：Timer Out */
         aud_beep_timer_id = CAUD_BEEP_BASE_TIMOUT;
         break;
         
      case CAUD_BEEP_TIM_TYPE4:      /* タイプ4：05ms + Mute End */
         Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_BEEP_TIM, t_val );
         
         aud_beep_timer_id = CAUD_BEEP_BASE_05MS_MUTE_END;
         break;
         
      default:                  /* その他(停止命令がない限り止まらない */
         aud_beep_timer_id = CAUD_BEEP_BASE_NOP;
         break;
   }
   
   if ((( b_data & 0x0F ) > 0x00 ) && (( b_data & 0x0F ) <= 0x0F ))      /* ON設定のみBEEPを出力する */
   {
      Io_beep_start(); /* BEEP出力要求 */
   }
}

/******************************************************************************
   関数名      ：aud_off_beep
   引数      ：無し
   戻り値      ：無し
   機能概略   ：BEEP Off処理
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
static void Aud_beep_off( void )
{
   /* タイマクリア */
   Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_BEEP_TIM, 0 );
   Aud_set_timer( CAUD_CNT_10MS, CAUD_C10MS_BEEP_TIM, 0 );
   
   Io_beep_stop(); /* beep停止 */
}

/******************************************************************************
   関数名      ：Aud_set_alarm
   引数      ：BYTE   on_off      ON：有効
                            OFF：無効
   戻り値      ：無し
   機能概略   ：BEEP Alarm On/Off設定処理
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
static void Aud_set_alarm( BYTE on_off )
{
   /* 動作中では受付を禁止にする */
   if ( aud_beep_req_info.on_off != ON )
   {
      switch ( on_off )
      {
         case ON:
            aud_beep_req_info.alarm_onoff = ON;      /* Alarm ON      */
            break;

         case OFF:
            aud_beep_req_info.alarm_onoff = OFF;   /* Alarm Off   */
            break;

         default:
            break;
      }
   }
}

/******************************************************************************
   関数名      ：Aud_clear_beep_info
   引数      ：無し
   戻り値      ：無し
   機能概略   ：BEEP情報のクリア処理
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
static void Aud_clear_beep_info( void )
{
   memset(&aud_beep_req_info, 0x00, sizeof ( aud_beep_req_info ));
   
   aud_beep_timer_id = CAUD_BEEP_BASE_NOP;
}

/******************************************************************************
   関数名      ：Aud_set_stop_beep
   引数      ：無し
   戻り値      ：無し
   機能概略   ：BEEP停止設定処理
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
   【001】      2012/11/07   leejh      アラームOFF要求判断追加(Fail Safe)
 ******************************************************************************/
void Aud_set_stop_beep( void )
{
   /* アラーム OFF要求があった場合、一時要求を受け付けない */
   if( aud_beep_req_info.alarm_off_req == OFF )
   {
      /* beep停止   */
      Aud_beep_off();
      
      /* アラームOn中の場合 */
      if( aud_beep_req_info.alarm_onoff == ON ) 
      {
         /* アラーム OFF要求 */
         aud_beep_req_info.alarm_off_req = ON;
         
         /* アラームOFFシーケンスへ */
         aud_beep_req_info.cnt      = CAUD_SET_ALARM_OFF_IDX;
         aud_beep_req_info.alarm_cnt = CAUD_BEEP_ALARM_END_CNT;
         
         /* BEEP ON要求 */
         Aud_beep_on();
      }
      else
      {
         #if (_POWER_DIAG_ == 0)
         /* Alarm端子 OFF */
         Aud_beep_alarm_cont( OFF );   
         #endif
         
         /* Mute ON(端子制御) */
         Aud_beep_mute_cont( ON );
         
         /* beep情報クリア   */
         Aud_clear_beep_info();
      }
   }
}

/******************************************************************************
   関数名      ：Aud_get_beep_info
   引数      ：BYTE
   戻り値      ：無し
   機能概略   ：BEEPタイプ情報を取得する
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
BYTE Aud_get_beep_info( void )
{
   BYTE ret;
   
   ret = CAUD_BEEP_STA_OFF;            /* Fail Safe */
   
   if ( aud_beep_req_info.on_off == ON )   /* BEEP動作中のみ      */
   {
      ret = aud_beep_req_info.type;      /* 動作中のタイプを格納   */
   }
   
   return ( ret );
}
/******************************************************************************
   関数名      ：Aud_beep_time_set
   引数      ：BYTE req      CAUD_REQ_BEEP_LONGKEY   : Longkey BEEP
                        CAUD_REQ_BEEP_ALARM      : Alarm BEEP
                          CAUD_REQ_BEEP_MULTIPLE   : Multiple BEEP
   戻り値      ：無し
   機能概略   ：BEEP要求受付処理
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
void Aud_req_beep( BYTE req )
{
   BYTE t_size;
   BYTE b_type;
   
   if ( aud_beep_req_info.on_off != ON ) /* BEEP ON中では受け付けない */
   {
      /* 範囲外は受け付けない */
      if (( req > CAUD_REQ_BEEP_NOP ) && ( req < CAUD_REQ_BEEP_MAX ))
      {
         /* fail safe */
         b_type = CAUD_BEEP_STA_OFF;
         t_size = 0;
      
         /* fail safe */
         Aud_clear_beep_info();
         
         switch ( req )
         {
            case CAUD_REQ_BEEP_LONGKEY:      /* Long Key */
               /* テーブルサイズ取得 */
               t_size = sizeof( aud_BEEP_longkey_data_tbl );
               
               romcopy(( BYTE * ) & aud_beep_req_info.data[0],
                   ( BYTE * ) & aud_BEEP_longkey_data_tbl[0], t_size );
               break;

            case CAUD_REQ_BEEP_ALARM:      /* Alarm */
               /* テーブルサイズ取得 */
               t_size = sizeof( aud_BEEP_alarm_data_tbl );

               romcopy(( BYTE * ) & aud_beep_req_info.data[0],
                   ( BYTE * ) & aud_BEEP_alarm_data_tbl[0], t_size );
               
               Aud_set_alarm ( ON );         /* Alarm ON   */
               break;

            case CAUD_REQ_BEEP_MULTIPLE:      /* Multiple      */
               t_size = sizeof( aud_BEEP_multiple_data_tbl );
               
               romcopy(( BYTE * ) & aud_beep_req_info.data[0],
                   ( BYTE * ) & aud_BEEP_multiple_data_tbl[0], t_size );
               break;
               
            default:                     /* 例外は終了   */
               return;
         }
         
         aud_beep_req_info.on_off = ON;                     /* req beep on   */
         aud_beep_req_info.type    = Aud_set_beep_type( req );   /* beep type   */
         aud_beep_req_info.max    = t_size;                  /* set size      */
         
         Aud_beep_on();                                 /* beep on      */
      }
   }
}

/******************************************************************************
   関数名      ：Aud_beep_cont
   引数      ：無し
   戻り値      ：無し
   機能概略   ：BEEP ON/OFFを制御する関数
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
static void Aud_beep_cont( void )
{
   Aud_beep_off();            /* ONでもOFFでもOFF   */
   aud_beep_req_info.cnt++;   /* CNT UP   */

   /* マルチショットBEEP出力*/
   if ( aud_beep_req_info.cnt < aud_beep_req_info.max )
   {
      Aud_beep_on(); /* BEEP出力要求 */
   }
   /* MAXまで辿り着いたら? */
   else
   {
      /* ALARMは180回(180秒)まで */
      if ( aud_beep_req_info.alarm_onoff == ON )
      {
         aud_beep_req_info.alarm_cnt++;
         if ( aud_beep_req_info.alarm_cnt < CAUD_BEEP_ALARM_MAX_CNT )
         {
            /* アラームOn インデックスに変更 */
            aud_beep_req_info.cnt = CAUD_SET_ALARM_ON_IDX;
            
            Aud_beep_on(); /* BEEP出力要求 */
            return;
         }
         else
         {
            Alm_beep_on_stop();/* 180s経ったら、beep停止、Wake Up Off   */
            
#if (_POWER_DIAG_ == 0)
            Aud_beep_alarm_cont( OFF );   /* ALARM端子 OFF            */
#endif
         }
      }
      Aud_clear_beep_info(); /* 情報クリア */
   }
}

/******************************************************************************
   関数名      ：Aud_beep_main
   引数      ：無し
   戻り値      ：無し
   機能概略   ：BEEP制御メイン
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
void Aud_beep_main( void )
{
   switch ( aud_beep_timer_id )
   {
      case CAUD_BEEP_BASE_05MS:      /* 05ms */
         if ( Aud_get_timer( CAUD_CNT_05MS, CAUD_C05MS_BEEP_TIM ) == 0 )
         {
            aud_beep_timer_id = CAUD_BEEP_BASE_TIMOUT;   /* Timoutへ */
         }
         break;
         
      case CAUD_BEEP_BASE_10MS:      /* 10ms */
         if ( Aud_get_timer( CAUD_CNT_10MS, CAUD_C10MS_BEEP_TIM ) == 0 )
         {
            aud_beep_timer_id = CAUD_BEEP_BASE_TIMOUT;   /* Timoutへ */
         }
         break;
      
      case CAUD_BEEP_BASE_TIMOUT:      /* Time Out         */
         Aud_beep_cont();         /* BEEP Control      */
         break;
      
      case CAUD_BEEP_BASE_05MS_MUTE_END: /* 05ms + Mute End   */
         if ( Aud_get_timer( CAUD_CNT_05MS, CAUD_C05MS_BEEP_TIM ) == 0 )
         {
            /* Mute Off End */
            if( Aud_get_mute_status() == CAUD_MUTE_OFF_END )
            {
               /* LONGKEY(MUTE制御無し)の場合 */
               if(aud_beep_req_info.type == CAUD_BEEP_STA_LONGKEY_M_NCONT)
               {
                  /* MUTE OFF終了後、50ms経過していない */
                  if ( Aud_get_timer( CAUD_CNT_05MS, CAUD_C05MS_AMUTE_END_TIM ) != 0 )
                  {
                     /* MUTE OFF終了後のカウント値を格納 */
                     Aud_set_timer( CAUD_CNT_05MS, CAUD_C05MS_BEEP_TIM, 
                               Aud_get_timer( CAUD_CNT_05MS, CAUD_C05MS_AMUTE_END_TIM ) );
                     
                     /* 05msへ */
                     aud_beep_timer_id = CAUD_BEEP_BASE_05MS;
                  }
                  else
                  {
                     /* Timoutへ */
                     aud_beep_timer_id = CAUD_BEEP_BASE_TIMOUT;
                  }
               }
               else
               {
                  /* Timoutへ */
                  aud_beep_timer_id = CAUD_BEEP_BASE_TIMOUT;
               }
            }
         }
         break;
         
      case CAUD_BEEP_BASE_NOP:      /* NOP         */
      default:
         break;
   }
}

/******************************************************************************
   関数名      ：Aud_init_beep_func
   引数      ：無し
   戻り値      ：無し
   機能概略   ：BEEP制御の初期化処理
------------------------------------------------------------------------------
   修正履歴
    番号      日付      担当者      修正内容
   【000】      2012/07/13   leejh      新規作成
 ******************************************************************************/
void Aud_init_beep_func( void )
{
   aud_aud_info = Aud_get_audio_info_list();
   
   Aud_clear_beep_info();
}

#undef _AUD_BEEP_FUNC_C_
