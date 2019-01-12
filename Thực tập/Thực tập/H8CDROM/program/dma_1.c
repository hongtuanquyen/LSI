//H8マイコンテストプログラム
//DMACによる4相パルスの出力
//ポートAに出力
//GRAコンペアマッチ
//内部クロック　1/1
//dma_1.c

#include <3048.h>

unsigned char outdata[4] = {0x11,0x22,0x44,0x88}; //出力データ

void ioinit(void)
{  
    PA.DDR = 0xff;            //ポートA全ビット出力
}

void dmainit(void)
{
    DMAC0A.MAR = outdata;     //メモリアドレス、転送元
    DMAC0A.IOAR = 0xd3;       //I/Oアドレス、転送先 PA
    DMAC0A.ETCR = 0x0404;     //転送データ数 4回
    
    DMAC0A.DTCR.BIT.DTSZ = 0; //バイトサイズ転送
    DMAC0A.DTCR.BIT.DTID = 0; //データ転送後MARをインクリメント
    DMAC0A.DTCR.BIT.RPE = 1;  //リピートモード 
    DMAC0A.DTCR.BIT.DTIE = 0; //リピートモード
    DMAC0A.DTCR.BIT.DTS = 0;  //ITU0コンペアマッチで割り込み起動
                              //以上で下位７ビットが0010000
    DMAC0A.DTCR.BIT.DTE = 1;  //データ転送許可
}
 
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;    //コンペアマッチによる出力禁止
    ITU0.GRA =12499;          //パルス周期　0.5ms
    ITU0.TCR.BYTE = 0x20;     //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/1
    ITU0.TIER.BIT.IMIEA = 1;  //割り込み許可
}

int main(void)
{
    ioinit();                  //I/Oポートイニシャライズ
    dmainit();                 //DMACイニシャライズ
    ituinit();                 //ITUイニシャライズ
    ITU.TSTR.BIT.STR0 = 1;     //ITU0スタート
    while(1);                  //何もしない
}
