#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

 void uartIntHandler(void);
uint32_t ui32Period = 0;
uint32_t recibido = 0;
uint8_t rojo = 0;
uint8_t verde = 0;
uint8_t azul = 0;
uint8_t parqueo1 = 0;
uint8_t parqueo2 = 0;
uint8_t parqueo3 = 0;
uint8_t parqueo4 = 0;
uint8_t sensor1 = 0;
uint8_t sensor2 = 0;
uint8_t sensor3 = 0;
uint8_t sensor4 = 0;
uint8_t libres = 0;
uint8_t libres_UART = 4;

/**
 * main.c
 */

int main(void)
{
    volatile uint32_t ui32Loop;

    SysCtlClockSet(
            SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
                    | SYSCTL_OSC_MAIN);
    //
           // Enable the GPIO port that is used for the on-board LED.
           //
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

           //
           // Enable the GPIO pin for the LED (PG2).  Set the direction as output, and
           // enable the GPIO pin for digital function.
           //
           // entradas , salidas y pullups
           GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,  GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 );  // luces
           GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,  GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_2 );  // luces
           GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  // make F4 an input
           GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);   // enable F4's pullup, the drive strength won't affect the input
       while(1){
           // leer los sensores
           sensor1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
           // prender los leds segun los sensores
           GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);
           GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);


           // sumar los espacios ocupados y obtener los espacios libres
           libres = 4 - (sensor1 + sensor2 + sensor3 + sensor4);
       }

}
