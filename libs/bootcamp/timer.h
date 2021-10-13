#ifndef TIMER_H
#define TIMER_H
#include <inttypes.h>
#include <stdbool.h>
#ifdef AVR
#include <avr/interrupt.h>
#else 
#include "bootcamp/mockInterrupt.h"
#endif

//initialize a timer which increases every 1 ms.
void timerInit(void);

//timer intended for PWM to generate a light fading effect
void sineTimerInitPWM(void);

// increase the timer here
void InterruptTimerIncrease(volatile uint16_t *);

#endif