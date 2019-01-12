// Ａ／Ｄコンバータテスト
// AN4よりの入力アナログ信号を入力する。０Ｖから５Ｖ
// 変換されたディジタル信号の上位８ビットをＰ４に出力
// 同信号の下位２ビットをＰＢに出力
//　AD-1.C

#include  <3048.h>            // 3048の内部I/O定義

void ioinit(void)             //I/Oポートイニシャライズ
{
    PB.DDR = 0xff;            // PBを全ビット出力に設定
    P4.DDR = 0xff;            // P4を全ビット出力に設定
}

void adinit(void)             //A/Dイニシャライズ
{
    AD.ADCSR.BIT.ADF = 0;       //ADFフラグクリア
    AD.ADCSR.BIT.SCAN = 0;      //単一モード選択
    AD.ADCSR.BIT.CKS = 1;       //クロックセレクト、変換時間134ステート
    AD.ADCSR.BIT.CH = 4;        //チャネルセレクト　AN4単一モード
}

int main(void)
{  
    unsigned int dh,dl;              //　変換された信号を記憶する
    ioinit();
    adinit();
    while(1){
        AD.ADCSR.BIT.ADST = 1;         //A/D変換スタート
        while(AD.ADCSR.BIT.ADF == 0);  //変換終了を待つ
        dh = (AD.ADDRA & 0xff00) >> 8; //上位8ビットをdhに格納
        dl = (AD.ADDRA & 0x00ff);      //下位8ビットをdlに格納
        PB.DR.BYTE = dl;               //下位ビットをPBに出力
        P4.DR.BYTE = dh;               //上位ビットをP4に出力
        AD.ADCSR.BIT.ADF = 0;          //フラグクリア
    }
}
