//H8マイコンテストプログラム
//TPCによる4相パルスの出力 グループ3、2　PB出力
//パルス出力はLEDで確認、ポートB 1Hz点灯移動
//GRAコンペアマッチ
//外部クロック　1kHz、TCLKA入力（PA0）
//tpc_2.c

#include <3048.h>

unsigned char outdata[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//出力データ

void ioinit(void)
{
    PA.DDR = 0x00;           //ポートAビット0に外部クロック入力、1kHz
    PB.DR.BYTE = 0x00;       //初期値出力データ
    PB.DDR = 0xff;           //全ビット出力 
}
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //コンペアマッチによる出力禁止
    ITU0.GRA =999;           //パルス周期　1s
    ITU0.TCR.BYTE = 0x24;    //GRAコンペアマッチカウンタクリア、外部クロック
    ITU0.TIER.BIT.IMIEA = 1;
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G3NOV = 0;  //グループ3通常動作
    TPC.TPMR.BIT.G2NOV = 0;  //グループ2通常動作
    TPC.NDERB.BYTE = 0xff;   //TPC出力許可
    TPC.TPCR.BIT.G3CMS = 0;  //グループ3コンペアマッチITU0
    TPC.TPCR.BIT.G2CMS = 0;  //グループ2コンペアマッチITU0
    TPC.NDRB1.BYTE = 0x01;   //1回目出力データ
}

void int_imia0(void)
{
    static int i=1;
    ITU0.TSR.BIT.IMFA = 0;
    TPC.NDRB1.BYTE = outdata[i];
        i=i+1;
        if(i==8)
            i=0;
}

int main(void)
{
    ioinit();
    ituinit();
    tpcinit();
    ITU0.TIER.BIT.IMIEA = 1;
    EI;
    ITU.TSTR.BIT.STR0 = 1;    //ITUスタート
    while(1);
}
