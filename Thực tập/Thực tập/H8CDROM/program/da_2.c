//D/A�R���o�[�^�e�X�g�v���O����
//D/A�R���o�[�^�ւ̏o�̓f�[�^��0����255�܂ŕω������ďo�͂���
//�A�i���O�M���̓d���ω��𒲂ׂ�
//da_2.c
#include <3048.h>

void wait(void)
{
    int loop=20000;
    while(loop--);
}

int main(void)
{
    int d;
    DA.DACR.BIT.DAOE0 = 1;     //DA0�g�p
    DA.DACR.BIT.DAE = 0;       //�`���l��0�@D/A�ϊ�������
    while(1){
        for(d=0;d<256;d++){    //�d���̏㏸
            DA.DADR0 = d;  
            wait();
        }
        for(d=255;d>0;d--){    //�d���̉��~
            DA.DADR0 = d;  
            wait();
        }
    }
}
