#ifndef UART_PACKET_H
#define UART_PACKET_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Frame bytes */
#define UART_PACKET_START_BYTE      (0xAAu)
#define UART_PACKET_END_BYTE        (0xBBu)

/* Layout sizes */
#define UART_PACKET_HEADER_SIZE     (4u)  /* start(1) + len(2) + cmd(1) */
#define UART_PACKET_TAIL_SIZE       (1u)  /* end(1) */
#define UART_PACKET_OVERHEAD_SIZE   (UART_PACKET_HEADER_SIZE + UART_PACKET_TAIL_SIZE)

/* Max payload for beginner-friendly fixed buffer handling */
#define UART_PACKET_MAX_PAYLOAD     (256u)
#define UART_PACKET_SENSOR_REPLY_SIZE (2u)
#define UART_PACKET_REBOOT_REPLY_SIZE (1u)

typedef enum
{
    UART_CMD_REQ_SENSOR_DATA  = 0x01u,
    UART_CMD_REQ_REBOOT       = 0x02u,
    UART_CMD_REPLY_SENSOR_DATA = 0x81u,
    UART_CMD_REPLY_REBOOT      = 0x82u,
} UartPacketCommand;

typedef enum
{
    UART_PACKET_STATUS_OK = 0,
    UART_PACKET_STATUS_NULL,
    UART_PACKET_STATUS_INVALID_START,
    UART_PACKET_STATUS_INVALID_END,
    UART_PACKET_STATUS_INVALID_LENGTH,
    UART_PACKET_STATUS_INVALID_COMMAND,
    UART_PACKET_STATUS_BUFFER_TOO_SMALL,
} UartPacketStatus;

typedef struct
{
    uint8_t start;
    uint16_t length;
    uint8_t cmd;
    uint8_t data[UART_PACKET_MAX_PAYLOAD];
    uint8_t end;
} UartPacket;

typedef enum
{
    UART_PARSER_WAIT_START = 0,
    UART_PARSER_LEN_L,
    UART_PARSER_LEN_H,
    UART_PARSER_CMD,
    UART_PARSER_DATA,
    UART_PARSER_END,
} UartParserState;

typedef struct
{
    UartParserState state;
    UartPacket packet;
    uint16_t data_index;
    UartPacketStatus last_error;
} UartParserContext;

typedef enum
{
    UART_PARSER_PROGRESS = 0,
    UART_PARSER_PACKET_READY,
    UART_PARSER_ERROR,
} UartParserResult;

void UartPacket_Init(UartPacket *packet);
bool UartPacket_IsCommandValid(uint8_t cmd);
UartPacketStatus UartPacket_Validate(const UartPacket *packet);

void UartPacket_MakeSensorRequest(UartPacket *packet);
void UartPacket_MakeRebootRequest(UartPacket *packet);
UartPacketStatus UartPacket_MakeSensorReplyMm(UartPacket *packet, uint16_t distance_mm);
UartPacketStatus UartPacket_MakeRebootReply(UartPacket *packet, uint8_t status_code);

void UartParser_Init(UartParserContext *ctx);
void UartParser_Reset(UartParserContext *ctx);

UartParserResult UartParser_PushByte(UartParserContext *ctx,
                                     uint8_t rx_byte,
                                     UartPacket *out_packet);

UartPacketStatus UartPacket_BuildFrame(const UartPacket *packet,
                                       uint8_t *out_frame,
                                       uint16_t out_frame_capacity,
                                       uint16_t *out_frame_length);

#ifdef __cplusplus
}
#endif

#endif /* UART_PACKET_H */
