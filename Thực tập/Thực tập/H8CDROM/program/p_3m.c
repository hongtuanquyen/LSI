//H8�}�C�R���e�X�g�v���O����
//�|�[�g4����͂ɐݒ� �����{�^���X�C�b�`
//�|�[�g5���o�͂ɐݒ� LED
//�|�[�g4�����͂����f�[�^���|�[�g5�ɏo��
//p_33m.c 

#include  "io.h"         //I/O�A�N�Z�X�p�C���N���[�h

int main(void)
{
    P4DDR = 0;           //�|�[�g4����
    P4PCR = 0xff;        //�v���A�b�vMOS ON
    P5DDR = 0xff;        //�|�[�g5�o��
    while(1)    
        P5DR = P4DR >> 4;//�f�[�^����4�r�b�g�E�ɃV�t�g�A�o��
}
