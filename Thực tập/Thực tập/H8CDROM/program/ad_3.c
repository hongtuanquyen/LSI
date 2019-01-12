//A/Dコンバータテストプログラム
//AN0からの入力電圧をLCDに表示
//ad_3.c
#include <3048.h>
#define   E_SIG  0x20     
#define   RS_SIG 0x10     
 
void ioinit(void)
{
    P3.DDR = 0xff;                //ポート3を全ビット出力に設定
}

void adinit(void)
{
    AD.ADCSR.BIT.ADF = 0;         //ADFフラグクリア
    AD.ADCSR.BIT.SCAN = 0;        //単一モード選択
    AD.ADCSR.BIT.CKS = 1;         //クロックセレクト
    AD.ADCSR.BIT.CH = 0;          //チャネルセレクト　AN0単一モード
}
void wait(void)           
{
    int t = 1000;
    while(t--);
}

void lcdo8(unsigned char d)	    //8ビット出力関数
{
    d = d | E_SIG;        
    P3.DR.BYTE = d;         
    d = d & 0xdf;           
    P3.DR.BYTE = d;         
    wait();                 
}

void lcdini(void)               //初期化関数
{
    lcdo8(3);      
    lcdo8(3);      
    lcdo8(3);      
    lcdo8(2);      
    lcdo8(2);      
    lcdo8(8);      
    lcdo8(0);      
    lcdo8(0x0c);   
    lcdo8(0);      
    lcdo8(6);      
}

void lcdclr(void)	              //表示クリア関数
{
    P3.DR.BYTE = 0;
    lcdo8(0);
    lcdo8(1);                   //00000001でクリア
    wait();
}

void lcdxy1(void)               //カーソルを1行目に移動
{
    P3.DR.BYTE = 0;
    lcdo8(8);                   //1行目
    lcdo8(0);                   //先頭位置
    wait();
}

void lcdxy2(void)               //カーソルを2行目に移動
{
    P3.DR.BYTE = 0;
    lcdo8(0x0c);                //2行目
    lcdo8(0);                   //先頭位置
    wait();
}

void lcdxy(int y,int x)         //カーソルをy、xに移動
{
    P3.DR.BYTE = 0;
    lcdo8(y);                   //y位置
    lcdo8(x);                   //x位置
    wait();
}
void lcdo4(unsigned char d)     //1文字出力関数
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
    for(c=0;c<16;c++){          //16文字表示
        lcdo4(*str);            //表示関数コール
        str++;                  //文字列を次の文字にする
    }
}

int main(void)
{
    int add;                    //変換された信号を記憶する
    char s[4];                  //数字を文字データにし記憶する配列
    int i;
    ioinit();
    adinit();
    wait();                     //電源ONで15ms以上待ち
    lcdini();                   //LCDをイニシャライズする
    wait();
    lcdclr();                   //表示クリア
    lcdxy1();                   //表示位置
    dsp1g("AN0 Input VOLT  ");  //タイトル表示文字列
    wait();
    lcdxy2();                   //2行目指定
    dsp1g("                ");  //2行目クリア
    while(1){
        AD.ADCSR.BIT.ADST = 1;        //A/D変換スタート
        while(AD.ADCSR.BIT.ADF == 0); //変換終了を待つ
        add = AD.ADDRA >> 6;          //AN0入力、6ビット右にシフト、大切
        AD.ADCSR.BIT.ADF = 0;         //フラグクリア
        add=(int)((add*(5.0/1024.0)*1000.0)+0.5);//入力データを電圧値に換算
        for(i=0;i<4;i++){             //電圧値を文字データに変換するためのデータを入れる
            s[i] = add % 10;          //10で割った余りを入れる
            add /= 10;                //10で割る
        }  
        lcdxy(0x0c,5);                //2行、6文字目にカーソルを動かす
        lcdo4(s[3]+'0');              //1桁目の文字　本文参照
        lcdo4('.');                   //小数点
        lcdo4(s[2]+'0');              //小数点第1位の文字
        lcdo4(s[1]+'0');              //小数第2位の文字
        lcdo4(s[0]+'0');              //小数第3位の文字
        lcdo4('V');                   //Vの文字 
        wait();
    }
}        
