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

#include "Nissan.h"
#include "Nissan_common.h"

using namespace nissan;

Nissan::Nissan(){
    name = "Nissan";
    const static uint32_t idList[] = {
            HANDBRAKE_MAG_ID,
            IGNITION_MAG_ID,
            SPEED_MAG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;

    handbrake_status=false;
    ignition_status=false;
}

Nissan::~Nissan() {
}

bool Nissan::init(void)
{
	return 0;
}

void Nissan::run(CanRMsg *pmsg)
{
	    
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
        case IGNITION_MAG_ID:
       		_updateIgnition(pmsg);
       		break;
        case HANDBRAKE_MAG_ID:
       		_updateHandbrake(pmsg);
       		break;
        case SPEED_MAG_ID:
            _updateSpeed(pmsg);
		default:
			break;
		}
	}
}



void Nissan::_updateIgnition(CanRMsg *pmsg)
{
    if(pmsg->msg.Data[0]==0x80){
        ignition_status = ON;
    }else if(pmsg->msg.Data[0]==0x0){
        ignition_status = OFF;
    }
}

void Nissan::_updateHandbrake(CanRMsg *pmsg)
{
    if((pmsg->msg.Data[0] & 0xF) == 0x4){
        handbrake_status=ON;
    }else if((pmsg->msg.Data[0] & 0xF) == 0x0){
        handbrake_status=OFF;
    }
}


void Nissan::_updateSpeed(CanRMsg *pmsg)
{
    u8 speed_status = 0;
    
    if((pmsg->msg.Data[2]>=0) && (pmsg->msg.Data[2]<=2)){
        speed_status = SPEED_0_5MPH;
    }else if((pmsg->msg.Data[2]>=3) && (pmsg->msg.Data[2]<=5)){
        speed_status = SPEED_5_10MPH;
    }else if((pmsg->msg.Data[2]>=6) && (pmsg->msg.Data[2]<=10)){
        speed_status = SPEED_10_20MPH;
    }else if((pmsg->msg.Data[2]>=11) && (pmsg->msg.Data[2]<=15)){
        speed_status = SPEED_20_30MPH;
    }else{
        speed_status = SPEED_OTHER;
    }
    
    if(ignition_status && !handbrake_status && speed_status==SPEED_0_5MPH){
        HandleOutput(1,ON);
        HandleOutput(2,OFF);
        HandleOutput(3,OFF);
        HandleOutput(4,OFF);
    }else if(speed_status==SPEED_5_10MPH){
        HandleOutput(2,ON);
        HandleOutput(1,OFF);
        HandleOutput(3,OFF);
        HandleOutput(4,OFF);
    }else if(speed_status==SPEED_10_20MPH){
        HandleOutput(3,ON);
        HandleOutput(2,OFF);
        HandleOutput(1,OFF);
        HandleOutput(4,OFF);
    }else if(speed_status==SPEED_20_30MPH){
        HandleOutput(4,ON);
        HandleOutput(2,OFF);
        HandleOutput(3,OFF);
        HandleOutput(1,OFF);
    }else{
        HandleOutput(1,OFF);
        HandleOutput(2,OFF);
        HandleOutput(3,OFF);
        HandleOutput(4,OFF);
    }
}


void Nissan::HandleOutput(u8 outputNum,bool status)
{
    switch(outputNum){
    case 1:
        if(status)
            gpDrvSpeedLimit->on();
        else
            gpDrvSpeedLimit->off();
        break;
    case 2:
        if(status)
            gpDrvReverseGear->on();
        else
            gpDrvReverseGear->off();
        break;
    case 3:
        if(status)
            gpDrvParkingBrake->on();
        else
            gpDrvParkingBrake->off();
        break;
    case 4:
        if(status)
            gpDrvRightIndicator->on();
        else
            gpDrvRightIndicator->off();
        break;
    case 5:
        if(status)
            gpDrvLeftIndicator->on();
        else
            gpDrvLeftIndicator->off();
        break;
    default:
        printf("outputNum error\n");
        break;
    }
}



void Nissan::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Nissan::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Nissan::filt(CanRMsg *pmsg)
{
	return false;
}



