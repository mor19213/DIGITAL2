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
uint8_t CONTX;
uint8_t bandera;

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
        if (bandera == 0){
        TXREG = s11;
        bandera = 1;           
        }
        else if (bandera == 1){
        TXREG = s12;
        bandera = 2;           
        }
        else if (bandera == 2){
        TXREG = s13;
        bandera = 3;           
        }
        else if (bandera == 3){
        TXREG = 0x2D;
        bandera = 4;           
        }
        else if (bandera == 4){
        TXREG = s21;
        bandera = 5;           
        }
        else if (bandera == 5){
        TXREG = s22;
        bandera = 6;           
        }
        else if (bandera == 6){
        TXREG = s23;
        bandera = 7;           
        }
        else if (bandera == 7){
        TXREG = 0x2D;
        bandera = 8;           
        }
        else if (bandera == 8){
        TXREG = s31;
        bandera = 9;           
        }
        else if (bandera == 9){
        TXREG = s32;
        bandera = 10;           
        }
        else if (bandera == 10){
        TXREG = 0x0D;
        bandera = 0;           
        }
        
        TXIE = 0; 
    }
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    while(1){  
        if(CONTX > 15){
            CONTX = 0;
            TXIE = 1;
            }
//*****************************************************************************
       PORTCbits.RC2 = 0;       //Slave 1 Select adc
       __delay_ms(1);
       
       spiWrite(0);
       ADC = spiRead();
       //ADC = 243;
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave 1 Deselect 
       
       
       s11 = (ADC/100) + 48;
       s12 = (ADC/10) - ((s11 - 48) * 10) + 48;
       s13 = (ADC) - ((s11 - 48) * 100) - ((s12 - 48) * 10) + 48;
       // mostrar valor en LCD
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(s11);
        Lcd_Set_Cursor(2,2);
        Lcd_Write_Char(s12);
        Lcd_Set_Cursor(2,3);
        Lcd_Write_Char(s13);
        
       
//*****************************************************************************
       PORTCbits.RC1 = 0;       //Slave 2 Select contador
       __delay_ms(1);
       
       spiWrite(0);
       contador = spiRead();
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
       
       spiWrite(0);
       temperatura = spiRead();
       //temperatura = 98;
       
       __delay_ms(1);
       PORTCbits.RC0 = 1;       //Slave 3 Deselect 
       
       
       s31 = (temperatura/10) + 48;
       s32 = (temperatura) - ((s31 - 48) * 10) + 48;
       // mostrar valor en lcd
        Lcd_Set_Cursor(2,13);
        Lcd_Write_Char(s31);
        Lcd_Set_Cursor(2,14);
        Lcd_Write_Char(s32);
       
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
    Lcd_Write_String("000");
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String("S2");
    Lcd_Set_Cursor(2,7);
    Lcd_Write_String("000");
    Lcd_Set_Cursor(1,14);
    Lcd_Write_String("S3");
    Lcd_Set_Cursor(2,13);
    Lcd_Write_String("00");

}