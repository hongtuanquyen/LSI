//H8マイコンテストプログラム
//タイマ割り込み使用しない、フラグが立つまで待つ
//ITU0使用　
//20msごとにカウンタクリア、50回のクリアでLED点灯反転
//itu_1m.c

#include  <3048.h>                  //3048、3052の内部I/O定義

int main(void)
{
    int  c=50;                      //割り込み回数
    P5.DDR = 0xff;
    ITU0.TCR.BIT.CCLR    = 1;       //カウンタクリア要因
    ITU0.TCR.BIT.TPSC    = 3;       //タイマプリスケーラ25MHz/8=3.125MHz
    ITU0.GRA        = 62499;        //3.125MHz/62,500=50Hz 周期20ms
    ITU.TSTR.BIT.STR0 = 1;          //タイマスタート
    P5.DR.BYTE = 0xff;              //LED出力データ初期値
    while(1){   
         while(!ITU0.TSR.BIT.IMFA);    
         c-=1;
         if(c==0){
             P5.DR.BYTE = ~P5.DR.BYTE; //LED出力データを反転
             c = 50;                //割り込み回数再設定
         }
         ITU0.TSR.BIT.IMFA = 0;     //割り込みステータスフラグクリア
    }
}
