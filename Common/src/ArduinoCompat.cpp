#include "ArduinoCompat.h"

// Arduino Mega 2560 pin mapping used here:
// D12 -> PB6
// D11 -> PB5
// D5  -> PE3
// D4  -> PG5
// D3  -> PE5
// D2  -> PE4

// På Mega-boardets digitale header:
//
// RS  sidder på pin 53
// EN  sidder på pin 52
// D4  sidder på pin 51
// D5  sidder på pin 50
// D6  sidder på pin 49
// D7  sidder på pin 48

static volatile uint8_t *pin_to_port(uint8_t pin)
{
    switch (pin)
    {
    case 53:
        return &PORTB;
    case 52:
        return &PORTB;
    case 51:
        return &PORTB;
    case 50:
        return &PORTB;
    case 49:
        return &PORTL;
    case 48:
        return &PORTL;
    default:
        return 0;
    }
}

static volatile uint8_t *pin_to_ddr(uint8_t pin)
{
    switch (pin)
    {
    case 53:
        return &DDRB;
    case 52:
        return &DDRB;
    case 51:
        return &DDRB;
    case 50:
        return &DDRB;
    case 49:
        return &DDRL;
    case 48:
        return &DDRL;
    default:
        return 0;
    }
}

static uint8_t pin_to_bit(uint8_t pin)
{
    switch (pin)
    {
    case 53:
        return PB0;
    case 52:
        return PB1;
    case 51:
        return PB2;
    case 50:
        return PB3;
    case 49:
        return PL0;
    case 48:
        return PL1;
    default:
        return 0;
    }
}

void pinMode(uint8_t pin, uint8_t mode)
{
    volatile uint8_t *ddr = pin_to_ddr(pin);
    uint8_t bit = pin_to_bit(pin);

    if (!ddr)
        return;

    if (mode == OUTPUT)
        *ddr |= (1 << bit);
    else
        *ddr &= ~(1 << bit);
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    volatile uint8_t *port = pin_to_port(pin);
    uint8_t bit = pin_to_bit(pin);

    if (!port)
        return;

    if (value == HIGH)
        *port |= (1 << bit);
    else
        *port &= ~(1 << bit);
}

void delay(unsigned long ms)
{
    while (ms--)
    {
        _delay_ms(1);
    }
}

void delayMicroseconds(unsigned int us)
{
    while (us--)
    {
        _delay_us(1);
    }
}
