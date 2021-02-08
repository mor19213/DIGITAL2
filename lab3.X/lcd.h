
//LCD Functions Developed by electroSome

#ifndef lcd_H
#define lcd_H
#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif



void Lcd_Port(char a);
void Lcd_Cmd(char a);
//void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);

#endif /*lcd_H */
