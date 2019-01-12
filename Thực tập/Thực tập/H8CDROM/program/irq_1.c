//H8マイコンテストプログラム
//IRQ0割り込みテスト
//NMI+IRQ0
//メインプログラムポート5のLED点滅
//NMI割り込みLED全部の点灯、点滅時間の５倍
//割り込みプログラム　ポート5LED交互点灯5回
//IRQ_1.c

#include <3048.h>

// 点滅時間間隔
void wait (void)
{
    long t=200000;
    while (t--);
}

void int_nmi(void)              //NMI割り込み関数
{
    int c=5;
    P5.DR.BYTE = 0xf;           //全部の点灯
    while(c--)                  //点灯時間
        wait();
}

void int_irq0(void)             //IRQ0割り込み関数
{
    int c=5;
    while(c--){                 //3回繰り返し
        P5.DR.BYTE = 0x5;
        wait();
        P5.DR.BYTE = 0xa;
        wait();
    }
    INTC.ISR.BIT.IRQ0F = 0; //割り込みフラグクリア
}

int main(void)
{
    DI;
    INTC.ISCR.BIT.IRQ0SC = 1;
    INTC.IER.BIT.IRQ0E = 1;
    P5.DDR = 0xff;              //P5を全ビット出力に設定
    EI;
    while (1) {                 //繰り返し実行
        P5.DR.BYTE = 0xff;      //点灯
        wait ();                //点灯時間
        P5.DR.BYTE = 0;         //消灯
        wait();                 //消灯時間
    }
}
