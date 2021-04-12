//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * IE3027: Electrónica Digital 2 - 2019
 */
//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
#define der2 PE_2
#define izq2 PE_3
#define izq1 PA_5
#define der1 PA_6
#define disp2 PF_1
#define disp1 PA_7
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7}; 
int J1 = 80;
int J2 = 240;
int D1 = J1 + 7;
int D2 = J2 + 7;
int Disp = 0;
int antirebote1 = 0;
int antirebote2 = 0;
int Y1 = 180;
int Y2 = 180;
int var = 0;
int CI = 15;
int mov = 1;
int vuelta = 0;
int LI = 20;
int M1 = 1;
int M2 = 1;
int M3 = 1;
int nivel = 10;
int juego = 0;
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
void reiniciar(void);

extern uint8_t fondo[];
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  reiniciar();
    pinMode(PUSH1, INPUT_PULLUP);
    pinMode(PUSH2, INPUT_PULLUP);
    pinMode(izq1, INPUT);
    pinMode(der1, INPUT);
    pinMode(izq2, INPUT);
    pinMode(der2, INPUT);
    pinMode(disp1, INPUT);
    pinMode(disp2, INPUT);
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  int reading1 = digitalRead(disp1);
  int reading2 = digitalRead(disp2);
   LCD_Bitmap(J1, 200, 13, 8, chunche);
   LCD_Bitmap(J2, 200, 13, 8, chunche);
   if (M1 == 1){
    LCD_Bitmap(CI, LI, 10, 8, malo1);
   }
   if (M2 == 1){
    LCD_Bitmap(CI+30, LI, 10, 8, malo1);
   }
   if (M3 == 1){
    LCD_Bitmap(CI+60, LI, 10, 8, malo1); 
   } 
   
  if (juego == 1){
    if (LI > 160){
      LCD_Clear(0x00);
      LCD_Print("ADIOS", 25, 5, 2, 0x3E1C, 0);
      M1 = 0;
      M2 = 0;
      M3 = 0;
      juego = 0;
   }
   if (M3 == 0 && M2 == 0 && M1==0){
    LCD_Print("NIVEL COMPLETADO", 25, 5, 2, 0x3E1C, 0);
    nivel++;
    var = 1;
    delay(1500);
    LI = 20;
    M1 = 1;
    M2 = 1;
    M3 = 1;
    vuelta = 0;
   } else if(var == 1){
    LCD_Print("                  ", 25, 5, 2, 0x3E1C, 0);
    var = 0;
   }
   if (mov == 1){
    CI++;
   } else if (mov == 0){
    CI--;
   }
   if (CI > 250){
    mov = 0;
   } else if (CI < 10){
    mov = 1;
    vuelta--;
   }
   if (vuelta > nivel){
    vuelta = 0;
    LCD_Print("                          ", CI, LI, 2, 0x3E1C, 0);
    LI = LI + 25;
   }
   int B_der1 = digitalRead(der1);
   if (B_der1 == HIGH){
    J1++;
    V_line(J1 -1, 200, 8, 0);  
   }
   int B_izq1 = digitalRead(izq1);
   if (B_izq1 == HIGH){
     J1--; 
     V_line(J1 + 14, 200, 8, 0);  
   }
   int B_der2 = digitalRead(der2);
   if (B_der2 == HIGH){
    J2++;
    V_line(J2 -1, 200, 8, 0);  
   }
   int B_izq2 = digitalRead(izq2);
   if (B_izq2 == HIGH){
    J2--; 
    V_line(J2 + 14, 200, 8, 0);  
   }
   J1 = (J1 > 146 ? 146 : J1);
   J1 = (J1 < 1 ? 1 : J1);
   J2 = (J2 > 306 ? 306 : J2);
   J2 = (J2 < 161 ? 161 : J2);

   if (Y1 < 186){
    V_line(D1, 185-Y1, 6, 0x3E1C);
    V_line(D1, 185+6-Y1, 6, 0x00); 
    Y1++;  
    if (185-Y1 >= (LI - 7) && (185-Y1)<= (LI+7)){
      if ((CI-1) <= D1 && (CI + 10) >= D1 && M1==1){
        Y1 = 190;       
        V_line(D1, LI-6, 20, 0x0);
        LCD_Bitmap(CI, LI, 10, 8, negro); 
        M1 = 0;
      } else if ((CI+30) <= D1 && (CI + 40) >= D1 && M2==1){
        Y1 = 190;       
        V_line(D1, LI-6, 20, 0x0); 
        LCD_Bitmap(CI+30, LI, 10, 8, negro);
        M2 = 0;
      } else if ((CI +60) <= D1 && (CI + 70) >= D1 && M3==1){
        Y1 = 190;       
        V_line(D1, LI-6, 20, 0x0);
        LCD_Bitmap(CI+60, LI, 10, 8, negro); 
        M3 = 0;
      }
   }
   } else {
    V_line(D1, 0, 15, 0x00); 
      if (reading1 == LOW){
      antirebote1 = 1;
      } else {
      if (antirebote1 == 1){
      antirebote1 = 0;
      D1 = J1 + 7;
      Y1 = 0;
    }
    }
   }

   if (Y2 < 186){
    V_line(D2, 185-Y2, 6, 0x3E1C);
    V_line(D2, 185+6-Y2, 6, 0x00); 
    Y2++;  
    if (185-Y2 >= (LI - 7) && (185-Y2)<= (LI+7)){
      if ((CI-1) <= D2 && (CI + 10) >= D2 && M1==1){
        Y2 = 190;       
        V_line(D2, LI-6, 20, 0x0);
        LCD_Bitmap(CI, LI, 10, 8, negro); 
        M1 = 0;
      } else if ((CI+30) <= D2 && (CI + 40) >= D2 && M2==1){
        Y2 = 190;       
        V_line(D2, LI-6, 20, 0x0); 
        LCD_Bitmap(CI+30, LI, 10, 8, negro);
        M2 = 0;
      } else if ((CI +60) <= D2 && (CI + 70) >= D2 && M3==1){
        Y2 = 190;       
        V_line(D2, LI-6, 20, 0x0);
        LCD_Bitmap(CI+60, LI, 10, 8, negro); 
        M3 = 0;
      }
   }
   } else {
    V_line(D2, 0, 15, 0x00); 
      if (reading2 == LOW){
      antirebote2 = 1;
      } else {
      if (antirebote2 == 1){
      antirebote2 = 0;
      D2 = J2 + 7;
      Y2 = 0;
    }
    }
   }
   delay(5);
  } else {
    reiniciar();
    }
  }
  
void reiniciar(void){
    LCD_Clear(0x00);
    for(int x = 0; x < 315-128; x++){
     LCD_Bitmap(x, 100, 127, 39, logo);
     V_line( x-1, 100, 30, 0x0000);
     delay(7);
    } 
    J1 = 80;
    J2 = 240;
    D1 = J1 + 7;
    D2 = J2 + 7;
    Disp = 0;
    antirebote1 = 0;
    antirebote2 = 0;
    Y1 = 180;
    Y2 = 180;
    var = 0;
    CI = 15;
    mov = 1;
    vuelta = 0;
    LI = 40;
    M1 = 1;
    M2 = 1;
    M3 = 1;
    nivel = 1;
    juego = 0;
    delay(1000);
    LCD_Clear(0x00);
     LCD_Bitmap(J1, 200, 13, 8, chunche);
     LCD_Bitmap(J2, 200, 13, 8, chunche);
     H_line(0,209,319,  0xD0A3);
     H_line(0,210,319,  0xD0A3);
     H_line(0,211,319,  0xD0A3);
     // dos jugadores divididos en 170
     V_line(160, 185, 120,  0xD0A3);
     String text = "jugador 1";
     LCD_Print(text, 50, 220, 1, 0x3E1C, 0);
     text = "jugador 2";
     LCD_Print(text, 210, 220, 1, 0x3E1C, 0);
     juego = 1;
  }
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}
