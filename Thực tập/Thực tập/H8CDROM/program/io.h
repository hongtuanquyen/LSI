//Ｉ／Ｏポートヘッダファイル
// io.h 
#define P2DDR (*(volatile unsigned char*)0xfffc1)
#define P2DR (*(volatile unsigned char*)0xfffc3)
#define P2PCR (*(volatile unsigned char*)0xfffd8)
#define P3DDR (*(volatile unsigned char*)0xfffc4)
#define P3DR (*(volatile unsigned char*)0xfffc6)

#define P4DDR (*(volatile unsigned char*)0xfffc5)
#define P4DR (*(volatile unsigned char*)0xfffc7)
#define P5DDR (*(volatile unsigned char*)0xfffc8)
#define P5DR (*(volatile unsigned char*)0xfffca)
#define P8DDR (*(volatile unsigned char*)0xfffcd)
#define P8DR (*(volatile unsigned char*)0xfffcf)

#define PADDR (*(volatile unsigned char*)0xfffd1)
#define PADR (*(volatile unsigned char*)0xfffd3)
#define PBDDR (*(volatile unsigned char*)0xfffd4)
#define PBDR (*(volatile unsigned char*)0xfffd6)
#define P4PCR (*(volatile unsigned char*)0xfffda)
#define P5PCR (*(volatile unsigned char*)0xfffdb)
