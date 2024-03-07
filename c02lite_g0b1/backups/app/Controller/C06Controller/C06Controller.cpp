/*
* RoadPassion CONFIDENTIAL
*
* Copyright 2018 RoadPassion Electronics Technology Co., Ltd.
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

#include "C06Controller.h"


C06Controller::C06Controller(Car *pCar)
{
    _pCar=pCar;
    _pCar->init();

    gpDrvCAN->setBaudrate(CAN1, _pCar->baudRate, CAN_Mode_Normal);
	if(gpDrvSwitch1Pin->getValue() && gpDrvSwitch2Pin->getValue() &&\
		gpDrvSwitch3Pin->getValue() && gpDrvSwitch4Pin->getValue()){
		//生产测试用,接收所有ID
	}else{
    	gpDrvCAN->addFilters(pCar->pCanFilterIDList);
	}

    delay_1ms(200);
    GPIOInit();
    delay_1ms(200);

    pHub = CanHub::getInstance();
    
    _pForwarder = new GearForwarder(_pCar);
    _pForwarder->init();
    _pForwarder->enable();

    gpDrvTJA1041->init();
    
#ifdef C06_V1_2
    // Auto Detect
    isTJA1043 = gpDrvTJAdetect->getValue();

    printf("CAN transceiver type: %s\n",isTJA1043?"TJA1043":"TJA1044");

    if(isTJA1043){
        gpDrvTJA1041->enterPownMode();
    }else{
        gpDrvTJA1041->TJA1044enterNormalMode();
    }
#else
	isTJA1043 = true;
    gpDrvTJA1041->enterPownMode();
#endif

    gpDrvCAN->start();
}

C06Controller::~C06Controller(void) {

}

void C06Controller::run(void)
{
	/* 生产测试用 */
	if(gpDrvSwitch1Pin->getValue() && gpDrvSwitch2Pin->getValue() &&\
		gpDrvSwitch3Pin->getValue() && gpDrvSwitch4Pin->getValue()){
		printf("Enter test mode!\n");
		TestMode();
	}
	/*
	 * V1.2 Uses StandBy Mode, so we can NOT use IWDG otherwise
	 * MCU will be wake up
	 */
//	if (isTJA1043) { //打开看门狗会导致V1.1硬件不休眠
//		DrvWatchDog::getInstance()->initTheDog(2500);		/* 2.5S */
//		DrvWatchDog::getInstance()->enableTheDog(true);
//	}
	zprintf("Controller: Start\n");

    while(1){
//    	if (isTJA1043) {
//    		DrvWatchDog::getInstance()->feedTheDog();
//    	}

        pHub->doSingleTask();			/* Handle One By One */
    }

}

void C06Controller::GPIOInit(void)
{
    gpDrvSpeedLimit->init();
    gpDrvSpeedLimit->setActiveLevel(true);
    gpDrvSpeedLimit->off();
    
    gpDrvReverseGear->init();
    gpDrvReverseGear->setActiveLevel(true);
    gpDrvReverseGear->off();

    gpDrvParkingBrake->init();
    gpDrvParkingBrake->setActiveLevel(true);
    gpDrvParkingBrake->off();

    gpDrvRightIndicator->init();
    gpDrvRightIndicator->setActiveLevel(true);
    gpDrvRightIndicator->off();

    gpDrvLeftIndicator->init();
    gpDrvLeftIndicator->setActiveLevel(true);
    gpDrvLeftIndicator->off();
}

 void C06Controller::TestMode(void)
 {
	while(1){
		if(gpDrvDel1->getValue()){
			ENABLE_AV1_OUTPUT(1);
			ENABLE_AV2_OUTPUT(1);
			ENABLE_AV3_OUTPUT(1);
			ENABLE_AV4_OUTPUT(1);
			ENABLE_AV5_OUTPUT(1);
		}else if(gpDrvDel2->getValue()){
			ENABLE_AV1_OUTPUT(0);
			ENABLE_AV2_OUTPUT(0);
			ENABLE_AV3_OUTPUT(0);
			ENABLE_AV4_OUTPUT(0);
			ENABLE_AV5_OUTPUT(0);
		}
	}
 }

