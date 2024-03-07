/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FDCAN2_EN_Pin GPIO_PIN_15
#define FDCAN2_EN_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_0
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_1
#define LED_G_GPIO_Port GPIOA
#define SW4_Pin GPIO_PIN_12
#define SW4_GPIO_Port GPIOB
#define SW1_Pin GPIO_PIN_13
#define SW1_GPIO_Port GPIOB
#define SW2_Pin GPIO_PIN_14
#define SW2_GPIO_Port GPIOB
#define SW3_Pin GPIO_PIN_15
#define SW3_GPIO_Port GPIOB
#define RELAY_CTRL_Pin GPIO_PIN_9
#define RELAY_CTRL_GPIO_Port GPIOA
#define FDCAN1_ERR_Pin GPIO_PIN_6
#define FDCAN1_ERR_GPIO_Port GPIOC
#define FDCAN1_STB_Pin GPIO_PIN_7
#define FDCAN1_STB_GPIO_Port GPIOC
#define FDCAN1_EN_Pin GPIO_PIN_15
#define FDCAN1_EN_GPIO_Port GPIOA
#define FDCAN2_ERR_Pin GPIO_PIN_8
#define FDCAN2_ERR_GPIO_Port GPIOB
#define FDCAN2_STB_Pin GPIO_PIN_9
#define FDCAN2_STB_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
