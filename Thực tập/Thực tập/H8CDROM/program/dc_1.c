//H8マイコンテストプログラム
//直流モータの制御
//ポートAの入力で制御する
//10:右回転、01:左回転、11:ブレーキ停止、00:ハイインピーダンス停止
//dc_1.c

#include <3048.h>

void ioinit(void)
{
     PA.DDR = 0x00;              //ポートA制御入力
     PB.DDR = 0xff;              //ポートBモータ出力
}

int main(void)
{
    ioinit();
    while(1)   
        PB.DR.BYTE = PA.DR.BYTE; //制御信号を出力
}
