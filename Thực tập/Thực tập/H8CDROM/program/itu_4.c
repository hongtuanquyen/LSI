//H8マイコンテストプログラム
//ITUによるトグル出力､LED点滅
//外部クロック　1kHz
//GRAコンペアマッチ、TIOCA0に出力
//itu_4.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0x04;           //ポートA PA2出力､TIOCA0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x24;    //GRAコンペアマッチ、TCNTクリア、プリスケーラ
    ITU0.TIOR.BIT.IOA = 3;   //GRAのコンペアマッチでトグル出力、LED
    ITU0.GRA =499;           //1kHz 0.5s　周期1s　1Hz
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITUスタート
    while(1);                //何もしない
}
