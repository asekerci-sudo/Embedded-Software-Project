#include "bootcamp/uartBuffer.h"

//circularBuffer_t cbuffer;

void transmit_uart_buffer(circularBuffer_t cbuf){
    uint8_t data;
    if(circularBuffer_read(cbuf,&data) != -1){
        transmit_uart_char(data);
    }
}

void receive_uart_buffer(circularBuffer_t cbuf){
    circularBuffer_push(cbuf, receive_uart_char());
}

void receive_uart_buffer_overwrite(circularBuffer_t cbuf){
    uint8_t Data = receive_uart();
    circularBuffer_overwrite(cbuf, Data);
}