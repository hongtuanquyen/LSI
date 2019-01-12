//H8マイコンテストプログラム
//I/Oモード　
//DMACによるLEDの点灯移動、1s周期
//ポートBに出力
//ポート4LED点灯左移動、通常のプログラム
//転送終了で右移動
//GRAコンペアマッチ
//外部クロック　1kHz
//dma_6.c

#include <3048.h>

unsigned char outdata[9] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00}; //出力データ
unsigned char flg;            //割り込みでセットするフラグ変数

void ioinit(void)
{
  
    PB.DDR = 0xff;            //ポートB全ビット出力
    P4.DDR = 0xff;            //ポート4全ビット出力
}

void dmainit(void)
{
    DMAC0A.MAR = outdata;     //メモリアドレス、転送元　配列
    DMAC0A.IOAR = 0xd6;       //I/Oアドレス、転送先　ポートB
    DMAC0A.ETCR = 0x0009;     //転送データ数9、ワードデータになる
    DMAC0A.DTCR.BIT.DTSZ = 0; //バイトサイズ転送
    DMAC0A.DTCR.BIT.DTID = 0; //データ転送後MARをインクリメント
    DMAC0A.DTCR.BIT.RPE = 0;  //I/Oモード
    DMAC0A.DTCR.BIT.DTIE = 1; //転送終了時割り込み許可
    DMAC0A.DTCR.BIT.DTE = 1;  //データ転送許可
}
 
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;    //コンペアマッチによる出力禁止
    ITU0.GRA =999;            //パルス周期　1s
    ITU0.TCR.BYTE = 0x24;     //GRAコンペアマッチカウンタクリア、TCNTクリア、外部クロック
    ITU0.TIER.BIT.IMIEA = 1;  //割り込み許可
}

void wait(void)               //mainプログラムのLED点灯移動時間
{
    long loop=100000;
    while(loop--);
}

void int_dend0a(void)          //転送終了割り込み
{  
    flg = 1;                   //転送終了でフラグセット
    ITU0.TIER.BIT.IMIEA = 0;   //割り込み禁止
    DMAC0A.DTCR.BIT.DTIE = 0;  //転送終了時割り込み禁止
}

int main(void)
{
    int i;
    flg = 0;
    unsigned char d;
    ioinit();                  //I/Oポートイニシャライズ
    dmainit();                 //DMACイニシャライズ
    ituinit();                 //ITUイニシャライズ
    ITU.TSTR.BIT.STR0 = 1;     //ITU0スタート
    EI;
    d = 1;
    while(flg == 0){           //転送中のルーチン、フラグがセットされるまで
        d = 1;
        for(i=0;i<8;i++){
            P4.DR.BYTE = d;
                d <<= 1;
            wait();
        }
    }
    while(1){                 //転送終了後のルーチン
        d = 0x80;
        for(i=0;i<8;i++){
            P4.DR.BYTE = d;
                d >>= 1;
            wait();
        }
    }          
}
