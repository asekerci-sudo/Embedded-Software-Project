#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_h

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "bootcamp/uart.h"
#include "bootcamp/task.h"
#include "bootcamp/uartBuffer.h"
#include "bootcamp/circularBuffer.h"
#ifdef AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#else
#include "bootcamp/mockRegister.h"
#endif

/*Commands are inserted here accordingly and the output is being transmitted from here too. This is controlled by two flags and one enter "flag"
who looks for if the enter command has been entered.*/
void commandIO(circularBuffer_t , circularBuffer_t);

/*
The commands are parsed part by part here. This return us 4 arrays, which are used as parameters for the taskscheduling.
*/
void commandParsing(bool,circularBuffer_t,circularBuffer_t);

/*
We clear input and output here to maintain the data integrity by resetting the buffer.
*/
void clearBuffers(circularBuffer_t,circularBuffer_t);

/*
Simple Flag to initiate the parsing process.
*/
bool commandEnteredBool(void);

/*
Set to True when commmand has been entered.
*/
void setCommandEnteredTrue(void);

bool setPeriodicBool(char *);

/*
We iterate through names of the functions and compare them with the function pointer to check if input is valid. If it is, we insert it into the scheduler 
and the command begins to execute one time or periodically.
*/
void addingCommandtoSchedule(void);

/*
Set to False when commmand has been entered.
*/
void setCommandEnteredFalse(void);


void setCommandErrorFalse(void);

void setCommandErrorTrue(void);
#endif