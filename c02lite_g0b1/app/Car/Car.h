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

#ifndef CAR_H_
#define CAR_H_

#include "BufferHandle.h"

class Car  {
public:

	Car();
	virtual ~Car();
	virtual bool init(void) = 0;
	virtual void run(FDCanRxHeader *pheader) = 0;
	virtual bool filt(FDCanRxHeader *pheader) = 0;

	virtual bool ack(FDCanRxHeader *pheader);

	virtual void updateSpeedStatus(FDCanRxHeader *pheader) = 0;
	virtual void updateTVFKeyStatus(FDCanRxHeader *pheader) = 0;

	uint32_t maxLimitingSpeed;
	uint32_t baudRate;
	uint32_t speedValue;
	const char *name;
	bool TVFKeyPressed;
	bool isTVFKeyHolded;

	const uint32_t *pCanFilterIDList;
	uint8_t pCanSpeedHighByte;	
	uint8_t pCanSpeedLowByte;

	bool isCanFD;
	uint32_t baudRateData;
};

#endif /* CAR_H_ */
