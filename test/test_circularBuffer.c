#include <unity.h>
#include "bootcamp/circularBuffer.h"
#include <math.h>
#include <stdlib.h>

static size_t testBufferSize = 100;

void test_circularBuffer_init(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    TEST_ASSERT_TRUE(testCircularBuffer);
    free(circularBuffer);
    circularBuffer_free(testCircularBuffer);
}


void test_circularBuffer_free(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    circularBuffer_free(testCircularBuffer);
    testCircularBuffer = NULL;
    TEST_ASSERT_FALSE(testCircularBuffer);
    free(circularBuffer);
    free(testCircularBuffer);
}

void test_circularBuffer_capacityRight(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    TEST_ASSERT_EQUAL_size_t(testBufferSize,circularBuffer_capacity(testCircularBuffer));
    free(circularBuffer);
    circularBuffer_free(testCircularBuffer);
}

void test_circularBuffer_pushAndSizeTest(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    for(uint8_t x = 0; x < testBufferSize; ++x){
        circularBuffer_push(testCircularBuffer, x);
        TEST_ASSERT_EQUAL_UINT8(x+1, circularBuffer_size(testCircularBuffer));
    }
    free(circularBuffer);
    circularBuffer_free(testCircularBuffer);
}

void test_circularBuffer_fullandemptyRight(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    uint8_t* data = malloc(sizeof(uint8_t));
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    for(uint8_t x = 0; x < testBufferSize; ++x){
        circularBuffer_push(testCircularBuffer, x);
    }
    TEST_ASSERT_TRUE(circularBuffer_full(testCircularBuffer));
    TEST_ASSERT_EQUAL_UINT8(-1,circularBuffer_push(testCircularBuffer,1));
    for(uint8_t x = 0; x < testBufferSize; ++x){
        circularBuffer_read(testCircularBuffer, data);
    }
    TEST_ASSERT_TRUE(circularBuffer_empty(testCircularBuffer));
    free(circularBuffer);
    circularBuffer_free(testCircularBuffer);
}

void test_circularBuffer_readWhenEmpty(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    uint8_t* data = malloc(sizeof(uint8_t));
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    TEST_ASSERT_EQUAL_UINT8(-1,circularBuffer_read(testCircularBuffer,data));
    free(circularBuffer);
    circularBuffer_free(testCircularBuffer);
}

void test_circularBuffer_pushpossibleafterReset(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    uint8_t* data = malloc(sizeof(uint8_t));
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    for(uint8_t x = 0; x < testBufferSize; ++x){
        circularBuffer_read(testCircularBuffer, data);
    }
    circularBuffer_reset(testCircularBuffer);
    TEST_ASSERT_EQUAL_INT(0,circularBuffer_push(testCircularBuffer,*data));
    free(circularBuffer);
    circularBuffer_free(testCircularBuffer);
}

void test_circularBuffer_pushinOverwrite(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    uint8_t* data = malloc(sizeof(uint8_t));
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    circularBuffer_overwrite(testCircularBuffer,data);
    TEST_ASSERT_EQUAL_INT(1,circularBuffer_size(testCircularBuffer));
    free(circularBuffer);
    circularBuffer_free(testCircularBuffer);
}

void test_circularBuffer_overwriteinOverwrite(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    uint8_t* data = malloc(sizeof(uint8_t));
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    for(uint8_t x=0; x < testBufferSize + 2;++x){
        circularBuffer_overwrite(testCircularBuffer, data);
    }
    TEST_ASSERT_TRUE(circularBuffer_full(testCircularBuffer));
    for(uint8_t x = 0; x < testBufferSize; ++x){
        circularBuffer_read(testCircularBuffer, data);
    }
    TEST_ASSERT_EQUAL_INT(0,circularBuffer_size(testCircularBuffer));
    free(circularBuffer);
    circularBuffer_free(testCircularBuffer);
}


void test_circularBUffer_multiplePush(void){
    uint8_t *circularBuffer = malloc(sizeof(uint8_t) * testBufferSize);
    circularBuffer_t testCircularBuffer = circularBuffer_init(circularBuffer, testBufferSize);
    uint8_t dataArray[10] = {0,1,2,3,4,5,6,7,8,9};
    circularBuffer_multiplePush(testCircularBuffer,dataArray,10);
    for(uint8_t x = 0; x < 10; ++x){
        uint8_t data = 0;
        circularBuffer_read(testCircularBuffer, &data);
        TEST_ASSERT_EQUAL_UINT8(x,data);
    }
    
}

