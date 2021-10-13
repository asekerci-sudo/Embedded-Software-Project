#include "bootcamp/uart.h"


bool interruptEnabled = false;
bool receiveComplete;
bool transmitComplete;
bool receiveStart;
bool transmitStart;


void init_uart_predefined(bool interruptMode,unsigned int ubrr){

    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char) ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = 3<<UCSZ00; 
    if(interruptMode == true){
        UCSR0B |= (1<<RXCIE0) | (1<<TXCIE0);
    }
    interruptEnabled = interruptMode;
    transmitStart = true;
    receiveStart = false;
    transmitComplete = true;

}

void transmit_uart(uint8_t data){
   if(!interruptEnabled){
       while (!transmitStartBool()){}
   }
   transmitComplete = false;  
   UDR0 = data;
}

void transmit_uart_char(unsigned char data){
   if(interruptEnabled == false){
       while (!transmitStartBool());
   }
   //transmitComplete = false;
   UDR0 = data;
}

uint8_t receive_uart(void){
    if(interruptEnabled == false){
        while (!receiveStartBool());
    }
    return UDR0;
}

unsigned char receive_uart_char(void){
    if(interruptEnabled == false){
    while (!receiveStartBool());
    }
    return UDR0;
}

void flush_uart(void){
    uint8_t flush;
    while (UCSR0A & (1<<RXC0)){
        flush = UDR0;
    }
}

void uart_transmit_interrupt(void){
    transmitStart = true;
}

void uart_receive_interrupt(void){
    receiveStart = true;
}

void transmit_after(void){
    transmitStart = false;
}

void receive_after(void){
    receiveStart = false;
}


void transmit_uart_complete(void){
    transmitComplete = true;
}

void getUartMode(bool interruptEnabled){
    return interruptEnabled;
}

void receive_uart_complete(void){
    receiveComplete = true;
}

bool receiveStartBool(void){
    return ((interruptEnabled == true) && receiveStart) || ((interruptEnabled == false) && (UCSR0A & (1<<RXC0)));
}

bool transmitStartBool(void){
    return ((interruptEnabled == true) && transmitStart) || ((interruptEnabled == false) && (UCSR0A & (1<<UDRE0)));
}


/*void receive_interrupt_enable(void){
    UCSR0B |= (1<<RXCIE0);
}

void transmit_complete_enable(void){
    UCSR0B |= (1<<TXCIE0);
}

void udre_interrupt_enable(void){
    UCSR0B |= (1<<UDRIE0);
}

void udre_interrupt_disable(void){
    UCSR0B &= ~(1<<UDRIE0);
}

void receive_interrupt_disable(void){
    UCSR0B &= ~(1<<RXCIE0);
}*/
