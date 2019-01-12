//H8�}�C�R���e�X�g�v���O����
//DMAC�ɂ��4���p���X�̏o��
//�|�[�gA�ɏo��
//GRA�R���y�A�}�b�`
//�����N���b�N�@1/1
//dma_1.c

#include <3048.h>

unsigned char outdata[4] = {0x11,0x22,0x44,0x88}; //�o�̓f�[�^

void ioinit(void)
{  
    PA.DDR = 0xff;            //�|�[�gA�S�r�b�g�o��
}

void dmainit(void)
{
    DMAC0A.MAR = outdata;     //�������A�h���X�A�]����
    DMAC0A.IOAR = 0xd3;       //I/O�A�h���X�A�]���� PA
    DMAC0A.ETCR = 0x0404;     //�]���f�[�^�� 4��
    
    DMAC0A.DTCR.BIT.DTSZ = 0; //�o�C�g�T�C�Y�]��
    DMAC0A.DTCR.BIT.DTID = 0; //�f�[�^�]����MAR���C���N�������g
    DMAC0A.DTCR.BIT.RPE = 1;  //���s�[�g���[�h 
    DMAC0A.DTCR.BIT.DTIE = 0; //���s�[�g���[�h
    DMAC0A.DTCR.BIT.DTS = 0;  //ITU0�R���y�A�}�b�`�Ŋ��荞�݋N��
                              //�ȏ�ŉ��ʂV�r�b�g��0010000
    DMAC0A.DTCR.BIT.DTE = 1;  //�f�[�^�]������
}
 
void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;    //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA =12499;          //�p���X�����@0.5ms
    ITU0.TCR.BYTE = 0x20;     //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/1
    ITU0.TIER.BIT.IMIEA = 1;  //���荞�݋���
}

int main(void)
{
    ioinit();                  //I/O�|�[�g�C�j�V�����C�Y
    dmainit();                 //DMAC�C�j�V�����C�Y
    ituinit();                 //ITU�C�j�V�����C�Y
    ITU.TSTR.BIT.STR0 = 1;     //ITU0�X�^�[�g
    while(1);                  //�������Ȃ�
}
