/*
 * File:   master.c
 * Author: danie
 *
 * Created on 11 de febrero de 2021, 10:10 AM
 */



#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "ADC.h"

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
uint8_t ADC;
uint8_t TEMP;


//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************
void Setup (void);


//******************************************************************************
//  INTERRUPCION
//******************************************************************************
void __interrupt() isr(void){
    if(SSPIF == 1){
        //TEMP = spiRead();
        spiWrite(PORTD);
        SSPIF = 0;              // Apagar bandera interrupcion
    }
    
    if(ADIF == 1){          // Vout = 10mV/C * T
    ADC = ADRESH;
    ADIF = 0;
    ADCON0bits.GO = 1; 
    }
}

//******************************************************************************
void main(void) {
    Setup();
    while(1){
        TEMP = ADC * 10;
        PORTB = (ADC * 100) / 51;
        // conversion temperatura
        
        // RANGO SEMAFORO
        if (TEMP > 184){
            PORTD = 0;
            RD2 = 1;        // ROJO
            
        } else if (TEMP < 128){
            PORTD = 0;
            RD0 = 1;        // VERDE
            
        } else {
            PORTD = 0;
            RD1 = 1;        // AMARILLO
        }  
            
        
    }
}

void Setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;              // VALOR ADC (PRUEBA)
    TRISC = 0;              // COMUNICACION SERIAL
    TRISD = 0;              // SEMAFORO
    ADC = 0; 
    TEMP = 0;
    
    INTCONbits.GIE = 1;     // habilitar interrupciones globales
    INTCONbits.PEIE = 1;    // interrupciones perifericas
    PIR1bits.SSPIF = 0;     // bandera int. (se ha recibido/enviado datos)
    PIE1bits.SSPIE = 1;     // interrupcion mssp habilitada
    TRISAbits.TRISA5 = 1;   // recibir ss
    
    
    initADC(0);             // configuracion ADC canal RA0
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}