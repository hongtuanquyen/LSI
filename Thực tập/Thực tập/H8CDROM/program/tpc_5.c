//H8マイコンテストプログラム
//TPCによる4相パルスの出力
//ノンオーバラップ4相パルス
//GRAコンペアマッチ　ノンオーバラップ期間
//GRBコンペアマッチ　パルス周期
//内部クロック　1/1
//tpc_5.c

#include <3048.h>

unsigned char outdata[4] = {0x11,0x22,0x44,0x88}; //TPC出力データ

void ioinit(void)
{
    PA.DR.BYTE = 0x00;       //出力データ初期値
    PA.DDR = 0xff;           //ポートA全ビット出力
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //GRAコンペアマッチによる出力禁止
    ITU0.TIOR.BIT.IOB = 0;   //GRBコンペアマッチによる出力禁止
    ITU0.GRA =4999;          //ノンオーバラップ期間
    ITU0.GRB =24999;         //パルス周期
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
    TPC.NDRA1.BYTE = 0x11;   //1回目出力データ
}

void int_imia0(void)
{
    static int i=1;
    ITU0.TSR.BIT.IMFA = 0;        //ステータスフラグクリア
    TPC.NDRA1.BYTE = outdata[i];  //出力データ
        i=i+1;
        if(i==4)
            i=0;
}

int main(void)
{
    ioinit();
    ituinit();
    tpcinit();
    EI;
    ITU.TSTR.BIT.STR0 = 1;         //ITU0スタート
    while(1);
}
