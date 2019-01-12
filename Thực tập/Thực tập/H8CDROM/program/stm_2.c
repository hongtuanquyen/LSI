//ステッピングモータの回転
//パルス間隔タイマ割り込み
//ITU0使用　
//5msごとに割り込み
//stm_2.c
#include  <3048.h>               //3048、3052の内部I/O定義

int md;                          //モータ出力データ、外部変数

void ioinit(void)
{
    PB.DDR = 0xff;
}

void ituinit(void)
{
    ITU0.TCR.BIT.CCLR    = 1;    //カウンタクリア要因
    ITU0.TCR.BIT.TPSC    = 3;    //タイマプリスケーラ25MHz/8=3.125MHz
    ITU0.GRA = 15624;            //1/3.125MHz=0.00032ms,5/0.00032=15,625
    ITU0.TIER.BIT.IMIEA = 1;     //IMFAフラグによる割り込み許可
}

//ITUインターバルタイマ割り込み
void int_imia0 (void)
{
    ITU0.TSR.BIT.IMFA = 0;       //割り込みステータスフラグクリア
    PB.DR.BYTE = md;             //モータへ出力
        md <<= 1;
        if(md == 0x110)          //4回シフトしたかの判断
            md = 0x11;           //4回シフトで初期値に戻す
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;       //タイマスタート
    md = 0x11;                   //モータ出力初期値
    EI;                          //割り込み許可
    while (1);                   //何もしない
}
