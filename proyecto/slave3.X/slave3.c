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
uint8_t ADC1;
uint8_t ADC2;
uint8_t TEMP;
uint8_t bandera;
uint8_t bandera1;


//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************
void Setup (void);


//******************************************************************************
//  INTERRUPCION
//******************************************************************************
void __interrupt() isr(void){
    if(SSPIF == 1){
        if (bandera1 == 0){
            spiWrite(ADC1);
            bandera1 = 1;
        } else if(bandera1 == 1){
            spiWrite(ADC2);
            bandera1 = 0;
        }
        SSPIF = 0;              // Apagar bandera interrupcion
    }
    
    if(ADIF == 1){          // Vout = 10mV/C * T
        if (bandera == 0){
        ADC1 = ADRESH;
        bandera = 1;
        ADCON0bits.CHS0 = 1;
        } else {
        ADC2 = ADRESH;
        ADCON0bits.CHS0 = 0;
        bandera = 0; 
        }
    ADIF = 0;
    ADCON0bits.GO = 1; 
    }
}

//******************************************************************************
void main(void) {
    Setup();
    while(1){
        
        // RANGO SEMAFORO
        if (ADC1 > 18.36){
            PORTE = 0;
            RE2 = 1;        // ROJO
            
        } else if (ADC1 < 12.75){
            PORTE = 0;
            RE0 = 1;        // VERDE
            
        } else {
            PORTE = 0;
            RE1 = 1;        // AMARILLO
        }  
            
        
    }
}

void Setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;              // VALOR ADC (PRUEBA)
    TRISC = 0;              // COMUNICACION SERIAL
    TRISE = 0;              // SEMAFORO
    PORTE = 0;
    TRISD = 0;
    PORTD = 0;
    ADC1 = 0;
    ADC2 = 0; 
    TEMP = 0;
    bandera = 0;
    
    INTCONbits.GIE = 1;     // habilitar interrupciones globales
    INTCONbits.PEIE = 1;    // interrupciones perifericas
    PIR1bits.SSPIF = 0;     // bandera int. (se ha recibido/enviado datos)
    PIE1bits.SSPIE = 1;     // interrupcion mssp habilitada
    TRISAbits.TRISA5 = 1;   // recibir ss
    
    
    initADC(0);             // configuracion ADC canal RA0
    initADC(1);             // configuracion ADC canal RA1
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}