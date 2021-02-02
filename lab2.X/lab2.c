/*
 * File:   laboratorio 2.c
 * Author: daniela morales
 *
 * Created on January 25, 2021, 2:25 PM
 */


#include <xc.h>
#include <stdint.h>
#include "oscilador.h"
#include "displays.h"

//******************************************************************************
//  PALABRA DE CONFIGURACION
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

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


//******************************************************************************
//  VARIABLES
//******************************************************************************
uint8_t antirebote1;
uint8_t antirebote2;
uint8_t valorADC;
uint8_t bandera;
uint8_t bandera1;
uint8_t display1;
uint8_t display2;
uint8_t var;
#define _XTAL_FREQ 4000000
#define ALARMA PORTEbits.RE0
#define b1 PORTBbits.RB1
#define b2 PORTBbits.RB2

//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************
void Setup (void);


void __interrupt() isr(void){
    
    if (INTCONbits.T0IF){           // INTERRUPCION TMR0
        if (PORTBbits.RB7 == 0){    // MULTIPLEXACION
            PORTBbits.RB6 = 0;      // SI RB6 ESTA EN 1, INVERTIR
            PORTBbits.RB7 = 1;
            initDisplays(display1);
        }   else{                   // SI RB7 ESTA EN 1, INVERTIR
            PORTBbits.RB6 = 1;
            PORTBbits.RB7 = 0;
            bandera1 = 1;
            initDisplays(display2);
        }
        INTCONbits.T0IF = 0;        // TERMINAR INTERRUPCION DE TMR0
    }
    
    if(ADIF == 1){                  // INTERRUPCION ADC
        valorADC = ADRESH;
        if (bandera == 1){
            bandera = 0;
            display1 = valorADC * 16;
            display1 = display1 / 16;
            PORTBbits.RB3 = 1;
        }
        else{
            bandera = 1;
            display2 = valorADC / 16;
            PORTBbits.RB3 = 0;
        }
        ADIF = 0;
        ADCON0bits.GO = 1;
    }
    
    if(INTCONbits.RBIF == 1){       // INTERRUPCION CAMBIO EN PORTB
        if (b1 == 1){               // SI SE PRECIONA EL BOTON1 INICIAR ANTIREBOTE
        antirebote1 = 1;
        }
        if (antirebote1 == 1){      // si antirebote1 = 1 y el boton se suelta
            if (b1 == 0){           // decrementar el valor de portc
                PORTC = PORTC - 1;
                antirebote1 = 0;
            }
        }
        if (b2 == 1){               // INICIAR ANTIREBOTE AL PRECIONAR BOTON2
            antirebote2 = 1;
        }
        if (antirebote2 == 1){      // SI ANTIREBOTE2 = 1 Y SE SUELTA EL BOTON
            if (b2 == 0){           // INCREMENTAR EL VALOR DE PORTC
                PORTC = PORTC + 1;
                antirebote2 = 0;
            }
        }
        
        INTCONbits.RBIF = 0;    // LIMPIAR BANDERA 
    }
}


void main(void) {
    Setup();
    //LOOP PRINCIPAL
    while(1){
    //var = var + 1;
    //__delay_ms(1000);
    //display1 = 2;
    //display2 = 13;
        if(valorADC < PORTC){   // COMPARAR VALOR ADC Y DEL PORTC 
            ALARMA = 1;         // PRENDER ALARMA SI PORTC ES MAYOR
        }
        else{                   // APAGAR ALARMA SI ADC ES MAYOR
            ALARMA = 0;
        }
    }
}


//******************************************************************************
//  SUBRUTINAS

void Setup(void){
    initOsc(7);             // CONFIGURAR OSCILADOR
    PORTB = 0;              // PORTB EN 0
    TRISE = 0;
    TRISB = 0;
    TRISD = 0;
    PORTE = 0;
    TRISC = 0;              // PORTC OUTPUT
    PORTC = 0;
    ANSEL = 0;              // APAGAR ANSEL Y ANSELH
    ANSELH = 0;
    TRISB = 0;              // PORTB OUTPUT
    //ei();                 // enable interrupt
    antirebote1 = 0;        // INICIAR VARIABLES EN 0
    antirebote2 = 0;
    display1 = 0;
    display2 = 0;
    bandera = 0;
    bandera1 = 0;
    var = 0;
    
//******************************************************************************    
//  CONFIGURACION BOTONES
    TRISBbits.TRISB1 = 1;           //boton1
    TRISBbits.TRISB2 = 1;           //boton2
    //WPUBbits.WPUB0 = 1;           // pull up por software en rb0
    //OPTION_REGbits.nRBPU = 0;     // habilitar pull-ups globales 
    IOCBbits.IOCB1 = 1;             // habilitar interrupcion de rb1 
    IOCBbits.IOCB2 = 1;             // habilitar interrupcion de rb2
    INTCONbits.RBIE = 1;            // habilitar isr del portb
    INTCONbits.RBIF = 0;            // limpiar la bandera isr 
    INTCONbits.GIE = 1;             // habilitar isr 
    INTCONbits.PEIE = 1;

    

//******************************************************************************
//CONFIGURACION ADC
    TRISAbits.TRISA0 = 1;           // entrada analogica
    ADCON1 = 0;
    ADCON0 = 0b10000011;
    PIE1bits.ADIE = 1;
    ANSELbits.ANS0 = 1;
    
    
//******************************************************************************
// CONFIGURACION TIMER0
    //asm{CLRWDT};
    OPTION_REGbits.T0CS = 0;        // TMR0 Clock source
    OPTION_REGbits.PSA = 0;         // Prescaler a tmr0
    OPTION_REGbits.PS = 111;        // prescaler 1:256
    TMR0 = 0;
    INTCONbits.T0IF = 0;
    
    // interrupcion tmr0
    INTCONbits.GIE = 1;             // Global interrupt enable    
    INTCONbits.T0IE = 1;            // tmr0 interrupt enable
    INTCONbits.T0IF = 0;            // bandera tmr0 
    
}
