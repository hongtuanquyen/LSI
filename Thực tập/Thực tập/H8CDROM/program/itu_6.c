//H8マイコンテストプログラム
//ITUによるPWM出力
//GRAコンペアマッチ、TIOCA0に“1”出力、カウンタクリア
//GRBコンペアマッチ、TIOCA0に“0”出力
//内部クロック　1/8
//itu_6.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0x04;           //ポートA PA2出力､TIOCA0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x23;    //GRAコンペアマッチ、TCNTクリア、クロック1/8
 
   
    ITU.TMDR.BIT.PWM0 = 1;   //チャネル0　PWMモード
    ITU0.GRA = 1562;         //パルス幅　2kHz　0.5ms
    ITU0.GRB = 312;          //“H”レベルパルス幅　1/5
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITUスタート
    while(1);                //何もしない
}
