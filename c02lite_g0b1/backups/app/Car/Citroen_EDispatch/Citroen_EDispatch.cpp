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

#include "Citroen_EDispatch.h"
#include "Citroen_EDispatch_common.h"

using namespace Citroen_eDispatch;

Citroen_EDispatch::Citroen_EDispatch(){
	name = "Citroen_EDispatch";
	const static uint32_t idList[] = {
			HANDBRAKE_MSG_ID,
			REVERSE_MSG_ID,
			SPEED_MSG_ID,
			LIGHTS_MSG_ID,
                        IGNITION_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;

	ignition_status  = false;
	reverse_status   = false;
	REVERSE_status = false;
    Indicator_l_status = false;
    Indicator_R_status = false;
    handbrake_status = false;
}

Citroen_EDispatch::~Citroen_EDispatch() {
}

bool Citroen_EDispatch::init(void)
{
	return 0;
}

void Citroen_EDispatch::run(CanRMsg *pmsg)
{    
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case HANDBRAKE_MSG_ID:
			_updateHandbrake(pmsg);
			break;
		case REVERSE_MSG_ID:
			_updateREVERSE(pmsg);
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

void Citroen_EDispatch::_updateIgnition(CanRMsg *pmsg)
{
	//ignition_status = (pmsg->msg.Data[0]&0x80)?true:false;
        if(pmsg->msg.Data[0] == 0x80){
		ignition_status = true;
	}else{
		ignition_status = false;
	}
}

void Citroen_EDispatch::_updateHandbrake(CanRMsg *pmsg)
{   
    //handbrake_status = (pmsg->msg.Data[0]&0x5F)?true:false;
        if(pmsg->msg.Data[0] == 0x5F){
		handbrake_status = true;
	}else{ 
		handbrake_status = false;
	}
}

void Citroen_EDispatch::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[0];
}

void Citroen_EDispatch::_updateREVERSE(CanRMsg *pmsg)
{
    //REVERSE_status = (pmsg->msg.Data[3]&0x01)?true:false;
        if(pmsg->msg.Data[3] == 0x01){
		REVERSE_status = true;
	}else{ 
		REVERSE_status = false;
	}
}

void Citroen_EDispatch::_updateLights(CanRMsg *pmsg)
{
//    static u8 output1ReleaseCnt = 0;
//	static u8 output4ReleaseCnt = 0;
	
	if((pmsg->msg.Data[1]==0x60) || (pmsg->msg.Data[1]==0x62)){
//		output1ReleaseCnt = 0;
		Indicator_l_status = true;                   
	}else{
//		if(Indicator_l_status && ++output1ReleaseCnt==30){
//			output1ReleaseCnt = 0;
			Indicator_l_status = false;              
		}
	
     
	if((pmsg->msg.Data[1]==0xA0) || (pmsg->msg.Data[1]==0xA2)){
//		output4ReleaseCnt = 0;
		Indicator_R_status = true;               
	}else{
//		if(Indicator_R_status && ++output4ReleaseCnt==30){
//			output4ReleaseCnt = 0;
			Indicator_R_status = false;
		}
	
}

void Citroen_EDispatch::outPutController(void)
{

	/* OUTPUT1 */
	ENABLE_AV1_OUTPUT(Indicator_l_status);        

	/* OUTPUT2 */
	ENABLE_AV2_OUTPUT(REVERSE_status);

	/* OUTPUT3 */
	ENABLE_AV3_OUTPUT(ignition_status);

	/* OUTPUT4 */
	ENABLE_AV4_OUTPUT(Indicator_R_status);

	/* OUTPUT5 */
	if((speedValue<=0x05) && !handbrake_status){
		ENABLE_AV5_OUTPUT(1);		
	}else{
		ENABLE_AV5_OUTPUT(0);
	}
     
}

void Citroen_EDispatch::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Citroen_EDispatch::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Citroen_EDispatch::filt(CanRMsg *pmsg)
{
	return false;
}


