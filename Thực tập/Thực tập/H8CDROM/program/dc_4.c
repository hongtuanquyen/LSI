//H8マイコンテストプログラム
//ITUによるPWM出力
//RCサーボモータの制御
//最初中央で停止、次にポートA入力データ角度まで回転
//ポートＡの入力で回転角を制御
//GRAコンペアマッチ、TIOCA3に“H”出力、カウンタクリア
//GRBコンペアマッチ、TIOCA3に“L”出力
//内部クロック　1/8
//dc_4.c

#include <3048.h>

void wait(void)             //モータ回転時間確保
{
    long  t = 800000;
    while(t--);
}

void ioinit(void)
{
    PA.DDR = 0x00;          //ポートAスイッチ入力
    PB.DDR = 0xff;          //ポートB出力､TIOCA3
}
void ituinit()
{
    ITU3.TCR.BYTE = 0x23;   //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU.TMDR.BIT.PWM3 = 1;  //チャネル3　PWMモード
    ITU3.GRA =49999;        //パルス周期　16ms
}

int main(void)
{
    int i,sd;
    ioinit();
    ituinit();
    PB.DR.BIT.B1 = 0;
    ITU.TSTR.BIT.STR3 = 1;             //ITUスタート
    while(ITU3.TSR.BIT.IMFA == 0);     //IMFAフラグチェック
    ITU3.TSR.BIT.IMFA = 0;             //IMFAフラグクリア
    ITU3.GRB = 4374;                   //“H”レベルパルス周期初期値、モータ角度中央
    wait();
    while(1){                          //スイッチ入力による角度制御
        while(ITU3.TSR.BIT.IMFA == 0); //IMFAフラグチェック
        ITU3.TSR.BIT.IMFA = 0;         //IMFAフラグクリア
        sd =  PA.DR.BYTE*32;           //“H”レベル周期
        if( sd > 7499)
            sd = 7499;
        if(sd < 1437)
            sd = 1437;
        ITU3.GRB = sd;                 //ポートAのスイッチデータセット    
    }
}
