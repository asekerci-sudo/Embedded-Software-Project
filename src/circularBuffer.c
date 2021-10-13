#include "bootcamp/circularBuffer.h"


struct circularBuffer{
    uint8_t *bufferSpace;
    uint8_t readIndex;
    uint8_t writeIndex;
    uint8_t capacity;
    size_t size;
    bool full;
};

circularBuffer_t circularBuffer_init(uint8_t* buffer, size_t size){ 
    circularBuffer_t circularBufferptr;
    circularBufferptr = malloc(sizeof(circularBuffer));
    circularBufferptr -> capacity = size;
    circularBufferptr -> size = 0;
    circularBufferptr -> full = false;
    circularBufferptr -> readIndex = 0;
    circularBufferptr -> writeIndex = 0;
    circularBufferptr -> bufferSpace = buffer;
    return circularBufferptr;
}

void circularBuffer_free(circularBuffer_t cbuf){
    free(cbuf);
}

void circularBuffer_reset(circularBuffer_t cbuf){
    cbuf -> readIndex = 0;
    cbuf -> writeIndex = 0;
    cbuf -> size = 0;
    cbuf -> full  = false;
   
}

void circularBuffer_overwrite(circularBuffer_t cbuf, uint8_t data){
    if(cbuf -> full){
        *(cbuf -> bufferSpace + cbuf -> writeIndex) = data;
        cbuf -> readIndex = (cbuf -> readIndex + 1) % (cbuf -> capacity);
        cbuf -> writeIndex = (cbuf -> writeIndex + 1) % (cbuf -> capacity);
    }
    else{
        circularBuffer_push(cbuf,data);
    }
}

int8_t circularBuffer_push(circularBuffer_t cbuf, uint8_t data){
    if(cbuf -> full){
        return -1;
    }
    *(cbuf -> bufferSpace + cbuf -> writeIndex) = data;
    cbuf -> writeIndex = (cbuf -> writeIndex + 1) % (cbuf -> capacity);
    cbuf -> size += 1;
    cbuf -> full = (cbuf -> readIndex == cbuf -> writeIndex);
    return 0;
}

int8_t circularBuffer_multiplePush(circularBuffer_t cbuf,uint8_t *arrayToPush, uint8_t numberToPush){
    for(int x = 0; x < numberToPush; x++){
        if(circularBuffer_push(cbuf,arrayToPush[x])==-1){
            return -1;
        }
    }
    return 0;
}

int8_t circularBuffer_read(circularBuffer_t cbuf, uint8_t* data){
    if (cbuf -> size == 0){
        return -1;
    }
    *data = *(cbuf -> bufferSpace + cbuf -> readIndex);
    cbuf -> readIndex = (cbuf -> readIndex + 1) % cbuf -> capacity;
    cbuf -> full = false;
    cbuf -> size -= 1;
    return 0;
}

bool circularBuffer_empty(circularBuffer_t cbuf){
    return cbuf -> size == 0;
}

bool circularBuffer_full(circularBuffer_t cbuf){
    return cbuf -> full;
}

size_t circularBuffer_capacity(circularBuffer_t cbuf){
    return cbuf -> capacity;
}

size_t circularBuffer_size(circularBuffer_t cbuf){
    return cbuf -> size;
}

