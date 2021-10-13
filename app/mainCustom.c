#include <avr/io.h>
#include <avr/interrupt.h>
//#define __AVR__

#include <stdbool.h>
#include <util/delay.h>

#include "bootcamp/uartBuffer.h"
#include "bootcamp/taskQueue.h"
#include "bootcamp/timer.h"
#include "bootcamp/uartBuffer.h"
#include "bootcamp/temperature.h"
#include "bootcamp/mySinusFunction.h"

volatile uint16_t timer;
volatile uint8_t * Buffer;
volatile circularBuffer_t cBuffer;
volatile priorityQueuePTR TaskQueue;
volatile Schedulerptr scheduler;

ISR(TIMER0_COMPA_vect){
    timer = timer + 1;
}

void LightTask(void){
    PORTB ^= _BV(5);
    _delay_ms(100);
    PORTB ^= _BV(5);
}

void TemperatureTask(void){
    ACDInit(false);
    ACDStart();
    uint8_t Temperature = getTemperature();
    transmit_uart(Temperature);
}

void transmitSinus(void){
    float sinusValue = SinusFunction(timer);
    transmit_uart(sinusValue);
}


int main(void){
    init_uart_predefined(false,103);
    timer = 0;
    size_t buffersize = 6;
    Buffer = malloc(sizeof(uint8_t)*buffersize);
    cBuffer = circularBuffer_init(Buffer, buffersize);
    priorityQueue = priorityQueueInit(5);
    scheduler = InterruptSchedulerInit(priorityQueue);
    
    while(true){
        receive_uart_buffer(cBuffer);
        InterruptSchedulerMain(scheduler,timer);
        if(circularBuffer_full(cBuffer)){
            int8_t functionIdentifier;
            uint8_t priority = 0;
            uint8_t timetoStart = 0;
            uint8_t timePeriod = 0;
            uint8_t periodic = 0;
            uint8_t ready= 0;
            circularBuffer_read(cBuffer,&functionIdentifier);
            switch(functionIdentifier){
                default:
                    char strArray[] = {"E","r","r","o","r"};
                    for(int i=0; i<5; i++){ 
                        transmit_uart_char(strArray[i]);
                    }
                    break;
                case 1: 
                    circularBuffer_read(cBuffer,&priority);
                    circularBuffer_read(cBuffer,&timetoStart);
                    circularBuffer_read(cBuffer,&timePeriod);
                    circularBuffer_read(cBuffer,&periodic);
                    circularBuffer_read(cBuffer,&ready);
                    InterruptSchedulerAdd(scheduler,LightTask,priority,timetoStart,timePeriod,periodic,ready);
                    break;
                case 2:
                    circularBuffer_read(cBuffer,&priority);
                    circularBuffer_read(cBuffer,&timetoStart);
                    circularBuffer_read(cBuffer,&timePeriod);
                    circularBuffer_read(cBuffer,&periodic);
                    circularBuffer_read(cBuffer,&ready);
                    InterruptSchedulerAdd(scheduler,TemperatureTask,priority,timetoStart,timePeriod,periodic,ready);
                    break;
                case 3:
                    circularBuffer_read(cBuffer,&priority);
                    circularBuffer_read(cBuffer,&timetoStart);
                    circularBuffer_read(cBuffer,&timePeriod);
                    circularBuffer_read(cBuffer,&periodic);
                    circularBuffer_read(cBuffer,&ready);
                    InterruptSchedulerAdd(scheduler,transmitSinus,priority,timetoStart,timePeriod,periodic,ready);
                    break;
        }
    }
}
    //free(Buffer);
    //free(cBuffer);
    //freePriorityQueue(TaskQueue);
    //freeInterruptScheduler(scheduler);
}

/*ISR(USART_RX_vect){

}

ISR(USART_TX_vect){

}

//when UDRE is empty, then next data can be transmitted
ISR(USART_UDRE_vect){
    
}*/
