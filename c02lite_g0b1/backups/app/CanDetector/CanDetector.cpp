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
#include "CanDetector.h"

#define SLEEP_TIME 4000 //ms

CanDetector *CanDetector::getInstance(void)
{
	static CanDetector *_this;
	if(!_this) {
		_this = new CanDetector;
	}
	return (_this);
}

CanDetector::CanDetector(void) {
	_pRedLEDTimer = gpDrvTimer2;
	_pSleepTimer = gpDrvTimer3;

	_needSleep = true;
}

CanDetector::~CanDetector(void) {

}

void CanDetector::_gotoSleep(void)
{
	gpDrvLEDRed->off();

	zprintf(" goto sleep.\n");
	_chipsSleep();

	/* Sleep Timer Reload and Restart */
	_pSleepTimer->reload();
	_pSleepTimer->start();

	_pRedLEDTimer->reload();
	_pRedLEDTimer->start();
}

void CanDetector::_chipsSleep(void)
{
	static u32 delayCnt = 0;
	
	gpDrvLEDGreen->getValue()? gpDrvLEDGreen->off(): gpDrvLEDGreen->on();
   
#ifdef C06_V1_2
    if(gpDrvTJAdetect->getValue()){                         //TJA1043
		gpDrvTJA1041->enterNormalMode();
		delay_ms(100);
		gpDrvTJA1041->enterGotoSleepCommandMode();
		gpDrvTJA1041->enterPownMode();
    }else{                                                  //TJA1044
        zprintf(" delay sleep.\n");
#ifdef NEED_DELAY_SLEEP
		delayCnt++;
		if((SLEEP_TIME*delayCnt) > 900000){ 
        gpDrvStandby->Sys_Standby();
		}
#else
		gpDrvStandby->Sys_Standby();
#endif		
    }
#else 
	gpDrvTJA1041->enterNormalMode();
	delay_ms(100);
	gpDrvTJA1041->enterGotoSleepCommandMode();
	gpDrvTJA1041->enterPownMode();
#endif
}

void CanDetector::start(bool needSleep)
{
    printf("CanDetector start\n");

    _needSleep = needSleep;

	/* Configure LED Timer as periodic */
	_pRedLEDTimer->setIrqCB(_CanRxIndicatorHandler,this);
	_pRedLEDTimer->setTimer(200, true);
    
	/* Sleep Monitor timer as OneShot */
	_pSleepTimer->setIrqCB(_CanRxMonitorTimerHandler, this);
	_pSleepTimer->setTimer(SLEEP_TIME, true);
    
	/* Set CAN RX IRQ Callback to refresh timer */
	gpDrvCAN->setCanRxMonitorCB(_CanRxIRQMonitorCB, this);
    
	_pRedLEDTimer->start();
	if (_needSleep) {
		_pSleepTimer->start();
	}
}

void CanDetector::stop(void)
{
	_pRedLEDTimer->stop();
	_pSleepTimer->stop();
}

void CanDetector::_CanRxIndicatorHandler(void *param)
{
	/* If no CAN messages in, turn LED ON */
	gpDrvLEDRed->on();
}

void CanDetector::_CanRxIRQMonitorCB(void *param)
{
	CanDetector *_this = (CanDetector *)param;

	/* If received CAN message, start flashing LED */
	gpDrvLEDRed->getValue()? gpDrvLEDRed->off(): gpDrvLEDRed->on();

	/* Reload the LED Indicator timer */
	_this->_pRedLEDTimer->reload();
	_this->_pRedLEDTimer->start();

	if (_this->_needSleep) {
		/* Reload the sleep timer */
		_this->_pSleepTimer->reload();
		_this->_pSleepTimer->start();
	}
}

/*
 * Sleep Timer IRQ Handler
 *   When this IRQ happen, try to enter sleep because no CAN signal
 *
 */
void CanDetector::_CanRxMonitorTimerHandler(void *param)
{
	CanDetector *_this = (CanDetector *)param;

	_this->_gotoSleep();
}
