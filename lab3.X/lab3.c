/*
 * File:   lab3.c
 * Author: danie
 *
 * Created on February 4, 2021, 11:39 AM
 */


#include <xc.h>
#define __XTAL_FREQ 4000000
#include <stdint.h>


#include "USART.h"
#include "adc.h"
#include "lcd.h"
#include "osc.h"

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
  uint8_t ughh;
  uint8_t CONTX;
  uint8_t banderaTX;
  uint8_t C11;
  uint8_t C12;
  uint8_t C13;
  uint8_t C21;
  uint8_t C22;
  uint8_t C23;
  uint8_t C31;
  uint8_t C32;
  uint8_t C33;
  uint8_t bandera1;
  
  
//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************
void Setup (void);


void __interrupt() isr(void){
    di();
    if (INTCONbits.T0IF){           // INTERRUPCION TMR0
        CONTX++;
        INTCONbits.T0IF = 0;        // TERMINAR INTERRUPCION DE TMR0
    }
    
    if(ADIF == 1){
        if (bandera == 1){
            ADC1 = ADRESH;
            ADCON0bits.CHS0 = 1;
            bandera = 0;
        } else{
            ADC2 = ADRESH;
            ADCON0bits.CHS0 = 0;
            bandera = 1;
        }
        ADIF = 0;
        ADCON0bits.GO = 1;
    } 
    if(PIR1bits.RCIF == 1){
        //ughh = RCREG;
        RA7 = 1;
        if (RCREG ==  0x0D){
        RA7 = 0;
            if (RC_temp == 0x2B){
                contador++;
                if (contador > 255){
                    contador = 0;
                }
            } else if (RC_temp == 0x2D){
                contador--;
                
                if (contador > 255){
                    contador = 255;
                }

            }
        } else {
        RC_temp = RCREG;
        }
    }
    
    if (TXIF == 1){
        if (bandera1 == 0){
            TXREG = C11 + 48;
            bandera1 = 1;
        } else if (bandera1 == 1){
            TXREG = C12 + 48;
            bandera1 = 2;
        } else if (bandera1 == 2){
            TXREG = C13 + 48;
            bandera1 = 3;
        } else if (bandera1 == 3){
            TXREG = 0x2D;
            bandera1 = 4;
        }
        else if (bandera1 == 4){
            TXREG = C21 + 48;
            bandera1 = 5;
        } else if (bandera1 == 5){
            TXREG = C22 + 48;
            bandera1 = 6;
        } else if (bandera1 == 6){
            TXREG = C23 + 48;
            bandera1 = 7;
        } else if (bandera1 == 7){
            TXREG = 0x0D;
            bandera1 = 0;
        }
        
    TXIE = 0; 
    }   
    ei();
}

void main(void) {
    Setup();
    while(1){  
        if(CONTX > 15){
            CONTX = 0;
            TXIE = 1;
            }
        
  
    C11 = ADC1 / 51;
    C12 = ((ADC1 * 100 / 51) - (C11*100))/10;
    C13 = ((ADC1 * 100 / 51) - (C11*100) - (C12*10));  
    
    C21 = ADC2 / 51;
    C22 = (((ADC2 * 100) / 51) - (C21*100))/10;
    C23 = (((ADC2 * 100) / 51) - (C21*100) - (C22*10));
      
    C31 = contador / 51;
    C32 = ((contador * 100 / 51) - (C31*100))/10;
    C33 = ((contador * 100 / 51) - (C31*100) - (C32*10));
    
    if (C12 > 9){
        C12 = 9;
    }
    if (C13 > 9){
        C13 = 9;
    }
    if (C22 > 9){
        C22 = 9;
    }
    if (C23 > 9){
        C23 = 9;
    }
    
    Lcd_Set_Cursor(2,1);
    Lcd_Write_Char(C11 +48);
    Lcd_Set_Cursor(2,3);
    Lcd_Write_Char(C12 + 48);
    Lcd_Set_Cursor(2,4);
    Lcd_Write_Char(C13 + 48);
    
    Lcd_Set_Cursor(2,7);
    Lcd_Write_Char(C21 +48);
    Lcd_Set_Cursor(2,9);
    Lcd_Write_Char(C22 + 48);
    Lcd_Set_Cursor(2,10);
    Lcd_Write_Char(C23 + 48);
       
    Lcd_Set_Cursor(2,13);
    Lcd_Write_Char(C31 +48);
    Lcd_Set_Cursor(2,15);
    Lcd_Write_Char(C32 + 48);
    Lcd_Set_Cursor(2,16);
    Lcd_Write_Char(C33 + 48);
    
  }
}

void Setup(void){
    
    TRISA = 0;
    TRISD = 0;
    PORTD = 0;
    TRISE = 0;
    TRISB = 0;

    ADCON0 = 0b10000011; 
    ADCON1 = 0;
    PORTA = 0;
    ADCON0 = 0b10000011; 
    ADCON1 = 0;
    PORTA = 0;
    PORTB = 0;
    ANSEL = 0;
    Lcd_Init();
    var_contador = 0;
    PIE1bits.ADIE = 1;
    initADC(0);
    initADC(1);
       
    //******************************************************************************
// CONFIGURACION TIMER0
    OPTION_REGbits.T0CS = 0;        // TMR0 Clock source
    OPTION_REGbits.PSA = 0;         // Prescaler a tmr0
    OPTION_REGbits.PS = 111;        // prescaler 1:256
    TMR0 = 10;
    
    // interrupcion tmr0
    INTCONbits.GIE = 1;             // Global interrupt enable    
    INTCONbits.T0IE = 1;            // tmr0 interrupt enable
    INTCONbits.T0IF = 0;            // bandera tmr0 
    
    initUSART();
    
    
    Lcd_Set_Cursor(1,3);
    Lcd_Write_String("S1");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("0.00");
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String("S2");
    Lcd_Set_Cursor(2,7);
    Lcd_Write_String("0.00");
    Lcd_Set_Cursor(1,14);
    Lcd_Write_String("S3");
    Lcd_Set_Cursor(2,13);
    Lcd_Write_String("0.00");
    
}
