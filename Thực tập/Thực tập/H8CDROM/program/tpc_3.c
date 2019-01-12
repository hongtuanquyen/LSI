//H8マイコンテストプログラム
//TPCによる4相パルスの出力
//グループ1とグループ0の独立出力
//GRAコンペアマッチ
//グループ1 ITU1割り込み、グループ0 ITU0割り込み
//内部クロック　1/1
//tpc_3.c

#include <3048.h>

unsigned char outdata[4] = {0x01,0x02,0x04,0x08};  //TPCグループ0出力データ
unsigned char outdata1[4] = {0x80,0x40,0x20,0x10}; //TPグループ1出力データ
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
    ITU1.TIOR.BIT.IOA = 0;   //コンペアマッチによる出力禁止
    ITU1.GRA = 49999;        //パルス周期　2ms
    ITU1.TCR.BYTE = 0x20;    //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/1
    ITU1.TIER.BIT.IMIEA = 1; //割り込み許可
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G0NOV = 0;  //TPC出力グループ0は通常動作
    TPC.TPMR.BIT.G1NOV = 0;  //TPC出力グループ1は通常動作
    TPC.NDERA.BYTE = 0xff;   //TPC7からTPC0出力を許可
    TPC.TPCR.BIT.G0CMS = 0;  //TP3からTP0の出力トリガITU0コンペアマッチ
    TPC.TPCR.BIT.G1CMS = 1;  //TP7からTP3の出力トリガITU1コンペアマッチ
    TPC.NDRA1.BYTE = 0x80;   //グループ1、1回目出力データ
    TPC.NDRA2.BYTE = 0x01;   //グループ0、1回目出力データ
}

void int_imia0(void)              //グループ0
{
    static int i=1;
    ITU0.TSR.BIT.IMFA = 0;        //ステータスフラグクリア
    TPC.NDRA2.BYTE = outdata[i];  //配列データ転送
        i=i+1;
        if(i==4)
            i=0;
}

void int_imia1(void)              //グループ1
{
    static int j=2;
    ITU1.TSR.BIT.IMFA = 0;        //ステータスフラグクリア
    TPC.NDRA1.BYTE = outdata1[j]; //配列データ転送
        j+=1;
        if(j==4)
            j=0;
}

int main(void)
{
    ioinit();
    ituinit();
    tpcinit();
    EI;
    ITU.TSTR.BIT.STR0 = ITU.TSTR.BIT.STR1 = 1;   //ITU0、1スタート
    while(1);
}
