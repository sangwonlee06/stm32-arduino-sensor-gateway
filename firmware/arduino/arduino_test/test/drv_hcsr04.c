//
// Created by Sangwon Lee on 2/1/26.
//

#include <Arduino.h>

#define TRIG_DELAY          10
#define ECHO_TIMEOUT        30000UL // us 30ms -> distance 5m

static unsigned char guc_InitStatus = 0;
static unsigned long gul_RawData;

static int gi_TrigPin = 3; // initial value
static int gi_EchoPin = 4; // initial value

static const float gf_SonarSpeed = 0.0340f;


static float drv_hcsr04_Cal()
{
  if (gul_RawData <= 0)
    return 0.0f;

  return (gul_RawData * gf_SonarSpeed) / 2.0f; //cm
    // calculate length in cm from the sensor data
}

void drv_hcsr04_Init(int i_TrigPinNum, int i_EchoPinNum)
{
    pinMode(i_TrigPinNum, OUTPUT);
    pinMode(i_EchoPinNum, INPUT);
    // pin2 -> trig echo -> configure pinmode
    digitalWrite(i_TrigPinNum, LOW);

    guc_InitStatus = 1;
}

float drv_hr04_GetRawData()
{
  if (!guc_InitStatus)
      return -1;
    //echo -> raw data return

  return gul_RawData;

}

float drv_hcsr04_GetCmData()
{
  if (!guc_InitStatus)
      return -1;

  return drv_hcsr04_Cal();
    //echo -> length data return in cm

}

void drv_hcsr04_SendSonar()
{
    if (!guc_InitStatus)
        return;

    unsigned long ulDuration = 0;

    digitalWrite(gi_TrigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(gi_TrigPin, HIGH);
    delayMicroseconds(TRIG_DELAY);
    digitalWrite(gi_TrigPin, LOW);

    ulDuration = pulseIn(gi_EchoPin, HIGH, ECHO_TIMEOUT);
    gul_RawData = ulDuration;
    // trig -> sonar send
}