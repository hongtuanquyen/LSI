//H8マイコンテストプログラム
//ポート2スイッチを入力し、ポート5のLEDに出力
//p_2.c

#define P2DDR (*(volatile unsigned char*)0xfffc1)
#define P2DR (*(volatile unsigned char*)0xfffc3)
#define P2PCR (*(volatile unsigned char*)0xfffd8)
#define P5DDR (*(volatile unsigned char*)0xfffc8)
#define P5DR (*(volatile unsigned char*)0xfffca)

int main(void)
{
    P2DDR = 0;         //P2を全ビット入力に設定
    P2PCR = 0xff;      //P2プルアップ　本文参照
    P5DDR = 0xff;      //P5を出力に設定
    while (1) {
        P5DR = P2DR;   //入力したデータを出力
    }
}
