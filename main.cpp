#include "uart.h"
#include "ArduinoCompat.h"

// Prototyper
void lcd_setup_task();
void lcd_update_status(bool led_on);

int main(void) {
	// INITIALISERING
	InitUART(9600, 8);
	lcd_setup_task(); // Kalder LCD-opstart
	DDRB |= 1 << PB7;

	while (1) {
		// Tænd LED og opdater LCD
		PORTB |= 1 << PB7;
		lcd_update_status(true);
		delay(3000);

		// Sluk LED og opdater LCD
		PORTB &= ~(1 << PB7);
		lcd_update_status(false);
		delay(3000);
	}
}