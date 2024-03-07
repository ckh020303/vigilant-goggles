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

#include "MAN_TGS2.h"
#include "MAN_TGS2_common.h"

using namespace man_tgs2;

MAN_TGS2::MAN_TGS2(){
    name = "MAN_TGS2";
    const static uint32_t idList[] = {
        SHIFT_POS_MSG_ID,
        HANDBRAKE_MAG_ID,
        SPEED_MAG_ID,
        SPEED1_MAG_ID,
        INDICATOR_MAG_ID,
		0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 250;

    handbrake_status=OFF;
}

MAN_TGS2::~MAN_TGS2() {
}

bool MAN_TGS2::init(void)
{
	return 0;
}

void MAN_TGS2::run(CanRMsg *pmsg)
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
        case SPEED1_MAG_ID:
       		_updateSpeed1(pmsg);
       		break;
        case INDICATOR_MAG_ID:
       		_updateIndicator(pmsg);
       		break;
		default:
			break;
		}
	}
}

void MAN_TGS2::_updateIndicator(CanRMsg *pmsg)
{
    u8 IndicatorStatus = (pmsg->msg.Data[5] >> 4) & 0xf;

    switch(IndicatorStatus){
    case 0x4:
        gpDrvRightIndicator->on();
        break;
    case 0x1:
        gpDrvParkingBrake->on();
        break;
    default:
        gpDrvRightIndicator->off();
        gpDrvParkingBrake->off();
    }

}


void MAN_TGS2::_updateHandbrake(CanRMsg *pmsg)
{
    u8 ishandbreakon = (pmsg->msg.Data[0] &0xf);

    if(ishandbreakon == 0x7){
        handbrake_status = ON;
    }else if(ishandbreakon == 0x3){
        handbrake_status = OFF;
    }
}

void MAN_TGS2::_updateShiftPos(CanRMsg *pmsg)
{
	if((pmsg->msg.Data[0] == 0x7C)){
       gpDrvReverseGear->on();
    }
    else{
        gpDrvReverseGear->off();
    }
}

void MAN_TGS2::_updateSpeed(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[2] < SPEED_LIMIT){
        //gpDrvSpeedLimit->off();
        if(handbrake_status==ON){
            gpDrvLeftIndicator->off();
        }else{
            gpDrvLeftIndicator->on();
        }
    }else{
        //gpDrvSpeedLimit->on();
        gpDrvLeftIndicator->off();
    }
}

void MAN_TGS2::_updateSpeed1(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[4] & 0x40){
        gpDrvSpeedLimit->on();
    }else{
        gpDrvSpeedLimit->off();
    }
}

void MAN_TGS2::updateSpeedStatus(CanRMsg *pmsg){}
void MAN_TGS2::updateTVFKeyStatus(CanRMsg *pmsg){}
bool MAN_TGS2::filt(CanRMsg *pmsg){}



