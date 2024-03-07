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

#include "LDV_Maxus_2021_5.h"
#include "LDV_Maxus_2021_5_common.h"

using namespace LDV_maxus_2021_5;

LDV_Maxus_2021_5::LDV_Maxus_2021_5(){
    name = "LDV_Maxus_2021_5";
    const static uint32_t idList[] = {
    	SPEED_MAG_ID,
		HANDBRAKE_MAG_ID,
		INDICATOR_MAG_ID,
		IGNITION_MAG_ID,
		_ID,
		0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;
	ignition_status = false ;
	output1 = false;
	output2 = false;
	output4 = false;
	_status = false;
	handbrake_status = false;
	ignition_status = false;

}

LDV_Maxus_2021_5::~LDV_Maxus_2021_5() {
}

bool LDV_Maxus_2021_5::init(void)
{
	return 0;
}

void LDV_Maxus_2021_5::run(CanRMsg *pmsg)
{
	if(pmsg->msg.IDT == CAN_ID_STD) {
			switch(pmsg->msg.StdId) {
				case SPEED_MAG_ID:
					_updateSpeed(pmsg);
					break;	
				case HANDBRAKE_MAG_ID:
					_updateHandbrake(pmsg);
					break;
				case INDICATOR_MAG_ID:
					_updateIndicatorstatus(pmsg);
					break;	
				case IGNITION_MAG_ID:
					_updateIgnitionstatus(pmsg);
					break;
				case _ID:
					_A_status(pmsg);
					break;
			}
    } 
	outPutController();   
}

void LDV_Maxus_2021_5::_updateIndicatorstatus(CanRMsg *pmsg)
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

void LDV_Maxus_2021_5::_updateIgnitionstatus(CanRMsg *pmsg)
{       
	if((pmsg->msg.Data[3] == 0x26) || (pmsg->msg.Data[3] == 0x16)){
		ignition_status = ON;
	}else{
		ignition_status = OFF;
	}
}

void LDV_Maxus_2021_5::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[4];
}

void LDV_Maxus_2021_5::_A_status(CanRMsg *pmsg)
{
	_status = pmsg->msg.Data[7]&0x20?ON:OFF;
}

void LDV_Maxus_2021_5::_updateHandbrake(CanRMsg *pmsg)
{
	handbrake_status = pmsg->msg.Data[6]&0x20?ON:OFF;
}

void LDV_Maxus_2021_5::outPutController(void)
{
	/* OUTPUT1 */
	ENABLE_AV1_OUTPUT(output1);

	/* OUTPUT2 */
	ENABLE_AV2_OUTPUT(output2);
	
	/* OUTPUT3 */
	if(speedValue <= 0xa)
		ENABLE_AV3_OUTPUT(ignition_status && !handbrake_status);
	else
		ENABLE_AV3_OUTPUT(0);
		
	/* OUTPUT4 */
	ENABLE_AV4_OUTPUT(output4);

	/* OUTPUT5 */
	if(ignition_status)
		ENABLE_AV5_OUTPUT(_status);
	else
		ENABLE_AV5_OUTPUT(0);


}


void LDV_Maxus_2021_5::updateSpeedStatus(CanRMsg *pmsg)
{
}

void LDV_Maxus_2021_5::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool LDV_Maxus_2021_5::filt(CanRMsg *pmsg)
{
}



