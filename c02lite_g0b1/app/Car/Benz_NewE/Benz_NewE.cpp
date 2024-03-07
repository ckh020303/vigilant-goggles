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

#include "Benz_NewE.h"
#include "Benz_NewE_common.h"
#include <CanHub.h>

using namespace BenzNewE;

Benz_NewE::Benz_NewE() {
	name = "BenzNewE";
	const static uint32_t idList[] = {
        SPEED_STATUS_MSG_ID,
        0x00,
	};
	pCanFilterIDList = idList;
	pCanSpeedHighByte = SPEED_HIGH_BYTE;
	pCanSpeedLowByte = SPEED_LOW_BYTE;
	
	maxLimitingSpeed = LIMITINGSPEED_VALUE;
	baudRate = 666;
	speedValue = 0;
	TVFKeyPressed = false;

	isCanFD = false;
}

Benz_NewE::~Benz_NewE() {
}

bool Benz_NewE::init(void)
{
	bool ret = true;
	return ret;
}

void Benz_NewE::updateSpeedStatus(FDCanRxHeader *pheader)
{
	if( (pheader->RxHeader.IdType == FDCAN_STANDARD_ID) && (pheader->RxHeader.Identifier == SPEED_STATUS_MSG_ID)) {
		//if((pmsg->msg.Data[SPEED_HIGH_BYTE]&0xf0)!= 0x00) {
			speedValue = ((uint32_t)((pheader->data[SPEED_HIGH_BYTE]&0x0f)<<8) | (pheader->data[SPEED_LOW_BYTE]));
		//}
	}
}

void Benz_NewE::updateTVFKeyStatus(FDCanRxHeader *pheader)
{
	if( (pheader->RxHeader.IdType == FDCAN_STANDARD_ID) && (pheader->RxHeader.Identifier == KEY_STATUS_BYTE)) {
		if((pheader->data[4] & 0x04)==0x04) {
			TVFKeyPressed = true;
		} else {
			TVFKeyPressed = false;
		}
	}
}

bool Benz_NewE::filt(FDCanRxHeader *pheader)
{
	bool ret = false;
	if( (pheader->RxHeader.IdType == FDCAN_STANDARD_ID) && (pheader->RxHeader.Identifier == SPEED_STATUS_MSG_ID)) {
			if(speedValue > LIMITINGSPEED_VALUE) {
				pheader->data[SPEED_HIGH_BYTE] = 0x80 | (((LIMITINGSPEED_VALUE)>>8) & 0x0F);
				pheader->data[SPEED_LOW_BYTE] = (LIMITINGSPEED_VALUE) & 0xFF;
				ret = true;
			}
	}
	return (ret);
}

