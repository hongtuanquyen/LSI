//H8マイコンテストプログラム
//12V　1.8度ステッピングモータ使用
//TPCによるステッピングモータの制御
//グループ2出力
//加速、定速、減速、配列に加速データ
//加速280パルス、定速2,000パルス、減速280パルス
//スタート200pps、最高1,200pps、加速時間0.4s
//GRAコンペアマッチ
//速度データはクロック周波数25MHzの場合
//グループ2、ITU0割り込み
//内部クロック　1/8
//stm_8.c

#include <3048.h>
int flg;
unsigned char outdata[4] = {0x01,0x02,0x04,0x08}; //TPCグループ2出力データ
int accdata[] = {14706,13934,13275,12702,12198,11750,11349,10987,10658,10356,
                 10080,9824,9587,9367,9161,8968,8787,8617,8456,8304,
                 8160,8023,7893,7769,7651,7538,7430,7327,7227,7132,
                 7040,6952,6867,6785,6706,6630,6556,6485,6416,6349,
                 6284,6221,6160,6101,6043,5987,5933,5880,5828,5778,
                 5729,5681,5635,5589,5545,5502,5459,5418,5378,5338,
                 5299,5262,5225,5188,5153,5118,5084,5050,5018,4986,
                 4954,4923,4893,4863,4834,4805,4777,4749,4722,4695,
                 4669,4643,4617,4592,4567,4543,4519,4496,4473,4450,
                 4427,4405,4384,4362,4341,4320,4300,4279,4259,4240,
                 4220,4201,4182,4164,4145,4127,4109,4091,4074,4057,
                 4040,4023,4006,3990,3974,3958,3942,3926,3911,3896,
                 3881,3866,3851,3836,3822,3808,3794,3780,3766,3752,
                 3739,3725,3712,3699,3686,3674,3661,3648,3636,3624,
                 3612,3600,3588,3576,3564,3553,3541,3530,3519,3508,
                 3497,3486,3475,3464,3453,3443,3433,3422,3412,3402,
                 3392,3382,3372,3362,3352,3343,3333,3324,3315,3305,
                 3296,3287,3278,3269,3260,3251,3242,3234,3225,3216,
                 3208,3199,3191,3183,3175,3166,3158,3150,3142,3134,
                 3126,3119,3111,3103,3096,3088,3081,3073,3066,3058,
                 3051,3044,3037,3029,3022,3015,3008,3001,2994,2987,
                 2981,2974,2967,2961,2954,2947,2941,2934,2928,2921,
                 2915,2909,2902,2896,2890,2884,2878,2872,2866,2860,
                 2854,2848,2842,2836,2830,2824,2818,2813,2807,2801,
                 2796,2790,2785,2779,2774,2768,2763,2757,2752,2747,
                 2741,2736,2731,2726,2721,2715,2710,2705,2700,2695,
                 2690,2685,2680,2675,2670,2666,2661,2656,2651,2646,
                 2642,2637,2632,2628,2623,2618,2614,2609,2605,2600
                 };
                
void ioinit(void)
{
    P4.DDR = 0x00;            //スタートスイッチ
    P4.PCR.BYTE = 0xff;       //プルアップMOS ON
    PB.DR.BYTE = 0x00;        //出力データ初期値
    PB.DDR = 0xff;            //ポートB全ビット出力
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;    //コンペアマッチによる出力禁止
    ITU0.GRA =15000;          //パルス周期　初期値　
    ITU0.TCR.BYTE = 0x23;     //GRAコンペアマッチカウンタクリア、TCNTクリア、クロック1/8
    ITU0.TIER.BIT.IMIEA = 1;  //割り込み許可
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G2NOV = 0;   //TPC出力グループ2は通常動作
    TPC.NDERB.BYTE = 0xff;    //TPC7からTPC0出力を許可
    TPC.TPCR.BIT.G2CMS = 0;   //TP3からTP0の出力トリガITU0コンペアマッチ
    TPC.NDRB2.BYTE = 0x01;    //グループ2、1回目出力データ
}

void int_imia0(void)          //グループ2
{
    flg = 1;                  //割り込みでフラグ立てる
    ITU0.TSR.BIT.IMFA = 0;    //ステータスフラグクリア    
}

void wait(void)               //停止時間
{
    long t = 800000;
    while(t--);
}

void waits(void)              //停止時間
{
    long t = 80000;
    while(t--);
}

int main(void)
{
    int i,k;                  //ワーキング変数
    ioinit();
    ituinit();
    tpcinit();
    k=1;                      //TPCに2番目のデータを出力する
    PB.DR.BYTE = 0x01;
    waits();
    EI;
    while(P4.DR.BIT.B4 == 1); //スタートスイッチ押されるまで待つ
    ITU.TSTR.BIT.STR0 = 1;    //ITU0、1スタート
    while(1){
        for(i=0;i<280;i++){               //加速区間201パルス
            while(flg == 0);              //割り込みフラグが立つまで待つ
            ITU0.GRA = accdata[i];        //速度データ 1つのデータ20回出力
            TPC.NDRB2.BYTE = outdata[k];  //配列データ転送
            flg = 0;                      //フラグクリア
            k=k+1;                        //モータ出力データ配列要素
            if(k==4)
                k=0;
        } 
        for(i=0;i<1000;i++){              //定速区間 4回転
            while(flg == 0);
            ITU0.GRA = accdata[199];      //速度データ
            TPC.NDRB2.BYTE = outdata[k];  //配列データ転送
            flg = 0;
            k=k+1;
            if(k==4)
                k=0;
        }   
        for(i=279;i>=0;i--){              //減速区間 200パルス
            while(flg == 0);
            ITU0.GRA = accdata[i];        //速度データ
            TPC.NDRB2.BYTE = outdata[k];  //配列データ転送
            flg = 0;
            k=k+1;
            if(k==4)
                k=0;
        } 
    wait();                               //停止時間  
    }   
}
