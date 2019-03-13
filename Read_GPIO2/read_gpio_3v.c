
/**
 * main.c
 */

// Define pins to be used on microcontroller
#include "driverlib.h"
#include "device.h"

//
// Globals
//
volatile uint32_t intReadInputCount;  // count the number of times read interrupt fired

//
// Function Prototypes
//
void intReadInput(void);

int main(void)
{
//    uint32_t tempX1Count;
//    uint32_t tempX2Count;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();
    Interrupt_register(INT_XINT1,&intReadInput);    // Create interrupt vector, go do this function for int 1
    intReadInputCount = 0;                          // Count Read Input interrupts

    //
    // Enable XINT1 and XINT2 in the PIE: Group 1 interrupt 4 & 5
    // Enable INT1 which is connected to WAKEINT:
    //
    Interrupt_enable(INT_XINT1);
    //Interrupt_enable(INT_XINT2);
    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    // set up pins for LED
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED2, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED2, GPIO_DIR_MODE_OUT);

    //
    // Make GPIO0 an input on GPIO0
    //
    //GPIO_setPadConfig(25, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO0
    GPIO_setPinConfig(GPIO_25_GPIO25);               // GPIO0 = GPIO0
    GPIO_setDirectionMode(25, GPIO_DIR_MODE_IN);     // GPIO0 = input
    GPIO_setQualificationMode(25, GPIO_QUAL_SYNC);
    GPIO_setQualificationPeriod(3, 510);


    // set the interrupt and enable it
    GPIO_setInterruptPin(25, GPIO_INT_XINT1);
    //GPIO_setInterruptPin(1, GPIO_INT_XINT2);
    // Rising edge interrupt
    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_FALLING_EDGE);
    GPIO_enableInterrupt(GPIO_INT_XINT1);
    //
    // GPIO0 and GPIO1 are inputs
    //
    //GPIO_setDirectionMode(0, GPIO_DIR_MODE_IN);          // input
    // XINT1 Synch to SYSCLKOUT only
    //GPIO_setDirectionMode(1, GPIO_DIR_MODE_IN);          // input
    // XINT2 Qual using 6 samples
    //GPIO_setQualificationMode(1, GPIO_QUAL_6SAMPLE);
    // Set qualification period for GPIO0 to GPIO7
    // Each sampling window is 510*SYSCLKOUT

//    //
//    // Interrupts that are used in this example are re-mapped to
//    // ISR functions found within this file.
//    //
//    Interrupt_register(INT_XINT1, &xint1_isr);
//    Interrupt_register(INT_XINT2, &xint2_isr);

//    //
//    // User specific code, enable interrupts:
//    //
//    // Clear the counters
//    //
//    XINT1Count = 0; // Count XINT1 interrupts
//    XINT2Count = 0; // Count XINT2 interrupts
//    loopCount = 0;  // Count times through idle loop


//    //
//    // GPIO30 & GPIO31 are outputs, start GPIO30 high and GPIO31 low
//    //
//    // Load the output latch
//    GPIO_setPortPins(GPIO_PORT_A, GPIO_GPADIR_GPIO30);
//    GPIO_setDirectionMode(30, GPIO_DIR_MODE_OUT);            // output
//    // Load the output latch
//    GPIO_clearPortPins(GPIO_PORT_A, GPIO_GPADIR_GPIO31);
//    GPIO_setDirectionMode(31, GPIO_DIR_MODE_OUT);            // output


//
//    //
//    // Configure XINT1
//    //
//    // Falling edge interrupt
//    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_FALLING_EDGE);

//    //
//    // Enable XINT1 and XINT2
//    //
//    GPIO_enableInterrupt(GPIO_INT_XINT1);         // Enable XINT1
//    GPIO_enableInterrupt(GPIO_INT_XINT2);         // Enable XINT2

//    //
//    // GPIO34 will go low inside each interrupt.  Monitor this on a scope
//    //
//    GPIO_setDirectionMode(34, GPIO_DIR_MODE_OUT);    // output

//    // read the state of pin0
//    state = GPIO_readPin(GPIO_0_GPIO0);

    //
    // Loop Forever
    //
    for (;;)
    {
//        tempX1Count = XINT1Count;
//        tempX2Count = XINT2Count;
//
//        //
//        // Trigger both XINT1
//        //
//
//        GPIO_setPortPins(GPIO_PORT_B, GPIO_GPBDIR_GPIO34); // GPIO34 is high
//        // Lower GPIO30, trigger XINT1
//        GPIO_clearPortPins(GPIO_PORT_A, GPIO_GPADIR_GPIO30);
//        while (XINT1Count == tempX1Count)
//        {
//        }
//
//        //
//        // Trigger both XINT2
//        //
//        GPIO_setPortPins(GPIO_PORT_B, GPIO_GPBDIR_GPIO34); // GPIO34 is high
//        DEVICE_DELAY_US(DELAY);                      // Wait for Qual period
//        // Raise GPIO31, trigger XINT2
//        GPIO_setPortPins(GPIO_PORT_A, GPIO_GPADIR_GPIO31);
//        while (XINT2Count == tempX2Count)
//        {
//        }
//
//        //
//        // Check that the counts were incremented properly and get ready
//        // to start over.
//        //
//        if (XINT1Count == tempX1Count + 1 && XINT2Count == tempX2Count + 1)
//        {
//            loopCount++;
//            //raise GPIO30
//            GPIO_setPortPins(GPIO_PORT_A, GPIO_GPADIR_GPIO30);
//            // lower GPIO31
//            GPIO_clearPortPins(GPIO_PORT_A, GPIO_GPADIR_GPIO31);
//        }
//        else
//        {
//            asm("      ESTOP0");
//            // stop here
//        }

//    if (GPIO_readPin(GPIO_0_GPIO0)!= state)
//         {
//             // Turn on LED
//             GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);
//             GPIO_writePin(DEVICE_GPIO_PIN_LED2, 0);
//
//         }
//         else
//         {
//             //  Turn off LED
//             //
//             GPIO_writePin(DEVICE_GPIO_PIN_LED1, 1);
//             GPIO_writePin(DEVICE_GPIO_PIN_LED2, 1);
//             //GPIO_writePin(DEVICE_GPIO_PIN_LED3, 1);
//         }
    }

//return 0;
}
//
// xint1_isr - External Interrupt 1 ISR
//
interrupt void intReadInput(void)
{
    if (intReadInputCount % 2) // if odd turn on LED
    {
      //Turn on LED
      GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);
      GPIO_writePin(DEVICE_GPIO_PIN_LED2, 0);
    }
    else // if even turn them off
    {
       //  Turn off LED
       GPIO_writePin(DEVICE_GPIO_PIN_LED1, 1);
       GPIO_writePin(DEVICE_GPIO_PIN_LED2, 1);
    }

    // increment the interrupt count
    intReadInputCount++;

//    //GPIO_clearPortPins(GPIO_PORT_B, GPIO_GPBDIR_GPIO34); //GPIO34 is low
//    XINT1Count++;

    //
    // Acknowledge this interrupt to get more from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

}
