//����ۑ�2
//�������[�^�̐���
//�|�[�gA�̓��́i�Z���T�̔����j�Ő��䂷��
//�|�[�gA�r�b�g0=�g1�h�Ŕ����A�g0�h�Ŕ����Ȃ��Ƃ���
//��~��Z���T�̔����𒲂ׂċt��]
//10:�E��]�A01:����]�A11:�u���[�L��~�A00:�n�C�C���s�[�_���X��~
//pt_2

#include <3048.h>        //��Ő����AI/O�|�[�g�̃A�h���X

void ioinit(void)
{
     PA.DDR = 0x00;      //�|�[�gA�������
     PB.DDR = 0xff;      //�|�[�gB���[�^�o��
}

void wait(int c)         //���ԑ҂��֐�
{
    long t = 700000;
    while(c--){
        while(t--);
        t = 700000;
    }
}

int main(void)
{
    ioinit();                    //�����ݒ�
    while(1){                    //�J��Ԃ�
        while(PA.DR.BIT.B0 == 0);//�Z���T��������܂ő҂�   
        PB.DR.BYTE = 2;          //���[�^�E��]
        wait(3);                 //3�b�ԑ҂�
        PB.DR.BYTE = 3;          //���[�^��~
        wait(5);                 //5�b�ԑ҂�
        while(PA.DR.BIT.B0 == 1);//�Z���T�����Ȃ��Ȃ�܂ő҂�   
        PB.DR.BYTE = 1;          //���[�^����]
        wait(3);                 //3�b�ԑ҂�
        PB.DR.BYTE = 3;          //���[�^��~
    }
}
