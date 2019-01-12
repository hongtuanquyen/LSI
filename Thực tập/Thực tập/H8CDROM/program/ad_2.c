//A/Dコンバータテスト
//AN0からAN3よりのアナログ信号を入力する。スキャンモード
//変換されたディジタル信号の上位8ビットをLEDに出力
//AN0-PA、AN1-PB、AN2-P4、AN3-P5に出力
//ad_2.c

#include  <3048.h>                         //3048、3052の内部I/O定義

void ioinit(void)
{
    PA.DDR = 0xff;                         //PAを全ビット出力に設定
    PB.DDR = 0xff;                         //PBを全ビット出力に設定
    P4.DDR = 0xff;                         //P4を全ビット出力に設定
    P5.DDR = 0xff;                         //P5を全ビット出力に設定
}

void adinit(void)
{
    AD.ADCSR.BIT.ADF = 0;                  //ADFフラグクリア
    AD.ADCSR.BIT.SCAN = 1;                 //スキャンモード選択
    AD.ADCSR.BIT.CKS = 1;                  //クロックセレクト
    AD.ADCSR.BIT.CH = 3;                   //チャネルセレクト　AN0-AN3スキャンモード
    AD.ADCSR.BIT.ADST = 1;                 //A/D変換スタート
}

int main(void)
{
    unsigned int dh;                       //変換された信号を記憶する
    ioinit();
    adinit();
    while(1){
        while(AD.ADCSR.BIT.ADF == 0);      //変換終了を待つ
        dh = (AD.ADDRA & 0xff00) >> 8;     //上位8ビットをdhに格納、AN0
        PA.DR.BYTE = dh;                   //上位ビットをPAに出力
        dh = (AD.ADDRB & 0xff00) >> 8;     //上位8ビットをdhに格納、AN1
        PB.DR.BYTE = dh;                   //上位ビットをPBに出力
        dh = (AD.ADDRC & 0xff00) >> 8;     //上位8ビットをdhに格納、AN2
        P4.DR.BYTE = dh;                   //上位ビットをP4に出力
        dh = (AD.ADDRD & 0xff00) >> 8;     //上位8ビットをdhに格納、AN3
        P5.DR.BYTE = dh;                   //上位ビットをP5に出力     
        AD.ADCSR.BIT.ADF = 0;              //フラグクリア
    }
}
