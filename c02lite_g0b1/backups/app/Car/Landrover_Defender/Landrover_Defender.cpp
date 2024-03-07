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

#include "Landrover_Defender.h"
#include "Landrover_Defender_common.h"

using namespace landrover_Defender;

Landrover_defender::Landrover_defender(){
	name = "Landrover_Defender";
	const static uint32_t idList[] = {
			INDICATOR_MSG_ID,
			REVERSE_MSG_ID,
                        IGNITION_MSG_ID,
			SPEED_MSG_ID,
			HANDBRAKE_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;

	LEFT_INDICATOR_status = false;
	RIGHT_INDICATOR_status = false;
	REVERSE_status = false;
	HANDBRAKE_status = false;
        IGNITION_status = false;
       
}

Landrover_defender::~Landrover_defender() {
}

bool Landrover_defender::init(void)
{
	return 0;
}

void Landrover_defender::run(CanRMsg *pmsg)
{    
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case INDICATOR_MSG_ID:
			_updateINDICATOR(pmsg);
			break;
		case REVERSE_MSG_ID:
			_updateREVERSE(pmsg);
			break;
		case SPEED_MSG_ID:
			_updateSPEED(pmsg);
			break;
		case IGNITION_MSG_ID:
			_updateIGNITION(pmsg);
			break;
		case HANDBRAKE_MSG_ID:
			_updateHANDBRAKE(pmsg);
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

void Landrover_defender::_updateINDICATOR(CanRMsg *pmsg)
{
  
        LEFT_INDICATOR_status  = (pmsg->msg.Data[5]&0x06)?ON:OFF;
        RIGHT_INDICATOR_status  = (pmsg->msg.Data[5]&0x07)?ON:OFF;
 
}

void Landrover_defender::_updateREVERSE(CanRMsg *pmsg)
{   
	REVERSE_status = (pmsg->msg.Data[4] == 1)?ON:OFF;
}

void Landrover_defender::_updateIGNITION(CanRMsg *pmsg)
{   
  
  	if((pmsg->msg.Data[7]==0x08) || (pmsg->msg.Data[7]==0x09)){
		IGNITION_status = ON;
	}else{
		IGNITION_status = OFF;
		}
 
}

void Landrover_defender::_updateSPEED(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[4];
}

void Landrover_defender::_updateHANDBRAKE(CanRMsg *pmsg)
{
	HANDBRAKE_status = (pmsg->msg.Data[4]&0x03)?ON:OFF;
}

void Landrover_defender::outPutController(void)
{
	/* OUTPUT1 */
  
	ENABLE_AV1_OUTPUT(LEFT_INDICATOR_status);
        
	/* OUTPUT2 */
        
	ENABLE_AV2_OUTPUT(REVERSE_status);

	/* OUTPUT3 */
        
	ENABLE_AV3_OUTPUT(IGNITION_status);
        
	/* OUTPUT4 */
        
	ENABLE_AV4_OUTPUT(RIGHT_INDICATOR_status);

	/* OUTPUT5 */
        
        if(!HANDBRAKE_status && (speedValue<0x5)){
		ENABLE_AV5_OUTPUT(1);
	}else{
		ENABLE_AV5_OUTPUT(0);
	}
     
}

void Landrover_defender::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Landrover_defender::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Landrover_defender::filt(CanRMsg *pmsg)
{
	return false;
}


