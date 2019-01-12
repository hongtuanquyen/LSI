//H8�}�C�R���e�X�g�v���O����
//ITU�ɂ��2���g�O���o��
//ITU�ɂ��g�O���o�́@1kHz
//GRA�R���y�A�}�b�`�ATIOCA0�ɏo��
//GRB�R���y�A�}�b�`�ATIOCB0�ɏo�́A�J�E���^�N���A
//�����N���b�N�@1/8
//itu_5.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0x0c;           //�|�[�gA PA2�APA3�o�� TIOCA0�ATIOCB0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x43;    //GRB�R���y�A�}�b�`�ATCNT�N���A�A�N���b�N1/8
    ITU0.TIOR.BIT.IOA = 3;   //GRA�̃R���y�A�}�b�`��TIOCA0�Ƀg�O���o��
    ITU0.TIOR.BIT.IOB = 3;   //GRB�̃R���y�A�}�b�`��TIOCB0�Ƀg�O���o�́A�J�E���^�N���A
    ITU0.GRA = 781;          //GRA�̔���
    ITU0.GRB = 1562;         //�R���y�A�}�b�`�ŃJ�E���^�N���A�A����0.5ms
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITU�X�^�[�g
    while(1);                //�������Ȃ�
}
