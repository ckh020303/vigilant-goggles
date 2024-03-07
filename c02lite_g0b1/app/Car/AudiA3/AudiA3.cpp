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

#include "AudiA3.h"
#include "AudiA3_common.h"
#include <CanHub.h>

using namespace AudiA3NS;

AudiA3::AudiA3() {
	name = "AudiA3";
	const static uint32_t idList[] = {
        SPEED_STATUS_MSG_ID,
        0x00,
	};
	pCanFilterIDList = idList;
	pCanSpeedHighByte = SPEED_HIGH_BYTE;
	pCanSpeedLowByte = SPEED_LOW_BYTE;
	
	maxLimitingSpeed = LIMITINGSPEED_VALUE;
	baudRate = 500;
	speedValue = 0;
	TVFKeyPressed = false;

	isCanFD = false;
}

AudiA3::~AudiA3() {
}

bool AudiA3::init(void)
{
	bool ret = true;
	return ret;
}

void AudiA3::updateSpeedStatus(FDCanRxHeader *pheader)
{

	if( (pheader->RxHeader.IdType == FDCAN_STANDARD_ID) && (pheader->RxHeader.Identifier == SPEED_STATUS_MSG_ID)) {
		if(pheader->data[0] == 0x20) {
			speedValue =(uint32_t) pheader->data[SPEED_HIGH_BYTE] <<8 | pheader->data[SPEED_LOW_BYTE];
		}
	}
}

void AudiA3::updateTVFKeyStatus(FDCanRxHeader *pheader)
{

}

bool AudiA3::filt(FDCanRxHeader *pheader)
{
	bool ret = false;
//	int i;
	if( (pheader->RxHeader.IdType == FDCAN_STANDARD_ID) && (pheader->RxHeader.Identifier == SPEED_STATUS_MSG_ID)) {
		if((pheader->data[0] == 0x20) || (pheader->data[0] == 0x30)) {
			if(speedValue > LIMITINGSPEED_VALUE) {
				pheader->data[SPEED_HIGH_BYTE] = LIMITINGSPEED_VALUE>>8;
				pheader->data[SPEED_LOW_BYTE] = LIMITINGSPEED_VALUE & 0xFF;
				ret = true;
			}
		}
	}
	return (ret);
}



