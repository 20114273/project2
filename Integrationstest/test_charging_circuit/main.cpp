#include <util/delay.h>
#include "uart.h"
#include "charging_circuit.h"
#include "adc.h"
#include <avr/interrupt.h>

void send_to_terminal(adc_output_t channel, unsigned int value)
{
    char pin = (channel == POTENTIOMETER) ? '0' : '1';

    SendString("ADC");
    SendChar(pin);
    SendString(": ");
    SendInteger(value);
    SendChar('\r');
    SendChar('\n');
}

/// @brief Validate code using Potentiometer and Capacitor ADC values, and adjust PWM signal accordingly.
///
/// Given VOLTAGE_DIVIDER_THRESHOLD_MILLIVOLTAGE + SHUNT_THRESHOLD_MILLIVOLTAGE = 1500,
/// POTENTIOMETER is same channel as CHARGING_CIRCUIT_VOLTAGE_DIVIDER
/// I expect the PWM signal to stop at ADC value
/// Vout = Vin * RESISTOR_4 / (RESISTOR_3 + RESISTOR_4)
/// ADC = Vout / V_REF_MILLIVOLTAGE * ADC_RESOLUTION = 153
///
/// CAPACITOR is same channel as CHARGING_CIRCUIT_VOLTAGE_SHUNT
/// Given THRESHOLD_SHUNT_MILLIVOLTAGE = 2100, I expect to PWM signal to
/// adjust such that ADC value is around 2100 / 5000 * 1024 = 430
/// @return
int main()
{
    sei();

    InitUART(9600, 8);
    init_adc();
    init_timer_1();
    init_timer_3();
    while (1)
    {
        // Voltage Divider
        send_to_terminal(POTENTIOMETER, get_adc_output(POTENTIOMETER));
        send_to_terminal(POTENTIOMETER, get_vin_from_vout(get_adc_output(POTENTIOMETER)));

        // Shunt
        send_to_terminal(CAPACITOR, get_adc_output(CAPACITOR));
        send_to_terminal(CAPACITOR, adc_to_millivoltage(get_adc_output(CAPACITOR)));

        adjust_charging_pulse_signal(adjust_pwm_signal);
        SendString("########\r\n");
        _delay_ms(1000);
    }
    return 0;
}
