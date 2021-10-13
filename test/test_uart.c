#include <unity.h>
#include "bootcamp/uart.h"
#include "bootcamp/circularBuffer.h"
#include "bootcamp/uartBuffer.h"

static uint8_t bufferCapacity = 20;

char UBRR0H = 0;
char UBRR0L = 0;
char UCSR0B = 0;
char UCSR0C = 6;
char RXEN0 = 4;
char TXEN0 = 3;
char UMSEL01 = 7;
char UMSEL00 = 6;
char UPM01 = 5;
char UPM00 = 4;
char USBS0 = 3;
char UCSZ00 = 1;
char UCSR0A = 32;
char UDRE0 = 5;
char UDR0 = 0;
char TXC0 = 6;
char RXC0 = 7;
char RXCIE0 = 0;
char TXCIE0 = 0;
char UDRIE0 = 0;



void test_uartFlushTest(void){
    init_uart_predefined(false,103);
    flush_uart();
    TEST_ASSERT_EQUAL_UINT8(UDR0,UDR0);
}

void test_uartTransmit(void){
    init_uart_predefined(false,103);
    for(int i = 0; i < 5;++i){
        transmit_uart(i);
        TEST_ASSERT_EQUAL_CHAR(i,UDR0);
    }
    for(int i = 0; i < 5;++i){
        transmit_uart_char(i);
        TEST_ASSERT_EQUAL_CHAR(i,UDR0);
    }
}

void test_uartInit(void){
     init_uart_predefined(false,103);
     TEST_ASSERT_EQUAL_UINT8(103,UBRR0L);
     TEST_ASSERT_EQUAL_UINT8(103 >> 8,UBRR0H);
}

void test_uartReceive(void){
    init_uart_predefined(false,103);
    UCSR0A = 128;
    for (uint8_t i = 0; i < 10; i++)
        {
            UDR0 = i;
            TEST_ASSERT_EQUAL_CHAR(i,receive_uart());
        }
}

void test_uartBufferReceive(void){
    uint8_t *testBuffer = malloc(sizeof(uint8_t)* bufferCapacity);
    circularBuffer_t testcircularBuffer = circularBuffer_init(testBuffer, bufferCapacity);
    init_uart_predefined(false,103);
    UCSR0A = 128;
    uint8_t* data = malloc(sizeof(uint8_t));
    for(int8_t i = 0; i < bufferCapacity; i++){
        UDR0 = i;
        receive_uart_buffer(testcircularBuffer);
        circularBuffer_read(testcircularBuffer,&data);
        TEST_ASSERT_EQUAL_INT8(UDR0,data);
    } // Test successfull receives
    TEST_ASSERT_EQUAL_INT8(-1,circularBuffer_read(testcircularBuffer,data)); // Test failure
    for(int8_t i = 0; i < bufferCapacity; ++i){
        UDR0 = i;
        receive_uart_buffer(testcircularBuffer);
    }
    TEST_ASSERT_EQUAL_INT8(-1,circularBuffer_push(testcircularBuffer,1));
    free(testcircularBuffer);
    free(testBuffer);
}

void test_uartBufferPush(void){
    uint8_t *testBuffer = malloc(sizeof(uint8_t)* bufferCapacity);
    circularBuffer_t testcircularBuffer = circularBuffer_init(testBuffer, bufferCapacity);
    init_uart_predefined(false,103);
    UCSR0A = 32;
    UDR0 = 1;
    TEST_ASSERT_EQUAL_INT8(0,circularBuffer_push(testcircularBuffer,UDR0));
    //TEST_ASSERT_EQUAL_INT8(1,UDR0);
    //TEST_ASSERT_EQUAL_INT8(1,circularBuffer_size(testcircularBuffer));
    free(testcircularBuffer);
    free(testBuffer);
}

void test_uartBufferTransmit(void){
    uint8_t *testBuffer = malloc(sizeof(uint8_t)* bufferCapacity);
    circularBuffer_t testcircularBuffer = circularBuffer_init(testBuffer, bufferCapacity);
    circularBuffer_push(testcircularBuffer,1);
    transmit_uart_buffer(testcircularBuffer);
    TEST_ASSERT_EQUAL_UINT8(UDR0,1);
    free(testcircularBuffer);
    free(testBuffer);
}

void test_uartBufferReceiveOverwrite(void){
    uint8_t *testBuffer = malloc(sizeof(uint8_t)* bufferCapacity);
    circularBuffer_t testcircularBuffer = circularBuffer_init(testBuffer, bufferCapacity);
    init_uart_predefined(false,103);
    UCSR0A = 128;
    uint8_t* data = malloc(sizeof(uint8_t));
    for(int8_t i = 0; i < bufferCapacity+5; i++){
        UDR0 = i;
        receive_uart_buffer_overwrite(testcircularBuffer);
        circularBuffer_read(testcircularBuffer,&data);
        TEST_ASSERT_EQUAL_INT8(UDR0,data);
    } // Test successfull receives
    TEST_ASSERT_EQUAL_INT8(-1,circularBuffer_read(testcircularBuffer,data)); // Test failure
    for(int8_t i = 0; i < bufferCapacity; ++i){
        UDR0 = i;
        receive_uart_buffer(testcircularBuffer);
    }
    TEST_ASSERT_EQUAL_INT8(-1,circularBuffer_push(testcircularBuffer,1));
    free(testcircularBuffer);
    free(testBuffer);
}



