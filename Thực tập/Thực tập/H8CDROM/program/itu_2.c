//�^�C�}���荞�݁@����2
//ITU0�AITU1�g�p�@
//ITU0�@20ms���Ɗ��荞�݁A50��̊��荞�݂Ńr�b�g0LED�_�����]
//ITU1�@10ms���Ɗ��荞�݁A50��̊��荞�݂Ńr�b�g1LED�_�����]
//itu_2.c

#include  <3048.h>                    //3048�A3052�̓���I/O��`

int c0,c1;                            //���荞�݉񐔁A�O���ϐ��Ƃ���

//ITU0�C���^�[�o���^�C�}���荞��
void int_imia0 (void)
{
    ITU0.TSR.BIT.IMFA = 0;            //���荞�݃X�e�[�^�X�t���O�N���A
    c0-=1;
    if(c0==0){
        P5.DR.BIT.B0 = ~P5.DR.BIT.B0; //LED�o�̓f�[�^�𔽓]
        c0=50;
    }
}

//ITU1�C���^�[�o���^�C�}���荞��
void int_imia1 (void)
{
    ITU1.TSR.BIT.IMFA = 0;             //���荞�݃X�e�[�^�X�t���O�N���A
    c1-=1;
    if(c1==0){
        P5.DR.BIT.B1 = ~P5.DR.BIT.B1;  //LED�o�̓f�[�^�𔽓]
        c1=50;
    }
}

int main(void)
{
    P5.DDR = 0xff;
    ITU0.TCR.BIT.CCLR    = 1;          //�J�E���^�N���A�v��
    ITU0.TCR.BIT.TPSC    = 3;          //�^�C�}�v���X�P�[��25MHz/8=3.125MHz
    ITU0.GRA        =62499;            //3.125MHz/62,500=50Hz�A����20ms
    ITU0.TIER.BIT.IMIEA = 1;           //IMFA�t���O�ɂ�銄�荞�݋���
    ITU1.TCR.BIT.CCLR    = 1;          //�J�E���^�N���A�v��
    ITU1.TCR.BIT.TPSC    = 3;          //�^�C�}�v���X�P�[��25MHz/8=3.125MHz
    ITU1.GRA        =31249;            //3.125MHz/31,250=100Hz�A����10ms
    ITU1.TIER.BIT.IMIEA = 1;           //IMFA�t���O�ɂ�銄�荞�݋���
    ITU.TSTR.BIT.STR0 = 1;             //�^�C�}0�X�^�[�g
    ITU.TSTR.BIT.STR1 = 1;             //�^�C�}1�X�^�[�g
    P5.DR.BYTE = 0x03;                 //LED�o�̓f�[�^�����l
    c0 = c1 = 50;                      //���荞�݉�
    EI;                                //���荞�݋���
    while (1);                         //�������Ȃ�
}
