//H8�}�C�R���e�X�g�v���O����
//ITU�ɂ��g�O���o�ͤLED�_��
//�O���N���b�N�@1kHz
//GRA�R���y�A�}�b�`�ATIOCA0�ɏo��
//itu_4.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0x04;           //�|�[�gA PA2�o�ͤTIOCA0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x24;    //GRA�R���y�A�}�b�`�ATCNT�N���A�A�v���X�P�[��
    ITU0.TIOR.BIT.IOA = 3;   //GRA�̃R���y�A�}�b�`�Ńg�O���o�́ALED
    ITU0.GRA =499;           //1kHz 0.5s�@����1s�@1Hz
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITU�X�^�[�g
    while(1);                //�������Ȃ�
}
