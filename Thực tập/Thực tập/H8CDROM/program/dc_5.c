//H8�}�C�R���e�X�g�v���O����
//ITU�ɂ��PWM�o��
//RC�T�[�{���[�^�̐���
//A/D�ϊ��f�[�^�ŉ�]�p�𐧌�
//�ŏ������Œ�~�A����A/D�ϊ��f�[�^�ŉ�]
//GRA�R���y�A�}�b�`�ATIOCA3�ɁgH�h�o�́A�J�E���^�N���A
//GRB�R���y�A�}�b�`�ATIOCA3�ɁgL�h�o��
//�����N���b�N�@1/8
//dc_5.c

#include <3048.h>

void wait(void)                  //���[�^��]���Ԋm��
{
    long  t = 800000;
    while(t--);
}

void ioinit(void)
{
    PB.DDR = 0xff;               //�|�[�gB �o�ͤTIOCA3
}
void ituinit()
{
    ITU3.TCR.BYTE = 0x23;        //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/8
    ITU.TMDR.BIT.PWM3 = 1;       //�`���l��3�@PWM���[�h
    ITU3.GRA =49999;             //�p���X�����@16ms
}
void adinit(void)                //A/D�C�j�V�����C�Y
{
    AD.ADCSR.BIT.ADF = 0;          //ADF�t���O�N���A
    AD.ADCSR.BIT.SCAN = 0;         //�P�ꃂ�[�h�I��
    AD.ADCSR.BIT.CKS = 1;          //�N���b�N�Z���N�g�A�ϊ�����134�X�e�[�g
    AD.ADCSR.BIT.CH = 4;           //�`���l���Z���N�g�@AN4�P�ꃂ�[�h
}

int main(void)
{
    unsigned int sd;
    ioinit();
    ituinit();
    adinit();
    PB.DR.BIT.B1 = 0;
    ITU.TSTR.BIT.STR3 = 1;            //ITU�X�^�[�g
    while(ITU3.TSR.BIT.IMFA == 0);    //IMFA�t���O�`�F�b�N
    ITU3.TSR.BIT.IMFA = 0;            //IMFA�t���O�N���A
    ITU3.GRB = 4374;                  //�gH�h���x���p���X���������l�A���[�^�p�x����
    wait();
    while(1){                         //A/D���͂ɂ��p�x����
        AD.ADCSR.BIT.ADST = 1;          //A/D�ϊ��X�^�[�g
        while(AD.ADCSR.BIT.ADF == 0);   //�ϊ��I����҂�
        AD.ADCSR.BIT.ADF = 0;           //�t���O�N���A 
        sd =AD.ADDRA >> 2;              //�K���ȃf�[�^�ɕϊ����邽��
        while(ITU3.TSR.BIT.IMFA == 0);//IMFA�t���O�`�F�b�N
        ITU3.TSR.BIT.IMFA = 0;        //IMFA�t���O�N���A
        if(sd > 7499)
            sd = 7499;
        if(sd < 1437)
            sd = 1437;
        ITU3.GRB = sd;                //��]�p�f�[�^���Z�b�g   
    }
}
