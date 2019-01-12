//H8�}�C�R���e�X�g�v���O����
//ITU�ɂ��PWM�o�́A�f���[�e�B�A����
//�������[�^��PWM����A�����A����
//GRA�R���y�A�}�b�` TIOCA3�ɁgH�h�o�́A�J�E���^�N���A
//GRB�R���y�A�}�b�`�ATIOCA3�ɁgL�h�o��
//�����N���b�N�@1/8
//dc_2.c

#include <3048.h>

void ioinit(void)
{
    PB.DDR = 0xff;           //�|�[�gB �o�ͤTIOCA3�PWM�o�͒[�qPB0
}
void ituinit()
{
    ITU3.TCR.BYTE = 0x23;    //GRA�R���y�A�}�b�`�J�E���^�N���A�ATCNT�N���A�A�N���b�N1/8
    ITU.TMDR.BIT.PWM3 = 1;   //�`���l��3�@PWM���[�h
    ITU3.GRA = 7812;         //�p���X�����@2.5ms
    ITU3.GRB =400;           //�gH�h���x���p���X���������l
}

int main(void)
{
    int i;
    ioinit();
    ituinit();
    PB.DR.BIT.B1 = 0;
    ITU.TSTR.BIT.STR3 = 1;                 //ITU�X�^�[�g
    while(1){
        for(i=400;i<7813;i++){             //���� �gH�h���x��������1/20����1/1�܂�
            while(ITU3.TSR.BIT.IMFA == 0); //IMFA�t���O�`�F�b�N
            ITU3.TSR.BIT.IMFA = 0;         //IMFA�t���O�N���A
            ITU3.GRB = i;                  //�gH�h�p���X���f�[�^��������
        }
        for(i=7813;i>400;i--){             //���� �gH�h���x���������炵�Ă���
            while(ITU3.TSR.BIT.IMFA == 0); //IMFA�t���O�`�F�b�N
            ITU3.TSR.BIT.IMFA = 0;         //IMFA�t���O�N���A
            ITU3.GRB = i;                  //�gH�h�p���X���f�[�^��������
        }      
    }
}
