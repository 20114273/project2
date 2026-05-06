#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum
{
    ADC_CHANNEL_0 = 0, // Potentiometer, PF PIN 0
    ADC_CHANNEL_1 = 1, // Capacitor, PF PIN 1
} adc_channel_t;

void init_adc()
{
    DDRF = 0; // Set PF pins as input for ADC
    PORTF = 0x00;

    ADCSRA |= (1 << ADEN); // Enable ADC
    // ADC prescaler 128 (125 kHz ADC clock with 16 MHz system clock)
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADMUX |= (0 << REFS1) | (1 << REFS0);                 // AVcc reference
    ADCSRB |= (1 << ADTS2) | (0 << ADTS1) | (1 << ADTS0); // Timer/Counter1 Compare Match B
    ADCSRA |= (1 << ADIE);                                // Enable ADC interrupt
    ADCSRA |= (1 << ADATE);                               // Enable auto-triggering
}

const unsigned int ADC_CHANNELS = 2;
unsigned int adc_channel = 0;
unsigned int adc_value_0 = 0;
unsigned int adc_value_1 = 0;

void adc_select_next_channel()
{
    adc_channel = (adc_channel + 1) % ADC_CHANNELS;
    ADMUX = (ADMUX & 0xF0) | (adc_channel & 0x0F); // Select next ADC channel
    ADCSRB |= (adc_channel >> 5) << MUX5;
}

unsigned int get_adc_value(adc_channel_t channel)
{
    switch (channel)
    {
    case ADC_CHANNEL_0:
        return adc_value_0;
    case ADC_CHANNEL_1:
        return adc_value_1;
    default:
        return -1; // Invalid channel
    }
}

unsigned int adc_value_toogle = 0;

/// Frequency of ADC conversion is 125 KHz.
/// Since auto-trigger enabled conversion time takes 13.5 cycles.
/// ADC-conversion time is around 108 us, which gives around 9.2 kHz sampling rate.
ISR(ADC_vect)
{
    adc_value_toogle = !adc_value_toogle;
    // Skip first conversion since it is not stable
    if (adc_value_toogle != 0)
    {
        return;
    }

    unsigned int adc_value = ADCW;
    switch (adc_channel)
    {
    case 0:
        adc_value_0 = adc_value;
        break;
    case 1:
        adc_value_1 = adc_value;
        break;
    default:
        break;
    }

    adc_select_next_channel();
}

unsigned int get_adc_output(adc_output_t output)
{
    switch (output)
    {
    case POTENTIOMETER:
        return get_adc_value(ADC_CHANNEL_0);
    case CAPACITOR:
        return get_adc_value(ADC_CHANNEL_1);
    case CHARGING_CIRCUIT_VOLTAGE_DIVIDER:
        return get_adc_value(ADC_CHANNEL_0);
    case CHARGING_CIRCUIT_VOLTAGE_SHUNT:
        return get_adc_value(ADC_CHANNEL_1);
    default:
        return -1; // Invalid output
    }
}
