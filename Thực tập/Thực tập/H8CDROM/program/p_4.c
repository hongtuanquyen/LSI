//LEDの点灯移動
//ポートA、B、4に接続されたLEDを右から左に点灯移動する
//p_4.c

#include  "io.h"       //3048、3052の内部I/O定義

void wait(void)        //時間稼ぎ関数、受け取る引数なし
{
    long t=400000;     //時間を決める数値、関数内で定義
    while(t--);
}

int main(void)
{
    int d = 0x01;       //LED出力初期値
    PADDR = 0xff;       //PAを全ビット出力に設定
    PBDDR = 0xff;       //PAを全ビット出力に設定
    P4DDR = 0xff;       //PAを全ビット出力に設定
    while(1){
        PADR = d;       //LEDに出力
        PBDR = d;
        P4DR = d;
        wait();         //時間、渡す引数なし
        d=d<<1;         //左に1ビットシフト
        if(d == 0x100)  //8回シフトしたかの判定
            d = 1;      //8回シフトしたら1を再設定
    }
}
