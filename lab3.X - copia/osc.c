
#include "osc.h"

void initOsc(uint8_t IRCF) {
    switch(IRCF){
        
        case 0:
            OSCCONbits.IRCF = 000;
            break;
        case 1:
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 1;
            break;
        case 2:
            OSCCONbits.IRCF = 010;
            break;
        case 3:
            OSCCONbits.IRCF = 011;
            break;
        case 4:
            OSCCONbits.IRCF = 100;
            break;
        case 5:
            OSCCONbits.IRCF = 101;
            break;
        case 6:
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 1;
            break;
        case 7:
            OSCCONbits.IRCF = 111;
            break;
        default:
            OSCCONbits.IRCF = 111;
            break;
    }
    OSCCONbits.SCS = 1;
}