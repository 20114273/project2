#include "charging_circuit.h"
#include "adc.h"

typedef enum
{
    NOT_CHARGING = 0,
    CHARGING = 1
} charging_state_t;

static charging_state_t charging_state = NOT_CHARGING;

static const unsigned int THRESHOLD_SHUNT_MILI_VOLTAGE = SHUNT_THRESHOLD_MILLIVOLTAGE * SHUNT_GAIN;

unsigned int adc_to_millivoltage(unsigned int adc_value)
{
    return (unsigned long)adc_value * V_REF_MILLIVOLTAGE / ADC_RESOLUTION;
}

// Use inverse of voltage divider formula to get Vin from Vout
unsigned int get_vin_from_vout(unsigned int voltage_divider_measure)
{
    unsigned int voltage_divider_vout_mili_voltage = adc_to_millivoltage(voltage_divider_measure);
    return (unsigned long)voltage_divider_vout_mili_voltage * RESISTOR_SUM / RESISTOR_4;
}

void adjust_charging_pulse_signal(void (*pwm_adjustment)(pwm_signal_adjustment_t))
{
    unsigned int voltage_divider_measure = get_adc_output(CHARGING_CIRCUIT_VOLTAGE_DIVIDER);

    unsigned int voltage_divider_vin_mili_voltage = get_vin_from_vout(voltage_divider_measure);

    // Stop if voltage above threshold
    if (voltage_divider_vin_mili_voltage > TOTAL_VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE)
    {
        pwm_adjustment(PWM_STOP);
        charging_state = NOT_CHARGING;
        return;
    }
    // When below threshold and prior not charging, start charging with symmetric signal
    if (charging_state == NOT_CHARGING)
    {
        pwm_adjustment(PWM_SYMMETRIC);
        charging_state = CHARGING;
        return;
    }

    unsigned int voltage_shunt_measure = get_adc_output(CHARGING_CIRCUIT_VOLTAGE_SHUNT);
    unsigned int voltage_shunt_mili_voltage = adc_to_millivoltage(voltage_shunt_measure);

    if (voltage_shunt_mili_voltage > THRESHOLD_SHUNT_MILI_VOLTAGE)
    {
        // If voltage is above threshold, decrease PWM signal
        pwm_adjustment(PWM_SIGNAL_DECREASE);
    }
    else if (voltage_shunt_mili_voltage < THRESHOLD_SHUNT_MILI_VOLTAGE)
    {
        // If voltage is below threshold, increase PWM signal
        pwm_adjustment(PWM_SIGNAL_INCREASE);
    }
}
