#pragma once

#define ADC_RESOLUTION 1024U

typedef enum
{
    POTENTIOMETER = 0,
    CAPACITOR = 1,
    CHARGING_CIRCUIT_VOLTAGE_DIVIDER = 2,
    CHARGING_CIRCUIT_VOLTAGE_SHUNT = 3
} adc_output_t;

void init_adc();
unsigned int get_adc_output(adc_output_t output);
