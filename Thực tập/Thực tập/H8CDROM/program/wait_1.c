//H8�}�C�R���e�X�g�v���O����
//wait�֐����ԃe�X�g
//wait_1.c 

#include <3048.h>

void wait(void)    //���[�^�p���X�Ԋu
{
    int t=20000;   //���̐��l�ŉ�]���x�����܂�
    while(t--);
}

int main(void)
{
    PB.DDR = 0xff;
    while(1){
        PB.DR.BYTE = 0xff;
        wait();
        PB.DR.BYTE = 0x00;
        wait();
    }
}
