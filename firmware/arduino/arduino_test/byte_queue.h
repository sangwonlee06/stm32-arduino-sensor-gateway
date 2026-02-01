#ifndef BYTE_QUEUE_H
#define BYTE_QUEUE_H

/* INCLUDES */
#include <stddef.h>   // size_t

#ifdef __cplusplus
extern "C" {
#endif

/* RETURN CODES */
typedef enum
{
    BYTE_QUEUE_OK = 0,
    BYTE_QUEUE_FULL = -1,
    BYTE_QUEUE_EMPTY = -2,
    BYTE_QUEUE_MALLOC_FAIL = -3,
    BYTE_QUEUE_INPUT_NULL = -4,
    BYTE_QUEUE_NAME_SET_FAIL = -5,
    BYTE_QUEUE_MUTEX_CREATE_FAIL = -6,
    BYTE_QUEUE_UNDERFLOW = -7,
} ByteQueueStatus;

/* BYTE QUEUE ID (index into global queue table) */
typedef enum
{
    BYTE_QUEUE_UART_ARD = 0,
    BYTE_QUEUE_ID_MAX,
} ByteQueueId;

/* BYTE QUEUE STRUCT (byte-oriented ring buffer) */
typedef struct
{
    unsigned char *buffer;        // ring buffer storage
    unsigned int head;            // read position
    unsigned int tail;            // write position
    unsigned int count;           // number of bytes stored
    unsigned int capacity;        // total buffer size in bytes
    char name[32];                // debug name (null-terminated)
//    SemaphoreHandle_t mutex;      // protects buffer state
} ByteQueue;

/*
 * API NOTES
 *
 * - Write      : append bytes to queue
 * - Read       : read and consume bytes
 * - Peek       : read without consuming
 * - ReadMax    : read up to max_len bytes
 * - PeekMax    : peek up to max_len bytes
 *
 * Return values for Read / Peek functions:
 *   >= 0 : number of bytes copied
 *   <  0 : ByteQueueStatus error code
 */

/* CREATE / DESTROY */
ByteQueueStatus ByteQueue_Create(ByteQueueId id,
                                 size_t capacity_bytes,
                                 const char *name,
                                 size_t name_len);

void            ByteQueue_Destroy(ByteQueueId id);
void            ByteQueue_Clear(ByteQueueId id);

/* STATE */
size_t          ByteQueue_Count(ByteQueueId id);

/* WRITE */
ByteQueueStatus ByteQueue_Write(ByteQueueId id,
                                const unsigned char *data,
                                size_t len);

//ByteQueueStatus ByteQueue_WriteFromISR(ByteQueueId id,
//                                       const unsigned char *data,
//                                       size_t len,
//                                       BaseType_t *pxHigherPriorityTaskWoken);

/* READ (consume) */
int             ByteQueue_Read(ByteQueueId id,
                               unsigned char *out,
                               size_t len);

int             ByteQueue_ReadMax(ByteQueueId id,
                                  unsigned char *out,
                                  size_t max_len);

/* PEEK (no consume) */
int             ByteQueue_Peek(ByteQueueId id,
                               unsigned char *out,
                               size_t len);

int             ByteQueue_PeekMax(ByteQueueId id,
                                  unsigned char *out,
                                  size_t max_len);

/* CONSUME WITHOUT COPYING */
ByteQueueStatus ByteQueue_Consume(ByteQueueId id,
                                  size_t len);

#ifdef __cplusplus
}
#endif

#endif /* BYTE_QUEUE_H */