//H8�}�C�R���e�X�g�v���O����
//�������[�^�̉�]�p����A10��]�Œ�~
//�p���X�W�F�l���[�^��TCLKA�ɓ���
//GRA�R���y�A�}�b�`�Ń��[�^��~
//�O���N���b�N�@�p���X�W�F�l���[�^�M��
//dc_3.c

#include <3048.h>

int flg;                    //�R���y�A�}�b�`�ŗ��Ă�t���O

void ioinit(void)
{
    PA.DDR = 0x00;          //�N���b�N����
    PB.DDR = 0xff;          //�S�r�b�g�o�� 
}
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA =120;           //�p���X�W�F�l���[�^�ƃR���y�A�}�b�`�f�[�^�A1��]12�p���X
    ITU0.TCR.BYTE = 0x24;    //GRA�R���y�A�}�b�`�J�E���^�N���A�A�O���N���b�N
    ITU0.TIER.BIT.IMIEA = 1;
}

void int_imia0(void)
{
    flg = 1;                  //�t���O�𗧂Ă�
    ITU0.TIER.BIT.IMIEA = 0;  //IMFA�t���O�ɂ�銄�荞�ݗv���֎~
}

int main(void)
{
    ioinit();
    ituinit();
    flg = 0;                  //�t���O�����l
    ITU0.TIER.BIT.IMIEA = 1;  //IMFA�ɂ�銄�荞�ݗv��������
    EI;
    ITU.TSTR.BIT.STR0 = 1;    //ITU�X�^�[�g
    PB.DR.BYTE = 0x02;
    while(flg == 0);          //���荞�݂܂ŉ�]
    DI;                       //���荞�݋֎~
    PB.DR.BYTE = 0x00;        //��~
    while(1);                 //�����Ŏ~�܂��Ă���
}
