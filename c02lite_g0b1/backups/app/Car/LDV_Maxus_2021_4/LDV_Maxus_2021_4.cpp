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

#include "LDV_Maxus_2021_4.h"
#include "LDV_Maxus_2021_4_common.h"

using namespace LDV_maxus_2021_4;

LDV_Maxus_2021_4::LDV_Maxus_2021_4(){
    name = "LDV_Maxus_2021_4";
    const static uint32_t idList[] = {
    	        SPEED_MAG_ID,
		HANDBRAKE_MAG_ID,
		IGNITION_MAG_ID,
                LIGHTS_MAG_ID,
                DRIVERSDOOR_MAG_ID,
                SEATBELT_MAG_ID,
		
		0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;
	handbrake_status = false;
	ignition_status = false;
        seatbelt_status = false;
        Lightstatus = false;
        Driversdoorstatus = false;
}

LDV_Maxus_2021_4::~LDV_Maxus_2021_4() {
}

bool LDV_Maxus_2021_4::init(void)
{
	return 0;
}

void LDV_Maxus_2021_4::run(CanRMsg *pmsg)
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
			case LIGHTS_MAG_ID:
				_updateLightstatus(pmsg);
				break;
                        case DRIVERSDOOR_MAG_ID:
				_updateDriversdoorstatus(pmsg);
				break;    
                        case SEATBELT_MAG_ID:
				_updateseatbelt(pmsg);
				break;  
		}
    } 
	outPutController();   
}

void LDV_Maxus_2021_4::_updateDriversdoorstatus(CanRMsg *pmsg)
{
	if((pmsg->msg.Data[0]==0x01) || (pmsg->msg.Data[0]==0x09)){   
		 Driversdoorstatus = true;
	}else{
	         Driversdoorstatus = false;
		}
}

void LDV_Maxus_2021_4::_updateLightstatus(CanRMsg *pmsg)
{
	if((pmsg->msg.Data[0]==0x01) || (pmsg->msg.Data[0]==0x03)){   
		 Lightstatus = true;
	}else{
	         Lightstatus = false;
		}
}

void LDV_Maxus_2021_4::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[4];
}

void LDV_Maxus_2021_4::_updateseatbelt(CanRMsg *pmsg)
{
	//seatbelt_status = pmsg->msg.Data[1]&0x00?ON:OFF;
        if(pmsg->msg.Data[1] == 0x10){  
		seatbelt_status = true;
	}else{
                seatbelt_status = false;
		}
}

void LDV_Maxus_2021_4::_updateHandbrake(CanRMsg *pmsg)
{
	//handbrake_status = pmsg->msg.Data[0]&0x00?ON:OFF;
        if(pmsg->msg.Data[0] == 0){  
		handbrake_status = true;
	}else{
                handbrake_status = false;
		}
}

void LDV_Maxus_2021_4::_updateIgnitionstatus(CanRMsg *pmsg)
{
	//ignition_status  = pmsg->msg.Data[0]&0x02?ON:OFF;
  	if(pmsg->msg.Data[0] == 0x02){  
		ignition_status = true;
	}else{
                ignition_status = false;
		}
}

void LDV_Maxus_2021_4::outPutController(void)
{
	/* OUTPUT1 */
        
        if((seatbelt_status == OFF) && (speedValue >= 0xf)){
		ENABLE_AV1_OUTPUT(1);
	}else{
		ENABLE_AV1_OUTPUT(0);
	}
        
	/* OUTPUT2 */
        
	if(speedValue >= 0xa)
		ENABLE_AV2_OUTPUT(1);
	else
		ENABLE_AV2_OUTPUT(0);
        
	/* OUTPUT3 */
        
		ENABLE_AV3_OUTPUT(ignition_status);
		
	/* OUTPUT4 */
                
          if(!ignition_status && Lightstatus && Driversdoorstatus){
		ENABLE_AV4_OUTPUT(1);
	}else{
		ENABLE_AV4_OUTPUT(0);
		}     
           
	/* OUTPUT5 */
          
          if(!ignition_status && handbrake_status && Driversdoorstatus){
		ENABLE_AV5_OUTPUT(1);
	}else{
		ENABLE_AV5_OUTPUT(0);
		}    
          
}

void LDV_Maxus_2021_4::updateSpeedStatus(CanRMsg *pmsg)
{
}

void LDV_Maxus_2021_4::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool LDV_Maxus_2021_4::filt(CanRMsg *pmsg)
{
}


