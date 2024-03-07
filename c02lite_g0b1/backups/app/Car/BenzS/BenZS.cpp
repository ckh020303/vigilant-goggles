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

#include "BenzS.h"
#include "BenzS_common.h"
#include <CanHub/CanHub.h>

using namespace BenzSNS;

BenzS::BenzS() {
	name = "BenzS";
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
}

BenzS::~BenzS() {
}

bool BenzS::init(void)
{
	bool ret = true;
	return ret;
}

void BenzS::updateSpeedStatus(CanRMsg *pmsg)
{
	if((pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == SPEED_STATUS_MSG_ID)) {
		if(pmsg->msg.Data[0]!= 0x10) {
			speedValue = ((uint32_t)(pmsg->msg.Data[SPEED_HIGH_BYTE] <<8) | (pmsg->msg.Data[SPEED_LOW_BYTE])) * 100;
		}
	}

}

void BenzS::updateTVFKeyStatus(CanRMsg *pmsg)
{
	if((pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == KEY_STATUS_BYTE)) {
		if( (pmsg->msg.Data[5]&0x02) == 0x02) {
			TVFKeyPressed = true;
		} else {
			TVFKeyPressed = false;
		}
	}
}

bool BenzS::filt(CanRMsg *pmsg)
{
	if(pmsg->msg.StdId == TVF_STATUS_MSG_ID)
		return true;
	return false;
}


