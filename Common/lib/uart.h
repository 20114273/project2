/****************************************
 * "uart_int.h":                         *
 * Header file for Mega2560 UART driver. *
 * Using UART 0.                         *
 * If parameter Rx_Int <> 0 :            *
 * Receiver interrupt will be enabled    *
 *                                       *
 *****************************************/
#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C"
{
#endif

    void InitUART(unsigned long BaudRate, unsigned char DataBit);
    unsigned char CharReady();
    char ReadChar();
    void SendChar(char Tegn);
    void SendString(const char *Streng);

#ifdef __cplusplus
}
#endif

#endif
/****************************************/
