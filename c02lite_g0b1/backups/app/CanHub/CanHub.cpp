/*
 * CanHub.cpp
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */
#include "CanHub.h"

CanHub *CanHub::getInstance(void){
	static CanHub *_this;
	if(!_this) {
		_this = new CanHub;
	}
	return (_this);
}

CanHub::CanHub(void)
{
}

CanHub::~CanHub(void){

}

bool CanHub::registerSlot(Slot_T function,void *param)
{
	bool ret = true;
	SlotFunction slotFunction = {
		.slot = function,
		.param = param,
	};

	_slotfunction_list.push_back(slotFunction);
	return (ret);
}

void CanHub::_passMsg(CanRMsg *msg)
{
	list<SlotFunction>::iterator itor;
	for(itor=_slotfunction_list.begin(); itor!=_slotfunction_list.end();itor++) {
		(*itor).slot(msg,(*itor).param);
	}
}

/*
 * CanHub Main Function
 *   - Get data from two ring buffers one by one
 *   - If data available, then send to CAN Bus
 *   - Will not exit
 *
 */
void CanHub::doTask(void)
{
	CanRMsg msg;
	int err;

	while(1) {
		/* Handle CAN1 Received Messages */
		err = gpDrvCAN->pCAN1RxBuffer->read(&msg);
		if (err) {
			_passMsg(&msg);
		}

		/* Handle CAN1 Received Messages */
		err = gpDrvCAN->pCAN2RxBuffer->read(&msg);
		if (err) {
			_passMsg(&msg);
		}
	}
}

/*
 * CanHub Main Function
 *   - Get data from two ring buffers one by one
 *   - If data available, then send to CAN Bus
 *
 */
void CanHub::doSingleTask(void)
{
	CanRMsg msg;
	int ret;
	/* Handle CAN1 Received Messages */
	ret = gpDrvCAN->pCAN1RxBuffer->read(&msg);
	if (ret) {
		_passMsg(&msg);
	}

	/* Handle CAN2 Received Messages */
	ret = gpDrvCAN->pCAN2RxBuffer->read(&msg);
	if (ret) {
		_passMsg(&msg);
	}

}
