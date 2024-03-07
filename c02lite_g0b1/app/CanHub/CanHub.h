/*
 * CanHub.h
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#ifndef CANHUB_H_
#define CANHUB_H_

#include "BufferHandle.h"

extern BufferHandle *bufferHandle;
extern FDCanRxHeader _FDCan1RxHeader,_FDCan2RxHeader;

typedef void (*Slot_T)(FDCanRxHeader *header,void *param);

struct SlotFunction {
	Slot_T slot;
	void *param;
};

class CanHub {
public:
	static CanHub *getInstance(void);
	bool registerSlot(Slot_T function,void *param);
	void doTask(void);
	void doSingleTask(void);

private:
	CanHub(void);
	~CanHub(void);
	void _passMsg(FDCanRxHeader *pheader);
	std::list<SlotFunction> _slotfunction_list;
};


#endif /* CANHUB_H_ */
