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

volatile uint8_t varme_aktiv = 0;
volatile uint16_t varme_timer = 0;

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
    // ===== 1. Boot-sekvens (Stabilisering) =====
    _delay_ms(2000); // Giv systemet ro til at starte op
    
    opstartBesked(); // Vis "Starter system.."
    InitUART(9600, 8);
    
    // Tøm bufferen for opstarts-støj fra ESP'en
    while(CharReady()) ReadChar();

    // ===== 2. Sensortjek =====
	
    if (hentSensorStatus())
    {
        _delay_ms(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Genstart system!");
        while(1); // Lås systemet her ved kritisk fejl
    }

    // ===== 3. Hardware Initialisering =====
    MyStepper motor;
    motor.init();

    char batStr[10];
    char gen_effStr[5];

    _delay_ms(1000);
    visBesked(); // Skriver "Batteri:" og "Gen.eff:" på LCD

    // ===== 4. Timer & ADC setup (Ikke-blokerende) =====
    TCNT4 = 31236;
    TCCR4A = 0;
    TCCR4B = 0b00000100; // Prescaler 256 (~0.5 sek interval)
    TIMSK4 = 0b00000001;
    ADCSRA = 0b11000111; // Tænd ADC med prescaler 128

    // Setup af IO til fremmedlegeme-logik (PB5 og PB6)
    DDRB |= (1 << PB5);
	DDRB |= (1 << PB6);
	
    PORTB &= ~(1 << PB5); // Start med at have den slukket
	PORTB &= ~(1 << PB6); // Start med at have den slukket
	
    sei(); // Aktiver interrupts

    while (1)
    {
	    // A. Hent NYESTE data fra ESP32
	    opdaterSystemFraUART();

	    // B. Opdater LCD og håndtér tidsbaseret logik (Hver 0.5 sek)
	    if (data_klar) {
		    bat = ((uint32_t)adc_data * 100) / 1023;
		    itoa(bat, batStr, 10);
		    itoa(gen_eff, gen_effStr, 10);
		    opdaterBesked(batStr, gen_effStr);

		    // --- NY LOGIK: Tæller til fremmedlegeme-tjek ---
		    if (varme_aktiv) {
			    varme_timer++;
		    }

		    data_klar = 0; // Nulstil flag
	    }

	    // C. Motor-styring (Kører nu flydende uden pauser!)
	    if (sidsteKommando == 'U') motor.stepRight();
	    else if (sidsteKommando == 'D') motor.stepLeft();
	    else motor.stop();

	    // D. Logik for fremmedlegeme (Uden blokerende delay)
	    float threshold = (float)ldr * 0.125f;

	    if ((float)gen_eff < threshold) {
		    
		    if (temp > 22.0) {
			    // Øjeblikkelig fejl: Varmt + ingen strøm
			    PORTB |= (1 << PB6);   // Fremmedlegeme ON
			    PORTB &= ~(1 << PB5);  // Varme OFF
			    varme_aktiv = 0;
		    }
		    else if (!varme_aktiv) {
			    // Vi har ikke startet varmen endnu - start nu
			    PORTB |= (1 << PB5);   // Varme ON
			    varme_aktiv = 1;
			    varme_timer = 0;       // Start tidtagning
		    }
		    
		    // Tjek om der er gået 10 sekunder (20 ticks a 0.5 sek)
		    if (varme_aktiv && varme_timer >= 20) {
			    if ((float)gen_eff < threshold) {
				    PORTB |= (1 << PB6);  // Stadig ingen strøm -> Fremmedlegeme!
				    PORTB &= ~(1 << PB5); // Sluk varme
			    }
			    // Vi lader varme_aktiv være 1, så den ikke "genstarter" opvarmningen hver gang
		    }
	    }
	    else {
		    // Alt normalt - Nulstil alt
		    PORTB &= ~(1 << PB6);
		    PORTB &= ~(1 << PB5);
		    varme_aktiv = 0;
		    varme_timer = 0;
	    }
    }
}
