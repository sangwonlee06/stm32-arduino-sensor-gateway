#ifndef UTIL_PROTO_H
#define UTIL_PROTO_H


#ifdef __cplusplus
extern "C" {
#endif

/* INCLUDES */
#include <stdint.h>



/* PROTOCOL CONSTANTS */
#define START_CODE               0xAA
#define END_CODE                 0xBB

#define CMD_REQ_SENSOR_DATA      0x01
#define CMD_REPLY_SENSOR_DATA    0x81

#define CMD_NUM                  1

/*
 * Packet format:
 *   [START][LEN_L][LEN_H][CMD][PAYLOAD ...][END]
 */
#define HEADER_SIZE              4   /* start(1) + length(2) + cmd(1) */
#define TAIL_SIZE                1   /* end(1) */

#define UART_RECV_DATA_SIZE      1024

/* RETURN CODES (values preserved) */
typedef enum
{
    NO_ERR          = -1,
    CMD_FAIL        = -2,
    LENGTH_FAIL     = -3,
    NULL_FAIL       = -4,
    START_CODE_FAIL = -5,
    END_CODE_FAIL   = -6,
} PROTOCOL_RET;

/* PROTOCOL FRAME STRUCTURE */
typedef struct
{
    unsigned char  ucStart;
    unsigned short usLength;
    unsigned char  ucCmd;
    unsigned char  ucData[UART_RECV_DATA_SIZE];
    unsigned char  ucEnd;
} __attribute__((packed)) t_Protocol;

/* API */
PROTOCOL_RET util_proto_MakePacket(t_Protocol *pst_Proto,
                                   unsigned char *puc_SendData,
                                   unsigned short us_SendLength);

unsigned int util_proto_Parsing(t_Protocol *pst_Proto,
                                unsigned char *puc_RecvData,
                                unsigned short us_RecvLength);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_PROTO_H */