//紙上課題2
//直流モータの制御
//ポートAの入力（センサの反応）で制御する
//ポートAビット0=“1”で反応、“0”で反応なしとする
//停止後センサの反応を調べて逆回転
//10:右回転、01:左回転、11:ブレーキ停止、00:ハイインピーダンス停止
//pt_2

#include <3048.h>        //後で説明、I/Oポートのアドレス

void ioinit(void)
{
     PA.DDR = 0x00;      //ポートA制御入力
     PB.DDR = 0xff;      //ポートBモータ出力
}

void wait(int c)         //時間待ち関数
{
    long t = 700000;
    while(c--){
        while(t--);
        t = 700000;
    }
}

int main(void)
{
    ioinit();                    //初期設定
    while(1){                    //繰り返し
        while(PA.DR.BIT.B0 == 0);//センサ反応あるまで待ち   
        PB.DR.BYTE = 2;          //モータ右回転
        wait(3);                 //3秒間待ち
        PB.DR.BYTE = 3;          //モータ停止
        wait(5);                 //5秒間待ち
        while(PA.DR.BIT.B0 == 1);//センサ反応なくなるまで待ち   
        PB.DR.BYTE = 1;          //モータ左回転
        wait(3);                 //3秒間待ち
        PB.DR.BYTE = 3;          //モータ停止
    }
}
