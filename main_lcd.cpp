// https://github.com/arduino-libraries/LiquidCrystal/blob/master/src/LiquidCrystal.h
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "ArduinoCompat.h"
#include "LiquidCrystal.h"
#include "uart.h"

int main(void)
{
    const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
    LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

    lcd.begin(16, 2);
    lcd.print("hello, world!");

    InitUART(9600, 8);

    DDRB |= 1 << PB7;

    while (true)
    {
        lcd.noBlink();
        PORTB |= 1 << PB7;
        SendString("On\r\n");
        delay(3000);

        lcd.blink();
        PORTB &= ~(1 << PB7);
        SendString("Off\r\n");
        delay(3000);
    }
}
