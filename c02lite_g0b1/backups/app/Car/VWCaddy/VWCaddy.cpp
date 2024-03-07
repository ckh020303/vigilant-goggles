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

#include "VWCaddy.h"
#include "VWCaddy_common.h"
#include <CanHub/CanHub.h>

using namespace VWCaddyNS;

VWCaddy::VWCaddy() {
	name = "VWCaddy";
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

	isFirstEnterCamera = false;
}

VWCaddy::~VWCaddy() {
}

bool VWCaddy::init(void)
{
	bool ret = true;
	return ret;
}

void VWCaddy::updateSpeedStatus(CanRMsg *pmsg)
{
	if( (pmsg->msg.IDT == CAN_ID_STD) && (pmsg->msg.StdId == SPEED_STATUS_MSG_ID)) {
		if  (pmsg->msg.Data[7] & 0x80) {
			speedValue = 10;
		} else {
			speedValue = 0;
		}
	}
}

void VWCaddy::updateTVFKeyStatus(CanRMsg *pmsg)
{

}

bool VWCaddy::filt(CanRMsg *pmsg)
{
	bool ret = false;

	if((pmsg->msg.IDT == CAN_ID_EXT) && ((pmsg->msg.ExtId == CAM_COM_ID1) || (pmsg->msg.ExtId == CAM_COM_ID2))) {
		if (pmsg->msg.ExtId == CAM_COM_ID1) {
			// HU -> GW: 17335500
#if 0
			if ((pmsg->msg.Data[1] == 0x0B) && (pmsg->msg.Data[3] == 0x15) && (pmsg->msg.Data[5] == 0x10)) {
				// 80 0B 00 15 01 10 03 00
				if ( pmsg->msg.Data[6] == 0x01) {
					pmsg->msg.Data[6] = 0x03;
				} else if (pmsg->msg.Data[6] == 0x03) {
					pmsg->msg.Data[6] = 0x01;
				}
				ret = true;
			}
#endif
		} else if (pmsg->msg.ExtId == CAM_COM_ID2) {
			// GW -> HU: 17335510
			if ((pmsg->msg.Data[1] == 0x0B) && (pmsg->msg.Data[2] == 0x40) && (pmsg->msg.Data[3] == 0x10)) {
				// Enter: 17335510   A0 0B 40 10 01 00 00 00
				// Exit:  17335510   A0 0B 40 10 00 00 00 00
			} else if ((pmsg->msg.Data[1] == 0x0B) && (pmsg->msg.Data[3] == 0x13) && (pmsg->msg.Data[5] == 0x10)) {
				// 90 0B 40 13 41 10 03 00 <-> 90 0B 40 13 41 10 01 00
#if 0
				if ((pmsg->msg.Data[6] == 0x01)) {
					pmsg->msg.Data[6] = 0x03;
				} else if ((pmsg->msg.Data[6] == 0x03)) {
					pmsg->msg.Data[6] = 0x01;
				}
				ret = true;
#endif
			}
		}
	}

	return (ret);
}

bool VWCaddy::ack(CanRMsg *pmsg)
{
	bool ret = false;
	CanTxMsg TxMessage;

	if((pmsg->msg.IDT == CAN_ID_EXT) && (pmsg->msg.ExtId == CAM_COM_ID2)) {
		// RCV: 17335510 A0 0B 40 10 01 00 00 00 (Enter Camera)
		//      17335510 90 0B 40 13 41 10 01 00 (View)
		// ACK: 17335500 80 0B 00 15 01 10 03 00 and C0 00 00 00 00 00 00 00 (Switch View)

		if ((pmsg->msg.Data[1] == 0x0B) && (pmsg->msg.Data[2] == 0x40) && \
				(pmsg->msg.Data[3] == 0x10) && (pmsg->msg.Data[4] == 0x01)) {

			TxMessage.ExtId = CAM_COM_ID1;
			TxMessage.IDT   = pmsg->msg.IDT;
			TxMessage.RTR   = pmsg->msg.RTR;
			TxMessage.StdId = pmsg->msg.StdId;
			TxMessage.DLC   = pmsg->msg.DLC;

			TxMessage.Data[0] = 0x80;
			TxMessage.Data[1] = 0x0B;
			TxMessage.Data[2] = 0x00;
			TxMessage.Data[3] = 0x15;
			TxMessage.Data[4] = 0x01;
			TxMessage.Data[5] = 0x10;
			TxMessage.Data[6] = 0x03;
			TxMessage.Data[7] = 0x00;

			/* Call driver to transmit to CAN Bus */
			gpDrvCAN->transmit(pmsg->CANx, &TxMessage, false);

			//delay_1ms(5);

			TxMessage.Data[0] = 0xC0;
			TxMessage.Data[1] = 0x00;
			TxMessage.Data[2] = 0x00;
			TxMessage.Data[3] = 0x00;
			TxMessage.Data[4] = 0x00;
			TxMessage.Data[5] = 0x00;
			TxMessage.Data[6] = 0x00;
			TxMessage.Data[7] = 0x00;

			gpDrvCAN->transmit(pmsg->CANx, &TxMessage, false);

			ret = true;
		}
	}

	return ret;
}

