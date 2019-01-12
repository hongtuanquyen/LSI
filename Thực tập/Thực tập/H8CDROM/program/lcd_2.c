//H8マイコンテストプログラム
//LCD表示プログラム
//文字列を表示する
//lcd_2.c 
#include "io.h"
#define  E_SIG  0x20            //E信号ビット
#define  RS_SIG 0x10            //RS信号ビット
 
void wait(void)                 //時間待ち関数
{
    int t = 1000;
    while(t--);
}

void lcdo8(unsigned char d)     //8ビット出力関数
{
    d=d | E_SIG;                //受け取ったデータにE信号を加える
    P3DR = d;                   //LCDに出力
    d = d & 0xdf;               //E信号を0にする
    P3DR = d;                   //LCDに出力
    wait();                     //確定するまでの待ち時間
}

void lcdini(void)               //初期化関数
{
    lcdo8(3);                   //00000011を出力
    lcdo8(3);                   //3回繰り返す
    lcdo8(3);      
    lcdo8(2);                   //00000010を出力
    lcdo8(2);                   //繰り返し
    lcdo8(8);                   //00001000を出力
    lcdo8(0);      
    lcdo8(0x0c);                //00001100を出力
    lcdo8(0);      
    lcdo8(6);                   //00000110を出力
}

void lcdclr(void)               //表示クリア関数
{
    P3DR = 0;
    lcdo8(0);
    lcdo8(1);                   //00000001でクリア
    wait();
}

void lcdxy1(void)               //カーソルを1行目に移動
{
    P3DR = 0;
    lcdo8(8);                   //1行目
    lcdo8(0);                   //先頭位置
    wait();
}

void lcdxy2(void)               //カーソルを2行目に移動
{
    P3DR = 0;
    lcdo8(0x0c);                //2行目
    lcdo8(0);                   //先頭位置
    wait();
}

void lcdo4(unsigned char d)     //文字出力関数
{
    unsigned dd;
    dd = d;                     //表示データ保存
    d = d >> 4;                 //上位4ビットを下位4ビットに移す
    d = d & 0x0f;               //下位4ビット取り出す
    d = d | RS_SIG;             //RS信号と合成
    lcdo8(d);                   //LCDに出力
    wait();                     //時間待ち
    dd = dd & 0x0f;             //下位4ビット取りだし
    dd = dd | RS_SIG;           //RS信号と合成
    lcdo8(dd);                  //LCDに出力
    wait();
}

void dsp1g(char *str)           //1行表示関数　*strは文字列変数
{
    int c = 16;                 //16文字
    for(c = 0;c < 16;c++){      //16文字表示
    lcdo4(*str);                //表示関数コール
    str++;                      //文字列を次の文字にする
    }
}

int main(void)
{
    P3DDR = 0xff;               //ポート3を全ビット出力に設定
    wait();                     //電源ONで15ms以上待ち
    lcdini();                   //LCDをイニシャライズする
    wait();
    lcdclr();                   //表示クリア
    lcdxy1();                   //表示位置
    dsp1g("ABCDEFGHIJKLMNOP");  //表示文字列
    wait();
    lcdxy2();                   //2行目指定
    dsp1g("QRSTUVWXYZ012345");  //表示文字列
    while(1);                   //終わり、永久ループ
}
