//H8�}�C�R���e�X�g�v���O����
//�|�[�g4����͂ɐݒ� �����{�^���X�C�b�`
//�|�[�g5���o�͂ɐݒ� LED
//�|�[�g4�����͂����f�[�^���|�[�g5��LED�ɏo��
//p_6.c 

#include <3048.h>

int main(void)
{
    P4.DDR = 0;                               //�|�[�g4����
    P4.PCR.BYTE = 0xff;                       //�v���A�b�vMOS ON
    P5.DDR = 0xff;                            //�|�[�g5�o��
    while(1){    
        P5.DR.BYTE =~(P4.DR.BIT.B4 |          //���͂����f�[�^��OR�����
        P4.DR.BIT.B5 <<1 | P4.DR.BIT.B6 <<2 | //�S�̂𔽓]����
        P4.DR.BIT.B7 << 3);                   //�X�C�b�`�̃r�b�g���V�t�g
    }
}
