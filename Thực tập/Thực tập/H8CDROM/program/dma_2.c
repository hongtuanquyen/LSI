//H8マイコンテストプログラム
//DMACによるLEDの点灯移動、1s周期
//ポートBに出力
//ポート4LED点灯移動、通常のプログラム
//GRAコンペアマッチ
//外部クロック　1kHz
//dma_2.c

#include <3048.h>

unsigned char outdata[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //出力データ

void ioinit(void)
{
  
    PB.DDR = 0xff;            //ポートB全ビット出力
    P4.DDR = 0xff;            //ポート4全ビット出力
}
void dmainit(void)
{
    DMAC0A.MAR = outdata;     //メモリアドレス、転送元　配列
    DMAC0A.IOAR = 0xd6;       //I/Oアドレス、転送先　ポートB
    DMAC0A.ETCR = 0x0808;     //転送データ数 8回
    DMAC0A.DTCR.BIT.RPE = 1;  //リピートモード
    DMAC0A.DTCR.BIT.DTE = 1;  //データ転送許可
}
 
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;    //コンペアマッチによる出力禁止
    ITU0.GRA =999;            //パルス周期　1s
    ITU0.TCR.BYTE = 0x24;     //GRAコンペアマッチカウンタクリア、TCNTクリア、外部クロック
    ITU0.TIER.BIT.IMIEA = 1;  //割り込み許可
}

void wait(void)
{
    long loop=100000;
    while(loop--);
}

int main(void)
{
    int i;
    unsigned char d;
    ioinit();                  //I/Oポートイニシャライズ
    dmainit();                 //DMACイニシャライズ
    ituinit();                 //ITUイニシャライズ
    ITU.TSTR.BIT.STR0 = 1;     //ITU0スタート
    while(1){                  //ポート4LED適当に点灯移動している
        d = 1;
        for(i=0;i<8;i++){
            P4.DR.BYTE = d;
            wait();
            d<<=1;
        }
    }
}
