//H8マイコンテストプログラム
//ITUによるPWM出力
//RCサーボモータの制御
//A/D変換データで回転角を制御
//最初中央で停止、次にA/D変換データで回転
//GRAコンペアマッチ、TIOCA3に“H”出力、カウンタクリア
//GRBコンペアマッチ、TIOCA3に“L”出力
//内部クロック　1/8
//dc_5.c

#include <3048.h>

void wait(void)                  //モータ回転時間確保
{
    long  t = 800000;
    while(t--);
}

void ioinit(void)
{
    PB.DDR = 0xff;               //ポートB 出力､TIOCA3
}
void ituinit()
{
    ITU3.TCR.BYTE = 0x23;        //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU.TMDR.BIT.PWM3 = 1;       //チャネル3　PWMモード
    ITU3.GRA =49999;             //パルス周期　16ms
}
void adinit(void)                //A/Dイニシャライズ
{
    AD.ADCSR.BIT.ADF = 0;          //ADFフラグクリア
    AD.ADCSR.BIT.SCAN = 0;         //単一モード選択
    AD.ADCSR.BIT.CKS = 1;          //クロックセレクト、変換時間134ステート
    AD.ADCSR.BIT.CH = 4;           //チャネルセレクト　AN4単一モード
}

int main(void)
{
    unsigned int sd;
    ioinit();
    ituinit();
    adinit();
    PB.DR.BIT.B1 = 0;
    ITU.TSTR.BIT.STR3 = 1;            //ITUスタート
    while(ITU3.TSR.BIT.IMFA == 0);    //IMFAフラグチェック
    ITU3.TSR.BIT.IMFA = 0;            //IMFAフラグクリア
    ITU3.GRB = 4374;                  //“H”レベルパルス周期初期値、モータ角度中央
    wait();
    while(1){                         //A/D入力による角度制御
        AD.ADCSR.BIT.ADST = 1;          //A/D変換スタート
        while(AD.ADCSR.BIT.ADF == 0);   //変換終了を待つ
        AD.ADCSR.BIT.ADF = 0;           //フラグクリア 
        sd =AD.ADDRA >> 2;              //適当なデータに変換するため
        while(ITU3.TSR.BIT.IMFA == 0);//IMFAフラグチェック
        ITU3.TSR.BIT.IMFA = 0;        //IMFAフラグクリア
        if(sd > 7499)
            sd = 7499;
        if(sd < 1437)
            sd = 1437;
        ITU3.GRB = sd;                //回転角データをセット   
    }
}
