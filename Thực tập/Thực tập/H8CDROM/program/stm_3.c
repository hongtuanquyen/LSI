//�X�e�b�s���O���[�^�̉�]
//ITU0�g�p�A�t���O�����̂�҂@
//5ms���ƂɃ��[�^�ɏo��
//stm_3.c
#include  <3048.h>               //3048�A3052�̓���I/O��`

void ioinit(void)
{
    PB.DDR = 0xff;
}

void ituinit(void)
{
    ITU0.TCR.BIT.CCLR    = 1;     //�J�E���^�N���A�v��
    ITU0.TCR.BIT.TPSC    = 3;     //�^�C�}�v���X�P�[��25MHz/8=3.125MHz
    ITU0.GRA       = 15624;       //3.125MHz/15625=200Hz�A5ms�A200pps
    ITU0.TIER.BIT.IMIEA = 0;      //IMFA�t���O�ɂ�銄�荞�݋֎~
}

int main(void)
{
    int md = 0x11;                //���[�^�o�̓f�[�^�����l
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;        //�^�C�}�X�^�[�g  
    while (1){
        PB.DR.BYTE = md;
            md <<= 1;
            if(md == 0x110)        //4��V�t�g�������̔��f
                md = 0x11;         //4��V�t�g�ŏ����l�ɖ߂�
        while(!ITU0.TSR.BIT.IMFA); //ITUO IMFA�t���O�����̂�҂�
        ITU0.TSR.BIT.IMFA = 0;     //�t���O�N���A
    }
}
