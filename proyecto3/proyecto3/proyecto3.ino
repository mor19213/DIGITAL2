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
int musica[] = {PE_0, PF_0};
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
int puntajeT = 0;
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
int tempx1;
int tempy1;
int tempx2;
int tempy2;
int tempx3;
int tempy3;
int tempx4;
int tempy4;
int tempx5;
int tempy5;
int var_fondo = 0;
int estrx = 0;
int estry = 0;
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
void fondos(int);

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
    //  Abrir archivo de highscores para 2 jugadores
    while (highscore1[var - 1] != 10) { // leer hasta el enter
      highscore1[var] = modo2.read();
      if ((highscore1[var] < 48) && (highscore1[var] != 10)) { // si el caracter es menor a 0 y no es un enter
        highscore1[var] = 48;
      }
      var++; // incrementar el indice del array
    }
    var = 0;  // reinicar el indice del array
    while (highscore2[var - 1] != 10) { // leer los caracteres hasta el enter
      highscore2[var] = modo2.read();
      if ((highscore2[var] < 48) && (highscore2[var] != 10)) {
        highscore2[var] = 48;
      }
      var++;
    }
    var = 0;  // reiniciar el indice del array
    while (highscore3[var - 1] != 10) { // leer los caracteres hasta el enter
      highscore3[var] = modo2.read();
      if ((highscore3[var] < 48) && (highscore3[var] != 10)) {
        highscore3[var] = 48;
      }
      var++;
    }
    var = 0; // reiniciar el indice del array
    Serial.println("leer modo 1");
    // close the file:
    modo2.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening 2jugador.txt");
  }
  // convertir a int el array 
  high1 = (((int)highscore1[0] - 48) * 100) + (((int)highscore1[1] - 48) * 10) + ((int)highscore1[2] - 48);
  high2 = (((int)highscore2[0] - 48) * 100) + (((int)highscore2[1] - 48) * 10) + ((int)highscore2[2] - 48);
  high3 = (((int)highscore3[0] - 48) * 100) + (((int)highscore3[1] - 48) * 10) + ((int)highscore3[2] - 48);
  Serial.println(high1);
  Serial.println(high2);
  Serial.println(high3);

  //*********************************** highscores para modo de 1 jugador
  // re-open the file for reading:
  modo2 = SD.open("1jugador.txt");
  if (modo2) {
    //    Serial.println("1jugador.txt:");

    while (highscore12[var - 1] != 10) {  // leer los caracteres hasta el enter
      highscore12[var] = modo2.read();
      if ((highscore12[var] < 48) && (highscore12[var] != 10)) {
        highscore12[var] = 48;
      }
      var++;  // incrementar el indice del array
    }
    var = 0;  // reiniciar el indice del array
    while (highscore22[var - 1] != 10) {  // leer los caracteres hasta el enter
      highscore22[var] = modo2.read();
      if ((highscore22[var] < 48) && (highscore22[var] != 10)) {
        highscore22[var] = 48;
      }
      var++;  // incrementar el indice del array
    }
    var = 0;  // reiniciar el indice del array 
    while (highscore32[var - 1] != 10) { // leer los caracteres hasta el enter
      highscore32[var] = modo2.read();
      if ((highscore32[var] < 48) && (highscore32[var] != 10)) {
        highscore32[var] = 48;
      }
      var++;  // incrementar el indice del array
    }
    var = 0;  // reiniciar el inidce del array
    Serial.println("leer modo 2");
    // close the file:
    modo2.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening 1jugador.txt");
  }
  // guardar en int los puntajes
  high12 = (((int)highscore12[0] - 48) * 100) + (((int)highscore12[1] - 48) * 10) + ((int)highscore12[2] - 48);
  high22 = (((int)highscore22[0] - 48) * 100) + (((int)highscore22[1] - 48) * 10) + ((int)highscore22[2] - 48);
  high32 = (((int)highscore32[0] - 48) * 100) + (((int)highscore32[1] - 48) * 10) + ((int)highscore32[2] - 48);
  Serial.println(high12);
  Serial.println(high22);
  Serial.println(high32);

// inicializar la pantalla 
  LCD_Init();
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  // botones de los jugadores
  pinMode(izq1, INPUT);
  pinMode(der1, INPUT);
  pinMode(izq2, INPUT);
  pinMode(der2, INPUT);
  pinMode(disp1, INPUT);
  pinMode(disp2, INPUT);
  // pines para indicar al pic la musica
  pinMode(musica[0], OUTPUT);
  pinMode(musica[1], OUTPUT);
  // no sonar musica
  digitalWrite(musica[0], LOW);//RB6
  digitalWrite(musica[1], LOW);//RB7
  reiniciar(); // reiniciar el juego y las variables
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  if (juego != 4) {
    // sonar musica de juego
    digitalWrite(musica[0], LOW);//RB6
    digitalWrite(musica[1], HIGH);//RB7
  }

  fondos(0);  // dibujar el fondo del juego
  while (juego == 1) {  // modo de juego 1
    var++;              // incrementar la variable para hacer disparos random
    if (var == 11) {    // limite de la variable 
      var = 1;          
    }
    var_fondo++;        // incrementar variable para fondo
    if (var_fondo == 20) {  // limite de fondos
      var_fondo = 1;
    }
    fondos(var_fondo);  // pintar uno de los fondos
    //    delay(2);
    if (Y1 < 186 || Y2 < 186) { // si se esta disparando hacer el sonido de disparo 
      digitalWrite(musica[0], HIGH);//RB6
    } else {
      digitalWrite(musica[0], LOW);//RB6
    }
    itoa(puntaje1, snum, 10);     // pasar puntaje1 a string
    LCD_Print(snum, 5, 5, 1, 0x3E1C, 0);  // poner el puntaje en la pantalla
    itoa(puntaje2, snum, 10);     // pasar puntaje2 a string
    LCD_Print(snum, 300, 5, 1, 0x3E1C, 0);  // poner el puntaje en la pantalla
    // si se matan a todos los malos
    malos = M1 + M2 + M3 + M4 + M5;
    if (malos == 0) { // ya no hay malos y se termina el nivel
      LCD_Print("NIVEL COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      nivel--;
      delay(1500); // reiniciar variabels
      LI = 20;    // posicion de los malos
      M1 = 1;     // pintar a los malos
      M2 = 1;
      M3 = 1;
      M4 = 1;
      M5 = 1;
      MY1 = 300;   // coordenadas de los disparos 
      MY2 = 300;
      MX1 = 400;
      MX2 = 400;
      MD1 = 0;
      MD2 = 0;
      for (int x = 0; x < 320; x++) { // pantalla 
        V_line(x, 0, 195, 0);
      }
      Y1 = 300;   // reiniciar disparos de tanques 
      Y2 = 300;
      vuelta = 0;
    }
    reading1 = digitalRead(disp1);  // leer botones 
    reading2 = digitalRead(disp2);

    if (MD1 == 0) {   // si no se esta disparando 
      var1++; 
      V_line(MX1, MY1, 12, 0);  // borrar ultimo disparo
      if (var1 > 6) {
        if (M1 == 1 && var == 2) {  // escoger que malo dispara random
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
    if (MD1 == 1) { // si se esta disparando 
      V_line(MX1, MY1, 6, 0xD0A3);  // disbujar disparo
      V_line(MX1, MY1 - 6, 6, 0x00);  // borrar disparo anterior
      MY1++;  // aumentar coordenada en y del disparo 
      if (MY1 > 194) {  // si el disparo llega a la coordenada del tanque 
        MD1 = 0;
        if (J1 < MX1 && MX1 < (J1 + 14)) {  // si esta en la coordenada x del tanque1 
          vida1--;  // pierde una vida

        }
        if (J2 < MX1 && MX1 < (J2 + 14)) {  // si esta en la coordenada x del tanque2
          vida2--;  // pierde una vida
        }
        if (vida1 == 0) { // el jugador 1 se queda sin vidas
          LCD_Print("jugador1 = 0 vidas", 10, 220, 1, 0x3E1C, 0);

          for (int x = J1; x < J1 + 15; x++) {
            V_line(x - 1, 200, 8, 0);   // borar tanque 1
          }
          if (Y1 < 186) {   // si el tanque estaba disparando 
            V_line(D1, 185 - Y1, 6, 0); // borra disparo
            Y1 = 390;
          }
          tanque1 = 0;
        } else if (vida1 == 2) {  // escribir cuantas vidas le quedan
          LCD_Print("jugador1 = 2 vidas", 10, 220, 1, 0x3E1C, 0);
        } else if (vida1 == 1) {
          LCD_Print("jugador1 = 1 vidas", 10, 220, 1, 0x3E1C, 0);
        }
        if (vida2 == 0) { // si el tanque2 se queda sin vidas
          tanque2 = 0;    // ya no dibujarlo y borrar el ultimo dibujo
          for (int x = J2; x < J2 + 15; x++) {
            V_line(x - 1, 200, 8, 0);
          }
          if (Y2 < 186) {
            V_line(D2, 185 - Y2, 6, 0); // borrar ultimo disparo 
            Y2 = 300;
          }
          LCD_Print("jugador2 = 0 vidas ", 165, 220, 1, 0x3E1C, 0); // colocar cuantas vidas le quedan 
        } else if (vida2 == 1) {
          LCD_Print("jugador2 = 1 vida  ", 165, 220, 1, 0x3E1C, 0);
        } else if (vida2 == 2) {
          LCD_Print("jugador2 = 2 vidas ", 165, 220, 1, 0x3E1C, 0);
        }
      }
    }
    V_line(160, 195, 209,  0xD0A3);
    // mostrar tanquecitos
    if (tanque1 == 1) {
      tanque_1();
    }
    if (tanque2 == 1) {
      tanque_2();
    }
    // mostrar malos si == 1
    if (M1 == 1) { // dibujar malo
      LCD_Bitmap(CI, LI, 10, 8, malo1);
    } else if (M1 < 7 && M1 != 0) {   // aumentar variable para hacer delay
      M1++;
    }  else if (M1 > 6) { // terminar delay
      M1 = 0;
      for (int x = tempx1; x < tempx1 + 13; x++) {
        V_line (x, tempy1, 8, 0);
      }
    }
    if (M2 == 1) {
      LCD_Bitmap(CI + 20, LI, 10, 8, malo1); // dibujar malo
    } else if (M2 < 7 && M2 != 0) {  // aumentar variable para hacer delay
      M2++;
    } else if (M2 > 6) { // terminar delay
      M2 = 0;
      for (int x = tempx2; x < tempx2 + 13; x++) {
        V_line (x, tempy2, 8, 0);
      }
    }
    if (M3 == 1) {
      LCD_Bitmap(CI + 40, LI, 10, 8, malo1); // dibujar malo
    } else if (M3 < 7 && M3 != 0) {  // aumentar variable para hacer delay
      M3++;
    }  else if (M3 > 6) { // terminar delay
      M3 = 0;
      for (int x = tempx3; x < tempx3 + 13; x++) {
        V_line (x, tempy3, 8, 0);
      }
    }
    if (M4 == 1) {
      LCD_Bitmap(CI + 60, LI, 10, 8, malo1); // dibujar malo
    } else if (M4 < 7 && M4 != 0) {  // aumentar variable para hacer delay
      M4++;
    }  else if (M4 > 6) { // terminar delay
      M4 = 0;
      for (int x = tempx4; x < tempx4 + 13; x++) {
        V_line (x, tempy4, 8, 0);
      }
    }
    if (M5 == 1) {
      LCD_Bitmap(CI + 80, LI, 10, 8, malo1); // dibujar malo
    } else if (M5 < 7 && M5 != 0) {  // aumentar variable para hacer delay
      M5++;
    }  else if (M5 > 6) { // terminar delay
      M5 = 0;
      for (int x = tempx5; x < tempx5 + 13; x++) {
        V_line (x, tempy5, 8, 0);
      }
    }


    if (mov == 1) { // movimiento de los malos 
      CI++;
    } else if (mov == 0) {
      CI--;
    }
    if (CI > 220) { // limite derecha
      mov = 0;
    } else if (CI < 10) { // limite izquierda 
      mov = 1;
      vuelta++; // aumentar la cantidad de vueltas que han dados los malos 
    }
    if (vuelta > nivel) { // si la cantidad de vueltas superan el nivel, bajar a los malos (en el eje y)
      vuelta = 0;
      LCD_Print("                          ", CI, LI, 2, 0x3E1C, 0);
      LI = LI + 25;
    }

    if (tanque1 == 0 && tanque2 == 0) {
      LI = 400; // si ambos tanques no tienen vidas terminal el juego
    }
    // jugador pierde
    if (LI > 160) { // si los malos llegan hasta abajo (eje y) 
      LCD_Clear(0x00); // borrar la pantalla y terminar el juego 
      LCD_Print("GAME OVER", 25, 15, 2, 0xD0A3, 0);
      juego = 0;
    }
    // jugador gana
    if (nivel == 1) { // si se llega al nivel 1 se termina el juego 
      LCD_Clear(0x00);
      LCD_Print("JUEGO COMPLETADO", 25, 5, 2, 0xFFFF, 0);
      juego = 0;
    }
  }

  while (juego == 2) { // modo de juego de 1 jugador
    var++;             // incrementar la variable para hacer disparos random
    if (var == 11) {   // limite de variable 
      var = 1;
    }
    var_fondo++;      // incrementar variable para fondo 
    if (var_fondo == 20) {  // limite de fondos 
      var_fondo = 1;
    }
    fondos(var_fondo);  // pintar uno de los fondos 
    delay(2);
    if (Y1 < 186 || Y2 < 186) { // si hay algun disparo, musica 11
      digitalWrite(musica[0], HIGH);//RB6
    } else {
      digitalWrite(musica[0], LOW);//RB6
    }
    // si se matan a todos los malos
    malos = M1 + M2 + M3 + M4 + M5;
    if (malos == 0) { // pasar al siguiente nivel
      digitalWrite(musica[0], LOW);//RB6 = 1;
      LCD_Print("NIVEL COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      nivel--;
      delay(1500);
      // reiniciar posicion de los malos
      LI = 20;  // posicion de los malos
      M1 = 1;   // pintar a los malos
      M2 = 1;
      M3 = 1;
      M4 = 1;
      M5 = 1;
      // reiniciar los disparos de los malos
      MY1 = 300;  // coordenadas de los disparos 
      MY2 = 300;
      MX1 = 400;
      MX2 = 400;
      MD1 = 0;
      MD2 = 0;
      // borrar pantalla
      for (int x = 0; x < 320; x++) { // pantalla 
        V_line(x, 0, 195, 0);
      }
      Y1 = 300; // reiniciar disparos de tanques 
      Y2 = 300;
      vuelta = 0;
    }
    itoa(puntaje1, snum, 10);
    LCD_Print(snum, 5, 5, 1, 0x3E1C, 0);
    reading1 = digitalRead(disp1);  // leer botones 
    reading2 = digitalRead(disp2);

    if (MD1 == 0) { // si no se esta disparando 
      var1++;
      V_line(MX1, MY1, 12, 0);
      if (var1 > 6) {
        if (M1 == 1 && var == 2) {  // escoger que malo dispara random 
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
    if (MD1 == 1) { // si se esta disparando 
      V_line(MX1, MY1, 6, 0xD0A3);  // dibujar disparo
      V_line(MX1, MY1 - 6, 6, 0x00);  // borrar disparo anterior 
      MY1++;  // aumentar coordenada en "y" del disparo 
      if (MY1 > 194) {  // si el disparo llega a la coordenada del tanque 
        MD1 = 0;
        if (tanque1 != 2) {
          if (J1 < MX1 && MX1 < (J1 + 13)) { // si esta en la coordenada "x" del tanque1 
            vida1--;  // pierde una vida 
            //            LCD_Print("wuuuuu", 25, 100, 2, 0x3E1C, 0);
          }
        }
        if (vida1 == 0) { // jugador sin vidas 
          tanque1 = 0;
          //          LCD_Print("sin vidas jugador 2", 25, 100, 2, 0x3E1C, 0);
          LCD_Print("jugador = 0 vidas", 10, 220, 1, 0x3E1C, 0);
          for (int x = J1; x < (J1 + 14); x++) {  // borrar tanque
            V_line( x, 200, 8, 0x0000);
          }
          LCD_Clear(0x00);  // terminar juego  
          LCD_Print("GAME OVER", 25, 5, 2, 0xD0A3, 0);  
          juego = 0;
          M1 = 0;
          M2 = 0;
          M3 = 0;
          M4 = 0;
          M5 = 0;
          tanque1 = 0;
          tanque2 = 0;
          tanque1 = 0;
          digitalWrite(musica[0], LOW);//RB6
          LCD_Clear(0x00);
          //            LCD_Print("GAME OVER", 25, 5, 2, 0x3E1C, 0);
          juego = 0;
          M1 = 0;
          M2 = 0;
          M3 = 0;
          M4 = 0;
          M5 = 0;
          tanque2 = 0;
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
    if (M1 == 1) {  // dibujar malo
      LCD_Bitmap(CI, LI, 10, 8, malo1); // aumentar variable para hacer delay 
    } else if (M1 < 7 && M1 != 0) { // aumentar variable para hacer delay 
      M1++;
    }  else if (M1 > 6) { // terminar delay
      M1 = 0;
      for (int x = tempx1; x < tempx1 + 13; x++) {
        V_line (x, tempy1, 8, 0);
      }
    }
    if (M2 == 1) { // dibujar malo
      LCD_Bitmap(CI + 20, LI, 10, 8, malo1);  // aumentar variable para hacer delay 
    } else if (M2 < 7 && M2 != 0) { // aumentar variable para hacer delay
      M2++;
    } else if (M2 > 6) { // terminar delay
      M2 = 0;
      for (int x = tempx2; x < tempx2 + 13; x++) { // aumentar variable para hacer delay
        V_line (x, tempy2, 8, 0);
      }
    }
    if (M3 == 1) {  // dibujar malo
      LCD_Bitmap(CI + 40, LI, 10, 8, malo1); // aumentar variable para hacer delay 
    } else if (M3 < 7 && M3 != 0) { // aumentar variable para hacer delay
      M3++;
    }  else if (M3 > 6) { // terminar delay
      M3 = 0;
      for (int x = tempx3; x < tempx3 + 13; x++) {
        V_line (x, tempy3, 8, 0);
      }
    }
    if (M4 == 1) {  // dibujar malo
      LCD_Bitmap(CI + 60, LI, 10, 8, malo1); // aumentar variable para hacer delay 
    } else if (M4 < 7 && M4 != 0) { // aumentar variable para hacer delay
      M4++;
    }  else if (M4 > 6) { // terminar delay
      M4 = 0;
      for (int x = tempx4; x < tempx4 + 13; x++) {
        V_line (x, tempy4, 8, 0);
      }
    }
    if (M5 == 1) {  // dibujar malo
      LCD_Bitmap(CI + 80, LI, 10, 8, malo1); // aumentar variable para hacer delay 
    } else if (M5 < 7 && M5 != 0) { // aumentar variable para hacer delay
      M5++;
    }  else if (M5 > 6) { // terminar delay
      M5 = 0;
      for (int x = tempx5; x < tempx5 + 13; x++) {
        V_line (x, tempy5, 8, 0);
      }
    }


    if (mov == 1) { // movimiento de los malos 
      CI++;
    } else if (mov == 0) {
      CI--;
    }
    if (CI > 220) { // limite derecha
      mov = 0;
    } else if (CI < 10) { // limite izquierda
      mov = 1;
      vuelta++;
    }
    if (vuelta > nivel) { // aumentar la cantidad de vueltas que han dado los malos
      vuelta = 0;
      LCD_Print("                          ", CI, LI, 2, 0x3E1C, 0);
      LI = LI + 25;
    }


    // jugador pierde
    if (LI > 160) {
      LCD_Clear(0x00);
      LCD_Print("GAME OVER", 25, 5, 2, 0x3E1C, 0);
      juego = 0;
    }
    // jugador gana
    if (nivel == 1) {
      LCD_Clear(0x00);
      LCD_Print("JUEGO COMPLETADO", 25, 5, 2, 0xFFFF, 0);
      juego = 0;
    }
  }

  while (juego == 3) {  // j1 vs j2
    var++;              // incrementar la variable para hacer disparos random
    if (var == 11) {    // limite de la variable
      var = 1;
    }
    var_fondo++;        // incrementar variable para fondo 
    if (var_fondo == 20) {  // limite de fondos 
      var_fondo = 1;
    }
    fondos(var_fondo);  // pintar uno d elos fondos 
    //    delay(2);

    if (Y1 < 186 || Y2 < 186) { // si se esta disparando hacer el sonido de disparo 
      digitalWrite(musica[0], HIGH);//RB6
    } else {
      digitalWrite(musica[0], LOW);//RB6
    }
    itoa(puntaje1, snum, 10); // pasar puntaje1 a string
    LCD_Print(snum, 5, 5, 1, 0x3E1C, 0);  // pintar en la pantalla
    itoa(puntaje2, snum, 10); // pasar puntaje2 a string
    LCD_Print(snum, 300, 5, 1, 0x3E1C, 0);  // poner el puntaje en la pantalla 
    // si se matan a todos los malos
    malos = M1 + M2 + M3 + M4 + M5;
    if (malos == 0) { // ya nohay malos y se termina nivel
      LCD_Print("NIVEL COMPLETADO", 25, 5, 2, 0x3E1C, 0);
      nivel--;
      delay(1500);  // reiniciar variables
      LI = 20;      // posicion de los malos
      M1 = 1;       // variable para pintar malos 
      M2 = 1;
      M3 = 1;
      M4 = 1;
      M5 = 1;
      MY1 = 300;    // coordenadas de los disparos 
      MY2 = 300;
      MX1 = 400;
      MX2 = 400;
      MD1 = 0;
      MD2 = 0;
      for (int x = 0; x < 320; x++) { // pantalla 
        V_line(x, 0, 195, 0);
      }
      Y1 = 300;   // reiniciar disparos de tanques 
      Y2 = 300;
      vuelta = 0;
    }
    reading1 = digitalRead(disp1); // leer botones
    reading2 = digitalRead(disp2);

    if (MD1 == 0) { // si no esta disparando 
      var1++;
      V_line(MX1, MY1, 12, 0);  // borrar ultimo disparo 
      if (var1 > 6) {
        if (M1 == 1 && var == 2) {  // escoger que malo dispara random
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
    if (MD1 == 1) { // si se esta disparando 
      V_line(MX1, MY1, 6, 0xD0A3);  // dibujar disparo 
      V_line(MX1, MY1 - 6, 6, 0x00);  // borrar disparo anterior 
      MY1++;  // aumentar coordenada en "y" del disparo 
      if (MY1 > 194) {  // si el disparo llega a la coordenada del tanque   
        MD1 = 0;
        if (tanque1 != 2) {
          if (J1 < MX1 && MX1 < (J1 + 13)) {  // si esta en la coordenada x tanque1
            vida1--;  // pierde una vida 
            //            LCD_Print("wuuuuu", 25, 100, 2, 0x3E1C, 0);
          }
        }
        if (J2 < MX1 && MX1 < (J2 + 13)) {  // si esta en la coordenada x tanque2
          vida2--;  // pierde una vida 
          //LCD_Print("wuuuuu", 25, 100, 2, 0x3E1C, 0);
        }
        if (vida1 == 0) { // el jugador 1 se queda sin vidas 
          LCD_Clear(0x00);  // acabar juego 
          LCD_Print("GAME OVER", 25, 5, 2, 0xD0A3, 0);
          juego = 0;
          M1 = 0;
          M2 = 0;
          M3 = 0;
          M4 = 0;
          M5 = 0;
          tanque1 = 0;
          tanque2 = 0;
        } else if (vida1 == 2) {
          LCD_Print("jugador1 = 2 vidas", 10, 220, 1, 0x3E1C, 0);
        } else if (vida1 == 1) {
          LCD_Print("jugador1 = 1 vidas", 10, 220, 1, 0x3E1C, 0);
        }
        if (vida2 == 0) {
          LCD_Clear(0x00);  // acabar juego 
          LCD_Print("GAME OVER", 25, 5, 2, 0xD0A3, 0);
          juego = 0;
          M1 = 0;
          M2 = 0;
          M3 = 0;
          M4 = 0;
          M5 = 0;
          tanque1 = 0;
          tanque2 = 0;
        } else if (vida2 == 1) {
          LCD_Print("jugador2 = 1 vida  ", 165, 220, 1, 0x3E1C, 0);
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
    if (M1 == 1) { // dibujar malo
      LCD_Bitmap(CI, LI, 10, 8, malo1);
    } else if (M1 < 7 && M1 != 0) {   // aumentar variable para hacer delay
      M1++;
    }  else if (M1 > 6) { // terminar delay
      M1 = 0;
      for (int x = tempx1; x < tempx1 + 13; x++) {
        V_line (x, tempy1, 8, 0);
      }
    }
    if (M2 == 1) { // dibujar malo
      LCD_Bitmap(CI + 20, LI, 10, 8, malo1);
    } else if (M2 < 7 && M2 != 0) {   // aumentar variable para hacer delay
      M2++;
    } else if (M2 > 6) { // terminar delay
      M2 = 0;
      for (int x = tempx2; x < tempx2 + 13; x++) {
        V_line (x, tempy2, 8, 0);
      }
    }
    if (M3 == 1) { // dibujar malo
      LCD_Bitmap(CI + 40, LI, 10, 8, malo1);
    } else if (M3 < 7 && M3 != 0) {   // aumentar variable para hacer delay
      M3++;
    }  else if (M3 > 6) { // terminar delay
      M3 = 0;
      for (int x = tempx3; x < tempx3 + 13; x++) {
        V_line (x, tempy3, 8, 0);
      }
    }
    if (M4 == 1) { // dibujar malo
      LCD_Bitmap(CI + 60, LI, 10, 8, malo1);
    } else if (M4 < 7 && M4 != 0) {   // aumentar variable para hacer delay
      M4++;
    }  else if (M4 > 6) { // terminar delay
      M4 = 0;
      for (int x = tempx4; x < tempx4 + 13; x++) {
        V_line (x, tempy4, 8, 0);
      }
    }
    if (M5 == 1) { // dibujar malo
      LCD_Bitmap(CI + 80, LI, 10, 8, malo1);
    } else if (M5 < 7 && M5 != 0) {   // aumentar variable para hacer delay
      M5++;
    }  else if (M5 > 6) { // terminar delay
      M5 = 0;
      for (int x = tempx5; x < tempx5 + 13; x++) {
        V_line (x, tempy5, 8, 0);
      }
    }


    if (mov == 1) { // movimiento de los malos
      CI++;
    } else if (mov == 0) {
      CI--;
    }
    if (CI > 220) { // limite derecha
      mov = 0;
    } else if (CI < 10) { // limite izquierda 
      mov = 1;
      vuelta++; // aumentar la cantidad de vueltas que han dados los malos 
    }
    if (vuelta > nivel) { // si la cantidad de vueltas superan el nivel, bajar a los malos (en el eje y)
      vuelta = 0;
      LCD_Print("                          ", CI, LI, 2, 0x3E1C, 0);
      LI = LI + 25;
    }

    if (tanque1 == 0 || tanque2 == 0) {
      LI = 400; // si alguno de los tanques no tienen vidas terminal el juego
    }
    // jugador pierde
    if (LI > 160) { // si los malos llegan hasta abajo (eje y) 
      LCD_Clear(0x00); // borrar la pantalla y terminar el juego
      LCD_Print("GAME OVER", 25, 15, 2, 0x3E1C, 0);
      juego = 0;
    }
    // jugador gana
    if (nivel == 1) { // si se llega al nivel 1 se termina el juego 
      LCD_Clear(0x00);
      LCD_Print("JUEGO COMPLETADO", 25, 5, 2, 0xFFFF, 0);
      juego = 0;
    }
  }

  fondos(0);
  LCD_Print("Presionar para ", 5, 190, 1, 0x3E1C, 0);
  LCD_Print(" ir al menu", 5, 200, 1, 0x3E1C, 0);
  if (juego == 4) {
    // mostrar los highscores
    // leer los archivos de la SD
    LCD_Print("Highscores", 80, 5, 2, 0xFFFF, 0);
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
    LCD_Print(snum, 90, 70, 2, 0x3E1C, 0);
    LCD_Print("vs", 125, 70, 2, 0x3E1C, 0);
    itoa(puntaje2, snum, 10);
    LCD_Print(snum, 170, 70, 2, 0x3E1C, 0);
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
    puntaje1 = puntaje1 * (vida1 + 1);
    itoa(puntaje1, snum, 10);
    LCD_Print("Puntaje Final:", 85, 70, 2, 0x3E1C, 0);
    LCD_Print(snum, 85, 100, 2, 0x3E1C, 0);
    highscores(puntaje1, 2);
  } else if (modo == 1) {
    // 2 jugadores juntos
    //    puntaje1 = puntaje1 * (vida1 + 1);
    //    puntaje2 = puntaje2 * (vida2 + 1);
    puntajeT = puntaje1 + puntaje2;
    itoa(puntajeT, snum, 10);
    LCD_Print("Puntaje Final:", 65, 85, 2, 0x3E1C, 0);
    LCD_Print(snum, 125, 110, 2, 0x3E1C, 0);
    highscores(puntajeT, 1);
  }

  while (juego == 0) {  // si se acaba el juego 
    digitalWrite(musica[0], LOW);//RB6
    digitalWrite(musica[1], LOW);//RB7
    modo = 0;
    reading1 = digitalRead(disp1);  // leer botones
    reading2 = digitalRead(disp2);  // leer botones 
    if (reading1 == HIGH || reading2 == HIGH) {
      reiniciar();  // si se preciona reiniciar el juego 
    }
  }
}
// 1  - modo 1
// 2 - modo 2
void highscores(int puntaje, int num) { // comparar el ultimo puntaje 
  if (num == 1) {

    Serial.println("......");
    if (puntaje > high1) {  // determinar en que lugar esta el nuevo puntaje
      high3 = high2;
      high2 = high1;
      high1 = puntaje;
      LCD_Print("new 1st highscore", 150, 170, 1, 0x3E1C, 0); // escribir si es un nuevo highscore
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
    modo2 = SD.open("2jugador.txt", FILE_WRITE);  // documento jugador 2
    // if the file opened okay, write to it:
    if (modo2) {
      Serial.println("Writing to 2jugador.txt...");
      Serial.println(high1);
      Serial.println(high2);
      Serial.println(high3);
      itoa(high1, snum, 10);  // pasar high1 a string
      if (high1 > 99) { // determinar cuantos digitos tiene high1
        modo2.print(snum[0]); 
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high1 > 9) {
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");  // escribir el enter


      itoa(high2, snum, 10);  //  determiar cuantos digitos tiene el high2
      if (high2 > 99) {  
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high2 > 9) {
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");      // escribir el enter


      itoa(high3, snum, 10);  // pasar high3 a string
      if (high3 > 99) {       // determinar cuantos digitos tiene el high3
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high3 > 9) {
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");      // escribir el enter

      // close the file:
      modo2.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening 2jugador.txt");
    }
  }
  if (num == 2) {   // para puntaje de jugador 1

    Serial.println("......");
    if (puntaje > high12) { // determinar en que posicion esta el nuevo puntaje 
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
    SD.remove("1jugador.txt");  // borrar datos anteriores 
    modo2 = SD.open("1jugador.txt", FILE_WRITE);
    // if the file opened okay, write to it:
    if (modo2) {
      Serial.println("Writing to 2jugador.txt...");
      Serial.println(high12);
      Serial.println(high22);
      Serial.println(high32);
      itoa(high12, snum, 10); // pasar a string
      if (high12 > 99) {  // determinar cuantos digitos tiene
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high12 > 9) {
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");  // escribir el enter


      itoa(high22, snum, 10); // pasar high2 a string
      if (high22 > 99) {  // determinar cuantos digitos tiene
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high22 > 9) {
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");  // escribir el enter


      itoa(high32, snum, 10); // pasar high3 a string
      if (high32 > 99) {  // determinar cuantos digitos tiene
        modo2.print(snum[0]);
        modo2.print(snum[1]);
        modo2.print(snum[2]);
      } else if (high32 > 9) {
        modo2.print("0");
        modo2.print(snum[0]);
        modo2.print(snum[1]);
      } else {
        modo2.print("0");
        modo2.print("0");
        modo2.print(snum[0]);
      }
      modo2.print("\n");    // escribir el enter

      // close the file:
      modo2.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening 1jugador.txt");
    }
  }
}
void tanque_2(void) { // dibujar tanque y disparo 
  J2 = (J2 > 306 ? 306 : J2); // movimiento del tanque 2
  J2 = (J2 < 161 ? 161 : J2);

  LCD_Bitmap(J2, 200, 13, 8, chunche);  // dibujar el tanque
  int B_der2 = digitalRead(der2);
  if (B_der2 == HIGH) {
    J2++; // mover el tanque
    V_line(J2 - 1, 200, 8, 0);
  }
  int B_izq2 = digitalRead(izq2);
  if (B_izq2 == HIGH) {
    J2--; // mover el tanque 
    V_line(J2 + 14, 200, 8, 0);
  }
  if (Y2 < 186) { // si se esta disparando 
    V_line(D2, 185 - Y2, 6, 0x3E1C);  // dibuar disparo
    V_line(D2, 185 + 6 - Y2, 6, 0x00);  // borrar disparo anterior 
    Y2++; // mover disparo 
    if (185 - Y2 >= (LI - 7) && (185 - Y2) <= (LI + 7)) { // si el disparo llega a la coordenada en "y" del malo
      if ((CI - 1) <= D2 && (CI + 10) >= D2 && M1 == 1) { // si el disparo esta en el eje "x" del malo
        Y2 = 190; // terminar disparo 
        V_line(D2, LI - 6, 20, 0x0);  // borrar disparo 
        LCD_Bitmap(CI, LI, 13, 8, explo); // dibujar explosion 
        puntaje2++; // aumentar disparo
        M1++;   // aumentar valor de malo 
        tempx1 = CI;  // posicion de la explosion en "x"
        tempy1 = LI;  // posicion de la explosion en "y"
      } else if ((CI + 20) <= D2 && (CI + 30) >= D2 && M2 == 1) { // si el disparo esta en el eje "x" del malo
        Y2 = 190; // terminar disparo 
        V_line(D2, LI - 6, 20, 0x0);  // borrar disparo 
        LCD_Bitmap(CI + 20, LI, 13, 8, explo); // dibujar explosion 
        puntaje2++; // aumentar disparo
        M2++;   // aumentar valor de malo 
        tempx2 = CI + 20;  // posicion de la explosion en "x"
        tempy2 = LI;  // posicion de la explosion en "y"
      } else if ((CI + 40) <= D2 && (CI + 50) >= D2 && M3 == 1) { // si el disparo esta en el eje "x" del malo
        Y2 = 190; // terminar disparo 
        V_line(D2, LI - 6, 20, 0x0);  // borrar disparo 
        LCD_Bitmap(CI + 40, LI, 13, 8, explo); // dibujar explosion 
        puntaje2++; // aumentar disparo
        M3++;   // aumentar valor de malo 
        tempx3 = CI + 40;  // posicion de la explosion en "x"
        tempy3 = LI;  // posicion de la explosion en "y"
      } else if ((CI + 60) <= D2 && (CI + 70) >= D2 && M4 == 1) { // si el disparo esta en el eje "x" del malo
        Y2 = 190; // terminar disparo 
        V_line(D2, LI - 6, 20, 0x0);  // borrar disparo 
        LCD_Bitmap(CI + 60, LI, 13, 8, explo); // dibujar explosion 
        puntaje2++; // aumentar disparo
        M4++;   // aumentar valor de malo 
        tempx4 = CI + 60;  // posicion de la explosion en "x"
        tempy4 = LI;  // posicion de la explosion en "y"
      } else if ((CI + 80) <= D2 && (CI + 90) >= D2 && M5 == 1) { // si el disparo esta en el eje "x" del malo
        Y2 = 190; // terminar disparo 
        V_line(D2, LI - 6, 20, 0x0);  // borrar disparo 
        LCD_Bitmap(CI + 80, LI, 13, 8, explo); // dibujar explosion 
        puntaje2++; // aumentar disparo
        M5++;   // aumentar valor de malo 
        tempx5 = CI + 80;  // posicion de la explosion en "x"
        tempy5 = LI;  // posicion de la explosion en "y"
      }
    }
  } else {
    V_line(D2, 0, 15, 0x00);// borrar disparo 
    if (reading2 == LOW) {
      antirebote2 = 1;
    } else {
      if (antirebote2 == 1) {
        antirebote2 = 0;
        D2 = J2 + 7;  // coordenadas disparo  
        Y2 = 0;       // variable par ainiciar disparo 
      }
    }
  }
}
void tanque_1(void) {
  if (modo != 2) {  // limite derecho del tanque1 para modo 2
    J1 = (J1 > 146 ? 146 : J1);
  } else {
    J1 = (J1 > 306 ? 306 : J1); // limite derecha del tanque2 para los demas modos
  }
  J1 = (J1 < 1 ? 1 : J1); // limites izquierda del tanque 1
  LCD_Bitmap(J1, 200, 13, 8, chunche);    // dibujo del tanque 
  int B_der1 = digitalRead(der1); 
  if (B_der1 == HIGH) {
    J1++; // mover a la derecha
    V_line(J1 - 1, 200, 8, 0);
  }
  int B_izq1 = digitalRead(izq1);
  if (B_izq1 == HIGH) { // mover a la izquierda 
    J1--;
    V_line(J1 + 14, 200, 8, 0);
  }
  if (Y1 < 186) { // se esta disparando 
    V_line(D1, 185 - Y1, 6, 0x3E1C);  // dibujar disparo 
    V_line(D1, 185 + 6 - Y1, 6, 0x00);  // borrar disparo anterior 
    Y1++; // mover disparo 
    if (185 - Y1 >= (LI - 7) && (185 - Y1) <= (LI + 7)) { // esta en la misma posicion en "y" del malo
      if ((CI - 1) <= D1 && (CI + 10) >= D1 && M1 == 1) { // esta en la misma posicion en "x" del malo 
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI, LI, 13, 8, explo);
        puntaje1++;
        M1++;
        tempx1 = CI;
        tempy1 = LI;
      } else if ((CI + 20) <= D1 && (CI + 30) >= D1 && M2 == 1) { // esta en la misma posicion en "x" del malo 
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 20, LI, 13, 8, explo);
        puntaje1++;
        M2++;
        tempx2 = CI + 20;
        tempy2 = LI;
      } else if ((CI + 40) <= D1 && (CI + 50) >= D1 && M3 == 1) { // esta en la misma posicion en "x" del malo 
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 40, LI, 13, 8, explo);
        puntaje1++;
        M3++;
        tempx3 = CI + 40;
        tempy3 = LI;
      } else if ((CI + 60) <= D1 && (CI + 70) >= D1 && M4 == 1) { // esta en la misma posicion en "x" del malo 
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 60, LI, 13, 8, explo);
        puntaje1++;
        M4++;
        tempx4 = CI + 60;
        tempy4 = LI;
      } else if ((CI + 80) <= D1 && (CI + 90) >= D1 && M5 == 1) { // esta en la misma posicion en "x" del malo 
        Y1 = 190;
        V_line(D1, LI - 6, 20, 0x0);
        LCD_Bitmap(CI + 80, LI, 13, 8, explo);
        puntaje1++;
        M5++;
        tempx5 = CI + 80;
        tempy5 = LI;
      }
    }
  } else {  // no se esta disparando 
    V_line(D1, 0, 15, 0x00);
    if (reading1 == LOW) {
      antirebote1 = 1;
    } else {
      if (antirebote1 == 1) {
        antirebote1 = 0;
        D1 = J1 + 7;  // coordenadas del disparo 
        Y1 = 0;
      }
    }
  }
}
void menuu(void) {  // menu del juego 
  //juego = 1;
  puntajeT = 0;
  var_flecha = 114; // posicion de la flecha 
  LCD_Bitmap(100, 50, 127, 39, logo);
  LCD_Print("2 jugadores", 40, 120, 2, 0xC992, 0);  // opciones de juego 
  LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
  LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
  LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
  digitalWrite(musica[0], HIGH);//RB6 // musica menu 
  digitalWrite(musica[1], LOW);//RB7
  while (menu == 1) {
    B_der1 = digitalRead(der1);
    B_der2 = digitalRead(der2);
    B_izq1 = digitalRead(izq1);
    B_izq2 = digitalRead(izq2);
    reading1 = digitalRead(disp1);
    reading2 = digitalRead(disp2);
    if (B_der1 == HIGH || B_der2 == HIGH) { // antirebote boton 1
      anti1 = 1;
    }
    if ((B_der1 == LOW && B_der2 == LOW) && anti1 == 1) { //  antirebote1
      anti1 = 0;
      if (var_flecha == 114) {
        var_flecha = 144;  // correr la flecha
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xC992, 0);// cambiar el color del seleccionado
        LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      } else  if (var_flecha == 144) {
        var_flecha = 174;  // correr la flecha
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xC992, 0);// cambiar el color del seleccionado
        LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      } else if (var_flecha == 174) {
        var_flecha = 204;  // correr la flecha
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xC992, 0);// cambiar el color del seleccionado
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      }
    }
    if (B_izq1 == HIGH || B_izq2 == HIGH) { // antirebote 2
      anti2 = 1;
    }
    if ((B_izq1 == LOW && B_izq2 == LOW) && anti2 == 1) { // antirebote 
      anti2 = 0;
      if (var_flecha == 174) {
        var_flecha = 144; // correr la flecha
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xC992, 0);// cambiar el color del seleccionado
        LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      } else if (var_flecha == 144) {
        var_flecha = 114; // correr la flecha
        LCD_Print("2 jugadores", 40, 120, 2, 0xC992, 0);// cambiar el color del seleccionado
        LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xFFFF, 0);
        LCD_Print("Highscores", 40, 210, 2, 0xFFFF, 0);
        for (int x = 20; x < 35; x++) {
          V_line( x, 114, 160, 0x0000);
        }
      } else if (var_flecha == 204) {
        var_flecha = 174; // correr la flecha
        LCD_Print("2 jugadores", 40, 120, 2, 0xFFFF, 0);
        LCD_Print("1 jugador", 40, 150, 2, 0xFFFF, 0);
        LCD_Print("j1 vs j2", 40, 180, 2, 0xC992, 0);// cambiar el color del seleccionado
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
    if ((reading1 == HIGH || reading2 == HIGH) && var_flecha == 114) { // se presiono el boton en modo de juego 1
      juego = 1;
      modo = 1;
      menu = 0;
    }
    if ((reading1 == HIGH || reading2 == HIGH) && var_flecha == 144) { // se presiono el boton en modo de juego 2
      juego = 2;
      modo = 2;
      menu = 0;
    }
    if ((reading1 == HIGH || reading2 == HIGH) && var_flecha == 174) { // se presiono el boton en modo de juego 3
      juego = 3;
      modo = 3;
      menu = 0;
    }
    if ((reading1 == HIGH || reading2 == HIGH) && var_flecha == 204) { // se presiono el boton en modo de juego 4
      juego = 4;
      modo = 0;
      menu = 0;
    }
  }
}
void fondos(int fondo) { // dibujar estrellitas 
  if (fondo == 0) {
    // 000 - 160 x | 000 - 120 y
    V_line(10, 1, 0, 0xCFB9);    // verde
    V_line(50, 89, 0, 0xA556);    // gris
    V_line(80, 120, 0, 0xFFF9);    // amarillo
    V_line(101, 60, 0, 0xFFF9);    // amarillo
    V_line(120, 13, 0, 0x216C);    // azul
    V_line(140, 104, 0, 0xFFFF);    // blanco
    V_line(158, 35, 0, 0xFFFF);    // blanco
    V_line(53, 45, 0, 0xFFFF);    // blanco
    V_line(22, 118, 0, 0xFFFF);    // blanco

    V_line(70, 75, 2, 0xF986);    // rojo
    H_line(69, 76, 2, 0xF986);    // rojo

    V_line(15, 15, 2, 0xF76C);    // amarillo
    H_line(14, 16, 2, 0xF76C);    // amarillo
    V_line(121, 87, 2, 0x216C);    // azul
    H_line(120, 88, 2, 0x216C);    // azul

    // 000 - 160 x | 120 - 240 y
    V_line(160, 189, 0, 0xCFB9);    // verde
    V_line(152, 235, 0, 0xA556);    // gris
    V_line(136, 164, 0, 0xF986);    // rojo
    V_line(124, 204, 0, 0xFFF9);    // amarillo
    V_line(111, 198, 0, 0xFFF9);    // amarillo
    V_line(97, 156, 0, 0x216C);    // azul
    V_line(82, 210, 0, 0xFFFF);    // blanco
    V_line(46, 132, 0, 0xFFFF);    // blanco
    V_line(25, 226, 0, 0xFFFF);    // blanco
    V_line(12, 185, 0, 0xFFFF);    // blanco
    // 160 - 320 x | 000 - 120 y
    V_line(162, 106, 0, 0xCFB9);    // verde
    V_line(185, 68, 0, 0xA556);    // gris
    V_line(196, 42, 0, 0xF986);    // rojo
    V_line(201, 108, 0, 0xFFF9);    // amarillo
    V_line(235, 62, 0, 0xFF46);    // amarillo
    V_line(268, 103, 0, 0x216C);    // azul
    V_line(289, 78, 0, 0xFFFF);    // blanco
    V_line(302, 110, 0, 0xFFFF);    // blanco
    V_line(318, 45, 0, 0xFFFF);    // blanco
    V_line(245, 15, 0, 0xFFFF);    // blanco
    // 160 - 320 x | 120 - 240 y
    V_line(160, 125, 0, 0xCFB9);    // verde
    V_line(185, 160, 0, 0xA556);    // gris
    V_line(240, 230, 0, 0xF986);    // rojo
    V_line(332, 120, 0, 0xFFF9);    // amarillo
    V_line(312, 201, 0, 0xF7F7);    // amarillo
    V_line(290, 153, 0, 0x216C);    // azul
    V_line(283, 134, 0, 0xFFFF);    // blanco
    V_line(160, 235, 0, 0xFFFF);    // blanco
    V_line(160, 126, 0, 0xFFFF);    // blanco
    V_line(252, 185, 0, 0xFFFF);    // blanco
  }
  else if (fondo == 1) {
    // 160 - 320 x | 120 - 240 y
    V_line(160, 125, 0, 0xCFB9);    // verde
    V_line(185, 160, 0, 0xA556);    // gris
    V_line(240, 230, 0, 0xF986);    // rojo
    V_line(332, 120, 0, 0xFFF9);    // amarillo
    V_line(312, 201, 0, 0xF7F7);    // amarillo
    V_line(290, 153, 0, 0x216C);    // azul
    V_line(283, 134, 0, 0xFFFF);    // blanco
    V_line(160, 235, 0, 0xFFFF);    // blanco
    V_line(160, 126, 0, 0xFFFF);    // blanco
    V_line(252, 185, 0, 0xFFFF);    // blanco
  }
  else if (fondo == 2) {
    // 000 - 160 x | 000 - 120 y
    V_line(154, 24, 0, 0xFFF9);    // amarillo
    V_line(136, 14, 0, 0xA556);    // gris
    V_line(148, 85, 0, 0xCFB9);    // verde
    // 000 - 160 x | 120 - 240 y
    V_line(43, 234, 0, 0xFFFF);    // blanco
    V_line(54, 201, 0, 0xFFF9);    // amarillo
    V_line(142, 185, 0, 0xFFF9);    // amarillo
    // 160 - 320 x | 000 - 120 y
    V_line(185, 97, 0, 0xFFFF);    // blanco
    V_line(201, 18, 0, 0xFFF9);    // amarillo
    V_line(312, 87, 0, 0xCFB9);    // verde
    // 160 - 320 x | 120 - 240 y
    V_line(196, 145, 0, 0xCFB9);    // verde
    V_line(204, 123, 0, 0xA556);    // gris
    V_line(298, 217, 0, 0xFFF9);    // amarillo
  }
  else if (fondo == 5) { // estrellas grandes
    estrx = 30;
    estry = 97;
    V_line(estrx + 1, estry, 2, 0xFFE0);  // amarillo
    H_line(estrx, estry + 1, 2, 0xFFE0);  // amarillo
    estrx = 212;
    estry = 185;
    V_line(estrx + 1, estry, 2, 0xCFB9);  // verde
    H_line(estrx, estry + 1, 2, 0xCFB9);  // verde
    estrx = 120;
    estry = 160;
    V_line(estrx + 1, estry, 2, 0xA556);  // gris
    H_line(estrx, estry + 1, 2, 0xF986);  // rojo
    estrx = 301;
    estry = 125;
    V_line(estrx + 1, estry, 2, 0x216C);  // azul
    H_line(estrx, estry + 1, 2, 0x216C);  // azul
    estrx = 195;
    estry = 52;
    V_line(estrx + 1, estry, 2, 0xF986);  // rojo
    H_line(estrx, estry + 1, 2, 0xF986);  // rojo
  }
  else if (fondo == 7) {
    // 000 - 160 x | 000 - 120 y
    V_line(10, 1, 0, 0xCFB9);    // verde
    V_line(50, 89, 0, 0xA556);    // gris
    V_line(70, 75, 0, 0xF986);    // rojo
    V_line(80, 120, 0, 0xFFF9);    // amarillo
    // 000 - 160 x | 120 - 240 y
    V_line(160, 189, 0, 0xCFB9);    // verde
    V_line(152, 235, 0, 0xA556);    // gris
    V_line(136, 164, 0, 0xF986);    // rojo
    V_line(124, 204, 0, 0xFFF9);    // amarillo
    // 160 - 320 x | 000 - 120 y
    V_line(162, 106, 0, 0xCFB9);    // verde
    V_line(185, 68, 0, 0xA556);    // gris
    V_line(196, 42, 0, 0xF986);    // rojo
    V_line(201, 108, 0, 0xFFF9);    // amarillo
    // 160 - 320 x | 120 - 240 y
    V_line(160, 125, 0, 0xCFB9);    // verde
    V_line(185, 160, 0, 0xA556);    // gris
    V_line(240, 230, 0, 0xF986);    // rojo
  }
  else if (fondo == 10) {                     // fondo 12
    // 000 - 160 x | 000 - 120 y
    V_line(10, 1, 0, 0);    // verde
    V_line(140, 104, 0, 0);    // blanco
    V_line(22, 118, 0, 0);    // blanco
    // 000 - 160 x | 120 - 240 y
    V_line(160, 189, 0, 0);    // verde
    V_line(97, 156, 0, 0);    // azul
    V_line(12, 185, 0, 0);    // blanco
    // 160 - 320 x | 000 - 120 y
    V_line(196, 42, 0, 0);    // rojo
    V_line(201, 108, 0, 0);    // amarillo
    V_line(245, 15, 0, 0);    // blanco
    // 160 - 320 x | 120 - 240 y
    V_line(160, 125, 0, 0);    // verde
    V_line(185, 160, 0, 0);    // gris
    V_line(160, 126, 0, 0);    // blanco
  }
  else if (fondo == 3) { // estrellas grandes - NEGRO
    estrx = 30;
    estry = 97;
    V_line(estrx + 1, estry, 2, 0);  // amarillo
    H_line(estrx, estry + 1, 2, 0);  // amarillo
    estrx = 212;
    estry = 185;
    V_line(estrx + 1, estry, 2, 0);  // verde
    H_line(estrx, estry + 1, 2, 0);  // verde
    estrx = 120;
    estry = 160;
    V_line(estrx + 1, estry, 2, 0);  // gris
    H_line(estrx, estry + 1, 2, 0);  // rojo
    estrx = 301;
    estry = 125;
    V_line(estrx + 1, estry, 2, 0);  // azul
    H_line(estrx, estry + 1, 2, 0);  // azul
    estrx = 195;
    estry = 52;
    V_line(estrx + 1, estry, 2, 0);  // rojo
    H_line(estrx, estry + 1, 2, 0);  // rojo
  }
  else if (fondo == 12) {
    // 000 - 160 x | 000 - 120 y
    V_line(10, 1, 0, 0xCFB9);    // verde
    V_line(140, 104, 0, 0xFFFF);    // blanco
    V_line(22, 118, 0, 0xFFFF);    // blanco
    // 000 - 160 x | 120 - 240 y
    V_line(160, 189, 0, 0xCFB9);    // verde
    V_line(97, 156, 0, 0x216C);    // azul
    V_line(12, 185, 0, 0xFFFF);    // blanco
    // 160 - 320 x | 000 - 120 y
    V_line(196, 42, 0, 0xF986);    // rojo
    V_line(201, 108, 0, 0xFFF9);    // amarillo
    V_line(245, 15, 0, 0xFFFF);    // blanco
    // 160 - 320 x | 120 - 240 y
    V_line(160, 125, 0, 0xCFB9);    // verde
    V_line(185, 160, 0, 0xA556);    // gris
    V_line(160, 126, 0, 0xFFFF);    // blanco
  }
  else if (fondo == 19) {                     // fondo 1 (esquina inferior derecha)
    // 160 - 320 x | 120 - 240 y
    V_line(160, 125, 0, 0);    // verde
    V_line(185, 160, 0, 0);    // gris
    V_line(240, 230, 0, 0);    // rojo
    V_line(332, 120, 0, 0);    // amarillo
    V_line(312, 201, 0, 0);    // amarillo
    V_line(290, 153, 0, 0);    // azul
    V_line(283, 134, 0, 0);    // blanco
    V_line(160, 235, 0, 0);    // blanco
    V_line(160, 126, 0, 0);    // blanco
    V_line(252, 185, 0, 0);    // blanco
  }
  else if (fondo == 18) {                       //fondo 2
    // 000 - 160 x | 000 - 120 y
    V_line(154, 24, 0, 0);    // verde
    V_line(136, 14, 0, 0);    // blanco
    V_line(148, 85, 0, 0);    // blanco
    // 000 - 160 x | 120 - 240 y
    V_line(43, 234, 0, 0);    // verde
    V_line(54, 201, 0, 0);    // azul
    V_line(142, 185, 0, 0);    // blanco
    // 160 - 320 x | 000 - 120 y
    V_line(185, 97, 0, 0);    // rojo
    V_line(201, 18, 0, 0);    // amarillo
    V_line(312, 87, 0, 0);    // blanco
    // 160 - 320 x | 120 - 240 y
    V_line(196, 145, 0, 0);    // verde
    V_line(204, 123, 0, 0);    // gris
    V_line(298, 217, 0, 0);    // blanco
  }
  
}
void reiniciar(void) {  //reiniciar juego 
  LCD_Clear(0x00);
  digitalWrite(musica[0], LOW);//RB6
  digitalWrite(musica[1], LOW);//RB7
  fondos(0);
//  delay(300000);
  for (int x = 0; x < 315 - 128; x++) {
    //    fondos(0);
    LCD_Bitmap(x, 100, 127, 39, logo);  // dibujar logo 
    fondos(0);  // dibujar el fondo 0
    V_line( x - 1, 100, 30, 0x0000);  // borrar linea del logo al moverse
  }
  Serial.println("highscores");
  puntaje1 = 0; // reiniciar variables para el juego 
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
  delay(800);
  LCD_Clear(0x00);  // borrar pantalla 
  menu = 1; // variable para while del menu 
  fondos(0);  // dibujar fondo 
  menuu();  // ir al menu
  LCD_Clear(0x00);  // borrar el menu 
  if (juego != 4) { // si no esta en highscores 
    LCD_Bitmap(J1, 200, 13, 8, chunche);  // dibujar el jugador 1
    H_line(0, 209, 319,  0xD0A3); // dibujar linea para poner los tanques
    H_line(0, 210, 319,  0xD0A3);
    H_line(0, 211, 319,  0xD0A3);

    if (modo != 2) {  // si no esta en modo de solo 1 jugador
      // dos jugadores divididos en 170
      V_line(160, 195, 209,  0xD0A3); // seprar tanques
      LCD_Print("jugador1 = 3 vidas", 10, 220, 1, 0x3E1C, 0);
      LCD_Print("jugador2 = 3 vidas", 165, 220, 1, 0x3E1C, 0);  // vidas d elos jugadores


      LCD_Bitmap(J2, 200, 13, 8, chunche);
    } else {
      LCD_Print("jugador = 3 vidas", 10, 220, 1, 0x3E1c, 0);  // vida del jugador 
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
