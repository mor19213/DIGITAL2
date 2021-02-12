/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef __SPI_H
#define __SPI_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <pic16f887.h>

//******************************************************************************
//  Registro SSPCON
//******************************************************************************

typedef enum
{   // master/slave, velocidad y ss
            SPI_MASTER_OSC_4    = 0b00100000,
            SPI_MASTER_OSC_16   = 0b00100001,
            SPI_MASTER_OSC_64   = 0b00100010,
            SPI_MASTER_TMR2     = 0b00100100,
            SPI_SLAVE_SS_EN     = 0b00100100,
            SPI_SLAVE_SS_DIS    = 0b00100101
}SPI_type;

typedef enum
{// normalmente 1 o 0
    SPI_IDLE_2_HIGH     = 0b00110000,
    SPI_CLOCK_IDLE_LOW  = 0b00100000
}clock_IDLE;

//******************************************************************************
// SSPSTAT
//******************************************************************************
typedef enum
{   // sample al medio o final
    MIDDLE  = 0b00000000,
    END     = 0b10000000
}SPI_data;


// transmitir en rising o falling
typedef enum
{
    SPI_IDLE_2_ACTIVE   = 0b00000000,
    SPI_ACTIVE_2_IDLE   = 0b01000000
}transmit_edge;

//******************************************************************************

void initSPI(SPI_type, SPI_data, clock_IDLE, transmit_edge);
void spiReceive(void);
void spiWrite(char);
char spiRead();

#endif	/* SPI_H */

