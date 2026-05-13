#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "uart.h"
#include "myStepper.h"
#include "display_driver.h"
#include "ArduinoCompat.h"
#include "LiquidCrystal.h"
#include "fremmedlegeme.h"

// ===== LCD =====
LiquidCrystal lcd(53,52,51,50,49,48);

// ===== ADC til effekten=====
volatile uint16_t bat = 0;
volatile uint16_t gen_eff = 0;
volatile uint16_t adc_data = 0;
volatile uint8_t data_klar = 0;

// ===== TIMER ISR =====
ISR(TIMER4_OVF_vect)
{
	// ADC2
	ADMUX = 0b01000010;
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF))) {;}
	adc_data = ADCW;
	ADCSRA |= (1 << ADIF);

	// ADC3
	ADMUX = 0b01000011;
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF))) {;}
	gen_eff = ADCW;
	ADCSRA |= (1 << ADIF);

	data_klar = 1;
	TCNT4 = 31236;
}


int main(void)
{
	opstartBesked();

	InitUART(9600, 8);
	_delay_ms(200);

	if (hentSensorStatus())
	{
		_delay_ms(2000);
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Genstart system!");
		while(1);
	}

	MyStepper motor;
	motor.init();

	char batStr[10];
	char gen_effStr[5];

	_delay_ms(2000);
	visBesked();

	// ===== TIMER + ADC setup =====
	TCNT4 = 31236;
	TCCR4A = 0;
	TCCR4B = 0b00000100;
	TIMSK4 = 0b00000001;
	ADCSRA = 0b11000111;

	// ===== IO =====
	DDRB |= (1 << PB5);
	DDRB |= (1 << PB6);

	sei();

	while (1)
	{
		// ? UART parser
		opdaterSystemFraUART();

		// ? ADC + LCD
		if (data_klar) {
			bat = ((uint32_t)adc_data * 100) / 1023;

			itoa(bat, batStr, 10);
			itoa(gen_eff, gen_effStr, 10);

			opdaterBesked(batStr, gen_effStr);

			data_klar = 0;
		}

		// ? Motor
		if (sidsteKommando == 'U') {
			motor.stepRight();
		}
		else if (sidsteKommando == 'D') {
			motor.stepLeft();
		}
		else {
			motor.stop();
		}
		
		
		SendString("gen_eff: ");
		SendInteger(gen_eff);
		SendString(" ldr: ");
		SendInteger(ldr);
		SendString(" temp: ");
		SendString("\r\n");


		if ((float)gen_eff < ((float)ldr * 0.125f)) {

			if (temp < 25.0) {
				PORTB |= (1 << PB5);
			}
			else {
				PORTB &= ~(1 << PB5);
			}
		}
		else {
			PORTB &= ~(1 << PB5);
		}		
	}
}

