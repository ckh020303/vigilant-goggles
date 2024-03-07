#ifndef __BSP_H_
#define __BSP_H_

#include "gpio.h"
#include "usart.h"
#include "tim.h"

void LED_G_WritePin(GPIO_PinState State);
void LED_G_TogglePin(void);
void TIM_SetTime(uint8_t delay_ms);
void TIM_Start(void);
void TIM_Stop(void);

#endif
