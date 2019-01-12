//H8マイコンテストプログラム
//WDTテスト
//mainプログラム　ポートB LED交互点灯
//ポートA　ビット0　“0”でWDTカウンタクリア
//ポートA　ビット0　“1”でWDTカウンタクリアしない。
//WDTオーバフローでリセット、プログラム進まない
//wdt_1.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0;               //ポートA入力
    PB.DDR = 0xff;            //ポートB出力
}

void wdtinit(void)
{
    WDT.WRITE.RSTCSR = 0x5a40;//リセット出力イネーブル
    WDT.WRITE.TCSR = 0xa566;  //WDT選択カウント動作、1/2048
}

int main(void)
{
    long c =80000;
    unsigned char d = 0x55;
    ioinit();
    PB.DR.BYTE = 0x00;              //最初の実行、LED消灯
    wdtinit();
    while(1){
        c=c-1;
        if(PA.DR.BIT.B0 ==0)        //スイッチOFFでWDTクリア
            WDT.WRITE.TCSR = 0x5a00;//WDTタイマカウンタクリア、TCSRと同じアドレス
        if(c == 0){                 //WDTオーバフローでこの中は実行されない
            PB.DR.BYTE = d;
            d = ~d;
            c = 80000;
        }
    }
}
