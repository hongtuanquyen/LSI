// �`�^�c�R���o�[�^�e�X�g
// AN4���̓��̓A�i���O�M������͂���B�O�u����T�u
// �ϊ����ꂽ�f�B�W�^���M���̏�ʂW�r�b�g���o�S�ɏo��
// ���M���̉��ʂQ�r�b�g���o�a�ɏo��
//�@AD-1.C

#include  <3048.h>            // 3048�̓���I/O��`

void ioinit(void)             //I/O�|�[�g�C�j�V�����C�Y
{
    PB.DDR = 0xff;            // PB��S�r�b�g�o�͂ɐݒ�
    P4.DDR = 0xff;            // P4��S�r�b�g�o�͂ɐݒ�
}

void adinit(void)             //A/D�C�j�V�����C�Y
{
    AD.ADCSR.BIT.ADF = 0;       //ADF�t���O�N���A
    AD.ADCSR.BIT.SCAN = 0;      //�P�ꃂ�[�h�I��
    AD.ADCSR.BIT.CKS = 1;       //�N���b�N�Z���N�g�A�ϊ�����134�X�e�[�g
    AD.ADCSR.BIT.CH = 4;        //�`���l���Z���N�g�@AN4�P�ꃂ�[�h
}

int main(void)
{  
    unsigned int dh,dl;              //�@�ϊ����ꂽ�M�����L������
    ioinit();
    adinit();
    while(1){
        AD.ADCSR.BIT.ADST = 1;         //A/D�ϊ��X�^�[�g
        while(AD.ADCSR.BIT.ADF == 0);  //�ϊ��I����҂�
        dh = (AD.ADDRA & 0xff00) >> 8; //���8�r�b�g��dh�Ɋi�[
        dl = (AD.ADDRA & 0x00ff);      //����8�r�b�g��dl�Ɋi�[
        PB.DR.BYTE = dl;               //���ʃr�b�g��PB�ɏo��
        P4.DR.BYTE = dh;               //��ʃr�b�g��P4�ɏo��
        AD.ADCSR.BIT.ADF = 0;          //�t���O�N���A
    }
}
