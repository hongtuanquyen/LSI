//D/A�R���o�[�^�e�X�g�v���O����
//�f�B�b�v�X�C�b�`P2����͂����̃f�[�^�̃A�i���O�M����DA0�ɏo�͂���
//da_1.c
#include <3048.h>

int main(void)
{
    P2.DDR = 0;                 //P2�S�r�b�g����
    P2.PCR.BYTE - 0xff;         //P2MOS�v���A�b�v
    DA.DACR.BIT.DAOE0 = 1;      //DA0�g�p
    DA.DACR.BIT.DAE = 0;        //�`���l��0�@D/A�ϊ�������
    while(1){
        DA.DADR0 = P2.DR.BYTE;  //P2�X�C�b�`���`���l��0�ɏo��  
    }
}
