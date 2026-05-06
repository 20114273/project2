#include <stdio.h>
#include "charging_circuit.h"
#include "adc.h"

static pwm_signal_adjustment_t last_adjustment = PWM_SYMMETRIC;

void adjust_pwm_signal(pwm_signal_adjustment_t adjustment)
{
    last_adjustment = adjustment;
}

static unsigned int CHARGING_CIRCUIT_VOLTAGE_DIVIDER_VALUE = 0;
static unsigned int CHARGING_CIRCUIT_VOLTAGE_SHUNT_VALUE = 0;

unsigned int get_adc_output(adc_output_t output)
{
    switch (output)
    {
    case CHARGING_CIRCUIT_VOLTAGE_DIVIDER:
        return CHARGING_CIRCUIT_VOLTAGE_DIVIDER_VALUE;
    case CHARGING_CIRCUIT_VOLTAGE_SHUNT:
        return CHARGING_CIRCUIT_VOLTAGE_SHUNT_VALUE;
    default:
        return -1; // Invalid output
    }
}

void set_shunt_millivoltage_threshold_adc(unsigned int shunt_millivoltage)
{
    CHARGING_CIRCUIT_VOLTAGE_SHUNT_VALUE = shunt_millivoltage * SHUNT_GAIN * ADC_RESOLUTION / V_REF_MILLIVOLTAGE;
}

void set_voltage_divider_threshold_adc(unsigned int voltage_divider_in_millivoltage)
{
    unsigned int voltage_divider_vout_millivoltage = voltage_divider_in_millivoltage * RESISTOR_4 / RESISTOR_SUM;
    CHARGING_CIRCUIT_VOLTAGE_DIVIDER_VALUE = voltage_divider_vout_millivoltage * ADC_RESOLUTION / V_REF_MILLIVOLTAGE;
}

void reset_charging_state()
{
    set_voltage_divider_threshold_adc(TOTAL_VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE + 50); // Set state to not charging
    adjust_charging_pulse_signal(adjust_pwm_signal);
    set_voltage_divider_threshold_adc(TOTAL_VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE);
    adjust_charging_pulse_signal(adjust_pwm_signal);
}

int test_when_above_voltage_divider_threshold_then_stop_pwm()
{
    // Arrange
    reset_charging_state();
    set_voltage_divider_threshold_adc(TOTAL_VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE + 50);
    last_adjustment = PWM_SIGNAL_INCREASE;

    // Act
    adjust_charging_pulse_signal(adjust_pwm_signal);

    // Assert
    if (last_adjustment != PWM_STOP)
    {
        return -1;
    }
    return 0;
}

int test_given_prior_no_charging_state_when_below_voltage_divider_threshold_then_symmetric_pwm()
{
    // Arrange
    set_voltage_divider_threshold_adc(TOTAL_VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE + 50); // Set state to not charging
    adjust_charging_pulse_signal(adjust_pwm_signal);
    set_voltage_divider_threshold_adc(TOTAL_VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE);

    // Act
    adjust_charging_pulse_signal(adjust_pwm_signal);

    // Assert
    if (last_adjustment != PWM_SYMMETRIC)
    {
        printf("Expected PWM_SYMMETRIC but got %d\n", last_adjustment);
        return -1;
    }
    return 0;
}

int test_when_above_shunt_threshold_then_decrease_pwm()
{
    // Arrange
    reset_charging_state();
    set_shunt_millivoltage_threshold_adc(SHUNT_THRESHOLD_MILLIVOLTAGE + 100);

    // Act
    adjust_charging_pulse_signal(adjust_pwm_signal);

    // Assert
    if (last_adjustment != PWM_SIGNAL_DECREASE)
    {
        printf("Expected PWM_SIGNAL_DECREASE but got %d\n", last_adjustment);
        return -1;
    }
    return 0;
}

int test_when_below_shunt_threshold_then_increase_pwm()
{
    // Arrange
    reset_charging_state();
    set_shunt_millivoltage_threshold_adc(SHUNT_THRESHOLD_MILLIVOLTAGE - 100);

    // Act
    adjust_charging_pulse_signal(adjust_pwm_signal);

    // Assert
    if (last_adjustment != PWM_SIGNAL_INCREASE)
    {
        printf("Expected PWM_SIGNAL_INCREASE but got %d\n", last_adjustment);
        return -1;
    }
    return 0;
}

int main()
{
    if (test_when_above_voltage_divider_threshold_then_stop_pwm() != 0)
    {
        printf("FAIL: test_when_above_voltage_divider_threshold_then_stop_pwm\n");
        return -1;
    }

    if (test_given_prior_no_charging_state_when_below_voltage_divider_threshold_then_symmetric_pwm() != 0)
    {
        printf("FAIL: test_given_prior_no_charging_state_when_below_voltage_divider_threshold_then_symmetric_pwm\n");
        return -1;
    }

    if (test_when_above_shunt_threshold_then_decrease_pwm() != 0)
    {
        printf("FAIL: test_when_above_shunt_threshold_then_decrease_pwm\n");
        return -1;
    }

    if (test_when_below_shunt_threshold_then_increase_pwm() != 0)
    {
        printf("FAIL: test_when_below_shunt_threshold_then_increase_pwm\n");
        return -1;
    }

    printf("PASS\n");
    return 0;
}
