//H8�}�C�R���e�X�g�v���O����
//TPC�ɂ��4���p���X�̏o�� 1kHz
//�N���b�N���g���@25MHz
//GRA�R���y�A�}�b�`
//�����N���b�N�@1/1
//tpc_1.c

#include <3048.h>

unsigned char outdata[4] = {0x11,0x22,0x44,0x88}; //TPC�o�̓f�[�^

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
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G0NOV = 0;  //TPC�o�̓O���[�v0�͒ʏ퓮��
    TPC.TPMR.BIT.G1NOV = 0;  //TPC�o�̓O���[�v1�͒ʏ퓮��
    TPC.NDERA.BYTE = 0xff;   //TPC7����TPC0�o�͂�����
    TPC.TPCR.BIT.G0CMS = 0;  //TP3����TP0�̏o�̓g���KITU0�R���y�A�}�b�`
    TPC.TPCR.BIT.G1CMS = 0;  //TP7����TP3�̏o�̓g���KITU0�R���y�A�}�b�`
    TPC.NDRA1.BYTE = 0x11;   //1��ڏo�̓f�[�^
}

void int_imia0(void)
{
    static int i=1;               //�z��̗v�f�A2�Ԗڂ���X�^�[�g
    ITU0.TSR.BIT.IMFA = 0;        //�X�e�[�^�X�t���O�N���A
    TPC.NDRA1.BYTE = outdata[i];  
        i=i+1;
        if(i==4)
            i=0;
}

int main(void)
{
    ioinit();
    ituinit();
    tpcinit();
    EI;
    ITU.TSTR.BIT.STR0 = 1;         //ITU0�X�^�[�g
    while(1);
}
