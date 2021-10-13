#ifndef TEMPERATURE
#define TEMPERATURE
#include <inttypes.h>
#include <stdbool.h>
#ifdef AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#else
#include  "bootcamp/mockTemperature.h"
#endif

// initialize the acd module to return the system temperature by interrupts or polling.
void ACDInit(bool);

//fetch temperature by setting register.
uint8_t getTemperature(void);

// get Temperature out of the 16 bit ACDW register and convert it into the correc format by subtracting and multiplying it.
void ACDStart(void);


#endif