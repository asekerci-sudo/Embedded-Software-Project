#include <avr/io.h>
#include <avr/interrupt.h>


#include <stdbool.h>
#include <util/delay.h>

#include "bootcamp/uart.h"
#include "bootcamp/taskQueue.h"
#include "bootcamp/uartBuffer.h"
#include "bootcamp/timer.h"
#define AVR
#define AVR_INTERRUPT


volatile uint8_t data;


ISR(USART_TX_vect){
    uart_transmit_interrupt();
}

ISR(USART_RX_vect){
    uart_receive_interrupt();
}



int main(void){
    size_t buffersize = 20;
    uint8_t* Buffer = malloc(sizeof(uint8_t)*buffersize);
    circularBuffer_t receiveBuffer = circularBuffer_init(Buffer, buffersize); 
    init_uart_predefined(true,103);
    sei();
    while(true){
        if(receiveStartBool()){
            uint8_t receivedValue = receive_uart_char();
            transmit_uart_char(0);
            circularBuffer_push(receiveBuffer,receivedValue); 
            receive_after();
        }
        if(transmitStartBool()){
            transmit_uart_buffer(receiveBuffer);
            transmit_after();
        }
    }
}
