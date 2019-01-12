//H8�}�C�R���e�X�g�v���O����
//IRQ0���荞�݃e�X�g
//NMI+IRQ0
//���C���v���O�����|�[�g5��LED�_��
//NMI���荞��LED�S���̓_���A�_�Ŏ��Ԃ̂T�{
//���荞�݃v���O�����@�|�[�g5LED���ݓ_��5��
//IRQ_1.c

#include <3048.h>

// �_�Ŏ��ԊԊu
void wait (void)
{
    long t=200000;
    while (t--);
}

void int_nmi(void)              //NMI���荞�݊֐�
{
    int c=5;
    P5.DR.BYTE = 0xf;           //�S���̓_��
    while(c--)                  //�_������
        wait();
}

void int_irq0(void)             //IRQ0���荞�݊֐�
{
    int c=5;
    while(c--){                 //3��J��Ԃ�
        P5.DR.BYTE = 0x5;
        wait();
        P5.DR.BYTE = 0xa;
        wait();
    }
    INTC.ISR.BIT.IRQ0F = 0; //���荞�݃t���O�N���A
}

int main(void)
{
    DI;
    INTC.ISCR.BIT.IRQ0SC = 1;
    INTC.IER.BIT.IRQ0E = 1;
    P5.DDR = 0xff;              //P5��S�r�b�g�o�͂ɐݒ�
    EI;
    while (1) {                 //�J��Ԃ����s
        P5.DR.BYTE = 0xff;      //�_��
        wait ();                //�_������
        P5.DR.BYTE = 0;         //����
        wait();                 //��������
    }
}
