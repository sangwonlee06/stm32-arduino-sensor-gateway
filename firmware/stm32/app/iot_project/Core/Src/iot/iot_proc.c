/* INCLUDE */
#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "byte_queue.h"
#include "util_proto.h"   // <-- needed for t_Protocol, HEADER_SIZE, TAIL_SIZE, START_CODE, END_CODE, util_proto_Parsing

/* DEFINEs */
#define DEQUEUE_BUFFER_SIZE 1024

/* GLOBAL VARIABLES */
static unsigned char gauc_DequeueBuffer[DEQUEUE_BUFFER_SIZE];

/* FUNCTIONS */
static void iot_proc_ReplySensorData(t_Protocol *pst_Protocol)
{
    (void)pst_Protocol;
    // todo: handle received sensor data
}

static void iot_proc_MainTask(void *pvParm)
{
    (void)pvParm;

    size_t uiQueueCnt;
    unsigned short usDataLength;
    t_Protocol stProtocol;

    while (1)
    {
        uiQueueCnt = ByteQueue_Count(BYTE_QUEUE_UART_ARD);

        /* Need at least header + tail */
        if (uiQueueCnt < (HEADER_SIZE + TAIL_SIZE))
        {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        /* Peek header (do not consume yet) */
        if (ByteQueue_Peek(BYTE_QUEUE_UART_ARD, gauc_DequeueBuffer, HEADER_SIZE) < 0)
        {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        /* Resync if start code is not aligned */
        if (gauc_DequeueBuffer[0] != START_CODE)
        {
            /* Drop 1 byte and retry */
            (void)ByteQueue_Consume(BYTE_QUEUE_UART_ARD, 1);
            continue;
        }

        /* Extract payload length (little endian) from header */
        usDataLength = (unsigned short)(
                ((unsigned short)gauc_DequeueBuffer[2] << 8) |
                (unsigned short)gauc_DequeueBuffer[1]
        );

        /* Full frame size = header + payload + tail */
        size_t frame_len = (size_t)HEADER_SIZE + (size_t)usDataLength + (size_t)TAIL_SIZE;

        /* Ensure our local buffer can hold it */
        if (frame_len > DEQUEUE_BUFFER_SIZE)
        {
            /* Packet claims absurd length; drop start byte and resync */
            (void)ByteQueue_Consume(BYTE_QUEUE_UART_ARD, 1);
            continue;
        }

        /* Wait until full packet is available */
        uiQueueCnt = ByteQueue_Count(BYTE_QUEUE_UART_ARD);
        if (uiQueueCnt < frame_len)
        {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        /* Peek entire frame */
        if (ByteQueue_Peek(BYTE_QUEUE_UART_ARD, gauc_DequeueBuffer, frame_len) < 0)
        {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        /* Validate end code before consuming */
        if (gauc_DequeueBuffer[frame_len - 1] != END_CODE)
        {
            /* Bad framing; drop 1 byte and resync */
            (void)ByteQueue_Consume(BYTE_QUEUE_UART_ARD, 1);
            continue;
        }

        /* Now consume the packet from the queue */
        (void)ByteQueue_Consume(BYTE_QUEUE_UART_ARD, frame_len);

        /* Parse into struct */
        (void)util_proto_Parsing(&stProtocol,
                                 gauc_DequeueBuffer,
                                 (unsigned short)frame_len);

        /* Dispatch by cmd */
        if (stProtocol.ucCmd == CMD_REPLY_SENSOR_DATA)
        {
            iot_proc_ReplySensorData(&stProtocol);
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void iot_proc_Init(void)
{
    BaseType_t iRet;

    iRet = xTaskCreate(iot_proc_MainTask,
                       "proc_main_task",
                       256,
                       NULL,
                       (tskIDLE_PRIORITY + 1),
                       NULL);

    if (iRet != pdPASS)
    {
        printf("%s : process task create fail\r\n", __func__);
    }
}