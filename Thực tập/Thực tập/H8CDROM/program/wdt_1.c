//H8�}�C�R���e�X�g�v���O����
//WDT�e�X�g
//main�v���O�����@�|�[�gB LED���ݓ_��
//�|�[�gA�@�r�b�g0�@�g0�h��WDT�J�E���^�N���A
//�|�[�gA�@�r�b�g0�@�g1�h��WDT�J�E���^�N���A���Ȃ��B
//WDT�I�[�o�t���[�Ń��Z�b�g�A�v���O�����i�܂Ȃ�
//wdt_1.c

#include <3048.h>

void ioinit(void)
{
    PA.DDR = 0;               //�|�[�gA����
    PB.DDR = 0xff;            //�|�[�gB�o��
}

void wdtinit(void)
{
    WDT.WRITE.RSTCSR = 0x5a40;//���Z�b�g�o�̓C�l�[�u��
    WDT.WRITE.TCSR = 0xa566;  //WDT�I���J�E���g����A1/2048
}

int main(void)
{
    long c =80000;
    unsigned char d = 0x55;
    ioinit();
    PB.DR.BYTE = 0x00;              //�ŏ��̎��s�ALED����
    wdtinit();
    while(1){
        c=c-1;
        if(PA.DR.BIT.B0 ==0)        //�X�C�b�`OFF��WDT�N���A
            WDT.WRITE.TCSR = 0x5a00;//WDT�^�C�}�J�E���^�N���A�ATCSR�Ɠ����A�h���X
        if(c == 0){                 //WDT�I�[�o�t���[�ł��̒��͎��s����Ȃ�
            PB.DR.BYTE = d;
            d = ~d;
            c = 80000;
        }
    }
}
