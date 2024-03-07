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

#include "LDV_Maxus_2021_2.h"
#include "LDV_Maxus_2021_2_common.h"

using namespace LDV_maxus_2021_2;

LDV_Maxus_2021_2::LDV_Maxus_2021_2(){
    name = "LDV_Maxus_2021_2";
    const static uint32_t idList[] = {
    	SPEED_MAG_ID,
		HANDBRAKE_MAG_ID,
		IGNITION_MAG_ID,
		_ID,
		0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;
	handbrake_status = false;
	ignition_status = false;
	_status = false;	
}

LDV_Maxus_2021_2::~LDV_Maxus_2021_2() {
}

bool LDV_Maxus_2021_2::init(void)
{
	return 0;
}

void LDV_Maxus_2021_2::run(CanRMsg *pmsg)
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
			case _ID:
				_A_status(pmsg);
				break;
		}
    } 
	outPutController();   
}

void LDV_Maxus_2021_2::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[4];
}

void LDV_Maxus_2021_2::_A_status(CanRMsg *pmsg)
{
	_status = pmsg->msg.Data[5] == 0x04?ON:OFF;
}

void LDV_Maxus_2021_2::_updateHandbrake(CanRMsg *pmsg)
{
	handbrake_status = pmsg->msg.Data[6]&0x20?ON:OFF;
}

void LDV_Maxus_2021_2::_updateIgnitionstatus(CanRMsg *pmsg)
{
	//ignition_status = pmsg->msg.Data[2]&0x12?ON:OFF;
	//ignition_status = (pmsg->msg.Data[3]==0x26)?ON:OFF;
        
    /*    if((pmsg->msg.Data[3] == 0x26) || (pmsg->msg.Data[3] == 0x16)){
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

void LDV_Maxus_2021_2::outPutController(void)
{
	/* OUTPUT1 */
	if(speedValue <= 0xa)
		ENABLE_AV1_OUTPUT(ignition_status && !handbrake_status);
	else
		ENABLE_AV1_OUTPUT(0);
	
	/* OUTPUT2 */	
	    ENABLE_AV2_OUTPUT(0);

	/* OUTPUT3 */
	if(speedValue >= 0xa)
		ENABLE_AV3_OUTPUT(ignition_status);
	else
		ENABLE_AV3_OUTPUT(0);
		
	/* OUTPUT4 */
	if(ignition_status)
		ENABLE_AV4_OUTPUT(_status);
	else
		ENABLE_AV4_OUTPUT(0);


	/* OUTPUT5 */
	if(speedValue <= 0xa)
		ENABLE_AV5_OUTPUT(ignition_status);
	else
		ENABLE_AV5_OUTPUT(0);
	
}


void LDV_Maxus_2021_2::updateSpeedStatus(CanRMsg *pmsg)
{
}

void LDV_Maxus_2021_2::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool LDV_Maxus_2021_2::filt(CanRMsg *pmsg)
{
}


