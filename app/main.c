#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
//#define __AVR__

#include <stdbool.h>
#include <util/delay.h>

#include "bootcamp/uart.h"
#include "bootcamp/taskQueue.h"
#include "bootcamp/timer.h"
#include "bootcamp/uartBuffer.h"
#include "bootcamp/temperature.h"
#include "bootcamp/mySinusFunction.h"
#include "bootcamp/commandHandler.h"
#include <string.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/eeprom.h>




volatile uint32_t timer;
volatile uint8_t * Buffer;
volatile uint8_t * commandOutputBuffer;
volatile circularBuffer_t cBuffer;
volatile circularBuffer_t circularCommandOutputBuffer;
volatile priorityQueuePTR TaskQueue;
volatile Schedulerptr scheduler;
volatile bool temperatureReady;
uint8_t count;
uint8_t taskIDNumber = 0;
uint8_t sinIndex;
bool eepromUpdateDone;
const uint8_t sineValues[] PROGMEM = 
{0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9c,0x9f,0xa2,0xa5,0xa8,0xab,0xae,
0xb0,0xb3,0xb6,0xb9,0xbc,0xbf,0xc1,0xc4,0xc7,0xc9,0xcc,0xce,0xd1,0xd3,0xd5,0xd8,
0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xed,0xef,0xf0,0xf2,0xf3,0xf5,
0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfc,0xfd,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfd,0xfc,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,
0xf6,0xf5,0xf3,0xf2,0xf0,0xef,0xed,0xec,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
0xda,0xd8,0xd5,0xd3,0xd1,0xce,0xcc,0xc9,0xc7,0xc4,0xc1,0xbf,0xbc,0xb9,0xb6,0xb3,
0xb0,0xae,0xab,0xa8,0xa5,0xa2,0x9f,0x9c,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x63,0x60,0x5d,0x5a,0x57,0x54,0x51,
0x4f,0x4c,0x49,0x46,0x43,0x40,0x3e,0x3b,0x38,0x36,0x33,0x31,0x2e,0x2c,0x2a,0x27,
0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x13,0x12,0x10,0x0f,0x0d,0x0c,0x0a,
0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x03,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x05,0x06,0x07,0x08,
0x09,0x0a,0x0c,0x0d,0x0f,0x10,0x12,0x13,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
0x25,0x27,0x2a,0x2c,0x2e,0x31,0x33,0x36,0x38,0x3b,0x3e,0x40,0x43,0x46,0x49,0x4c,
0x4f,0x51,0x54,0x57,0x5a,0x5d,0x60,0x63,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c};


const char commands[16][9] = {"help","toggle","inc","dec","counter","temp","echo","led","flash","sine","delete","zero","sineoff","sineon","reset","dice"};


ISR(TIMER0_COMPA_vect){
    timer++;                         
}

ISR(ADC_vect){
    temperatureReady = true;       //at every ADC conversion set the temperature Flag to ready
}


/*ISR(USART_UDRE_vect){
    uart_transmit_interrupt();     
}*/

ISR(USART_TX_vect){
   uart_transmit_interrupt();
}

ISR(USART_RX_vect){
    uart_receive_interrupt();
}


ISR(TIMER1_COMPA_vect){
    PORTB |= _BV(5);
}                                   // timer for sine to imitate fading by using ovverflow and underflow and doint this rapidly fast

ISR(TIMER1_COMPB_vect){
    PORTB &= ~_BV(5);
}

void delete(char * arg){
    int8_t taskisInQueue = dequeueWithtaskID(TaskQueue,atoi(arg));
    if(taskisInQueue == 0){
        circularBuffer_multiplePush(circularCommandOutputBuffer,"Task has been deleted succesfully!\n",36);  //dequeue the Task if there is a Task with the given id, else output error.
    }
    else{
        circularBuffer_multiplePush(circularCommandOutputBuffer,"No Task with this specific ID!\n",32);
    }
}

void reset(char * arg){
    free(TaskQueue);
    free(scheduler);
    TaskQueue = NULL;
    scheduler = NULL;
    taskIDNumber = 0;
    count = 0;                       //reset data structures and numeric values to clear the scheduler
    timer = 0;
    TaskQueue = priorityQueueInit(20);
    scheduler = InterruptSchedulerInit(TaskQueue);
}


void inc(char * arg){
    count++;
}

void dec(char * arg){
    count--;                   // functions to manipulate counter
}

void zero(char * arg){
    count = 0;
}

void sineoff(char * arg){
   power_timer1_disable();
}
                                //these function exist to power of and on the sine timer to enable toggling,flashing and led tasks to work after sine has been executed the first time.
void sineon(char * arg){
    power_timer1_enable();
}


void counter(char * arg){
    char counterArg[7];
    itoa(count,counterArg,10);
    circularBuffer_multiplePush(circularCommandOutputBuffer,counterArg,strlen(counterArg));
    circularBuffer_push(circularCommandOutputBuffer,'\n');
}

void dice(char * arg){
    if(eepromUpdateDone == false){
        uint16_t seed = eeprom_read_word(0);    //If dice has been commanded the eeprom beginning cell will be updated one time to create a pseudo random sequence. Use at caution of course!
        srand(seed++);                          
        eeprom_update_word(0, seed);
    }
    eepromUpdateDone = true;  //set the flag here to true to not execute the eeprom after the first dice command 
    char randNumberArg[2];
    uint8_t randomNumber = rand() % 6 + 1;   // random Number between 1 and 6 here.
    circularBuffer_multiplePush(circularCommandOutputBuffer,"You have thrown a ",19);
    circularBuffer_multiplePush(circularCommandOutputBuffer,itoa(randomNumber,randNumberArg,10),2);
    circularBuffer_push(circularCommandOutputBuffer,'\n');
    
}

void echo(char * arg){
    size_t echoLength= strlen(arg);
    circularBuffer_multiplePush(circularCommandOutputBuffer,arg,echoLength);   //push the arg into the buffer obviously :).
    circularBuffer_push(circularCommandOutputBuffer,'\n');
}


void led(char * arg){
    if(strcmp(arg,"1") == 0){
        PORTB |= _BV(5);
    }                                    //turn on LED if argument is 1  and turn it off if it is zero
    if(strcmp(arg,"0") == 0){
        PORTB &= ~(_BV(5));
    }
}


void help(char * arg){
    circularBuffer_multiplePush(circularCommandOutputBuffer,"help, echo <string>, led <bit>, toggle, flash <int>, sine <int>, inc,dec,counter,temp,delete <int>, zero,sineoff,sineon,reset,dice\n",132);
}

void error(void){
    circularBuffer_multiplePush(circularCommandOutputBuffer,"Wrong or not complete command. Please try again!\n",50);
}

void toggle(char * arg){
    PORTB ^= _BV(5);
}


void flash(char * arg){
    uint8_t numberOfFlashes = atoi(arg);                   //insert toggle tasks at a specific time intervall to imitate flash and call flash recursively until it is flash 0 and stops.
    if(numberOfFlashes != 0){
        numberOfFlashes--;
        char * toggleTaskParameter1;
        char * toggleTaskParameter2;
        InterruptSchedulerAdd(scheduler,toggle,1,timer,0,false,false,taskIDNumber,toggleTaskParameter1);
        InterruptSchedulerAdd(scheduler,toggle,1,timer + 300,0,false,false,taskIDNumber,toggleTaskParameter2); 
        if(numberOfFlashes != 0 ){
            char * numberOfFlashesString = malloc(sizeof(char)*3);
            char * taskParameter = strdup(itoa(numberOfFlashes,numberOfFlashesString,10));
            free(numberOfFlashesString);
            InterruptSchedulerAdd(scheduler,flash,1,timer + 600,0,false,false,taskIDNumber,taskParameter);
        }
    }
}

void temp(char * arg){
    if(temperatureReady){
        uint8_t currentTemp = getTemperature();
        char TempString[3];            
        itoa(currentTemp, TempString,10);
        circularBuffer_multiplePush(circularCommandOutputBuffer,"Temperature: ",14);
        circularBuffer_multiplePush(circularCommandOutputBuffer,TempString,strlen(TempString));
        circularBuffer_push(circularCommandOutputBuffer,'\n');
        temperatureReady = false;
    }
}

void sineMain(char * arg){
    OCR1B = pgm_read_byte(&sineValues[sinIndex]);        //read out values from flash and write it into the OCR1b register to have a top value. When this increases and falls it seems like it is dimming.
    sinIndex++;
}

void sine(char * arg){
    sineTimerInitPWM();                               //slow down sine timer every 255 argument
    uint32_t sinePeriod = atoi(arg)/255;
    InterruptSchedulerAdd(scheduler,sineMain,1,timer + sinePeriod,sinePeriod,true,true,taskIDNumber,"");
}

void (*taskCommands[16]) (char*) = {help,toggle,inc,dec,counter,temp,echo,led,flash,sine,delete,zero,sineoff,sineon,reset,dice}; // function pointer array which is in the same order as the function names to shorten up the code.

int main(void){
    bool commandEntered = false;
    bool commandError = false;
    //taskIDArrayElementCount = 5;
    count = 0;
    timer = 0;
    size_t buffersize = 140;
    //char StringBuffer[20];
    Buffer = malloc(sizeof(uint8_t)*buffersize);
    cBuffer = circularBuffer_init(Buffer, buffersize);
    commandOutputBuffer = malloc(sizeof(uint8_t)*buffersize);
    circularCommandOutputBuffer = circularBuffer_init(commandOutputBuffer,buffersize);
    TaskQueue = priorityQueueInit(20);
    scheduler = InterruptSchedulerInit(TaskQueue);
    init_uart_predefined(false,103);  //polling mode here, but you can try it with interrupts too (caution please !)
    ACDInit(true);
    DDRB = _BV(5);
    timerInit();
    sei();           // initiate pseudo random number generator through the noise of unused PORTC
    power_all_disable();
    power_timer0_enable();
    power_timer1_enable();     //disable all other function of the arduino which are not needed to save energy
    power_usart0_enable();
    power_adc_enable();
    set_sleep_mode(SLEEP_MODE_IDLE); //set sleep mode id before code execution
    sleep_enable(); 
    while(true){
        InterruptSchedulerMain(scheduler,timer); //scheduling happens here of course
        commandIO(cBuffer,circularCommandOutputBuffer);        // insert commands here into the inputBuffer and output there function output in the OutputBuffer
        if(commandEnteredBool()){
            uint8_t commandLength = 0;
            uint8_t commandIndex = 0;
            uint8_t arrayIndex = 0;
            uint8_t StringLength = 0;                     
            uint8_t parameterLength = 0;
            size_t len;
            char mainCommand[10] = "";
            char commandParameter[10] = "";
            char periodic[7] = "";
            char startTime[7] = "";
            while((!(circularBuffer_empty(cBuffer))) && commandError == false){
                char tmpLetter;
                if(commandIndex > 4 || parameterLength > 10){
                    commandError = true;
                    break;
                }
                circularBuffer_read(cBuffer,&tmpLetter);
                if(tmpLetter != 0x20 && commandError == false && tmpLetter != 0x0A && tmpLetter != 0x0D){
                    switch(commandIndex){
                        case 0:
                            //transmit_uart_char(49);
                            len = strlen(mainCommand);
                            mainCommand[len] = tmpLetter;
                            mainCommand[len+1] = '\0';
                            break;
                        case 1:
                            len = strlen(commandParameter);
                            commandParameter[len] = tmpLetter;
                            commandParameter[len+1] = '\0';
                            parameterLength++;
                            break; 
                        case 2:                                   //parse through each subcommand to fill out char parameters array to have our startTime, periodic time, the actual function and function argument for the TaskScheduler
                            len = strlen(startTime);              // the '\0' is being attached at the end to not disrupt the string format
                            startTime[len] = tmpLetter;
                            startTime[len+1] = '\0';
                            parameterLength++;
                            break; 
                        case 3:
                            len = strlen(periodic);
                            periodic[len] = tmpLetter;
                            periodic[len+1] = '\0';
                            parameterLength++;
                            break; 
                    }
                }
                else if(tmpLetter == 0x20){
                    commandIndex++;            // the switching between subcommand happens here.
                    commandLength = 0;
                }
            }
            bool periodicBool = setPeriodicBool(periodic);      //set the periodicity to false if there is no argument provided or the periodicValue is zero
            for(uint8_t i = 0; i < 17; i++){
                if(i == 16){
                    commandError = true;
                    char * taskParameter;
                    InterruptSchedulerAdd(scheduler,error,1,timer,0,false,false,taskIDNumber,taskParameter);
                    break;
                }                                                                          
                else if(strcmp(mainCommand,commands[i]) == 0){
                    char * taskParameter = strdup(commandParameter);
                    InterruptSchedulerAdd(scheduler,taskCommands[i],1,timer + atoi(startTime),atoi(periodic),periodicBool,false,taskIDNumber,taskParameter);
                    if(periodicBool){
                        taskIDNumber++;
                    }
                    break;
                }
            }
            // If the task is valid it is inserted into the scheduler. If not, there is and error output. I malloc a string and free it later on.
            clearBuffers(cBuffer,circularCommandOutputBuffer);  //clear the Buffers to not mix up the output
            
            setCommandEnteredFalse();
            commandError = false;
            
            // sleep mode idle at the end to not interfere with the code execution.
            sleep_mode();
        }
    }
}