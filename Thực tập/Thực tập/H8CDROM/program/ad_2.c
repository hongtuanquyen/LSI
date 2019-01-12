//A/D�R���o�[�^�e�X�g
//AN0����AN3���̃A�i���O�M������͂���B�X�L�������[�h
//�ϊ����ꂽ�f�B�W�^���M���̏��8�r�b�g��LED�ɏo��
//AN0-PA�AAN1-PB�AAN2-P4�AAN3-P5�ɏo��
//ad_2.c

#include  <3048.h>                         //3048�A3052�̓���I/O��`

void ioinit(void)
{
    PA.DDR = 0xff;                         //PA��S�r�b�g�o�͂ɐݒ�
    PB.DDR = 0xff;                         //PB��S�r�b�g�o�͂ɐݒ�
    P4.DDR = 0xff;                         //P4��S�r�b�g�o�͂ɐݒ�
    P5.DDR = 0xff;                         //P5��S�r�b�g�o�͂ɐݒ�
}

void adinit(void)
{
    AD.ADCSR.BIT.ADF = 0;                  //ADF�t���O�N���A
    AD.ADCSR.BIT.SCAN = 1;                 //�X�L�������[�h�I��
    AD.ADCSR.BIT.CKS = 1;                  //�N���b�N�Z���N�g
    AD.ADCSR.BIT.CH = 3;                   //�`���l���Z���N�g�@AN0-AN3�X�L�������[�h
    AD.ADCSR.BIT.ADST = 1;                 //A/D�ϊ��X�^�[�g
}

int main(void)
{
    unsigned int dh;                       //�ϊ����ꂽ�M�����L������
    ioinit();
    adinit();
    while(1){
        while(AD.ADCSR.BIT.ADF == 0);      //�ϊ��I����҂�
        dh = (AD.ADDRA & 0xff00) >> 8;     //���8�r�b�g��dh�Ɋi�[�AAN0
        PA.DR.BYTE = dh;                   //��ʃr�b�g��PA�ɏo��
        dh = (AD.ADDRB & 0xff00) >> 8;     //���8�r�b�g��dh�Ɋi�[�AAN1
        PB.DR.BYTE = dh;                   //��ʃr�b�g��PB�ɏo��
        dh = (AD.ADDRC & 0xff00) >> 8;     //���8�r�b�g��dh�Ɋi�[�AAN2
        P4.DR.BYTE = dh;                   //��ʃr�b�g��P4�ɏo��
        dh = (AD.ADDRD & 0xff00) >> 8;     //���8�r�b�g��dh�Ɋi�[�AAN3
        P5.DR.BYTE = dh;                   //��ʃr�b�g��P5�ɏo��     
        AD.ADCSR.BIT.ADF = 0;              //�t���O�N���A
    }
}
