//H8�}�C�R���e�X�g�v���O����
//�|�[�g2�X�C�b�`����͂��|�[�g5��LED�ɏo��
//p_2m.c

#define P2DDR (*(volatile unsigned char*)0xfffc1)
#define P2DR (*(volatile unsigned char*)0xfffc3)
#define P2PCR (*(volatile unsigned char*)0xfffd8)
#define P5DDR (*(volatile unsigned char*)0xfffc8)
#define P5DR (*(volatile unsigned char*)0xfffca)

int main(void)
{
    P2DDR = 0;             //P2��S�r�b�g���͂ɐݒ�
    P2PCR = 0xff;          //P2�v���A�b�v�@�{���Q��
    P5DDR = 0xff;          //P5���o�͂ɐݒ�
    while (1) 
        P5DR = P2DR;       //���͂����f�[�^���o��
}
