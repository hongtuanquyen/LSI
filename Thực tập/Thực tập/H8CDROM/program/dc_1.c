//H8�}�C�R���e�X�g�v���O����
//�������[�^�̐���
//�|�[�gA�̓��͂Ő��䂷��
//10:�E��]�A01:����]�A11:�u���[�L��~�A00:�n�C�C���s�[�_���X��~
//dc_1.c

#include <3048.h>

void ioinit(void)
{
     PA.DDR = 0x00;              //�|�[�gA�������
     PB.DDR = 0xff;              //�|�[�gB���[�^�o��
}

int main(void)
{
    ioinit();
    while(1)   
        PB.DR.BYTE = PA.DR.BYTE; //����M�����o��
}
