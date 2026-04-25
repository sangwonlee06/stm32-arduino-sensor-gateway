#include <string.h>

#include "uart_packet.h"

static bool UartPacket_IsLengthValid(uint16_t len)
{
    return (len <= UART_PACKET_MAX_PAYLOAD);
}

bool UartPacket_IsCommandValid(uint8_t cmd)
{
    switch (cmd)
    {
        case UART_CMD_REQ_SENSOR_DATA:
        case UART_CMD_REQ_REBOOT:
        case UART_CMD_REPLY_SENSOR_DATA:
        case UART_CMD_REPLY_REBOOT:
            return true;

        default:
            return false;
    }
}

void UartPacket_Init(UartPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    memset(packet, 0, sizeof(UartPacket));
    packet->start = UART_PACKET_START_BYTE;
    packet->end = UART_PACKET_END_BYTE;
}

UartPacketStatus UartPacket_Validate(const UartPacket *packet)
{
    if (packet == NULL)
    {
        return UART_PACKET_STATUS_NULL;
    }

    if (packet->start != UART_PACKET_START_BYTE)
    {
        return UART_PACKET_STATUS_INVALID_START;
    }

    if (packet->end != UART_PACKET_END_BYTE)
    {
        return UART_PACKET_STATUS_INVALID_END;
    }

    if (!UartPacket_IsLengthValid(packet->length))
    {
        return UART_PACKET_STATUS_INVALID_LENGTH;
    }

    if (!UartPacket_IsCommandValid(packet->cmd))
    {
        return UART_PACKET_STATUS_INVALID_COMMAND;
    }

    switch (packet->cmd)
    {
        case UART_CMD_REQ_SENSOR_DATA:
        case UART_CMD_REQ_REBOOT:
            if (packet->length != 0u)
            {
                return UART_PACKET_STATUS_INVALID_LENGTH;
            }
            break;

        case UART_CMD_REPLY_SENSOR_DATA:
            if (packet->length != UART_PACKET_SENSOR_REPLY_SIZE)
            {
                return UART_PACKET_STATUS_INVALID_LENGTH;
            }
            break;

        case UART_CMD_REPLY_REBOOT:
            if (packet->length != UART_PACKET_REBOOT_REPLY_SIZE)
            {
                return UART_PACKET_STATUS_INVALID_LENGTH;
            }
            break;

        default:
            return UART_PACKET_STATUS_INVALID_COMMAND;
    }

    return UART_PACKET_STATUS_OK;
}

void UartPacket_MakeSensorRequest(UartPacket *packet)
{
    UartPacket_Init(packet);
    if (packet == NULL)
    {
        return;
    }

    packet->cmd = UART_CMD_REQ_SENSOR_DATA;
}

void UartPacket_MakeRebootRequest(UartPacket *packet)
{
    UartPacket_Init(packet);
    if (packet == NULL)
    {
        return;
    }

    packet->cmd = UART_CMD_REQ_REBOOT;
}

UartPacketStatus UartPacket_MakeSensorReplyMm(UartPacket *packet, uint16_t distance_mm)
{
    if (packet == NULL)
    {
        return UART_PACKET_STATUS_NULL;
    }

    UartPacket_Init(packet);
    packet->cmd = UART_CMD_REPLY_SENSOR_DATA;
    packet->length = UART_PACKET_SENSOR_REPLY_SIZE;
    packet->data[0] = (uint8_t)(distance_mm & 0xFFu);
    packet->data[1] = (uint8_t)((distance_mm >> 8) & 0xFFu);
    return UART_PACKET_STATUS_OK;
}

UartPacketStatus UartPacket_MakeRebootReply(UartPacket *packet, uint8_t status_code)
{
    if (packet == NULL)
    {
        return UART_PACKET_STATUS_NULL;
    }

    UartPacket_Init(packet);
    packet->cmd = UART_CMD_REPLY_REBOOT;
    packet->length = UART_PACKET_REBOOT_REPLY_SIZE;
    packet->data[0] = status_code;
    return UART_PACKET_STATUS_OK;
}

void UartParser_Init(UartParserContext *ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    memset(ctx, 0, sizeof(UartParserContext));
    UartPacket_Init(&ctx->packet);
    ctx->state = UART_PARSER_WAIT_START;
    ctx->last_error = UART_PACKET_STATUS_OK;
}

void UartParser_Reset(UartParserContext *ctx)
{
    UartParser_Init(ctx);
}

UartParserResult UartParser_PushByte(UartParserContext *ctx,
                                     uint8_t rx_byte,
                                     UartPacket *out_packet)
{
    if ((ctx == NULL) || (out_packet == NULL))
    {
        return UART_PARSER_ERROR;
    }

    switch (ctx->state)
    {
        case UART_PARSER_WAIT_START:
            if (rx_byte == UART_PACKET_START_BYTE)
            {
                UartPacket_Init(&ctx->packet);
                ctx->packet.start = rx_byte;
                ctx->state = UART_PARSER_LEN_L;
            }
            return UART_PARSER_PROGRESS;

        case UART_PARSER_LEN_L:
            ctx->packet.length = (uint16_t)rx_byte;
            ctx->state = UART_PARSER_LEN_H;
            return UART_PARSER_PROGRESS;

        case UART_PARSER_LEN_H:
            ctx->packet.length |= (uint16_t)((uint16_t)rx_byte << 8);

            if (!UartPacket_IsLengthValid(ctx->packet.length))
            {
                UartParser_Reset(ctx);
                ctx->last_error = UART_PACKET_STATUS_INVALID_LENGTH;
                return UART_PARSER_ERROR;
            }

            ctx->state = UART_PARSER_CMD;
            return UART_PARSER_PROGRESS;

        case UART_PARSER_CMD:
            if (!UartPacket_IsCommandValid(rx_byte))
            {
                UartParser_Reset(ctx);
                ctx->last_error = UART_PACKET_STATUS_INVALID_COMMAND;
                return UART_PARSER_ERROR;
            }

            ctx->packet.cmd = rx_byte;
            ctx->data_index = 0;

            if (ctx->packet.length == 0u)
            {
                ctx->state = UART_PARSER_END;
            }
            else
            {
                ctx->state = UART_PARSER_DATA;
            }
            return UART_PARSER_PROGRESS;

        case UART_PARSER_DATA:
            ctx->packet.data[ctx->data_index++] = rx_byte;
            if (ctx->data_index >= ctx->packet.length)
            {
                ctx->state = UART_PARSER_END;
            }
            return UART_PARSER_PROGRESS;

        case UART_PARSER_END:
            if (rx_byte != UART_PACKET_END_BYTE)
            {
                UartParser_Reset(ctx);
                ctx->last_error = UART_PACKET_STATUS_INVALID_END;
                return UART_PARSER_ERROR;
            }

            ctx->packet.end = rx_byte;

            {
                UartPacketStatus status = UartPacket_Validate(&ctx->packet);
                if (status != UART_PACKET_STATUS_OK)
                {
                    UartParser_Reset(ctx);
                    ctx->last_error = status;
                    return UART_PARSER_ERROR;
                }
            }

            memcpy(out_packet, &ctx->packet, sizeof(UartPacket));
            UartParser_Reset(ctx);
            return UART_PARSER_PACKET_READY;

        default:
            UartParser_Reset(ctx);
            return UART_PARSER_ERROR;
    }
}

UartPacketStatus UartPacket_BuildFrame(const UartPacket *packet,
                                       uint8_t *out_frame,
                                       uint16_t out_frame_capacity,
                                       uint16_t *out_frame_length)
{
    uint16_t idx = 0;
    uint16_t frame_size;
    UartPacketStatus status;

    if ((packet == NULL) || (out_frame == NULL) || (out_frame_length == NULL))
    {
        return UART_PACKET_STATUS_NULL;
    }

    status = UartPacket_Validate(packet);
    if (status != UART_PACKET_STATUS_OK)
    {
        return status;
    }

    frame_size = (uint16_t)(UART_PACKET_OVERHEAD_SIZE + packet->length);
    if (out_frame_capacity < frame_size)
    {
        return UART_PACKET_STATUS_BUFFER_TOO_SMALL;
    }

    out_frame[idx++] = UART_PACKET_START_BYTE;
    out_frame[idx++] = (uint8_t)(packet->length & 0xFFu);
    out_frame[idx++] = (uint8_t)((packet->length >> 8) & 0xFFu);
    out_frame[idx++] = packet->cmd;

    if (packet->length > 0u)
    {
        memcpy(&out_frame[idx], packet->data, packet->length);
        idx = (uint16_t)(idx + packet->length);
    }

    out_frame[idx++] = UART_PACKET_END_BYTE;
    *out_frame_length = idx;
    return UART_PACKET_STATUS_OK;
}
