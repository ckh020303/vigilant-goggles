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

#include "AudiQ7_0FD.h"
#include "AudiQ7_0FD_common.h"
#include <CanHub.h>

using namespace AudiQ7_0FD_NS;

AudiQ7_0FD::AudiQ7_0FD() {
		name = "AduiQ7_0FD";
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

AudiQ7_0FD::~AudiQ7_0FD() {
}

bool AudiQ7_0FD::init(void)
{
	bool ret = true;
	return ret;
}

void AudiQ7_0FD::updateSpeedStatus(FDCanRxHeader *pheader)
{
	if( (pheader->RxHeader.IdType == FDCAN_STANDARD_ID) && (pheader->RxHeader.Identifier == SPEED_STATUS_MSG_ID)) {
		if(pheader->data[0] == 0x20) {
			speedValue =(uint32_t) pheader->data[SPEED_HIGH_BYTE] <<8 | pheader->data[SPEED_LOW_BYTE];
		}
	}
}

void AudiQ7_0FD::updateTVFKeyStatus(FDCanRxHeader *pheader)
{
	if( (pheader->RxHeader.IdType == FDCAN_STANDARD_ID) && (pheader->RxHeader.Identifier == KEY_STATUS_MSG_ID)) {
		if((pheader->data[0] & 0x08) == 0x08) {	/* Return Key */
			if ( pheader->data[2] != 0x0) {
				/* Make it simple here:
				 *   - 0x01 - short press
				 *   - 0x04,0x05: keep press
				 *   - 0x06 - holded
				 * but we detect the long press in Controller so don't handle here
				 */
				TVFKeyPressed = true;
			} else {
				TVFKeyPressed = false;
			}
		} else if (pheader->data[0] == 0x0) {
			TVFKeyPressed = false;
		}
	}
}

bool AudiQ7_0FD::filt(FDCanRxHeader *pheader)
{
	bool ret = false;
	if( (pheader->RxHeader.IdType == FDCAN_STANDARD_ID) && (pheader->RxHeader.Identifier == SPEED_STATUS_MSG_ID)) {
		if((pheader->data[0] == 0x20) || (pheader->data[0] == 0x30)) {
			if(speedValue > LIMITINGSPEED_VALUE) {
				pheader->data[SPEED_HIGH_BYTE] = 0;  //LIMITINGSPEED_VALUE>>8;
				pheader->data[SPEED_LOW_BYTE] = 0;     //LIMITINGSPEED_VALUE & 0xFF;
				ret = true;	
			    //printf("speed:%x %x\n",pmsg->msg.Data[SPEED_HIGH_BYTE],pmsg->msg.Data[SPEED_LOW_BYTE]);
			}
		}
	}
	return (ret);
}


