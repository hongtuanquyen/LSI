//H8マイコンテストプログラム
//ポート5のLED点滅
//p_1.c

#define P5DDR (*(volatile unsigned char*)0xfffc8)
#define P5DR (*(volatile unsigned char*)0xfffca)

//点滅時間間隔
void wait (long loop)
{
    while (loop--);
}

int main(void)
{
    P5DDR = 0xff;        //P5を全ビット出力に設定
    while (1) {          //繰り返し実行
        P5DR = 0xff;     //点灯
        wait (200000);   //点灯時間
        P5DR = 0;        //消灯
        wait(200000);    //消灯時間
    }
}
