//H8マイコンテストプログラム
//ポート4を入力に設定 押しボタンスイッチ
//ポート5を出力に設定 LED
//押しボタンスイッチS1（ポート4ビット4）が押されている間LED点滅
//離したらLED消灯
//p_7.c

#include <3048.h>

void wait(void)
{
    long loop = 200000;
    while(loop--);
}

int main(void)
{
    P4.DDR = 0;                //ポート4入力
    P4.PCR.BYTE = 0xff;        //プルアップMOS ON
    P5.DDR = 0xff;             //ポート5出力
    while(1){    
        if(!P4.DR.BIT.B4){     //スイッチが押されたかの判定、反転している
            P5.DR.BYTE = 0x0f; //LED点灯
            wait();
            P5.DR.BYTE = 0;
            wait();
        }
        else                   //スイッチが押されていない
            P5.DR.BYTE = 0;    //LED消灯
   }
}
