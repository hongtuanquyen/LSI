//H8マイコンテストプログラム
//ITUによるPWM出力、デューティ連続可変
//LEDの点灯がフゥアーとなる
//GRAコンペアマッチ、TIOCA0に“1”出力、カウンタクリア
//GRBコンペアマッチ、TIOCB0に“0”出力
//内部クロック　1/8
//itu_7.c

#include <3048.h>

void wait(void)              //オシロスコープで観測するため
{
    int t=5000;
    while(t--);
}

void ioinit(void)
{
    PA.DDR = 0x04;           //ポートA PA2出力､TIOCA0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x23;    //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU.TMDR.BIT.PWM0 = 1;   //チャネル0　PWMモード
    ITU0.GRA =1562;          //パルス周期　0.5ms
    ITU0.GRB = 1;            //“H”レベルパルス周期初期値
}

int main(void)
{
    int i;
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITUスタート
    while(1){
        for(i=1;i<1562;i++){
            while(ITU0.TSR.BIT.IMFA == 0);  //IMFAフラグチェック
            ITU0.TSR.BIT.IMFA = 0;          //IMFAフラグクリア
            ITU0.GRB = i;                   //“H”パルス幅データ書き込み
            wait();                         //観測のための時間待ち
        }
    }
}
