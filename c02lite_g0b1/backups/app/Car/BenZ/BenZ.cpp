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

#include "BenZ.h"
#include "BenZ_common.h"
#include <CanHub/CanHub.h>

using namespace BenZNS;

BenZ::BenZ() {
	name = "BenZ";
	const static uint32_t idList[] = {
        SPEED_STATUS_MSG_ID,
        0x00,
	};
	pCanFilterIDList = idList;
	pCanSpeedHighByte = SPEED_HIGH_BYTE;
	pCanSpeedLowByte = SPEED_LOW_BYTE;
	
	maxLimitingSpeed = LIMITINGSPEED_VALUE;
	baudRate = 125;
	speedValue = 0;
	TVFKeyPressed = false;

}

BenZ::~BenZ() {
}

bool BenZ::init(void)
{
	bool ret = true;
	return ret;
}

void BenZ::updateSpeedStatus(CanRMsg *pmsg)
{
	if((pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == SPEED_STATUS_MSG_ID)) {
//		if((pmsg->msg.Data[0] & 0x10)!= 0x10) {
			speedValue = pmsg->msg.Data[SPEED_HIGH_BYTE] &0x0F;
			speedValue <<= 8;
			speedValue |= pmsg->msg.Data[SPEED_LOW_BYTE];
			speedValue *= 10;
//		}
	}
}

void BenZ::updateTVFKeyStatus(CanRMsg *pmsg)
{
	if((pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == KEY_STATUS_BYTE)) {
		if( (pmsg->msg.Data[5]&0x02) == 0x02) {
			TVFKeyPressed = true;
		} else {
			TVFKeyPressed = false;
		}
	}
}

bool BenZ::filt(CanRMsg *pmsg)
{
	bool ret = false;
	if((pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == SPEED_STATUS_MSG_ID)) {
//		if((pmsg->msg.Data[0] & 0x10)!= 0x10) {
			if(speedValue > LIMITINGSPEED_VALUE) {
				pmsg->msg.Data[SPEED_HIGH_BYTE] &= ((LIMITINGSPEED_VALUE/10)>>8) & 0x0F;
				pmsg->msg.Data[SPEED_LOW_BYTE] = (LIMITINGSPEED_VALUE/10) & 0xFF;
				ret = true;
			}
//		}
	}
	return (ret);
}

