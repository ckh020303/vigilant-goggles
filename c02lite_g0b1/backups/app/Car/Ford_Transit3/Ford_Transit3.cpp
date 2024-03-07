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

#include "Ford_Transit3.h"
#include "Ford_Transit3_common.h"

using namespace Ford_transit3;

Ford_Transit3::Ford_Transit3(){
	name = "Ford_Transit3";
	const static uint32_t idList[] = {
			HANDBRAKE_MSG_ID,
			FOOTBRAKE_MSG_ID,
			SPEED_MSG_ID,
			LIGHTS_MSG_ID,
            IGNITION_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;

	ignition_status  = false;
	reverse_status   = false;
	footbrake_status = false;
    Indicator_l_status = false;
    Indicator_R_status = false;
    handbrake_status = false;
}

Ford_Transit3::~Ford_Transit3() {
}

bool Ford_Transit3::init(void)
{
	return 0;
}

void Ford_Transit3::run(CanRMsg *pmsg)
{    
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case HANDBRAKE_MSG_ID:
			_updateHandbrake(pmsg);
			break;
		case FOOTBRAKE_MSG_ID:
			_updateFootbrake(pmsg);
			break;
		case IGNITION_MSG_ID:
			_updateIgnition(pmsg);
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
    if(ignition_status){
	  outPutController();                 
	}else{
	  ENABLE_AV1_OUTPUT(0);
          ENABLE_AV2_OUTPUT(0);
          ENABLE_AV3_OUTPUT(0);
          ENABLE_AV4_OUTPUT(0);
          ENABLE_AV5_OUTPUT(0);
	}

}

void Ford_Transit3::_updateIgnition(CanRMsg *pmsg)
{
	ignition_status = (pmsg->msg.Data[0]&0x20)?true:false;
}

void Ford_Transit3::_updateHandbrake(CanRMsg *pmsg)
{   
    handbrake_status = (pmsg->msg.Data[2]&0x10)?true:false;
}

void Ford_Transit3::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[0];
}

void Ford_Transit3::_updateFootbrake(CanRMsg *pmsg)
{
    footbrake_status = (pmsg->msg.Data[0]&0xb)?true:false;
}

void Ford_Transit3::_updateLights(CanRMsg *pmsg)
{
    static u8 output1ReleaseCnt = 0;
	static u8 output4ReleaseCnt = 0;
	
	if(pmsg->msg.Data[0]==0x10){
		output1ReleaseCnt = 0;
		Indicator_l_status = true;                   
	}else{
		if(Indicator_l_status && ++output1ReleaseCnt==30){
			output1ReleaseCnt = 0;
			Indicator_l_status = false;              
		}
	}
     
	if(pmsg->msg.Data[0]==0x20){
		output4ReleaseCnt = 0;
		Indicator_R_status = true;               
	}else{
		if(Indicator_R_status && ++output4ReleaseCnt==30){
			output4ReleaseCnt = 0;
			Indicator_R_status = false;
		}
	}

}

void Ford_Transit3::outPutController(void)
{

	/* OUTPUT1 */
	ENABLE_AV1_OUTPUT(Indicator_l_status);        

	/* OUTPUT2 */
	ENABLE_AV2_OUTPUT(footbrake_status);

	/* OUTPUT3 */
	ENABLE_AV3_OUTPUT(ignition_status);

	/* OUTPUT4 */
    ENABLE_AV4_OUTPUT(Indicator_R_status);

	/* OUTPUT5 */
	if((speedValue<=0x5) && !handbrake_status){
		ENABLE_AV5_OUTPUT(1);		
	}else{
		ENABLE_AV5_OUTPUT(0);
	}
     
}

void Ford_Transit3::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Ford_Transit3::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Ford_Transit3::filt(CanRMsg *pmsg)
{
	return false;
}


