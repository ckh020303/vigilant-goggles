#ifndef __BSP_H_
#define __BSP_H_

#include "gpio.h"
#include "usart.h"
#include "main.h"

typedef void (*pFunction)(void);

void LED_G_WritePin(GPIO_PinState State);
void LED_G_TogglePin(void);
void LED_R_WritePin(GPIO_PinState State);
void LED_R_TogglePin(void);

void enterNormalMode(void);

FlagStatus GetReadOutProtectionStatus(void);
uint32_t FLASH_GetReadOutProtectionLevel(void);

uint8_t FLASH_Read(uint32_t Address);
void FLASH_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength);
void FLASH_Erase(uint8_t *p_Data, uint32_t DataLength);

void jump_to_app(uint32_t APP_FLASH_ADDR);

#endif
