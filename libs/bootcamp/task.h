#ifndef TASK
#define TASK
#include <inttypes.h>

typedef struct Task{
    uint8_t priority;      //priority of Task
    uint32_t timetoStart; //time when it starts
    uint32_t timePeriod; //periodic value of task
    bool ready; //flag of readyness
    bool periodic; // periodic or not?
    uint8_t taskID;  //id of each task
    void (*placeholderFunction)(char * arg); // function which can also have a parameter
    char * arg; //argument of function
    //bool argShouldBeFreed;
}Task;


#endif