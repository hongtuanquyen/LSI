//H8マイコンテストプログラム
//ポート4を入力に設定 押しボタンスイッチ
//ポート5を出力に設定 LED
//押しボタンスイッチS1（ポート4ビット4）が押されている間LED点滅
//離したらLED消灯
//p_5.c

#include "io.h"

void wait(void)
{
    long loop = 200000;
    while(loop--);
}

int main(void)
{
    unsigned char sw;  
    P4DDR = 0;           //ポート4入力
    P4PCR = 0xff;        //プルアップMOS ON
    P5DDR = 0xff;        //ポート5出力
    while(1){    
        sw = ~P4DR;      //データ入力、“1”、“0”を反転する
        if(sw & 0x10){   //スイッチが押されたかの判定
            P5DR = 0x0f; //LED点灯
            wait();
            P5DR = 0;
            wait();
        }
        else             //スイッチが押されていない
            P5DR = 0;    //LED消灯
    }
}
