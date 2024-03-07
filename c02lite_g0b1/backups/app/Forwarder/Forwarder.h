/*
 * Forwarder.h
 *
 *  Created on: 2013-11-12
 *      Author: Administrator
 */

#ifndef FORWARDER_H_
#define FORWARDER_H_

#include <includes.h>
#include <Car/Car.h>

class Forwarder {
public:
	enum {
		STATUS_ENABLE=true,
		STATUS_DISABLE=false,
	};
	Forwarder(Car *pCar);
	virtual ~Forwarder(void);
	virtual void init(void);
	void enable(void);
	void disable(void);
	static void slot(CanRMsg *pmsg,void *param);
protected:
	void _doForward(CanRMsg *pmsg);
	Car *_pCar;
	
	bool _status;
//	GreenLEDMonostable *_pGreenLEDMonostable;
	const static uint32_t _BUFFER_TIME = 500;
private:	
	
};

#endif /* FORWARDER_H_ */
