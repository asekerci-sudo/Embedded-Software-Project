#include "bootcamp/commandHandler.h"


bool commandEntered = false;
bool commandError = false;
//uint8_t taskIDNumber = 0;

void commandIO(circularBuffer_t cBuffer, circularBuffer_t circularCommandOutputBuffer){
    if(receiveStartBool()){
            uint8_t receivedValue = receive_uart_char();
            if(receivedValue != 0x0A){
                circularBuffer_push(cBuffer,receivedValue);
                //transmit_uart_char(0);    //comment this out to test out the interrupt mode if you want (does not work as good as polling)
            }
            else{
                commandEntered = true;      
            }
            receive_after();
        }
    if(transmitStartBool()){
        transmit_uart_buffer(circularCommandOutputBuffer);
        transmit_after();
    }
}

bool commandEnteredBool(void){
    return commandEntered;
}



bool setPeriodicBool(char * periodic){
    bool periodicBool = true;
    if(strcmp(periodic,"0") == 0 || strcmp(periodic,"") == 0){
        periodicBool = false;
    }
    return periodicBool;
}

void setCommandEnteredFalse(void){
    commandEntered = false;
}

void setCommandEnteredTrue(void){
    commandEntered = true;
}

void setCommandErrorFalse(void){
    commandError = false;
}

void setCommandErrorTrue(void){
    commandError = true;
}

void clearBuffers(circularBuffer_t cBuffer,circularBuffer_t circularCommandOutputBuffer){
    uint8_t* wasteTmp = malloc(sizeof(uint8_t));
    while(!(circularBuffer_empty(cBuffer))){
        circularBuffer_read(cBuffer,wasteTmp);
    }
    free(wasteTmp);
    circularBuffer_reset(cBuffer);
    circularBuffer_reset(circularCommandOutputBuffer);
}
