#include "bootcamp/taskQueue.h"
#include "bootcamp/task.h"
#include "bootcamp/uart.h"


struct priorityQueue{
    uint8_t maxCapacity;
    uint8_t elementsinQueue;
    Task TaskArray[];
};

struct InterruptScheduler{
    priorityQueuePTR ScheduleQueue;
};

priorityQueuePTR priorityQueueInit(uint8_t maxCapacity){
    priorityQueuePTR initqueue = malloc(sizeof(priorityQueue)+ sizeof(Task) * maxCapacity);
    initqueue -> maxCapacity = maxCapacity;
    initqueue -> elementsinQueue = 0;
    return initqueue;
}

void freeInterruptScheduler(Schedulerptr Schedulerptr){
    free(Schedulerptr);
}

void executeTask(Task task){
    task.placeholderFunction(task.arg);
    if(task.periodic == false){
        free(task.arg);               
        task.arg = NULL;
    }
}

void freePriorityQueue(priorityQueuePTR prioptr){
    free(prioptr);
}

void arrayElementDelete(priorityQueuePTR priorityQueue,uint8_t index){
    for(index; index < (priorityQueue -> elementsinQueue); index++){
        priorityQueue -> TaskArray[index] = priorityQueue -> TaskArray[index+1];
    }
}

uint8_t enqueue(priorityQueuePTR priorityQueue,Task Task){
    if(!(fullQueue(priorityQueue)))
    {
        if(emptyQueue(priorityQueue)){
            priorityQueue -> TaskArray[0] = Task;
            priorityQueue -> elementsinQueue += 1;
            return 0;
        }
        else{
            uint8_t index = 0;
            while(index != (priorityQueue->elementsinQueue) && Task.priority > (priorityQueue -> TaskArray[index].priority)){
               index = index + 1;
            }
            for(uint8_t movingIndex = (priorityQueue -> maxCapacity)-1; movingIndex > index; movingIndex--){
                priorityQueue -> TaskArray[movingIndex] = priorityQueue -> TaskArray[movingIndex - 1];
            }
            priorityQueue -> TaskArray[index] = Task;
            priorityQueue -> elementsinQueue += 1;
            return 0;
        }
    }
    return -1;
}

Task dequeue(priorityQueuePTR priorityQueue){
    if(!(emptyQueue(priorityQueue))){
    uint8_t index = 0;
    Task TaskToReturn;
    while(index != (priorityQueue->elementsinQueue)-1 && priorityQueue ->TaskArray[index].ready == false){
        index = index + 1;
    }
    if(priorityQueue -> TaskArray[index].ready == true){
        TaskToReturn = priorityQueue -> TaskArray[index];
        arrayElementDelete(priorityQueue,index);
        priorityQueue -> elementsinQueue -= 1;
        return TaskToReturn;
    }
  }
}

int8_t dequeueWithtaskID(priorityQueuePTR priorityQueue,uint8_t searchedTaskID){
    if(!(emptyQueue(priorityQueue))){
        for(int i = 0; i < priorityQueue -> elementsinQueue; i++){
            if(priorityQueue -> TaskArray[i].taskID == searchedTaskID){
                free(priorityQueue -> TaskArray[i].arg);
                priorityQueue->TaskArray[i].arg = NULL;
                arrayElementDelete(priorityQueue,i);
                priorityQueue -> elementsinQueue -= 1;
                return 0;
            }
        }
    }
    return -1;
}



Schedulerptr InterruptSchedulerInit(priorityQueuePTR priorityQueue){
    Schedulerptr initScheduler = malloc(sizeof(InterruptScheduler));
    initScheduler -> ScheduleQueue = priorityQueue;
    return initScheduler;
}

void InterruptSchedulerAdd(Schedulerptr scheduler, void (*taskFunction) (char * FunctionArg) ,uint8_t priority,uint32_t timetoStart,uint32_t timePeriod,bool periodic,bool ready,uint8_t taskID,char * arg){
    if(!(fullQueue(scheduler -> ScheduleQueue))){
        Task Task;
        Task.priority = priority;
        Task.placeholderFunction = taskFunction;
        Task.timetoStart = timetoStart;
        Task.timePeriod = timePeriod;
        Task.ready = ready;
        Task.periodic = periodic;
        Task.taskID = taskID;
        Task.arg = arg;
        enqueue(scheduler -> ScheduleQueue, Task);
    }
} 

bool declareReady(Schedulerptr scheduler, uint32_t timer){
    bool oneIsReady = false;
    for (int x = 0; x < scheduler -> ScheduleQueue -> elementsinQueue; x++){
        if((scheduler -> ScheduleQueue -> TaskArray[x].timetoStart) <= timer){
            scheduler -> ScheduleQueue -> TaskArray[x].ready = true;
            oneIsReady = true;
        }
    }
    return oneIsReady;
}

void InterruptSchedulerMain(Schedulerptr schedule,uint32_t timer){
    uint32_t timetoStart;
    Task Task;
    if(!(emptyQueue(schedule -> ScheduleQueue))){
        bool oneIsReady = declareReady(schedule,timer);
        if(oneIsReady){
            //transmit_uart_char(49);
            Task = dequeue(schedule -> ScheduleQueue);
            executeTask(Task);
            if(Task.periodic == true){
                timetoStart = timer + (Task.timePeriod);
                InterruptSchedulerAdd(schedule, Task.placeholderFunction, Task.priority, timetoStart, Task.timePeriod,Task.periodic,false,Task.taskID,Task.arg);
            }
        }
    }
}

void InterruptSchedulerTimerIncrement(Schedulerptr scheduler,uint32_t *timer){
    *timer = *timer + 1;
}

bool fullQueue(priorityQueuePTR priorityQueue){
    return  priorityQueue -> maxCapacity == priorityQueue -> elementsinQueue;  
}

bool emptyQueue(priorityQueuePTR priorityQueue){
    return priorityQueue -> elementsinQueue == 0;
}

uint8_t returnCapacity(priorityQueuePTR priorityQueue){
    return priorityQueue -> maxCapacity;
}

uint8_t returnElements(priorityQueuePTR priorityQueue){
    return priorityQueue -> elementsinQueue;
}

uint32_t returnCurrentTime(uint32_t timer){
    return timer;
}

