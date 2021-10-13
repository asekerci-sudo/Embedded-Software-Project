#ifndef UART_H
#define UART_H
#include "inttypes.h"
#include "stdbool.h"
#ifdef AVR

#include <avr/io.h>
#include <avr/interrupt.h>
#else
#include "bootcamp/mockRegister.h"
#endif

//initialize uart with an interrupt and polling mode. parity and such are predefined.
void init_uart_predefined(bool,unsigned int);

// transmit module of the uart interface to send out integer hex data
void transmit_uart(uint8_t data);

// transmit module of the uart interface to send out char data
void transmit_uart_char(unsigned char data);

// receive module of the uart interface to receive data through a serial interface.
uint8_t receive_uart(void);

// receive and transmit and flush are mostly similar as described in the Arduino datasheet.

// flush the UDR0 register to reset left out data.
void flush_uart(void);


// These are interrupt flag functions to control the transmit and receive flow. Simple Boolean change operations.
void receive_interrupt_enable(void);

void transmit_complete_enable(void);

void udre_interrupt_enable(void);

void udre_interrupt_disable(void);

void transmit_after(void);

void receive_after(void);

unsigned char receive_uart_char(void);

void receive_interrupt_disable(void);

void uart_transmit_interrupt(void);

void uart_receive_interrupt(void);

void transmit_uart_complete(void);

void receive_uart_complete(void);

// check if receive and transmit can be started through flag comparison
bool receiveStartBool(void);

bool transmitStartBool(void);



#endif