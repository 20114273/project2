/************************************************
 * "uart.c":                                     *
 * Implementation file for Mega2560 UART driver. *
 * Using UART 0.                                 *
 *                                               *
 *************************************************/
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

// Target CPU frequency
#define XTAL 16000000UL

void InitUART(unsigned long BaudRate, unsigned char DataBit)
{
   if (BaudRate < 300 || BaudRate > 115200 || DataBit < 5 || DataBit > 8)
   {
      return;
   }
   UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
   UCSR0C |= (DataBit - 5) << 1;
   UBRR0 = XTAL / (16 * BaudRate) - 1;
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
