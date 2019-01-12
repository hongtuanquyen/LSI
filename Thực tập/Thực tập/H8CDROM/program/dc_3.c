//H8マイコンテストプログラム
//直流モータの回転角制御、10回転で停止
//パルスジェネレータをTCLKAに入力
//GRAコンペアマッチでモータ停止
//外部クロック　パルスジェネレータ信号
//dc_3.c

#include <3048.h>

int flg;                    //コンペアマッチで立てるフラグ

void ioinit(void)
{
    PA.DDR = 0x00;          //クロック入力
    PB.DDR = 0xff;          //全ビット出力 
}
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //コンペアマッチによる出力禁止
    ITU0.GRA =120;           //パルスジェネレータとコンペアマッチデータ、1回転12パルス
    ITU0.TCR.BYTE = 0x24;    //GRAコンペアマッチカウンタクリア、外部クロック
    ITU0.TIER.BIT.IMIEA = 1;
}

void int_imia0(void)
{
    flg = 1;                  //フラグを立てる
    ITU0.TIER.BIT.IMIEA = 0;  //IMFAフラグによる割り込み要求禁止
}

int main(void)
{
    ioinit();
    ituinit();
    flg = 0;                  //フラグ初期値
    ITU0.TIER.BIT.IMIEA = 1;  //IMFAによる割り込み要求を許可
    EI;
    ITU.TSTR.BIT.STR0 = 1;    //ITUスタート
    PB.DR.BYTE = 0x02;
    while(flg == 0);          //割り込みまで回転
    DI;                       //割り込み禁止
    PB.DR.BYTE = 0x00;        //停止
    while(1);                 //ここで止まっている
}
