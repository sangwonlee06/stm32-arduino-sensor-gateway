/* INCLUDE */
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "drv_uart.h"
#include "byte_queue.h"

#include "FreeRTOS.h"
#include "task.h"

/* DEFINEs */
#define UART_ARDU_QUEUE_SIZE        2048u
#define NUM_RX_INT                  1u

/* GLOBAL VARIABLES */
static uint8_t guc_ArduReadBuffer;
static t_UartAsnyc gst_UartArdu;

/* LOCAL FUNCTIONS */
static void Drv_Uart_AsyncInit(t_UartAsnyc *pst_Uart, UART_HandleTypeDef *huart)
{
    if (!pst_Uart) return;

    pst_Uart->huart    = huart;
    pst_Uart->bBusy    = false;
    pst_Uart->usLength = 0;
}

static void Drv_Uart_AsyncTxDone(t_UartAsnyc *pst_Uart)
{
    if (!pst_Uart) return;
    pst_Uart->bBusy = false;
}

static bool Drv_Uart_AsyncSend(t_UartAsnyc *pst_Uart,
                               uint8_t *puc_Buffer,
                               uint16_t us_Length)
{
    if (!pst_Uart || !pst_Uart->huart || !puc_Buffer ||
        us_Length == 0 || us_Length > UART_ASYNC_SRTACH_SIZE)
        return false;

    uint32_t uiPrimask = __get_PRIMASK();
    __disable_irq();

    if ((pst_Uart->bBusy == true) ||
        (pst_Uart->huart->gState != HAL_UART_STATE_READY))
    {
        if (!uiPrimask) __enable_irq();
        return false;
    }

    pst_Uart->bBusy = true;

    memcpy(pst_Uart->aucBuffer, puc_Buffer, us_Length);
    pst_Uart->usLength = us_Length;

    HAL_StatusTypeDef tRet =
            HAL_UART_Transmit_IT(pst_Uart->huart, pst_Uart->aucBuffer, pst_Uart->usLength);

    if (!uiPrimask) __enable_irq();

    if (tRet != HAL_OK)
    {
        pst_Uart->bBusy = false;
        return false;
    }

    return true;
}

/* PUBLIC FUNCTIONS */
bool Drv_Uart_Send(UART_INDEX index, const uint8_t *puc_Buffer, uint16_t us_Length)
{
    t_UartAsnyc *pst_Uart = NULL;

    switch (index)
    {
        case UART_ARDU:
            pst_Uart = &gst_UartArdu;
            break;                 /* FIX: missing break in original */
        default:
            return false;
    }

    return Drv_Uart_AsyncSend(pst_Uart, (uint8_t *)puc_Buffer, us_Length);
}

void Drv_Uart_Init(UART_HandleTypeDef *huart1)
{
    /* 1) Create ByteQueue */
    const char *qname = "ardu_uart_queue";

    ByteQueueStatus qret = ByteQueue_Create(
            BYTE_QUEUE_UART_ARD,
            UART_ARDU_QUEUE_SIZE,
            qname,
            strlen(qname)
    );

    if (qret != BYTE_QUEUE_OK)
    {
        printf("%s : Arduino ByteQueue Create Fail! (%d)\r\n", __func__, (int)qret);
        while (1) { ; }
    }

    /* 2) Init UART async state */
    Drv_Uart_AsyncInit(&gst_UartArdu, huart1);

    /* 3) Arm first RX interrupt */
    if (HAL_UART_Receive_IT(huart1, &guc_ArduReadBuffer, NUM_RX_INT) != HAL_OK)
    {
        printf("%s : HAL_UART_Receive_IT fail\r\n", __func__);
        while (1) { ; }
    }
}

/* HAL CALLBACKS */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        /* enqueue received byte(s) */
        (void)ByteQueue_WriteFromISR(
                BYTE_QUEUE_UART_ARD,
                &guc_ArduReadBuffer,
                NUM_RX_INT,
                &xHigherPriorityTaskWoken
        );

        /* CRITICAL: re-arm the next RX interrupt */
        (void)HAL_UART_Receive_IT(huart, &guc_ArduReadBuffer, NUM_RX_INT);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        Drv_Uart_AsyncTxDone(&gst_UartArdu);
    }
}