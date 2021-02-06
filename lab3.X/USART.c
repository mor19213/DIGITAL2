#include "USART.h"

void initUSART(uint8_t IRCF) {
  
    
// TX
    TXSTAbits.SYNC = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TX9 = 0;
    
// velocidad
    BAUDCTLbits.BRG16 = 0;
    SPBRG = 25;
    SPBRGH = 0;
    
// RX
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
}