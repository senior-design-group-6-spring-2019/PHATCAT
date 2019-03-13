

/**
 * main.c
 */

// Define pins to be used on microcontroller

#include "driverlib.h"
#include "device.h"

int main(void)
{
    uint16_t state;
     //
     // Initialize device clock and peripherals
     //
     Device_init();

     //
     // Initialize GPIO and configure the GPIO pin as a push-pull output
     //
     Device_initGPIO();
     GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
     GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
     GPIO_setPadConfig(DEVICE_GPIO_PIN_LED2, GPIO_PIN_TYPE_STD);
     GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED2, GPIO_DIR_MODE_OUT);

     //
     // Make GPIO34 an input on GPIO34
     //
     GPIO_setPadConfig(0, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO0
     GPIO_setPinConfig(GPIO_0_GPIO0);               // GPIO0 = GPIO0
     GPIO_setDirectionMode(0, GPIO_DIR_MODE_IN);     // GPIO0 = input
     //GPIO_setAnalogMode(0, GPIO_ANALOG_DISABLED);   // disable analog mode
     //GPIO_setQualificationMode(0, GPIO_QUAL_SYNC);
     //GPIO_setQualificationPeriod(3,510);
     //
     // Initialize PIE and clear PIE registers. Disables CPU interrupts.
     //
     Interrupt_initModule();

     //
     // Initialize the PIE vector table with pointers to the shell Interrupt
     // Service Routines (ISR).
     //
     Interrupt_initVectorTable();

     //
     // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
     //
     EINT;
     ERTM;

     //
     // Loop Forever
     //
     state = GPIO_readPin(GPIO_0_GPIO0);

     for(;;)
     {
         // get the input value
         //uint32_t i = GPIO_readPin(GPIO_0_GPIO0);
         //uint32_t i = GPIO_readPortData(GPIO_0_GPIO0);

         if (GPIO_readPin(GPIO_0_GPIO0)!= state)
         {
             // Turn on LED
             GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);
             GPIO_writePin(DEVICE_GPIO_PIN_LED2, 0);

         }
         else
         {
             //  Turn off LED
             //
             GPIO_writePin(DEVICE_GPIO_PIN_LED1, 1);
             GPIO_writePin(DEVICE_GPIO_PIN_LED2, 1);
             //GPIO_writePin(DEVICE_GPIO_PIN_LED3, 1);
         }
       }

    //return 0;
}
