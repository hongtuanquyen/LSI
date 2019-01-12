//H8�}�C�R���e�X�g�v���O����
//ITU�ɂ��PWM�o�́A�f���[�e�B�A����
//LED�̓_�����t�D�A�[�ƂȂ�
//GRA�R���y�A�}�b�`�ATIOCA0�Ɂg1�h�o�́A�J�E���^�N���A
//GRB�R���y�A�}�b�`�ATIOCB0�Ɂg0�h�o��
//�����N���b�N�@1/8
//itu_7.c

#include <3048.h>

void wait(void)              //�I�V���X�R�[�v�Ŋϑ����邽��
{
    int t=5000;
    while(t--);
}

void ioinit(void)
{
    PA.DDR = 0x04;           //�|�[�gA PA2�o�ͤTIOCA0
}
void ituinit()
{
    ITU0.TCR.BYTE = 0x23;    //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/8
    ITU.TMDR.BIT.PWM0 = 1;   //�`���l��0�@PWM���[�h
    ITU0.GRA =1562;          //�p���X�����@0.5ms
    ITU0.GRB = 1;            //�gH�h���x���p���X���������l
}

int main(void)
{
    int i;
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;   //ITU�X�^�[�g
    while(1){
        for(i=1;i<1562;i++){
            while(ITU0.TSR.BIT.IMFA == 0);  //IMFA�t���O�`�F�b�N
            ITU0.TSR.BIT.IMFA = 0;          //IMFA�t���O�N���A
            ITU0.GRB = i;                   //�gH�h�p���X���f�[�^��������
            wait();                         //�ϑ��̂��߂̎��ԑ҂�
        }
    }
}
