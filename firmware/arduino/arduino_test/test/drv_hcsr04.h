//
// Created by Sangwon Lee on 2/1/26.
//



#ifndef ARDUINO_TEST_DRV_HCSR04_H
#define ARDUINO_TEST_DRV_HCSR04_H

#ifdef __cplusplus
extern "C" {
#endif


void drv_hcsr04_Init(int i_TrigPinNum, int i_EchoPinNum);
float drv_hr04_GetRawData();
float drv_hcsr04_GetCmData();
void drv_hcsr04_SendSonar();

#ifdef __cplusplus
}

#endif //ARDUINO_TEST_DRV_HCSR04_H

#endif
