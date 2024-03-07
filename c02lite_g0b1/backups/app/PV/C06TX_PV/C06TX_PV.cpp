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

#include <Controller/C06TXController/C06TXController.h>
#include <CanDetector/CanDetector.h>

#include <Car/Maxus/Maxus.h>

#include "C06TX_PV.h"
#include <includes.h>

C06TX_PV::C06TX_PV() {

}

C06TX_PV::~C06TX_PV() {
	// TODO Auto-generated destructor stub
}

void C06TX_PV::run(void)
{
	Car *pCar;
	uint8_t value = 0;

	if(gpDrvSwitch1Pin->getValue()) value |= 0x01;
	if(gpDrvSwitch2Pin->getValue()) value |= 0x02;
	if(gpDrvSwitch3Pin->getValue()) value |= 0x04;
	if(gpDrvSwitch4Pin->getValue()) value |= 0x08;

	pCar = new Maxus();
	printf("%s\n", pCar->name);

	C06TXController *pController = new C06TXController(pCar);
#if 1
	CanDetector::getInstance()->start(false);
#endif
	pController->run();
}

