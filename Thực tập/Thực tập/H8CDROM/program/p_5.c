//H8�}�C�R���e�X�g�v���O����
//�|�[�g4����͂ɐݒ� �����{�^���X�C�b�`
//�|�[�g5���o�͂ɐݒ� LED
//�����{�^���X�C�b�`S1�i�|�[�g4�r�b�g4�j��������Ă����LED�_��
//��������LED����
//p_5.c

#include "io.h"

void wait(void)
{
    long loop = 200000;
    while(loop--);
}

int main(void)
{
    unsigned char sw;  
    P4DDR = 0;           //�|�[�g4����
    P4PCR = 0xff;        //�v���A�b�vMOS ON
    P5DDR = 0xff;        //�|�[�g5�o��
    while(1){    
        sw = ~P4DR;      //�f�[�^���́A�g1�h�A�g0�h�𔽓]����
        if(sw & 0x10){   //�X�C�b�`�������ꂽ���̔���
            P5DR = 0x0f; //LED�_��
            wait();
            P5DR = 0;
            wait();
        }
        else             //�X�C�b�`��������Ă��Ȃ�
            P5DR = 0;    //LED����
    }
}
