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

#include <Controller/C06Controller/C06Controller.h>
#include <CanDetector/CanDetector.h>
#include <Car/IVECO/IVECO.h>
#include <Car/IVECO-radio/Iveco_Radio.h>
#include <Car/IVECO-radio2/Iveco_Radio2.h>
#include <Car/IVECO-Yellow/Iveco_Yellow.h>
#include <Car/IVECOS-WAY/IvecoS_Way.h>
#include <Car/IVECOS-WAY2/IvecoS_Way2.h>
#include <Car/IVECOS/IvecoS.h>
#include <Car/Fiat_Ducato/Fiat_Ducato.h>
#include <Car/Sprinter_Psm/Sprinter_Psm.h>
#include <Car/Sprinter_OBD/Sprinter_OBD.h>
#include <Car/Sprinter_OBD_2/Sprinter_OBD_2.h>
#include <Car/Ford_Transit/Ford_Transit.h>
#include <Car/Ford_Fiesta/Ford_Fiesta.h>
#include <Car/Ford_Transit2/Ford_Transit2.h>
#include <Car/Ford_Transit3/Ford_Transit3.h>
#include <Car/Ford_Etransit/Ford_Etransit.h>
#include <Car/Hyundai_Kona/Hyundai_Kona.h>
#include <Car/Citroen_EDispatch/Citroen_EDispatch.h>
#include <Car/MAN_TGS/MAN_TGS.h>
#include <Car/MAN_TGS2/MAN_TGS2.h>
#include <Car/IVECO_Daily/IVECO_Daily.h>
#include <Car/IVECO_Daily2/IVECO_Daily2.h>
#include <Car/IVECO_Daily3/IVECO_Daily3.h>
#include <Car/IVECO_Daily4/IVECO_Daily4.h>
#include <Car/IVECO_Daily5/IVECO_Daily5.h>
#include <Car/Maxus/Maxus.h>
#include <Car/LEVC/LEVC.h>
#include <Car/DAF_CF/DAF_CF.h>
#include <Car/DAF_CF1/DAF_CF1.h>
#include <Car/DAF_1/DAF_1.h>
#include <Car/Landrover_Defender/Landrover_Defender.h>
#include <Car/VOLVO_FH/VOLVO_FH.h>
#include <Car/Nissan_E-NV200/Nissan.h>
#include <Car/LDV_Maxus_2021/LDV_Maxus_2021.h>
#include <Car/LDV_Maxus_2021_2/LDV_Maxus_2021_2.h>
#include <Car/LDV_Maxus_2021_3/LDV_Maxus_2021_3.h>
#include <Car/LDV_Maxus_2021_4/LDV_Maxus_2021_4.h>
#include <Car/LDV_Maxus_2021_5/LDV_Maxus_2021_5.h>
#include <Car/LDV_Maxus_2021_6/LDV_Maxus_2021_6.h>
#include <Car/Fiat_talento_2017/Fiat_talento_2017.h>

#include "C06_PV.h"
#include <includes.h>

C06_PV::C06_PV() {

}

C06_PV::~C06_PV() {
	// TODO Auto-generated destructor stub
}

void C06_PV::run(void)
{
	Car *pCar;
	uint8_t value = 0;

	if(gpDrvSwitch1Pin->getValue()) value |= 0x01;
	if(gpDrvSwitch2Pin->getValue()) value |= 0x02;
	if(gpDrvSwitch3Pin->getValue()) value |= 0x04;
	if(gpDrvSwitch4Pin->getValue()) value |= 0x08;

#if defined(NISSAN)
	pCar = new Nissan();
#elif defined(FIAT_DUCATO)
	pCar = new Fiat_Ducato();
#elif defined(FORD_FIESTA)
	pCar = new Ford_Fiesta();
#elif defined(IVECOS_WAY2)
	pCar = new IvecoS_Way2();
#elif defined(CITROEN_EDISPATCH)
	pCar = new Citroen_EDispatch();
#elif defined(SPRINTER_OBD)
	pCar = new Sprinter_OBD();
#elif defined(SPRINTER_OBD_2)
	pCar = new Sprinter_OBD_2();
#elif defined(FORD_TRANSIT3)
	pCar = new Ford_Transit3();
#elif defined(FORD_ETRANSIT)
	pCar = new Ford_Etransit();
#elif defined(HYUNDAI_KONA)
	pCar = new Hyundai_Kona();
#elif defined(IVECO_RADIO2)
	pCar = new Iveco_Radio2();
#elif defined(IVECO_DAILY2)
	pCar = new IVECO_Daily2();
#elif defined(DAF_CF)
	switch(value){
	case 0:
		pCar = new DAF_cf();
		break;
	case 1:
		pCar = new DAF_cf1();
		break;
	default:
		pCar = new DAF_cf();
		break;
	}
#elif defined(DAF_1)
	pCar = new Daf_1();
#elif defined(VOLVO_FH)
	pCar = new VOLVO_fH();
#elif defined(Landrover_Defender)
	pCar = new Landrover_defender();
#elif defined(IVECO_DAILY3)
	pCar = new IVECO_Daily3();
#elif defined(IVECO_DAILY4)
	pCar = new IVECO_Daily4();
#elif defined(Levc)
	pCar = new LEVC();
#elif defined(IVECO_DAILY5)
	pCar = new IVECO_Daily5();
#elif defined(LDV_MAXUS_2021)
	switch(value){
	case 0:
		pCar = new LDV_Maxus_2021();
		break;
	case 1:
		pCar = new LDV_Maxus_2021_2();
		break;
	case 2:
		pCar = new LDV_Maxus_2021_3();     //LDV ED3 1
		break;
	case 3:
		pCar = new LDV_Maxus_2021_4();     //LDV ED3 2
		break;
	case 4:
		pCar = new LDV_Maxus_2021_5();     
		break;
	case 5:
		pCar = new LDV_Maxus_2021_6();     
		break;
	default:
		pCar = new LDV_Maxus_2021();
		break;
	}
#elif defined(SPRINTER_PSM)
	pCar = new Sprinter_Psm();
#elif defined(FIAT_TALENTO_2017)
	pCar = new Fiat_talento_2017();
#else
	// DIP Settings
	switch (value) {
	case 0:
		pCar = new Iveco();
		break;
	case 1:
		pCar = new Iveco_Radio();
		break;
	case 2:
		pCar = new Iveco_Yellow();
		break;
	case 3:
		pCar = new IvecoS_Way();
		break;
	case 4:
		pCar = new Fiat_Ducato();
		break;
	case 5:
		pCar = new IvecoS();
		break;
	case 6:
		pCar = new Ford_Transit();
		break;
	case 7:
		pCar = new MAN_TGS();
		break;
	case 8:
		pCar = new IVECO_Daily();
		break;
	case 9:
		pCar = new Maxus();
		break;
	case 10:
		pCar = new MAN_TGS2();
		break;
	case 11:
		pCar = new Sprinter_Psm();
		break;
	case 12:
		pCar = new Sprinter_OBD();
		break;
	case 13:
		pCar = new IVECO_Daily2();
		break;
	case 14:
		pCar = new Ford_Transit2();
		break;
	default:
		//pCar = new Iveco();
		pCar = new Fiat_Ducato();
		break;
	}
#endif

	printf("%s\n", pCar->name);

	C06Controller *pController = new C06Controller(pCar);
	CanDetector::getInstance()->start();
	pController->run();
}

