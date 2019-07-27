
//
// Included Files
//

#include "driverlib.h"
#include "ctype.h"
#include "device.h"
#include "stdlib.h"
#include "group6_sci.h"
#include "stdio.h"
#include "string.h"

////
//// Function Prototypes
////
//
//__interrupt void scibTXFIFOISR(void);
//__interrupt void scibRXFIFOISR(void);
//void initSCIBFIFO(void);
//void error(void);

//

// Defines
//
// Define AUTOBAUD to use the autobaud lock feature
//#define AUTOBAUD

////
//// Globals
////
//
volatile uint16_t rcvd_array[10] = {0};
volatile uint16_t rcvd_array2[10] = {0};
volatile char send_array[3000] = "";
////
//// Received data for SCI-A
////
//uint16_t rDataB[50];
////
//// Send data for SCI-A
////
//uint16_t sDataA[50];
////
//// Used for checking the received data
////
//uint16_t loopCounter = 0;
//volatile uint16_t rcvdArray[1024];
//uint16_t receivedChar;
//char *msg1;
//char *msg2;
//char *msg3;
//volatile uint16_t d = 0;
//volatile uint16_t c = 0;
//volatile uint16_t j = 0;
//volatile uint16_t i = 0;
//volatile uint16_t count = 0;
//
////int count = 0;

//
// Main
//

void main(void)

{
    //
    // Configure PLL, disable WD, enable peripheral clocks.
    //

    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //

    Device_initGPIO();

    //
    // GPIO19 is the SCI Rx pin.
    //
    GPIO_setMasterCore(19, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_19_SCIRXDB);
    GPIO_setDirectionMode(19, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(19, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(19, GPIO_QUAL_ASYNC);

    //
    // GPIO18 is the SCI Tx pin.
    //

    GPIO_setMasterCore(18, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_18_SCITXDB);
    GPIO_setDirectionMode(18, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(18, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(18, GPIO_QUAL_ASYNC);

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
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    Interrupt_register(INT_SCIB_RX, scibRXFIFOISR);
    Interrupt_register(INT_SCIB_TX, scibTXFIFOISR);

    //
    // Initialize the Device Peripherals:
    //

    initSCIBFIFO();

    //
    // Init the send data.  After each transmission this data
    // will be updated for the next transmission
    //
//    uint16_t i;
//    for(i = 0; i < 10; i++)
//    {
//        sDataA[i] = i;
//    }
//    rDataPointA = sDataA[0];

    Interrupt_enable(INT_SCIB_RX);
    Interrupt_enable(INT_SCIB_TX);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

//    msg = "Ready";
//    SCI_writeCharArray(SCIB_BASE, msg, strlen(msg));
//    msg1 = '1';
//    SCI_writeCharArray(SCIB_BASE, msg1, 1);
    for (;;)
    {
        count = 0;
//        loopCounter++;
    }
}

//void error(void)
//
//{
//    Example_Fail = 1;
//    asm("     ESTOP0");
//    // Test failed!! Stop!
//    for (;;)
//        ;
//
//}

// scibTXFIFOISR - SCIA Transmit FIFO ISR
__interrupt void scibTXFIFOISR(void)
{
//    uint16_t i;
//    SCI_writeCharArray(SCIA_BASE, sDataA, 15);
//
// Increment send data for next cycle
//
//    for(i = 0; i < 2; i++)
//    {
//        sDataA[i] = (sDataA[i] + 1) & 0x00FF;
//    }
    SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_TXFF);
    //
    // Issue PIE ACK
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

//
// sciaRXFIFOISR - SCIA Receive FIFO ISR
//
__interrupt void scibRXFIFOISR(void)
{

    volatile uint16_t length;
    volatile uint16_t size;

    uint16_t num_char = 5;
    uint16_t num_char2 = 4;

    count++;

    char digit_char_array[40] = "";
//    settings_ptr = &settings;
    // TODO send ready for characters

    //
    // Wait for RRDY/RXFFST = 1 for 1 data available in FIFO
    //
//    while (SCI_getRxFIFOStatus(SCIA_BASE) == SCI_FIFO_RX0)
//    {
//        ;
//    }
//    if (SCI_getRxFIFOStatus(SCIA_BASE) == SCI_FIFO_RX0)
//    {
//        goto INT_END;
//    }
//    SCI_readCharArray(SCIB_BASE, rcvd_array, num_char);

        SCI_readCharArray(SCIB_BASE, rcvd_array, num_char);

//        length = sizeof(rcvdArray) / sizeof(uint16_t);
//    length = num_char;
//    createSettings(rcvd_array, num_char, &settings);


        SCI_readCharArray(SCIB_BASE, rcvd_array2, num_char2);
        length = strlen(rcvd_array2);

//    INT_END:
    SCI_clearOverflowStatus(SCIB_BASE);
    SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_RXFF);
//        count++;

//    createSettingsArray(send_array, num_char, &settings, digit_char_array);
//    }

    //
    // Issue PIE ack
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);

//    Example_PassCount++;
//    }
}
void initSCIBFIFO(void)
{
//    memset(rcvd_array, 0, 4000);
    //
    // 8 char bits, 1 stop bit, no parity. Baud rate is 9600.
    //
    SCI_setConfig(SCIB_BASE, DEVICE_LSPCLK_FREQ, 9600, (SCI_CONFIG_WLEN_8 |
    SCI_CONFIG_STOP_ONE | SCI_CONFIG_PAR_NONE));
    SCI_enableModule(SCIB_BASE);
    // disable loopback test mode if I turn it off it breaks
    SCI_disableLoopback(SCIB_BASE);
    SCI_resetChannels(SCIB_BASE);
    SCI_enableFIFO(SCIB_BASE);
    SCI_resetRxFIFO(SCIB_BASE);
    SCI_resetTxFIFO(SCIB_BASE);
    //
    // RX and TX FIFO Interrupts Enabled
    //
    SCI_enableInterrupt(SCIB_BASE, (SCI_INT_RXFF | SCI_INT_TXFF));
    SCI_enableInterrupt(SCIB_BASE, (INT_SCIB_RX | INT_SCIB_TX));
    SCI_disableInterrupt(SCIB_BASE, SCI_INT_RXERR);
    SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_clearInterruptStatus(SCIB_BASE, INT_SCIB_RX | INT_SCIB_TX);
    SCI_setFIFOInterruptLevel(SCIB_BASE, SCI_FIFO_TX2, SCI_FIFO_RX2);
    SCI_performSoftwareReset(SCIB_BASE);
    SCI_resetTxFIFO(SCIB_BASE);
    SCI_resetRxFIFO(SCIB_BASE);

#ifdef AUTOBAUD
    //
    // Perform an autobaud lock.
    // SCI expects an 'a' or 'A' to lock the baud rate.
    //
    SCI_lockAutobaud(SCIA_BASE);
#endif

}
// TODO store the struct settings into the array so we can send it back to desktop application
void createSettingsArray(volatile char *send_array, int length, settings_struct* settings_ptr,
                         char *digit_char_array)
{
    uint16_t i = 0;
    uint16_t ptr_i = 0;
    uint16_t sa_i = 0;
    length = 0;
//    int size = sizeof(char);
//    int size2 = sizeof(uint16_t);

//    while (1)
//    {
        // get the name from the settings
        length = strlen(settings_ptr->name);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->name[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get max mva from settings
//gcvt (settings_ptr->max_mva, 7, digit_array);
        sprintf(digit_char_array, "%.2f", settings_ptr->max_mva);
//        ftoa(settings_ptr->max_mva, digit_char_array, 7);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get nom freq from the settings
//gcvt(settings_ptr->nom_freq, 7, digit_char_array);
        sprintf(digit_char_array, "%.2f", settings_ptr->nom_freq);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get winding from settings
        length = strlen(settings_ptr->winding);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->winding[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get connection type from settings
        length = strlen(settings_ptr->connection_type);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->connection_type[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the w_wind volt rate from settings
//gcvt (settings_ptr->w_wind_volt_rate, 7, digit_char_array);
        sprintf(digit_char_array, "%.2f", settings_ptr->w_wind_volt_rate);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind ct ratio
//gcvt (settings_ptr->w_wind_ct_ratio, 7, digit_char_array);
        sprintf(digit_char_array, "%.2f", settings_ptr->w_wind_ct_ratio);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind neutral ratio
//gcvt (settings_ptr->w_wind_neu_ratio, 7, digit_char_array);
        sprintf(digit_char_array, "%.2f", settings_ptr->w_wind_neu_ratio);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind pt ratio
//gcvt (settings_ptr->w_wind_pt_ratio, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->w_wind_pt_ratio);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);


        // get the x_wind volt rate from settings
//gcvt (settings_ptr->x_wind_volt_rate, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->x_wind_volt_rate);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind ct ratio
//gcvt (settings_ptr->x_wind_ct_ratio, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->x_wind_ct_ratio);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind neutral ratio
//gcvt (settings_ptr->x_wind_neu_ratio, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->x_wind_neu_ratio);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind pt ratio
//gcvt (settings_ptr->x_wind_pt_ratio, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->x_wind_pt_ratio);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the nom phase volt
//gcvt (settings_ptr->nom_phase_volt, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->nom_phase_volt);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the led1 status from the settings
        length = strlen(settings_ptr->led1_status);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->led1_status[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the led2 status from the settings
        length = strlen(settings_ptr->led2_status);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->led2_status[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the led3 status from the settings
        length = strlen(settings_ptr->led3_status);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->led3_status[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the led4 status from the settings
        length = strlen(settings_ptr->led4_status);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->led4_status[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the oc1 status from the settings
        length = strlen(settings_ptr->oc1_status);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->oc1_status[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the oc2 status from the settings
        length = strlen(settings_ptr->oc2_status);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->oc2_status[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the oc3 status from the settings
        length = strlen(settings_ptr->oc3_status);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->oc3_status[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the rest diff pickup current
//gcvt (settings_ptr->restrained_diff_pickup_current, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->restrained_diff_pickup_current);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the rest diff first slope
//gcvt (settings_ptr->restrained_diff_fir_slope, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->restrained_diff_fir_slope);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the rest diff first slope
//gcvt (settings_ptr->restrained_diff_sec_slope, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->restrained_diff_sec_slope);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the rest diff first slope
//gcvt (settings_ptr->restrained_diff_slope_intersect, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->restrained_diff_slope_intersect);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the thresh 2nd harmonic
//gcvt (settings_ptr->threshold_blocking_2nd_harmonic, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->threshold_blocking_2nd_harmonic);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the thresh 4th harmonic
//gcvt (settings_ptr->threshold_blocking_4th_harmonic, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->threshold_blocking_4th_harmonic);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the thresh 5th harmonic
//gcvt (settings_ptr->threshold_blocking_5th_harmonic, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->threshold_blocking_5th_harmonic);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the curr diff enable the settings
        length = strlen(settings_ptr->curr_diff_enable_eq);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->curr_diff_enable_eq[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the w_wind phase oc
//gcvt (settings_ptr->phase_w_wind_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_w_wind_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind phase delay
//gcvt (settings_ptr->phase_w_wind_oc_delay, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_w_wind_oc_delay);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind phase time inverse
//gcvt (settings_ptr->phase_w_wind_time_inverse_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_w_wind_time_inverse_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind phase curve style
//gcvt(settings_ptr->phase_w_wind_curve_style, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_x_wind_curve_style);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind phase time dial
//gcvt (settings_ptr->phase_w_wind_time_dial, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_w_wind_time_dial);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);


        // get the w_wind elctro reset the settings
        length = strlen(settings_ptr->phase_w_wind_electro_reset);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->phase_w_wind_electro_reset[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the w_wind neutral oc
//gcvt(settings_ptr->neutral_w_wind_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->neutral_w_wind_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind neutral delay
//gcvt(settings_ptr->neutral_w_wind_oc_delay, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->neutral_w_wind_oc_delay);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind ground oc pickup
//gcvt (settings_ptr->ground_w_wind_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_w_wind_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind ground delay
//gcvt (settings_ptr->ground_w_wind_oc_delay, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_w_wind_oc_delay);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind ground time inverse
//gcvt (settings_ptr->ground_w_wind_time_inverse_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_w_wind_time_inverse_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind ground curve style
//gcvt(settings_ptr->ground_w_wind_curve_style, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_x_wind_curve_style);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the w_wind ground time dial
//gcvt (settings_ptr->ground_w_wind_time_dial, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_w_wind_time_dial);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);


        // get the w_wind elctro reset the settings
        length = strlen(settings_ptr->ground_w_wind_electro_reset);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->ground_w_wind_electro_reset[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;


        // get the x_wind phase oc
//gcvt (settings_ptr->phase_x_wind_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_x_wind_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind phase delay
//gcvt (settings_ptr->phase_x_wind_oc_delay, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_x_wind_oc_delay);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind phase time inverse
//gcvt (settings_ptr->phase_x_wind_time_inverse_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_x_wind_time_inverse_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind phase curve style
//gcvt(settings_ptr->phase_x_wind_curve_style, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_x_wind_curve_style);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind phase time dial
//gcvt (settings_ptr->phase_x_wind_time_dial, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->phase_x_wind_time_dial);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);


        // get the x_wind elctro reset the settings
        length = strlen(settings_ptr->phase_x_wind_electro_reset);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->phase_x_wind_electro_reset[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the x_wind neutral oc
//gcvt(settings_ptr->neutral_x_wind_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->neutral_x_wind_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind neutral delay
//gcvt(settings_ptr->neutral_x_wind_oc_delay, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->neutral_x_wind_oc_delay);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind ground oc
//gcvt (settings_ptr->ground_x_wind_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_x_wind_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind ground delay
//gcvt (settings_ptr->ground_x_wind_oc_delay, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_x_wind_oc_delay);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind ground time inverse
//gcvt (settings_ptr->ground_x_wind_time_inverse_oc_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_x_wind_time_inverse_oc_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind ground curve style
//gcvt(settings_ptr->ground_x_wind_curve_style, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_x_wind_curve_style);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the x_wind ground time dial
//gcvt (settings_ptr->ground_x_wind_time_dial, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->ground_x_wind_time_dial);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);


        // get the x_wind elctro reset the settings
        length = strlen(settings_ptr->ground_x_wind_electro_reset);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = settings_ptr->ground_x_wind_electro_reset[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        length = 0;

        // get the volts/hz level1 pickup
//gcvt (settings_ptr->level1_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->level1_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the volts/hz level1 time delay
//gcvt (settings_ptr->level1_time_delay, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->level1_time_delay);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the volts/hz level2 pickup
//gcvt (settings_ptr->level2_pickup, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->level2_pickup);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);

        // get the volts/hz level2 time delay
//gcvt (settings_ptr->level2_time_delay, 7, digit_char_array);
        //sprintf(digit_char_array, "%.2f", settings_ptr->level2_time_delay);
        length = strlen(digit_char_array);
        for (i = 0; i < length; i++)
        {
            send_array[sa_i] = digit_char_array[i];
            //ptr_i++;
            sa_i++;
        }
        ptr_i = 0;
        memset(digit_char_array, 0, 40);
//
//        // break out when finished
//        break;
//    }
}

void createSettings(volatile uint16_t rcvd_array[], int length,
                    settings_struct* settings_ptr)
{
    volatile uint16_t char_array[120] = { 0 };
    volatile uint16_t digit_array[40] = { 0 };
    volatile uint16_t delim_index = 0;
    volatile uint16_t size = 0;

    // TODO added them here for scope see if works when can
    volatile uint16_t d = 0;
    volatile uint16_t c = 0;
    volatile uint16_t j = 0;
    volatile uint16_t i = 0;

    while (j < length)
    {

        // TODO make it check for char or int and convert accordingly and store it
        // if its is a digit
        if (isdigit(rcvd_array[j]) != 0 || rcvd_array[j] == '.')
        {
            digit_array[d] = rcvd_array[j];
            d++;
            //           DEVICE_DELAY_US(100);
        }
        // if its is a character
        else if (isalpha(rcvd_array[j]) != 0 || rcvd_array[j] == '-'
                || rcvd_array[j] == ' ')
        {
            char_array[c] = rcvd_array[j];
            c++;
            //           DEVICE_DELAY_US(100);
        }
        //if its a ; we know thats the end of that setting part
        else if (rcvd_array[j] == ';')
        {
//            if (rcvd_array[j + 1] == ';')
//            {
//                break;
//            }
            delim_index++;
            // TODO make a switch loop for the index to put the arrays in their correct spot in struct
            switch (delim_index)
            {
            case 1:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->name[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;

            }
            case 2:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while (i < size)
//                   {
                settings_ptr->max_mva = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 3:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->nom_freq = atof(digit_array);
//                       i++;
//
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 4:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->winding[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 5:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->connection_type[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 6:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->w_wind_volt_rate = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 7:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->w_wind_ct_ratio = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 8:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->w_wind_neu_ratio = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 9:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->w_wind_pt_ratio = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 10:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->x_wind_volt_rate = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 11:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->x_wind_ct_ratio = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 12:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->x_wind_neu_ratio = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 13:
            {
//                   i = 0;
//                   size = sizeof(digit_array) / sizeof(uint16_t);
//                   while(i < size)
//                   {
                settings_ptr->x_wind_pt_ratio = atof(digit_array);
//                       i++;
//                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 14:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->nom_phase_volt = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 15:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->led1_status[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 16:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->led2_status[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 17:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->led3_status[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 18:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->led4_status[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 19:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->oc1_status[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 20:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->oc2_status[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 21:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->oc3_status[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 22:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->restrained_diff_pickup_current = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 23:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->restrained_diff_fir_slope = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 24:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->restrained_diff_sec_slope = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 25:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->restrained_diff_slope_intersect = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 26:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->threshold_blocking_2nd_harmonic = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 27:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->threshold_blocking_4th_harmonic = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 28:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->threshold_blocking_5th_harmonic = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 29:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->unrest_diff_pickup_current = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 30:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->curr_diff_enable_eq[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 31:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->phase_w_wind_oc_pickup = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 32:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->phase_w_wind_oc_delay = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 33:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->phase_w_wind_time_inverse_oc_pickup = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }

            case 34:
            {
//                i = 0;
//                size = sizeof(digit_array) / sizeof(uint16_t);
//                while (i < size)
//                {
                settings_ptr->phase_w_wind_curve_style = atof(digit_array);
//                    i++;
//                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 35:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->phase_w_wind_time_dial = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 36:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->phase_w_wind_electro_reset[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
//            case 37:
//            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
//                    settings_ptr->phase_w_wind_neu_enable_eq[i] =
//                            char_array[i];
//                    i++;
//                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
            case 37:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->neutral_w_wind_oc_pickup = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 38:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->neutral_w_wind_oc_delay = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
//            case 40:
//            {
//                //                   i = 0;
//                //                   size = sizeof(digit_array) / sizeof(uint16_t);
//                //                   while(i < size)
//                //                   {
//                settings_ptr->neutral_w_wind_time_inverse_oc_pickup = atof(
//                        digit_array);
//                //                       i++;
//                //                   }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
//
//            case 41:
//            {
////                i = 0;
////                size = sizeof(char_array) / sizeof(uint16_t);
////                while (i < size)
////                {
//                settings_ptr->neutral_w_wind_curve_style = atof(digit_array);
////                    i++;
////                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
//            case 42:
//            {
//                //                   i = 0;
//                //                   size = sizeof(digit_array) / sizeof(uint16_t);
//                //                   while(i < size)
//                //                   {
//                settings_ptr->neutral_w_wind_time_dial = atof(digit_array);
//                //                       i++;
//                //                   }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
//            case 43:
//            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
//                    settings_ptr->neutral_w_wind_electro_reset[i] =
//                            char_array[i];
//                    i++;
//                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
//            case 44:
//            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
//                    settings_ptr->neutral_w_wind_neu_enable_eq[i] =
//                            char_array[i];
//
//                    i++;
//                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
            case 39:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->ground_w_wind_oc_pickup = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 40:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->ground_w_wind_oc_delay = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 41:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->ground_w_wind_time_inverse_oc_pickup = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }

            case 42:
            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
                settings_ptr->ground_w_wind_curve_style = atof(digit_array);
//                    i++;
//                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 43:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->ground_w_wind_time_dial = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 44:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->ground_w_wind_electro_reset[i] =
                            char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
//            case 51:
//            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
//                    settings_ptr->ground_w_wind_neu_enable_eq[i] =
//                            char_array[i];
//
//                    i++;
//                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
            case 45:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->phase_x_wind_oc_pickup = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 46:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->phase_x_wind_oc_delay = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 47:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->phase_x_wind_time_inverse_oc_pickup = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }

            case 48:
            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
                settings_ptr->phase_x_wind_curve_style = atof(digit_array);
//                    i++;
//                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 49:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->phase_x_wind_time_dial = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 50:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->phase_x_wind_electro_reset[i] = char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
////            case 44:
////            {
////                i = 0;
////                size = sizeof(char_array) / sizeof(uint16_t);
////                while (i < size)
////                {
////                    settings_ptr->phase_x_wind_neu_enable_eq[i] =
////                            char_array[i];
////
////                    i++;
////                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
            case 51:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->neutral_x_wind_oc_pickup = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 52:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->neutral_x_wind_oc_delay = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
//            case 53:
//            {
//                //                   i = 0;
//                //                   size = sizeof(digit_array) / sizeof(uint16_t);
//                //                   while(i < size)
//                //                   {
//                settings_ptr->neutral_x_wind_time_inverse_oc_pickup = atof(
//                        digit_array);
//                //                       i++;
//                //                   }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }

//            case 54:
//            {
////                i = 0;
////                size = sizeof(char_array) / sizeof(uint16_t);
////                while (i < size)
////                {
//                settings_ptr->neutral_x_wind_curve_style = atof(digit_array);
////                    i++;
////                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
//            case 55:
//            {
//                //                   i = 0;
//                //                   size = sizeof(digit_array) / sizeof(uint16_t);
//                //                   while(i < size)
//                //                   {
//                settings_ptr->neutral_x_wind_time_dial = atof(digit_array);
//                //                       i++;
//                //                   }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
//            case 56:
//            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
//                    settings_ptr->neutral_x_wind_electro_reset[i] =
//                            char_array[i];
//                    i++;
//                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
//            case 65:
//            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
//                    settings_ptr->neutral_x_wind_neu_enable_eq[i] =
//                            char_array[i];
//
//                    i++;
//                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
            case 53:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->ground_x_wind_oc_pickup = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 54:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->ground_x_wind_oc_delay = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 55:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->ground_x_wind_time_inverse_oc_pickup = atof(
                        digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }

            case 56:
            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
                settings_ptr->ground_x_wind_curve_style = atof(digit_array);
//                    i++;
//                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 57:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->ground_x_wind_time_dial = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 58:
            {
                i = 0;
                size = sizeof(char_array) / sizeof(uint16_t);
                while (i < size)
                {
                    settings_ptr->ground_x_wind_electro_reset[i] =
                            char_array[i];
                    i++;
                }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
//            case 72:
//            {
//                i = 0;
//                size = sizeof(char_array) / sizeof(uint16_t);
//                while (i < size)
//                {
//                    settings_ptr->ground_x_wind_neu_enable_eq[i] =
//                            char_array[i];
//
//                    i++;
//                }
//                memset(char_array, 0, 120);
//                memset(digit_array, 0, 40);
//                c = 0;
//                d = 0;
//                break;
//            }
            case 59:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->level1_pickup = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 60:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->level1_time_delay = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 61:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->level2_pickup = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            case 62:
            {
                //                   i = 0;
                //                   size = sizeof(digit_array) / sizeof(uint16_t);
                //                   while(i < size)
                //                   {
                settings_ptr->level2_time_delay = atof(digit_array);
                //                       i++;
                //                   }
                memset(char_array, 0, 120);
                memset(digit_array, 0, 40);
                c = 0;
                d = 0;
                break;
            }
            }
        }
        j++;
    }
//    return settings;
}

//int run_timer(int *count)
//{
//    for(;;)
//    {
//        *count++;
//        if (*count == 2000)
//        {
//            return 0;
//        }
//    }
//}
//
// End of File
//
