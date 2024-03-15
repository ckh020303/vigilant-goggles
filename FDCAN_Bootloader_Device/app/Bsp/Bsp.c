#include "../../app/Bsp/Bsp.h"

#define FLASH_FLAG_ALL_ERRORS           (FLASH_FLAG_OPERR   | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | \
                                         FLASH_FLAG_PGAERR  | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR | \
                                         FLASH_FLAG_MISERR  | FLASH_FLAG_FASTERR | \
                                         FLASH_FLAG_OPTVERR | FLASH_FLAG_ECCC    | FLASH_FLAG_ECCD)

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

void FLASH_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
	uint32_t index  = 0U;
	uint32_t length = DataLength;

	/* Unlock the flash memory for write operation */
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

	for (index = 0U; index < length; (index += 8U))
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (Address + index), *(uint64_t *)(&Data[index]));
	}

	/* Lock the Flash to disable the flash control register access */
	HAL_FLASH_Lock();
}

void FLASH_Erase(uint8_t *p_Data, uint32_t DataLength)
{
	uint32_t counter      = 0U;
	uint32_t pages_number = 0U;
	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef pEraseInit;

	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

	pages_number  = (uint32_t)(*(uint16_t *)(p_Data));
	p_Data += 2;

	pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	pEraseInit.Banks = FLASH_BANK_1;
	pEraseInit.NbPages   = 1U;

	for (counter = 0U; ((counter < pages_number) && (counter < DataLength)) ; counter++)
	{
		pEraseInit.Page = ((uint32_t)(*(uint16_t *)(p_Data)));
		HAL_FLASHEx_Erase(&pEraseInit, &PageError);
		p_Data += 2;
	}

	HAL_FLASH_Lock();
}
