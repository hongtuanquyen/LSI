//H8マイコンテストプログラム
//LCD表示プログラム、指定した場所に表示する
//lcd_1.c 

#include "io.h"
#define  E_SIG  0x20       //E信号ビット
#define  RS_SIG 0x10       //RS信号ビット
 
void wait(void)            //待ち時間関数
{
    int  t= 1000;          //3048Fでは800まで下げられる
    while(t--);
}

void lcdo8(unsigned char d)	//8ビット出力関数
{
    d = d | E_SIG;          //受け取ったデータにＥ信号を加える
    P3DR = d;               //LCDに出力
    d = d & 0xdf;           //E信号を0にする
    P3DR = d;               //LCDに出力
    wait();                 //確定するまでの待ち時間
}

void lcdini(void)	          //初期化関数
{
    lcdo8(3);               //00000011を出力
    lcdo8(3);               //3回繰り返す
    lcdo8(3);      
    lcdo8(2);               //00000010を出力
    lcdo8(2);               //繰り返し
    lcdo8(8);               //00001000を出力
    lcdo8(0);      
    lcdo8(0x0c);            //00001100を出力
    lcdo8(0);      
    lcdo8(6);               //00000110を出力
}

void lcdclr(void)	          //LCDクリア関数
{
    P3DR = 0;
    lcdo8(0);
    lcdo8(1);               //00000001でクリア
    wait();
}

void lcdxy(unsigned char y,unsigned char x)	//カーソルを移動
{
    P3DR = 0;
    lcdo8(y);               //1行目　y=8、2行目　y=0x0c
    lcdo8(x);               //0から15､0で一番左
    wait();
}

void lcdo4(unsigned char d)	//文字出力関数
{
    unsigned dd;
    dd = d;                 //表示データ保存
    d = d >> 4;             //上位4ビットを下位4ビットに移す
    d = d & 0x0f;           //下位4ビット取り出す
    d = d | RS_SIG;         //RS信号と合成
    lcdo8(d);               //LCDに出力
    wait();                 //時間待ち
    dd = dd & 0x0f;         //下位4ビット取りだし
    dd = dd | RS_SIG;       //RS信号と合成
    lcdo8(dd);              //LCDに出力
    wait();
}

int main(void)
{
    P3DDR = 0xff;           //ポート3を全ビット出力に設定
    wait();                 //電源ONで15ms以上待ち
    lcdini();               //LCDをイニシャライズする
    wait();
    lcdclr();               //LCDクリア
    lcdxy(8,8);             //表示位置セット
    lcdo4('Y');             //表示文字セット、表示関数コール
    wait();                 //確定までの待ち時間
    lcdxy(0x0c,1);          //2文字目の表示
    lcdo4('?');       
    while(1);         
}
