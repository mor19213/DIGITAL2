/*
 * File:   main_pic.c
 * Author: danie
 *
 * Created on 25 de febrero de 2021, 09:23 AM
 */
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "USART.h"
#include <xc.h>
#include <proc/pic16f887.h>

//*****************************************************************************
// variables
//*****************************************************************************
#define _XTAL_FREQ 4000000
uint8_t eje_x;
uint8_t eje_y;
uint8_t eje_z;
uint8_t xH;
uint8_t xL;
uint8_t yH;
uint8_t yL;
uint8_t zH;
uint8_t zL;
uint8_t bandera;
uint8_t CONTX;
uint8_t prueba; 
uint8_t tx_var; 

//*****************************************************************************
// funciones
//*****************************************************************************
void setup(void);


void __interrupt() isr(void){
    if (INTCONbits.T0IF){           // INTERRUPCION TMR0
        CONTX++;
        prueba++; 
        INTCONbits.T0IF = 0;        // TERMINAR INTERRUPCION DE TMR0
    }
  
    if (TXIF == 1){  
        if (tx_var == 1){
            tx_var = 0;
            if (bandera == 0){
                TXREG = eje_x;
                //bandera++;
            } else if (bandera == 1) {
                TXREG = eje_y;
                bandera++;
            } else if (bandera == 2){
                TXREG = eje_z;
                bandera++; 
            } else if (bandera == 3){
                TXREG = 10;
            }
        }
        
        TXIE = 0;   // deshabilitar interrupcion de tx
    }
    
    
    if(PIR1bits.RCIF == 1){
        PORTE = RCREG;
        if (RCREG > 30){
            tx_var = 1;
        }
    }
}
//*****************************************************************************
// loop principal
//*****************************************************************************
void main(void) {
    setup();
    while(1){
        if (CONTX > 15){
            TXIE = 1;
        }
        xL = ADXL345_READ(DATAX0);
        xH = ADXL345_READ(DATAX1);
        yL = ADXL345_READ(DATAY0);
        yH = ADXL345_READ(DATAY1);
        zL = ADXL345_READ(DATAZ0);
        zH = ADXL345_READ(DATAZ1);
        eje_x = (xH * 32) + (xL / 4);
        eje_y = (yH * 32) + (yL / 4);
        eje_z = (zH * 32) + (zL / 4);
        
        PORTB = eje_x;
        __delay_ms(200);   
        
    }
    return;
}
//*****************************************************************************
// Setup
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;

//*****************************************************************************
// Inicializacion de puertos
    TRISA = 0;
    PORTA = 0;
    TRISB = 0;
    PORTB = 0;
    TRISD = 0;          //  verificar recepcion sensor
    PORTD = 0;
    TRISE = 0;          // Luces piloto
    PORTE = 0;
    prueba = 126;
    tx_var = 0;
    
    eje_x = 0xf8;
    initUSART();                    // Comunicacion USART
    // CONFIGURACION TIMER0
    OPTION_REGbits.T0CS = 0;        // TMR0 Clock source
    OPTION_REGbits.PSA = 0;         // Prescaler a tmr0
    OPTION_REGbits.PS = 111;        // prescaler 1:256
    TMR0 = 10;
    
    // interrupcion tmr0
    INTCONbits.GIE = 1;             // Global interrupt enable    
    INTCONbits.T0IE = 1;            // tmr0 interrupt enable
    INTCONbits.T0IF = 0;            // bandera tmr0 
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    ADXL345_init();
}