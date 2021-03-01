#include "USART.h"

void initUSART(void) {
    
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    
// velocidad
    SPBRG = 25;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    SPBRGH = 0;
    
// TX
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
    
// RX
    PIE1bits.RCIE = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
            
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

}