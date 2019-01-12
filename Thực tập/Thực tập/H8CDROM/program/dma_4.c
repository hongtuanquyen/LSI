//H8�}�C�R���e�X�g�v���O����
//DMAC���8���p���X�̏o��
//�m���I�[�o���b�v8���p���X
//GRA�R���y�A�}�b�`�@�m���I�[�o���b�v����
//GRA�R���y�A�}�b�`�@�p���X����
//�����N���b�N 1/1
//dma_4.c

#include <3048.h>

unsigned char outdata[8] = {0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x01}; //TPC�o�̓f�[�^

void ioinit(void)
{
    PA.DR.BYTE = 0x00;       //�o�̓f�[�^�����l
    PA.DDR = 0xff;           //�|�[�gA�S�r�b�g�o��
}

void dmainit(void)
{
    DMAC0A.MAR = outdata;    //�������A�h���X�A�]����
    DMAC0A.IOAR = 0xa5;      //I/O�A�h���X�A�]���� NDRA���W�X�^
    DMAC0A.ETCR = 0x0808;    //�]���f�[�^��
    DMAC0A.DTCR.BIT.RPE = 1; //���s�[�g���[�h
    DMAC0A.DTCR.BIT.DTE = 1; //�f�[�^�]������
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;   //GRA�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.TIOR.BIT.IOB = 0;   //GRB�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA =4999;          //�m���I�[�o���b�v���� 20%
    ITU0.GRB =24999;         //�p���X����  1ms �m���I�[�o���b�v���Ԋ܂�
    ITU0.TCR.BYTE = 0x40;    //GRB�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/1
    ITU0.TIER.BIT.IMIEA = 1; //���荞�݋���
}

void tpcinit(void)
{
    TPC.TPMR.BIT.G0NOV = 1;  //TPC�o�̓O���[�v0�̓m���I�[�o���b�v����
    TPC.TPMR.BIT.G1NOV = 1;  //TPC�o�̓O���[�v1�̓m���I�[�o���b�v����
    TPC.NDERA.BYTE = 0xff;   //TPC7����TPC0�o�͂�����
    TPC.TPCR.BIT.G0CMS = 0;  //TP3����TP0�̏o�̓g���KITU0�R���y�A�}�b�`
    TPC.TPCR.BIT.G1CMS = 0;  //TP7����TP3�̏o�̓g���KITU0�R���y�A�}�b�`
    TPC.NDRA1.BYTE = 0x01;   //1��ڏo�̓f�[�^
}

int main(void)
{
    ioinit();
    dmainit();
    tpcinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;    //ITU0�X�^�[�g
    while(1);
}
