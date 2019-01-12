//H8�}�C�R���e�X�g�v���O����
//DMAC�ɂ��LED�̓_���ړ��A1s����
//�|�[�gB�ɏo��
//�|�[�g4LED�_���ړ��A�ʏ�̃v���O����
//GRA�R���y�A�}�b�`
//�O���N���b�N 1kHz
//�f�[�^��~�t���@DTCR���W�X�^��DTE�r�b�g����
//ITU0�̊��荞�݋֎~������
//dma_2m.c

#include <3048.h>

unsigned char outdata[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //�o�̓f�[�^

void ioinit(void)
{
  
    PB.DDR = 0xff;      //�|�[�gB�S�r�b�g�o��
    P4.DDR = 0xff;      //�|�[�g4�S�r�b�g�o��
}
void dmainit(void)
{
    DMAC0A.MAR = outdata;     //�������A�h���X�A�]�����@�z��
    DMAC0A.IOAR = 0xd6;       //I/O�A�h���X�A�]����@�|�[�gB
    DMAC0A.ETCR = 0x0808;     //�]���f�[�^��
    DMAC0A.DTCR.BIT.RPE = 1;  //���s�[�g���[�h
    DMAC0A.DTCR.BIT.DTE = 1;  //�f�[�^�]������ 
}
 
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA =299;           //�p���X�����@PB LED�_���ړ����x�A��������
    ITU0.TCR.BYTE = 0x24;    //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�O���N���b�N
    ITU0.TIER.BIT.IMIEA = 1; //���荞�݋���
}

void wait(void)       //�|�[�g4LED�̓_���ړ����x�A�x������
{
    long loop=400000;
    while(loop--);
}

int main(void)
{
    int i;
    unsigned char d;
    ioinit();               //I/O�|�[�g�C�j�V�����C�Y
    dmainit();              //DMAC�C�j�V�����C�Y
    ituinit();              //ITU�C�j�V�����C�Y
    ITU.TSTR.BIT.STR0 = 1;  //ITU0�X�^�[�g
    while(1){
        d = 1;
        for(i=0;i<8;i++){         //P4�@LED���܂ňړ�����ԋ���
            P4.DR.BYTE = d;
            wait();
            d<<=1;
        }
        DMAC0A.DTCR.BIT.DTE = 0;  //�f�[�^�]����~
        ITU0.TIER.BIT.IMIEA = 0;  //���荞�݋֎~
        d = 0x80;
        for(i=0;i<8;i++){         //P4�@LED�E�܂ňړ�����Ԓ�~
            P4.DR.BYTE = d;
            wait();
            d>>=1;
        }
        DMAC0A.DTCR.BIT.DTE = 1;  //�f�[�^�]������
        ITU0.TIER.BIT.IMIEA = 1;  //���荞�݋���
    }
}
