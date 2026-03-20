#ifndef ARDUINO_COMPAT_H
#define ARDUINO_COMPAT_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stddef.h>

#define HIGH 0x1
#define LOW 0x0
#define OUTPUT 0x1
#define INPUT 0x0

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

#endif
