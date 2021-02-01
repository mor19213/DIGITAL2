
#include "DISPLAYS.h"

void initDisplays(uint8_t VALOR) {
    VALOR = VALOR / 16;
    switch(VALOR){
        case 0:
            PORTD = 0b01111110;
            break;
        case 1:
            PORTD = 0b11000000;
            break;
        case 2:
            PORTD = 0b11011000;
            break;
        case 3:
            PORTD = 0b11110010;
            break;
        case 4:
            PORTD = 0b11100100;
            break;
        case 5:
            PORTD = 0b10110110;
            break;
        case 6:
            PORTD = 0b10111110;
            break;
        case 7:
            PORTD = 0b01100010;
            break;
        case 8:
            PORTD = 0b11111110;
            break;
        case 9:
            PORTD = 0b11110110;
            break;
        case 10:                //A
            PORTD = 0b11101110;
            break;
        case 11:                //b
            PORTD = 0b10111100;
            break;
        case 12:                //C
            PORTD = 0b00011110;
            break;
        case 13:                //d
            PORTD = 0b11111000;
            break;
        case 14:                //E
            PORTD = 0b10011110;
            break;
        case 15:                //F
            PORTD = 0b10001110;
        default:
            PORTD = 0;
            break;
    }
}