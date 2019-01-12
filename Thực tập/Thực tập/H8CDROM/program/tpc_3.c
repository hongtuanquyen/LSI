//H8�}�C�R���e�X�g�v���O����
//TPC�ɂ��4���p���X�̏o��
//�O���[�v1�ƃO���[�v0�̓Ɨ��o��
//GRA�R���y�A�}�b�`
//�O���[�v1 ITU1���荞�݁A�O���[�v0 ITU0���荞��
//�����N���b�N�@1/1
//tpc_3.c

#include <3048.h>

unsigned char outdata[4] = {0x01,0x02,0x04,0x08};  //TPC�O���[�v0�o�̓f�[�^
unsigned char outdata1[4] = {0x80,0x40,0x20,0x10}; //TP�O���[�v1�o�̓f�[�^
void ioinit(void)
{
    PA.DR.BYTE = 0x00;       //�o�̓f�[�^�����l
    PA.DDR = 0xff;           //�|�[�gA�S�r�b�g�o��
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA =24999;         //�p���X�����@1ms
    ITU0.TCR.BYTE = 0x20;    //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/1
    ITU0.TIER.BIT.IMIEA = 1; //���荞�݋���
    ITU1.TIOR.BIT.IOA = 0;   //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU1.GRA = 49999;        //�p���X�����@2ms
    ITU1.TCR.BYTE = 0x20;    //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/1
    ITU1.TIER.BIT.IMIEA = 1; //���荞�݋���
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G0NOV = 0;  //TPC�o�̓O���[�v0�͒ʏ퓮��
    TPC.TPMR.BIT.G1NOV = 0;  //TPC�o�̓O���[�v1�͒ʏ퓮��
    TPC.NDERA.BYTE = 0xff;   //TPC7����TPC0�o�͂�����
    TPC.TPCR.BIT.G0CMS = 0;  //TP3����TP0�̏o�̓g���KITU0�R���y�A�}�b�`
    TPC.TPCR.BIT.G1CMS = 1;  //TP7����TP3�̏o�̓g���KITU1�R���y�A�}�b�`
    TPC.NDRA1.BYTE = 0x80;   //�O���[�v1�A1��ڏo�̓f�[�^
    TPC.NDRA2.BYTE = 0x01;   //�O���[�v0�A1��ڏo�̓f�[�^
}

void int_imia0(void)              //�O���[�v0
{
    static int i=1;
    ITU0.TSR.BIT.IMFA = 0;        //�X�e�[�^�X�t���O�N���A
    TPC.NDRA2.BYTE = outdata[i];  //�z��f�[�^�]��
        i=i+1;
        if(i==4)
            i=0;
}

void int_imia1(void)              //�O���[�v1
{
    static int j=2;
    ITU1.TSR.BIT.IMFA = 0;        //�X�e�[�^�X�t���O�N���A
    TPC.NDRA1.BYTE = outdata1[j]; //�z��f�[�^�]��
        j+=1;
        if(j==4)
            j=0;
}

int main(void)
{
    ioinit();
    ituinit();
    tpcinit();
    EI;
    ITU.TSTR.BIT.STR0 = ITU.TSTR.BIT.STR1 = 1;   //ITU0�A1�X�^�[�g
    while(1);
}
