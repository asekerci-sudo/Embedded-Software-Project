#ifndef TASKQUEUE
#define TASKQUEUE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "bootcamp/timer.h"
#include "bootcamp/task.h"
#include <string.h>
#ifdef AVR
#include <avr/interrupt.h>
#else 
#include "bootcamp/mockInterrupt.h"
#endif

// rename priorityQueue and Taskscheduler as a struct
typedef struct priorityQueue priorityQueue;

typedef priorityQueue *priorityQueuePTR;

typedef struct InterruptScheduler InterruptScheduler;

typedef InterruptScheduler *Schedulerptr;

// Initializing priorityQueue with a max capacity. The Queue is bassed on an static array. The memory is assigned with the struct last member trick.
priorityQueuePTR priorityQueueInit(uint8_t);

Schedulerptr InterruptSchedulerInit(priorityQueuePTR);

// insert a Task into the array sorted by priority. If the array is not empty, the function is not executed.
uint8_t enqueue(priorityQueuePTR,Task);

// Tasks which are ready to start are removed from the queue with this function. An Task can only be removed if it was declared as 'ready'.
Task dequeue(priorityQueuePTR);

//return if the queue is full
bool fullQueue(priorityQueuePTR);

//return if the queue is empty
bool emptyQueue(priorityQueuePTR);

// iterate through array and declare tasks as ready if their assigned starttime is due.
bool declareReady(Schedulerptr,uint32_t);

// deallocate memory for queue and scheduler here.
void freePriorityQueue(priorityQueuePTR);

void freeInterruptScheduler(Schedulerptr);

// initilaize 1ms timer here
void timerInit(void);

// dequeue the task which by the given taskID.
int8_t dequeueWithtaskID(priorityQueuePTR priorityQueue,uint8_t);

// The TasksScheduling happens here. Declaring task as ready if there are any tasks and executing their functions. If a task is periodic it is reinserted into the queue.
void InterruptSchedulerMain(Schedulerptr,uint32_t );

// enqueue function for the InterruptScheduler. All parameters of the task are assigned by declaring them in the function head.
void InterruptSchedulerAdd(Schedulerptr , void (*function)(char *),uint8_t ,uint32_t ,uint32_t ,bool,bool,uint8_t, char *);

void InterruptSchedulerTimerIncrement(Schedulerptr ,uint32_t *);

//Task InterruptSchedulerGet(Schedulerptr scheduler);

// return the capacity  of the priorityQueue
uint8_t returnCapacity(priorityQueuePTR priorityQueue);

// execute Task. Here non periodic task arguments are freed, so no memory leak.
void executeTask(Task);

// return Element count of the priorityQueue
uint8_t returnElements(priorityQueuePTR priorityQueue);

//uint32_t returnCurrentTime(uint32_t);

#endif