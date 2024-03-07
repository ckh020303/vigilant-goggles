/*
 * Forwarder.cpp
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#include "GearForwarder.h"
#include <CanHub/CanHub.h>

GearForwarder::GearForwarder(Car *pCar) : Forwarder(pCar) {
	_status = STATUS_DISABLE;
	_pCar = pCar;
}

GearForwarder::~GearForwarder(void) {

}

void GearForwarder::init(void) {
	CanHub::getInstance()->registerSlot(slot,this);
}

void GearForwarder::slot(CanRMsg *pmsg,void *param)
{
	GearForwarder *_this = (GearForwarder*)param;

	if(pmsg->CANx == CAN1) {
		/* Filter CAN Message from CAN1 */
		_this->_pCar->run(pmsg);
	}
//		_this->_doForward(pmsg);

}

void GearForwarder::_doForward(CanRMsg *pcanRMsg)
{
	CanTxMsg TxMessage;
	CAN_Type *CANx;

	TxMessage.ExtId = pcanRMsg->msg.ExtId;
	TxMessage.StdId = pcanRMsg->msg.StdId;
	TxMessage.IDT   = pcanRMsg->msg.IDT;
	TxMessage.RTR   = pcanRMsg->msg.RTR;
	TxMessage.StdId = pcanRMsg->msg.StdId;
	TxMessage.DLC   = pcanRMsg->msg.DLC;
	for (int i = 0; i<TxMessage.DLC; i++) {
		TxMessage.Data[i] = pcanRMsg->msg.Data[i];
	}

	CANx = CAN1;

	/* Call driver to transmit to CAN Bus */
	gpDrvCAN->transmit(CANx, &TxMessage, false);
}

void GearForwarder::enable(void)
{
	_status = STATUS_ENABLE;
	gpDrvCAN->clearBuffer();
}

void GearForwarder::disable(void)
{
	_status = STATUS_DISABLE;
}

