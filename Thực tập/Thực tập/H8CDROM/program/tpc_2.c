//H8�}�C�R���e�X�g�v���O����
//TPC�ɂ��4���p���X�̏o�� �O���[�v3�A2�@PB�o��
//�p���X�o�͂�LED�Ŋm�F�A�|�[�gB 1Hz�_���ړ�
//GRA�R���y�A�}�b�`
//�O���N���b�N�@1kHz�ATCLKA���́iPA0�j
//tpc_2.c

#include <3048.h>

unsigned char outdata[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//�o�̓f�[�^

void ioinit(void)
{
    PA.DDR = 0x00;           //�|�[�gA�r�b�g0�ɊO���N���b�N���́A1kHz
    PB.DR.BYTE = 0x00;       //�����l�o�̓f�[�^
    PB.DDR = 0xff;           //�S�r�b�g�o�� 
}
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA =999;           //�p���X�����@1s
    ITU0.TCR.BYTE = 0x24;    //GRA�R���y�A�}�b�`�J�E���^�N���A�A�O���N���b�N
    ITU0.TIER.BIT.IMIEA = 1;
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G3NOV = 0;  //�O���[�v3�ʏ퓮��
    TPC.TPMR.BIT.G2NOV = 0;  //�O���[�v2�ʏ퓮��
    TPC.NDERB.BYTE = 0xff;   //TPC�o�͋���
    TPC.TPCR.BIT.G3CMS = 0;  //�O���[�v3�R���y�A�}�b�`ITU0
    TPC.TPCR.BIT.G2CMS = 0;  //�O���[�v2�R���y�A�}�b�`ITU0
    TPC.NDRB1.BYTE = 0x01;   //1��ڏo�̓f�[�^
}

void int_imia0(void)
{
    static int i=1;
    ITU0.TSR.BIT.IMFA = 0;
    TPC.NDRB1.BYTE = outdata[i];
        i=i+1;
        if(i==8)
            i=0;
}

int main(void)
{
    ioinit();
    ituinit();
    tpcinit();
    ITU0.TIER.BIT.IMIEA = 1;
    EI;
    ITU.TSTR.BIT.STR0 = 1;    //ITU�X�^�[�g
    while(1);
}
