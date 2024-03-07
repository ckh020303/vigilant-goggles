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

#include "Iveco_Yellow.h"
#include "Iveco_Yellow_common.h"

using namespace Iveco_yellow;

Iveco_Yellow::Iveco_Yellow(){
    name = "Iveco_yellow";
    const static uint32_t idList[] = {

			0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 250;

    preReverse=0;
    curReverse=0;
    preIgnition=0;
    curIgnition=0;
    handbrake_status=false;
}

Iveco_Yellow::~Iveco_Yellow() {
}

bool Iveco_Yellow::init(void)
{
	return 0;
}

void Iveco_Yellow::run(CanRMsg *pmsg)
{

		if(pmsg->msg.IDT == CAN_ID_EXT) {
			switch(pmsg->msg.ExtId) {
			case IGNITION_MAG_ID:
				_updateIgnition(pmsg);
				break;
			case HANDBRAKE_MAG_ID:
				_updateHandbrake(pmsg);
				break;
			case SHIFT_POS_MSG_ID:
				_updateShiftPos(pmsg);
                 break;
            case SPEED_MAG_ID:
           		_updateSpeed(pmsg);
           		break;
			default:
				break;
			}
		}
}

void Iveco_Yellow::_updateIgnition(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[5] == 0x4){
        curIgnition=1;
    }else if(pmsg->msg.Data[6] == 0x1){
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

void Iveco_Yellow::_updateHandbrake(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[6] == 0x20){
        gpDrvParkingBrake->off();
        handbrake_status = true;
    }else{
        gpDrvParkingBrake->on();
        handbrake_status = false;
    }

}

void Iveco_Yellow::_updateShiftPos(CanRMsg *pmsg)
{
	 if((pmsg->msg.Data[2] == 0x1E)){
       curReverse = 1;
    }
    else{
        curReverse = 0;
    }

    if(curReverse != preReverse){
        preReverse = curReverse;
        if(curReverse){
            gpDrvReverseGear->on();
        }else{
            gpDrvReverseGear->off();
        }
    }

}

void Iveco_Yellow::_updateSpeed(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[2] < 0x0a){
        gpDrvSpeedLimit->on();
        if(handbrake_status==true){
            gpDrvLeftIndicator->off();
        }else{
            gpDrvLeftIndicator->on();
        }
    }else{
        gpDrvSpeedLimit->off();
        gpDrvLeftIndicator->off();
    }

}

void Iveco_Yellow::transfer(void)
{
    CanTxMsg TxMessage;

	TxMessage.ExtId = 0x0;
	TxMessage.IDT   = CAN_Id_Extended;
	TxMessage.RTR   = CAN_RTR_DATA;
	TxMessage.DLC   = 8;
	for (int i = 0; i<TxMessage.DLC; i++) {
		TxMessage.Data[i] = 0x0;
	}

	gpDrvCAN->transmit(CAN1, &TxMessage, false);
}


void Iveco_Yellow::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Iveco_Yellow::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Iveco_Yellow::filt(CanRMsg *pmsg)
{
    bool ret = false;
	if((pmsg->msg.IDT == CAN_ID_EXT) && (pmsg->msg.ExtId == SHIFT_POS_MSG_ID)) {  
        pmsg->msg.Data[0] = pmsg->msg.Data[4] =0x7c;
        pmsg->msg.Data[4] = pmsg->msg.Data[6] =0x52;
        ret = true;
	}
	
	return (ret);
}


