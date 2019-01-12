//H8�}�C�R���e�X�g�v���O����
//LCD�\���v���O����
//�������\������
//lcd_2.c 
#include "io.h"
#define  E_SIG  0x20            //E�M���r�b�g
#define  RS_SIG 0x10            //RS�M���r�b�g
 
void wait(void)                 //���ԑ҂��֐�
{
    int t = 1000;
    while(t--);
}

void lcdo8(unsigned char d)     //8�r�b�g�o�͊֐�
{
    d=d | E_SIG;                //�󂯎�����f�[�^��E�M����������
    P3DR = d;                   //LCD�ɏo��
    d = d & 0xdf;               //E�M����0�ɂ���
    P3DR = d;                   //LCD�ɏo��
    wait();                     //�m�肷��܂ł̑҂�����
}

void lcdini(void)               //�������֐�
{
    lcdo8(3);                   //00000011���o��
    lcdo8(3);                   //3��J��Ԃ�
    lcdo8(3);      
    lcdo8(2);                   //00000010���o��
    lcdo8(2);                   //�J��Ԃ�
    lcdo8(8);                   //00001000���o��
    lcdo8(0);      
    lcdo8(0x0c);                //00001100���o��
    lcdo8(0);      
    lcdo8(6);                   //00000110���o��
}

void lcdclr(void)               //�\���N���A�֐�
{
    P3DR = 0;
    lcdo8(0);
    lcdo8(1);                   //00000001�ŃN���A
    wait();
}

void lcdxy1(void)               //�J�[�\����1�s�ڂɈړ�
{
    P3DR = 0;
    lcdo8(8);                   //1�s��
    lcdo8(0);                   //�擪�ʒu
    wait();
}

void lcdxy2(void)               //�J�[�\����2�s�ڂɈړ�
{
    P3DR = 0;
    lcdo8(0x0c);                //2�s��
    lcdo8(0);                   //�擪�ʒu
    wait();
}

void lcdo4(unsigned char d)     //�����o�͊֐�
{
    unsigned dd;
    dd = d;                     //�\���f�[�^�ۑ�
    d = d >> 4;                 //���4�r�b�g������4�r�b�g�Ɉڂ�
    d = d & 0x0f;               //����4�r�b�g���o��
    d = d | RS_SIG;             //RS�M���ƍ���
    lcdo8(d);                   //LCD�ɏo��
    wait();                     //���ԑ҂�
    dd = dd & 0x0f;             //����4�r�b�g��肾��
    dd = dd | RS_SIG;           //RS�M���ƍ���
    lcdo8(dd);                  //LCD�ɏo��
    wait();
}

void dsp1g(char *str)           //1�s�\���֐��@*str�͕�����ϐ�
{
    int c = 16;                 //16����
    for(c = 0;c < 16;c++){      //16�����\��
    lcdo4(*str);                //�\���֐��R�[��
    str++;                      //����������̕����ɂ���
    }
}

int main(void)
{
    P3DDR = 0xff;               //�|�[�g3��S�r�b�g�o�͂ɐݒ�
    wait();                     //�d��ON��15ms�ȏ�҂�
    lcdini();                   //LCD���C�j�V�����C�Y����
    wait();
    lcdclr();                   //�\���N���A
    lcdxy1();                   //�\���ʒu
    dsp1g("ABCDEFGHIJKLMNOP");  //�\��������
    wait();
    lcdxy2();                   //2�s�ڎw��
    dsp1g("QRSTUVWXYZ012345");  //�\��������
    while(1);                   //�I���A�i�v���[�v
}
