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

#include "IVECO_Daily5.h"
#include "IVECO_Daily5_common.h"

using namespace iveco_daily5;

static unsigned int __en;
static unsigned int __en1;
static u32 startTicks1 = 0;
static u32 startTicks2 = 0;

IVECO_Daily5::IVECO_Daily5(){
	name = "IVECO_Daily5";
	const static uint32_t idList[] = {
          	HANDBRAKE_MSG_ID,
			IGNITION_MSG_ID,
			DOOR_MSG_ID,
			SEATBELT_MSG_ID,
			SPEED_MSG_ID,
			LIGHTS_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;  
    Det1_status = false;
	ignition_status = false;
    ignition_status1 = false; 
	ignition_status2 = false; 
	seatbelt_status = false;
	lights_status = false;
	doors_status = IGNITION_OFF_DOOR_CLOSE;   
	__en = 0;
	__en1 = 0; 
}

IVECO_Daily5::~IVECO_Daily5() {
}

bool IVECO_Daily5::init(void)
{
	return 0;
}

void IVECO_Daily5::run(CanRMsg *pmsg)
{    
	if(pmsg->msg.IDT == CAN_ID_EXT) {
		switch(pmsg->msg.ExtId) {
		case HANDBRAKE_MSG_ID:
			_updateHandbrakeDoorCTL(pmsg);
			break;
		case IGNITION_MSG_ID:
			_updateIgnition(pmsg);
			break;
		case SEATBELT_MSG_ID:
			_updateSeatBelt(pmsg);
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

	/*  Set the outputs  */
        
	outPutController();
	
	if(__en){	
		if(startTicks1 == 0){                      //延时5S打开Output3
			startTicks1 = systick_gettime();
		}
		if((systick_gettime()-startTicks1)>5000){
			__en = 0;
	     	startTicks1 = 0;
			ignition_status1 = ON;
		}
	}
	
	if(__en1){	                                    //延时25S关闭Output3
		if(startTicks2 == 0){
			startTicks2 = systick_gettime();
		}
		if((systick_gettime()-startTicks2)>25000){
			__en1 = 0;
			startTicks2 = 0;
			ignition_status1 = OFF;
		}
	}


}

void IVECO_Daily5::_updateIgnition(CanRMsg *pmsg)
{
    static bool isDelay2Output3 = false;
	static u32 startTicks = 0;

	if(pmsg->msg.Data[5] & 0x04) {
		__en  = 1;
	 	__en1  = 0;
		ignition_status = ON;
		ignition_status2 = ON;
		startTicks = 0;
		isDelay2Output3 = false;
		startTicks2 = 0;
	}else{ 
		ignition_status = OFF;
		if(Det1_status){
 	  		__en  = 0;
 	  		__en1 = 0; 
	     	startTicks1 = 0;
 	  		ignition_status1 = OFF;
		}else {
			__en  = 0;  
		    __en1 = 1; 
	     	startTicks1 = 0;
		}

		if(isDelay2Output3 == false)
			ignition_status2 = OFF;
		
		if(startTicks == 0){
			startTicks = systick_gettime();
		}
		
		if((isDelay2Output3==false) && ((systick_gettime()-startTicks)>2000)){
			printf("isDelay2Output3!\n");
			isDelay2Output3 = true;
			ignition_status2 = ON;
		}
		
		if((systick_gettime()-startTicks)>902000){
			ignition_status2 = OFF;
			}
	}

    }


void IVECO_Daily5::_updateHandbrakeDoorCTL(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[2] & 0x10){
		doors_status= (pmsg->msg.Data[2]&0x08)?IGNITION_OFF_DOOR_OPEN:IGNITION_OFF_DOOR_CLOSE;
	}else if(pmsg->msg.Data[2] & 0x40){
		doors_status= (pmsg->msg.Data[2]&0x08)?IGNITION_ON_DOOR_OPEN:IGNITION_ON_DOOR_CLOSE;
	}else{
		doors_status = 0;
	}
}

void IVECO_Daily5::_updateSeatBelt(CanRMsg *pmsg)
{
	seatbelt_status = (pmsg->msg.Data[3]&0x40)?ON:OFF;
}

void IVECO_Daily5::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[1];
}

void IVECO_Daily5::_updateLights(CanRMsg *pmsg)
{
	lights_status = (pmsg->msg.Data[3]==0x44)?true:false;
}

void IVECO_Daily5::outPutController(void)
{

  	Det1_status = gpDrvDel1->getValue()?ON:OFF;    //Read GPIO status
  
	/* OUTPUT1 */
        
	if((seatbelt_status==OFF) && (speedValue>0xa)){
		ENABLE_AV1_OUTPUT(1);
	}else{
		ENABLE_AV1_OUTPUT(0);
	}

	/* OUTPUT2 */
        
	ENABLE_AV2_OUTPUT(ignition_status2);

	/* OUTPUT3 */
        
	ENABLE_AV3_OUTPUT(ignition_status1);
        
	/* OUTPUT4 */
        
	if(!ignition_status){
		if(lights_status){
			if((doors_status==IGNITION_OFF_DOOR_OPEN)){
			ENABLE_AV4_OUTPUT(1);
            }
		}else{
			ENABLE_AV4_OUTPUT(0);
		}
    }else{
 		    ENABLE_AV4_OUTPUT(0);
    }

		/* OUTPUT5 */	

	if(!ignition_status){     
		if((Det1_status==OFF)){
	        if(_isDoorOpen())
			ENABLE_AV5_OUTPUT(1);
		}else{
			ENABLE_AV5_OUTPUT(0);
		}
	}else{
			ENABLE_AV5_OUTPUT(0);
		}
}

void IVECO_Daily5::updateSpeedStatus(CanRMsg *pmsg)
{
}

void IVECO_Daily5::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool IVECO_Daily5::filt(CanRMsg *pmsg)
{
	return false;
}


