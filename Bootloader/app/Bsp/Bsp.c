#include "../../app/Bsp/Bsp.h"

void LED_G_WritePin(GPIO_PinState State)
{
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin,State);
}

void LED_G_TogglePin(void)
{
	HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		LED_G_TogglePin();
	}
}

void TIM_SetTime(uint8_t delay_ms)
{
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 6399;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = delay_ms*10-1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
	Error_Handler();
	}
}

void TIM_Start(void)
{
	__HAL_TIM_SET_COUNTER(&htim3,0);
	__HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&htim3);
}

void TIM_Stop(void)
{
	__HAL_TIM_SET_COUNTER(&htim3,0);
	__HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Stop_IT(&htim3);
}

