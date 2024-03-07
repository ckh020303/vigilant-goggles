/*
 * ExForwarder.cpp
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#include "ExForwarder.h"
#include <CanHub/CanHub.h>

ExForwarder::ExForwarder(Car *pCar) : Forwarder(pCar){
}

ExForwarder::~ExForwarder(void) {

}

void ExForwarder::init(void) {
	CanHub::getInstance()->registerSlot(slot,this);
}


void ExForwarder::slot(CanRMsg *pmsg,void *param)
{
	ExForwarder *_this = (ExForwarder*)param;
    bool need_filter = false;
    
	/* update Steering Button Keys */
	if(pmsg->CANx == CAN1) _this->_pCar->updateTVFKeyStatus(pmsg);

	if(_this->_status == STATUS_ENABLE) {
        printf("ExForwarder run\n");
		if(pmsg->CANx == CAN1) _this->_pCar->updateSpeedStatus(pmsg);

		if(_this->_pCar->speedValue > 0) {
			gpDrvRelay->on();
        }
        if(gpDrvRelay->getValue()){
			if(((pmsg->CANx == CAN1))&&(_this->_pCar->filt(pmsg))) {
				/* Don't send this message and drop it */
                need_filter = true;
			}
            if(!need_filter){
			    _this->_doForward(pmsg);
            }
		}
	} else {
		gpDrvRelay->off();
		gpDrvCAN->clearBuffer();			/* Clear CAN RX Buffer and drop datas */
	}
}

