//H8テストプログラム
//通信実験、SCIポートとパソコン間の通信
//パソコン側ハイパーターミナル
//H8スタートでメッセージ送信
//受信は割り込み動作
//パソコンに送り返すとともに文字コードをLEDに出力
//mainの中はポート4LED点滅
//sci_2.c

#include <3048.h>

//ビットレート設定リスト  クロック16MHz時(3048F)
/*typedef enum {
    BR2400   = 207,  //n=0、SMRの設定
    BR4800   = 103,
    BR9600   = 51,
    BR19200  = 25,
    BR38400  = 12,
    BR57600  = 8,
    BR115200 = 3,
} BaudRate;*/

//ビットレート設定リスト  クロック25MHz時(3052F)
//25MHzクロックの場合はこちらを使用
typedef enum {
    BR2400   = 80,   //n=1、SMRの設定
    BR4800   = 162,  //n=0、SMRの設定,以下すべて0
    BR9600   = 80,   
    BR19200  = 40,   
    BR38400  = 19,   
    BR57600  = 13,   
    BR115200 = 5,    
} BaudRate;

//SCI初期化
void initSCI1 (BaudRate b)            //ビットレートを受け取る
{
    int i;
    SCI1.SCR.BYTE = SCI1.SMR.BYTE = 0;//初期値は0、本文参照
    SCI1.BRR = b;                     //ビットレート
    for (i = 0; i <4000; i++) ;       //待つ
    SCI1.SCR.BYTE = 0x70;             //受信割り込み許可、送受信許可
    SCI1.SSR.BYTE;                    //ダミーリード
    SCI1.SSR.BYTE = 0x80;             //エラーフラグクリア
}

//1文字送信
void SCI1_OUT_d (char c)              //文字コードを受け取る
{
    while (SCI1.SSR.BIT.TDRE == 0) ;  //0でデータあり、1になるまで待つ
    SCI1.TDR = c;                     //受け取った文字を送信
    SCI1.SSR.BIT.TDRE = 0;            //TDREビットをクリア
}

//文字列送信
void SCI1_OUT_s (char *st)            //文字列を受け取る
{
    while (*st) SCI1_OUT_d(*st++);    //1文字送信関数呼び出し
}

//エラー割り込み
void int_eri1(void)
{
    SCI1.SSR.BYTE &= 0x80;            //エラーなら エラーフラグクリア
}

//受信割り込み
void int_rxi1(void)
{
    char rd;                          //受信データ格納変数
    rd = SCI1.RDR;                    //受信データ格納
    SCI1_OUT_d(rd);                   //エコーバック、1文字送信呼び出し
    PB.DR.BYTE = rd;                  //LEDに出力 
    SCI1.SSR.BIT.RDRF = 0;            //受信フラグクリア
}

void ioinit(void)
{
    PB.DDR = 0xff;                    //ポートB全ビット出力
    P4.DDR = 0xff;                    //ポート4全ビット出力
}

void wait(void)
{
    long t = 400000;
    while(t--);
}

int main(void)
{
    ioinit();   
    initSCI1(BR19200);                     //SCI初期化
    SCI1_OUT_s(" HELLO PC this is H8!!");  //H8よりパソコンに送信
    EI;
    while(1){                              //ポート4LED点滅
        P4.DR.BYTE = 0xff;                 //メイン関数には受信処理はない
        wait();
        P4.DR.BYTE = 0;
        wait();
    }
}
