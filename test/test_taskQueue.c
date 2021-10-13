#include "bootcamp/taskQueue.h"
#include "bootcamp/task.h"
#include "bootcamp/timer.h"
#include <unity.h>

char EICRA = 0;
char EIMSK = 0;
char TCNT0 = 0;
char TCCR0A = 0;
char WGM01 = 0;
char OCR0A = 0;
char TIMSK0 = 0;
char OCIE0A = 0;
char TCCR0B = 0;
char CS01 = 0;
char CS00 = 0;

uint8_t timer = 0;

void sei(void){
}

void dummy(void){

}

void dummyWithChar(char * arg){

}



void test_FullAndEmptyFunctionTest(void){
    priorityQueuePTR testQueue = priorityQueueInit(2);
    TEST_ASSERT_TRUE(emptyQueue(testQueue));
    TEST_ASSERT_FALSE(fullQueue(testQueue));
    Task task;
    for(int i=0; i<2;i++){
        enqueue(testQueue,task);
    }
    TEST_ASSERT_TRUE(fullQueue(testQueue));
    TEST_ASSERT_FALSE(emptyQueue(testQueue));
    testQueue = NULL;
    freePriorityQueue(testQueue);
}


void test_priorityQueueMaxCapacity(void){
    priorityQueuePTR testQueue = priorityQueueInit(10);
    TEST_ASSERT_TRUE(testQueue);
    TEST_ASSERT_EQUAL_UINT8(10,returnCapacity(testQueue));
    testQueue = NULL;
    freePriorityQueue(testQueue);
}

void test_priorityQueueSizeCheck(void){
    priorityQueuePTR testQueue = priorityQueueInit(4);
    Task task;
    task.placeholderFunction = enqueue;
    task.periodic = false;
    task.ready= true;
    task.timetoStart = 0;
    task.timePeriod = 0;

    for(int i = 1; i < 3;i++){
        task.priority = i;
        enqueue(testQueue,task);
        TEST_ASSERT_EQUAL_UINT8(i,returnElements(testQueue));
    }
    //enqueue(testQueue,task);
    TEST_ASSERT_EQUAL_UINT8(2,returnElements(testQueue));
    for(int i = 1; i < 3;i++){
        Task dequeuedTask = dequeue(testQueue); 
        TEST_ASSERT_EQUAL_UINT8(i,dequeuedTask.priority);
    }
    //TEST_ASSERT_EQUAL_UINT8(0,returnElements(testQueue));
    testQueue = NULL;
    freePriorityQueue(testQueue);
}

void test_freeSchedulerandQueue(void){
    priorityQueuePTR testQueue = priorityQueueInit(10);
    Schedulerptr testScheduler = InterruptSchedulerInit(testQueue);
    freeInterruptScheduler(testScheduler);
    freePriorityQueue(testQueue);
    testQueue = NULL;
    testScheduler = NULL;
    TEST_ASSERT_FALSE(testScheduler);
    TEST_ASSERT_FALSE(testQueue);
}

void test_priorityQueueReadyDequeue(void){
    priorityQueuePTR testQueue = priorityQueueInit(9);
    Task task;
    task.placeholderFunction = enqueue;
    task.periodic = false;
    task.timetoStart = 0;
    task.timePeriod = 0;
    task.priority = 1;

    for(int i = 0; i < 8;i++){
        if(i % 2 == 0){
            task.ready = true;
        }
        else{
            task.ready = false;
        }
        enqueue(testQueue,task);
    }
    for(int i = 0; i < 8;i++){
        dequeue(testQueue);
    }
    TEST_ASSERT_EQUAL_UINT8(4,returnElements(testQueue));
    testQueue = NULL;
    freePriorityQueue(testQueue);
}

void test_InterruptsSchedulerGetAndAdd(void){
    priorityQueuePTR testQueue = priorityQueueInit(10);
    Schedulerptr testScheduler = InterruptSchedulerInit(testQueue);
    Task task;
    task.placeholderFunction = enqueue;
    task.periodic = true;
    task.timetoStart = 0;
    task.timePeriod = 0;
    task.priority = 1;
    task.ready = true;
    enqueue(testQueue,task);
    TEST_ASSERT_EQUAL_UINT8(1,dequeue(testQueue).periodic);
    testQueue = NULL;
    testScheduler = NULL;
    freePriorityQueue(testQueue);
    freeInterruptScheduler(testScheduler);

}


void test_declareReady(void){
    priorityQueuePTR testQueue = priorityQueueInit(2);
    Schedulerptr testScheduler = InterruptSchedulerInit(testQueue);
    uint16_t timer = 0;
    //uint16_t* timerPTR = &timer;
    InterruptSchedulerAdd(testScheduler,enqueue,1,1,120, true,false,0,"");
    TEST_ASSERT_EQUAL_UINT8(1,returnElements(testQueue));
    timer +=1;
    TEST_ASSERT_EQUAL_UINT8(1,returnCurrentTime(timer));
    declareReady(testScheduler,timer);
    dequeue(testQueue);
    TEST_ASSERT_EQUAL_UINT8(0,returnElements(testQueue));
    testQueue = NULL;
    testScheduler = NULL;
    freePriorityQueue(testQueue);
    freeInterruptScheduler(testScheduler);   
}

void test_InterruptSchedulerMain(void){
    priorityQueuePTR testQueue = priorityQueueInit(2);
    Schedulerptr testScheduler = InterruptSchedulerInit(testQueue);
    uint16_t timer = 0;
    InterruptSchedulerAdd(testScheduler,dummy,1,0,2, true,false,0,"");
    TEST_ASSERT_EQUAL_UINT8(1,returnElements(testQueue));
    InterruptSchedulerMain(testScheduler,timer);
    TEST_ASSERT_EQUAL_UINT8(1,returnElements(testQueue));
    timer +=1;
    timer +=1;
    InterruptSchedulerMain(testScheduler,timer);
    TEST_ASSERT_EQUAL_UINT8(1,returnElements(testQueue));
    testQueue = NULL;
    testScheduler = NULL;
    freePriorityQueue(testQueue);
    freeInterruptScheduler(testScheduler);
}

void test_TasksRightPriorityOrder(void){
    priorityQueuePTR testQueue = priorityQueueInit(3);
    Task task;
    task.placeholderFunction = enqueue;
    task.periodic = false;
    task.ready= true;
    task.timetoStart = 0;
    task.timePeriod = 0;
    for(int i = 3; i > 0;i--){
        task.priority = i;
        enqueue(testQueue,task);
    }
    for(int i = 1; i < 4;i++){
        Task dequeuedTask = dequeue(testQueue); 
        TEST_ASSERT_EQUAL_UINT8(i,dequeuedTask.priority);
    }
    testQueue = NULL;
    freePriorityQueue(testQueue);
}

void test_TaskDequeueWithID(void){
    priorityQueuePTR testQueue = priorityQueueInit(1);
    Schedulerptr testScheduler = InterruptSchedulerInit(testQueue);
    char * arg = malloc(sizeof(char));
    InterruptSchedulerAdd(testScheduler,dummyWithChar,1,1,120, true,false,1,arg);
    TEST_ASSERT_EQUAL_UINT8(1,returnElements(testQueue));
    dequeueWithtaskID(testQueue,1);
    TEST_ASSERT_EQUAL_UINT8(0,returnElements(testQueue));
    freePriorityQueue(testQueue);
    freeInterruptScheduler(testScheduler);
}

void test_taskExecute(void){
    priorityQueuePTR testQueue = priorityQueueInit(1);
    Task taskPeriodic;
    taskPeriodic.periodic  = true;
    char testString[4] = "test";
    char * taskArgument = strdup(testString);
    taskPeriodic.placeholderFunction = dummyWithChar;
    taskPeriodic.arg = taskArgument;
    enqueue(testQueue,taskPeriodic);
    executeTask(dequeue(testQueue));
    TEST_ASSERT_TRUE(taskPeriodic.arg);
}






