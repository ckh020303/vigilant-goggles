/*
 * RoadPassion CONFIDENTIAL
 *
 * Copyright 2013 RoadPassion Electronics Co., Ltd.
 *
 * DO NOT COPY AND/OR REDISTRIBUTE WITHOUT PERMISSION.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <PV.h>

#include "includes.h"
#include "version.h"
#include "../Bsp/Bsp.h"
//#include <Zox.h>
//#include "includes.h"
//#include <board.h>
//#include "common.h"
//#include "version.h"
//
//#include <secure/Activatee.h>
//#include <PV/PV.h>
////#include <Controller/Controller.h>
//#include <CanDetector/CanDetector.h>
//#include <CanHub/CanHub.h>


/* Kernel includes. */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "timers.h"
//#include "semphr.h"
//#include "queue.h"
//#include "limits.h"


//#ifndef ZDEBUG
//#define NEED_ACTIVATION 1
//
//static void blinkGreenLed(void)
//{
//	for (int i = 0; i < 10; i++) {
//		gpDrvLEDGreen->off();
//        Delay_ms(80);
//		gpDrvLEDGreen->on();
//		Delay_ms(80);
//	}
//}
//#endif
//
//static char *banner =
//		"\n********************************************************\n"
//		"Guangzhou RoadPassion Electronics Technology Co., Ltd.\n"
//		"Model: C02Lite Artery.\n"
//		"Version: %s\n"
//		"Date: %s\n"
//		"********************************************************\n\n";
//int rec_len = 0;
//char rec_Buff[10];
//
//
//int time2_test;
//void Time2IrqCB(void *data)
//{
//  static bool flag = true;
//
//  if(flag)
//  {
//    gpDrvLEDGreen->on();
//    gpDrvLEDRed->on();
//  }
//  else
//  {
//    gpDrvLEDGreen->off();
//    gpDrvLEDRed->off();
//  }
//}

//void extend_SRAM(void)
//{
//	int status=0;
//	/* Target set_SRAM_64K is selected */
//	if(((UOPTB->EOPB0)&0xFF)!=0xFC)
//	{
//		printf("set SRAM to 64K.\r\n");
//		/* Unlock Option Bytes Program Erase controller */
//		FLASH_Unlock();
//		/* Erase Option Bytes */
//		status=FLASH_EraseUserOptionBytes();
//		printf("status=%d\r\n",status);
//		/* Change SRAM size to 64KB */
//		status=FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0,0xFC);
//		printf("status=%d\r\n",status);
//		//NVIC_SystemReset();
//	}
//	printf("SRAM is 64K.\r\n");
//}

void appMain(void)
{
	LED_R_WritePin(GPIO_PIN_RESET);
	printf(banner,version,date);
	printf("SystemCoreClock=%d\r\n", (int)SystemCoreClock);

//#ifdef AT32F413Rx_MD
////	extend_SRAM();
//#endif

//#ifndef FLASH_READ_PROTECT
//	FLASH_Unlock();
//	FLASH_ReadProtectConfig(ENABLE);
//	FLASH_Lock();
//#endif
	
	getPV_run();
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
    HAL_UART_Transmit(&huart3 , (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#ifdef __cplusplus
}
#endif


