/*
 * Forwarder.cpp
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#include "Forwarder.h"
#include <CanHub/CanHub.h>

Forwarder::Forwarder(Car *pCar) {
	_status = STATUS_DISABLE;
	_pCar = pCar;
}

Forwarder::~Forwarder(void) {

}

void Forwarder::init(void) {
	CanHub::getInstance()->registerSlot(slot,this);
}

void Forwarder::slot(CanRMsg *pmsg,void *param)
{
	Forwarder *_this = (Forwarder*)param;

	/* update Steering Button Keys */
	if(pmsg->CANx == CAN1) _this->_pCar->updateTVFKeyStatus(pmsg);

	if(_this->_status == STATUS_ENABLE) {
		if(pmsg->CANx == CAN1) _this->_pCar->updateSpeedStatus(pmsg);
		if(_this->_pCar->speedValue > 0) {
			gpDrvRelay->on();
			gpDrvLEDGreen->on();
#ifndef PC_C02_VW_CADDY
			if(pmsg->CANx == CAN1) {
				/* Filter CAN Message from CAN1 */
				_this->_pCar->filt(pmsg);
			}
#else
			// Both Direction Need Filter
			_this->_pCar->filt(pmsg);
#endif
			_this->_doForward(pmsg);

			_this->_pCar->ack(pmsg);
		} else {
			gpDrvRelay->off();
			gpDrvLEDGreen->off();
			gpDrvCAN->clearBuffer();		/* Clear CAN RX Buffer and drop datas */
		}
	} else {
		gpDrvRelay->off();
		gpDrvLEDGreen->off();
		gpDrvCAN->clearBuffer();			/* Clear CAN RX Buffer and drop datas */
	}
}

void Forwarder::_doForward(CanRMsg *pcanRMsg)
{
	CanTxMsg TxMessage;
	CAN_Type *CANx;

	TxMessage.ExtId = pcanRMsg->msg.ExtId;
	TxMessage.IDT   = pcanRMsg->msg.IDT;
	TxMessage.RTR   = pcanRMsg->msg.RTR;
	TxMessage.StdId = pcanRMsg->msg.StdId;
	TxMessage.DLC   = pcanRMsg->msg.DLC;
	for (int i = 0; i<TxMessage.DLC; i++) {
		TxMessage.Data[i] = pcanRMsg->msg.Data[i];
	}

	/* Forward: CAN1 -> CAN2; CAN2 -> CAN1 */
	CANx = pcanRMsg->CANx==CAN1 ? CAN2 : CAN1;

	/* Call driver to transmit to CAN Bus */
	gpDrvCAN->transmit(CANx, &TxMessage, false);
}

void Forwarder::enable(void)
{
	_status = STATUS_ENABLE;
	gpDrvCAN->clearBuffer();
}

void Forwarder::disable(void)
{
	_status = STATUS_DISABLE;
}
