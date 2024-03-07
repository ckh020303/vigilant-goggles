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

#include "IVECO_Daily4.h"
#include "IVECO_Daily4_common.h"

using namespace iveco_daily4;

static unsigned long __tick;
static unsigned int __en;
static unsigned long __tick1;
static unsigned int __en1;

IVECO_Daily4::IVECO_Daily4(){
	name = "IVECO_Daily4";
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
	seatbelt_status = false;
	lights_status = false;
	doors_status = IGNITION_OFF_DOOR_CLOSE;
        
	__tick = 0;
	__en = 0;
	__tick1 = 0;
	__en1 = 0;        
}

IVECO_Daily4::~IVECO_Daily4() {
}

bool IVECO_Daily4::init(void)
{
	return 0;
}

void IVECO_Daily4::run(CanRMsg *pmsg)
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
		__tick++;	
		if(__tick >= 60){          //4S 
			__en = 0;
			__tick = 0;
			ignition_status1 = ON;
		}
	        }
	if(__en1){
		__tick1++;	
		if(__tick1 >= 6000){          //25S 
			__en1 = 0;
			__tick1 = 0;
			ignition_status1 = OFF;
		}
          }
}

void IVECO_Daily4::_updateIgnition(CanRMsg *pmsg)
{
        
	if (pmsg->msg.Data[5] & 0x04) 
               {
		 __en  = 1;
	 	__en1  = 0;
	        __tick1 = 0;
	        __en1 = 0; 
	        ignition_status = ON;
	        } 
        else{ 
          ignition_status = OFF;
          if(Det1_status)
               {
                __en  = 0;
	        __tick = 0;
	        __en = 0;  
		ignition_status1 = OFF;
	        }
          else {
                __en  = 0;
	        __tick = 0;
	        __en = 0;  
		__en1  = 1; 
                if(Det1_status)
               {
                 __en  = 0;
	         __tick = 0;
	         __en = 0;   
		ignition_status1 = OFF;
	        }
	        }
        }
}

void IVECO_Daily4::_updateHandbrakeDoorCTL(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[2] & 0x10){
		doors_status= (pmsg->msg.Data[2]&0x08)?IGNITION_OFF_DOOR_OPEN:IGNITION_OFF_DOOR_CLOSE;
	}else if(pmsg->msg.Data[2] & 0x40){
		doors_status= (pmsg->msg.Data[2]&0x08)?IGNITION_ON_DOOR_OPEN:IGNITION_ON_DOOR_CLOSE;
	}else{
		doors_status = 0;
	}
}

void IVECO_Daily4::_updateSeatBelt(CanRMsg *pmsg)
{
	seatbelt_status = (pmsg->msg.Data[3]&0x40)?ON:OFF;
}

void IVECO_Daily4::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[1];
}

void IVECO_Daily4::_updateLights(CanRMsg *pmsg)
{
	lights_status = (pmsg->msg.Data[3]==0x44)?true:false;
}

void IVECO_Daily4::outPutController(void)
{

  	Det1_status = gpDrvDel1->getValue()?ON:OFF;    //Read GPIO status
  
	/* OUTPUT1 */
        
	if((seatbelt_status==OFF) && (speedValue>0xa)){
		ENABLE_AV1_OUTPUT(1);
	}else{
		ENABLE_AV1_OUTPUT(0);
	}

	/* OUTPUT2 */
        
        if(speedValue>0xa){
		ENABLE_AV2_OUTPUT(1);
	}else{
		ENABLE_AV2_OUTPUT(0);
	}

	/* OUTPUT3 */
        
	ENABLE_AV3_OUTPUT(ignition_status1);
        
	/* OUTPUT4 */
        
	if(lights_status){
		if((doors_status==IGNITION_OFF_DOOR_OPEN))
		ENABLE_AV4_OUTPUT(1);
	}else{
		ENABLE_AV4_OUTPUT(0);
	}

	/* OUTPUT5 */
        
	if((Det1_status==OFF)){
        if(_isDoorOpen())
		ENABLE_AV5_OUTPUT(1);
	}else{
		ENABLE_AV5_OUTPUT(0);
	}
	
}

void IVECO_Daily4::updateSpeedStatus(CanRMsg *pmsg)
{
}

void IVECO_Daily4::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool IVECO_Daily4::filt(CanRMsg *pmsg)
{
	return false;
}


