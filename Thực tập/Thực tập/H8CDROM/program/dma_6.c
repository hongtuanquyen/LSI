//H8�}�C�R���e�X�g�v���O����
//I/O���[�h�@
//DMAC�ɂ��LED�̓_���ړ��A1s����
//�|�[�gB�ɏo��
//�|�[�g4LED�_�����ړ��A�ʏ�̃v���O����
//�]���I���ŉE�ړ�
//GRA�R���y�A�}�b�`
//�O���N���b�N�@1kHz
//dma_6.c

#include <3048.h>

unsigned char outdata[9] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00}; //�o�̓f�[�^
unsigned char flg;            //���荞�݂ŃZ�b�g����t���O�ϐ�

void ioinit(void)
{
  
    PB.DDR = 0xff;            //�|�[�gB�S�r�b�g�o��
    P4.DDR = 0xff;            //�|�[�g4�S�r�b�g�o��
}

void dmainit(void)
{
    DMAC0A.MAR = outdata;     //�������A�h���X�A�]�����@�z��
    DMAC0A.IOAR = 0xd6;       //I/O�A�h���X�A�]����@�|�[�gB
    DMAC0A.ETCR = 0x0009;     //�]���f�[�^��9�A���[�h�f�[�^�ɂȂ�
    DMAC0A.DTCR.BIT.DTSZ = 0; //�o�C�g�T�C�Y�]��
    DMAC0A.DTCR.BIT.DTID = 0; //�f�[�^�]����MAR���C���N�������g
    DMAC0A.DTCR.BIT.RPE = 0;  //I/O���[�h
    DMAC0A.DTCR.BIT.DTIE = 1; //�]���I�������荞�݋���
    DMAC0A.DTCR.BIT.DTE = 1;  //�f�[�^�]������
}
 
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;    //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA =999;            //�p���X�����@1s
    ITU0.TCR.BYTE = 0x24;     //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�O���N���b�N
    ITU0.TIER.BIT.IMIEA = 1;  //���荞�݋���
}

void wait(void)               //main�v���O������LED�_���ړ�����
{
    long loop=100000;
    while(loop--);
}

void int_dend0a(void)          //�]���I�����荞��
{  
    flg = 1;                   //�]���I���Ńt���O�Z�b�g
    ITU0.TIER.BIT.IMIEA = 0;   //���荞�݋֎~
    DMAC0A.DTCR.BIT.DTIE = 0;  //�]���I�������荞�݋֎~
}

int main(void)
{
    int i;
    flg = 0;
    unsigned char d;
    ioinit();                  //I/O�|�[�g�C�j�V�����C�Y
    dmainit();                 //DMAC�C�j�V�����C�Y
    ituinit();                 //ITU�C�j�V�����C�Y
    ITU.TSTR.BIT.STR0 = 1;     //ITU0�X�^�[�g
    EI;
    d = 1;
    while(flg == 0){           //�]�����̃��[�`���A�t���O���Z�b�g�����܂�
        d = 1;
        for(i=0;i<8;i++){
            P4.DR.BYTE = d;
                d <<= 1;
            wait();
        }
    }
    while(1){                 //�]���I����̃��[�`��
        d = 0x80;
        for(i=0;i<8;i++){
            P4.DR.BYTE = d;
                d >>= 1;
            wait();
        }
    }          
}
