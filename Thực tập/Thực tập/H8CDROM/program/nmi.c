//H8�}�C�R���e�X�g�v���O����
//NMI���荞�݃e�X�g
//���C���v���O�����|�[�g5��LED�_��
//���荞�݃v���O�����@�|�[�g5LED���ݓ_��3��
//nmi.c

#include <3048.h>

//�_�Ŏ��ԊԊu
void wait (void)
{
    long t=200000;
    while (t--);
}

void int_nmi(void)          //���荞�݊֐�
{
    int c=3;
    while(c--){             //3��J��Ԃ�
        P5.DR.BYTE = 0x5;   //1�����_��
        wait();
        P5.DR.BYTE = 0xa;   //�_��LED�؂�ւ�
        wait();
    }
}

int main(void)
{
    SYSCR.BIT.NMIEG = 0;    //���荞�ݗv���M������������G�b�W�ݒ�
    P5.DDR = 0xff;          //P5��S�r�b�g�o�͂ɐݒ�
    while (1) {             //�J��Ԃ����s
        P5.DR.BYTE = 0xff;  //�_��
        wait ();            //�_������
        P5.DR.BYTE = 0;     //����
        wait();             //��������
    }}
