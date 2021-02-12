/*
 * File:   lcd.c
 * Author: danie
 *
 * Created on February 4, 2021, 11:42 AM
 */


#include <xc.h>
#include "lcd.h"

void Lcd_Port(char a)
{
	PORTD = a;
}
void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(10);
        EN  = 0;             // => E = 0
}

//void Lcd_Clear()
//{
//	Lcd_Cmd(0x01);
//}

void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		Lcd_Cmd(temp);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		Lcd_Cmd(temp);
	}
}

void Lcd_Init(){
   __delay_ms(15);
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x030);
	__delay_ms(5);
  Lcd_Cmd(0x030);
	__delay_us(160);
  Lcd_Cmd(0x030);
  //////////////////////////////////////////////////////////////////////////////
  Lcd_Cmd(0x038);
  Lcd_Cmd(0x010);
  Lcd_Cmd(0x001);
  Lcd_Cmd(0x006);
  
  Lcd_Cmd(0x0C);
}

void Lcd_Write_Char(char a)
{
    RS = 1;             // => RS = 1
   Lcd_Port(a);              //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
}

void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x1C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x18);
}
