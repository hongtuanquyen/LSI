//LED�̓_���ړ�
//�|�[�gA�AB�A4�ɐڑ����ꂽLED���E���獶�ɓ_���ړ�����
//p_4.c

#include  "io.h"       //3048�A3052�̓���I/O��`

void wait(void)        //���ԉ҂��֐��A�󂯎������Ȃ�
{
    long t=400000;     //���Ԃ����߂鐔�l�A�֐����Œ�`
    while(t--);
}

int main(void)
{
    int d = 0x01;       //LED�o�͏����l
    PADDR = 0xff;       //PA��S�r�b�g�o�͂ɐݒ�
    PBDDR = 0xff;       //PA��S�r�b�g�o�͂ɐݒ�
    P4DDR = 0xff;       //PA��S�r�b�g�o�͂ɐݒ�
    while(1){
        PADR = d;       //LED�ɏo��
        PBDR = d;
        P4DR = d;
        wait();         //���ԁA�n�������Ȃ�
        d=d<<1;         //����1�r�b�g�V�t�g
        if(d == 0x100)  //8��V�t�g�������̔���
            d = 1;      //8��V�t�g������1���Đݒ�
    }
}
