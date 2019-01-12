//H8マイコンテストプログラム
//wait関数時間テスト
//wait_1.c 

#include <3048.h>

void wait(void)    //モータパルス間隔
{
    int t=20000;   //この数値で回転速度が決まる
    while(t--);
}

int main(void)
{
    PB.DDR = 0xff;
    while(1){
        PB.DR.BYTE = 0xff;
        wait();
        PB.DR.BYTE = 0x00;
        wait();
    }
}
