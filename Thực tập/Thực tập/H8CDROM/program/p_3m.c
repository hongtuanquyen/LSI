//H8マイコンテストプログラム
//ポート4を入力に設定 押しボタンスイッチ
//ポート5を出力に設定 LED
//ポート4より入力したデータをポート5に出力
//p_33m.c 

#include  "io.h"         //I/Oアクセス用インクルード

int main(void)
{
    P4DDR = 0;           //ポート4入力
    P4PCR = 0xff;        //プルアップMOS ON
    P5DDR = 0xff;        //ポート5出力
    while(1)    
        P5DR = P4DR >> 4;//データ入力4ビット右にシフト、出力
}
