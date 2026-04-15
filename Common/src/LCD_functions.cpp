#include "LiquidCrystal.h"
#include "ArduinoCompat.h"

// Vi definerer objektet her, uden for funktionerne, så det kan huskes
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcd_setup_task() {
	lcd.begin(16, 2);
	lcd.print("System Ready");
}

void lcd_update_status(bool led_on) {
	if (led_on) {
		lcd.noBlink();
		// Eventuelt lcd.setCursor(0,1); lcd.print("LED is ON ");
		} else {
		lcd.blink();
		// Eventuelt lcd.setCursor(0,1); lcd.print("LED is OFF");
	}
}