//H8�}�C�R���e�X�g�v���O����
//�|�[�g4����͂ɐݒ� �����{�^���X�C�b�`
//�|�[�g5���o�͂ɐݒ� LED
//�����{�^���X�C�b�`S1�i�|�[�g4�r�b�g4�j��������Ă����LED�_��
//��������LED����
//p_7.c

#include <3048.h>

void wait(void)
{
    long loop = 200000;
    while(loop--);
}

int main(void)
{
    P4.DDR = 0;                //�|�[�g4����
    P4.PCR.BYTE = 0xff;        //�v���A�b�vMOS ON
    P5.DDR = 0xff;             //�|�[�g5�o��
    while(1){    
        if(!P4.DR.BIT.B4){     //�X�C�b�`�������ꂽ���̔���A���]���Ă���
            P5.DR.BYTE = 0x0f; //LED�_��
            wait();
            P5.DR.BYTE = 0;
            wait();
        }
        else                   //�X�C�b�`��������Ă��Ȃ�
            P5.DR.BYTE = 0;    //LED����
   }
}
