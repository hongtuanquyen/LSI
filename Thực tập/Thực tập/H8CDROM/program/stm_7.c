//H8マイコンテストプログラム
//TPCによるステッピングモータの制御
//グループ3とグループ2の独立出力、左右逆回転
//スイッチの入力で回転速度を変化する
//GRAコンペアマッチ
//グループ3、ITU1割り込み、グループ2、ITU0割り込み
//内部クロック　1/8
//stm_7.c

#include <3048.h>

unsigned char outdata[4] = {0x01,0x02,0x04,0x08};  //TPCグループ2出力データ
unsigned char outdata1[4] = {0x80,0x40,0x20,0x10}; //TPグループ3出力データ
void ioinit(void)
{
    PA.DDR = 0x00;
    PB.DR.BYTE = 0x00;                             //初回出力データ
    PB.DDR = 0xff;                                 //ポートB全ビット出力
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;                         //コンペアマッチによる出力禁止
    ITU0.GRA =65000;                               //パルス周期　初期値　
    ITU0.TCR.BYTE = 0x23;                          //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU0.TIER.BIT.IMIEA = 1;                       //割り込み許可
    ITU1.TIOR.BIT.IOA = 0;                         //コンペアマッチによる出力禁止
    ITU1.GRA =65000;                               //パルス周期　初期値
    ITU1.TCR.BYTE = 0x23;                          //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU1.TIER.BIT.IMIEA = 1;                       //割り込み許可
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G2NOV = 0;                        //TPC出力グループ2は通常動作
    TPC.TPMR.BIT.G3NOV = 0;                        //TPC出力グループ3は通常動作
    TPC.NDERB.BYTE = 0xff;                         //TPC7からTPC0出力を許可
    TPC.TPCR.BIT.G2CMS = 0;                        //TP3からTP0の出力トリガITU0コンペアマッチ
    TPC.TPCR.BIT.G3CMS = 1;                        //TP7からTP3の出力トリガITU1コンペアマッチ
    TPC.NDRB1.BYTE = 0x80;                         //グループ3、1回目出力データ
    TPC.NDRB2.BYTE = 0x01;                         //グループ2、1回目出力データ
}

void int_imia0(void)                               //グループ2
{
    static int i=1;
    ITU0.GRA = (int)(65000/(PA.DR.BYTE & 0x0f));   //スイッチ入力、下位4ビット
    ITU0.TSR.BIT.IMFA = 0;                         //ステータスフラグクリア
    TPC.NDRB2.BYTE = outdata[i];                   //配列データ転送
        i=i+1;
        if(i==4)
            i=0;
}
void int_imia1(void)                               //グループ3
{
    static int j=1;
    ITU1.GRA = (int)(65000/((PA.DR.BYTE & 0xf0)>>4)); //スイッチ入力、上位4ビット 
    ITU1.TSR.BIT.IMFA = 0;                            //ステータスフラグクリア
    TPC.NDRB1.BYTE = outdata1[j];                     //配列データ転送
        j+=1;
        if(j==4)
            j=0;
}
int main(void)
{
    int speed;
    ioinit();
    ituinit();
    tpcinit();
    EI;
    ITU.TSTR.BIT.STR0 = ITU.TSTR.BIT.STR1 = 1;        //ITU0、1スタート
    while(1);
}
