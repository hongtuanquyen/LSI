//H8�}�C�R���e�X�g�v���O����
//�|�[�g5��LED�_��
//p_1.c

#define P5DDR (*(volatile unsigned char*)0xfffc8)
#define P5DR (*(volatile unsigned char*)0xfffca)

//�_�Ŏ��ԊԊu
void wait (long loop)
{
    while (loop--);
}

int main(void)
{
    P5DDR = 0xff;        //P5��S�r�b�g�o�͂ɐݒ�
    while (1) {          //�J��Ԃ����s
        P5DR = 0xff;     //�_��
        wait (200000);   //�_������
        P5DR = 0;        //����
        wait(200000);    //��������
    }
}
