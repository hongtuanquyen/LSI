//LEDの点灯移動
//ポートA、B、4に接続されたLEDを右から左に点灯移動する
//p_4m.c
#include  "io.h"        //3048、3052の内部I/O定義

void wait(void)
{
  long t=400000;
  while(t--);
}
int main(void)
{
  int i;                //繰り返し演算データを入れる変数
  int d = 0x01;         //LED出力初期値
  PADDR = 0xff;         //PAを全ビット出力に設定
  PBDDR = 0xff;         //PAを全ビット出力に設定
  P4DDR = 0xff;         //PAを全ビット出力に設定
  while(1){
      for(i=0;i<8;i++){ //8回繰り返す
         PADR = d;
         PBDR = d;
         P4DR = d;
         wait();
         d <<= 1;
         }
         d = 1;         //for文のループを抜けたら再設定
      }
}
