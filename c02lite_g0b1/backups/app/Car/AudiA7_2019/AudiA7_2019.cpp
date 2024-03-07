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

#include "AudiA7_2019.h"
#include "AudiA7_2019_common.h"

using namespace AUDIA7_2019;

AudiA7_2019::AudiA7_2019() {
	name = "AudiA7_2019";
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

AudiA7_2019::~AudiA7_2019() {
}

bool AudiA7_2019::init(void)
{
	bool ret = true;
	return ret;
}

void AudiA7_2019::updateSpeedStatus(CanRMsg *pmsg)
{
	if((pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == SPEED_STATUS_MSG_ID)) {
		speedValue = (u8)((pmsg->msg.Data[5] <<8 | pmsg->msg.Data[4])/100);
	}
}

void AudiA7_2019::updateTVFKeyStatus(CanRMsg *pmsg)
{
	if((pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == TVF_STATUS_MSG_ID)) {
		if( (pmsg->msg.Data[0]==0x21) && (pmsg->msg.Data[2]&0x07)) {
			TVFKeyPressed = true;
		} else {
			TVFKeyPressed = false;
		}
	}
}

bool AudiA7_2019::filt(CanRMsg *pmsg)
{
	bool ret = false;
	if((pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == SPEED_STATUS_MSG_ID)) {
		if(speedValue > LIMITINGSPEED_VALUE) {
			pmsg->msg.Data[SPEED_HIGH_BYTE] = 0;
			pmsg->msg.Data[SPEED_LOW_BYTE] = 0;
			ret = true;
		}	
	}
	return (ret);
}


