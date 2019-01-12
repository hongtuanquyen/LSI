//H8テストプログラム
//通信実験、SCIポートとパソコン間の通信
//パソコン側ハイパーターミナル
//H8スタートでメッセージ送信、次にパソコンよりの文字を受信
//パソコンに送り返すとともに文字コードをLEDに出力
//sci_1.c

#include <3048.h>

// ビットレート設定リスト  クロック16MHz時(3048F)
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
    BR4800   = 162,  //n=0、SMRの設定、以下すべて0
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
    for (i = 0; i < 3000; i++) ;      //待つ
    SCI1.SCR.BYTE = 0x30;             //送受信許可
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

//1文字受信
char SCI1_IN_d (void)                 //受信文字を返す
{
    char c;                           //受信データ格納変数
    while (!(SCI1.SSR.BYTE & 0x78)) ; //受信またはエラーフラグが立つまで待つ
    if (SCI1.SSR.BIT.RDRF == 1) {     //受信完了なら
        c = SCI1.RDR;                 //データ取り出し
        SCI1.SSR.BIT.RDRF = 0;        //受信フラグクリア
    } 
    else {
        SCI1.SSR.BYTE &= 0x80;        //エラーなら エラーフラグクリア
        c = 0;
    }
    return c;
}

int main(void)
{
    unsigned char rd = 0;                 //受信データ格納変数
    PB.DDR = 0xff;                        //ポートB出力
    PB.DR.BYTE = 0xff;                    //LED点灯
    initSCI1(BR19200);                    //SCI初期化
    SCI1_OUT_s("HELLO PC this is H8!!");  //H8よりパソコンに送信
    while(1){                            
        rd = SCI1_IN_d();                 //受信
        SCI1_OUT_d(rd);                   //エコーバック
        PB.DR.BYTE = rd;                  //LEDに出力
    }
}
