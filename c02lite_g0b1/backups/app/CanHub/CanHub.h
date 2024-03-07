/*
 * CanHub.h
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#ifndef CANHUB_H_
#define CANHUB_H_
#include <includes.h>

typedef void (*Slot_T)(CanRMsg *msg,void *param);
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
	void _passMsg(CanRMsg *msg);
	list<SlotFunction> _slotfunction_list;
};


#endif /* CANHUB_H_ */
