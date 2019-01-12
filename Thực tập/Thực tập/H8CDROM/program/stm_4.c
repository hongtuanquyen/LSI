//ステッピングモータの回転
//右2回転、停止、左1回転、停止 これを3回繰り返す。
//ITU0使用、フラグが立つのを待つ　
//10msごとにモータに出力
//stm_4.c
#include  <3048.h>                //3048、3052の内部I/O定義

void ioinit(void)
{
    PB.DDR = 0xff;
}

void ituinit(void)
{
    ITU0.TCR.BIT.CCLR    = 1;     //カウンタクリア要因
    ITU0.TCR.BIT.TPSC    = 3;     //タイマプリスケーラ25MHz/8=3.125MHz
    ITU0.GRA = 31249;             //3.125MHz/31250=100Hz、10ms、100pps
    ITU0.TIER.BIT.IMIEA = 0;      //IMFAフラグによる割り込み禁止
}

void wait(void)                   //停止時間
{
    long t=200000;
    while(t--);
}

int main(void)
{
    int i = 3;                    //繰り返しカウンタ
    int p;                        //回転パルス数
    int md = 0x11;                //モータ出力データ
    ioinit();
    ituinit();
    PB.DR.BYTE = md;
    wait();
    ITU.TSTR.BIT.STR0 = 1;        //タイマスタート
    while(i--){
        for(p=0;p<96;p++){        //2回転
            PB.DR.BYTE = md;
                md <<= 1;         //右回転
                if(md == 0x110)   //4回シフトしたかの判断
                    md = 0x11;         //4回シフトで初期値に戻す
            while(!ITU0.TSR.BIT.IMFA); //フラグが立つのを待つ
            ITU0.TSR.BIT.IMFA = 0;     //フラグクリア
        }
        wait();                        //少し停止
        for(p=50;p>0;p--){             //1回転、1パルス多くした
            PB.DR.BYTE = md;
                md >>= 1;              //左回転
                if(md == 0x08)         //4回シフトしたかの判断
                    md = 0x88;         //4回シフトで初期値に戻す
            while(!ITU0.TSR.BIT.IMFA); //フラグが立つのを待つ
            ITU0.TSR.BIT.IMFA = 0;     //フラグクリア
        }
        wait();
    }    
    PB.DR.BYTE = 0x00;                 //モータ励磁OFF
    while(1);                          //ここで止まっている
}
