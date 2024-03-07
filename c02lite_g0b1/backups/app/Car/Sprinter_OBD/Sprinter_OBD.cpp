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

#include "Sprinter_OBD.h"
#include "Sprinter_OBD_common.h"

using namespace sprinter_obd;

Sprinter_OBD::Sprinter_OBD(){
	name = "Sprinter_OBD";
	const static uint32_t idList[] = {
			SHIFT_POS_MSG_ID,
			HANDBRAKE_MSG_ID,
			IGNITION_MSG_ID,
			SPEED_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;

	ignition_status  = false;
	reverse_status   = false;
	handbrake_status = false;
}

Sprinter_OBD::~Sprinter_OBD() {
}

bool Sprinter_OBD::init(void)
{
	return 0;
}

void Sprinter_OBD::run(CanRMsg *pmsg)
{
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case IGNITION_MSG_ID:
			_updateIgnition(pmsg);
			break;
		case HANDBRAKE_MSG_ID:
			_updateHandbrake(pmsg);
			break;
		case SHIFT_POS_MSG_ID:
			_updateShiftPos(pmsg);
			break;
		case SPEED_MSG_ID:
			_updateSpeed(pmsg);
			break;
		default:
			break;
		}
	}
	outPutController();
}

void Sprinter_OBD::_updateShiftPos(CanRMsg *pmsg)
{
	// Auto Gear
	if(pmsg->msg.Data[6] == 0x59){
		reverse_status = true;
	}else{
		reverse_status = false;
	}

	// TODO: Manual Gear
}

void Sprinter_OBD::_updateHandbrake(CanRMsg *pmsg)
{
	// Data[1] == 0x10 -> On,  HandBrake Active  -> Output 3 Low
	// Data[1] == 0x0  -> Off, No HandBrake      -> Output 3 High
	if(pmsg->msg.Data[1] & 0x10){
		handbrake_status = true;
	}else{
		handbrake_status = false;
	}
}

void Sprinter_OBD::_updateIgnition(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[7] == 0x90){
		ignition_status = true;
	}else{
		ignition_status = false;
	}
}

void Sprinter_OBD::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[4];
}

void Sprinter_OBD::outPutController(void)
{
	/* OUTPUT1 */
	if(speedValue <= 0xa)
		ENABLE_AV1_OUTPUT(1);
	else
		ENABLE_AV1_OUTPUT(0);

	/* OUTPUT2 */
	ENABLE_AV2_OUTPUT(reverse_status);

	/* OUTPUT3 */
	ENABLE_AV3_OUTPUT(!handbrake_status);

	/* OUTPUT4 */
	ENABLE_AV4_OUTPUT(ignition_status);

	/* OUTPUT5 */
	if(speedValue <= 0xa){
		ENABLE_AV5_OUTPUT(!handbrake_status);		// Speed <= 10 and HandBrake Off then Output High
	}else{
		ENABLE_AV5_OUTPUT(0);
	}	

}

void Sprinter_OBD::updateSpeedStatus(CanRMsg *pmsg){}
void Sprinter_OBD::updateTVFKeyStatus(CanRMsg *pmsg){}
bool Sprinter_OBD::filt(CanRMsg *pmsg){}



