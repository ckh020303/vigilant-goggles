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
#include "stm32g0xx_hal_flash.h"
#include "iwdg.h"

#define FLASH_FLAG_ALL_ERRORS           (FLASH_FLAG_OPERR   | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | \
                                         FLASH_FLAG_PGAERR  | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR | \
                                         FLASH_FLAG_MISERR  | FLASH_FLAG_FASTERR | \
                                         FLASH_FLAG_OPTVERR | FLASH_FLAG_ECCC    | FLASH_FLAG_ECCD)

#define PAGE_SIZE   FLASH_PAGE_SIZE          /* 2KB Page */
#define PAGE_TOUCH  (0x08000000 + FLASH_MAX_SIZE - PAGE_SIZE * 2 + 1)
#define PAGE_NB		((PAGE_TOUCH - 0x08000000) / PAGE_SIZE)
#define PAGE_INIT_FLAG      0x55AA55AA

Controller::Controller(Car *pCar,uint8_t mode)
{

	_pCar = pCar;
	_pCar->init();

	pHub = CanHub::getInstance();

	bufferHandle->init();

	if (strcmp(pCar->name,"BenzS")==0){
		_pForwarder = new ExForwarder(pCar);
		printf("ExForwarder run\r\n");
	}
	else{
		_pForwarder = new Forwarder(pCar);
		printf("Forwarder run\r\n");
	}

	_pForwarder->init();

//	bufferHandle->setBaudrate(hfdcan1, _pCar->baudRate);
//	bufferHandle->setBaudrate(hfdcan2, _pCar->baudRate);

	if(pCar->isCanFD){
		printf("FDCAN\n");
		bufferHandle->setBaudratefd(hfdcan1, _pCar->baudRate,pCar->baudRateData);
		bufferHandle->setBaudratefd(hfdcan2, _pCar->baudRate,pCar->baudRateData);
		printf("NominalBitRate:%d\n",_pCar->baudRate);
		printf("DataBitRate:%d\n",_pCar->baudRateData);
	}
	else
	{
		printf("CAN\n");
		bufferHandle->setBaudrate(hfdcan1, _pCar->baudRate);
		bufferHandle->setBaudrate(hfdcan2, _pCar->baudRate);
		printf("NominalBitRate:%d\n",_pCar->baudRate);
	}

	_mode = mode;

	if(_mode == MODE_NORMAL) {
		_status = STATUS_OFF;
	}else{
//		_status = STATUS_ON;
		_status = _readStatusFromFlash();
	}

	if(_status == STATUS_ON) {
		LED_G_WritePin(GPIO_PIN_RESET);
		_pForwarder->enable();
		printf("enable\n");
	} else{
		LED_G_WritePin(GPIO_PIN_SET);
		_pForwarder->disable();
		printf("disable\n");
	}

	Relay_WritePin(GPIO_PIN_SET);
}

Controller::~Controller(void) {

}

void Controller::run(void)
{
	HAL_IWDG_Init(&hiwdg);

	printf("Controller: Start\r\n");
	/* Enable FDCAN1 and FDCAN2 */

	FDCAN_start();

//	FDCAN_TxHeaderTypeDef TxHeader;
//	uint8_t data[8] = {'1','2','3','4','5','6','7','8'};
//	TxHeader.Identifier = 0x3DA;
//	TxHeader.IdType = FDCAN_STANDARD_ID;
//	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
//	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
//	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
//	TxHeader.BitRateSwitch = FDCAN_BRS_ON;
//	TxHeader.FDFormat = FDCAN_FD_CAN;
//	TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
//	TxHeader.MessageMarker       = 0;
//
//	FDCAN_ProtocolStatusTypeDef Status_FDCAN1;
//	printf("abcd\r\n");
	while(1) {
		HAL_IWDG_Refresh(&hiwdg);
//		printf("RX:%d\n",i);

		pHub->doSingleTask();			/* Handle One By One */

//		if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, data) != HAL_OK)
//		{
//			Error_Handler();
//		}
//		printf("1\r\n");
//		HAL_Delay(1000);
//
//		HAL_FDCAN_GetProtocolStatus(&hfdcan1,&Status_FDCAN1);
//
//		printf("TDC:%x\n",Status_FDCAN1.TDCvalue);

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

//    printf("time=%d\n",HAL_GetTick());

	if (key_curStatus != key_lastStatus) {
		key_lastStatus = key_curStatus;
		if (key_curStatus) {
			/* Key pressed detected */
			key_starttime = HAL_GetTick();
			key_first_detection = true;
		}
	} else {
		if (key_curStatus) {
			/* Key Pressed KEEP */
			key_endtime = HAL_GetTick();

			if (key_endtime - key_starttime > 2000) {
				key_longPressed = true;
			}
		}
	}

	/* Change the status and save to flash */
	if (key_longPressed && key_first_detection) {
		printf("Steer Button Long Pressed\n");

		if(_status == STATUS_ON) {
			_status = STATUS_OFF;
			_pForwarder->disable();
			LED_G_WritePin(GPIO_PIN_SET);
		} else {
			_status = STATUS_ON;
			_pForwarder->enable();
			LED_G_WritePin(GPIO_PIN_RESET);
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
	uint32_t _status;
	FLASH_EraseInitTypeDef pEraseInit;

	uint32_t PageError = 0;
	pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	pEraseInit.Banks = FLASH_BANK_1;
	pEraseInit.NbPages = 1;

	int A[5];

        A[0] = PAGE_INIT_FLAG;
	if(status == STATUS_ON) {
		A[1] = 0;
	} else {
		A[1] = 0x02;
	}

	HAL_FLASH_Unlock();

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

	pEraseInit.Page = PAGE_NB;
	_status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
	pEraseInit.Page = PAGE_NB + 1;
	_status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);

	if (_status != HAL_OK) {
		goto out;
	}
	/* Program 3 words */
	for (uint8_t i = 0; i < 2; i++) {
		_status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, PAGE_TOUCH + i*8, A[i]);
		if (_status != HAL_OK) {
			goto out ;
		}
	}
//	printf("write\n");
//	printf("%x\n",(*(__IO uint32_t*)(PAGE_TOUCH)));
//	printf("%x\n",(*(__IO uint32_t*)(PAGE_TOUCH + 8)));
	out:
	HAL_FLASH_Lock();
}

bool Controller::_readStatusFromFlash(void)
{
	bool ret=STATUS_OFF;

//	printf("%x\n",(*(__IO uint32_t*)(PAGE_TOUCH)));
//	printf("%x\n",(*(__IO uint32_t*)(PAGE_TOUCH + 8)));
//	printf("%x\n",PAGE_INIT_FLAG);

	/* Read */
	if ( (*(__IO uint32_t*)(PAGE_TOUCH)) != PAGE_INIT_FLAG) {
		_writeStatus2Flash(true);
	}

	if( (*(__IO uint32_t*)(PAGE_TOUCH + 8)) == 0x02) {
		ret = STATUS_OFF;
	} else {
		ret = STATUS_ON;
	}
	return (ret);
}
