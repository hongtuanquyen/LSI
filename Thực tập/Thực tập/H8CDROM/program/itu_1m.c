//H8�}�C�R���e�X�g�v���O����
//�^�C�}���荞�ݎg�p���Ȃ��A�t���O�����܂ő҂�
//ITU0�g�p�@
//20ms���ƂɃJ�E���^�N���A�A50��̃N���A��LED�_�����]
//itu_1m.c

#include  <3048.h>                  //3048�A3052�̓���I/O��`

int main(void)
{
    int  c=50;                      //���荞�݉�
    P5.DDR = 0xff;
    ITU0.TCR.BIT.CCLR    = 1;       //�J�E���^�N���A�v��
    ITU0.TCR.BIT.TPSC    = 3;       //�^�C�}�v���X�P�[��25MHz/8=3.125MHz
    ITU0.GRA        = 62499;        //3.125MHz/62,500=50Hz ����20ms
    ITU.TSTR.BIT.STR0 = 1;          //�^�C�}�X�^�[�g
    P5.DR.BYTE = 0xff;              //LED�o�̓f�[�^�����l
    while(1){   
         while(!ITU0.TSR.BIT.IMFA);    
         c-=1;
         if(c==0){
             P5.DR.BYTE = ~P5.DR.BYTE; //LED�o�̓f�[�^�𔽓]
             c = 50;                //���荞�݉񐔍Đݒ�
         }
         ITU0.TSR.BIT.IMFA = 0;     //���荞�݃X�e�[�^�X�t���O�N���A
    }
}
