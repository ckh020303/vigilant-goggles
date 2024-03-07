/*
 * RoadPassion CONFIDENTIAL
 *
 * Copyright 2018 RoadPassion Electronics Co., Ltd.
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

//#include <Controller/Controller.h>
#include <CanDetector/CanDetector.h>
#include <Car/BenZ/BenZ.h>
#include <Car/AudiA3/AudiA3.h>
#include <Car/AudiQ7_0FD/AudiQ7_0FD.h>
#include <Car/Golf7/Golf7.h>
#include <Car/BenzS/BenzS.h>
#include <Car/BenzC/BenzC.h>
#include <Car/AudiA7_2019/AudiA7_2019.h>
#include "C02_TESTER_PV.h"
#include <includes.h>
#include <CanHub/CanHub.h>
#include <utils/RingBuffer.h>
#include <DrvCAN.h>


C02_TESTER_PV::C02_TESTER_PV() {
	// TODO Auto-generated constructor stub
//	_pCar = NULL;
//	_pGreenLEDMonostable = NULL;
//	_pCanResetMonostable = NULL;
//	_pTester = NULL;
}

C02_TESTER_PV::~C02_TESTER_PV() {
	// TODO Auto-generated destructor stub
}

void C02_TESTER_PV::init(void)
{

	uint8_t value = 0;
	if(gpDrvSwitch1Pin->getValue()) value |= 0x01;
	if(gpDrvSwitch2Pin->getValue()) value |= 0x02; //for add to choice Benz-S Car
	if(gpDrvSwitch3Pin->getValue()) value |= 0x04; //for add to choice Benz-S Car
	switch (value) {
	case 0:
		_pCar = new AudiA3;
		break;
	case 1:
		_pCar = new BenZ();
		break;
	case 2:							//add Benz-S
		_pCar = new Golf7();
		break;
	case 3:
		_pCar = new BenzS();
		break;
	case 4:
		_pCar = new BenzC();
		break;
	case 5:
		_pCar = new AudiQ7_0FD();
		break;
	case 6:
		_pCar = new AudiA7_2019();
		break;
	default:
		_pCar = new BenzC();
		break;
	}
	
	delay_1ms(2500);
    
    CanHub *pHub = CanHub::getInstance();
    
    gpDrvTJA1041->enterNormalMode();
    gpDrvTJA1041_2->enterNormalMode();

	gpDrvCAN->setBaudrate(CAN1,_pCar->baudRate);
	gpDrvCAN->setBaudrate(CAN2,_pCar->baudRate);

	gpDrvLEDGreen->setActiveLevel(0);
	gpDrvLEDRed->setActiveLevel(0);

	gpDrvLEDGreen->off();
	gpDrvLEDRed->on();

	gpDrvCAN->start();

//	gpDrvRelay->on();
}

void C02_TESTER_PV::run(void)
{
	init();

	u8 speed = 20;

    CanTxMsg _msg =
    {
      .StdId = 0x00,
      .ExtId = 0,
      .IDT = 0,
      .RTR = 0,
      .DLC = 8,
      .Data = {0x00, 0x00, 0x00, 0x00,0x00,0x00,0x00,0x00},
    };

    CanTxMsg _TVmsg =
    {
      .StdId = 0x00,
      .ExtId = 0,
      .IDT = 0,
      .RTR = 0,
      .DLC = 8,
      .Data = {0x00, 0x00, 0x00, 0x00,0x00,0x00,0x00,0x00},
    };

    if((strcmp(_pCar->name,"Golf7")==0)||(strcmp(_pCar->name,"AudiA3")==0))
    {
        _msg.StdId = _pCar->pCanFilterIDList[0];
        _msg.Data[0] = 0x20;
        _msg.Data[_pCar->pCanSpeedLowByte] = speed&0xff;
        _msg.Data[_pCar->pCanSpeedHighByte] = speed>>8;

        _TVmsg.StdId = 0x5bf;
        _TVmsg.Data[0] = 0x08;
        _TVmsg.Data[2] = 0x04;
    }
    else if(strcmp(_pCar->name,"BenzC")==0)
    {
        _msg.StdId = _pCar->pCanFilterIDList[0];
        _msg.Data[_pCar->pCanSpeedLowByte] = 0xff;
        _msg.Data[_pCar->pCanSpeedHighByte] = 0xff;

        _TVmsg.StdId = 0x401;
        _TVmsg.Data[4] = 0x04;
    }
    else if(strcmp(_pCar->name,"BenZ")==0||strcmp(_pCar->name,"BenzS")==0) 
    {
        _msg.StdId= _pCar->pCanFilterIDList[0];
        _msg.Data[_pCar->pCanSpeedLowByte] = 0xff;
        _msg.Data[_pCar->pCanSpeedHighByte] = 0xff;

        _TVmsg.StdId = 0x45;
        _TVmsg.Data[5] = 0x02;
    }
    else if(strcmp(_pCar->name,"AduiQ7_0FD")==0)
    {
        _msg.StdId = _pCar->pCanFilterIDList[0];
        _msg.Data[_pCar->pCanSpeedLowByte] = speed>>8;
        _msg.Data[_pCar->pCanSpeedHighByte] = speed&0xff;
    }
    else if(strcmp(_pCar->name,"AudiA7_2019")==0)
    {
        _msg.StdId = _pCar->pCanFilterIDList[0];
        _msg.Data[_pCar->pCanSpeedLowByte] = speed>>8;
        _msg.Data[_pCar->pCanSpeedHighByte] = speed&0xff;

        _TVmsg.StdId = 0x5bf;
        _TVmsg.Data[0] = 0x21;
        _TVmsg.Data[2] = 0x07;
    }

    printf("%s\n",_pCar->name);
    printf("%x\n",_msg.StdId );
    
    u32 SpeedValue_CAN1,SpeedValue_CAN2;
    printf("Tester is init!\n");
    u8 a,b;
    while(1)
    {   
        gpDrvCAN->transmit(CAN1, &_TVmsg);
        a = gpDrvCAN->transmit(CAN1, &_msg);
//        b = gpDrvCAN->transmit(CAN2, &_msg);
//        printf("%d,%d\n",a,b);
        if((a==0||a==1||a==2) || (b==0||b==1||b==2))
            gpDrvLEDRed->toggle();
//        printf("%d,%d\n",a,b);
//        printf("CAN%d:  ID:0x%X\n",gpDrvCAN->_Can1RMsg.CANx==CAN1?1:2,gpDrvCAN->_Can1RMsg.msg.StdId);
//        for(int a = 0;a<8;a++)
//            printf("%X ",gpDrvCAN->_Can1RMsg.msg.Data[a]);
//         printf("\n");
//
//         printf("CAN%d:  ID:0x%X\n",gpDrvCAN->_Can2RMsg.CANx==CAN1?1:2,gpDrvCAN->_Can2RMsg.msg.StdId);
//        for(int a = 0;a<8;a++)
//            printf("%X ",gpDrvCAN->_Can2RMsg.msg.Data[a]);
//         printf("\n");


        if(gpDrvCAN->_Can1RMsg.msg.StdId!=_pCar->pCanFilterIDList[0] && gpDrvCAN->_Can2RMsg.msg.StdId!=_pCar->pCanFilterIDList[0])
        {
            gpDrvLEDGreen->off();
            gpDrvLEDRed->on();
        }

        if(gpDrvCAN->_Can1RMsg.msg.StdId==_pCar->pCanFilterIDList[0] || gpDrvCAN->_Can2RMsg.msg.StdId==_pCar->pCanFilterIDList[0])
        {
            
            delay_1ms(50);
            if(gpDrvCAN->_Can1RMsg.msg.StdId==_pCar->pCanFilterIDList[0] && gpDrvCAN->_Can2RMsg.msg.StdId==_pCar->pCanFilterIDList[0])
            {
                if(strcmp(_pCar->name,"BenZ")==0||strcmp(_pCar->name,"BenzS")==0)
                {
                    if(gpDrvCAN->_Can2RMsg.msg.Data[_pCar->pCanSpeedHighByte]==0x00||gpDrvCAN->_Can2RMsg.msg.Data[_pCar->pCanSpeedLowByte]==0x10)
                    {
                        gpDrvLEDGreen->toggle();
                    }
                }
                else if(strcmp(_pCar->name,"BenzC")==0)
                {
                    if(gpDrvCAN->_Can2RMsg.msg.Data[_pCar->pCanSpeedHighByte]==0x80||gpDrvCAN->_Can2RMsg.msg.Data[_pCar->pCanSpeedLowByte]==0x00)
                    {
                        gpDrvLEDGreen->toggle();
                    }
                }
                else
                {
                    SpeedValue_CAN1 = (u32) gpDrvCAN->_Can1RMsg.msg.Data[_pCar->pCanSpeedHighByte]<<8|gpDrvCAN->_Can1RMsg.msg.Data[_pCar->pCanSpeedLowByte];
                    SpeedValue_CAN2 = (u32) gpDrvCAN->_Can2RMsg.msg.Data[_pCar->pCanSpeedHighByte]<<8|gpDrvCAN->_Can2RMsg.msg.Data[_pCar->pCanSpeedLowByte];
                    
                    if(SpeedValue_CAN1!=0 && SpeedValue_CAN2==0)
                    {
                        gpDrvLEDGreen->toggle();
                    }
                }
            }   
        }

        gpDrvCAN->_Can1RMsg.msg.StdId=0;
        gpDrvCAN->_Can2RMsg.msg.StdId=0;

        delay_1ms(50);
    }
}
