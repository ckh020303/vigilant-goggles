#include "Bsp.h"

void Relay_WritePin(GPIO_PinState State)
{
	HAL_GPIO_WritePin(RELAY_CTRL_GPIO_Port,RELAY_CTRL_Pin,State);
}

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
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin,State);
}

void LED_R_TogglePin(void)
{
	HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
}

uint8_t getValue(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	uint8_t status;
	status = HAL_GPIO_ReadPin(GPIOx,GPIO_Pin);
	return status;
}

void enterGotoSleepCommandMode(void)						//睡眠模式
{
	HAL_GPIO_WritePin(FDCAN1_STB_GPIO_Port, FDCAN1_STB_Pin, GPIO_PIN_RESET);		//CAN1_STB
	HAL_GPIO_WritePin(FDCAN2_STB_GPIO_Port, FDCAN2_STB_Pin, GPIO_PIN_RESET);		//CAN2_STB

	HAL_GPIO_WritePin(FDCAN1_EN_GPIO_Port, FDCAN1_EN_Pin, GPIO_PIN_SET);		//CAN1_EN
	HAL_GPIO_WritePin(FDCAN2_EN_GPIO_Port, FDCAN2_EN_Pin, GPIO_PIN_SET);		//CAN2_EN
}

void enterPownMode(void)									//监听模式
{
	HAL_GPIO_WritePin(FDCAN1_STB_GPIO_Port, FDCAN1_STB_Pin, GPIO_PIN_SET);		//CAN1_STB
//	HAL_GPIO_WritePin(FDCAN2_STB_GPIO_Port, FDCAN2_STB_Pin, GPIO_PIN_SET);		//CAN2_STB

	HAL_GPIO_WritePin(FDCAN1_EN_GPIO_Port, FDCAN1_EN_Pin, GPIO_PIN_RESET);		//CAN1_EN
//	HAL_GPIO_WritePin(FDCAN2_EN_GPIO_Port, FDCAN2_EN_Pin, GPIO_PIN_RESET);		//CAN2_EN
}

void enterNormalMode(void)									//正常模式
{
	HAL_GPIO_WritePin(FDCAN1_STB_GPIO_Port, FDCAN1_STB_Pin, GPIO_PIN_SET);		//CAN1_STB
	HAL_GPIO_WritePin(FDCAN2_STB_GPIO_Port, FDCAN2_STB_Pin, GPIO_PIN_SET);		//CAN2_STB

	HAL_GPIO_WritePin(FDCAN1_EN_GPIO_Port, FDCAN1_EN_Pin, GPIO_PIN_SET);		//CAN1_EN
	HAL_GPIO_WritePin(FDCAN2_EN_GPIO_Port, FDCAN2_EN_Pin, GPIO_PIN_SET);		//CAN2_EN
}

void FDCAN_start(void)											//FDCAN启动函数
{
	enterNormalMode();											//CAN收发器正常模�?????
	HAL_Delay(5);

	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0) != HAL_OK)		//使能FIFO0新消息中�?????
	{
	  	Error_Handler();
	}
	if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0) != HAL_OK)		//使能FIFO0新消息中�?????
	{
		Error_Handler();
	}
	if (HAL_FDCAN_Start(&hfdcan1)!= HAL_OK)														//使能FDCAN
	{
		Error_Handler();
	}
	if (HAL_FDCAN_Start(&hfdcan2)!= HAL_OK)														//使能FDCAN
	{
		Error_Handler();
	}
}

void FDCAN_SendData(FDCAN_HandleTypeDef hfdcan,FDCAN_TxHeaderTypeDef TxHeader,uint8_t *pdata)
{
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, pdata);				//发送数据到TX FIFO
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)			//接收中断回调函数重写
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
	  /* Retrieve Rx messages from RX FIFO0 */
//		enterNormalMode();
		bufferHandle->FDCAN_RxData(hfdcan);
	}
}

void Timer_Start(TIM_HandleTypeDef htim)
{
	__HAL_TIM_CLEAR_FLAG(&htim, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&htim);
}

void Timer_Stop(TIM_HandleTypeDef htim)
{
	HAL_TIM_Base_Stop_IT(&htim);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1) {
	    HAL_IncTick();
	}
	if(htim->Instance == TIM2)
	{
//		printf("tim2\n");
		LED_R_WritePin(GPIO_PIN_RESET);
	}

	if(htim->Instance == TIM3)
	{
//		printf("tim3\n");
		_gotoSleep();
	}
}

void Timer_Reload(TIM_HandleTypeDef htim)
{
	__HAL_TIM_SET_COUNTER(&htim,0);
}

void Timer_Reload_Start(TIM_HandleTypeDef htim)
{
	__HAL_TIM_SET_COUNTER(&htim,0);
	__HAL_TIM_CLEAR_FLAG(&htim, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&htim);
}

