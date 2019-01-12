//タイマ割り込み　実験2
//ITU0、ITU1使用　
//ITU0　20msごと割り込み、50回の割り込みでビット0LED点灯反転
//ITU1　10msごと割り込み、50回の割り込みでビット1LED点灯反転
//itu_2.c

#include  <3048.h>                    //3048、3052の内部I/O定義

int c0,c1;                            //割り込み回数、外部変数とする

//ITU0インターバルタイマ割り込み
void int_imia0 (void)
{
    ITU0.TSR.BIT.IMFA = 0;            //割り込みステータスフラグクリア
    c0-=1;
    if(c0==0){
        P5.DR.BIT.B0 = ~P5.DR.BIT.B0; //LED出力データを反転
        c0=50;
    }
}

//ITU1インターバルタイマ割り込み
void int_imia1 (void)
{
    ITU1.TSR.BIT.IMFA = 0;             //割り込みステータスフラグクリア
    c1-=1;
    if(c1==0){
        P5.DR.BIT.B1 = ~P5.DR.BIT.B1;  //LED出力データを反転
        c1=50;
    }
}

int main(void)
{
    P5.DDR = 0xff;
    ITU0.TCR.BIT.CCLR    = 1;          //カウンタクリア要因
    ITU0.TCR.BIT.TPSC    = 3;          //タイマプリスケーラ25MHz/8=3.125MHz
    ITU0.GRA        =62499;            //3.125MHz/62,500=50Hz、周期20ms
    ITU0.TIER.BIT.IMIEA = 1;           //IMFAフラグによる割り込み許可
    ITU1.TCR.BIT.CCLR    = 1;          //カウンタクリア要因
    ITU1.TCR.BIT.TPSC    = 3;          //タイマプリスケーラ25MHz/8=3.125MHz
    ITU1.GRA        =31249;            //3.125MHz/31,250=100Hz、周期10ms
    ITU1.TIER.BIT.IMIEA = 1;           //IMFAフラグによる割り込み許可
    ITU.TSTR.BIT.STR0 = 1;             //タイマ0スタート
    ITU.TSTR.BIT.STR1 = 1;             //タイマ1スタート
    P5.DR.BYTE = 0x03;                 //LED出力データ初期値
    c0 = c1 = 50;                      //割り込み回数
    EI;                                //割り込み許可
    while (1);                         //何もしない
}
