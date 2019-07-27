/*
 * group6_sci.h
 *
 *  Created on: Jul 18, 2019
 *      Author: johnd
 */

#ifndef GROUP6_SCI_H_
#define GROUP6_SCI_H_

// struct to hold the channels for current and voltages
// which will have a magnitude and a phase
typedef struct Settings_Struct
{
    // eq data settings
    float max_mva, nom_freq, w_wind_volt_rate, w_wind_ct_ratio,
            w_wind_neu_ratio, w_wind_pt_ratio, x_wind_volt_rate,
            x_wind_ct_ratio, x_wind_neu_ratio, x_wind_pt_ratio, nom_phase_volt;
    char name[40], winding[9], connection_type[6];

    // led settings
    char led1_status[120], led2_status[120], led3_status[120], led4_status[120];

    // output contact settings
    char oc1_status[120], oc2_status[120], oc3_status[120];

    // current diff settings
    float restrained_diff_pickup_current, restrained_diff_fir_slope,
            restrained_diff_sec_slope, restrained_diff_slope_intersect;
    float threshold_blocking_2nd_harmonic, threshold_blocking_4th_harmonic,
            threshold_blocking_5th_harmonic, unrest_diff_pickup_current;
    char curr_diff_enable_eq[120];

    // w-winding settings
    // phase
    float phase_w_wind_oc_pickup, phase_w_wind_oc_delay,
            phase_w_wind_time_inverse_oc_pickup, phase_w_wind_time_dial;
    int phase_w_wind_curve_style;
    char phase_w_wind_electro_reset[3];
//            phase_w_wind_neu_enable_eq[120];
    // neutral
    float neutral_w_wind_oc_pickup, neutral_w_wind_oc_delay;
//           neutral_w_wind_time_inverse_oc_pickup, neutral_w_wind_time_dial;
//    int neutral_w_wind_curve_style;
//    char neutral_w_wind_electro_reset[3];
//            neutral_w_wind_neu_enable_eq[120];
    // ground
    float ground_w_wind_oc_pickup, ground_w_wind_oc_delay,
            ground_w_wind_time_inverse_oc_pickup, ground_w_wind_time_dial;
    int ground_w_wind_curve_style;
    char ground_w_wind_electro_reset[3];
//            ground_w_wind_neu_enable_eq[120];
    // x-winding settings
    // phase
    float phase_x_wind_oc_pickup, phase_x_wind_oc_delay,
            phase_x_wind_time_inverse_oc_pickup, phase_x_wind_time_dial;
    int phase_x_wind_curve_style;
    char phase_x_wind_electro_reset[3];
//            phase_x_wind_neu_enable_eq[120];
    // neutral
    float neutral_x_wind_oc_pickup, neutral_x_wind_oc_delay;
//            neutral_x_wind_time_inverse_oc_pickup, neutral_x_wind_time_dial;
//    int neutral_x_wind_curve_style;
//    char neutral_x_wind_electro_reset[3];
//            neutral_x_wind_neu_enable_eq[120];
    // ground
    float ground_x_wind_oc_pickup, ground_x_wind_oc_delay,
            ground_x_wind_time_inverse_oc_pickup, ground_x_wind_time_dial;
    int ground_x_wind_curve_style;
    char ground_x_wind_electro_reset[3];
//            ground_x_wind_neu_enable_eq[120];
    // volts/hz settings
    float level1_pickup, level1_time_delay, level2_pickup, level2_time_delay;

} settings_struct;

//
// Globals
//
//
// Used for checking the received data
//
uint16_t loopCounter = 0;
//uint16_t receivedChar;
//volatile uint16_t d = 0;
//volatile uint16_t c = 0;
//volatile uint16_t j = 0;
//volatile uint16_t i = 0;
volatile uint16_t count = 0;

settings_struct settings;
settings_struct *settings_ptr;

//
// Function Prototypes
//

__interrupt void scibTXFIFOISR(void);
__interrupt void scibRXFIFOISR(void);
void initSCIBFIFO(void);
void error(void);
void createSettings(volatile uint16_t rcvd_array[], int length,
                    settings_struct* settings_ptr);
void createSettingsArray(volatile char *send_array, int length,
                         settings_struct* settings_ptr, char* digit_char_array);

#endif /* GROUP6_SCI_H_ */
