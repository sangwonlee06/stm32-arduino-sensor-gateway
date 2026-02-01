#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "byte_queue.h"

/* GLOBAL VARIABLES */
static ByteQueue g_byteQueues[BYTE_QUEUE_ID_MAX];

/* FUNCTIONS */

ByteQueueStatus ByteQueue_Create(ByteQueueId id,
                                 size_t capacity_bytes,
                                 const char *name,
                                 size_t name_len)
{
    memset(&g_byteQueues[id], 0, sizeof(ByteQueue));

    g_byteQueues[id].buffer = (unsigned char *)malloc(capacity_bytes);
    if (g_byteQueues[id].buffer == NULL)
    {
        printf("%s: [%s] Malloc fail\r\n", __func__, name);
        return BYTE_QUEUE_MALLOC_FAIL;
    }

    if (name_len > sizeof(g_byteQueues[id].name) - 1)
    {
        printf("%s: [%s] Name size fail\r\n", __func__, name);
        free(g_byteQueues[id].buffer);
        return BYTE_QUEUE_NAME_SET_FAIL;
    }

    g_byteQueues[id].capacity = (unsigned int)capacity_bytes;
    memcpy(g_byteQueues[id].name, name, name_len);
    g_byteQueues[id].name[name_len] = '\0';

//    g_byteQueues[id].mutex = xSemaphoreCreateMutex();
//    if (g_byteQueues[id].mutex == NULL)
//    {
//        printf("%s: [%s] Mutex create fail\r\n", __func__, name);
//        free(g_byteQueues[id].buffer);
//        return BYTE_QUEUE_MUTEX_CREATE_FAIL;
//    }

    return BYTE_QUEUE_OK;
}

size_t ByteQueue_Count(ByteQueueId id)
{
    size_t count;

//    xSemaphoreTake(g_byteQueues[id].mutex, portMAX_DELAY);
    count = g_byteQueues[id].count;
//    xSemaphoreGive(g_byteQueues[id].mutex);

    return count;
}

void ByteQueue_Destroy(ByteQueueId id)
{
//    if (g_byteQueues[id].mutex != NULL)
//    {
////        vSemaphoreDelete(g_byteQueues[id].mutex);
////        g_byteQueues[id].mutex = NULL;
//    }

    free(g_byteQueues[id].buffer);
    memset(&g_byteQueues[id], 0, sizeof(ByteQueue));
}

void ByteQueue_Clear(ByteQueueId id)
{
    if (g_byteQueues[id].buffer == NULL)
    {
        printf("%s : Queue not created\r\n", __func__);
        return;
    }

//    xSemaphoreTake(g_byteQueues[id].mutex, portMAX_DELAY);

    g_byteQueues[id].count = 0;
    g_byteQueues[id].head = 0;
    g_byteQueues[id].tail = 0;

//    xSemaphoreGive(g_byteQueues[id].mutex);
}

ByteQueueStatus ByteQueue_Write(ByteQueueId id,
                                const unsigned char *data,
                                size_t len)
{
    ByteQueueStatus ret = BYTE_QUEUE_OK;
    unsigned int split;

    if (g_byteQueues[id].buffer == NULL)
    {
        printf("%s : Queue not created\r\n", __func__);
        return BYTE_QUEUE_MALLOC_FAIL;
    }

//    xSemaphoreTake(g_byteQueues[id].mutex, portMAX_DELAY);

    if (g_byteQueues[id].count + len > g_byteQueues[id].capacity)
    {
        printf("[%s] receive queue full\r\n", g_byteQueues[id].name);
        ret = BYTE_QUEUE_FULL;
    }
    else
    {
        if (g_byteQueues[id].tail + len <= g_byteQueues[id].capacity)
        {
            memcpy(g_byteQueues[id].buffer + g_byteQueues[id].tail, data, len);
            g_byteQueues[id].tail += len;

            if (g_byteQueues[id].tail == g_byteQueues[id].capacity)
                g_byteQueues[id].tail = 0;
        }
        else
        {
            split = g_byteQueues[id].capacity - g_byteQueues[id].tail;
            memcpy(g_byteQueues[id].buffer + g_byteQueues[id].tail, data, split);
            memcpy(g_byteQueues[id].buffer, data + split, len - split);
            g_byteQueues[id].tail = len - split;
        }

        g_byteQueues[id].count += len;
    }

//    xSemaphoreGive(g_byteQueues[id].mutex);
    return ret;
}

//ByteQueueStatus ByteQueue_WriteFromISR(ByteQueueId id,
//                                       const unsigned char *data,
//                                       size_t len,
//                                       BaseType_t *pxHigherPriorityTaskWoken)
//{
//    ByteQueueStatus ret = BYTE_QUEUE_OK;
//    unsigned int split;
//
//    (void)pxHigherPriorityTaskWoken;
//
//    if (g_byteQueues[id].buffer == NULL)
//        return BYTE_QUEUE_MALLOC_FAIL;
//
//    UBaseType_t state = taskENTER_CRITICAL_FROM_ISR();
//
//    if (g_byteQueues[id].count + len > g_byteQueues[id].capacity)
//    {
//        ret = BYTE_QUEUE_FULL;
//    }
//    else
//    {
//        if (g_byteQueues[id].tail + len <= g_byteQueues[id].capacity)
//        {
//            memcpy(g_byteQueues[id].buffer + g_byteQueues[id].tail, data, len);
//            g_byteQueues[id].tail += len;
//
//            if (g_byteQueues[id].tail == g_byteQueues[id].capacity)
//                g_byteQueues[id].tail = 0;
//        }
//        else
//        {
//            split = g_byteQueues[id].capacity - g_byteQueues[id].tail;
//            memcpy(g_byteQueues[id].buffer + g_byteQueues[id].tail, data, split);
//            memcpy(g_byteQueues[id].buffer, data + split, len - split);
//            g_byteQueues[id].tail = len - split;
//        }
//
//        g_byteQueues[id].count += len;
//    }
//
//    taskEXIT_CRITICAL_FROM_ISR(state);
//    return ret;
//}

int ByteQueue_Read(ByteQueueId id,
                   unsigned char *out,
                   size_t len)
{
    int ret = BYTE_QUEUE_OK;
    unsigned int split;

    if (g_byteQueues[id].buffer == NULL)
        return BYTE_QUEUE_MALLOC_FAIL;

//    xSemaphoreTake(g_byteQueues[id].mutex, portMAX_DELAY);

    if (g_byteQueues[id].count < len)
    {
        ret = BYTE_QUEUE_EMPTY;
    }
    else
    {
        if (g_byteQueues[id].head + len <= g_byteQueues[id].capacity)
        {
            memcpy(out, g_byteQueues[id].buffer + g_byteQueues[id].head, len);
            g_byteQueues[id].head += len;

            if (g_byteQueues[id].head == g_byteQueues[id].capacity)
                g_byteQueues[id].head = 0;
        }
        else
        {
            split = g_byteQueues[id].capacity - g_byteQueues[id].head;
            memcpy(out, g_byteQueues[id].buffer + g_byteQueues[id].head, split);
            g_byteQueues[id].head = 0;
            memcpy(out + split,
                   g_byteQueues[id].buffer + g_byteQueues[id].head,
                   len - split);
            g_byteQueues[id].head = len - split;
        }

        g_byteQueues[id].count -= len;
    }

//    xSemaphoreGive(g_byteQueues[id].mutex);
    return ret == BYTE_QUEUE_OK ? (int)len : ret;
}

int ByteQueue_Peek(ByteQueueId id,
                   unsigned char *out,
                   size_t len)
{
    int ret = BYTE_QUEUE_OK;
    unsigned int split;

    if (g_byteQueues[id].buffer == NULL)
        return BYTE_QUEUE_MALLOC_FAIL;

//    xSemaphoreTake(g_byteQueues[id].mutex, portMAX_DELAY);

    if (g_byteQueues[id].count < len)
    {
        ret = BYTE_QUEUE_EMPTY;
    }
    else
    {
        if (g_byteQueues[id].head + len <= g_byteQueues[id].capacity)
        {
            memcpy(out, g_byteQueues[id].buffer + g_byteQueues[id].head, len);
        }
        else
        {
            split = g_byteQueues[id].capacity - g_byteQueues[id].head;
            memcpy(out, g_byteQueues[id].buffer + g_byteQueues[id].head, split);
            memcpy(out + split,
                   g_byteQueues[id].buffer,
                   len - split);
        }
    }

//    xSemaphoreGive(g_byteQueues[id].mutex);
    return ret == BYTE_QUEUE_OK ? (int)len : ret;
}

ByteQueueStatus ByteQueue_Consume(ByteQueueId id,
                                  size_t len)
{
//    xSemaphoreTake(g_byteQueues[id].mutex, portMAX_DELAY);

    if (len > g_byteQueues[id].count)
    {
//        xSemaphoreGive(g_byteQueues[id].mutex);
        return BYTE_QUEUE_UNDERFLOW;
    }

    g_byteQueues[id].head =
            (g_byteQueues[id].head + len) % g_byteQueues[id].capacity;
    g_byteQueues[id].count -= len;

//    xSemaphoreGive(g_byteQueues[id].mutex);
    return BYTE_QUEUE_OK;
}

int ByteQueue_ReadMax(ByteQueueId id,
                      unsigned char *out,
                      size_t max_len)
{
    unsigned int split;
    unsigned int read_len;

    if (g_byteQueues[id].buffer == NULL)
        return BYTE_QUEUE_MALLOC_FAIL;

    if (g_byteQueues[id].count == 0)
        return BYTE_QUEUE_EMPTY;

//    xSemaphoreTake(g_byteQueues[id].mutex, portMAX_DELAY);

    read_len = (g_byteQueues[id].count < max_len)
               ? g_byteQueues[id].count
               : max_len;

    if (g_byteQueues[id].head + read_len <= g_byteQueues[id].capacity)
    {
        memcpy(out, g_byteQueues[id].buffer + g_byteQueues[id].head, read_len);
        g_byteQueues[id].head += read_len;

        if (g_byteQueues[id].head == g_byteQueues[id].capacity)
            g_byteQueues[id].head = 0;
    }
    else
    {
        split = g_byteQueues[id].capacity - g_byteQueues[id].head;
        memcpy(out, g_byteQueues[id].buffer + g_byteQueues[id].head, split);
        g_byteQueues[id].head = 0;
        memcpy(out + split,
               g_byteQueues[id].buffer + g_byteQueues[id].head,
               read_len - split);
        g_byteQueues[id].head = read_len - split;
    }

    g_byteQueues[id].count -= read_len;

//    xSemaphoreGive(g_byteQueues[id].mutex);
    return read_len;
}

int ByteQueue_PeekMax(ByteQueueId id,
                      unsigned char *out,
                      size_t max_len)
{
    unsigned int split;
    unsigned int peek_len;

    if (g_byteQueues[id].buffer == NULL)
        return BYTE_QUEUE_MALLOC_FAIL;

    if (g_byteQueues[id].count == 0)
        return BYTE_QUEUE_EMPTY;

//    xSemaphoreTake(g_byteQueues[id].mutex, portMAX_DELAY);

    peek_len = (g_byteQueues[id].count < max_len)
               ? g_byteQueues[id].count
               : max_len;

    if (g_byteQueues[id].head + peek_len <= g_byteQueues[id].capacity)
    {
        memcpy(out, g_byteQueues[id].buffer + g_byteQueues[id].head, peek_len);
    }
    else
    {
        split = g_byteQueues[id].capacity - g_byteQueues[id].head;
        memcpy(out, g_byteQueues[id].buffer + g_byteQueues[id].head, split);
        memcpy(out + split,
               g_byteQueues[id].buffer,
               peek_len - split);
    }

//    xSemaphoreGive(g_byteQueues[id].mutex);
    return peek_len;
}
