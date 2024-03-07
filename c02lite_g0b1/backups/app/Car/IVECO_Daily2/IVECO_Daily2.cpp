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

#include "IVECO_Daily2.h"
#include "IVECO_Daily2_common.h"

using namespace iveco_daily2;

IVECO_Daily2::IVECO_Daily2(){
	name = "IVECO_Daily2";
	const static uint32_t idList[] = {
			HANDBRAKE_MSG_ID,
			DOOR_MSG_ID,
			SEATBELT_MSG_ID,
			SPEED_MSG_ID,
			LIGHTS_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;
        
	ignition_status = false;
	handbrake_status = false;
	seatbelt_status = false;
	lights_status = false;
	doors_status = IGNITION_OFF_DOOR_CLOSE;
}

IVECO_Daily2::~IVECO_Daily2() {
}

bool IVECO_Daily2::init(void)
{
	return 0;
}

void IVECO_Daily2::run(CanRMsg *pmsg)
{    
	if(pmsg->msg.IDT == CAN_ID_EXT) {
		switch(pmsg->msg.ExtId) {
		case HANDBRAKE_MSG_ID:
			_updateHandbrake(pmsg);
			_updateHandbrakeDoorCTL(pmsg);
			break;
//		case DOOR_MSG_ID:
//			_updateHandbrakeDoorCTL(pmsg);
//			break;
		case IGNITION_MSG_ID:
			_updateIgnition(pmsg);
			break;
		case SEATBELT_MSG_ID:
			_updateSeatBelt(pmsg);
			break;
		case SPEED_MSG_ID:
			_updateSpeed(pmsg);
			break;
		case LIGHTS_MSG_ID:
			_updateLights(pmsg);
			break;
		default:
			break;
		}
	}

	/*
	 * Set the outputs
	 */
	outPutController();
}

void IVECO_Daily2::_updateIgnition(CanRMsg *pmsg)
{
	if (pmsg->msg.Data[5] & 0x04) {
		ignition_status = ON;
	} else {
		ignition_status = OFF;
	}
}

void IVECO_Daily2::_updateHandbrake(CanRMsg *pmsg)
{   
	handbrake_status = (pmsg->msg.Data[6]&0x20)?ON:OFF;
}

void IVECO_Daily2::_updateHandbrakeDoorCTL(CanRMsg *pmsg)
{
	//	if((pmsg->msg.Data[2]==0x40) || (pmsg->msg.Data[2]==0xC0)){
	if(pmsg->msg.Data[2] & 0x10){
		doors_status= (pmsg->msg.Data[2]&0x08)?IGNITION_OFF_DOOR_OPEN:IGNITION_OFF_DOOR_CLOSE;
	}else if(pmsg->msg.Data[2] & 0x40){
		doors_status= (pmsg->msg.Data[2]&0x08)?IGNITION_ON_DOOR_OPEN:IGNITION_ON_DOOR_CLOSE;
	}
}

void IVECO_Daily2::_updateSeatBelt(CanRMsg *pmsg)
{
	seatbelt_status = (pmsg->msg.Data[3]&0x40)?ON:OFF;
}


void IVECO_Daily2::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[1];
}

void IVECO_Daily2::_updateLights(CanRMsg *pmsg)
{
	lights_status = (pmsg->msg.Data[3]==0x44)?true:false;
}

void IVECO_Daily2::outPutController(void)
{
	// Update HandBrake Status for Ignition is OFF
	handbrake_status = gpDrvDel1->getValue()?ON:OFF;


	/* OUTPUT1 */
	if((seatbelt_status==OFF) && (speedValue>0xa)){
		ENABLE_AV1_OUTPUT(1);
	}else{
		ENABLE_AV1_OUTPUT(0);
	}

	/* OUTPUT2 */
        if(speedValue>0xa){
		ENABLE_AV2_OUTPUT(1);
	}else{
		ENABLE_AV2_OUTPUT(0);
	}

	/* OUTPUT3 */
	/*
	 * High if Ignition is OFF, and Low when Ignition is ON
	 */
	ENABLE_AV3_OUTPUT(!ignition_status);
        
	/* OUTPUT4 */
	if(lights_status){
		if((doors_status==IGNITION_OFF_DOOR_OPEN))
			ENABLE_AV4_OUTPUT(1);
	}else{
		ENABLE_AV4_OUTPUT(0);
	}

	/* OUTPUT5 */
	if(!ignition_status){
		if((handbrake_status==OFF)){
			if(_isDoorOpen())
				ENABLE_AV5_OUTPUT(1);
		}else{
			ENABLE_AV5_OUTPUT(0);
		}
	}else
		ENABLE_AV5_OUTPUT(0);
}

void IVECO_Daily2::updateSpeedStatus(CanRMsg *pmsg)
{
}

void IVECO_Daily2::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool IVECO_Daily2::filt(CanRMsg *pmsg)
{
	return false;
}


