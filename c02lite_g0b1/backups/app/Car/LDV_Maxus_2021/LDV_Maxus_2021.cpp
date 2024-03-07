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

#include "LDV_Maxus_2021.h"
#include "LDV_Maxus_2021_common.h"

using namespace LDV_maxus_2021;

LDV_Maxus_2021::LDV_Maxus_2021(){
    name = "LDV_Maxus_2021";
    const static uint32_t idList[] = {
		INDICATOR_MAG_ID,
		IGNITION_MAG_ID,
		FOOTBRAKE_MAG_ID,
		
		0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;
	ignition_status = false ;
	footbreak_status = false;
	output1 = false;
	output2 = false;
	output4 = false;
}

LDV_Maxus_2021::~LDV_Maxus_2021() {
}

bool LDV_Maxus_2021::init(void)
{
	return 0;
}

void LDV_Maxus_2021::run(CanRMsg *pmsg)
{
	if(pmsg->msg.IDT == CAN_ID_STD) {
			switch(pmsg->msg.StdId) {
				case INDICATOR_MAG_ID:
					_updateIndicatorstatus(pmsg);
					break;	
				case IGNITION_MAG_ID:
					_updateIgnitionstatus(pmsg);
					break;
				case FOOTBRAKE_MAG_ID:
					_updateFootbrakestatus(pmsg);	
					break;
			}
    } 
	outPutController();   
}

void LDV_Maxus_2021::_updateIndicatorstatus(CanRMsg *pmsg)
{
	static u8 output1ReleaseCnt = 0;
	static u8 output4ReleaseCnt = 0;
	
	if((pmsg->msg.Data[0]==0xa) || (pmsg->msg.Data[0]==0x2a)){
		output1ReleaseCnt = 0;
		output1 = true;
	}else{
		if(output1 && ++output1ReleaseCnt==30){
			output1ReleaseCnt = 0;
			output1 = false;
		}
	}
        
	if(pmsg->msg.Data[0] != 0x2a && pmsg->msg.Data[0] != 0x32){ //output2--忽略变换成0x2a和0x32
		if((pmsg->msg.Data[0] == 0x22) || (pmsg->msg.Data[0] == 0x3a))
			output2 = true;
		else
			output2 = false;
	}   
        
	
	if((pmsg->msg.Data[0]==0x12) || (pmsg->msg.Data[0] == 0x32)){
		output4ReleaseCnt = 0;
		output4 = true;
	}else{
		if(output4 && ++output4ReleaseCnt==30){
			output4ReleaseCnt = 0;
			output4 = false;
		}
	}
	
}

void LDV_Maxus_2021::_updateIgnitionstatus(CanRMsg *pmsg)
{
	//ignition_status = pmsg->msg.Data[2]&0x12?ON:OFF;
	//ignition_status = (pmsg->msg.Data[3]==0x26)?ON:OFF;  
        
	/*if((pmsg->msg.Data[3] == 0x26) || (pmsg->msg.Data[3] == 0x16)){
		ignition_status = ON;
	}else{
		ignition_status = OFF;
	}*/

	if(pmsg->msg.Data[3] == 0x36){
		ignition_status = ON;
	}else{
		ignition_status = OFF;
	}

}

void LDV_Maxus_2021::_updateFootbrakestatus(CanRMsg *pmsg)
{
	if((pmsg->msg.Data[2] >= 0x17) && (pmsg->msg.Data[2] <= 0x64))
	//if((pmsg->msg.Data[2] >= 0x16) && (pmsg->msg.Data[2] <= 0x64))
		footbreak_status = ON;
	else
		footbreak_status = OFF;
}

void LDV_Maxus_2021::outPutController(void)
{
	/* OUTPUT1 */
	ENABLE_AV1_OUTPUT(output1);

	/* OUTPUT2 */
	ENABLE_AV2_OUTPUT(output2);
	
	/* OUTPUT3 */
	ENABLE_AV3_OUTPUT(ignition_status && footbreak_status);
		
	/* OUTPUT4 */
	ENABLE_AV4_OUTPUT(output4);

	/* OUTPUT5 */
	ENABLE_AV5_OUTPUT(ignition_status);

}


void LDV_Maxus_2021::updateSpeedStatus(CanRMsg *pmsg)
{
}

void LDV_Maxus_2021::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool LDV_Maxus_2021::filt(CanRMsg *pmsg)
{
}



