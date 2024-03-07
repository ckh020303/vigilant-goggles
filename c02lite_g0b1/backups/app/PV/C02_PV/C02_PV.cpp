/*
 * RoadPassion CONFIDENTIAL
 *
 * Copyright 2013 RoadPassion Electronics Co., Ltd.
 *
 * DO NOT COPY AND/OR REDISTRIBUTE WITHOUT PERMISSION.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <Controller/C02Controller/Controller.h>
#include <CanDetector/CanDetector.h>
#include <Car/BenZ/BenZ.h>
#include <Car/AudiA3/AudiA3.h>
#include <Car/AudiQ7_0FD/AudiQ7_0FD.h>
#include <Car/Golf7/Golf7.h>
#include <Car/BenzS/BenzS.h>
#include <Car/BenzC/BenzC.h>
#include <Car/Benz_NewE/Benz_NewE.h>
#include <Car/AudiA7_2019/AudiA7_2019.h>
#include <Car/VWCaddy/VWCaddy.h>

//#include <../Zox/utils/SysTick.h>
#include <SysTick.h>

#include "C02_PV.h"
#include <includes.h>

C02_PV::C02_PV() {
	// TODO Auto-generated constructor stub
}

C02_PV::~C02_PV() {
	// TODO Auto-generated destructor stub
}
void C02_PV::run(void)
{
	Car *pCar;
	uint8_t value = 0;
	if(gpDrvSwitch1Pin->getValue()) value |= 0x01;
	if(gpDrvSwitch2Pin->getValue()) value |= 0x02;
	if(gpDrvSwitch3Pin->getValue()) value |= 0x04;

#ifndef PV_C02_VW_CADDY
	switch (value) {
	case 0:
		pCar = new AudiA3();
		break;
	case 1:
		pCar = new BenZ();
		break;
	case 2:
		pCar = new Golf7();
		break;
	case 3:
		pCar = new BenzS();
		break;
	case 4:
		pCar = new BenzC();
		break;
	case 5:
		pCar = new AudiQ7_0FD();
		break;
	case 6:
		pCar = new AudiA7_2019();
		break;
    case 7:
        pCar = new Benz_NewE();
        break;
	default:
		pCar = new BenzC();
		break;
	}
#else
	pCar = new VWCaddy();
#endif
	zprintf("%s\n",pCar->name);

	/* IMPORTANT NOTES:
	 *   - Wait about 2 seconds for NTG5.6 Headunit booting
	 *   - otherwise CAN2 initialization failed;
	 *   - CAN1 can receive data without this delay;
	 */
	delay_1ms(2500);

	uint8_t mode = gpDrvSwitch4Pin->getValue() ? Controller::MODE_SAFE : Controller::MODE_NORMAL;
	Controller *pController = new Controller(pCar,mode);
	CanDetector::getInstance()->start();
	pController->run();
}
