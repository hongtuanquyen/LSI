//H8マイコンテストプログラム
//ITUによるトグル出力、周波数1kHz　周期1ms、半サイクル分で0.5ms
//GRAコンペアマッチ、TIOCA0に出力
//itu_3.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0x04;           //ポートA PA2出力､TIOCA0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x23;    //GRAコンペアマッチ、TCNTクリア、プリスケーラ1/8
    ITU0.TIOR.BIT.IOA = 3;   //GRAのコンペアマッチでトグル出力
    ITU0.GRA = 1562;         //25MHz/8 0.32μs　周期0.5ms　0.5/0.00032=1562.5
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITUスタート
    while(1);                //何もしない
}
