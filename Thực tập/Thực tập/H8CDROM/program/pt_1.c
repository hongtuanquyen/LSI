//����ۑ�1
//�������[�^�̐���
//�|�[�gA�̓��́i�Z���T�̔����j�Ő��䂷��
//�|�[�gA�r�b�g0=�g1�h�Ŕ����A�g0�h�Ŕ����Ȃ��Ƃ���
//10:�E��]�A01:����]�A11:�u���[�L��~�A00:�n�C�C���s�[�_���X��~
//pt_1

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
    ioinit();                //�����ݒ�
    while(PA.DR.BIT.B0 == 0);//�Z���T��������܂ő҂�   
    PB.DR.BYTE = 2;          //���[�^�E��]
    wait(3);                 //3�b�ԑ҂�
    PB.DR.BYTE = 3;          //���[�^��~
    while(1);                //�����Ŏ~�܂��Ă���A���Z�b�g�ōăX�^�[�g
}
