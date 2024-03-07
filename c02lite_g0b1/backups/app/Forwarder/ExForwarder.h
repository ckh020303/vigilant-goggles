/*
 * Forwarder.h
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#ifndef EXFORWARDER_H_
#define EXFORWARDER_H_

#include <includes.h>
#include <Car/Car.h>
//#include "GreenLEDMonostable/GreenLEDMonostable.h"
#include <Forwarder/Forwarder.h>

class ExForwarder : public Forwarder{
public:
	ExForwarder(Car *pCar);
	~ExForwarder(void);
	void init(void);
	static void slot(CanRMsg *pmsg,void *param);

};

#endif /* FORWARDER_H_ */
