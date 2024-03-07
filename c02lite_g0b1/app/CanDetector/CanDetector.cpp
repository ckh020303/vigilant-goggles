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
#include "iwdg.h"

CanDetector *CanDetector::getInstance(void)
{
	static CanDetector *_this;
	if(!_this) {
		_this = new CanDetector;
	}
	return (_this);
}

CanDetector::CanDetector(void) {
	_needSleep = true;
}

CanDetector::~CanDetector(void) {

}

void delay_ms(uint32_t nms){
	uint32_t i=0;
	while(nms--){
		i=10100;
		while(i--);
	}
}

void _gotoSleep(void)
{
	LED_R_WritePin(GPIO_PIN_SET);
	printf(" go to sleep.\n");

	enterGotoSleepCommandMode();
	delay_ms(10);

	while(1)
	{
		if(!getValue(FDCAN1_ERR_GPIO_Port,FDCAN1_ERR_Pin)){
			printf("Wake up!\n");
			enterNormalMode();
			break;
		}
		HAL_IWDG_Refresh(&hiwdg);
		delay_ms(10);
	}

	/* Sleep Timer Reload and Restart */
	Timer_Reload(htim2);
	Timer_Reload(htim3);
	Timer_Start(htim2);
	Timer_Start(htim3);
}

void CanDetector::start(void)
{
    printf("CanDetector start\n");

    bufferHandle->setFDCanRxMonitorCB(_FDCanRxIRQMonitorCB, this);

    Timer_Reload_Start(htim2);
	if (_needSleep) {
		Timer_Reload(htim3);
		Timer_Start(htim3);
	}
}

void CanDetector::stop(void)
{
	Timer_Stop(htim2);
	Timer_Stop(htim3);
}

void CanDetector::_FDCanRxIRQMonitorCB(void *param)
{
	CanDetector *_this = (CanDetector *)param;

	/* If received CAN message, start flashing LED */
	LED_R_TogglePin();

	/* Reload the LED Indicator timer */
	__HAL_TIM_SET_COUNTER(&htim2,0);
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&htim2);

	if(_this->_needSleep)
	{
		/* Reload the sleep timer */
		__HAL_TIM_SET_COUNTER(&htim3,0);
		__HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
		HAL_TIM_Base_Start_IT(&htim3);
	}
}

