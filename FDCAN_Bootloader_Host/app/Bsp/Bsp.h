#ifndef __BSP_H_
#define __BSP_H_

#include "gpio.h"
#include "usart.h"
#include "main.h"

void LED_G_WritePin(GPIO_PinState State);
void LED_G_TogglePin(void);
void LED_R_WritePin(GPIO_PinState State);
void LED_R_TogglePin(void);

void enterNormalMode(void);

FlagStatus GetReadOutProtectionStatus(void);
uint32_t FLASH_GetReadOutProtectionLevel(void);
uint8_t FLASH_Read(uint32_t Address);

#endif
