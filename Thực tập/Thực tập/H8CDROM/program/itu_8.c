//ITU位相計数モード
//ロータリエンコーダよりのパルスを計測しPB、P4に出力
//ロータリエンコーダのパルスはTCLKA(PA0)、TCLKB(PA1)に入力　
//P4上位8ビット出力、PB下位8ビット出力
//LEDの点灯で確認する
//itu_8.c
#include  <3048.h>         //3048、3052の内部I/O定義

void ituinit(void)
{
    ITU.TMDR.BIT.MDF = 1;  //ITU2は位相計数モード
    ITU.TMDR.BIT.FDIR = 1; //オーバフロー、アンダフローでフラグセット
}

void ioinit(void)
{
    PB.DDR = 0xff;         //ポートB出力
    P4.DDR = 0xff;         //ポート4出力     
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR2 = 1;          //タイマスタート
    while(1){
        P4.DR.BYTE = ITU2.TCNT >> 8;//TCNTの上位8ビットを出力
        PB.DR.BYTE = ITU2.TCNT;     //TCNTの下位8ビットを出力
    }
}
