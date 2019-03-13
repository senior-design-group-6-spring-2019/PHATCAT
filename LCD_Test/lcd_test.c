/*
 * LCD_main.c
 *
 *  Created on: Mar 10, 2019
 *      Author: Rossy
 */
// Included Files
//
#include "driverlib.h"
#include "device.h"

// Define LCD pins used
#define DB0 0
#define DB1 1
#define DB2 2
#define DB3 3
#define DB4 4
#define DB5 5
#define DB6 6
#define DB7 7
#define E1  24
#define E2  16
#define R_W  61
#define RS  123
//

// Globals
volatile uint32_t intReadInputCount; // Count number of times read input. volatile since interrupts can modify it

// Prototype Functions
void intReadInput(void);        // Reads an input and prints a coy message on the LCD
void CommandLCDLower(char a);   // Send an instruction configuring the lower half of the LCD
void CommandLCDUpper(char a);   // Send an instruction configuring the upper half of the LCD
void LCDinit(void);             // Function to initialize the LCD. Run this on start.
void PrintChar(char a, int row, int column);    // Print a single character a on the LCD at a row and column
void init_LCD_GPIO(void);        // Initialize GPIO pins for controlling LCD - use #define above to define pin assignments
void PrintString(char* a, int row, int column);  // Print a string to the LCD, define starting row and column and pass the string.

//
// Main
//

void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Device_initGPIO();
    init_LCD_GPIO();    // Sets GPIO pins for use with LCD
    LCDinit();          // Initializes registers within LCD
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
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    Interrupt_enable(INT_XINT1);    // Enable the XINT1 interrupt
    EINT;
    ERTM;

    GPIO_setDirectionMode(25,GPIO_DIR_MODE_IN);          // GPIO25 as input
    GPIO_setQualificationMode(25, GPIO_QUAL_SYNC);      // Sync it with system clock
    GPIO_setQualificationPeriod(3,510);                 // Sync with 510 cycles
    GPIO_setInterruptPin(25,GPIO_INT_XINT1);            // Use this input as the source for int 1
    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_RISING_EDGE);  // Make it rising edge triggered
    GPIO_enableInterrupt(GPIO_INT_XINT1);               // Enable XINT1 via GPIO

    //PrintChar('E',3,7);
    PrintString("EAT MY ASS\0",2,9); // Print a string, don't forget null terminator, then pick row and column of starting.

    //
    // Loop Forever
    //
    for(;;)
    {
        //
        // Turn on LED
        //
        GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);
        //
        // Delay for a bit.
        //
        DEVICE_DELAY_US(500000);

        //
        // Turn off LED
        //
        GPIO_writePin(DEVICE_GPIO_PIN_LED1, 1);
        //
        // Delay for a bit.
        //
        DEVICE_DELAY_US(500000);
    }
}

interrupt void intReadInput(void)
{
    if (intReadInputCount % 2)  // if odd, just used to alternate message.
    {
        PrintString("ANAL BLAST ACHIEVED\0",3,3);   // Print Message
    }
    else
    {
        PrintString("                   \0",3,3);   // Clear message
    }
    intReadInputCount++;
    //
    // Acknowledge this interrupt to get more from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

}

// Send an instruction configuring the lower half of the LCD
void CommandLCDLower(char a)
{
  int i = 0;
  int j[8];

  // Convert char into an array for individual pins
  for (i = 0; i < 8; i++)
  {
      j[i] = !!((a << i) & 0x80);
  }
    // Put char bits on the DB outputs for LCD:
    GPIO_writePin(DB7, j[0]);
    GPIO_writePin(DB6, j[1]);
    GPIO_writePin(DB5, j[2]);
    GPIO_writePin(DB4, j[3]);
    GPIO_writePin(DB3, j[4]);
    GPIO_writePin(DB2, j[5]);
    GPIO_writePin(DB1, j[6]);
    GPIO_writePin(DB0, j[7]);

    GPIO_writePin(R_W, 0);
    GPIO_writePin(RS, 0);

    GPIO_writePin(E1, 1);   // 2us pulse on chip enable - LCD triggers on falling edge
    DEVICE_DELAY_US(2);
    GPIO_writePin(E1, 0);
}
//

// Send an instruction configuring the upper half of the LCD
void CommandLCDUpper(char a)
{
  int i = 0;
  int j[8];

  // Convert char into an array for individual pins
  for (i = 0; i < 8; i++)
  {
      j[i] = !!((a << i) & 0x80);
      //  printf("%d",j[i]);
  }
    // Put char bits on the DB outputs for LCD:
    GPIO_writePin(DB7, j[0]);
    GPIO_writePin(DB6, j[1]);
    GPIO_writePin(DB5, j[2]);
    GPIO_writePin(DB4, j[3]);
    GPIO_writePin(DB3, j[4]);
    GPIO_writePin(DB2, j[5]);
    GPIO_writePin(DB1, j[6]);
    GPIO_writePin(DB0, j[7]);

    GPIO_writePin(R_W, 0);
    GPIO_writePin(RS, 0);

    GPIO_writePin(E2, 1);   // Put char bits on the DB outputs for LCD:
    DEVICE_DELAY_US(2);
    GPIO_writePin(E2, 0);
}
//

// Function to initialize the LCD. Run this on start.
void LCDinit(void)
{
    // Wake up
    DEVICE_DELAY_US(40);
    CommandLCDLower(0x30);
    DEVICE_DELAY_US(40);
    CommandLCDUpper(0x30);
    DEVICE_DELAY_US(40);
    CommandLCDLower(0x30);
    DEVICE_DELAY_US(40);
    CommandLCDUpper(0x30);
    DEVICE_DELAY_US(40);
    CommandLCDLower(0x30);
    DEVICE_DELAY_US(40);
    CommandLCDUpper(0x30);
    DEVICE_DELAY_US(40);

    // Function set, 8 bit, 2 line, 5x8
    CommandLCDLower(0x38);
    DEVICE_DELAY_US(40);
    CommandLCDUpper(0x38);
    DEVICE_DELAY_US(40);

    // Clear
    CommandLCDLower(0x01);
    DEVICE_DELAY_US(1600);
    CommandLCDUpper(0x01);
    DEVICE_DELAY_US(1600);

    // Turn off
    CommandLCDLower(0x08);
    DEVICE_DELAY_US(40);
    CommandLCDUpper(0x08);
    DEVICE_DELAY_US(40);

    // Turn on w/out blinking cursor
    CommandLCDLower(0x0C);
    DEVICE_DELAY_US(40);
    CommandLCDUpper(0x0C);
    DEVICE_DELAY_US(40);
}
//

// Print a single character a on the LCD at a row and column
void PrintChar(char a, int row, int column)
{
    int address = column - 1;
    int address_array[7];
    char char_array[8];
    int i = 0 ; // eat my ass

    // Sending address for character - where it is printed on LCD
    GPIO_writePin(DB7, 1);

    if(row % 2 == 0)    // If row is even
    {
        address += 0x40;    // Adds offset to put in second row
    }

    // Convert char into an array for individual pins
    for (i = 1; i < 8; i++)
    {
        address_array[i-1] = !!((address << i) & 0x80);
    }
    GPIO_writePin(DB6, address_array[0]);
    GPIO_writePin(DB5, address_array[1]);
    GPIO_writePin(DB4, address_array[2]);
    GPIO_writePin(DB3, address_array[3]);
    GPIO_writePin(DB2, address_array[4]);
    GPIO_writePin(DB1, address_array[5]);
    GPIO_writePin(DB0, address_array[6]);

    GPIO_writePin(R_W, 0);
    GPIO_writePin(RS, 0);

    // Pulse upper or lower part of display based on row
    if (row > 2)
    {
        GPIO_writePin(E2, 1);
        DEVICE_DELAY_US(100);
        GPIO_writePin(E2, 0);
    } else
    {
        GPIO_writePin(E1, 1);
        DEVICE_DELAY_US(100);
        GPIO_writePin(E1, 0);
    }

    DEVICE_DELAY_US(2000);

    // Sending character - what character code gets written at address

    // Convert char into an array for individual pins
    for (i = 0; i < 8; i++)
    {
        char_array[i] = !!((a << i) & 0x80);
        //  printf("%d",j[i]);
    }

    GPIO_writePin(DB7, char_array[0]);
    GPIO_writePin(DB6, char_array[1]);
    GPIO_writePin(DB5, char_array[2]);
    GPIO_writePin(DB4, char_array[3]);
    GPIO_writePin(DB3, char_array[4]);
    GPIO_writePin(DB2, char_array[5]);
    GPIO_writePin(DB1, char_array[6]);
    GPIO_writePin(DB0, char_array[7]);

    GPIO_writePin(R_W, 0);
    GPIO_writePin(RS, 1);

    if (row > 2)
    {
        GPIO_writePin(E2, 1);
        DEVICE_DELAY_US(2000);
        GPIO_writePin(E2, 0);
    } else
    {
        GPIO_writePin(E1, 1);
        DEVICE_DELAY_US(2000);
        GPIO_writePin(E1, 0);
    }
}
//

// Initialize GPIO pins for controlling LCD - use #define above to define pin assignments
void init_LCD_GPIO()
{
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);

    GPIO_setPadConfig(DB0, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO0
    GPIO_writePin(DB0, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_0_GPIO0);                // GPIO0 = GPIO0
    GPIO_setDirectionMode(DB0, GPIO_DIR_MODE_OUT);    // GPIO0 = output

    GPIO_setPadConfig(DB1, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO2
    GPIO_writePin(DB1, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_1_GPIO1);                // GPIO2 = GPIO2
    GPIO_setDirectionMode(DB1, GPIO_DIR_MODE_OUT);    // GPIO2 = output

    GPIO_setPadConfig(DB2, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO2
    GPIO_writePin(DB2, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_2_GPIO2);                // GPIO2 = GPIO3
    GPIO_setDirectionMode(DB2, GPIO_DIR_MODE_OUT);    // GPIO2 = output

    GPIO_setPadConfig(DB3, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO3
    GPIO_writePin(DB3, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_3_GPIO3);                // GPIO3 = GPIO3
    GPIO_setDirectionMode(DB3, GPIO_DIR_MODE_OUT);    // GPIO3 = output

    GPIO_setPadConfig(DB4, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO4
    GPIO_writePin(DB4, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_4_GPIO4);                // GPIO4 = GPIO4
    GPIO_setDirectionMode(DB4, GPIO_DIR_MODE_OUT);    // GPIO4 = output

    GPIO_setPadConfig(DB5, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO5
    GPIO_writePin(DB5, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_5_GPIO5);                // GPIO5 = GPIO5
    GPIO_setDirectionMode(DB5, GPIO_DIR_MODE_OUT);    // GPIO5 = output

    GPIO_setPadConfig(DB6, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO6
    GPIO_writePin(DB6, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_6_GPIO6);                // GPIO6 = GPIO6
    GPIO_setDirectionMode(DB6, GPIO_DIR_MODE_OUT);    // GPIO6 = output

    GPIO_setPadConfig(DB7, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO7
    GPIO_writePin(DB7, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_7_GPIO7);                // GPIO7 = GPIO7
    GPIO_setDirectionMode(DB7, GPIO_DIR_MODE_OUT);    // GPIO7 = output

    GPIO_setPadConfig(E1, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPI24
    GPIO_writePin(E1, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_24_GPIO24);                // GPI24 = GPI24
    GPIO_setDirectionMode(E1, GPIO_DIR_MODE_OUT);    // GPI24 = output

    GPIO_setPadConfig(E2, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPI16
    GPIO_writePin(E2, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_16_GPIO16);                // GPI16 = GPI16
    GPIO_setDirectionMode(E2, GPIO_DIR_MODE_OUT);    // GPI16 = output

    GPIO_setPadConfig(R_W, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPI61
    GPIO_writePin(R_W, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_61_GPIO61);                // GPI61 = GPI61
    GPIO_setDirectionMode(R_W, GPIO_DIR_MODE_OUT);    // GPI61 = output

    GPIO_setPadConfig(RS, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPI123
    GPIO_writePin(RS, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_123_GPIO123);              // GPI123 = GPI123
    GPIO_setDirectionMode(RS, GPIO_DIR_MODE_OUT);    // GPI123 = output
}

// Print a string to the LCD, define starting row and column and pass the string.
void PrintString(char* a, int row, int column)
{
    int i = 0;
    for (i = 0 ; i < strlen(a) ; i++) // despite the weird error for strlen, it works just fine...
    {
        PrintChar(a[i],row,column+i);
    }
}
//



//
// End of File
//
