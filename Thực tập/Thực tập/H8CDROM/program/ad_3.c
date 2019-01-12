//A/D�R���o�[�^�e�X�g�v���O����
//AN0����̓��͓d����LCD�ɕ\��
//ad_3.c
#include <3048.h>
#define   E_SIG  0x20     
#define   RS_SIG 0x10     
 
void ioinit(void)
{
    P3.DDR = 0xff;                //�|�[�g3��S�r�b�g�o�͂ɐݒ�
}

void adinit(void)
{
    AD.ADCSR.BIT.ADF = 0;         //ADF�t���O�N���A
    AD.ADCSR.BIT.SCAN = 0;        //�P�ꃂ�[�h�I��
    AD.ADCSR.BIT.CKS = 1;         //�N���b�N�Z���N�g
    AD.ADCSR.BIT.CH = 0;          //�`���l���Z���N�g�@AN0�P�ꃂ�[�h
}
void wait(void)           
{
    int t = 1000;
    while(t--);
}

void lcdo8(unsigned char d)	    //8�r�b�g�o�͊֐�
{
    d = d | E_SIG;        
    P3.DR.BYTE = d;         
    d = d & 0xdf;           
    P3.DR.BYTE = d;         
    wait();                 
}

void lcdini(void)               //�������֐�
{
    lcdo8(3);      
    lcdo8(3);      
    lcdo8(3);      
    lcdo8(2);      
    lcdo8(2);      
    lcdo8(8);      
    lcdo8(0);      
    lcdo8(0x0c);   
    lcdo8(0);      
    lcdo8(6);      
}

void lcdclr(void)	              //�\���N���A�֐�
{
    P3.DR.BYTE = 0;
    lcdo8(0);
    lcdo8(1);                   //00000001�ŃN���A
    wait();
}

void lcdxy1(void)               //�J�[�\����1�s�ڂɈړ�
{
    P3.DR.BYTE = 0;
    lcdo8(8);                   //1�s��
    lcdo8(0);                   //�擪�ʒu
    wait();
}

void lcdxy2(void)               //�J�[�\����2�s�ڂɈړ�
{
    P3.DR.BYTE = 0;
    lcdo8(0x0c);                //2�s��
    lcdo8(0);                   //�擪�ʒu
    wait();
}

void lcdxy(int y,int x)         //�J�[�\����y�Ax�Ɉړ�
{
    P3.DR.BYTE = 0;
    lcdo8(y);                   //y�ʒu
    lcdo8(x);                   //x�ʒu
    wait();
}
void lcdo4(unsigned char d)     //1�����o�͊֐�
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
    for(c=0;c<16;c++){          //16�����\��
        lcdo4(*str);            //�\���֐��R�[��
        str++;                  //����������̕����ɂ���
    }
}

int main(void)
{
    int add;                    //�ϊ����ꂽ�M�����L������
    char s[4];                  //�����𕶎��f�[�^�ɂ��L������z��
    int i;
    ioinit();
    adinit();
    wait();                     //�d��ON��15ms�ȏ�҂�
    lcdini();                   //LCD���C�j�V�����C�Y����
    wait();
    lcdclr();                   //�\���N���A
    lcdxy1();                   //�\���ʒu
    dsp1g("AN0 Input VOLT  ");  //�^�C�g���\��������
    wait();
    lcdxy2();                   //2�s�ڎw��
    dsp1g("                ");  //2�s�ڃN���A
    while(1){
        AD.ADCSR.BIT.ADST = 1;        //A/D�ϊ��X�^�[�g
        while(AD.ADCSR.BIT.ADF == 0); //�ϊ��I����҂�
        add = AD.ADDRA >> 6;          //AN0���́A6�r�b�g�E�ɃV�t�g�A���
        AD.ADCSR.BIT.ADF = 0;         //�t���O�N���A
        add=(int)((add*(5.0/1024.0)*1000.0)+0.5);//���̓f�[�^��d���l�Ɋ��Z
        for(i=0;i<4;i++){             //�d���l�𕶎��f�[�^�ɕϊ����邽�߂̃f�[�^������
            s[i] = add % 10;          //10�Ŋ������]�������
            add /= 10;                //10�Ŋ���
        }  
        lcdxy(0x0c,5);                //2�s�A6�����ڂɃJ�[�\���𓮂���
        lcdo4(s[3]+'0');              //1���ڂ̕����@�{���Q��
        lcdo4('.');                   //�����_
        lcdo4(s[2]+'0');              //�����_��1�ʂ̕���
        lcdo4(s[1]+'0');              //������2�ʂ̕���
        lcdo4(s[0]+'0');              //������3�ʂ̕���
        lcdo4('V');                   //V�̕��� 
        wait();
    }
}        
