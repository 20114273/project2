#ifndef MYSTEPPER_H
#define MYSTEPPER_H

#include <avr/io.h>

class MyStepper {
	public:
	MyStepper();
	void init();
	void stepRight();
	void stepLeft();
	void stop();
	
	private:
	void setMotor(int a, int b, int c, int d);
};

#endif