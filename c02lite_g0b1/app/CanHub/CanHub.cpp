/*
 * CanHub.cpp
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */
#include "CanHub.h"

BufferHandle *bufferHandle;

CanHub *CanHub::getInstance(void){
	static CanHub *_this;
	if(!_this) {
		_this = new CanHub;
	}
	return (_this);
}

CanHub::CanHub(void)
{
	static BufferHandle _bufferHandle;
	bufferHandle = &_bufferHandle;
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

void CanHub::_passMsg(FDCanRxHeader *pheader)
{
	std::list<SlotFunction>::iterator itor;
	for(itor=_slotfunction_list.begin(); itor!=_slotfunction_list.end();itor++) {
		(*itor).slot(pheader,(*itor).param);
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
	FDCanRxHeader header;
	int err;

	while(1) {
		/* Handle CAN1 Received Messages */
		err = bufferHandle->pFDCAN1RxBuffer->read(&header);
		if (err) {
			_passMsg(&header);
		}

		/* Handle CAN1 Received Messages */
		err = bufferHandle->pFDCAN2RxBuffer->read(&header);
		if (err) {
			_passMsg(&header);
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
	FDCanRxHeader header;
	int ret;
	/* Handle CAN1 Received Messages */
	ret = bufferHandle->pFDCAN1RxBuffer->read(&header);
	if (ret) {
		_passMsg(&header);
	}

	/* Handle CAN2 Received Messages */
	ret = bufferHandle->pFDCAN2RxBuffer->read(&header);
	if (ret) {
		_passMsg(&header);
	}
}
