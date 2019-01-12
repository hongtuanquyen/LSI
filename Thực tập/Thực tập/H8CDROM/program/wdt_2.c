//H8マイコンテストプログラム
//WDTテスト
//8ビットタイマとしての使用
//タイマ割り込みでLED点滅 約42ms
//wdt_2.c

#include <3048.h>

void ioinit(void)
{
    PB.DDR = 0xff;                 //ポートB全ビット出力
}

void wdtinit(void)
{
    WDT.WRITE.TCSR = 0xa527;       //インターバルタイマ選択カウント動作、1/4096
}

void int_wovi(void)                //WDTタイマ割り込み
{
    int dummy;                     //読み込みのダミー変数
    static unsigned char d = 0xff; //LED出力データ初期値
    dummy =  WDT.WRITE.TCSR;       //TCSRレジスタを読む
    WDT.WRITE.TCSR = 0xa527;       //割り込みフラグのクリア
    PB.DR.BYTE = d;                //データ出力
    d = ~d;                        //データ反転
}

int main(void)
{
    ioinit();
    wdtinit();
    EI;
    while(1);
}
