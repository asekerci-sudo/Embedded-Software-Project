#ifndef UARTWITHBUFFER_H
#define UARTWITHBUFFER_H

#include <inttypes.h>
#include <bootcamp/uart.h>
#include <bootcamp/circularBuffer.h>


// transmit from circularBuffer by checking first if data is existing and then sending it out from the buffer
void transmit_uart_buffer(circularBuffer_t cbuf);

//Simply pipelining the received Value into the Buffer by a push operation.
void receive_uart_buffer(circularBuffer_t cbuf);

//Same as receive_uart_buffer but overwriting instead of pushing. Both can be used according to need.
void receive_uart_buffer_overwrite(circularBuffer_t cbuf);
#endif