//H8マイコンテストプログラム
//ポート4を入力に設定 押しボタンスイッチ
//ポート5を出力に設定 LED
//ポート4より入力したデータをポート5のLEDに出力
//p_6.c 

#include <3048.h>

int main(void)
{
    P4.DDR = 0;                               //ポート4入力
    P4.PCR.BYTE = 0xff;                       //プルアップMOS ON
    P5.DDR = 0xff;                            //ポート5出力
    while(1){    
        P5.DR.BYTE =~(P4.DR.BIT.B4 |          //入力したデータのORを取る
        P4.DR.BIT.B5 <<1 | P4.DR.BIT.B6 <<2 | //全体を反転する
        P4.DR.BIT.B7 << 3);                   //スイッチのビット分シフト
    }
}
