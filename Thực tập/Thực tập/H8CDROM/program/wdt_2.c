//H8�}�C�R���e�X�g�v���O����
//WDT�e�X�g
//8�r�b�g�^�C�}�Ƃ��Ă̎g�p
//�^�C�}���荞�݂�LED�_�� ��42ms
//wdt_2.c

#include <3048.h>

void ioinit(void)
{
    PB.DDR = 0xff;                 //�|�[�gB�S�r�b�g�o��
}

void wdtinit(void)
{
    WDT.WRITE.TCSR = 0xa527;       //�C���^�[�o���^�C�}�I���J�E���g����A1/4096
}

void int_wovi(void)                //WDT�^�C�}���荞��
{
    int dummy;                     //�ǂݍ��݂̃_�~�[�ϐ�
    static unsigned char d = 0xff; //LED�o�̓f�[�^�����l
    dummy =  WDT.WRITE.TCSR;       //TCSR���W�X�^��ǂ�
    WDT.WRITE.TCSR = 0xa527;       //���荞�݃t���O�̃N���A
    PB.DR.BYTE = d;                //�f�[�^�o��
    d = ~d;                        //�f�[�^���]
}

int main(void)
{
    ioinit();
    wdtinit();
    EI;
    while(1);
}
