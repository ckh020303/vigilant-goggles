/*
 * Controller.h
 *
 *  Created on: 2013-11-4
 *      Author: Administrator
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <includes.h>
#include <Car/Car.h>
#include <Forwarder/Forwarder.h>
#include <Forwarder/ExForwarder.h>
#include <CanHub/CanHub.h>

class Controller {
public:
	Controller(Car *pCar,uint8_t mode=MODE_NORMAL);
	~Controller(void);
	void run(void);
	enum {
		MODE_NORMAL=0,
		MODE_SAFE,
	};
	enum {
		STATUS_ON=true,
		STATUS_OFF=false,
	};

private:
	void _writeStatus2Flash(bool needLimiting =false);
	bool _readStatusFromFlash(void);

	void handleUserButtons(void);

	Car *_pCar;
	Forwarder *_pForwarder;
	CanHub *pHub;
	uint8_t _mode;
	bool _status;
};

#endif /* CONTROLLER_H_ */
