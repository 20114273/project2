#pragma once

typedef enum
{
    PWM_SIGNAL_DECREASE = 0,
    PWM_SIGNAL_INCREASE = 1,
    PWM_STOP = 2,
    PWM_SYMMETRIC = 3
} pwm_signal_adjustment_t;

void init_timer_1();
void init_timer_3();
void adjust_pwm_signal(pwm_signal_adjustment_t adjustment);
