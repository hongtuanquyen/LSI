/******************************************************************************
..     クラリオン殿向け                    2007ﾓﾃﾞﾙ

      プロジェクト名   :
      ファイル名      : tu_def.h
      機能         : ①Tunerﾓｼﾞｭｰﾙ型、定数定義ﾍｯﾀﾞﾌｧｲﾙ
------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[sun051125]   新規作成
[sun051220] TTU_BAND定義ﾐｽ修正、ﾗｽﾄ入れると7局分ですが、6局分しか定義していなかった
[zhang070313] clarion 対応修正
[matu120903] S-meter判定 Local値追加 
[matu120926] テストモード対応
******************************************************************************/
#ifndef      _TU_DEF_H_
#define      _TU_DEF_H_

#include   "../model.h"

/*----------------------------------------------------
   定数定義
----------------------------------------------------*/

#define      CTU_E_TEST         0x00         /*   TEF6606初期値のE向けテスト用   */

#if   (CTU_E_TEST == 0x01)
#define      CTU_DEST   8            /* Tuner仕向け */
#else
#define      CTU_DEST   7            /* Tuner仕向け */
#endif
#define      CTU_BAND   3            /* Band 3Band */
#define      CTU_PRESET   6            /*Preset 6局分*/

#define      CTU_MW      0            /*MW Band*/
#define      CTU_LW      1            /*LW Band*/

#define      CTU_FMLW   1            /* FM LW Bandあり */
#define      CTU_AMLW   2            /* AM LW Bandあり */

#define      CTU_DEST_JPN_A      0      /* 日本仕向け AMラジオ */
#define      CTU_DEST_JPN_FA      1      /* 日本仕向け AM/FMラジオ */
#define      CTU_DEST_USA      2      /* 北米/中南米仕向け */
#define      CTU_DEST_CSA      3      /* 中南米仕向け */
#define      CTU_DEST_OCA      4      /* オセアニア仕向け */
#define      CTU_DEST_AAM      5      /* アジア/アフリカ/中東仕向け */
#define      CTU_DEST_THR      6      /* 第三地域仕向け */
#define      CTU_DEST_MAX      7      /* MAX */

#if   (CTU_E_TEST == 0x01)
#define      CTU_DEST_E   7
#endif

#define      CTU_DOWN   0
#define      CTU_UP      1
#define      CTU_PREMUTE         50/5+1            /* Tuner PreMute 50ms timer */
#define      CTU_SEEKMUTE      100/5+1            /* Tuner Seek PreMute 100ms timer */
#define      CTU_PWRWAIT         800/5+1            /* PowerOn後PLLｾｯﾄまでの安定800mstimer */
#define      CTU_SCANWAIT_TIM   5000/100         /* SCAN受信時間 5s timer */
#define      CTU_PSTWAIT_TIM      10000/100         /* Preset 書き込み待ち時間 10s timer */

#define      CTU_AFTER100   11         /* AfterMute100mstimer:100/10+1 */
#define      CTU_AFTER250   250/10+1   /* AfterMute100mstimer:250/10+1 */
#define      CTU_AFTER300   31         /* AfterMute300mstimer:300/10+1 */
#define      CTU_AFTER500   500/10+1         /* AfterMute500mstimer */
#define      CTU_AFTER1000   101         /* AfterMute100mstimer:1000/10+1 */
#define      CTU_AFTER1500   1500/10+1         /* AfterMute1500mstimer */


#define      CTU_NOTEDGE         0
#define      CTU_BNDEDGE         1               /* Tuner Up/Down BandEdge */
#define      CTU_LMEDGE         2               /* Tuner Up/Down LW/MW Edge */

/* TESTMODE対応 *//* [matu120926] */
/* LO/DX切替変数使用 */
#define      CTU_TEST_NO         0
#define      CTU_TEST_LOCAL      1               /* DX/LO 切替 LOCAL設定要求 */
#define      CTU_TEST_DX         2               /* DX/LO 切替 DX設定要求 */
/* Q-INFOチェック回数 */
#define      CTU_SEEK_NUM      5
#define      CTU_TEST_NUM      100
/*↑TESTMODE対応*//* [matu120926] */

#define      CTU_I2C_STOP      0
#define      CTU_I2C_IDLE      1
#define      CTU_I2C_BUSY      2
#define      CTU_I2C_ERR         3

#define      CTU_PARA_TUNER0         0x00
#define      CTU_PARA_TUNER1         0x01
#define      CTU_PARA_TUNER2         0x02
#define      CTU_PARA_RADIO         0x03
#define      CTU_PARA_SOFTMUTE0      0x04
#define      CTU_PARA_SOFTMUTE1      0x05
#define      CTU_PARA_SOFTMUTE_AM   0x06
#define      CTU_PARA_HIGHFCUT0      0x07
#define      CTU_PARA_HIGHFCUT1      0x08
#define      CTU_PARA_HIGHFCUT2      0x09
#define      CTU_PARA_STEREO0      0x0A
#define      CTU_PARA_STEREO1      0x0B
#define      CTU_PARA_STEREO2      0x0C
#define      CTU_PARA_CONTROL      0x0D
#define      CTU_PARA_LEVEL_OFFSET   0x0E
#define      CTU_PARA_AM_LNA         0x0F
#define      CTU_PARA_LEN         0x10

#if (_MODEL_EEPROM_ == 1)
#define      CTU_EEPWR_PST         1

#define      CTU_EEPOFS_PREMEM         0x20   /*   Tuner二週間メモリデータ保存領域   */
#if 0
#define      CTU_EEPADR_PSTLEN         36      /* 2byte * 6pch * 3band */
#else
#define      CTU_EEPADR_PSTLEN         36      /* 16BYTE * 3行 */
#endif

#define      CTU_EEPADR_PARA            0x60         /*   Tuner各仕向け初期コマンド保存領域開始アドレス   */

#define      CTU_EEPOFS_CHKCOEF         (CTU_PARA_LEN * (2) * CTU_DEST_MAX)
#define      CTU_EEPOFS_FM20DB         CTU_EEPOFS_CHKCOEF
#define      CTU_EEPOFS_FM40DB         (CTU_EEPOFS_FM20DB + 1)
#define      CTU_EEPOFS_AM30DB         (CTU_EEPOFS_FM20DB + 9)
#define      CTU_EEPOFS_AM50DB         (CTU_EEPOFS_AM30DB + 1)

/*****   EEPROM ｱﾄﾞﾚｽ値 ******/
#define      CTU_EEP_ADR_FMSMETER   0x00         //FM Sﾒｰﾀ(DSP-PJ発行仕様書上のｱﾄﾞﾚｽ)
#define      CTU_EEP_ADR_FMSMETER20DB 0x00         //FM 20dB時のSﾒｰﾀ(DSP-PJ発行仕様書上のｱﾄﾞﾚｽ)
#define      CTU_EEP_ADR_FMSMETER40DB 0x01         //FM 40dB時のSﾒｰﾀ(DSP-PJ発行仕様書上のｱﾄﾞﾚｽ)

#define      CTU_EEP_ADR_AMSMETER   0x02         //AM Sﾒｰﾀ(DSP-PJ発行仕様書上のｱﾄﾞﾚｽ)
#define      CTU_EEP_ADR_AMSMETER20DB 0x02         //AM 30dB時のSﾒｰﾀ(DSP-PJ発行仕様書上のｱﾄﾞﾚｽ)
#define      CTU_EEP_ADR_AMSMETER40DB 0x03         //AM 50dB時のSﾒｰﾀ(DSP-PJ発行仕様書上のｱﾄﾞﾚｽ)

#define      CTU_EEP_ADR_AMSMLOLV   0x06         /* [matu120903] */
#define      CTU_EEP_ADR_AMSMDXLV   0x07
#define      CTU_EEP_ADR_FMSMDXLV   0x09

#define      CTU_EEP_ADR_FMWAM      0x0B
#define      CTU_EEP_ADR_FMUSN      0x0C
/* EEPROMテーブル先頭バッファ */
#define      CTU_EEP_ADR_Q_INF1      0x00         /* アライメント、DX、WAN、USN設定 */
#define      CTU_EEP_ADR_PARA_FMU   0x10         /* FM USAパラメータ設定 */
#define      CTU_EEP_ADR_PARA_FMJ   0x20         /* FM JAPANパラメータ設定 */
#define      CTU_EEP_ADR_PARA_AM      0x30         /* AM JAPAN/USAパラメータ設定 */
#define      CTU_EEP_ADR_Q_INF2      0x40         /* IF-COUNT、WAIT TIMER設定 */
#define      CTU_EEP_ADR_Q_INF3      0x50         /* S-meter、noise設定 */
/* Check Sam */
#define      CTU_EEP_ADR_Q_INF1_CHK      BIN_00000001
#define      CTU_EEP_ADR_PARA_FM_CHK      BIN_00000010
#define      CTU_EEP_ADR_PARA_AM_CHK      BIN_00000100
#define      CTU_EEP_ADR_Q_INF2_CHK      BIN_00001000
#define      CTU_EEP_ADR_Q_INF3_CHK      BIN_00010000

#define      CTU_EEP_ADR_AM_IFROUGH   0x43
#define      CTU_EEP_ADR_AM_IFDETAIL   0x44
#define      CTU_EEP_ADR_FM_IFROUGH   0x45
#define      CTU_EEP_ADR_FM_IFDETAIL   0x46

#define      CTU_EEP_ADR_FM_Q_WAIT   0x47
#define      CTU_EEP_ADR_AM_Q_WAIT   0x48

#define      CTU_EEP_ADR_AM_PERC_LV   0x51
#define      CTU_EEP_ADR_FM_PERC_LV   0x52
#define      CTU_EEP_ADR_FM_PERC_NS   0x53

#endif
/*----------------------------------------------------
   型定義
----------------------------------------------------*/
typedef union {
   WORD   word;
   struct
   {
      BYTE   byte0;
      BYTE   byte1;
   }byte;
}WORD_BYTE;

/* Tuning係数定義 */
typedef  struct {
   WORD   min_freq;         /* 最小周波数 FM単位10kHz  WB/AM単位kHz */
   WORD   max_ch;            /* Maxﾁｬﾝﾈﾙ数 */
   WORD   min_ch;            /* Minﾁｬﾝﾈﾙ */
   BYTE   step;            /* ｽﾃｯﾌﾟ 単位：kHz */
   BYTE   wait_edge;         /* Waitﾀｲﾏ[ﾊﾞﾝﾄﾞｴｯｼﾞ] 単位：5ms */
   BYTE   c_wait;            /* 連続ｱｯﾌﾟﾀﾞｳﾝWAITﾀｲﾏ */
   }TTU_INIT;

/* 仕向け毎のTuning係数定義 */
typedef struct {
   BYTE      mode;
   BYTE      dummy;
   TTU_INIT   fm;
   TTU_INIT   fm_lw;
   TTU_INIT   am;
   TTU_INIT   am_lw;
   }TTU_DEST;

/* 周波数基本情報定義 */
typedef struct {
   WORD   freq;            /* 周波数 */
   }TTU_CH;

/* Band毎の基本情報定義 */
/* [sun051220]定義ﾐｽ修正、ﾗｽﾄ入れると7局分ですが、6局分しか定義していなかった */
typedef struct {
   TTU_CH   pset[CTU_PRESET+1];   /* 0: ﾗｽﾄ周波数エリア、 1～6：Presetエリア      0: last frequency area, 1 to 6: Preset area */
   BYTE   pstno;            /* BandﾗｽﾄPresetNo */
   BYTE   bno;            /* Band No */
   BYTE   band;            /* FM/AM情報 */
   }TTU_BAND;

/* Controlﾃｰﾌﾞﾙ定義 */
typedef  struct{
   BYTE (*func)(void);
   BYTE ok_seq;
   BYTE ng_seq;
   }TTU_CONTTBL;

/* 通信基本情報定義 */
typedef union {
   BYTE   subaddr;
   struct
   {
      BYTE   addr:4;
      BYTE   dumy:1;
      BYTE   mode:3;
   }bit;
}TRAS_MSA;

typedef struct {   
   BYTE txbuf[16];      /*送信buffer         */
   BYTE rxbuf[5];      /*受信buffer         */
   TRAS_MSA msa;         /*I2C送信のMSA         */
   BYTE length;      /*read/writeデータ長   */
   BYTE status;      /*I2C状態            */
   }TTU_I2C;

/*   Average S-Meter Check↓   */
typedef struct {
   BYTE   Status;      //   準備完了ｽﾃｰﾀｽﾃﾞｰﾀ
   BYTE   Smeter;      //   電界強度ﾃﾞｰﾀ
   BYTE   Mnoise:4;   //   ﾏﾙﾁﾊﾟｽﾉｲｽﾞﾃﾞｰﾀ
   BYTE   Rnoise:4;   //   隣接ﾉｲｽﾞﾃﾞｰﾀ
   BYTE   IFval:5;
   BYTE   IFneg:1;
   BYTE   IFsts:2;   //   IFCOUNTER Status
   BYTE   Bwidth;      //   IF帯域幅ﾃﾞｰﾀ
} TTU_QINFO,*PTTU_QINFO;

typedef struct {
   BYTE   cnt;            //   計測回数 1:計測完了
   BYTE   data;            //   累積ﾃﾞｰﾀ
   BYTE   rem;            //   計算剰余
} TTU_QI_CALC;

typedef union {
   WORD   word;
   BYTE   byte[2];
} CHG_WORD;

typedef   struct {
   BYTE   sm_20db;
   BYTE   sm_40db;
   BYTE   sm_lodb;   /* [matu120903] */
   BYTE   sm_dxdb;
   BYTE   if_rough;
   BYTE   if_detail;
   BYTE   wait_tm;
   BYTE   prec_sm;
   BYTE   prec_noise;
   BYTE   usn_max;
   BYTE   wam_max;
} TTU_CHKCOEF;

/*   Average S-Meter Check↑   */
   
/*----------------------------------------------------
   Mainﾓｰﾄﾞ定義

★★TTU_CONTTBL   * _rom TU_SeqTBL[]の順番と連動しています、
変更時に必ず確認事
----------------------------------------------------*/
enum
{
   CTU_NOP   = 0,         /* 0 */
   CTU_M_PWRON,         /* 1 */
   CTU_M_LSTCAL,         /* 2 */
   CTU_M_BANDCHG,         /* 3 */
   CTU_M_MAN,            /* 4 */
   CTU_M_CMAN,            /* 5 */
   CTU_M_SEEK,            /* 6 */
   CTU_M_PSTRD,         /* 7 */
   CTU_M_PSTWT,         /* 8 */
   CTU_M_AS,            /* 9 */
   CTU_M_SCAN,            /* a */
   CTU_M_ISRRD,         /* b */
   CTU_M_MAX            /* c */
};

/*----------------------------------------------------
   Mainモード内シーケンス定義
----------------------------------------------------*/
#define      CTU_SCANWAIT_SEQ      0x05         /* SCAN5s受信中*/
#endif
