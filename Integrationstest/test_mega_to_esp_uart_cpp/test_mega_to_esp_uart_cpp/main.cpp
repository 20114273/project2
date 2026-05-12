#include <avr/io.h>
#include "uart.h"
#include "myStepper.h"
#include "display_driver.h"
#include "ArduinoCompat.h"
#include "LiquidCrystal.h"
#include <avr/interrupt.h>
#include <stdlib.h>

int main(void) {
	// Initialisering
	InitUART(9600, 8);
	
	MyStepper motor;
	motor.init();

	// Vi opretter en variabel til at huske, hvad motoren skal gøre
	char sidsteKommando = 'S';

	while (1) {
		// 1. Tjek om der er kommet NY data fra ESP32
		if (CharReady()) {
			char modtagetData = ReadChar();
			
			// Echo til Tera Term (valgfrit, men godt til fejlfinding)
			SendChar(modtagetData);

			// Opdater kun kommandoen, hvis det er et af vores kontrol-tegn
			if (modtagetData == 'U' || modtagetData == 'D' || modtagetData == 'S') {
				sidsteKommando = modtagetData;
			}
		}

		// 2. Udfør handlingen baseret på den SIDSTE kendte kommando
		// Dette sker nu hver eneste gang løkken kører, uafhængigt af UART-hastighed
		if (sidsteKommando == 'U') {
			motor.stepRight();
		}
		else if (sidsteKommando == 'D') {
			motor.stepLeft();
		}
		else if (sidsteKommando == 'S') {
			motor.stop();
		}
	}
}