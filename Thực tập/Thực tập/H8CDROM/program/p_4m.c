//LED�̓_���ړ�
//�|�[�gA�AB�A4�ɐڑ����ꂽLED���E���獶�ɓ_���ړ�����
//p_4m.c
#include  "io.h"        //3048�A3052�̓���I/O��`

void wait(void)
{
  long t=400000;
  while(t--);
}
int main(void)
{
  int i;                //�J��Ԃ����Z�f�[�^������ϐ�
  int d = 0x01;         //LED�o�͏����l
  PADDR = 0xff;         //PA��S�r�b�g�o�͂ɐݒ�
  PBDDR = 0xff;         //PA��S�r�b�g�o�͂ɐݒ�
  P4DDR = 0xff;         //PA��S�r�b�g�o�͂ɐݒ�
  while(1){
      for(i=0;i<8;i++){ //8��J��Ԃ�
         PADR = d;
         PBDR = d;
         P4DR = d;
         wait();
         d <<= 1;
         }
         d = 1;         //for���̃��[�v�𔲂�����Đݒ�
      }
}
