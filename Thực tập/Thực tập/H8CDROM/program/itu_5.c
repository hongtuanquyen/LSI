//H8マイコンテストプログラム
//ITUによる2相トグル出力
//ITUによるトグル出力　1kHz
//GRAコンペアマッチ、TIOCA0に出力
//GRBコンペアマッチ、TIOCB0に出力、カウンタクリア
//内部クロック　1/8
//itu_5.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0x0c;           //ポートA PA2、PA3出力 TIOCA0、TIOCB0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x43;    //GRBコンペアマッチ、TCNTクリア、クロック1/8
    ITU0.TIOR.BIT.IOA = 3;   //GRAのコンペアマッチでTIOCA0にトグル出力
    ITU0.TIOR.BIT.IOB = 3;   //GRBのコンペアマッチでTIOCB0にトグル出力、カウンタクリア
    ITU0.GRA = 781;          //GRAの半分
    ITU0.GRB = 1562;         //コンペアマッチでカウンタクリア、周期0.5ms
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITUスタート
    while(1);                //何もしない
}
