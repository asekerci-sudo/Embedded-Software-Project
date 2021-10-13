#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "bootcamp/uart.h"
#include "bootcamp/uartBuffer.h"
#include "bootcamp/circularBuffer.h"
#define AVR
#define AVR_INTERRUPT

int main (void){
    size_t buffersize = 5;
    uint8_t* Buffer = malloc(sizeof(uint8_t)*buffersize);
    circularBuffer_t receiveBuffer = circularBuffer_init(Buffer, buffersize); 
    init_uart_predefined(false,103);
    while(true){
        receive_uart_buffer(receiveBuffer);
        if(circularBuffer_full(receiveBuffer)){
            while(!(circularBuffer_empty(receiveBuffer))){
                transmit_uart_buffer(receiveBuffer);
            }
        }
    }   
    flush_uart();
    circularBuffer_free(receiveBuffer);
    free(Buffer);
    return 0;
}