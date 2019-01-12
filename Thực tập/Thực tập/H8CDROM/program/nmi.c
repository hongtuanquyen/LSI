//H8マイコンテストプログラム
//NMI割り込みテスト
//メインプログラムポート5のLED点滅
//割り込みプログラム　ポート5LED交互点灯3回
//nmi.c

#include <3048.h>

//点滅時間間隔
void wait (void)
{
    long t=200000;
    while (t--);
}

void int_nmi(void)          //割り込み関数
{
    int c=3;
    while(c--){             //3回繰り返し
        P5.DR.BYTE = 0x5;   //1個おき点灯
        wait();
        P5.DR.BYTE = 0xa;   //点灯LED切り替え
        wait();
    }
}

int main(void)
{
    SYSCR.BIT.NMIEG = 0;    //割り込み要求信号立ち下がりエッジ設定
    P5.DDR = 0xff;          //P5を全ビット出力に設定
    while (1) {             //繰り返し実行
        P5.DR.BYTE = 0xff;  //点灯
        wait ();            //点灯時間
        P5.DR.BYTE = 0;     //消灯
        wait();             //消灯時間
    }}
