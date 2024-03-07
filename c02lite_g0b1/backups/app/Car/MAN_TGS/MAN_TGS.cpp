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

#include "MAN_TGS.h"
#include "MAN_TGS_common.h"

using namespace man_tgs;

MAN_TGS::MAN_TGS(){
    name = "MAN_TGS";
    const static uint32_t idList[] = {
		0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 250;

    handbrake_status=OFF;
}

MAN_TGS::~MAN_TGS() {
}

bool MAN_TGS::init(void)
{
	return 0;
}

void MAN_TGS::run(CanRMsg *pmsg)
{
	if(pmsg->msg.IDT == CAN_ID_EXT) {
		switch(pmsg->msg.ExtId) {
		case HANDBRAKE_MAG_ID:
			_updateHandbrake(pmsg);
			break;
		case SHIFT_POS_MSG_ID:
			_updateShiftPos(pmsg);
             break;
        case SPEED_MAG_ID:
       		_updateSpeed(pmsg);
       		break;
        case INDICATOR_MAG_ID:
       		_updateIndicator(pmsg);
       		break;
		default:
			break;
		}
	}
}

void MAN_TGS::_updateIndicator(CanRMsg *pmsg)
{
    u8 IndicatorStatus = (pmsg->msg.Data[5] >> 4) & 0xf;

    if(IndicatorStatus == 0x4){
        gpDrvRightIndicator->on();
    }else{
        gpDrvRightIndicator->off();
    }
}


void MAN_TGS::_updateHandbrake(CanRMsg *pmsg)
{
    u8 ishandbreakon = (pmsg->msg.Data[0] &0xf);

    if(ishandbreakon == 0x7){
        gpDrvParkingBrake->off();
        handbrake_status = ON;printf("handbrake_status=on\n");
    }else if(ishandbreakon == 0x3){
        gpDrvParkingBrake->on();
        handbrake_status = OFF;printf("handbrake_status=off\n");
    }
}

void MAN_TGS::_updateShiftPos(CanRMsg *pmsg)
{
	if((pmsg->msg.Data[0] == 0x7C)){
       gpDrvReverseGear->on();
    }
    else{
        gpDrvReverseGear->off();
    }
}

void MAN_TGS::_updateSpeed(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[2] < SPEED_LIMIT){
        gpDrvSpeedLimit->off();printf("speed\n");
        if(handbrake_status==ON){
            gpDrvLeftIndicator->off();printf("output 5 off\n");
        }else{
            gpDrvLeftIndicator->on();printf("output 5 off\n");
        }
    }else{
        gpDrvSpeedLimit->on();
        gpDrvLeftIndicator->off();
    }
}








void MAN_TGS::updateSpeedStatus(CanRMsg *pmsg){}
void MAN_TGS::updateTVFKeyStatus(CanRMsg *pmsg){}
bool MAN_TGS::filt(CanRMsg *pmsg){}



