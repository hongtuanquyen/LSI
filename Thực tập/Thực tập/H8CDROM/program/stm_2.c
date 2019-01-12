//�X�e�b�s���O���[�^�̉�]
//�p���X�Ԋu�^�C�}���荞��
//ITU0�g�p�@
//5ms���ƂɊ��荞��
//stm_2.c
#include  <3048.h>               //3048�A3052�̓���I/O��`

int md;                          //���[�^�o�̓f�[�^�A�O���ϐ�

void ioinit(void)
{
    PB.DDR = 0xff;
}

void ituinit(void)
{
    ITU0.TCR.BIT.CCLR    = 1;    //�J�E���^�N���A�v��
    ITU0.TCR.BIT.TPSC    = 3;    //�^�C�}�v���X�P�[��25MHz/8=3.125MHz
    ITU0.GRA = 15624;            //1/3.125MHz=0.00032ms,5/0.00032=15,625
    ITU0.TIER.BIT.IMIEA = 1;     //IMFA�t���O�ɂ�銄�荞�݋���
}

//ITU�C���^�[�o���^�C�}���荞��
void int_imia0 (void)
{
    ITU0.TSR.BIT.IMFA = 0;       //���荞�݃X�e�[�^�X�t���O�N���A
    PB.DR.BYTE = md;             //���[�^�֏o��
        md <<= 1;
        if(md == 0x110)          //4��V�t�g�������̔��f
            md = 0x11;           //4��V�t�g�ŏ����l�ɖ߂�
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;       //�^�C�}�X�^�[�g
    md = 0x11;                   //���[�^�o�͏����l
    EI;                          //���荞�݋���
    while (1);                   //�������Ȃ�
}
