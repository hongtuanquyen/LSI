//H8マイコンテストプログラム
//配列にデータを用意する
//ポートBに接続したステッピングモータを回転する
//2個同時回転、上位4ビット、下位4ビット
//2相励磁
//パルス間隔はループによる
//stm_1mm.c 

#include <3048.h>

void wait(void)                                //モータパルス間隔
{
    int t=10000;                               //この数値で回転速度が決まる
    while(t--);
}

int main(void)
{
    int i;
    unsigned char md[] = {0x33,0x66,0xcc,0x99};//モータ出力データ
    PB.DDR = 0xff;                             //ポートB出力
    while(1){
        for(i=0;i<4;i++){                      //配列要素を進める
            PB.DR.BYTE = md[i];                //配列データ出力
            wait();
        }
    }
}
