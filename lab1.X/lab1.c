/*
 * File:   helloworld.c
 * Author: danie
 *
 * Created on January 18, 2021, 2:55 PM
 */


#include <xc.h>

//******************************************************************************
//  PALABRA DE CONFIGURACION
// CONFIG1
#pragma config FOSC = EXTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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
#define ledR PORTEbits.RE0
#define ledA PORTEbits.RE1
#define ledV PORTEbits.RE2
#define _XTAL_FREQ 8000000
unsigned char carrera;
unsigned char antirebote1;
unsigned char antirebote2;

//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************
void Setup (void);
void semaforo(void);


void main(void) {
    Setup();
    //LOOP PRINCIPAL
    while(1){
        if(PORTBbits.RB0 == 1) {
            // rutina semaforo si se presiona el boton de inicio
            semaforo();
        }
        if(carrera == 1){
            // al presionar antirebote = 1 
            if (PORTBbits.RB1 == 1){
                antirebote1 = 1;
            }
            if (PORTBbits.RB2 == 1){
                antirebote2 = 1;
            }
            // se incrementa hasta que antirebote = 1 y boton = 0
            if (antirebote1 == 1){
                if (PORTBbits.RB1 == 0){ // volver a poner antirebote en 0
                    antirebote1 = 0;
                    if (PORTCbits.RC7 ==0){
                    // para incrementar decadas multiplicar por 2
                    PORTC = PORTC * 2;
                    if (PORTC == 0){    // si el puerto esta en 0, sumarle 1
                        PORTC = PORTC + 1;
                    }
                    } else {    // si el pin 7 ya estaba prendido, hacer overflow
                        PORTC = 0;
                        PORTBbits.RB6 = 1;
                        carrera = 0;    //terminar carrera
                    }
                }
            }
            
            // se incrementa hasta que antirebote = 1 y boton = 0
            if (antirebote2 == 1){
                if (PORTBbits.RB2 == 0){ // volver a poner antirebote en 0
                    antirebote2 = 0;
                    if (PORTDbits.RD7 == 0){
                    // para incrementar decadas multiplicar por 2
                    PORTD = PORTD * 2;
                    if (PORTD == 0){    // si el puerto esta en 0, sumarle 1
                        PORTD = PORTD + 1;
                    }
                    }else{    // si el pin 7 ya estaba prendido, hacer overflow
                        PORTD = 0;
                        PORTBbits.RB7 = 1;
                        carrera = 0;    //terminar carrera
                    }
                }
            }
        }
    }
}


//******************************************************************************
//  SUBRUTINAS

void semaforo(void){
    if (carrera == 0){
            // Si la carrera no ha iniciado prender el semaforo
            // reiniciar las variables y puertos usados a 0 
            PORTBbits.RB6 = 0;
            PORTBbits.RB7 = 0;
            antirebote1 = 0;
            antirebote2 = 0;
            PORTD = 0;
            PORTC = 0;
            PORTE = 0; 
            ledR =1;
            __delay_ms(50);
            ledR = 0;
            ledA = 1;
            __delay_ms(50);
            ledA = 0;
            ledV = 1;
            carrera = 1;
    }
            
}

void Setup(void){
    PORTB = 0;
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISBbits.TRISB0 = 1;   // Boton semaforo
    TRISBbits.TRISB1 = 1;   // jugador 1
    TRISBbits.TRISB2 = 1;   // jugador 2
    TRISE = 0;              // luces semaforo
    PORTE = 0;
    TRISC = 0;              // jugador 1
    TRISD = 0;              // jugador 2
    PORTC = 0;
    PORTD = 0;
    carrera = 0;
    antirebote1 = 0;
    antirebote2 = 0;
    
}
