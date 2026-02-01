#include <SoftwareSerial.h>
#include "arduino_test_proc.h"
#include "util_proto.h"

#define TX_PIN        10
#define RX_PIN        11

SoftwareSerial g_mySerial(RX_PIN, TX_PIN);

void dumpHex(const unsigned char *buf, unsigned int len)
{
  for (unsigned int i = 0; i < len; i++)
  {
    if (buf[i] < 0x10)
      Serial.print('0');

    Serial.print(buf[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  g_mySerial.begin(115200);

  Serial.println("TEST Loop back");

  unsigned char ucData[4] = {0};
        unsigned char ucEnqueueBuffer[256];
        t_Protocol stTestProto;

        memset(ucEnqueueBuffer, 0, sizeof(ucEnqueueBuffer));
        memset(&stTestProto, 0, sizeof(stTestProto));

        /* Fill protocol fields */
        stTestProto.ucStart  = START_CODE;
        stTestProto.usLength = (unsigned short)sizeof(ucData);
        stTestProto.ucCmd    = CMD_REPLY_SENSOR_DATA;
        memcpy(&stTestProto.ucData, ucData, 4);
        stTestProto.ucEnd    = END_CODE;

        {
            PROTOCOL_RET ret =  util_proto_MakePacket(&stTestProto,
                                                     ucEnqueueBuffer,
                                                     (unsigned short)sizeof(ucEnqueueBuffer));

            if (ret < NO_ERR)
            {
                Serial.print(__func__);
                Serial.print(" : make packet fail (");
                Serial.print((int)ret);
                Serial.println(")");
            }
            else
            {
                /* Frame length = header + payload + tail */
                unsigned short usFrameLen =
                    (unsigned short)(HEADER_SIZE + stTestProto.usLength + TAIL_SIZE);

                dumpHex(ucEnqueueBuffer, usFrameLen);

                // (void)ByteQueue_Write(BYTE_QUEUE_UART_ARD,
                //                       ucEnqueueBuffer,
                //                       usFrameLen);
            }
        }
}

void loop() {
  g_mySerial.write(0xAB);
  
  delay(1000);

}