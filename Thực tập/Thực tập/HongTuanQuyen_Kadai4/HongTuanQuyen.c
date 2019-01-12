#define P4DDR (*(volatile unsigned char*)0xfffc5)
#define P4DR (*(volatile unsigned char*)0xfffc7)
#define P4PCR (*(volatile unsigned char*)0xfffda)
//H8????????????
//LCD???????
//????????
//lcd_2.c 
#include "io.h"
#define  E_SIG  0x20            //E?????
#define  RS_SIG 0x10            //RS?????
 

void wait(int t)                 //??????
{
    while(t--);
}

void lcdo8(unsigned char d)     //8???????
{
    d=d | E_SIG;                //?????????E??????
    P3DR = d;                   //LCD???
    d = d & 0xdf;               //E???0???
    P3DR = d;                   //LCD???
    wait(1000);                     //???????????
}

void lcdini(void)               //?????
{
    lcdo8(3);                   //00000011???
    lcdo8(3);                   //3?????
    lcdo8(3);      
    lcdo8(2);                   //00000010???
    lcdo8(2);                   //????
    lcdo8(8);                   //00001000???
    lcdo8(0);      
    lcdo8(0x0c);                //00001100???
    lcdo8(0);      
    lcdo8(6);                   //00000110???
}

void lcdclr(void)               //???????
{
    P3DR = 0;
    lcdo8(0);
    lcdo8(1);                   //00000001????
    wait(1000);
}

void lcdxy1(void)               //?????1?????   // di chuy?n con tr? d?n déìg d?u tiåì
{
    P3DR = 0;
    lcdo8(8);                   //1??
    lcdo8(0);                   //????  v? trç?con tr?
    wait(1000);
}

void lcdxy2(void)               //?????2?????  // di chuy?n con tr? d?n déìg th? 2 
{
    P3DR = 0;
    lcdo8(0x0c);                //2??
    lcdo8(0);                   //????
    wait(1000);
}
void lcdxy(unsigned char y, unsigned char x)
{
	P3DR=0;
	lcdo8(y);
	lcdo8(x);
	wait(1000);
}


void lcdo4(unsigned char d)     //??????    // d?u ra 
{
    unsigned dd;
    dd = d;                     //???????
    d = d >> 4;                 //??4??????4??????
    d = d & 0x0f;               //??4???????
    d = d | RS_SIG;             //RS?????
    lcdo8(d);                   //LCD???
    wait(1000);                     //????
    dd = dd & 0x0f;             //??4???????
    dd = dd | RS_SIG;           //RS?????
    lcdo8(dd);                  //LCD???
    wait(1000);
}

void dsp1g(char *str)           //1????? *str??????    // hi?n th? d? li?u
{
    int c = 16;                 //16??
    for(c = 0;c < 16;c++){      //16????
    lcdo4(*str);                //???????
    str++;                      //???????????
    }
}


void lcd_print(char* string, int t)
{
	wait(t);                    //??ON?15ms????
    lcdxy(0x0c,0);                 //2????
    dsp1g(string);  //?????
}

int main(void)
{
  	P4DDR = 0;       //???4??
    P4PCR = 0xff;    //?????MOS ON
    P3DDR = 0xff;               //???3??????????
    wait(1000);                   //??ON?15ms????
    lcdini();                   //LCD??????????
    wait(1000);
    lcdclr();                   //?????
    lcdxy1();                   //????
    dsp1g("VER1 H.T.QUYEN  ");  //?????
    wait(1000);
    lcdxy2();                   //2????
    dsp1g("    1 1 1 1     ");  //?????
    while(1)
	{
	  	while(P4DR == 0x0f) {   lcd_print("0 0 0 0         ",1000);   }
	    while(P4DR == 0x1f)	{   lcd_print("0 0 0 1         ",1000);   }	
	  	while(P4DR == 0x2f)	{   lcd_print("0 0 1 0         ",1000);   }
	    while(P4DR == 0x3f) {   lcd_print("0 0 1 1         ",1000);   }
	  	while(P4DR == 0x4f)	{   lcd_print("0 1 0 0         ",1000);   }
    	while(P4DR == 0x5f)	{   lcd_print("0 1 0 1         ",1000);   }
	    while(P4DR == 0x6f) {   lcd_print("0 1 1 0         ",1000);   }
      while(P4DR == 0x7f)	{   lcd_print("0 1 1 1         ",1000);   }
    	while(P4DR == 0x8f)	{   lcd_print("1 0 0 0         ",1000);   }
	  	while(P4DR == 0x9f) {   lcd_print("1 0 0 1         ",1000);   }
	    while(P4DR == 0xaf) {   lcd_print("1 0 1 0         ",1000);   }
	    while(P4DR == 0xbf)	{   lcd_print("1 0 1 1         ",1000);   }
	    while(P4DR == 0xcf) {   lcd_print("1 1 0 0         ",1000);   }
	  	while(P4DR == 0xdf) {   lcd_print("1 1 0 1         ",1000);   }
	    while(P4DR == 0xef) {   lcd_print("1 1 1 0         ",1000);   }
      lcd_print("1 1 1 1         ",1000);
	}  
}


