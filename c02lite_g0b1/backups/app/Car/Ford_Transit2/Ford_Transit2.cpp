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

#include "Ford_Transit2.h"
#include "Ford_Transit2_common.h"

using namespace ford_transit2;

Ford_Transit2::Ford_Transit2(){
    name = "Ford_Transit2";
    const static uint32_t idList[] = {
			SHIFT_POS_MSG_ID,
            HANDBRAKE_MSG_ID,
            IGNITION_MSG_ID,
            SPEED_MSG_ID,
            INDICATOR_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;

    handbrake_status=false;
}

Ford_Transit2::~Ford_Transit2() {
}

bool Ford_Transit2::init(void)
{
	return 0;
}

void Ford_Transit2::run(CanRMsg *pmsg)
{
	    
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case SHIFT_POS_MSG_ID:
			_updateShiftPos(pmsg);
			break;
        case INDICATOR_MSG_ID:
       		_updateIndicator(pmsg);
       		break;
        case IGNITION_MSG_ID:
       		_updateIgnition(pmsg);
            _updateSpeed(pmsg);
       		break;
        case HANDBRAKE_MSG_ID:
       		_updateHandbrake(pmsg);
       		break;
		default:
			break;
		}
	}
}

void Ford_Transit2::_updateShiftPos(CanRMsg *pmsg)
{
    if(pmsg->msg.Data[0] == 0x4){
        HandleOutput(2,OFF);
    }else if(pmsg->msg.Data[0] == 0xC){
        HandleOutput(2,ON);
    }
    
}

void Ford_Transit2::_updateIndicator(CanRMsg *pmsg)
{
    u8 indicator_status = (pmsg->msg.Data[3] & 0xF0);

    switch(indicator_status){
    case 0x0:
        HandleOutput(1,OFF);
        HandleOutput(3,OFF);
        break;
    case 0x10:
        HandleOutput(3,ON);
        break;    
    case 0x20:
        HandleOutput(1,ON);
        break; 
    default:
        break;
    }
    
}

void Ford_Transit2::_updateIgnition(CanRMsg *pmsg)
{
    u8 ignition_status = pmsg->msg.Data[2] & 0xF;

    if(ignition_status==0xE){
        HandleOutput(4,OFF);
    }else if(ignition_status==0x1){
        HandleOutput(4,ON);
    }
}

void Ford_Transit2::_updateHandbrake(CanRMsg *pmsg)
{
    if(pmsg->msg.Data[2] == 0x10){
        handbrake_status=ON;
    }else if(pmsg->msg.Data[2] == 0x0){
        handbrake_status=OFF;
    }
}


void Ford_Transit2::_updateSpeed(CanRMsg *pmsg)
{
    if(pmsg->msg.Data[4] < 0xa){
        if(handbrake_status==OFF)
            HandleOutput(5,ON);
        else
            HandleOutput(5,OFF);
    }else
        HandleOutput(5,OFF);
}


void Ford_Transit2::HandleOutput(u8 outputNum,bool status)
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



void Ford_Transit2::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Ford_Transit2::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Ford_Transit2::filt(CanRMsg *pmsg)
{
	return false;
}



