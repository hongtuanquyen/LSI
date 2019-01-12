//H8マイコンテストプログラム
//ポート4を入力に設定 押しボタンスイッチ
//ポート5を出力に設定　LED
//ポート4より入力したデータをポート5に出力
//p_3.c 

#define P4DDR (*(volatile unsigned char*)0xfffc5)
#define P4DR (*(volatile unsigned char*)0xfffc7)
#define P4PCR (*(volatile unsigned char*)0xfffda)
#define P5DDR (*(volatile unsigned char*)0xfffc8)
#define P5DR (*(volatile unsigned char*)0xfffca)

int main(void)
{
    int d;
    P4DDR = 0;       //ポート4入力
    P4PCR = 0xff;    //プルアップMOS　ON
    P5DDR = 0xff;    //ポート5出力
    while(1){    
        d = P4DR;    //データ入力
        d = d >> 4;  //4ビット右にシフト
        P5DR = d;
    }
}
