//H8マイコンテストプログラム
//TPCによる4相パルスの出力 1kHz
//クロック周波数　25MHz
//GRAコンペアマッチ
//内部クロック　1/1
//tpc_1.c

#include <3048.h>

unsigned char outdata[4] = {0x11,0x22,0x44,0x88}; //TPC出力データ

void ioinit(void)
{
    PA.DR.BYTE = 0x00;       //出力データ初期値
    PA.DDR = 0xff;           //ポートA全ビット出力
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //コンペアマッチによる出力禁止
    ITU0.GRA =24999;         //パルス周期　1ms
    ITU0.TCR.BYTE = 0x20;    //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/1
    ITU0.TIER.BIT.IMIEA = 1; //割り込み許可
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G0NOV = 0;  //TPC出力グループ0は通常動作
    TPC.TPMR.BIT.G1NOV = 0;  //TPC出力グループ1は通常動作
    TPC.NDERA.BYTE = 0xff;   //TPC7からTPC0出力を許可
    TPC.TPCR.BIT.G0CMS = 0;  //TP3からTP0の出力トリガITU0コンペアマッチ
    TPC.TPCR.BIT.G1CMS = 0;  //TP7からTP3の出力トリガITU0コンペアマッチ
    TPC.NDRA1.BYTE = 0x11;   //1回目出力データ
}

void int_imia0(void)
{
    static int i=1;               //配列の要素、2番目からスタート
    ITU0.TSR.BIT.IMFA = 0;        //ステータスフラグクリア
    TPC.NDRA1.BYTE = outdata[i];  
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
