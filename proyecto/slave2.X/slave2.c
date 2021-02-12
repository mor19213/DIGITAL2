/*
 * File:   master.c
 * Author: danie
 *
 * Created on 11 de febrero de 2021, 10:10 AM
 */



#include <xc.h>
#include <stdint.h>
#include "SPI.h"

//******************************************************************************
//  PALABRA DE CONFIGURACION
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

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

#define _XTAL_FREQ 4000000
uint8_t antirebote1;
uint8_t antirebote2;


//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************
void Setup (void);


//******************************************************************************
//  INTERRUPCION
//******************************************************************************
void __interrupt() isr(void){
    if(SSPIF == 1){
        // = spiRead();
        spiWrite(PORTD);
        SSPIF = 0;              // Apagar bandera interrupcion
    }

}

//******************************************************************************
void main(void) {
    Setup();
    while(1){
       if (RB0 == 1){
           antirebote1 = 1;
       }     
       if (RB0 == 0){
           if (antirebote1 == 1){
               antirebote1 = 0;
               PORTD++;
           }
       }
       if (RB1 == 1){
           antirebote2 = 1;
       }     
       if (RB1 == 0){
           if (antirebote2 == 1){
               antirebote2 = 0;
               PORTD--;
           }
       }
        
    }
}

void Setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;      // BOTONES CONTADOR
    TRISC = 0;      // COMUNICACION SERIAL
    TRISD = 0;      // OUTPUT CONTADOR
    PORTD = 0;      // CONTADOR 8 BITS
    PORTB = 0;
    
    INTCONbits.GIE = 1;     // habilitar interrupciones globales
    INTCONbits.PEIE = 1;    // interrupciones perifericas
    PIR1bits.SSPIF = 0;     // bandera int. (se ha recibido/enviado datos)
    PIE1bits.SSPIE = 1;     // interrupcion mssp habilitada
    TRISAbits.TRISA5 = 1;   // recibir ss
    
    
    initSPI(SPI_SLAVE_SS_EN, MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}