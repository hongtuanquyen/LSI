//H8�}�C�R���e�X�g�v���O����
//�^�C�}���荞�݂ɂ��X�e�b�s���O���[�^�̐���
//ITU0�AITU1���荞�ݎg�p�A2�̃��[�^�Ɨ���]
//ITU0�@5ms���Ɗ��荞�݂ŉE���[�^��]
//ITU1�@10ms���Ɗ��荞�݂ō����[�^��]
//�����N���b�N�@1/8
//stm_5.c

#include <3048.h>

int rmd = 0x01;
int lmd = 0x80;

void ioinit(void)
{
    PB.DDR = 0xff;           //�|�[�gB�S�r�b�g�o��
}

void ituinit()
{
    ITU0.TCR.BIT.CCLR = 1;   //GRA�R���y�A�}�b�`,�J�E���^�N���A
    ITU0.TCR.BIT.TPSC = 3;   //�^�C�}�v���X�P�[���@25MHz/8=3.125MHz
    ITU0.GRA = 15624;        //�p���X����3.125MHz/15625�A5ms�@�@
    ITU0.TIER.BIT.IMIEA = 1; //���荞�݋���
    ITU1.TCR.BIT.CCLR = 1;   //GRA�R���y�A�}�b�`�A�J�E���^�N���A
    ITU1.TCR.BIT.TPSC = 3;   //�^�C�}�v���X�P�[���@25MHz/8=3.125MHz
    ITU1.GRA = 31249;        //�p���X����3.125MHz/31250�A10ms�@�@
    ITU1.TIER.BIT.IMIEA = 1; //���荞�݋���
}

void int_imia0(void)         //�O���[�v2
{
    ITU0.TSR.BIT.IMFA = 0;   //�X�e�[�^�X�t���O�N���A
    PB.DR.BYTE = lmd + rmd;  //2�̃��[�^�f�[�^�����ďo��
        rmd <<= 1;           //�E��]
        if(rmd == 0x10)
            rmd = 0x01;
}

void int_imia1(void)         //�O���[�v3
{
    ITU1.TSR.BIT.IMFA = 0;   //�X�e�[�^�X�t���O�N���A
    PB.DR.BYTE = lmd + rmd;
        lmd >>= 1;           //����]
        if(lmd == 0x08)
            lmd = 0x80;
}
int main(void)
{
    ioinit();
    ituinit();
    EI;
    ITU.TSTR.BIT.STR0 = ITU.TSTR.BIT.STR1 = 1;     //ITU0�A1�X�^�[�g
    while(1);
}
