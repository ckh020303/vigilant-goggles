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

#include "LDV_Maxus_2021_3.h"
#include "LDV_Maxus_2021_3_common.h"

using namespace LDV_maxus_2021_3;

LDV_Maxus_2021_3::LDV_Maxus_2021_3(){
    name = "LDV_Maxus_2021_3";
    const static uint32_t idList[] = {
    	        SPEED_MAG_ID,
		HANDBRAKE_MAG_ID,
		IGNITION_MAG_ID,
		REVERSE_MAG_ID,
		INDICATOR_MAG_ID,
		0x0,
	};
	pCanFilterIDList = idList;
        baudRate = 500;
	handbrake_status = false;
	ignition_status = false;
        output1 = false;
	output2 = false;
	output4 = false;
}

LDV_Maxus_2021_3::~LDV_Maxus_2021_3() {
}

bool LDV_Maxus_2021_3::init(void)
{
	return 0;
}

void LDV_Maxus_2021_3::run(CanRMsg *pmsg)
{
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
			case SPEED_MAG_ID:
				_updateSpeed(pmsg);
				break;	
			case HANDBRAKE_MAG_ID:
				_updateHandbrake(pmsg);
				break;
			case IGNITION_MAG_ID:
				_updateIgnitionstatus(pmsg);
				break;
			case REVERSE_MAG_ID:
				_updateReversestatus(pmsg);
				break;
			case INDICATOR_MAG_ID:
				_updateIndicatorstatus(pmsg);
				break;
		}
    } 
	outPutController();   
}

void LDV_Maxus_2021_3::_updateIndicatorstatus(CanRMsg *pmsg)
{
	static u8 output1ReleaseCnt = 0;
	static u8 output4ReleaseCnt = 0;
	
	if((pmsg->msg.Data[0]==0x08) || (pmsg->msg.Data[0]==0x28)){   //ID 641 B1=8/40
		output1ReleaseCnt = 0;
		output1 = true;
	}else{
		if(output1 && ++output1ReleaseCnt==30){
			output1ReleaseCnt = 0;
			output1 = false;
		}
	}
	
	if((pmsg->msg.Data[0]==0x10) || (pmsg->msg.Data[0] == 0x30)){  //ID 641 B1=16/48
		output4ReleaseCnt = 0;
		output4 = true;
	}else{
		if(output4 && ++output4ReleaseCnt==30){
			output4ReleaseCnt = 0;
			output4 = false;
		}
	}
}

void LDV_Maxus_2021_3::_updateReversestatus(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[3] == 0x02){  
		output2 = true;
	}else{
                output2 = false;
		}
}

void LDV_Maxus_2021_3::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[4];
}

void LDV_Maxus_2021_3::_updateHandbrake(CanRMsg *pmsg)
{
	//handbrake_status = pmsg->msg.Data[4]&0x20?ON:OFF;
          if(pmsg->msg.Data[0] == 0x80){  
		handbrake_status = true;
	}else{
                handbrake_status = false;
		}
}

void LDV_Maxus_2021_3::_updateIgnitionstatus(CanRMsg *pmsg)
{ 
       // ignition_status = pmsg->msg.Data[2]&0x5b?ON:OFF;
        if(pmsg->msg.Data[4] == 0x5f){  
		ignition_status = true;
	}else{
                ignition_status = false;
		}
}

void LDV_Maxus_2021_3::outPutController(void)
{
	/* OUTPUT1 */
        ENABLE_AV1_OUTPUT(output1);
        
	/* OUTPUT2 */
        ENABLE_AV2_OUTPUT(output2);
        
	/* OUTPUT3 */
         
        if(speedValue <= 0xf)
	ENABLE_AV3_OUTPUT(ignition_status && !handbrake_status);
	else
	ENABLE_AV3_OUTPUT(0);
        
//        if(handbrake_status){
//	        ENABLE_AV3_OUTPUT(0);
//	}else{
//          if(speedValue <= 0xf){
//	        ENABLE_AV3_OUTPUT(1);
//		}
//	}
        
	/* OUTPUT4 */
        ENABLE_AV4_OUTPUT(output4);
	/* OUTPUT5 */
        ENABLE_AV5_OUTPUT(ignition_status); 
}


void LDV_Maxus_2021_3::updateSpeedStatus(CanRMsg *pmsg)
{
}

void LDV_Maxus_2021_3::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool LDV_Maxus_2021_3::filt(CanRMsg *pmsg)
{
}


