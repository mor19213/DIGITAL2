/*
 * File:   lab3.c
 * Author: danie
 *
 * Created on February 4, 2021, 11:39 AM
 */


#include <xc.h>
#define __XTAL_FREQ 8000000
#include <stdint.h>


#include "../lab3.X/adc.h"
#include "../lab3.X/osc.h"
#include "../lab3.X/USART.h"
#include "../lab3.X/lcd.h"



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
  unsigned int a;
  uint8_t ADC1;
  uint8_t ADC2;
  uint8_t bandera;
  uint8_t RC_temp;
  uint8_t var_contador;
  uint16_t contador;
  uint8_t cont1;
  uint8_t cont2;
  
//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************
void Setup (void);


void __interrupt() isr(void){
    if(ADIF == 1){
        if (bandera == 1){
            bandera = 0;
            ADC1 = ADRESH;
            ADCON0bits.CHS0 = 1;
            RA3 = 1;
        } else{
            bandera = 1;
            ADC2 = ADRESH;
            ADCON0bits.CHS0 = 0;
            RA3 = 0;
        }
        ADIF = 0;
        ADCON0bits.GO = 1;
    } 
    if(PIR1bits.RCIF == 1){
        if (RCREG ==  13){
            if (RC_temp == 43){
                var_contador = var_contador + 1;
            } else if (RC_temp == 45){
                var_contador = var_contador - 1;
        PORTB = var_contador; 

            }
        } else {
        RC_temp = RCREG;
        }
    }
    if(PIR1bits.TXIF == 1){
    }
}

void main(void) {
    Setup();
    while(1){
        PORTB = var_contador; 
        contador = var_contador * 100 / 51;
        cont1 = contador / 100;
        cont2 = contador - (cont1 * 100);
        RA6 = 1;
  }
}

void Setup(void){
    initOsc(7);
    TRISA = 0;
    TRISD = 0x00;
    TRISE = 0;
    ADCON0 = 0b10000011; 
    ADCON1 = 0;
    PORTA = 0;
    ADCON0 = 0b10000011; 
    ADCON1 = 0;
    PORTA = 0;
    Lcd_Init();
    TRISB = 0;
    PORTB = 0;
    var_contador = 0;
    
    initUSART();
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    initADC(0);
    initADC(1);
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 0;
    INTCONbits.GIE = 1;
}
