/*
 * ExForwarder.cpp
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#include "ExForwarder.h"

ExForwarder::ExForwarder(Car *pCar) : Forwarder(pCar){
}

ExForwarder::~ExForwarder(void) {

}

void ExForwarder::init(void) {
	CanHub::getInstance()->registerSlot(slot,this);
}

void ExForwarder::slot(FDCanRxHeader *pheader,void *param)
{
	ExForwarder *_this = (ExForwarder*)param;
    bool need_filter = false;

	/* update Steering Button Keys */
	if(pheader->fdcanx.Instance == hfdcan1.Instance) _this->_pCar->updateTVFKeyStatus(pheader);

	if(_this->_status == STATUS_ENABLE) {
        printf("ExForwarder run\n");
		if(pheader->fdcanx.Instance == hfdcan1.Instance) _this->_pCar->updateSpeedStatus(pheader);

		if(_this->_pCar->speedValue > 0) {
			Relay_WritePin(GPIO_PIN_SET);
        }
        if(getValue(RELAY_CTRL_GPIO_Port, RELAY_CTRL_Pin)){
			if(((pheader->fdcanx.Instance == hfdcan1.Instance))&&(_this->_pCar->filt(pheader))) {
				/* Don't send this message and drop it */
				printf("not send!\n");
                need_filter = true;
			}
            if(!need_filter){
			    _this->_doForward(pheader);
            }
		}
	} else {
		Relay_WritePin(GPIO_PIN_RESET);
		bufferHandle->clearBuffer();			/* Clear CAN RX Buffer and drop datas */
	}
}

