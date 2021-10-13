#include "unity.h"
#include <math.h>
#include <string.h>
#include "bootcamp/mySinusFunction.h"
#define DELTA 0.01


static float convertToArcMeasure = 3.141592f/ 180.0f;
static float precisionFactor = 0.1f;

void testSinusFunction(void){
    for (int i = 0; i < 1000; i++){
        TEST_ASSERT_FLOAT_WITHIN(0.000001f, sin(i*precisionFactor*convertToArcMeasure),SinusFunction(precisionFactor*i));
    }
}

void test_FactorialZero(void){
    TEST_ASSERT_EQUAL_INT(1, FactorialFunction(0));
}

void test_FactorialPositive(void){
    for (int i = 1; i < 15; ++i){
        TEST_ASSERT_FLOAT_WITHIN(10.0f,tgamma(i+1),FactorialFunction(i));
    }
}

void test_powExponentZero(void){
    for(int i = 0; i < 10; ++i){
        TEST_ASSERT_EQUAL_UINT8(1,powerFunction(i,0));
    }
}

void test_powPositiveBase(void){

    for(int i = 0; i < 10;i++){ 
        TEST_ASSERT_FLOAT_WITHIN(0.1f,powf(i,i),powerFunction(i,i));
    }
}

void test_powNegativeBase(void){
    for(int i = -10; i < 0;i++){ 
        TEST_ASSERT_FLOAT_WITHIN(0.5f,powf(i,i+10),powerFunction(i,i+10));
    }
}

