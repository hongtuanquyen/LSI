/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

      プロジェクト名   :
      ファイル名      : ma_ext.h
      機能         : Systemﾒｲﾝﾙｰﾌﾟ処理
------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]   新規作成
******************************************************************************/
#ifndef   _MA_EXT_H_
#define   _MA_EXT_H_

#include   "../model.h"

#undef   EXT
#ifdef   _MA_EXT_C_
#define   EXT
#else
#define   EXT  extern
#endif

/*----------------------------------------------------
   関数プロトタイプ
----------------------------------------------------*/
EXT   void   Ma_set_Msg(BYTE ,BYTE);      /* Message登録処理 */
EXT void    Ma_key_src(void);
EXT   void    Ma_mode_event(BYTE , BYTE, BYTE );
EXT   BYTE    Ma_current_mode(void);
EXT   BYTE    Ma_model_dest(void);
EXT   BYTE    Ma_target_mode(void);
EXT   void   Ma_ini_data_set(BYTE *chk);
EXT   BYTE    Ma_get_clock(void);
EXT   BYTE    Ma_get_alarm(void);
EXT BYTE   Ma_is_mode_chg(void);
EXT BYTE    Ma_Chk_EEP_Initial(void);
EXT void   Ma_part_num_get(BYTE * n_buf);

#if _MODEL_EEPROM_
EXT   void Ma_SaveEepData(BYTE *BufAddr);
#endif

#if _TEST_MODE_
void   Ma_Test_Func(BYTE ,BYTE *,BYTE);
#endif

/*----------------------------------------------------
   Model定義
[sun070310]C社機種名対応
----------------------------------------------------*/
#define      CMA_POWER    0
#define      CMA_MODE   1
#define      CMA_TIMER   2

enum
{
   CMA_MODEL_0 = 0,                           /* 仕向け無し */
   CMA_MODEL_1 = 1,
   CMA_MODEL_AM_J = CMA_MODEL_1,      /* AMラジオ      国内 */
   CMA_MODEL_2 = 2,
   CMA_MODEL_AMFM_J = CMA_MODEL_2,      /* AMFMラジオ   国内 */
   CMA_MODEL_3 = 3,
   CMA_MODEL_AMFM_U = CMA_MODEL_3,      /* AMFMラジオ   北米/中南米 */
   CMA_MODEL_4 = 4,
   CMA_MODEL_AMFM_B = CMA_MODEL_4,      /* AMFMラジオ   中南米 */
   CMA_MODEL_5 = 5,
   CMA_MODEL_AMFM_T = CMA_MODEL_5,      /* AMFMラジオ   オセアニア */
   CMA_MODEL_6 = 6,
   CMA_MODEL_AMFM_A = CMA_MODEL_6,      /* AMFMラジオ   アジア/アフリカ/中東 */
   CMA_MODEL_7 = 7,
   CMA_MODEL_AMFM_X = CMA_MODEL_7      /* AMFMラジオ   第三地域 */
};


/* Mainﾓｰﾄﾞ定義 */
enum
{
   CMA_TUNER = 1,               /* Tuner Mode */
   CMA_AUX,                  /* AUX Mode */
   CMA_ISR                  /* ISR Mode */
};
#define      CMA_MODE_MAX         CMA_ISR

/* Mainﾓｰﾄﾞ制御ﾊﾞｯﾌｧID定義 */
enum
{
   CMA_ID_TU = 0,               /* TunerID「TunerMode/HD Radio」登録可能 */
   CMA_ID_AUX1,
};
#define      CMA_ID_MAX         CMA_ID_AUX1
#define      CMA_ID_SRCMAX      CMA_ID_AUX1         /* ｿｰｽｷｰで切替ﾓｰﾄﾞMAX値    Gia tri MAX cua mode chuyen doi khi nhan nut SRC KEY*/

enum
{
   CMA_NOCONN = 0,
   CMA_CONN,
   CMA_LOAD,
   CMA_MODEON,
   CMA_MODEOFF
};
#define      CMA_EVENT_MAX   CMA_MODEOFF

enum
{
   CMA_INIT_STA_ALL_NG,      /* 全初期値NG */
   CMA_INIT_STA_AREA_NG,      /* area値NG */
   CMA_INIT_STA_CKAL_NG,      /* clock、alarm値NG */
   CMA_INIT_STA_ALL_OK         /* 全初期値OK */
};

typedef struct{
   BYTE fac1[2];
   BYTE area;
   BYTE clock;
   BYTE alarm;
   BYTE fac2[2];
   BYTE ini_status;
}MA_INI_DATA;

EXT MA_INI_DATA ma_ini_data;
EXT BYTE ma_part_num[5];

#endif
