/******************************************************************************
..     株式会社クラリオン殿向け

      プロジェクト名   :AMFMラジオモデル
      ファイル名      : model.h
      機能         : SystemModel/VERSION定義ヘッダﾌｧｲﾙ
------------------------------------------------------------------------------
      修正履歴   【年月日】【Version】【名前】 【説明】
------------------------------------------------------------------------------
[chin20120719]新規作成
******************************************************************************/
#ifndef      _MODEL_H_
#define      _MODEL_H_

/*----------------------------------------------------
   Version 定義
----------------------------------------------------*/
#if 0
#define      _VERSION_         "01"            /*   1OTP,20120827 ハードチェック用   */
#define      _VERSION_         "02"            /*   2OTP,20120914 EEPROM書込み   */
#define      _VERSION_         "03"            /*   3OTP,20120919 SIO不具合対策   */
#define      _VERSION_         "04"            /*   4OTP,20120926   */
#define      _VERSION_         "06"            /*   6OTP,20121005 全機能対応   */
#define      _VERSION_         "07"            /*   7OTP,20121025 時計調整制御変更対応 */
#define      _VERSION_         "08"            /*   8OTP,20121116 テストモード仕様変更対応 */
#define      _VERSION_         "07"            /*   T07,20121122 テストモード仕様変更対応 */
#define      _VERSION_         "09"            /*   9OTP,20121214 POWER DIAG機能対応 */
#define      _VERSION_         "10"            /*   10OTP,20130118 不具合管理表No.15対応 */
#define      _VERSION_         "11"            /*   11OTP,20130215 不具合管理表No.17,18対応 */
#define      _VERSION_         "12"            /*   12OTP,20130308 不具合管理表No.19,20対応 */
#define      _VERSION_         "13"            /*   13OTP,20130322 第三地域仕向け,5k/9k混合SEEK,スピーカーLR反転対応 */
#define      _VERSION_         "14"            /*   14OTP,20130404 STチャタリング時間・表示処理変更対応 */
#define      _VERSION_         "15"            /*   15OTP,20130415 BEEP周波数変更対応 */
#endif

#define      _VERSION_         "16"            /*   16OTP,20130422 第三地域仕向けIFDetail設定値変更対応 */

/*----------------------------------------------------
   MODEL 定義
----------------------------------------------------*/
#define      _MODEL_TYPE_      0

/*----------------------------------------------------
   機能 Type 定義
----------------------------------------------------*/
#define      _MODEL_DISC_      0         /* CD/MP3 Model */
#define      _MODEL_USB_         0         /* USB Model */
#define      _MODEL_USBIPOD_      0         /* USBIPOD Model */
#define      _MODEL_RDS_         0         /* RDS Model */
#define      _MODEL_RADIO_      1         /* TUNER Model */
#define      _MODEL_BT_         0         /* BT Model       */
#define      _MODEL_SIRIUS_      0         /* ｼﾘｳｽ Model       */
#define      _MODEL_HD_RADIO_   0         /* HD-Radio Model    */
#define      _MODEL_EEPROM_      1         /* EEPROM Model */

#define      _STSW_KEY_         0         /*   Steering Key対応   */

#define      _AUDIO_DEBUG_      0

#define      _SYSTEM_DEBUG_      0      /*   システムデバッグ、ベースタイマーを無効   */

#define      _POWER_DIAG_      1      /*   POWER ICのDIAG機能   */

#define      _TEST_ALARM_ON_      0      /*   アラーム鳴動テスト   */
/*-----------------------------------------------------
   テストモード定義
------------------------------------------------------*/
#define      _TEST_MODE_         1      /*   テストモード機能   */

/*-----------------------------------------------------
   HOLDXモード定義
------------------------------------------------------*/
#define      _HOLDX_            1

/*-----------------------------------------------------
   WDT定義
------------------------------------------------------*/
#define      _WDT_            1         /* 1:WDT有効   WDT Enable        */
#define      _WDT_TEST_         0         /* 1:WDT テスト版ソフト有効 */

#endif
