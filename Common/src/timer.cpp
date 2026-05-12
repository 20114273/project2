#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int FREQ = 5000;
unsigned int PRESCALER = 1024;

void init_timer_1()
{
    TIMSK1 |= (1 << OCIE1B);             // Enable Timer1 compare match interrupt
    TCCR1B |= (1 << WGM12);              // CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // Start Timer1 with prescaler 1024

    OCR1A = F_CPU / PRESCALER / FREQ; // Set compare value for FREQ overflows per second
}

const unsigned long TIMER3_FERQUENCY = 100000;
const unsigned int TIMER3_PRESCALER = 1;
const unsigned int TIMER3_MAX = F_CPU / (TIMER3_PRESCALER * TIMER3_FERQUENCY) - 1;

void init_timer_3()
{
    DDRE |= (1 << 3); // OC3A som output

    // Fast PWM, Mode 14 (ICR3 som TOP)
    TCCR3A |= (1 << WGM31);
    TCCR3B |= (1 << WGM33) | (1 << WGM32);

    // Non-inverting PWM
    TCCR3A |= (1 << COM3A1);

    // Prescaler = 1
    TCCR3B |= (1 << CS30);

    // TOP for 100 kHz
    ICR3 = TIMER3_MAX;

    // 50% duty cycle
    OCR3A = TIMER3_MAX / 2;
}

ISR(TIMER1_COMPB_vect)
{
}

void adjust_pwm_signal(pwm_signal_adjustment_t adjustment)
{
    switch (adjustment)
    {
    case PWM_SIGNAL_DECREASE:
        if (OCR3A != 0)
        {
            OCR3A--;
        }
        break;
    case PWM_SIGNAL_INCREASE:
        if (OCR3A != TIMER3_MAX)
        {
            OCR3A++;
        }
        break;
    case PWM_STOP:
        OCR3A = 0;
        break;
    case PWM_SYMMETRIC:
        OCR3A = TIMER3_MAX / 2;
        break;
    default:
        break;
    }
}
