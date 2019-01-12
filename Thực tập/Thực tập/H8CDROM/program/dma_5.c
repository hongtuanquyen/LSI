//H8マイコンテストプログラム
//DMACよる8相パルスの出力
//ノンオーバラップ8相パルス、ポートB LED点灯移動
//GRAコンペアマッチ　ノンオーバラップ期間 LED消灯
//GRBコンペアマッチ　パルス周期
//外部クロック　1kHz
//dma_5.c

#include <3048.h>

unsigned char outdata[8] ={0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x01}; //TPC出力データ

void ioinit(void)
{
    PA.DDR = 0xfe;            //ポートAビット0入力、外部クロック入力
    PB.DR.BYTE = 0x00;        //出力データ初期値
    PB.DDR = 0xff;            //ポートB全ビット出力
}

void dmainit(void)
{
    DMAC0A.MAR = outdata;     //メモリアドレス、転送元
    DMAC0A.IOAR = 0xa4;       //I/Oアドレス、転送先 NDRBレジスタ
    DMAC0A.ETCR = 0x0808;     //転送データ数
    DMAC0A.DTCR.BIT.RPE = 1;  //リピートモード
    DMAC0A.DTCR.BIT.DTE = 1;  //データ転送許可
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;    //コンペアマッチによる出力禁止
    ITU0.TIOR.BIT.IOB = 0;    //コンペアマッチによる出力禁止
    ITU0.GRA = 999;           //ノンオーバラップ期間 1s
    ITU0.GRB = 1999;          //パルス周期 2s
    ITU0.TCR.BYTE = 0x44;     //GRBコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU0.TIER.BIT.IMIEA = 1;  //割り込み許可
}

void tpcinit(void)
{
    
    TPC.TPMR.BIT.G2NOV = 1;   //TPC出力グループ2はノンオーバラップ動作
    TPC.TPMR.BIT.G3NOV = 1;   //TPC出力グループ3はノンオーバラップ動作
    TPC.NDERB.BYTE = 0xff;    //TPC15からTPC8出力を許可
    TPC.TPCR.BIT.G2CMS = 0;   //TP11からTP8の出力トリガITU0コンペアマッチ
    TPC.TPCR.BIT.G3CMS = 0;   //TP15からTP12の出力トリガITU0コンペアマッチ
    TPC.NDRB1.BYTE = 0x01;    //1回目出力データ
}

int main(void)
{
    ioinit();
    dmainit();
    tpcinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;     //ITU0スタート
    while(1);
}
