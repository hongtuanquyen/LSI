//D/Aコンバータテストプログラム
//D/Aコンバータへの出力データを0から255まで変化させて出力する
//アナログ信号の電圧変化を調べる
//da_2.c
#include <3048.h>

void wait(void)
{
    int loop=20000;
    while(loop--);
}

int main(void)
{
    int d;
    DA.DACR.BIT.DAOE0 = 1;     //DA0使用
    DA.DACR.BIT.DAE = 0;       //チャネル0　D/A変換を許可
    while(1){
        for(d=0;d<256;d++){    //電圧の上昇
            DA.DADR0 = d;  
            wait();
        }
        for(d=255;d>0;d--){    //電圧の下降
            DA.DADR0 = d;  
            wait();
        }
    }
}
