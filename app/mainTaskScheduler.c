#include <avr/io.h>
#include <avr/interrupt.h>

//#define __AVR__

#include <stdbool.h>
#include <util/delay.h>

#include "bootcamp/uartBuffer.h"
#include "bootcamp/taskQueue.h"
#include "bootcamp/timer.h"


volatile uint8_t counter;
volatile uint32_t timer;
volatile Schedulerptr testScheduler;
uint8_t sinIndex = 0;


void LightTask(char * arg){
    PORTB ^= _BV(5);
    _delay_ms(80);
    PORTB ^= _BV(5);
}

void CounterSendTask(char * arg){
    counter += 1;
    transmit_uart(counter);
}

int main(void){
    timer = 0;
    DDRB = _BV(5);
    counter = 0;
    char * testArg = malloc(sizeof(char));
    uint8_t testNumber;
    //init_uart_predefined(false,103);
    priorityQueuePTR testQueue = priorityQueueInit(2);
    testScheduler = InterruptSchedulerInit(testQueue);
    timerInit();
    sei();
    InterruptSchedulerAdd(testScheduler,LightTask,1,50,100,true,false,testNumber,testArg);
    InterruptSchedulerAdd(testScheduler,CounterSendTask,2,100,20,true,false,testNumber,testArg);
    while(true){
        InterruptSchedulerMain(testScheduler,timer);
    }
}

ISR(TIMER0_COMPA_vect){
    //InterruptSchedulerTimerIncrement(testScheduler,timer);
    timer = timer + 1;
}
