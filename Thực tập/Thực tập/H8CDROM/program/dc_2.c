//H8マイコンテストプログラム
//ITUによるPWM出力、デューティ連続可変
//直流モータのPWM制御、加速、減速
//GRAコンペアマッチ TIOCA3に“H”出力、カウンタクリア
//GRBコンペアマッチ、TIOCA3に“L”出力
//内部クロック　1/8
//dc_2.c

#include <3048.h>

void ioinit(void)
{
    PB.DDR = 0xff;           //ポートB 出力､TIOCA3､PWM出力端子PB0
}
void ituinit()
{
    ITU3.TCR.BYTE = 0x23;    //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU.TMDR.BIT.PWM3 = 1;   //チャネル3　PWMモード
    ITU3.GRA = 7812;         //パルス周期　2.5ms
    ITU3.GRB =400;           //“H”レベルパルス周期初期値
}

int main(void)
{
    int i;
    ioinit();
    ituinit();
    PB.DR.BIT.B1 = 0;
    ITU.TSTR.BIT.STR3 = 1;                 //ITUスタート
    while(1){
        for(i=400;i<7813;i++){             //加速 “H”レベル周期約1/20から1/1まで
            while(ITU3.TSR.BIT.IMFA == 0); //IMFAフラグチェック
            ITU3.TSR.BIT.IMFA = 0;         //IMFAフラグクリア
            ITU3.GRB = i;                  //“H”パルス幅データ書き込み
        }
        for(i=7813;i>400;i--){             //減速 “H”レベル幅を減らしていく
            while(ITU3.TSR.BIT.IMFA == 0); //IMFAフラグチェック
            ITU3.TSR.BIT.IMFA = 0;         //IMFAフラグクリア
            ITU3.GRB = i;                  //“H”パルス幅データ書き込み
        }      
    }
}
