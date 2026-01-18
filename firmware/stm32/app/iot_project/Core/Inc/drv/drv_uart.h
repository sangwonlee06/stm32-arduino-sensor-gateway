#ifndef _DRV_UART_H_
#define _DRV_UART_H_

/* INCLUDE */
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"   // or your project HAL header

/* DEFINEs */
#define UART_ASYNC_SRTACH_SIZE      256u

/* TYPEDEFS & STRUCTURES */
typedef enum
{
    UART_ARDU = 0,
    UART_INDEX_MAX,
} UART_INDEX;

typedef struct
{
    UART_HandleTypeDef *huart;
    bool bBusy;
    uint16_t usLength;
    uint8_t aucBuffer[UART_ASYNC_SRTACH_SIZE];
} __attribute__((packed)) t_UartAsnyc;

/* FUNCTIONS */
void Drv_Uart_Init(UART_HandleTypeDef *huart1);
bool Drv_Uart_Send(UART_INDEX index, const uint8_t *puc_Buffer, uint16_t us_Length);

#endif /* _DRV_UART_H_ */