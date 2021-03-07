/*
 * File:   I2C.c
 * Author: danie
 *
 * Created on 25 de febrero de 2021, 09:27 AM
 */

#include "I2C.h"

    
//*****************************************************************************
// Función para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000; 
    // enable serial port (config SDA y SCL), master mode
    SSPCON2 = 0; 
    SSPADD = (_XTAL_FREQ/(4*c))-1;  //elegir master mode
    SSPSTAT = 0; 
    TRISCbits.TRISC3 = 1;               //  SCL
    TRISCbits.TRISC4 = 1;               //  SDA
}
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.SEN = 1;    //inicia la comunicación i2c
}
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RSEN = 1;   //reinicia la comunicación i2c
}
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Stop()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.PEN = 1;    //detener la comunicación i2c
}
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPBUF = d;             
}
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato leído
}
//*****************************************************************************
// Función para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}

//******************************************************************************
// ADXL345
//******************************************************************************
void ADXL345_init(void){
    ADXL345_WRITE(DATA_FOR, 0x0B);  // formato del dato 
    __delay_ms(2);
    ADXL345_WRITE(POWER_CTL, 0x08); // registro power 
    __delay_ms(2);
}
signed short ADXL345_READ(uint8_t var){
    signed short leido;
    I2C_Master_Start();
    I2C_Master_Write(WRITE_REG);    // escribir registro Write 
    I2C_Master_Write(var);          // Data que se quiere leer
    I2C_Master_Start();             
    I2C_Master_Write(READ_REG);     // registro lectura
    leido = I2C_Master_Read(0);     // leer 
    I2C_Master_Stop();
    return leido;
}
void ADXL345_WRITE(uint8_t var, uint8_t data){
    I2C_Master_Start();
    I2C_Master_Write(WRITE_REG);        // registro escritura
    I2C_Master_Write(var);
    I2C_Master_Write(data);
    I2C_Master_Stop();
}

signed short ADXL345_readX(void){
     int8_t data_x[2];
     int16_t accel_x;
     signed short acc_x;
     data_x[0] = ADXL345_READ(DATAX0);      // obtener 2 bytes data x
     data_x[1] = ADXL345_READ(DATAX1);
     accel_x = (data_x[1]*256)+(data_x[0]); // unir dos bytes 
     acc_x = (accel_x)/16;
     return acc_x;
}
signed short ADXL345_readY(void){
     int8_t data_y[2];
     int16_t accel_y;
     signed short acc_y;
     data_y[0] = ADXL345_READ(DATAY0); // obtener 2 bytes data y
     data_y[1] = ADXL345_READ(DATAY1);
     accel_y = (data_y[1]*256)+(data_y[0]); // unir dos bytes
     acc_y = (accel_y)/16;
     return acc_y;
}
signed short ADXL345_readZ(void){
     int8_t data_z[2];
     int16_t accel_z;
     signed short acc_z;
     data_z[0] = ADXL345_READ(DATAZ0);  // obtener 2 bytes data z 
     data_z[1] = ADXL345_READ(DATAZ1);
     accel_z = (data_z[1]*256)+(data_z[0]); // unir dos bytes 
     acc_z = (accel_z)/16.384;
     return acc_z;
}
