//H8�}�C�R���e�X�g�v���O����
//TPC�ɂ��X�e�b�s���O���[�^�̐���
//�O���[�v3�ƃO���[�v2�̓Ɨ��o�́A���E�t��]
//GRA�R���y�A�}�b�`
//�O���[�v3�AITU1���荞�݁A�O���[�v2�AITU0���荞��
//�����N���b�N�@1/8
//stm_6.c

#include <3048.h>

unsigned char outdata[4] = {0x01,0x02,0x04,0x08};  //TPC�O���[�v2�o�̓f�[�^
unsigned char outdata1[4] = {0x80,0x40,0x20,0x10}; //TP�O���[�v3�o�̓f�[�^
void ioinit(void)
{
    PB.DR.BYTE = 0x00;       //�o�̓f�[�^�����l
    PB.DDR = 0xff;           //�|�[�gA�S�r�b�g�o��
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA =31249;         //�p���X�����@10ms�@
    ITU0.TCR.BYTE = 0x23;    //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/8
    ITU0.TIER.BIT.IMIEA = 1; //���荞�݋���
    ITU1.TIOR.BIT.IOA = 0;   //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU1.GRA =62499;         //�p���X�����@25ms
    ITU1.TCR.BYTE = 0x23;    //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/8
    ITU1.TIER.BIT.IMIEA = 1; //���荞�݋���
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G2NOV = 0;  //TPC�o�̓O���[�v2�͒ʏ퓮��
    TPC.TPMR.BIT.G3NOV = 0;  //TPC�o�̓O���[�v3�͒ʏ퓮��
    TPC.NDERB.BYTE = 0xff;   //TPC7����TPC0�o�͂�����
    TPC.TPCR.BIT.G2CMS = 0;  //TP3����TP0�̏o�̓g���KITU0�R���y�A�}�b�`
    TPC.TPCR.BIT.G3CMS = 1;  //TP7����TP3�̏o�̓g���KITU1�R���y�A�}�b�`
    TPC.NDRB1.BYTE = 0x80;   //�O���[�v3�A1��ڏo�̓f�[�^
    TPC.NDRB2.BYTE = 0x01;   //�O���[�v2�A1��ڏo�̓f�[�^
}

void int_imia0(void)         //�O���[�v2
{
    static int i=1;
    ITU0.TSR.BIT.IMFA = 0;        //�X�e�[�^�X�t���O�N���A
    TPC.NDRB2.BYTE = outdata[i];  //�z��f�[�^�]��
        i=i+1;
        if(i==4)
            i=0;
}
void int_imia1(void)              //�O���[�v3
{
    static int j=1;
    ITU1.TSR.BIT.IMFA = 0;        //�X�e�[�^�X�t���O�N���A
    TPC.NDRB1.BYTE = outdata1[j]; //�z��f�[�^�]��
        j=j+1;
        if(j==4)
            j=0;
}
int main(void)
{
    ioinit();
    ituinit();
    tpcinit();
    EI;
    ITU.TSTR.BIT.STR0 = ITU.TSTR.BIT.STR1 = 1;     //ITU0�A1�X�^�[�g
    while(1);
}
