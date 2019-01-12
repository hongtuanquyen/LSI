//H8�}�C�R���e�X�g�v���O����
//ITU�ɂ��PWM�o��
//RC�T�[�{���[�^�̐���
//�ŏ������Œ�~�A���Ƀ|�[�gA���̓f�[�^�p�x�܂ŉ�]
//�|�[�g�`�̓��͂ŉ�]�p�𐧌�
//GRA�R���y�A�}�b�`�ATIOCA3�ɁgH�h�o�́A�J�E���^�N���A
//GRB�R���y�A�}�b�`�ATIOCA3�ɁgL�h�o��
//�����N���b�N�@1/8
//dc_4.c

#include <3048.h>

void wait(void)             //���[�^��]���Ԋm��
{
    long  t = 800000;
    while(t--);
}

void ioinit(void)
{
    PA.DDR = 0x00;          //�|�[�gA�X�C�b�`����
    PB.DDR = 0xff;          //�|�[�gB�o�ͤTIOCA3
}
void ituinit()
{
    ITU3.TCR.BYTE = 0x23;   //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/8
    ITU.TMDR.BIT.PWM3 = 1;  //�`���l��3�@PWM���[�h
    ITU3.GRA =49999;        //�p���X�����@16ms
}

int main(void)
{
    int i,sd;
    ioinit();
    ituinit();
    PB.DR.BIT.B1 = 0;
    ITU.TSTR.BIT.STR3 = 1;             //ITU�X�^�[�g
    while(ITU3.TSR.BIT.IMFA == 0);     //IMFA�t���O�`�F�b�N
    ITU3.TSR.BIT.IMFA = 0;             //IMFA�t���O�N���A
    ITU3.GRB = 4374;                   //�gH�h���x���p���X���������l�A���[�^�p�x����
    wait();
    while(1){                          //�X�C�b�`���͂ɂ��p�x����
        while(ITU3.TSR.BIT.IMFA == 0); //IMFA�t���O�`�F�b�N
        ITU3.TSR.BIT.IMFA = 0;         //IMFA�t���O�N���A
        sd =  PA.DR.BYTE*32;           //�gH�h���x������
        if( sd > 7499)
            sd = 7499;
        if(sd < 1437)
            sd = 1437;
        ITU3.GRB = sd;                 //�|�[�gA�̃X�C�b�`�f�[�^�Z�b�g    
    }
}
