
#include "adc.h"

void initADC(uint8_t IRCF) {
    
    ADCON0 = 0b10000011; 
    ADCON1 = 0;
    PIE1bits.ADIE = 1;
    switch(IRCF){
        case 0:
            TRISAbits.TRISA0 = 1;           // entrada
            ANSELbits.ANS0 = 1;             // analogica
            break;
        case 1:
            TRISAbits.TRISA1 = 1;           // entrada
            ANSELbits.ANS1 = 1;             // analogica
            break;
        case 2:
            TRISAbits.TRISA2 = 1;           // entrada
            ANSELbits.ANS2 = 1;             // analogica
            break;
        case 3:
            TRISAbits.TRISA3 = 1;           // entrada
            ANSELbits.ANS3 = 1;             // analogica
            break;
        case 4:
            TRISAbits.TRISA5 = 1;           // entrada
            ANSELbits.ANS4 = 1;             // analogica
            break;
        case 5:
            TRISEbits.TRISE0 = 1;           // entrada
            ANSELbits.ANS5 = 1;             // analogica
            break;
        case 6:
            TRISEbits.TRISE1 = 1;           // entrada
            ANSELbits.ANS6 = 1;             // analogica
            break;
        case 7:
            TRISEbits.TRISE2 = 1;           // entrada
            ANSELbits.ANS7 = 1;             // analogica
            break;
        default:
            TRISAbits.TRISA0 = 1;           // entrada
            ANSELbits.ANS0 = 1;             // analogica
            break;
    }
}