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
// RS  sidder på pin 12
// EN  sidder på pin 11
// D4  sidder på pin 5
// D5  sidder på pin 4
// D6  sidder på pin 3
// D7  sidder på pin 2

static volatile uint8_t *pin_to_port(uint8_t pin)
{
    switch (pin)
    {
    case 12:
        return &PORTB;
    case 11:
        return &PORTB;
    case 5:
        return &PORTE;
    case 4:
        return &PORTG;
    case 3:
        return &PORTE;
    case 2:
        return &PORTE;
    default:
        return 0;
    }
}

static volatile uint8_t *pin_to_ddr(uint8_t pin)
{
    switch (pin)
    {
    case 12:
        return &DDRB;
    case 11:
        return &DDRB;
    case 5:
        return &DDRE;
    case 4:
        return &DDRG;
    case 3:
        return &DDRE;
    case 2:
        return &DDRE;
    default:
        return 0;
    }
}

static uint8_t pin_to_bit(uint8_t pin)
{
    switch (pin)
    {
    case 12:
        return PB6;
    case 11:
        return PB5;
    case 5:
        return PE3;
    case 4:
        return PG5;
    case 3:
        return PE5;
    case 2:
        return PE4;
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
