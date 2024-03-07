
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
出货OBD测试软件

*/

static char *banner =
		"\n********************************************************\n"
		"Guangzhou RoadPassion Electronics Technology Co., Ltd.\n"
		"Model: C02Lite.\n"
		"Version: %s\n"
		"Date: %s\n"
		"********************************************************\n\n";

int extend_SRAM(void)
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
    /* Change SRAM size to 64KB */
    status=FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0,0xFC);
    NVIC_SystemReset();
  }
  printf("SRAM is 64K.\r\n");  
  status=1;
  return status;
}



int  appMain(void *p_arg)
{
    (void)p_arg;
    printf(banner,version,date);

    gpDrvLEDGreen->setActiveLevel(true);
    gpDrvLEDRed->setActiveLevel(true);

#ifdef AT32F413Rx_MD 
    int status=extend_SRAM();
#endif

//    FLASH_Unlock();
//    FLASH_ReadProtectConfig(ENABLE);
//    FLASH_Lock();

    gpDrvCAN->setBaudrate(CAN1, 500);
    gpDrvCAN->start();
    
    while(1)
    {
#ifdef AT32F413Rx_MD 
        if(status !=1){
            gpDrvLEDRed->off();
            continue;
        }
#endif
        
        if(gpDrvCAN->_Can1RMsg.msg.StdId==0x111)
        {
            if(gpDrvCAN->_Can1RMsg.msg.Data[0]==0x22)
                gpDrvLEDRed->toggle();
        }
        delay_1ms(100);
    }
    
    return 0;
}

