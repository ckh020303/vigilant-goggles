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

#include "Controller.h"

#define PAGE_SIZE   0x0400          /* 1KB Page */
#define PAGE_TOUCH  (0x08000000 + FLASH_MAX_SIZE - PAGE_SIZE * 2 + 1)

#define PAGE_INIT_FLAG      0x55AA55AA

Controller::Controller(Car *pCar,uint8_t mode)
{

	_pCar = pCar;
	_pCar->init();

	pHub = CanHub::getInstance();

	if(strcmp(pCar->name,"BenzS")==0){
		_pForwarder = new ExForwarder(pCar);
		printf("ExForwarder run\r\n");
	}else{
		_pForwarder = new Forwarder(pCar);
		printf("Forwarder run\r\n");
	}
	_pForwarder->init();

	zprintf("DrvCAN: Set Bardrate to %dK\r\n",_pCar->baudRate);

	gpDrvCAN->setBaudrate(CAN1, _pCar->baudRate, CAN_Mode_Normal);
	gpDrvCAN->setBaudrate(CAN2, _pCar->baudRate, CAN_Mode_Normal);
//	gpDrvCAN->addFilters(pCar->pCanFilterIDList);

	_mode = mode;

#ifndef PV_C02_VW_CADDY
	if(_mode == MODE_NORMAL) {
		_status = STATUS_OFF;
	}else{
		_status = _readStatusFromFlash();
	}
#else
	_status = STATUS_ON;
#endif

	if(_status == STATUS_ON) {
		gpDrvLEDGreen->on();
		_pForwarder->enable();
	} else{
		gpDrvLEDGreen->off();
		_pForwarder->disable();
	}
}
Controller::~Controller(void) {

}

void Controller::run(void)
{
	gpDrvLEDRed->off();

	DrvWatchDog::getInstance()->initTheDog(2500);		/* 2.5S */
	DrvWatchDog::getInstance()->enableTheDog(true);

	zprintf("Controller: Start\r\n");
	/* Enable CAN1 and CAN2 */
	gpDrvTJA1041->enterNormalMode();
	gpDrvTJA1041_2->enterNormalMode();
	delay_1ms(1000);
	gpDrvCAN->start();

	while(1) {
		DrvWatchDog::getInstance()->feedTheDog();

		pHub->doSingleTask();			/* Handle One By One */

#ifndef PV_C02_VW_CADDY
		handleUserButtons();			/* Steering Button to Disable/Enable TV Free */
#endif
	}
}

/*
 * Long press "Return" key in steering buttons, to disable/enable TV Free,
 * and save the status to internal flash.
 * At next boot, it will read the status from flash and set to that status.
 */
void Controller::handleUserButtons(void)
{
	static uint32_t key_starttime = 0;
	uint32_t key_endtime;

	static bool key_lastStatus = false;
	bool key_curStatus = _pCar->TVFKeyPressed;

	bool key_longPressed = false;
	static bool key_first_detection = false;
    
//    printf("time=%d\n",systick_gettime());
    
	if (key_curStatus != key_lastStatus) {
		key_lastStatus = key_curStatus;
		if (key_curStatus) {
			/* Key pressed detected */
			key_starttime = systick_gettime();
			key_first_detection = true;
		}
	} else {
		if (key_curStatus) {
			/* Key Pressed KEEP */
			key_endtime = systick_gettime();

			if (key_endtime - key_starttime > 2000) {
				key_longPressed = true;
			}
		}
	}

	/* Change the status and save to flash */
	if (key_longPressed && key_first_detection) {
		zprintf("Steer Button Long Pressed\n");

		if(_status == STATUS_ON) {
			_status = STATUS_OFF;
			_pForwarder->disable();
			gpDrvLEDGreen->off();
		} else {
			_status = STATUS_ON;
			_pForwarder->enable();
			gpDrvLEDGreen->on();
		}
        
         /* Save status when in SAFE mode */
        if(_mode != MODE_NORMAL) {
            printf("new _status=%d\n",_status);
            _writeStatus2Flash(_status);
        }

        key_longPressed = false;
        key_first_detection = false;
	}
}
/*************** Flash Read&Write Functions *****************************/

void Controller::_writeStatus2Flash(bool status)
{
	u32 _status;

	int A[5];

        A[0] = PAGE_INIT_FLAG;
	if(status == STATUS_ON) {
		A[1] = 0;
	} else {
		A[1] = 0x02;
	}

	FLASH_Unlock();
	_status = FLASH_ErasePage(PAGE_TOUCH);
	_status = FLASH_ErasePage(PAGE_TOUCH + 0x04);
	if (_status != FLASH_PRC_DONE) {
		goto out;
	}
	/* Program 3 words */
	for (uint8_t i = 0; i < 2; i++) {
		_status = FLASH_ProgramWord(PAGE_TOUCH + i*4, A[i]);
		if (_status != FLASH_PRC_DONE) {
			goto out ;
		}
	}
	out:
	FLASH_Lock();
}

bool Controller::_readStatusFromFlash(void)
{
	bool ret=STATUS_OFF;
        
        /* Read */
        if ( (*(vs32*)(PAGE_TOUCH)) != PAGE_INIT_FLAG) {
            _writeStatus2Flash(true);
        }
        
	if( (*(vs32*)(PAGE_TOUCH+0x04))==0x02 ) {
		ret = STATUS_OFF;
	} else {
		ret = STATUS_ON;
	}
	return (ret);
}
