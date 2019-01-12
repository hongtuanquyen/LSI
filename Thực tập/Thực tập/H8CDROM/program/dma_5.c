//H8�}�C�R���e�X�g�v���O����
//DMAC���8���p���X�̏o��
//�m���I�[�o���b�v8���p���X�A�|�[�gB LED�_���ړ�
//GRA�R���y�A�}�b�`�@�m���I�[�o���b�v���� LED����
//GRB�R���y�A�}�b�`�@�p���X����
//�O���N���b�N�@1kHz
//dma_5.c

#include <3048.h>

unsigned char outdata[8] ={0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x01}; //TPC�o�̓f�[�^

void ioinit(void)
{
    PA.DDR = 0xfe;            //�|�[�gA�r�b�g0���́A�O���N���b�N����
    PB.DR.BYTE = 0x00;        //�o�̓f�[�^�����l
    PB.DDR = 0xff;            //�|�[�gB�S�r�b�g�o��
}

void dmainit(void)
{
    DMAC0A.MAR = outdata;     //�������A�h���X�A�]����
    DMAC0A.IOAR = 0xa4;       //I/O�A�h���X�A�]���� NDRB���W�X�^
    DMAC0A.ETCR = 0x0808;     //�]���f�[�^��
    DMAC0A.DTCR.BIT.RPE = 1;  //���s�[�g���[�h
    DMAC0A.DTCR.BIT.DTE = 1;  //�f�[�^�]������
}

void ituinit()
{
    ITU0.TIOR.BIT.IOA = 0;    //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.TIOR.BIT.IOB = 0;    //�R���y�A�}�b�`�ɂ��o�͋֎~
    ITU0.GRA = 999;           //�m���I�[�o���b�v���� 1s
    ITU0.GRB = 1999;          //�p���X���� 2s
    ITU0.TCR.BYTE = 0x44;     //GRB�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/8
    ITU0.TIER.BIT.IMIEA = 1;  //���荞�݋���
}

void tpcinit(void)
{
    
    TPC.TPMR.BIT.G2NOV = 1;   //TPC�o�̓O���[�v2�̓m���I�[�o���b�v����
    TPC.TPMR.BIT.G3NOV = 1;   //TPC�o�̓O���[�v3�̓m���I�[�o���b�v����
    TPC.NDERB.BYTE = 0xff;    //TPC15����TPC8�o�͂�����
    TPC.TPCR.BIT.G2CMS = 0;   //TP11����TP8�̏o�̓g���KITU0�R���y�A�}�b�`
    TPC.TPCR.BIT.G3CMS = 0;   //TP15����TP12�̏o�̓g���KITU0�R���y�A�}�b�`
    TPC.NDRB1.BYTE = 0x01;    //1��ڏo�̓f�[�^
}

int main(void)
{
    ioinit();
    dmainit();
    tpcinit();
    ituinit();
    ITU.TSTR.BIT.STR0 = 1;     //ITU0�X�^�[�g
    while(1);
}
