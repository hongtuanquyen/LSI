//H8�}�C�R���e�X�g�v���O����
//�|�[�g4����͂ɐݒ� �����{�^���X�C�b�`
//�|�[�g5���o�͂ɐݒ�@LED
//�|�[�g4�����͂����f�[�^���|�[�g5�ɏo��
//p_3.c 

#define P4DDR (*(volatile unsigned char*)0xfffc5)
#define P4DR (*(volatile unsigned char*)0xfffc7)
#define P4PCR (*(volatile unsigned char*)0xfffda)
#define P5DDR (*(volatile unsigned char*)0xfffc8)
#define P5DR (*(volatile unsigned char*)0xfffca)

int main(void)
{
    int d;
    P4DDR = 0;       //�|�[�g4����
    P4PCR = 0xff;    //�v���A�b�vMOS�@ON
    P5DDR = 0xff;    //�|�[�g5�o��
    while(1){    
        d = P4DR;    //�f�[�^����
        d = d >> 4;  //4�r�b�g�E�ɃV�t�g
        P5DR = d;
    }
}
