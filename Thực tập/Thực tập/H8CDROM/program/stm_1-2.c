//H8�}�C�R���e�X�g�v���O����
//�z��Ƀf�[�^��p�ӂ���
//�|�[�gB�ɐڑ������X�e�b�s���O���[�^����]����
//2������]�A���4�r�b�g�A����4�r�b�g
//1-2���㎥
//�p���X�Ԋu�̓��[�v�ɂ��
//stm_1-2.c 

#include <3048.h>

void wait(void)                                //���[�^�p���X�Ԋu
{
    int t=10000;                               //���̐��l�ŉ�]���x�����܂�
    while(t--);
}

int main(void)
{
    int i;
    unsigned char md[] = {0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99};//���[�^�o�̓f�[�^
    PB.DDR = 0xff;                             //�|�[�gB�o��
    while(1){
        for(i=0;i<8;i++){                      //�z��v�f��i�߂�
            PB.DR.BYTE = md[i];                //�z��f�[�^�o��
            wait();
        }
    }
}
