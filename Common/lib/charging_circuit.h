#pragma once

#include "timer.h"

#define V_REF_MILLIVOLTAGE 5000U

#define SHUNT_GAIN 21U
#define SHUNT_THRESHOLD_MILLIVOLTAGE 100U

#define VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE 1400U
#define TOTAL_VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE (VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE + SHUNT_THRESHOLD_MILLIVOLTAGE)
#define RESISTOR_3 100000UL // 100 kOhm
#define RESISTOR_4 100000UL // 100 kOhm
#define RESISTOR_SUM (RESISTOR_3 + RESISTOR_4)

unsigned int adc_to_millivoltage(unsigned int adc_value);
unsigned int get_vin_from_vout(unsigned int voltage_divider_measure);
void adjust_charging_pulse_signal(void (*pwm_adjustment)(pwm_signal_adjustment_t));
