//H8�}�C�R���e�X�g�v���O����
//LCD�\���v���O�����A�w�肵���ꏊ�ɕ\������
//lcd_1.c 

#include "io.h"
#define  E_SIG  0x20       //E�M���r�b�g
#define  RS_SIG 0x10       //RS�M���r�b�g
 
void wait(void)            //�҂����Ԋ֐�
{
    int  t= 1000;          //3048F�ł�800�܂ŉ�������
    while(t--);
}

void lcdo8(unsigned char d)	//8�r�b�g�o�͊֐�
{
    d = d | E_SIG;          //�󂯎�����f�[�^�ɂd�M����������
    P3DR = d;               //LCD�ɏo��
    d = d & 0xdf;           //E�M����0�ɂ���
    P3DR = d;               //LCD�ɏo��
    wait();                 //�m�肷��܂ł̑҂�����
}

void lcdini(void)	          //�������֐�
{
    lcdo8(3);               //00000011���o��
    lcdo8(3);               //3��J��Ԃ�
    lcdo8(3);      
    lcdo8(2);               //00000010���o��
    lcdo8(2);               //�J��Ԃ�
    lcdo8(8);               //00001000���o��
    lcdo8(0);      
    lcdo8(0x0c);            //00001100���o��
    lcdo8(0);      
    lcdo8(6);               //00000110���o��
}

void lcdclr(void)	          //LCD�N���A�֐�
{
    P3DR = 0;
    lcdo8(0);
    lcdo8(1);               //00000001�ŃN���A
    wait();
}

void lcdxy(unsigned char y,unsigned char x)	//�J�[�\�����ړ�
{
    P3DR = 0;
    lcdo8(y);               //1�s�ځ@y=8�A2�s�ځ@y=0x0c
    lcdo8(x);               //0����15�0�ň�ԍ�
    wait();
}

void lcdo4(unsigned char d)	//�����o�͊֐�
{
    unsigned dd;
    dd = d;                 //�\���f�[�^�ۑ�
    d = d >> 4;             //���4�r�b�g������4�r�b�g�Ɉڂ�
    d = d & 0x0f;           //����4�r�b�g���o��
    d = d | RS_SIG;         //RS�M���ƍ���
    lcdo8(d);               //LCD�ɏo��
    wait();                 //���ԑ҂�
    dd = dd & 0x0f;         //����4�r�b�g��肾��
    dd = dd | RS_SIG;       //RS�M���ƍ���
    lcdo8(dd);              //LCD�ɏo��
    wait();
}

int main(void)
{
    P3DDR = 0xff;           //�|�[�g3��S�r�b�g�o�͂ɐݒ�
    wait();                 //�d��ON��15ms�ȏ�҂�
    lcdini();               //LCD���C�j�V�����C�Y����
    wait();
    lcdclr();               //LCD�N���A
    lcdxy(8,8);             //�\���ʒu�Z�b�g
    lcdo4('Y');             //�\�������Z�b�g�A�\���֐��R�[��
    wait();                 //�m��܂ł̑҂�����
    lcdxy(0x0c,1);          //2�����ڂ̕\��
    lcdo4('?');       
    while(1);         
}
