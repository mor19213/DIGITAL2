//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
   Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
   Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
   Con ayuda de: José Guerra
   IE3027: Electrónica Digital 2 - 2019
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
#define izq1 PE_5
#define der1 PA_6
#define disp2 PF_1
#define disp1 PA_7

#include <SPI.h>
#include <SD.h>

File modo1;
File modo2;

//         int num = 321;
//       char snum[5];
//       // convert 123 to string [buf]
//        itoa(num, snum, 10);
//       LCD_Print(snum, 165, 220, 1, 0x3E1C, 0);
//       Serial.println(snum);
char snum[5];
int modo = 0;
int highscore1[] = {48, 48, 48, 48, 48};
int highscore2[] = {48, 48, 48, 48, 48};
int highscore3[] = {48, 48, 48, 48, 48};
int highscore12[] = {48, 48, 48, 48, 48};
int highscore22[] = {48, 48, 48, 48, 48};
int highscore32[] = {48, 48, 48, 48, 48};
int high1;
int high2;
int high3;
int high12;
int high22;
int high32;
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
int MY1 = 300;
int MY2 = 300;
int MX1 = 400;
int MX2 = 400;
int MD1 = 0;
int MD2 = 0;
int vida1 = 3;
int vida2 = 3;
int tanque1 = 1;
int tanque2 = 1;
int var = 0;
int var1 = 246;
int CI = 15;
int mov = 1;
int vuelta = 0;
int LI = 20;
int M1 = 1;
int M2 = 1;
int M3 = 1;
int M4 = 1;
int M5 = 1;
int B_der1;
int B_der2;
int B_izq1;
int B_izq2;
int puntaje1;
int puntaje2;
int puntaje = 0;
int anti1 = 0;
int anti2 = 0;
int var_flecha = 114;
int nivel = 12;
int juego = 0;
int reading1;
int reading2;
int menu;
int malos;
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
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);
void reiniciar(void);
void menuu(void);
void tanque_1(void);
void tanque_2(void);
void highscores(int, int);

extern uint8_t fondo[];
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  SPI.setModule(0);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);

  if (!SD.begin(32)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  //*********************************** highscores para modo de 2 jugadores
  // re-open the file for reading:
  modo2 = SD.open("2jugador.txt");
  if (modo2) {
    //    Serial.println("2jugador.txt:");

    while (highscore1[var - 1] != 10) {
      highscore1[var] = modo2.read();
      if ((highscore1[var] < 48) && (highscore1[var] != 10)) {
        highscore1[var] = 48;
      }
      var++;
    }
    var = 0;
    while (highscore2[var - 1] != 10) {
      highscore2[var] = modo2.read();
      if ((highscore2[var] < 48) && (highscore2[var] != 10)) {
        highscore2[var] = 48;
      }
      var++;
    }
    var = 0;
    while (highscore3[var - 1] != 10) {
      highscore3[var] = modo2.read();
      if ((highscore3[var] < 48) && (highscore3[var] != 10)) {
        highscore3[var] = 48;
      }
      var++;
    }
    var = 0;
    Serial.println("leer modo 1");
    // close the file:
    modo2.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening 2jugador.txt");
  }
  high1 = (((int)highscore1[0] - 48)*100)+(((int)highscore1[1] - 48)*10)+((int)highscore1[2] - 48);
  high2 = (((int)highscore2[0] - 48)*100)+(((int)highscore2[1] - 48)*10)+((int)highscore2[2] - 48);
  high3 = (((int)highscore3[0] - 48)*100)+(((int)highscore3[1] - 48)*10)+((int)highscore3[2] - 48);
  Serial.println(high1);
  Serial.println(high2);
  Serial.println(high3);
  
  //*********************************** highscores para modo de 1 jugador
  // re-open the file for reading:
  modo2 = SD.open("1jugador.txt");
  if (modo2) {
    //    Serial.println("1jugador.txt:");

    while (highscore12[var - 1] != 10) {
      highscore12[var] = modo2.read();
      if ((highscore12[var] < 48) && (highscore12[var] != 10)) {
        highscore12[var] = 48;
      }
      var++;
    }
    var = 0;
    while (highscore22[var - 1] != 10) {
      highscore22[var] = modo2.read();
      if ((highscore22[var] < 48) && (highscore22[var] != 10)) {
        highscore22[var] = 48;
      }
      var++;
    }
    var = 0;
    while (highscore32[var - 1] != 10) {
      highscore32[var] = modo2.read();
      if ((highscore32[var] < 48) && (highscore32[var] != 10)) {
        highscore32[var] = 48;
      }
      var++;
    }
    var = 0;
    Serial.println("leer modo 2");
    // close the file:
    modo2.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening 1jugador.txt");
  }
  high12 = (((int)highscore12[0] - 48)*100)+(((int)highscore12[1] - 48)*10)+((int)highscore12[2] - 48);
  high22 = (((int)highscore22[0] - 48)*100)+(((int)highscore22[1] - 48)*10)+((int)highscore22[2] - 48);
  high32 = (((int)highscore32[0] - 48)*100)+(((int)highscore32[1] - 48)*10)+((int)highscore32[2] - 48);
  Serial.println(high12);
  Serial.println(high22);
  Serial.println(high32);
  

  LCD_Init();
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  pinMode(izq1, INPUT);
  pinMode(der1, INPUT);
  pinMode(izq2, INPUT);
  pinMode(der2, INPUT);
  pinMode(disp1, INPUT);
  pinMode(disp2, INPUT);
  reiniciar();
  
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  while (juego == 1) { //2 jugadores
    var++;
    if (var == 11) {
      var = 1;
    }

    itoa(puntaje1, snum, 10);
    LCD_Print(snum, 5, 5, 1, 0x3E1C, 0);
    itoa(puntaje2, snum, 10);
    LCD_Print(snum, 300, 5, 1, 0x3E1C, 0);
    reading1 = digitalRead(disp1);
    reading2 = digitalRead(disp2);

    if (MD1 == 0) {
      var1++;
      V_line(MX1, MY1, 12, 0);
      if (var1 > 6) {
        if (M1 == 1 && var == 2) {
          MD1 = 1;
          MX1 = CI;
        } else if (M2 == 1 && var == 4) {
          MD1 = 1;
          MX1 = CI + 20;
        } else if (M3 == 1 && var == 6) {
          MD1 = 1;
          MX1 = CI + 40;
        } else if (M4 == 1 && var == 8) {
          MD1 = 1;
          MX1 = CI + 60;
          MY1 = LI + 8;
        } else if (M5 == 1 && var == 10) {
          MD1 = 1;
          MX1 = CI + 80;
        }
        MY1 = LI + 8;
        var1 = 0;
      }
    }
    if (MD1 == 1) {
      V_line(MX1, MY1, 6, 0xD0A3);
      V_line(MX1, MY1 - 6, 6, 0x00);
      MY1++;
      if (MY1 > 194) {
        MD1 = 0;
        if (tanque1 != 2) {
          if (J1 < MX1 && MX1 < (J1 + 13)) {
            vida1--;
            //            LCD_Print("wuuuuu", 25, 100, 2, 0x3E1C, 0);
          }
        }
        if (J2 < MX1 && MX1 < (J2 + 13)) {
          vida2--;
          //LCD_Print("wuuuuu", 25, 100, 2, 0x3E1C, 0);
        }
        if (vida1 == 0) {
          tanque1 = 0;
          //          LCD_Print("sin vidas jugador 2", 25, 100, 2, 0x3E1C, 0);
          LCD_Print("jugador1 = 0 vidas", 10, 220, 1, 0x3E1C, 0);
          for (int x = J1; x < (J1 + 14); x++) {
            V_line( x, 200, 8, 0x0000);
          }
        } else if (vida1 == 2) {
          LCD_Print("jugador1 = 2 vidas", 10, 220, 1, 0x3E1C, 0);
        } else if (vida1 == 1) {
          LCD_Print("jugador1 = 1 vidas", 10, 220, 1, 0x3E1C, 0);
        }
        if (vida2 == 0) {
          tanque2 = 0;
          for (int x = J2; x < (J2 + 15); x++) {
            V_line( x, 200, 8, 0x0000);
          }
          LCD_Print("jugador2 = 0 vidas ", 165, 220, 1, 0x3E1C, 0);
        } else if (vida2 == 1) {
          LCD_Print("jugador2 = 1 vidas ", 165, 220, 1, 0x3E1C, 0);
        } else if (vida2 == 2) {
          LCD_Print("jugador2 = 2 vidas ", 165, 220, 1, 0x3E1C, 0);
        }
      }
    }
    V_line(160, 195, 209,  0xD0A3);
    // mostrar tanquecitos y malos
    if (tanque1 == 1) {
      tanque_1();
    }
    if (tanque2 == 1) {
      tanque_2();
    }
    if (M1 == 1) {
      LCD_Bitmap(CI, LI, 10, 8, malo1);
    }
    if (M2 == 1) {
      LCD_Bitmap(CI + 20, LI, 10, 8, malo1);
    }
    if (M3 == 1) {
      LCD_Bitmap(CI + 40, LI, 10, 8, malo1);
    }
    if (M4 == 1) {
      LCD_Bitmap(CI + 60, LI, 10, 8, malo1);
    }
    if (M5 == 1) {
      LCD_Bitmap(CI + 80, LI, 10, 8, malo1);
    }

    // si se matan a todos los malos
    malos = M1 + M2 + M3 + M4 + M5;
    if (malos == 0) {
      LCD_Print("NIVEL COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      nivel--;
      delay(1500);
      LI = 20;
      M1 = 1;
      M2 = 1;
      M3 = 1;
      M4 = 1;
      M5 = 1;
      MY1 = 300;
      MY2 = 300;
      MX1 = 400;
      MX2 = 400;
      MD1 = 0;
      MD2 = 0;
      for (int x = 0; x < 320; x++) {
        V_line(x, 0, 195, 0);
      }
      Y1 = 300;
      Y2 = 300;
      vuelta = 0;
    }
    if (mov == 1) {
      CI++;
    } else if (mov == 0) {
      CI--;
    }
    if (CI > 220) {
      mov = 0;
    } else if (CI < 10) {
      mov = 1;
      vuelta++;
    }
    if (vuelta > nivel) {
      vuelta = 0;
      LCD_Print("                          ", CI, LI, 2, 0x3E1C, 0);
      LI = LI + 25;
    }

    delay(5);
    if (tanque1 == 0 && tanque2 == 0) {
      LI = 400;
    }
    // jugador pierde
    if (LI > 160) {
      LCD_Clear(0x00);
      LCD_Print("ADIOS", 25, 5, 2, 0x3E1C, 0);
      juego = 0;
      delay(100);
    }
    // jugador gana
    if (nivel == 1) {
      LCD_Clear(0x00);
      LCD_Print("JUEGO COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      juego = 0;
      delay(100);
    }
  }

  while (juego == 2) { // 1 jugador
    var++;
    if (var == 11) {
      var = 1;
    }

    itoa(puntaje1, snum, 10);
    LCD_Print(snum, 5, 5, 1, 0x3E1C, 0);
    // si se matan a todos los malos
    malos = M1 + M2 + M3 + M4 + M5;
    if (malos == 0) {
      LCD_Print("NIVEL COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      nivel--;
      delay(1500);
      LI = 20;
      M1 = 1;
      M2 = 1;
      M3 = 1;
      M4 = 1;
      M5 = 1;
      MY1 = 300;
      MY2 = 300;
      MX1 = 400;
      MX2 = 400;
      MD1 = 0;
      MD2 = 0;
      for (int x = 0; x < 320; x++) {
        V_line(x, 0, 195, 0);
      }
      Y1 = 300;
      Y2 = 300;
      vuelta = 0;
    }
    reading1 = digitalRead(disp1);
    reading2 = digitalRead(disp2);

    if (MD1 == 0) {
      var1++;
      V_line(MX1, MY1, 12, 0);
      if (var1 > 6) {
        if (M1 == 1 && var == 2) {
          MD1 = 1;
          MX1 = CI;
        } else if (M2 == 1 && var == 4) {
          MD1 = 1;
          MX1 = CI + 20;
        } else if (M3 == 1 && var == 6) {
          MD1 = 1;
          MX1 = CI + 40;
        } else if (M4 == 1 && var == 8) {
          MD1 = 1;
          MX1 = CI + 60;
          MY1 = LI + 8;
        } else if (M5 == 1 && var == 10) {
          MD1 = 1;
          MX1 = CI + 80;
        }
        MY1 = LI + 8;
        var1 = 0;
      }
    }
    if (MD1 == 1) {
      V_line(MX1, MY1, 6, 0xD0A3);
      V_line(MX1, MY1 - 6, 6, 0x00);
      MY1++;
      if (MY1 > 194) {
        MD1 = 0;
        if (tanque1 != 2) {
          if (J1 < MX1 && MX1 < (J1 + 13)) {
            vida1--;
            //            LCD_Print("wuuuuu", 25, 100, 2, 0x3E1C, 0);
          }
        }
        if (vida1 == 0) {
          tanque1 = 0;
          //          LCD_Print("sin vidas jugador 2", 25, 100, 2, 0x3E1C, 0);
          LCD_Print("jugador = 0 vidas", 10, 220, 1, 0x3E1C, 0);
          for (int x = J1; x < (J1 + 14); x++) {
            V_line( x, 200, 8, 0x0000);
          }
          LCD_Clear(0x00);
          LCD_Print("ADIOS", 25, 5, 2, 0x3E1C, 0);
          juego = 0;
          M1 = 0;
          M2 = 0;
          M3 = 0;
          M4 = 0;
          M5 = 0;
          tanque1 = 0;
          tanque2 = 0;
          delay(100);
        } else if (vida1 == 2) {
          LCD_Print("jugador = 2 vidas", 10, 220, 1, 0x3E1C, 0);
        } else if (vida1 == 1) {
          LCD_Print("jugador = 1 vidas", 10, 220, 1, 0x3E1C, 0);
        }
      }
    }
    // mostrar tanquecitos y malos
    if (tanque1 == 1) {
      tanque_1();
    }
    if (M1 == 1) {
      LCD_Bitmap(CI, LI, 10, 8, malo1);
    }
    if (M2 == 1) {
      LCD_Bitmap(CI + 20, LI, 10, 8, malo1);
    }
    if (M3 == 1) {
      LCD_Bitmap(CI + 40, LI, 10, 8, malo1);
    }
    if (M4 == 1) {
      LCD_Bitmap(CI + 60, LI, 10, 8, malo1);
    }
    if (M5 == 1) {
      LCD_Bitmap(CI + 80, LI, 10, 8, malo1);
    }


    if (mov == 1) {
      CI++;
    } else if (mov == 0) {
      CI--;
    }
    if (CI > 220) {
      mov = 0;
    } else if (CI < 10) {
      mov = 1;
      vuelta++;
    }
    if (vuelta > nivel) {
      vuelta = 0;
      LCD_Print("                          ", CI, LI, 2, 0x3E1C, 0);
      LI = LI + 25;
    }

    delay(5);

    // jugador pierde
    if (LI > 160) {
      LCD_Clear(0x00);
      LCD_Print("ADIOS", 25, 5, 2, 0x3E1C, 0);
      juego = 0;
      delay(100);
    }
    // jugador gana
    if (nivel == 1) {
      LCD_Clear(0x00);
      LCD_Print("JUEGO COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      juego = 0;
      delay(100);
    }
  }

  while (juego == 3) {
    var++;
    if (var == 11) {
      var = 1;
    }

    itoa(puntaje1, snum, 10);
    LCD_Print(snum, 5, 5, 1, 0x3E1C, 0);
    itoa(puntaje2, snum, 10);
    LCD_Print(snum, 300, 5, 1, 0x3E1C, 0);
    // si se matan a todos los malos
    malos = M1 + M2 + M3 + M4 + M5;
    if (malos == 0) {
      LCD_Print("NIVEL COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      nivel--;
      delay(1500);
      LI = 20;
      M1 = 1;
      M2 = 1;
      M3 = 1;
      M4 = 1;
      M5 = 1;
      MY1 = 300;
      MY2 = 300;
      MX1 = 400;
      MX2 = 400;
      MD1 = 0;
      MD2 = 0;
      for (int x = 0; x < 320; x++) {
        V_line(x, 0, 195, 0);
      }
      Y1 = 300;
      Y2 = 300;
      vuelta = 0;
    }
    reading1 = digitalRead(disp1);
    reading2 = digitalRead(disp2);

    if (MD1 == 0) {
      var1++;
      V_line(MX1, MY1, 12, 0);
      if (var1 > 6) {
        if (M1 == 1 && var == 2) {
          MD1 = 1;
          MX1 = CI;
        } else if (M2 == 1 && var == 4) {
          MD1 = 1;
          MX1 = CI + 20;
        } else if (M3 == 1 && var == 6) {
          MD1 = 1;
          MX1 = CI + 40;
        } else if (M4 == 1 && var == 8) {
          MD1 = 1;
          MX1 = CI + 60;
          MY1 = LI + 8;
        } else if (M5 == 1 && var == 10) {
          MD1 = 1;
          MX1 = CI + 80;
        }
        MY1 = LI + 8;
        var1 = 0;
      }
    }
    if (MD1 == 1) {
      V_line(MX1, MY1, 6, 0xD0A3);
      V_line(MX1, MY1 - 6, 6, 0x00);
      MY1++;
      if (MY1 > 194) {
        MD1 = 0;
        if (tanque1 != 2) {
          if (J1 < MX1 && MX1 < (J1 + 13)) {
            vida1--;
            //            LCD_Print("wuuuuu", 25, 100, 2, 0x3E1C, 0);
          }
        }
        if (J2 < MX1 && MX1 < (J2 + 13)) {
          vida2--;
          //LCD_Print("wuuuuu", 25, 100, 2, 0x3E1C, 0);
        }
        if (vida1 == 0) {
          LCD_Clear(0x00);
          LCD_Print("ADIOS", 25, 5, 2, 0x3E1C, 0);
          juego = 0;
          puntaje1 = 0;
          M1 = 0;
          M2 = 0;
          M3 = 0;
          M4 = 0;
          M5 = 0;
          tanque1 = 0;
          tanque2 = 0;
          delay(100);
        } else if (vida1 == 2) {
          LCD_Print("jugador1 = 2 vidas", 10, 220, 1, 0x3E1C, 0);
        } else if (vida1 == 1) {
          LCD_Print("jugador1 = 1 vidas", 10, 220, 1, 0x3E1C, 0);
        }
        if (vida2 == 0) {
          LCD_Clear(0x00);
          LCD_Print("ADIOS", 25, 5, 2, 0x3E1C, 0);
          juego = 0;
          puntaje2 = 0;
          M1 = 0;
          M2 = 0;
          M3 = 0;
          M4 = 0;
          M5 = 0;
          tanque1 = 0;
          tanque2 = 0;
          delay(100);
        } else if (vida2 == 1) {
          LCD_Print("jugador2 = 1 vidas ", 165, 220, 1, 0x3E1C, 0);
        } else if (vida2 == 2) {
          LCD_Print("jugador2 = 2 vidas ", 165, 220, 1, 0x3E1C, 0);
        }
      }
    }
    V_line(160, 195, 209,  0xD0A3);
    // mostrar tanquecitos y malos
    if (tanque1 == 1) {
      tanque_1();
    }
    if (tanque2 == 1) {
      tanque_2();
    }
    if (M1 == 1) {
      LCD_Bitmap(CI, LI, 10, 8, malo1);
    }
    if (M2 == 1) {
      LCD_Bitmap(CI + 20, LI, 10, 8, malo1);
    }
    if (M3 == 1) {
      LCD_Bitmap(CI + 40, LI, 10, 8, malo1);
    }
    if (M4 == 1) {
      LCD_Bitmap(CI + 60, LI, 10, 8, malo1);
    }
    if (M5 == 1) {
      LCD_Bitmap(CI + 80, LI, 10, 8, malo1);
    }


    if (mov == 1) {
      CI++;
    } else if (mov == 0) {
      CI--;
    }
    if (CI > 220) {
      mov = 0;
    } else if (CI < 10) {
      mov = 1;
      vuelta++;
    }
    if (vuelta > nivel) {
      vuelta = 0;
      LCD_Print("                          ", CI, LI, 2, 0x3E1C, 0);
      LI = LI + 25;
    }

    delay(5);
    if (tanque1 == 0 && tanque2 == 0) {
      LI = 400;
    }
    // jugador pierde
    if (LI > 160) {
      LCD_Clear(0x00);
      LCD_Print("GAME OVER", 25, 15, 2, 0x3E1C, 0);
      juego = 0;
      delay(100);
    }
    // jugador gana
    if (nivel == 1) {
      LCD_Clear(0x00);
      LCD_Print("JUEGO COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      juego = 0;
      delay(100);
    }
  }

  LCD_Print("Presionar para ", 5, 190, 1, 0x3E1C, 0);
  LCD_Print(" ir al menu", 5, 200, 1, 0x3E1C, 0);
  if (juego == 4){
    // mostrar los highscores
    // leer los archivos de la SD
    LCD_Print("Highscores", 90, 5, 2, 0x3E1C, 0);
    LCD_Print("2 players", 5, 50, 2, 0x3E1C, 0);
    LCD_Print("|", 155, 50, 2, 0x3E1C, 0);
    LCD_Print("1 player", 170, 50, 2, 0x3E1C, 0);
    
    itoa(high1, snum, 10);
    LCD_Print("1-", 5, 70, 2, 0x3E1C, 0);
    LCD_Print(snum, 50, 70, 2, 0x3E1C, 0);
    LCD_Print("|", 155, 70, 2, 0x3E1C, 0);
    itoa(high12, snum, 10);
    LCD_Print("1-", 170, 70, 2, 0x3E1C, 0);
    LCD_Print(snum, 215, 70, 2, 0x3E1C, 0);
    
    itoa(high2, snum, 10);
    LCD_Print("2-", 5, 90, 2, 0x3E1C, 0);
    LCD_Print(snum, 50, 90, 2, 0x3E1C, 0);
    LCD_Print("|", 155, 90, 2, 0x3E1C, 0);
    itoa(high22, snum, 10);
    LCD_Print("2-", 170, 90, 2, 0x3E1C, 0);
    LCD_Print(snum, 215, 90, 2, 0x3E1C, 0);
    
    itoa(high3, snum, 10);
    LCD_Print("3- ", 5, 110, 2, 0x3E1C, 0);
    LCD_Print(snum, 50, 110, 2, 0x3E1C, 0);
    LCD_Print("|", 155, 110, 2, 0x3E1C, 0);
    itoa(high32, snum, 10);
    LCD_Print("3- ", 170, 110, 2, 0x3E1C, 0);
    LCD_Print(snum, 215, 110, 2, 0x3E1C, 0);
    juego = 0;
  }
  if (modo == 3) {
    puntaje1 = puntaje1 * (vida1 + 1);
    puntaje2 = puntaje2 * (vida2 + 1);
    itoa(puntaje1, snum, 10);
    LCD_Print(snum, 105, 70, 2, 0x3E1C, 0);
    LCD_Print("vs", 125, 70, 2, 0x3E1C, 0);
    itoa(puntaje2, snum, 10);
    LCD_Print(snum, 160, 70, 2, 0x3E1C, 0);
    if (puntaje1 > puntaje2) {
      LCD_Print("Ganador:", 105, 120, 2, 0x3E1C, 0);
      LCD_Print("jugador 1", 105, 140, 2, 0x3E1C, 0);
    } else if (puntaje1 < puntaje2) {
      LCD_Print("Ganador:", 105, 120, 2, 0x3E1C, 0);
      LCD_Print("jugador 2", 105, 140, 2, 0x3E1C, 0);
    } else if (puntaje1 == puntaje2) {
      LCD_Print("Empate", 105, 140, 2, 0x3E1C, 0);
    }
  } else if (modo == 2) {
    // 1 jugador
    itoa(puntaje1, snum, 10);
    LCD_Print("Puntaje Final:", 85, 70, 2, 0x3E1C, 0);
    LCD_Print(snum, 85, 100, 2, 0x3E1C, 0);
      highscores(puntaje1, 2);
  } else if (modo == 1) {
    // 2 jugadores juntos
    //    puntaje1 = puntaje1 - (5*vida1);
    //    puntaje2 = puntaje2 - (5*vida2);
    itoa(puntaje1 + puntaje2, snum, 10);
    LCD_Print("puntaje final:", 85, 100, 2, 0x3E1C, 0);
    LCD_Print(snum, 105, 70, 2, 0x3E1C, 0);
      highscores(puntaje1 + puntaje2, 1);
  }
  
  while (juego == 0) {
    modo = 0;
    reading1 = digitalRead(disp1);
    reading2 = digitalRead(disp2);
    if (reading1 == HIGH && reading2 == HIGH) {
      reiniciar();
    }
  }
}
// 1  - modo 1
// 2 - modo 2
void highscores(int puntaje, int num) {
  if (num == 1) {
    
  Serial.println("......");
    if (puntaje > high1) {
      high3 = high2;
      high2 = high1;
      high1 = puntaje;
    LCD_Print("new 1st highscore", 150, 170, 1, 0x3E1C, 0);
    Serial.println("primer lugar");
    } else if (puntaje > high2) {
      high3 = high2;
      high2 = puntaje;
    LCD_Print("new 2nd highscore", 150, 170, 1, 0x3E1C, 0);
    Serial.println("2 lugar");
    } else if (puntaje > high3) {
      high3 = puntaje;
    LCD_Print("new 3rd highscore", 150, 170, 1, 0x3E1C, 0);
    Serial.println("3 lugar");
    } else {
      high3 = high3;
      high2 = high2;
      high1 = high1;
    }
    SD.remove("2jugador.txt");
    modo2 = SD.open("2jugador.txt", FILE_WRITE);
    // if the file opened okay, write to it:
    if (modo2) {
      Serial.println("Writing to 2jugador.txt...");
      Serial.println(high1);
      Serial.println(high2);
      Serial.println(high3);
      itoa(high1, snum, 10);
      if (high1 > 99){
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high1 > 9){
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");
      

      itoa(high2, snum, 10);
      if (high2 > 99){
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high2 > 9){
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");

      
      itoa(high3, snum, 10);
      if (high3 > 99){
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high3 > 9){
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");
      
      // close the file:
      modo2.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening 2jugador.txt");
    }
  }
  if (num == 2) {
    
  Serial.println("......");
    if (puntaje > high12) {
      high32 = high22;
      high22 = high12;
      high12 = puntaje;
    LCD_Print("new 1st highscore", 150, 170, 1, 0x3E1C, 0);
    Serial.println("primer lugar");
    } else if (puntaje > high22) {
      high32 = high22;
      high22 = puntaje;
    LCD_Print("new 2nd highscore", 150, 170, 1, 0x3E1C, 0);
    Serial.println("2 lugar");
    } else if (puntaje > high32) {
      high32 = puntaje;
    LCD_Print("new 3rd highscore", 150, 170, 1, 0x3E1C, 0);
    Serial.println("3 lugar");
    } else {
      high32 = high32;
      high22 = high22;
      high12 = high12;
    }
    SD.remove("1jugador.txt");
    modo2 = SD.open("1jugador.txt", FILE_WRITE);
    // if the file opened okay, write to it:
    if (modo2) {
      Serial.println("Writing to 2jugador.txt...");
      Serial.println(high12);
      Serial.println(high22);
      Serial.println(high32);
      itoa(high12, snum, 10);
      if (high12 > 99){
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high12 > 9){
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");
      

      itoa(high22, snum, 10);
      if (high22 > 99){
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high22 > 9){
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");

      
      itoa(high32, snum, 10);
      if (high32 > 99){
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high32 > 9){
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");
      
      // close the file:
      modo2.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening 1jugador.txt");
    }
  }
}
void tanque_2(void) {
  J2 = (J2 > 306 ? 306 : J2);
  J2 = (J2 < 161 ? 161 : J2);

  LCD_Bitmap(J2, 200, 13, 8, chunche);
  int B_der2 = digitalRead(der2);
  if (B_der2 == HIGH) {
    J2++;
    V_line(J2 - 1, 200, 8, 0);
  }
  int B_izq2 = digitalRead(izq2);
  if (B_izq2 == HIGH) {
    J2--;
    V_line(J2 + 14, 200, 8, 0);
  }
  if (Y2 < 186) {
    V_line(D2, 185 - Y2, 6, 0x3E1C);
    V_line(D2, 185 + 6 - Y2, 6, 0x00);
    Y2++;
    if (185 - Y2 >= (LI - 7) && (185 - Y2) <= (LI + 7)) {
      if ((CI - 1) <= D2 && (CI + 10) >= D2 && M1 == 1) {
        Y2 = 190;
        V_line(D2, LI - 6, 20, 0x0);
        LCD_Bitmap(CI, LI, 10, 8, negro);
        puntaje2++;
        M1 = 0;
      } else if ((CI + 20) <= D2 && (CI + 30) >= D2 && M2 == 1) {
        Y2 = 190;
        V_line(D2, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 20, LI, 10, 8, negro);
        puntaje2++;
        M2 = 0;
      } else if ((CI + 40) <= D2 && (CI + 50) >= D2 && M3 == 1) {
        Y2 = 190;
        V_line(D2, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 40, LI, 10, 8, negro);
        puntaje2++;
        M3 = 0;
      } else if ((CI + 60) <= D2 && (CI + 70) >= D2 && M4 == 1) {
        Y2 = 190;
        V_line(D2, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 60, LI, 10, 8, negro);
        puntaje2++;
        M4 = 0;
      } else if ((CI + 80) <= D2 && (CI + 90) >= D2 && M5 == 1) {
        Y2 = 190;
        V_line(D2, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 80, LI, 10, 8, negro);
        puntaje2++;
        M5 = 0;
      }
    }
  } else {
    V_line(D2, 0, 15, 0x00);
    if (reading2 == LOW) {
      antirebote2 = 1;
    } else {
      if (antirebote2 == 1) {
        antirebote2 = 0;
        D2 = J2 + 7;
        Y2 = 0;
      }
    }
  }
}
void tanque_1(void) {
  if (modo != 2) {
    J1 = (J1 > 146 ? 146 : J1);
  } else {
    J1 = (J1 > 306 ? 306 : J1);
  }
  J1 = (J1 < 1 ? 1 : J1);
  LCD_Bitmap(J1, 200, 13, 8, chunche);
  int B_der1 = digitalRead(der1);
  if (B_der1 == HIGH) {
    J1++;
    V_line(J1 - 1, 200, 8, 0);
  }
  int B_izq1 = digitalRead(izq1);
  if (B_izq1 == HIGH) {
    J1--;
    V_line(J1 + 14, 200, 8, 0);
  }
  if (Y1 < 186) {
    V_line(D1, 185 - Y1, 6, 0x3E1C);
    V_line(D1, 185 + 6 - Y1, 6, 0x00);
    Y1++;
    if (185 - Y1 >= (LI - 7) && (185 - Y1) <= (LI + 7)) {
      if ((CI - 1) <= D1 && (CI + 10) >= D1 && M1 == 1) {
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI, LI, 10, 8, negro);
        puntaje1++;
        M1 = 0;
      } else if ((CI + 20) <= D1 && (CI + 30) >= D1 && M2 == 1) {
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 20, LI, 10, 8, negro);
        puntaje1++;
        M2 = 0;
      } else if ((CI + 40) <= D1 && (CI + 50) >= D1 && M3 == 1) {
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 40, LI, 10, 8, negro);
        puntaje1++;
        M3 = 0;
      } else if ((CI + 60) <= D1 && (CI + 70) >= D1 && M4 == 1) {
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 60, LI, 10, 8, negro);
        puntaje1++;
        M4 = 0;
      } else if ((CI + 80) <= D1 && (CI + 90) >= D1 && M5 == 1) {
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 80, LI, 10, 8, negro);
        puntaje1++;
        M5 = 0;
      }
    }
  } else {
    V_line(D1, 0, 15, 0x00);
    if (reading1 == LOW) {
      antirebote1 = 1;
    } else {
      if (antirebote1 == 1) {
        antirebote1 = 0;
        D1 = J1 + 7;
        Y1 = 0;
      }
    }
  }
}
void menuu(void) {
  //juego = 1;
  var_flecha = 114;
  LCD_Bitmap(100, 50, 127, 39, logo);
  LCD_Print("2 jugadores", 40, 120, 2, 0xC992, 0);
  LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
  LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
  LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
  while (menu == 1) {
    B_der1 = digitalRead(der1);
    B_der2 = digitalRead(der2);
    B_izq1 = digitalRead(izq1);
    B_izq2 = digitalRead(izq2);
    reading1 = digitalRead(disp1);
    reading2 = digitalRead(disp2);
    if (B_der1 == HIGH || B_der2 == HIGH) {
      anti1 = 1;
    }
    if ((B_der1 == LOW && B_der2 == LOW) && anti1 == 1) {
      anti1 = 0;
      if (var_flecha == 114) {
        var_flecha = 144;
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xC992, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      } else  if (var_flecha == 144) {
        var_flecha = 174;
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xC992, 0);
  LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      } else if (var_flecha == 174){
        var_flecha = 204;
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xC992, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      }
    }
    if (B_izq1 == HIGH || B_izq2 == HIGH) {
      anti2 = 1;
    }
    if ((B_izq1 == LOW && B_izq2 == LOW) && anti2 == 1) {
      anti2 = 0;
      if (var_flecha == 174) {
        var_flecha = 144;
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xC992, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      } else if (var_flecha == 144) {
        var_flecha = 114;
        LCD_Print("2 jugadores", 40, 120, 2, 0xC992, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      } else if (var_flecha == 204) {
        var_flecha = 174;
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xC992, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
        H_line(5, 230, 100, 0);
        H_line(5, 229, 100, 0);
        H_line(5, 228, 100, 0);
        H_line(5, 227, 100, 0);
      } 
    }
    LCD_Bitmap(20, var_flecha, 13, 26, flecha);
    if ((reading1 == HIGH || reading2 == HIGH) && var_flecha == 114) {
      juego = 1;
      modo = 1;
      menu = 0;
    }
    if ((reading1 == HIGH || reading2 == HIGH) && var_flecha == 144) {
      juego = 2;
      modo = 2;
      menu = 0;
    }
    if ((reading1 == HIGH || reading2 == HIGH) && var_flecha == 174) {
      juego = 3;
      modo = 3;
      menu = 0;
    }
    if ((reading1 == HIGH || reading2 == HIGH) && var_flecha == 194) {
      juego = 4;
      modo = 0;
      menu = 0;
    }
  }
}
void reiniciar(void) {
  LCD_Clear(0x00);
  for (int x = 0; x < 315 - 128; x++) {
    LCD_Bitmap(x, 100, 127, 39, logo);
    V_line( x - 1, 100, 30, 0x0000);
    delay(7);
  }
  Serial.println("highscores");
  puntaje1 = 0;
  puntaje2 = 0;
  puntaje = 0;
  J1 = 80;
  J2 = 240;
  D1 = J1 + 7;
  D2 = J2 + 7;
  Disp = 0;
  antirebote1 = 0;
  antirebote2 = 0;
  Y1 = 300;
  Y2 = 300;
  var = 0;
  CI = 15;
  mov = 1;
  vuelta = 0;
  LI = 40;
  M1 = 1;
  M2 = 1;
  M3 = 1;
  M4 = 1;
  M5 = 1;
  vida1 = 3;
  vida2 = 3;
  tanque1 = 1;
  tanque2 = 1;
  nivel = 8;
  delay(1000);
  LCD_Clear(0x00);
  menu = 1;
  menuu();
  LCD_Clear(0x00);
  if (juego != 4){
    LCD_Bitmap(J1, 200, 13, 8, chunche);
    H_line(0, 209, 319,  0xD0A3);
    H_line(0, 210, 319,  0xD0A3);
    H_line(0, 211, 319,  0xD0A3);
  
    if (modo != 2) {
      // dos jugadores divididos en 170
      V_line(160, 195, 209,  0xD0A3);
      LCD_Print("jugador1 = 3 vidas", 10, 220, 1, 0x3E1C, 0);
      LCD_Print("jugador2 = 3 vidas", 165, 220, 1, 0x3E1C, 0);
  
  
      LCD_Bitmap(J2, 200, 13, 8, chunche);
    } else {
      LCD_Print("jugador = 3 vidas", 10, 220, 1, 0x3E1c, 0);
      LCD_Print("                 ", 165, 220, 1, 0x3E1C, 0);
    }
  }
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
  for (uint8_t i = 0; i < 8; i++) {
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
  LCD_DATA(0x40 | 0x80 | 0x20 | 0x08); // LCD_DATA(0x19);
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
void LCD_Clear(unsigned int c) {
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
  unsigned int i, j;
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
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
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
  x2 = x + w;
  y2 = y + h;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = w * h * 2 - 1;
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

  if (fontSize == 1) {
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if (fontSize == 2) {
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }

  char charInput ;
  int cLength = text.length();
  //  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength + 1];
  text.toCharArray(char_array, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    //    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2) {
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
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + width;
  y2 = y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k + 1]);
      //LCD_DATA(bitmap[k]);
      k = k + 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 =   x + width;
  y2 =    y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  int k = 0;
  int ancho = ((width * columns));
  if (flip) {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width - 1 - offset) * 2;
      k = k + width * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k - 2;
      }
    }
  } else {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width + 1 + offset) * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k + 2;
      }
    }


  }
  digitalWrite(LCD_CS, HIGH);
}
