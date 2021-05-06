#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

uint8_t LED = 2;
uint8_t antirebote = 0;
uint8_t reinicio = 1;
uint8_t boton = 0;

/**
 * main.c
 */
int main(void)
{
    volatile uint32_t ui32Loop;
    SysCtlClockSet(2);
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
       GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);  // azul
       GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);  // rojo
       GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);  // verde
       GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  // make F4 an input
       GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);   // enable F4's pullup, the drive strength won't affect the input
       GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);   // enable F4's pullup, the drive strength won't affect the input
       GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);  // boton

       while(1){
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);   // rojo on
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

           //
           // Delay for a bit.
           //
           for(ui32Loop = 0; ui32Loop < 500000; ui32Loop++)
           {
           }

           //amarillo
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

           //
           // Delay for a bit.
           //
           for(ui32Loop = 0; ui32Loop < 500000; ui32Loop++)
           {
           }
           // verde
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

           //
           // Delay for a bit.
           //
           for(ui32Loop = 0; ui32Loop < 500000; ui32Loop++)
           {
           }
           reinicio = 1;
           while (reinicio){
               boton = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
               if (boton == 0){
                   boton = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
                   antirebote = 1;
               } else if(antirebote == 1){
                   antirebote = 2;
               }
               // verde on
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

               //
               // Delay for a bit.
               //
               for(ui32Loop = 0; ui32Loop < 100000; ui32Loop++)
               {
               }
               // verde off
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

               //
               // Delay for a bit.
               //
               for(ui32Loop = 0; ui32Loop < 100000; ui32Loop++)
               {
               }
               if (antirebote == 2){
                   reinicio = 0;
                   antirebote = 0;
               }
               //while(1){
               //    boton = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
               //    if (boton > 0){
               //        boton = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
               //        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);   // rojo on
               //    } else{
               //        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);   // rojo on
               //    }
               // }
           }
       }
}
