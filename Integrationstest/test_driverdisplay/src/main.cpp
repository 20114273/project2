/*
 * main.cpp
 *
 * Created: 06/05/2026 16:44:44
 * Author : Emil
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "display_driver.h"
#include "switch.h"
#include "ArduinoCompat.h"
#include "LiquidCrystal.h"

LiquidCrystal lcd(53,52,51,50,49,48); //Initialiserer LCD-objektet med de pins der er forbundet til LCD-displayet

volatile uint16_t bat = 0;
volatile uint16_t gen_eff = 0;
volatile uint16_t adc_data = 0;
volatile uint8_t data_klar = 0;

ISR(TIMER4_OVF_vect) { //Interrupt service routine for timer 4 overflow interrupt
  ADMUX = 0b01000010; //Skifter ADC input til ADC2 og starter konvertering

  ADCSRA |= (1 << ADSC); //Starter ADC konvertering

  while (!(ADCSRA & (1 << ADIF))) {
    ; // vent på konvertering er færdig
  }
  adc_data = ADCW;
  ADCSRA |= (1 << ADIF); //Ryd ADC interrupt flag
    
  ADMUX = 0b01000011; //Skifter ADC input til ADC3 og starter konvertering

  ADCSRA |= (1 << ADSC); //Starter ADC konvertering

  while (!(ADCSRA & (1 << ADIF))) {
    ; // venter på konvertering er færdig
  }

  gen_eff = ADCW;

  ADCSRA |= (1 << ADIF); //Ryd ADC interrupt flag
  
  data_klar = 1; //Sætter flag for at data er klar til at blive vist på LCD-displayet. Bruges i main.
  TCNT4 = 31236;
};



int main(void)
{
  //Variabler til værdier der skal til display.
  char batStr[10];
  char gen_effStr[5];

  opstartBesked();
  initSwitchPort(); //Bruges som simulering af start signal fra systemet.
  while (!switchOn(0)){
		; //Venter på signal fra systemet er klar (her sw0 på shield) Skulle gerne være et interrupt signal.
	};

	visBesked();

  TCNT4 = 31236; //Sætter timer 4 til at tælle ned fra 31236.
  TCCR4A = 0; //Sætter timer 4 til normal mode.
  TCCR4B = 0b00000100; //Sætter timer 4 til at tælle ned med en prescaler på 256. Dette giver et delay på 0,5 sekunder.
  TIMSK4 = 0b00000001; //Aktiverer timer 4 overflow interrupt.
  ADCSRA = 0b11000111; //Tænd ADC og sæt prescaler til 128. Frekvens på 125 kHz.

  sei(); 

  while (1) {

    if (data_klar) {
      bat = ((uint32_t)adc_data * 100) / 1023; //Omregner ADC værdien til en procentværdi

      itoa(bat, batStr, 10); //Konverterer batteriniveauet til en const char*
      itoa(gen_eff, gen_effStr, 10); //Konverterer generator effekt til en const char*

      opdaterBesked(batStr, gen_effStr); //Opdaterer LCD-displayet med de nye værdier

      data_klar = 0;
    }
    
  }
}

