#include "bootcamp/temperature.h"

bool interruptsEnabled;
//bool readTemperature;

void ACDInit(bool interruptsEnabled){
    ADMUX = 3 << REFS0 | 1 << MUX3; 
    ADCSRA |= 1 << ADEN;
    ADCSRA |= 7 << ADPS0;
    ADCSRA |= 1 << ADATE;
    ADCSRA |= 1 << ADSC;
    if(interruptsEnabled){
        ADCSRA |= 1 << ADIE;
    }
}

void ACDStart(void){
    ADCSRA |= 1 << ADSC;
}

uint8_t getTemperature(void){
    if(interruptsEnabled = false){
        while(ADCSRA & (1<<ADSC));
    }
    uint8_t Temperatur = (ADCW-324)*3/5;  
    return  Temperatur;
}

