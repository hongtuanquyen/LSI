//H8�}�C�R���e�X�g�v���O����
//�|�[�gB�ɐڑ������X�e�b�s���O���[�^����]����
//2������]�A���4�r�b�g�A����4�r�b�g
//1���㎥
//�p���X�Ԋu�̓��[�v�ɂ��
//stm_1.c 

#include <3048.h>

void wait(void)                //���[�^�p���X�Ԋu
{
    int t=10000;               //���̐��l�ŉ�]���x�����܂�
    while(t--);
}

int main(void)
{
    int i,md;
    PB.DDR = 0xff;             //�|�[�gB�o��
    while(1){
        md = 0x11;             //���[�^�o�͏����f�[�^
        for(i=0;i<4;i++){      //�E�V�t�g��4��J��Ԃ�
            PB.DR.BYTE = md;
            md <<= 1;          //�o�̓f�[�^���E�V�t�g����
            wait();
        }
    }
}
