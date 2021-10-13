#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "bootcamp/uart.h"
#define AVR
#define AVR_INTERRUPT

int main (void){
    init_uart_predefined(false,103);
    uint8_t receivedValue = 1;
    while(receivedValue != 0){
        receivedValue = receive_uart_char();
        transmit_uart(receivedValue);
    }
    flush_uart();
    return 0;
}
