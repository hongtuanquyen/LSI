//�X�e�b�s���O���[�^�̉�]
//�E2��]�A��~�A��1��]�A��~ �����3��J��Ԃ��B
//ITU0�g�p�A�t���O�����̂�҂@
//10ms���ƂɃ��[�^�ɏo��
//stm_4.c
#include  <3048.h>                //3048�A3052�̓���I/O��`

void ioinit(void)
{
    PB.DDR = 0xff;
}

void ituinit(void)
{
    ITU0.TCR.BIT.CCLR    = 1;     //�J�E���^�N���A�v��
    ITU0.TCR.BIT.TPSC    = 3;     //�^�C�}�v���X�P�[��25MHz/8=3.125MHz
    ITU0.GRA = 31249;             //3.125MHz/31250=100Hz�A10ms�A100pps
    ITU0.TIER.BIT.IMIEA = 0;      //IMFA�t���O�ɂ�銄�荞�݋֎~
}

void wait(void)                   //��~����
{
    long t=200000;
    while(t--);
}

int main(void)
{
    int i = 3;                    //�J��Ԃ��J�E���^
    int p;                        //��]�p���X��
    int md = 0x11;                //���[�^�o�̓f�[�^
    ioinit();
    ituinit();
    PB.DR.BYTE = md;
    wait();
    ITU.TSTR.BIT.STR0 = 1;        //�^�C�}�X�^�[�g
    while(i--){
        for(p=0;p<96;p++){        //2��]
            PB.DR.BYTE = md;
                md <<= 1;         //�E��]
                if(md == 0x110)   //4��V�t�g�������̔��f
                    md = 0x11;         //4��V�t�g�ŏ����l�ɖ߂�
            while(!ITU0.TSR.BIT.IMFA); //�t���O�����̂�҂�
            ITU0.TSR.BIT.IMFA = 0;     //�t���O�N���A
        }
        wait();                        //������~
        for(p=50;p>0;p--){             //1��]�A1�p���X��������
            PB.DR.BYTE = md;
                md >>= 1;              //����]
                if(md == 0x08)         //4��V�t�g�������̔��f
                    md = 0x88;         //4��V�t�g�ŏ����l�ɖ߂�
            while(!ITU0.TSR.BIT.IMFA); //�t���O�����̂�҂�
            ITU0.TSR.BIT.IMFA = 0;     //�t���O�N���A
        }
        wait();
    }    
    PB.DR.BYTE = 0x00;                 //���[�^�㎥OFF
    while(1);                          //�����Ŏ~�܂��Ă���
}
