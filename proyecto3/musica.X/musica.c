/*
 * File:   laboratorio 2.c
 * Author: daniela morales
 *
 * Created on January 25, 2021, 2:25 PM
 */


#include <xc.h>
#include <stdint.h>
#include "Osc.h"
#include <pic16f887.h>
//******************************************************************************
//  PALABRA DE CONFIGURACION
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

//******************************************************************************
//  VARIABLES
//******************************************************************************

#define _XTAL_FREQ 8000000
#define nota1 PORTCbits.RC4
#define nota2 PORTCbits.RC5
#define nota3 PORTCbits.RC6
#define nota4 PORTCbits.RC7
#define nota5 PORTDbits.RD4
#define nota6 PORTDbits.RD5
#define nota7 PORTDbits.RD6
#define nota8 PORTDbits.RD7
//uint32_t C4;
//uint32_t D4;
//uint32_t E4;
//uint32_t F4;
//uint32_t G4;
//uint32_t A4;
//uint32_t B4;
//uint32_t C5;
//C4 = 100000/262; = 191
//D4 = 100000/294; = 170
//E4 = 100000/330; = 152
//F4 = 100000/349; = 143
//G4 = 100000/392; = 128
//A4 = 100000/440; = 114
//B4 = 100000/466; = 108
//C5 = 100000/494; = 101

long C4 = 382;
long D4 = 340;
long E4 = 303;
long F4 = 287;
long G4 = 255;
long A4 = 227;
long B4 = 215;
long C5 = 202;


//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************
void Setup (void);

void main(void) {
    Setup();
    //LOOP PRINCIPAL
    while(RB7 == 1 && RB6 == 1){ //  disparar
                nota1 = 1;
                __delay_us(400);
                nota1 = 0;
                __delay_us(400);
        
    }
    if (RB7 == 0 && RB6 == 0){
        nota1 = 0;
    }

// CANCION MENU
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(955);
                nota1 = 0;
                __delay_us(955); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(350);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(1275);
                nota1 = 0;
                __delay_us(1275); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(350);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(1515);
                nota1 = 0;
                __delay_us(1515); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(350);
        }
    
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(1135);
                nota1 = 0;
                __delay_us(1135); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(1015);
                nota1 = 0;
                __delay_us(1015); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(1075);
                nota1 = 0;
                __delay_us(1075); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(1135);
                nota1 = 0;
                __delay_us(1135); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    // tercer compas
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 67; x++){
                nota1 = 1;
                __delay_us(1275);
                nota1 = 0;
                __delay_us(1275); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 67; x++){
                nota1 = 1;
                __delay_us(758);
                nota1 = 0;
                __delay_us(758); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 67; x++){
                nota1 = 1;
                __delay_us(638);
                nota1 = 0;
                __delay_us(638); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(568);
                nota1 = 0;
                __delay_us(568); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(716);
                nota1 = 0;
                __delay_us(716); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(638);
                nota1 = 0;
                __delay_us(638); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    // cuarto compas
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(758);
                nota1 = 0;
                __delay_us(758); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(956);
                nota1 = 0;
                __delay_us(956); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(851);
                nota1 = 0;
                __delay_us(851); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    if (RB7 == 0 && RB6 == 1){
            for (int x = 0; x < 100; x++){
                nota1 = 1;
                __delay_us(1012);
                nota1 = 0;
                __delay_us(1012); 
            }
        }
    if (RB7 == 0 && RB6 == 1){
            __delay_ms(200);
        }
    
    
    
    
    
    
    
    
    
    //**************************************************************************
    // MENU 
    if (RB7 == 1 && RB6 == 0){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(382);
                nota1 = 0;
                __delay_us(382); 
            }
        }
    if (RB7 == 1 && RB6 == 0){
            __delay_ms(200);
        }
    if (RB7 == 1 && RB6 == 0){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(191);
                nota1 = 0;
                __delay_us(191); 
            }
        }
    if (RB7 == 1 && RB6 == 0){
            __delay_ms(250);
        }
    if (RB7 == 1 && RB6 == 0){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(454);
                nota1 = 0;
                __delay_us(454); 
            }
        }
    if (RB7 == 1 && RB6 == 0){
            __delay_ms(250);
        }
    if (RB7 == 1 && RB6 == 0){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(227);
                nota1 = 0;
                __delay_us(227); 
            }
        }
    if (RB7 == 1 && RB6 == 0){
            __delay_ms(250);
        }
    if (RB7 == 1 && RB6 == 0){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(405);
                nota1 = 0;
                __delay_us(405); 
            }
        }
    if (RB7 == 1 && RB6 == 0){
            __delay_ms(250);
        }
    if (RB7 == 1 && RB6 == 0){
            for (int x = 0; x < 50; x++){
                nota1 = 1;
                __delay_us(202);
                nota1 = 0;
                __delay_us(202); 
            }
        }
    if (RB7 == 1 && RB6 == 0){
            __delay_ms(500);
        }
        
}

//******************************************************************************
//  SUBRUTINAS

void Setup(void){
    initOsc(7);             // CONFIGURAR OSCILADOR
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    ANSEL = 0; 
    ANSELH = 0;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB1 = 1;
    
    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA6 = 0;
    //OPTION_REGbits.nRBPU=0;

    
}