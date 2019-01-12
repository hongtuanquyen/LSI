//H8マイコンテストプログラム
//TPCによるステッピングモータの制御
//グループ3とグループ2の独立出力、左右逆回転
//GRAコンペアマッチ
//グループ3、ITU1割り込み、グループ2、ITU0割り込み
//内部クロック　1/8
//stm_6.c

#include <3048.h>

unsigned char outdata[4] = {0x01,0x02,0x04,0x08};  //TPCグループ2出力データ
unsigned char outdata1[4] = {0x80,0x40,0x20,0x10}; //TPグループ3出力データ
void ioinit(void)
{
    PB.DR.BYTE = 0x00;       //出力データ初期値
    PB.DDR = 0xff;           //ポートA全ビット出力
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //コンペアマッチによる出力禁止
    ITU0.GRA =31249;         //パルス周期　10ms　
    ITU0.TCR.BYTE = 0x23;    //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU0.TIER.BIT.IMIEA = 1; //割り込み許可
    ITU1.TIOR.BIT.IOA = 0;   //コンペアマッチによる出力禁止
    ITU1.GRA =62499;         //パルス周期　25ms
    ITU1.TCR.BYTE = 0x23;    //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU1.TIER.BIT.IMIEA = 1; //割り込み許可
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G2NOV = 0;  //TPC出力グループ2は通常動作
    TPC.TPMR.BIT.G3NOV = 0;  //TPC出力グループ3は通常動作
    TPC.NDERB.BYTE = 0xff;   //TPC7からTPC0出力を許可
    TPC.TPCR.BIT.G2CMS = 0;  //TP3からTP0の出力トリガITU0コンペアマッチ
    TPC.TPCR.BIT.G3CMS = 1;  //TP7からTP3の出力トリガITU1コンペアマッチ
    TPC.NDRB1.BYTE = 0x80;   //グループ3、1回目出力データ
    TPC.NDRB2.BYTE = 0x01;   //グループ2、1回目出力データ
}

void int_imia0(void)         //グループ2
{
    static int i=1;
    ITU0.TSR.BIT.IMFA = 0;        //ステータスフラグクリア
    TPC.NDRB2.BYTE = outdata[i];  //配列データ転送
        i=i+1;
        if(i==4)
            i=0;
}
void int_imia1(void)              //グループ3
{
    static int j=1;
    ITU1.TSR.BIT.IMFA = 0;        //ステータスフラグクリア
    TPC.NDRB1.BYTE = outdata1[j]; //配列データ転送
        j=j+1;
        if(j==4)
            j=0;
}
int main(void)
{
    ioinit();
    ituinit();
    tpcinit();
    EI;
    ITU.TSTR.BIT.STR0 = ITU.TSTR.BIT.STR1 = 1;     //ITU0、1スタート
    while(1);
}
