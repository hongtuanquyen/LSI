//H8マイコンテストプログラム
//IRQ0割り込みテスト
//メインプログラム ポート5のLED点滅
//割り込みプログラム　ポート5LED交互点灯3回
//IRQ_0.c

#include <3048.h>

//点滅時間間隔
void wait (void)
{
    long t=200000;
    while (t--);
}

void int_irq0(void)            //割り込み関数
{
    int c=3;
    while(c--){                //3回繰り返し
        P5.DR.BYTE = 0x5;
        wait();
        P5.DR.BYTE = 0xa;
        wait();
    }
    INTC.ISR.BIT.IRQ0F = 0;//割り込みフラグクリア
}

int main(void)
{
    DI;                        //割り込み禁止
    INTC.ISCR.BIT.IRQ0SC = 1;  //入力の立ち下がりエッジで割り込み要求発生
    INTC.IER.BIT.IRQ0E = 1;    //IRQ0割り込みを許可
    P5.DDR = 0xff;             //P5を全ビット出力に設定
    EI;                        //割り込み許可
    while (1) {                //繰り返し実行
        P5.DR.BYTE = 0xff;     //点灯
        wait ();               //点灯時間
        P5.DR.BYTE = 0;        //消灯
        wait();                //消灯時間
    }
}
