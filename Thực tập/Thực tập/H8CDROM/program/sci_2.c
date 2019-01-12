//H8�e�X�g�v���O����
//�ʐM�����ASCI�|�[�g�ƃp�\�R���Ԃ̒ʐM
//�p�\�R�����n�C�p�[�^�[�~�i��
//H8�X�^�[�g�Ń��b�Z�[�W���M
//��M�͊��荞�ݓ���
//�p�\�R���ɑ���Ԃ��ƂƂ��ɕ����R�[�h��LED�ɏo��
//main�̒��̓|�[�g4LED�_��
//sci_2.c

#include <3048.h>

//�r�b�g���[�g�ݒ胊�X�g  �N���b�N16MHz��(3048F)
/*typedef enum {
    BR2400   = 207,  //n=0�ASMR�̐ݒ�
    BR4800   = 103,
    BR9600   = 51,
    BR19200  = 25,
    BR38400  = 12,
    BR57600  = 8,
    BR115200 = 3,
} BaudRate;*/

//�r�b�g���[�g�ݒ胊�X�g  �N���b�N25MHz��(3052F)
//25MHz�N���b�N�̏ꍇ�͂�������g�p
typedef enum {
    BR2400   = 80,   //n=1�ASMR�̐ݒ�
    BR4800   = 162,  //n=0�ASMR�̐ݒ�,�ȉ����ׂ�0
    BR9600   = 80,   
    BR19200  = 40,   
    BR38400  = 19,   
    BR57600  = 13,   
    BR115200 = 5,    
} BaudRate;

//SCI������
void initSCI1 (BaudRate b)            //�r�b�g���[�g���󂯎��
{
    int i;
    SCI1.SCR.BYTE = SCI1.SMR.BYTE = 0;//�����l��0�A�{���Q��
    SCI1.BRR = b;                     //�r�b�g���[�g
    for (i = 0; i <4000; i++) ;       //�҂�
    SCI1.SCR.BYTE = 0x70;             //��M���荞�݋��A����M����
    SCI1.SSR.BYTE;                    //�_�~�[���[�h
    SCI1.SSR.BYTE = 0x80;             //�G���[�t���O�N���A
}

//1�������M
void SCI1_OUT_d (char c)              //�����R�[�h���󂯎��
{
    while (SCI1.SSR.BIT.TDRE == 0) ;  //0�Ńf�[�^����A1�ɂȂ�܂ő҂�
    SCI1.TDR = c;                     //�󂯎���������𑗐M
    SCI1.SSR.BIT.TDRE = 0;            //TDRE�r�b�g���N���A
}

//�����񑗐M
void SCI1_OUT_s (char *st)            //��������󂯎��
{
    while (*st) SCI1_OUT_d(*st++);    //1�������M�֐��Ăяo��
}

//�G���[���荞��
void int_eri1(void)
{
    SCI1.SSR.BYTE &= 0x80;            //�G���[�Ȃ� �G���[�t���O�N���A
}

//��M���荞��
void int_rxi1(void)
{
    char rd;                          //��M�f�[�^�i�[�ϐ�
    rd = SCI1.RDR;                    //��M�f�[�^�i�[
    SCI1_OUT_d(rd);                   //�G�R�[�o�b�N�A1�������M�Ăяo��
    PB.DR.BYTE = rd;                  //LED�ɏo�� 
    SCI1.SSR.BIT.RDRF = 0;            //��M�t���O�N���A
}

void ioinit(void)
{
    PB.DDR = 0xff;                    //�|�[�gB�S�r�b�g�o��
    P4.DDR = 0xff;                    //�|�[�g4�S�r�b�g�o��
}

void wait(void)
{
    long t = 400000;
    while(t--);
}

int main(void)
{
    ioinit();   
    initSCI1(BR19200);                     //SCI������
    SCI1_OUT_s(" HELLO PC this is H8!!");  //H8���p�\�R���ɑ��M
    EI;
    while(1){                              //�|�[�g4LED�_��
        P4.DR.BYTE = 0xff;                 //���C���֐��ɂ͎�M�����͂Ȃ�
        wait();
        P4.DR.BYTE = 0;
        wait();
    }
}
