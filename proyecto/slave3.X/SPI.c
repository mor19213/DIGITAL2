/*
 * File:   ISP.c
 * Author: danie
 *
 * Created on 11 de febrero de 2021, 10:11 AM
 */


#include <xc.h>
#include "SPI.h"


void initSPI(SPI_type Type, SPI_data Data_sample, clock_IDLE clock_idle, transmit_edge edge){
    TRISC5 = 0;
    // unir seleccion master/slave (y config) con seleccion de reloj 
    SSPCON = Type | clock_idle;
    
    if(Type & 0b00000100){      // si es slave con ss enable
        SSPSTAT = edge;         // solamente elegir edge, bit7 = 0 (middle)

    } else{
        SSPSTAT = Data_sample | edge;   // unir edge y sample
    }
    TRISC3 = 1;             // SCK
}

void spiReceive(void){
    while (!SSPSTATbits.BF);    // esperar a terminar de recibir
};

void spiWrite(char variable){
    SSPBUF = variable;          // mover una variable a sspbuf para escribir
}

char spiRead(void){
    spiReceive();               // al terminar de recibir
    return(SSPBUF);      // retornar el valor de sspbuf (con el valor recibido)
}
