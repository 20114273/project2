#include "../lib/MyStepper.h"
#include <util/delay.h>

MyStepper::MyStepper() {}

void MyStepper::init() {
	// Motor pins (ULN2003) på Port A
	DDRA |= (1 << PA0) | (1 << PA2) | (1 << PA4) | (1 << PA6);
}

void MyStepper::stop() {
	setMotor(0, 0, 0, 0);
}

void MyStepper::setMotor(int a, int b, int c, int d) {
	// Rydder pins PA0, PA2, PA4, PA6
	PORTA &= ~((1 << PA0) | (1 << PA2) | (1 << PA4) | (1 << PA6));

	if (a) PORTA |= (1 << PA0);
	if (b) PORTA |= (1 << PA2);
	if (c) PORTA |= (1 << PA4);
	if (d) PORTA |= (1 << PA6);
}

void MyStepper::stepRight() {
	setMotor(1,0,0,0); _delay_ms(1);
	setMotor(1,1,0,0); _delay_ms(1);
	setMotor(0,1,0,0); _delay_ms(1);
	setMotor(0,1,1,0); _delay_ms(1);
	setMotor(0,0,1,0); _delay_ms(1);
	setMotor(0,0,1,1); _delay_ms(1);
	setMotor(0,0,0,1); _delay_ms(1);
	setMotor(1,0,0,1); _delay_ms(1);
}

void MyStepper::stepLeft() {
	setMotor(1,0,0,1); _delay_ms(1);
	setMotor(0,0,0,1); _delay_ms(1);
	setMotor(0,0,1,1); _delay_ms(1);
	setMotor(0,0,1,0); _delay_ms(1);
	setMotor(0,1,1,0); _delay_ms(1);
	setMotor(0,1,0,0); _delay_ms(1);
	setMotor(1,1,0,0); _delay_ms(1);
	setMotor(1,0,0,0); _delay_ms(1);
}