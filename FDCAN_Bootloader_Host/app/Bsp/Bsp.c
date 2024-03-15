#include "../../app/Bsp/Bsp.h"

void LED_G_WritePin(GPIO_PinState State)
{
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin,State);
}

void LED_G_TogglePin(void)
{
	HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
}

void LED_R_WritePin(GPIO_PinState State)
{
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin,State);
}

void LED_R_TogglePin(void)
{
	HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
}

void enterNormalMode(void)									//正常模式
{
	HAL_GPIO_WritePin(FDCAN1_STB_GPIO_Port, FDCAN1_STB_Pin, GPIO_PIN_SET);		//CAN1_STB
	HAL_GPIO_WritePin(FDCAN1_EN_GPIO_Port, FDCAN1_EN_Pin, GPIO_PIN_SET);		//CAN1_EN
}

FlagStatus GetReadOutProtectionStatus(void)
{
	FlagStatus status;

	if (FLASH_GetReadOutProtectionLevel() != OB_RDP_LEVEL_0)
	{
		status = SET;
	}
	else
	{
		status = RESET;
	}

	return status;
}

uint32_t FLASH_GetReadOutProtectionLevel(void)
{
	FLASH_OBProgramInitTypeDef flash_ob;

	/* Get the Option bytes configuration */
	HAL_FLASHEx_OBGetConfig(&flash_ob);

	return flash_ob.RDPLevel;
}

uint8_t FLASH_Read(uint32_t Address)
{
	return (*(uint8_t *)(Address));
}

