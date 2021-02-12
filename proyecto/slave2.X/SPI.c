/*
 * File:   ISP.c
 * Author: danie
 * basado en "SPI Library for MPLAB XC8" de electroSome
 * Created on 11 de febrero de 2021, 10:11 AM
 */

#include "SPI.h"

void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    // unir seleccion master/slave (y config) con seleccion de reloj 
    if(sType & 0b00000100)          // si es slave con ss enable
    {
        SSPSTAT = sTransmitEdge;    // solamente elegir edge, bit7 = 0 (middle
        TRISC3 = 1;                 // SCK innput
    }
    else              //If Master Mode
    {
        SSPSTAT = sDataSample | sTransmitEdge;  // unir edge y sample
        TRISC3 = 0;                             // SCK output
    }
    
    SSPCON = sType | sClockIdle;
}

static void spiReceiveWait(){
    while ( !SSPSTATbits.BF ); // esperar a terminar de recibir
}

void spiWrite(char variable)  {
    SSPBUF = variable;          // mover una variable a sspbuf para escribir
}

//unsigned spiDataReady() //Check whether the data is ready to read
//{
  //  if(SSPSTATbits.BF)
    //    return 1;
   // else
     //   return 0;
//}

char spiRead(){
    spiReceiveWait();        // al terminar de recibir
    return(SSPBUF);       // retornar el valor de sspbuf (con el valor recibido)
}

