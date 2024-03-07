/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../app/SPI_SD_driver/SPI_SD_driver.h"
#include "../../app/Bsp/Bsp.h"
//#include "SPIFLashUpdater.h"
#include "InternalFlashUpdater.h"
#include "UpdateCfgParser.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* 定义类型 */
typedef void (*pFunction)(void);

#define ApplicationFlashAddress 0x0800F000  //定义应用程序地址

#define BUF_MAX_LEN 1024
#define CFGENTRY_MAX_NUM 20
#define CFG_FILE_NAME "updateAT.cfg"
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void ProgramDead(void)
{
	TIM_SetTime(200);
	TIM_Start();
	while(1);
}

static u32 GetUpdateFlie(char *buf,u32 max_len)
{
	//FATFS fs;
	FRESULT res;
	FIL file;
	u32 len=0;
//	res = f_mount(0, &fs);
	res = f_open(&file, CFG_FILE_NAME, FA_OPEN_EXISTING | FA_READ);//CFG_FILE_NAME--updateAT.cfg。FA_OPEN_EXISTING | FA_READ以只读方式打�????????
	if(res == FR_OK) {
		f_read(&file,buf,max_len,&len);
		f_close(&file);
	}
	return len;
}

void jump_to_app(uint32_t APP_FLASH_ADDR)
{
	uint32_t JumpAddress;
	pFunction Jump_To_Application;

	/* 检查栈顶地址是否合法 */
	if(((*(__IO uint32_t *)APP_FLASH_ADDR) & 0x2FFDB000) == 0x20000000)
	{
		printf("jump\n");

		/* 去初始化 */
		HAL_SuspendTick();
		HAL_RCC_DeInit();
		HAL_DeInit();

		/* 屏蔽所有中断，防止在跳转过程中，中断干扰出现异常 */
		__disable_irq();

		/* 用户代码区第二个 字 为程序开始地址(复位地址) */
		JumpAddress = *(__IO uint32_t *) (APP_FLASH_ADDR + 4);

		/* Initialize user application's Stack Pointer */
		/* 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址) */
		__set_MSP(*(__IO uint32_t *) APP_FLASH_ADDR);

		/* 类型转换 */
		Jump_To_Application = (pFunction) JumpAddress;

		/* 跳转到 APP */
		Jump_To_Application();
	}
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __cplusplus
extern "C" {
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart2 , (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#ifdef __cplusplus
}
#endif


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//	StartApplication(ApplicationFlashAddress);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  if (MX_FATFS_Init() != APP_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN 2 */
  LED_G_WritePin(GPIO_PIN_SET);

//  SD_Init();
  printf("hello\n");

  if(SPI_SD_GetSDCard())
  {
	  LED_G_WritePin(GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  TIM_SetTime(50);
	  TIM_Start();

	  char buf[BUF_MAX_LEN];

	  FATFS fs;
	  u32 res;
	  res = f_mount(&fs,"0:",1);									//挂载
	  printf("fmount ok\n");
	  u32 buf_len = GetUpdateFlie(buf,BUF_MAX_LEN);			//获取文件升级描述
	  printf("buf_len:%d\n",buf_len);
	  if (buf_len){
		  	struct UpdateCfgEntry pupdateCfg[CFGENTRY_MAX_NUM];
			int update_num=0;
			update_num = UpdateCfgParser(buf,buf_len,pupdateCfg);//获取待升级文件数�????????
			printf("update_num:%d\n",update_num);
			if(update_num) {
				for(int i=0;i<update_num;i++) {
					int ret;
					if(pupdateCfg[i].type == INTERNAL)
						ret = InternalFlashUpdate(&pupdateCfg[i]);
//					else if(pupdateCfg[i].type == SPI)//SD卡升�????????
//					{
//						ret = SPIFlashUpdate(&pupdateCfg[i]);
//						StartApplication(ApplicationFlashAddress);
//					}
					else ret = -1;
					if(ret) ProgramDead();
				}
			} else {
				ProgramDead();
			}
	  }
	  TIM_Stop();
	  LED_G_WritePin(GPIO_PIN_SET);
  }

  SPI_SD_Release();
  printf("jump_to_app...\n");
  jump_to_app(ApplicationFlashAddress);
  ProgramDead();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
