#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/**
 * @typedef circularBuffer
 * @brief   Opaque circular buffer structure.
 * 
 * This enables encapsulation: the internals will be implemented in the .c file, but the user must not know about this!
 * Please implement this buffer with a FIFO (first in, first out) policy!
 */
typedef struct circularBuffer circularBuffer;

/**
 * @typedef circularBuffer_t
 * @brief   Circular buffer abstract data type interface.
 * 
 * Handle type, this way users will interact with the API.
 */
typedef circularBuffer* circularBuffer_t;

/**
 * @brief   Creates circular buffer container.
 * 
 * For embedded memory management, we want to have full control over memory resources.
 * Therefore, we pass a data buffer into the circular buffer to work with.
 * This way, the control of the memory size and lifetime lies with the user!
 * 
 * @param   buffer
 *  Storage buffer for the elements to be stored, must be created independently
 * @param   size
 *  Size of the storage buffer
 * @returns
 *  A circular buffer handle
 */
circularBuffer_t circularBuffer_init(uint8_t* buffer, size_t size);

/**
 * @brief   Frees a circular buffer structure.
 * 
 * This only frees the memory overhead that is necessary for managing the circular buffer.
 * It does not free the data buffer; the owner is responsible for that.
 * 
 * @param   cbuf
 *  Circular buffer to be freed
 */
void circularBuffer_free(circularBuffer_t cbuf);

/**
 * @brief   Reset the circular buffer to empty.
 * 
 * @param   cbuf
 *  Circular buffer to be reset
 */
void circularBuffer_reset(circularBuffer_t cbuf);

/**
 * @brief   Adds data to a circular buffer and will overwrite old data.
 * 
 * If the buffer is full, the oldest data element will be overwritten.
 * 
 * @param   cbuf
 *  Circular buffer to add data to
 * @param   data
 *  Data item to add
 */
void circularBuffer_overwrite(circularBuffer_t cbuf, uint8_t data);

/**
 * @brief   Adds data to a circular buffer and will reject new data if the buffer is full.
 * 
 * @param   cbuf
 *  Circular buffer to add data to
 * @param   data
 *  Data item to add
 * @returns
 *  0 on success, -1 if buffer is full
 */
int8_t circularBuffer_push(circularBuffer_t cbuf, uint8_t data);

int8_t circularBuffer_multiplePush(circularBuffer_t , uint8_t *, uint8_t numberToPush); 

/**
 * @brief   Read a value from the buffer.
 * 
 * @param   cbuf
 *  Circular buffer to read from
 * @param   data
 *  Provided pointer which points to the data element after calling
 * @returns
 *  0 on success, -1 if buffer is empty
 */
int8_t circularBuffer_read(circularBuffer_t cbuf, uint8_t* data);

/**
 * @brief   Checks whether the buffer is empty.
 * 
 * @param   cbuf
 *  Circular buffer to check
 * @returns
 *  true if the buffer is empty, else false
 */
bool circularBuffer_empty(circularBuffer_t cbuf);

/**
 * @brief   Checks whether the buffer is full.
 * 
 * @param   cbuf
 *  Circular buffer to check
 * @returns
 *  true if the buffer is full, else false
 */
bool circularBuffer_full(circularBuffer_t cbuf);

/**
 * @brief   Returns maximum capacity of the buffer.
 * 
 * @param   cbuf
 *  Circular buffer to check
 * @returns
 *  Maximum capacity of the buffer
 */
size_t circularBuffer_capacity(circularBuffer_t cbuf);

/**
 * @brief   Returns the current number of elements in the buffer.
 * 
 * @param   cbuf
 *  Circular buffer to check
 * @returns
 *  Current number of elements in the buffer
 */
size_t circularBuffer_size(circularBuffer_t cbuf);

#endif //CIRCULARBUFFER_H
