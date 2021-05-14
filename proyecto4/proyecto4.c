#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"

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
void InitUART(void);

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

           //
           // Check if the peripheral access is enabled.
           //
           while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
           {
           }

           //
           // Enable the GPIO pin for the LED (PG2).  Set the direction as output, and
           // enable the GPIO pin for digital function.
           //
           // entradas , salidas y pullups
           GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);  // azul
           GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);  // rojo
           GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);  // verde
           GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);   // enable F4's pullup, the drive strength won't affect the input
           GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);   // enable F4's pullup, the drive strength won't affect the input
           GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);  // boton

           IntMasterEnable();
           IntEnable(INT_UART0);


           InitUART();

       SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
       // CONFIG del timer0 como temporizador periodico
       // TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
       // ui32Period = (SysCtlClockGet())*2;   // periodo para el temporizador (2 segundos)
       // TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -  1); // establece el periodo para el timer
       // IntEnable(INT_TIMER0A);
       // TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
       IntMasterEnable();
       // TimerEnable(TIMER0_BASE, TIMER_A);

       while(1){
           // leer los sensores
           sensor1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
           sensor2 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
           sensor3 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
           sensor4 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);

           // prender los leds segun los sensores
           if (sensor1 == 1){
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
           } else {
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
           }
           if (sensor2 == 1){
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
           } else {
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
           }
           if (sensor3 == 1){
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
           } else {
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
           }
           if (sensor4 == 1){
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
           } else {
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
           }

           // sumar los espacios ocupados y obtener los espacios libres
           libres = 4 - (sensor1 + sensor2 + sensor3 + sensor4);
           // enviar en uart la cantidad de espacios libres
           if (libres_UART != libres){  // si los espacios libres cambiaron enviar el dato
               libres_UART = libres;
               UARTCharPut(UART0_BASE, libres_UART);
               }
           }

}
void InitUART(void){


    /*Enable the GPIO Port A*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /*Enable the peripheral UART Module 0*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);


    /* Make the UART pins be peripheral controlled. */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_TX);

    // IntEnable(INT_UART0);
    //UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

}
