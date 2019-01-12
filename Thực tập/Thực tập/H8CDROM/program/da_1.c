//D/Aコンバータテストプログラム
//ディップスイッチP2を入力しこのデータのアナログ信号をDA0に出力する
//da_1.c
#include <3048.h>

int main(void)
{
    P2.DDR = 0;                 //P2全ビット入力
    P2.PCR.BYTE - 0xff;         //P2MOSプルアップ
    DA.DACR.BIT.DAOE0 = 1;      //DA0使用
    DA.DACR.BIT.DAE = 0;        //チャネル0　D/A変換を許可
    while(1){
        DA.DADR0 = P2.DR.BYTE;  //P2スイッチをチャネル0に出力  
    }
}
