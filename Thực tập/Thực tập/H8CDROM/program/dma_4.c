//H8マイコンテストプログラム
//DMACよる8相パルスの出力
//ノンオーバラップ8相パルス
//GRAコンペアマッチ　ノンオーバラップ期間
//GRAコンペアマッチ　パルス周期
//内部クロック 1/1
//dma_4.c

#include <3048.h>

unsigned char outdata[8] = {0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x01}; //TPC出力データ

void ioinit(void)
{
    PA.DR.BYTE = 0x00;       //出力データ初期値
    PA.DDR = 0xff;           //ポートA全ビット出力
}

void dmainit(void)
{
    DMAC0A.MAR = outdata;    //メモリアドレス、転送元
    DMAC0A.IOAR = 0xa5;      //I/Oアドレス、転送先 NDRAレジスタ
    DMAC0A.ETCR = 0x0808;    //転送データ数
    DMAC0A.DTCR.BIT.RPE = 1; //リピートモード
    DMAC0A.DTCR.BIT.DTE = 1; //データ転送許可
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //GRAコンペアマッチによる出力禁止
    ITU0.TIOR.BIT.IOB = 0;   //GRBコンペアマッチによる出力禁止
    ITU0.GRA =4999;          //ノンオーバラップ期間 20%
    ITU0.GRB =24999;         //パルス周期  1ms ノンオーバラップ期間含む
    ITU0.TCR.BYTE = 0x40;    //GRBコンペアマッチカウンタクリア、TCNTクリア、クロック1/1
    ITU0.TIER.BIT.IMIEA = 1; //割り込み許可
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G0NOV = 1;  //TPC出力グループ0はノンオーバラップ動作
    TPC.TPMR.BIT.G1NOV = 1;  //TPC出力グループ1はノンオーバラップ動作
    TPC.NDERA.BYTE = 0xff;   //TPC7からTPC0出力を許可
    TPC.TPCR.BIT.G0CMS = 0;  //TP3からTP0の出力トリガITU0コンペアマッチ
    TPC.TPCR.BIT.G1CMS = 0;  //TP7からTP3の出力トリガITU0コンペアマッチ
    TPC.NDRA1.BYTE = 0x01;   //1回目出力データ
}

int main(void)
{
    ioinit();
    dmainit();
    tpcinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;    //ITU0スタート
    while(1);
}
