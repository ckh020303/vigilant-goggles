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

#include "DAF_1.h"
#include "DAF_1_common.h"

using namespace daf_1;

Daf_1::Daf_1(){
	name = "DAF_LF";
	const static uint32_t idList[] = {
			INDICATOR_MSG_ID,
			REVERSE_MSG_ID,
			SPEED_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 250;
	LEFT_INDICATOR_status = false;
	RIGHT_INDICATOR_status = false;
	REVERSE_status = false;
	Det1_status = false;
}

Daf_1::~Daf_1() {
}

bool Daf_1::init(void)
{
	return 0;
}

void Daf_1::run(CanRMsg *pmsg)
{    
	if(pmsg->msg.IDT == CAN_ID_EXT) {
		switch(pmsg->msg.ExtId) {
		case INDICATOR_MSG_ID:
			_updateINDICATOR(pmsg);
			break;
		case REVERSE_MSG_ID:
			_updateREVERSE(pmsg);
			break;
		case SPEED_MSG_ID:
			_updateSPEED(pmsg);
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

void Daf_1::_updateINDICATOR(CanRMsg *pmsg)
{
  
   LEFT_INDICATOR_status   = (pmsg->msg.Data[1]&0x01)?ON:OFF;
   RIGHT_INDICATOR_status  = (pmsg->msg.Data[1]&0x02)?ON:OFF;

}

void Daf_1::_updateREVERSE(CanRMsg *pmsg)
{
  
   REVERSE_status = ((pmsg->msg.Data[0]&0xF) == 0x0C)?ON:OFF;
        
}

void Daf_1::_updateSPEED(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[5];
}

void Daf_1::outPutController(void)
{

  	Det1_status = gpDrvDel1->getValue()?ON:OFF;    //Read GPIO status

	/* OUTPUT1 */
	ENABLE_AV1_OUTPUT(LEFT_INDICATOR_status);
        
	/* OUTPUT2 */   
	ENABLE_AV2_OUTPUT(REVERSE_status);

	/* OUTPUT3 */   
    if(LEFT_INDICATOR_status && (speedValue <= 0xF)){
		ENABLE_AV3_OUTPUT(1);
	}else{
		ENABLE_AV3_OUTPUT(0);
	}
        
	/* OUTPUT4 */  
    if(Det1_status && (speedValue >= 0x5)){
		ENABLE_AV4_OUTPUT(1);
	}else{
		ENABLE_AV4_OUTPUT(0);
	}

	/* OUTPUT5 */    
	ENABLE_AV5_OUTPUT(RIGHT_INDICATOR_status);
        
}

void Daf_1::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Daf_1::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Daf_1::filt(CanRMsg *pmsg)
{
	return false;
}


