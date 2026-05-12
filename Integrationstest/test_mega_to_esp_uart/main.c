#include "../../Common/lib/uart.h"
#include <avr/io.h>

int main(void) {
	InitUART(9600, 8);
	DDRL |= (1 << PL7); // LED7 på shieldet

	while (1) {
		// Vi bruger din ReadChar() som venter på data
		char c = ReadChar();
		
		// Hver gang et tegn modtages, skifter LED7 tilstand
		PORTL ^= (1 << PL7);
		
		// Send det modtagne tegn tilbage (Echo), så du kan se det i Tera Term
		SendChar(c);
	}
}