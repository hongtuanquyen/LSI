//タイマ割り込み
//ITU0使用　
//20msごとに割り込み、50回の割り込みでLED点灯反転
//itu_1.c

#include  <3048.h>                //3048、3052の内部I/O定義

int c;                            //割り込み回数、外部変数とする

//ITUインターバルタイマ割り込み
void int_imia0 (void)
{
    ITU0.TSR.BIT.IMFA = 0;        //割り込みステータスフラグクリア
    c-=1;
    if(c==0){
        P5.DR.BYTE = ~P5.DR.BYTE; //LED出力データを反転
        c=50;
    }
}

int main(void)
{
    P5.DDR = 0xff;
    ITU0.TCR.BIT.CCLR    = 1;     //カウンタクリア要因
    ITU0.TCR.BIT.TPSC    = 3;     //タイマプリスケーラ25MHz/8=3.125MHz
    ITU0.GRA        =62499;       //3.125MHz/62,500=50Hz、周期20ms
    ITU0.TIER.BIT.IMIEA = 1;      //IMFAフラグによる割り込み許可
    ITU.TSTR.BIT.STR0 = 1;        //タイマスタート
    P5.DR.BYTE = 0xff;            //LED出力データ初期値
    c=50;                         //割り込み回数
    EI;                           //割り込み許可
    while (1);                    //何もしない
}
