//ステッピングモータの回転
//ITU0使用、フラグが立つのを待つ　
//5msごとにモータに出力
//stm_3.c
#include  <3048.h>               //3048、3052の内部I/O定義

void ioinit(void)
{
    PB.DDR = 0xff;
}

void ituinit(void)
{
    ITU0.TCR.BIT.CCLR    = 1;     //カウンタクリア要因
    ITU0.TCR.BIT.TPSC    = 3;     //タイマプリスケーラ25MHz/8=3.125MHz
    ITU0.GRA       = 15624;       //3.125MHz/15625=200Hz、5ms、200pps
    ITU0.TIER.BIT.IMIEA = 0;      //IMFAフラグによる割り込み禁止
}

int main(void)
{
    int md = 0x11;                //モータ出力データ初期値
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;        //タイマスタート  
    while (1){
        PB.DR.BYTE = md;
            md <<= 1;
            if(md == 0x110)        //4回シフトしたかの判断
                md = 0x11;         //4回シフトで初期値に戻す
        while(!ITU0.TSR.BIT.IMFA); //ITUO IMFAフラグが立つのを待つ
        ITU0.TSR.BIT.IMFA = 0;     //フラグクリア
    }
}
