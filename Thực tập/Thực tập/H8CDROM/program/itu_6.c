//H8�}�C�R���e�X�g�v���O����
//ITU�ɂ��PWM�o��
//GRA�R���y�A�}�b�`�ATIOCA0�Ɂg1�h�o�́A�J�E���^�N���A
//GRB�R���y�A�}�b�`�ATIOCA0�Ɂg0�h�o��
//�����N���b�N�@1/8
//itu_6.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0x04;           //�|�[�gA PA2�o�ͤTIOCA0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x23;    //GRA�R���y�A�}�b�`�ATCNT�N���A�A�N���b�N1/8
 
   
    ITU.TMDR.BIT.PWM0 = 1;   //�`���l��0�@PWM���[�h
    ITU0.GRA = 1562;         //�p���X���@2kHz�@0.5ms
    ITU0.GRB = 312;          //�gH�h���x���p���X���@1/5
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITU�X�^�[�g
    while(1);                //�������Ȃ�
}
