#include "drv_hcsr04.h"

#define TRIG_PIN      3
#define ECHO_PIN      4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("TEST CODE");

  drv_hcsr04_Init(TRIG_PIN, ECHO_PIN);

}

void loop() {
  drv_hcsr04_SendSonar();

  float fCm = drv_hcsr04_GetCmData();

  Serial.print("cm = ");
  Serial.println(fCm);

  delay(1000);

}
