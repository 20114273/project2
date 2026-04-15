/*
 * test_mega_to_esp_uart.c
 *
 * Created: 15/04/2026 15.24.42
 * Author : sebbl
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>

void uart_putc(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Vent på at buffer er tom
	UDR0 = data;
}

void uart_puts(const char* s) {
	while (*s) uart_putc(*s++);
}

int main(void) {
	int counter = 0;
	char buffer[20];

	while (1) {
		// Formater en streng: "Solar: 10", "Solar: 11" osv.
		sprintf(buffer, "Solar: %d\n", counter++);
		
		// Send det ud gennem ledningen til ESP32
		uart_puts(buffer);
		
		_delay_ms(1000); // Vent 1 sekund
	}
}