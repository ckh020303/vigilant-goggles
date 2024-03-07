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

#include "Ford_Transit.h"
#include "Ford_Transit_common.h"

using namespace ford_transit;

Ford_Transit::Ford_Transit(){
    name = "Ford_Transit";
    const static uint32_t idList[] = {
			REAR_DOORS_MSG_ID,
            SIDE_DOOR_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;
    preReverse=0;
    curReverse=0;
    preIgnition=0;
    curIgnition=0;
    handbrake_status=false;
}

Ford_Transit::~Ford_Transit() {
}

bool Ford_Transit::init(void)
{
	return 0;
}

void Ford_Transit::run(CanRMsg *pmsg)
{
	    
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case REAR_DOORS_MSG_ID:
			_updateRearDoorStatus(pmsg);
			break;
        case SIDE_DOOR_MSG_ID:
       		_updateSideDoorStatus(pmsg);
       		break;
		default:
			break;
		}
	}
}

void Ford_Transit::_updateRearDoorStatus(CanRMsg *pmsg)
{
//	u8 rear_door_status = (pmsg->msg.Data[7] & 0xF);
//    printf("raer:%x\n",rear_door_status);
    if((pmsg->msg.Data[7]==0x4) || (pmsg->msg.Data[7]==0x6) || (pmsg->msg.Data[7]==0x24) || (pmsg->msg.Data[7]==0x26) || (pmsg->msg.Data[7]==0x34) || (pmsg->msg.Data[7]==0x36) || (pmsg->msg.Data[7]==0x14) || (pmsg->msg.Data[7]==0x16)){
        gpDrvRightIndicator->on();
    }else if((pmsg->msg.Data[7]==0x0)||(pmsg->msg.Data[7]==0x2) || (pmsg->msg.Data[7]==0x10) || (pmsg->msg.Data[7]==0x12) || (pmsg->msg.Data[7]==0x20) || (pmsg->msg.Data[7]==0x22) || (pmsg->msg.Data[7]==0x30) || (pmsg->msg.Data[7]==0x32)){
        gpDrvRightIndicator->off();
    }
    
}

void Ford_Transit::_updateSideDoorStatus(CanRMsg *pmsg)
{
    u8 side_door_status = (pmsg->msg.Data[5] & 0xF);
    
	if((side_door_status==0x8)){
        gpDrvReverseGear->on();
    }else if((side_door_status==0x0)){
        gpDrvReverseGear->off();
    }

    
}


void Ford_Transit::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Ford_Transit::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Ford_Transit::filt(CanRMsg *pmsg)
{
}



