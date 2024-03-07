#ifndef __BSP_H_
#define __BSP_H_

#include "gpio.h"
#include "fdcan.h"
#include "CanHub.h"
#include "CanDetector.h"
#include "tim.h"

void Relay_WritePin(GPIO_PinState State);
void LED_G_WritePin(GPIO_PinState State);
void LED_G_TogglePin(void);
void LED_R_WritePin(GPIO_PinState State);
void LED_R_TogglePin(void);
uint8_t getValue(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void enterGotoSleepCommandMode(void);
void enterPownMode(void);
void enterNormalMode(void);

void FDCAN_start(void);
void FDCAN_SendData(FDCAN_HandleTypeDef hfdcan,FDCAN_TxHeaderTypeDef TxHeader,uint8_t *pdata);
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

void Timer_Start(TIM_HandleTypeDef htim);
void Timer_Stop(TIM_HandleTypeDef htim);
void Timer_Reload(TIM_HandleTypeDef htim);
void Timer_Reload_Start(TIM_HandleTypeDef htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


#endif
