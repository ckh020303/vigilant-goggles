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

#include "VOLVO_FH.h"
#include "VOLVO_FH_common.h"

using namespace vOLVO_FH;

VOLVO_fH::VOLVO_fH(){
	name = "VOLVO_FH";
	const static uint32_t idList[] = {
			INDICATOR_MSG_ID,
			REVERSE_MSG_ID,
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
       
}

VOLVO_fH::~VOLVO_fH() {
}

bool VOLVO_fH::init(void)
{
	return 0;
}

void VOLVO_fH::run(CanRMsg *pmsg)
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

void VOLVO_fH::_updateINDICATOR(CanRMsg *pmsg)
{
        
	static u8 output1ReleaseCnt = 0;
	static u8 output4ReleaseCnt = 0;
	
	if(pmsg->msg.Data[3] == 0x04){   
		output1ReleaseCnt = 0;
		LEFT_INDICATOR_status = true;
	}else{
		if(LEFT_INDICATOR_status && ++output1ReleaseCnt==10){
			output1ReleaseCnt = 0;
			LEFT_INDICATOR_status = false;
		}
	}
	
	if(pmsg->msg.Data[3] == 0x05){  
		output4ReleaseCnt = 0;
		RIGHT_INDICATOR_status = true;
	}else{
		if(RIGHT_INDICATOR_status && ++output4ReleaseCnt==10){
			output4ReleaseCnt = 0;
			RIGHT_INDICATOR_status = false;
		}
	}
 
}

void VOLVO_fH::_updateREVERSE(CanRMsg *pmsg)
{   
	REVERSE_status = (pmsg->msg.Data[0] == 0x11)?ON:OFF;
}

void VOLVO_fH::_updateSPEED(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[7];
}

void VOLVO_fH::_updateHANDBRAKE(CanRMsg *pmsg)
{
        
	HANDBRAKE_status = (pmsg->msg.Data[5] & 0x01)?ON:OFF;

}

void VOLVO_fH::outPutController(void)
{
	/* OUTPUT1 */
  
	ENABLE_AV1_OUTPUT(LEFT_INDICATOR_status);
        
	/* OUTPUT2 */
        
	ENABLE_AV2_OUTPUT(REVERSE_status);

	/* OUTPUT3 */
        
  	if(LEFT_INDICATOR_status && (speedValue<0x15)){
		ENABLE_AV3_OUTPUT(1);
	}else{
		ENABLE_AV3_OUTPUT(0);
		}
        
	/* OUTPUT4 */
        
	ENABLE_AV4_OUTPUT(RIGHT_INDICATOR_status);

	/* OUTPUT5 */
        
        if(!HANDBRAKE_status && (speedValue<0x15)){
		ENABLE_AV5_OUTPUT(1);
	}else{
		ENABLE_AV5_OUTPUT(0);
	}
     
}

void VOLVO_fH::updateSpeedStatus(CanRMsg *pmsg)
{
}

void VOLVO_fH::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool VOLVO_fH::filt(CanRMsg *pmsg)
{
	return false;
}


