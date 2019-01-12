//H8�}�C�R���e�X�g�v���O����
//ITU�ɂ��g�O���o�́A���g��1kHz�@����1ms�A���T�C�N������0.5ms
//GRA�R���y�A�}�b�`�ATIOCA0�ɏo��
//itu_3.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0x04;           //�|�[�gA PA2�o�ͤTIOCA0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x23;    //GRA�R���y�A�}�b�`�ATCNT�N���A�A�v���X�P�[��1/8
    ITU0.TIOR.BIT.IOA = 3;   //GRA�̃R���y�A�}�b�`�Ńg�O���o��
    ITU0.GRA = 1562;         //25MHz/8 0.32��s�@����0.5ms�@0.5/0.00032=1562.5
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITU�X�^�[�g
    while(1);                //�������Ȃ�
}
