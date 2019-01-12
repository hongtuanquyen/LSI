//H8マイコンテストプログラム
//TPCによる4相パルスの出力
//グループ2とグループ3の独立出力
//GRAコンペアマッチ
//グループ2 ITU0割り込み、グループ3 ITU1割り込み
//外部クロック　1kHz
//tpc_4.c

#include <3048.h>

unsigned char outdata[4] = {0x01,0x02,0x04,0x08};  //TPCグループ2出力データ
unsigned char outdata1[4] = {0x80,0x40,0x20,0x10}; //TPグループ3出力データ
void ioinit(void)
{
    PA.DDR = 0x00;           //ポートAビット0　クロック入力
    PB.DR.BYTE = 0x00;       //出力データ初期値
    PB.DDR = 0xff;           //ポートB全ビット出力
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //コンペアマッチによる出力禁止
    ITU0.GRA =499;           //パルス周期　0.5s
    ITU0.TCR.BYTE = 0x24;    //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU0.TIER.BIT.IMIEA = 1; //割り込み許可
    ITU1.TIOR.BIT.IOA = 0;   //コンペアマッチによる出力禁止
    ITU1.GRA =999;           //パルス周期　1s
    ITU1.TCR.BYTE = 0x24;    //GRAコンペアマッチカウンタクリア、TCNTクリア、外部クロック
    ITU1.TIER.BIT.IMIEA = 1; //割り込み許可
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G2NOV = 0;  //TPC出力グループ2は通常動作
    TPC.TPMR.BIT.G3NOV = 0;  //TPC出力グループ3は通常動作
    TPC.NDERB.BYTE = 0xff;   //TPC15からTPC8出力を許可
    TPC.TPCR.BIT.G2CMS = 0;  //TP11からTP8の出力トリガITU0コンペアマッチ
    TPC.TPCR.BIT.G3CMS = 1;  //TP15からTP12の出力トリガITU1コンペアマッチ
    TPC.NDRB1.BYTE = 0x80;   //グループ3、1回目出力データ
    TPC.NDRB2.BYTE = 0x01;   //グループ2、1回目出力データ
}

void int_imia0(void)               //グループ2
{
    static int i=1;
    ITU0.TSR.BIT.IMFA = 0;         //ステータスフラグクリア
    TPC.NDRB2.BYTE = outdata[i];   //2回目出力データ
        i=i+1;
        if(i==4)
            i=0;
}

void int_imia1(void)               //グループ3
{
    static int j=1;
    ITU1.TSR.BIT.IMFA = 0;         //ステータスフラグクリア
    TPC.NDRB1.BYTE = outdata1[j];  //2回目出力データ
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
