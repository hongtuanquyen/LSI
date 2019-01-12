//H8マイコンテストプログラム
//タイマ割り込みによるステッピングモータの制御
//ITU0、ITU1割り込み使用、2個のモータ独立回転
//ITU0　5msごと割り込みで右モータ回転
//ITU1　10msごと割り込みで左モータ回転
//内部クロック　1/8
//stm_5.c

#include <3048.h>

int rmd = 0x01;
int lmd = 0x80;

void ioinit(void)
{
    PB.DDR = 0xff;           //ポートB全ビット出力
}

void ituinit()
{
    ITU0.TCR.BIT.CCLR = 1;   //GRAコンペアマッチ,カウンタクリア
    ITU0.TCR.BIT.TPSC = 3;   //タイマプリスケーラ　25MHz/8=3.125MHz
    ITU0.GRA = 15624;        //パルス周期3.125MHz/15625、5ms　　
    ITU0.TIER.BIT.IMIEA = 1; //割り込み許可
    ITU1.TCR.BIT.CCLR = 1;   //GRAコンペアマッチ、カウンタクリア
    ITU1.TCR.BIT.TPSC = 3;   //タイマプリスケーラ　25MHz/8=3.125MHz
    ITU1.GRA = 31249;        //パルス周期3.125MHz/31250、10ms　　
    ITU1.TIER.BIT.IMIEA = 1; //割り込み許可
}

void int_imia0(void)         //グループ2
{
    ITU0.TSR.BIT.IMFA = 0;   //ステータスフラグクリア
    PB.DR.BYTE = lmd + rmd;  //2個のモータデータ加えて出力
        rmd <<= 1;           //右回転
        if(rmd == 0x10)
            rmd = 0x01;
}

void int_imia1(void)         //グループ3
{
    ITU1.TSR.BIT.IMFA = 0;   //ステータスフラグクリア
    PB.DR.BYTE = lmd + rmd;
        lmd >>= 1;           //左回転
        if(lmd == 0x08)
            lmd = 0x80;
}
int main(void)
{
    ioinit();
    ituinit();
    EI;
    ITU.TSTR.BIT.STR0 = ITU.TSTR.BIT.STR1 = 1;     //ITU0、1スタート
    while(1);
}
