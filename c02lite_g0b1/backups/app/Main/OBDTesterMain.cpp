
#include <Zox.h>
#include "includes.h"
#include <board.h>
#include "common.h"
#include "version.h"

#include <secure/Activatee.h>
#include <PV/PV.h>
//#include <Controller/Controller.h>
#include <CanDetector/CanDetector.h>
#include <CanHub/CanHub.h>

/*
OBD测试板软件

*/


static char *banner =
		"\n********************************************************\n"
		"Guangzhou RoadPassion Electronics Technology Co., Ltd.\n"
		"Model: C02Lite.\n"
		"Version: %s\n"
		"Date: %s\n"
		"********************************************************\n\n";

void extend_SRAM(void)
{
    int status=0;
/* Target set_SRAM_64K is selected */
  if(((UOPTB->EOPB0)&0xFF)!=0xFC)
  {
    printf("set SRAM to 64K.\r\n");
    /* Unlock Option Bytes Program Erase controller */
    FLASH_Unlock();
    /* Erase Option Bytes */
    status=FLASH_EraseUserOptionBytes();
    printf("status=%d\r\n",status);
    /* Change SRAM size to 64KB */
    status=FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0,0xFC);
    printf("status=%d\r\n",status);
    NVIC_SystemReset();
  }
  printf("SRAM is 64K.\r\n");
}



int  appMain(void *p_arg)
{
    (void)p_arg;
    printf(banner,version,date);
    //#ifdef NEED_ACTIVATION
    //  // 处理激活。flash地址选为最后一个页面，需确保不与主程序空间冲突。
    //#ifdef FLASH_MAX_SIZE
    //  Activatee activatee(gpDrvUSART, 0x08000000+FLASH_MAX_SIZE - 0x0f);
    //#else
    //  Activatee activatee(gpDrvUSART, 0x0800fff0);
    //#endif
    //    if (!activatee.isActivated()) {
    //      zprintf("This product is not activated.\n");
    //      blinkGreenLed();
    //      activatee.waiteActivation();
    //  }
    //#endif
    
//    extend_SRAM();

    FLASH_Unlock();
    FLASH_ReadProtectConfig(ENABLE);
    FLASH_Lock();

    gpDrvLEDGreen->setActiveLevel(true);
    gpDrvLEDRed->setActiveLevel(true);
    gpDrvLEDGreen->on();

    gpDrvCAN->setBaudrate(CAN1, 500);
    gpDrvCAN->start();
    
    CanTxMsg msg =
	{
			.StdId = 0x111,
			.ExtId = 0,
			.IDT = CAN_ID_STD,
			.RTR = 0,
			.DLC = 4,
			.Data = {0x22, 0x95, 0x00, 0x00}
	};
	
    int status=0;
    while(1)
    {
        
        status=gpDrvCAN->transmit(CAN1, &msg, false);
        printf("status=%d",status);
        if(status == 0 || status == 1 || status == 2)
        {
            gpDrvLEDRed->toggle();
        }
        delay_1ms(100);
    }
    
    return 0;
}

