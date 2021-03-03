/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef __I2C_H
#define	__I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

//*****************************************************************************
// Función para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(const unsigned long c);
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2C_Master_Wait(void);
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start(void);
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart(void);
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Stop(void);
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d);
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a);
//*****************************************************************************
// Función para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address);
//*****************************************************************************

//******************************************************************************
// ADXL345
//******************************************************************************
// VARIABLES PARA REGISTROS
#ifndef WRITE_REG
#define WRITE_REG   0xA6
#endif
#ifndef READ_REG
#define READ_REG    0xA7
#endif
#ifndef DATA_FOR
#define DATA_FOR    0x31
#endif
#ifndef BW_RATE
#define BW_RATE     0x2C
#endif
#ifndef POWER_CTL
#define POWER_CTL   0x2D
#endif
#ifndef FIFO_CTL
#define FIFO_CTL    0x38
#endif
#ifndef DATAX0
#define DATAX0      0x32    
#endif
#ifndef DATAX1
#define DATAX1      0x33    
#endif
#ifndef DATAY0
#define DATAY0      0x34    
#endif
#ifndef DATAY1
#define DATAY1      0x35   
#endif
#ifndef DATAZ0
#define DATAZ0      0x36   
#endif
#ifndef DATAZ1
#define DATAZ1      0x37   
#endif

//******************************************************************************
// FUNCIONES ADXL345
void ADXL345_init(void);
unsigned short ADXL345_READ(uint8_t var);
void ADXL345_WRITE(uint8_t var, uint8_t data);

//******************************************************************************

#endif	/* __I2C_H */