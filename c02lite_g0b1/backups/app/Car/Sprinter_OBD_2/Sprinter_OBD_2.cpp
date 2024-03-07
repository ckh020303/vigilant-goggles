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

#include "Sprinter_OBD_2.h"
#include "Sprinter_OBD_2_common.h"

using namespace sprinter_obd_2;

Sprinter_OBD_2::Sprinter_OBD_2(){
	name = "Sprinter_OBD_2";
	const static uint32_t idList[] = {
			DOOR_MSG_ID,
			HANDBRAKE_MSG_ID,
			IGNITION_MSG_ID,
			SEATBELT_MSG_ID,
			SPEED_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;

	ignition_status  = false;
	door_status      = false;
	handbrake_status = false;
	seatbelt_status  = false;
}

Sprinter_OBD_2::~Sprinter_OBD_2() {
}

bool Sprinter_OBD_2::init(void)
{
	return 0;
}

void Sprinter_OBD_2::run(CanRMsg *pmsg)
{
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case IGNITION_MSG_ID:
			_updateIgnition(pmsg);
			break;
		case HANDBRAKE_MSG_ID:
			_updateHandbrake(pmsg);
			break;
		case DOOR_MSG_ID:
			_updateDoor(pmsg);
			break;
		case SPEED_MSG_ID:
			_updateSpeed(pmsg);
			break;
		case SEATBELT_MSG_ID:
			_updateSeatbelt(pmsg);
			break;
		default:
			break;
		}
	}
	outPutController();
}

void Sprinter_OBD_2::_updateSeatbelt(CanRMsg *pmsg)
{

	if(pmsg->msg.Data[7] & 0x10){
		seatbelt_status = true;
	}else{
		seatbelt_status = false;
	}

}

void Sprinter_OBD_2::_updateHandbrake(CanRMsg *pmsg)
{

	if(pmsg->msg.Data[2] & 0x03){
		handbrake_status = true;
	}else{
		handbrake_status = false;
	}
	
}

void Sprinter_OBD_2::_updateIgnition(CanRMsg *pmsg)
{

	if(pmsg->msg.Data[6] == 0x10){
		ignition_status = true;
	}else{
		ignition_status = false;
	}
}

void Sprinter_OBD_2::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[4];
}

void Sprinter_OBD_2::_updateDoor(CanRMsg *pmsg)
{

	if(pmsg->msg.Data[1] == 0xF3){
		door_status = true;
	}else{
		door_status = false;
	}

}

void Sprinter_OBD_2::outPutController(void)
{
	/* OUTPUT1 */
	if(ignition_status){
		
		ENABLE_AV1_OUTPUT(!handbrake_status && door_status);

	}else{

	   if(handbrake_status){
		
	    ENABLE_AV1_OUTPUT(0);
	
	   }else{
	   
	    ENABLE_AV1_OUTPUT(1);
		
	   }
	}


	/* OUTPUT2 */		
	ENABLE_AV2_OUTPUT(0);

	/* OUTPUT3 */
	if(ignition_status){
		
		ENABLE_AV3_OUTPUT(1);

	}else{

	   if(handbrake_status){
		
	    ENABLE_AV3_OUTPUT(0);
	
	   }else{
	   
	    ENABLE_AV3_OUTPUT(1);
		
	   }
	}

	/* OUTPUT4 */
	if((speedValue >= 0x05) && seatbelt_status){
		
	ENABLE_AV4_OUTPUT(1);
	
	}else{
	
	ENABLE_AV4_OUTPUT(0);
	
	}
	
	/* OUTPUT5 */	
	ENABLE_AV5_OUTPUT(!handbrake_status);
	

}

void Sprinter_OBD_2::updateSpeedStatus(CanRMsg *pmsg){}
void Sprinter_OBD_2::updateTVFKeyStatus(CanRMsg *pmsg){}
bool Sprinter_OBD_2::filt(CanRMsg *pmsg){}



