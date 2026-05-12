/************************************
* "Switch.c"                        *
* Implementation for SWITCH driver. *
* Version: MA                       *
************************************/
#include <avr/io.h>
#define MAXSwitchNr 7
#include "switch.h"

//Klarg�r switchport p� port A
void initSwitchPort()
{
	DDRA = 0x00;
}

// L�ser alle switches samtidigt
unsigned char switchStatus()
{
	return ~PINA;
}

// Returnerer TRUE, hvis switchen med nummeret
// "switch_nr" er aktiveret - ellers returneres FALSE
unsigned char switchOn(unsigned char switch_nr)
{
	if (switch_nr <= MAXSwitchNr){
		unsigned char mask = 0b00000001;
		if (switchStatus() & (mask << switch_nr)){
			return 1;	
		}
	}
	return 0;
}