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

#include "Sprinter_Psm.h"
#include "Sprinter_Psm_common.h"

using namespace sprinter_psm;

Sprinter_Psm::Sprinter_Psm(){
    name = "Sprinter_Psm";
    const static uint32_t idList[] = {
			SHIFT_POS_MSG_ID,
			HANDBRAKE_MSG_ID,
			IGNITION_MSG_ID,
            SPEED_MSG_ID,
            Indicator_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 250;
//    shiftPos = SHIFT_POS_OTHER; // 鐢变簬鐩墠娌℃湁妗ｄ綅淇℃伅锛岃鍏朵竴鐩村浜庡�掓。

    preReverse=0;
    curReverse=0;
    preIgnition=0;
    curIgnition=0;
    handbrake_status=false;
    Indicator_status=false;
}

Sprinter_Psm::~Sprinter_Psm() {
}

bool Sprinter_Psm::init(void)
{
	return 0;
}

void Sprinter_Psm::run(CanRMsg *pmsg)
{
		
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case IGNITION_MSG_ID:
			_updateIgnition(pmsg);
			break;
		case Indicator_MSG_ID:
			_updateIndicator(pmsg);
			break;
		case HANDBRAKE_MSG_ID:
			_updateHandbrake(pmsg);
			break;
		case SHIFT_POS_MSG_ID:
			_updateShiftPos(pmsg);
             break;
        case SPEED_MSG_ID:
       		_updateSpeed(pmsg);
       		break;
		default:
			break;
		}
	}
}

void Sprinter_Psm::_updateShiftPos(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[4] == 0xc2){
       curReverse = 1;
    }else{
        curReverse = 0;
    }

    if(curReverse != preReverse){
        preReverse = curReverse;
        if(curReverse)
            gpDrvReverseGear->on();
        else
            gpDrvReverseGear->off();
    }
}

void Sprinter_Psm::_updateHandbrake(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[2] == 0x23){
        gpDrvParkingBrake->off();
        handbrake_status = false;
    }else if(pmsg->msg.Data[2] == 0x24){
        gpDrvParkingBrake->on();
        handbrake_status = true;
    }
}

void Sprinter_Psm::_updateIndicator(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[3] == 0x01){
        Indicator_status = true;
    }else {
        Indicator_status = false;
    }
}

void Sprinter_Psm::_updateIgnition(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[6] == 0x10){
        curIgnition=1;
    }else{
        curIgnition=0;
    }

    if(curIgnition != preIgnition){
        preIgnition = curIgnition;
        if(curIgnition)
            gpDrvRightIndicator->on();
        else
            gpDrvRightIndicator->off();
    }
}

void Sprinter_Psm::_updateSpeed(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[4] < 0x0a){

        if(Indicator_status==true){
            gpDrvSpeedLimit->on();
        }else{
            gpDrvSpeedLimit->off();
        }

        if(handbrake_status==true){
            gpDrvLeftIndicator->on();
        }else{
            gpDrvLeftIndicator->off();
        }
    }else{
        gpDrvSpeedLimit->off();
        gpDrvLeftIndicator->off();
    }
}


void Sprinter_Psm::updateSpeedStatus(CanRMsg *pmsg){}
void Sprinter_Psm::updateTVFKeyStatus(CanRMsg *pmsg){}
bool Sprinter_Psm::filt(CanRMsg *pmsg){return false;}


