#include <string.h>     /* memcpy */
#include "util_proto.h"

/*
 * Build a protocol packet.
 */
PROTOCOL_RET util_proto_MakePacket(t_Protocol *pst_Proto,
                                   unsigned char *puc_SendData,
                                   unsigned short us_SendLength)
{
    unsigned int uiOffset = 0;
    unsigned char ucCMDStart = CMD_REPLY_SENSOR_DATA;

    if (pst_Proto == NULL || puc_SendData == NULL)
        return NULL_FAIL;

    /* Check output buffer capacity */
    if ((pst_Proto->usLength + HEADER_SIZE + TAIL_SIZE) > us_SendLength)
        return LENGTH_FAIL;

    /* Start code */
    if (pst_Proto->ucStart != START_CODE)
        return START_CODE_FAIL;

    puc_SendData[uiOffset++] = pst_Proto->ucStart;

    /* Payload length (little endian) */
    puc_SendData[uiOffset++] = (unsigned char)(pst_Proto->usLength & 0xFF);
    puc_SendData[uiOffset++] = (unsigned char)((pst_Proto->usLength >> 8) & 0xFF);

    /* Command validation */
    for (int i = 0; i < CMD_NUM; i++)
    {
        if (pst_Proto->ucCmd != (ucCMDStart + i))
            return CMD_FAIL;
    }

    /* Command */
    puc_SendData[uiOffset++] = pst_Proto->ucCmd;

    /* Payload */
    memcpy(&puc_SendData[uiOffset], pst_Proto->ucData, pst_Proto->usLength);
    uiOffset += pst_Proto->usLength;

    /* End code */
    if (pst_Proto->ucEnd != END_CODE)
        return END_CODE_FAIL;

    puc_SendData[uiOffset++] = pst_Proto->ucEnd;

    return (PROTOCOL_RET)uiOffset;
}

/*
 * Parse a received packet.
 */
unsigned int util_proto_Parsing(t_Protocol *pst_Proto,
                                unsigned char *puc_RecvData,
                                unsigned short us_RecvLength)
{
    unsigned int uiOffset = 0;

    if (pst_Proto == NULL || puc_RecvData == NULL)
        return NULL_FAIL;

    /* Minimum packet size check */
    if (us_RecvLength < (HEADER_SIZE + TAIL_SIZE))
        return LENGTH_FAIL;

    /* Start code */
    if (puc_RecvData[uiOffset] != START_CODE)
        return START_CODE_FAIL;

    pst_Proto->ucStart = puc_RecvData[uiOffset++];

    /* Length */
    pst_Proto->usLength =
            (unsigned short)((puc_RecvData[uiOffset + 1] << 8) |
                             puc_RecvData[uiOffset]);
    uiOffset += 2;

    /* Validate total length */
    if (us_RecvLength < (HEADER_SIZE + pst_Proto->usLength + TAIL_SIZE))
        return LENGTH_FAIL;

    /* Command */
    pst_Proto->ucCmd = puc_RecvData[uiOffset++];

    /* Payload */
    memcpy(pst_Proto->ucData,
           puc_RecvData + uiOffset,
           pst_Proto->usLength);
    uiOffset += pst_Proto->usLength;

    /* End code */
    if (puc_RecvData[uiOffset] != END_CODE)
        return END_CODE_FAIL;

    pst_Proto->ucEnd = END_CODE;
    uiOffset++;

    return uiOffset;
}