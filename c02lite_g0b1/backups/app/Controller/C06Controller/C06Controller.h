/*
 * Controller.h
 *
 *  Created on: 2013-11-4
 *      Author: Administrator
 */

#ifndef C06CONTROLLER_H_
#define C06CONTROLLER_H_

#include <includes.h>
#include <Car/Car.h>
#include <Forwarder/Forwarder.h>
#include <Forwarder/GearForwarder.h>
#include <CanHub/CanHub.h>

class C06Controller {
public:
	C06Controller(Car *pCar);
	~C06Controller(void);
	void run(void);


private:
    void GPIOInit(void);
	void TestMode(void);

	Car *_pCar;
	Forwarder *_pForwarder;
	CanHub *pHub;
	uint8_t _mode;
	bool _status;

	bool isTJA1043;		// True: TJA1043; False: TJA1044
};

#endif /* CONTROLLER_H_ */
