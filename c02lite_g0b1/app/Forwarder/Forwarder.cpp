/*
 * Forwarder.cpp
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#include "Forwarder.h"

Forwarder::Forwarder(Car *pCar) {
	_status = STATUS_DISABLE;
	_pCar = pCar;
}

Forwarder::~Forwarder(void) {

}

void Forwarder::init(void) {
	CanHub::getInstance()->registerSlot(slot,this);
}

void Forwarder::slot(FDCanRxHeader *pheader,void *param)
{
	Forwarder *_this = (Forwarder*)param;

	/* update Steering Button Keys */
	if(pheader->fdcanx.Instance == hfdcan1.Instance) _this->_pCar->updateTVFKeyStatus(pheader);

	if(_this->_status == STATUS_ENABLE) {
		if(pheader->fdcanx.Instance == hfdcan1.Instance) _this->_pCar->updateSpeedStatus(pheader);
		if(_this->_pCar->speedValue > 0) {
			if(!getValue(RELAY_CTRL_GPIO_Port,RELAY_CTRL_Pin)){
				Relay_WritePin(GPIO_PIN_SET);
				LED_G_WritePin(GPIO_PIN_RESET);
			}

			if(pheader->fdcanx.Instance == hfdcan1.Instance) {
				/* Filter FDCAN Message from FDCAN1 */
				_this->_pCar->filt(pheader);
			}

			_this->_doForward(pheader);
//			_this->_pCar->ack(pheader);
		} else {
			Relay_WritePin(GPIO_PIN_RESET);
			LED_G_WritePin(GPIO_PIN_SET);
			bufferHandle->clearBuffer();		/* Clear FDCAN RX Buffer and drop datas */
		}
	} else {
		Relay_WritePin(GPIO_PIN_RESET);
		LED_G_WritePin(GPIO_PIN_SET);
		bufferHandle->clearBuffer();		/* Clear FDCAN RX Buffer and drop datas */
	}
}

void Forwarder::_doForward(FDCanRxHeader *pheader)
{
	FDCAN_TxHeaderTypeDef TxHeader;
//	uint8_t data[8];
//	uint8_t data[64];
	TxHeader.Identifier = pheader->RxHeader.Identifier;
	TxHeader.IdType = pheader->RxHeader.IdType;
	TxHeader.TxFrameType = pheader->RxHeader.RxFrameType;
	TxHeader.DataLength = pheader->RxHeader.DataLength;
	TxHeader.ErrorStateIndicator = pheader->RxHeader.ErrorStateIndicator;
	TxHeader.BitRateSwitch = pheader->RxHeader.BitRateSwitch;
	TxHeader.FDFormat = pheader->RxHeader.FDFormat;
	TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker       = 0;

//	if(pheader->RxHeader.Identifier == 0x3DA)
//	{
//		for (int i = 0; i < 8; i++){
//			printf("%d ",pheader->data[i]);
////			data[i] = pheader->data[i];
//		}
//		printf("\n");
//	}
//	for (int i = 0; i < 8; i++){
////		printf("%d ",pheader->data[i]);
//		data[i] = pheader->data[i];
//	}
//	memcpy(data,pheader->data,sizeof(pheader->data));
//	printf("\n");

	/* Forward: FDCAN1 -> FDCAN2; FDCAN2 -> FDCAN1 */
	if(pheader->fdcanx.Instance == hfdcan1.Instance)
	{
		FDCAN_SendData(hfdcan2,TxHeader,pheader->data);
	}
	else
	{
		FDCAN_SendData(hfdcan1,TxHeader,pheader->data);
	}
	/* Call driver to transmit to CAN Bus */
}

void Forwarder::enable(void)
{
	_status = STATUS_ENABLE;
	bufferHandle->clearBuffer();
}

void Forwarder::disable(void)
{
	_status = STATUS_DISABLE;
}
