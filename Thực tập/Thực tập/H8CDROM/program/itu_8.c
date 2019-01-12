//ITU�ʑ��v�����[�h
//���[�^���G���R�[�_���̃p���X���v����PB�AP4�ɏo��
//���[�^���G���R�[�_�̃p���X��TCLKA(PA0)�ATCLKB(PA1)�ɓ��́@
//P4���8�r�b�g�o�́APB����8�r�b�g�o��
//LED�̓_���Ŋm�F����
//itu_8.c
#include  <3048.h>         //3048�A3052�̓���I/O��`

void ituinit(void)
{
    ITU.TMDR.BIT.MDF = 1;  //ITU2�͈ʑ��v�����[�h
    ITU.TMDR.BIT.FDIR = 1; //�I�[�o�t���[�A�A���_�t���[�Ńt���O�Z�b�g
}

void ioinit(void)
{
    PB.DDR = 0xff;         //�|�[�gB�o��
    P4.DDR = 0xff;         //�|�[�g4�o��     
}

int main(void)
{
    ioinit();
    ituinit();
    ITU.TSTR.BIT.STR2 = 1;          //�^�C�}�X�^�[�g
    while(1){
        P4.DR.BYTE = ITU2.TCNT >> 8;//TCNT�̏��8�r�b�g���o��
        PB.DR.BYTE = ITU2.TCNT;     //TCNT�̉���8�r�b�g���o��
    }
}
