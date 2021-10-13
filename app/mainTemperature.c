#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "bootcamp/circularBuffer.h"
#include "bootcamp/uart.h"
#include "bootcamp/task.h"
#include "bootcamp/temperature.h"
#include "bootcamp/taskQueue.h"
#define AVR
#define AVR_INTERRUPT

volatile uint16_t currentTime = 0;

ISR(ADC_vect){  
  uint8_t Temperature = getTemperature();
  transmit_uart(Temperature);
}

/*ISR(USART_TX_vect){
  transmit_uart();
}*/

int main(void){
  init_uart_predefined(false,103);
  ACDInit(true);
  sei();
  while(true){
    ACDStart();
  }
}
