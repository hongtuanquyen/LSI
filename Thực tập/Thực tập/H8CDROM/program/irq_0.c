//H8�}�C�R���e�X�g�v���O����
//IRQ0���荞�݃e�X�g
//���C���v���O���� �|�[�g5��LED�_��
//���荞�݃v���O�����@�|�[�g5LED���ݓ_��3��
//IRQ_0.c

#include <3048.h>

//�_�Ŏ��ԊԊu
void wait (void)
{
    long t=200000;
    while (t--);
}

void int_irq0(void)            //���荞�݊֐�
{
    int c=3;
    while(c--){                //3��J��Ԃ�
        P5.DR.BYTE = 0x5;
        wait();
        P5.DR.BYTE = 0xa;
        wait();
    }
    INTC.ISR.BIT.IRQ0F = 0;//���荞�݃t���O�N���A
}

int main(void)
{
    DI;                        //���荞�݋֎~
    INTC.ISCR.BIT.IRQ0SC = 1;  //���̗͂���������G�b�W�Ŋ��荞�ݗv������
    INTC.IER.BIT.IRQ0E = 1;    //IRQ0���荞�݂�����
    P5.DDR = 0xff;             //P5��S�r�b�g�o�͂ɐݒ�
    EI;                        //���荞�݋���
    while (1) {                //�J��Ԃ����s
        P5.DR.BYTE = 0xff;     //�_��
        wait ();               //�_������
        P5.DR.BYTE = 0;        //����
        wait();                //��������
    }
}
