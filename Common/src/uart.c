/************************************************
 * "uart.c":                                     *
 * Implementation file for Mega2560 UART driver. *
 * Using UART 0.                                 *
 *                                               *
 *************************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h> // Tilf�jet til delays
#include "../lib/uart.h"

// F_CPU som standard, da avr-libc bibliotekerne kigger efter den
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// XTAL = F_CPU s� BaudRate-beregning stadig virker
#define XTAL F_CPU

void InitUART(unsigned long BaudRate, unsigned char DataBit)
{
	// Beregn UBRR værdi for 16MHz (Normal mode, ikke Double Speed)
	// Vi bruger 16.0 som float for at sikre præcision i beregningen
	unsigned int ubrr = (unsigned int)(16000000UL / (16UL * BaudRate) - 1);
	
	// Sæt Baud rate registre
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	// Enable modtager og sender
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Sæt format: 8-bit, 1 stop bit, ingen paritet
	// (DataBit - 5) << 1 er smart, men for 8-bit er det sikrest at skrive:
	UCSR0C = (3 << UCSZ00);
}

/*************************************************************************
  Returns 0 (FALSE), if the UART has NOT received a new character.
  Returns value 1 (TRUE), if the UART HAS received a new character.
*************************************************************************/
unsigned char CharReady()
{
   return (UCSR0A & (1 << RXC0));
}

/*************************************************************************
Awaits new character received.
Then this character is returned.
*************************************************************************/
char ReadChar()
{
   while (!CharReady())
      ;
   return UDR0;
}

/*************************************************************************
Awaits transmitter register ready.
Then send the character.
Parameter :
   Tegn : Character for sending.
*************************************************************************/
void SendChar(char Tegn)
{
   while (!(UCSR0A & (1 << UDRE0)))
      ;
   UDR0 = Tegn;
}

/*************************************************************************
Sends 0 terminated string.
Parameter:
   Streng: Pointer to the string.
*************************************************************************/
void SendString(const char *Streng)
{
   for (; *Streng != 0; Streng++)
   {
      SendChar(*Streng);
   }
}

void SendInteger(unsigned int value)
{
   char buffer[10];
   itoa(value, buffer, 10);
   SendString(buffer);
}
