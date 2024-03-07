/*
 * Forwarder.h
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#ifndef EXFORWARDER_H_
#define EXFORWARDER_H_

#include <Car.h>
#include <Forwarder.h>
#include "CanHub.h"
#include "../Bsp/Bsp.h"

class ExForwarder : public Forwarder{
public:
	ExForwarder(Car *pCar);
	~ExForwarder(void);
	void init(void);
	static void slot(FDCanRxHeader *pheader,void *param);
};

#endif /* FORWARDER_H_ */
