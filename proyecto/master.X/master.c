/*
 * File:   master.c
 * Author: danie
 *
 * Created on 11 de febrero de 2021, 10:10 AM
 */
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//*****************************************************************************
// LIBRERIAS
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "USART.h"
#include "lcd.h"
//*****************************************************************************
// VARIABLES
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t ADC;
uint8_t contador;
uint8_t temperatura;
uint8_t s11;
uint8_t s12;
uint8_t s13;
uint8_t s21;
uint8_t s22;
uint8_t s23;
uint8_t s31;
uint8_t s32;
uint8_t s33;
uint8_t CONTX;
uint8_t bandera;
uint8_t var1;
uint8_t var2;
uint16_t temperatura_;
uint8_t banderaS3;
uint8_t banderaspi;
uint8_t band;
uint8_t val;

//*****************************************************************************
// PROTOTIPO DE FUNCIOENS
//*****************************************************************************
void setup(void);


void __interrupt() isr(void){
    if (INTCONbits.T0IF){           // INTERRUPCION TMR0
        CONTX++;
        INTCONbits.T0IF = 0;        // TERMINAR INTERRUPCION DE TMR0
    }
  
    if (TXIF == 1){       
        // valores slave 1
        if (bandera == 0){
        TXREG = s11;
        bandera = 1;           
        } 
        // punto 2E
        else if (bandera == 1){
        TXREG = 0x2E;
        bandera = 2;           
        }
        else if (bandera == 2){
        TXREG = s12;
        bandera = 3;           
        }
        else if (bandera == 3){
        TXREG = s13;
        bandera = 4;           
        }
        // espacio
        else if (bandera == 4){
        TXREG = 0x20;
        bandera = 5;           
        }
        // valores slave 2
        else if (bandera == 5){
        TXREG = s21;
        bandera = 6;           
        }
        else if (bandera == 6){
        TXREG = s22;
        bandera = 7;           
        }
        else if (bandera == 7){
        TXREG = s23;
        bandera = 8;           
        }
        // espacio
        else if (bandera == 8){
        TXREG = 0x20;
        bandera = 9;           
        }
        // signo
        else if (bandera == 9){
        TXREG = val;
        bandera = 10;           
        }
        // valores slave 3
        else if (bandera == 10){
        TXREG = s31;
        bandera = 11;           
        }
        else if (bandera == 11){
        TXREG = s32;
        bandera = 12;           
        }
        else if (bandera == 12){
        TXREG = s33;
        bandera = 13;           
        }
        // enter
        else if (bandera == 13){
        TXREG = 0x0D;
        bandera = 0;           
        }
        
        TXIE = 0;   // deshabilitar interrupcion de tx
    }
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    temperatura = 0;
    while(1){  
        if(CONTX > 15){     // habilitar interrupcion de tx 
            CONTX = 0;
            TXIE = 1;
            }
//*****************************************************************************
       PORTCbits.RC2 = 0;       //Slave 1 Select adc
       __delay_ms(1);
       
       spiWrite(0);
       ADC = spiRead();         // guardar valor adc de slave1
       //ADC = 243;
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave 1 Deselect 
       
       
               
  //conversiones valores adc  a 0.00 a 5.00V
  // convertir por decenas
        s11 = (ADC / 51);
        s12 = (ADC * 10 / 51) - (s11 * 10);
        s13 = (ADC * 100 / 51) - (s11 * 100) - (s12 * 10);  
        
  // sumar 48 para convertir a ascii
        s11 = s11 + 48;
        s12 = s12 + 48;
        s13 = s13 + 48;
        
  // limites de valores ascii
            if (s12 > 57){
            s12 = 57;
        }
        if (s13 > 57){
            s13 = 57;
        }
        
       // mostrar valor en LCD
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(s11);
        Lcd_Set_Cursor(2,3);
        Lcd_Write_Char(s12);
        Lcd_Set_Cursor(2,4);
        Lcd_Write_Char(s13);
        
       
//*****************************************************************************
       PORTCbits.RC1 = 0;       //Slave 2 Select contador
       __delay_ms(1);
       
       spiWrite(0);
       contador = spiRead();   // guardar valor contador slave 2
       //contador = 156;
       
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave 2 Deselect 
       
       s21 = (contador/100) + 48;
       s22 = (contador/10) - ((s21 - 48) * 10) + 48;
       s23 = (contador) - ((s21 - 48) * 100) - ((s22 - 48) * 10) + 48;
       // mostrar valor en LCD
        Lcd_Set_Cursor(2,7);
        Lcd_Write_Char(s21);
        Lcd_Set_Cursor(2,8);
        Lcd_Write_Char(s22);
        Lcd_Set_Cursor(2,9);
        Lcd_Write_Char(s23);
       
//*****************************************************************************
       PORTCbits.RC0 = 0;       //Slave 3 Select temperatura
       __delay_ms(1);
       
       if (banderaspi == 0){    // alternar que valor se recibe 
       spiWrite(0);             // escribir valor para bandera 
       var2 = spiRead();        // guardar valor temperatura negativa
       banderaspi = 1;
       } 
       else {
       spiWrite(1);             // escribir valor bandera slave 3
       var1 = spiRead();        // guardar valor temperatura positiva
       banderaspi = 0;
       }
       __delay_ms(1);
       PORTCbits.RC0 = 1;       //Slave 3 Deselect
       
       if (var2 == 0){          // si la temperatura no es negativa
           if (var1 > 29){      // hacer conversion temp positva
               // debido a aproximaciones la conversion es diferente en algunos rangos
               temperatura = (var1 * 50) / 51;
               if (var1 > 51){
                   if (var1 < 71){
                   temperatura = temperatura + 1;
                   }
               }
               if (var1 > 102){
                   if (var1 < 109){
                   temperatura = temperatura + 1;
                   }
               }
               if (var1 > 151){
                   if (var1 < 155){
                       temperatura = temperatura - 1;
                   }
               }
           } else{
               temperatura = var1;
           }
           // guardar ascii + en variable (usar para tx despues)
           val = 0x2B;
            Lcd_Set_Cursor(2,13);
            Lcd_Write_Char(val);
       } else {
           // si hay temperatura negativa, hacer conversion
           if (var2 > 38){
               temperatura = (var2 * 50) / 51;
               if (var2 > 51){
                   temperatura = temperatura + 1;
               }
           } else{
               temperatura = var2;
           }
           // guardar ascii - en variable
           val = 0x2D;
            Lcd_Set_Cursor(2,13);
            Lcd_Write_Char(val);
       }
       
       // conversion por decenas del valor de temperatura
        s31 = (temperatura/100) + 48;
        s32 = (temperatura / 10) - ((s31 - 48) * 10) + 48;
        s33 = temperatura - ((s32 - 48) * 10) - ((s31 - 48) * 100) + 48;
            
       
       // mostrar valor en lcd
        Lcd_Set_Cursor(2,14);
        Lcd_Write_Char(s31);
        Lcd_Set_Cursor(2,15);
        Lcd_Write_Char(s32);
        Lcd_Set_Cursor(2,16);
        Lcd_Write_Char(s33);
       
//*****************************************************************************
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    PORTA = 0;
    
    //SLAVES
    TRISC2 = 0;
    TRISC1 = 0;
    TRISC0 = 0;
    
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    
    // INICIAR SLAVES 
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC0 = 1;
    temperatura = 0;
    
    // CONFIGURACION SPI
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
        
    // iniciar usart
    initUSART();
    
    // CONFIGURACION TIMER0
    OPTION_REGbits.T0CS = 0;        // TMR0 Clock source
    OPTION_REGbits.PSA = 0;         // Prescaler a tmr0
    OPTION_REGbits.PS = 111;        // prescaler 1:256
    TMR0 = 10;
    
    // interrupcion tmr0
    INTCONbits.GIE = 1;             // Global interrupt enable    
    INTCONbits.T0IE = 1;            // tmr0 interrupt enable
    INTCONbits.T0IF = 0;            // bandera tmr0 
    
    // iniciar lcd
    Lcd_Init();
    
    // formato lcd
    Lcd_Set_Cursor(1,3);
    Lcd_Write_String("S1");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("0.00V");
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String("S2");
    Lcd_Set_Cursor(2,7);
    Lcd_Write_String("000");
    Lcd_Set_Cursor(1,14);
    Lcd_Write_String("S3");
    Lcd_Set_Cursor(2,14);
    Lcd_Write_String("000");
    
    s11 = 0;
    s12 = 0;
    s13 = 0;
    
    s21 = 0;
    s22 = 0;
    s23 = 0;
    
    s31 = 0;
    s32 = 0;
    s33 = 0;
    

}