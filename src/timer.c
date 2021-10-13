#include "bootcamp/timer.h"


void timerInit(void){
    TCNT0 = 0;
    TCCR0A=(1<<WGM01);
    OCR0A=250;
    TIMSK0=(1<<OCIE0A);
    TCCR0B=(1<<CS01)|(1<<CS00);
}

void sineTimerInitPWM(void){
    TCNT1 = 0;
    TCCR1B |= (1<<CS10) | (1<<WGM12);
    TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B);

    OCR1A = 300; 
    OCR1B = 100;

}


void InterruptTimerIncrease(volatile uint16_t * timer){
    (*timer)++;
}