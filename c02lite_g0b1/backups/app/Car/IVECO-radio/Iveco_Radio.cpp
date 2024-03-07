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

#include "Iveco_Radio.h"
#include "Iveco_Radio_common.h"

using namespace Iveco_radio;

Iveco_Radio::Iveco_Radio(){
    name = "Iveco_Radio";
    const static uint32_t idList[] = {

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

Iveco_Radio::~Iveco_Radio() {
}

bool Iveco_Radio::init(void)
{
	return 0;
}

void Iveco_Radio::run(CanRMsg *pmsg)
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

void Iveco_Radio::_updateIgnition(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[0] == 0x04){
//        curIgnition=1;
           gpDrvRightIndicator->on();
           }
        else if(pmsg->msg.Data[6] == 0x0){
//        curIgnition=0;
                gpDrvRightIndicator->off();
                }
//    if(curIgnition != preIgnition){
//        preIgnition = curIgnition;
//        if(curIgnition)
//            gpDrvRightIndicator->on();
//        else
//            gpDrvRightIndicator->off();
//    }

}

void Iveco_Radio::_updateHandbrake(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[6] == 0x0){
           gpDrvParkingBrake->on();
           handbrake_status = false;
           }
        else if(pmsg->msg.Data[6] == 0x20){
                gpDrvParkingBrake->off();
                handbrake_status = true;
                }
}

void Iveco_Radio::_updateShiftPos(CanRMsg *pmsg)
{
	 if((pmsg->msg.Data[3] == 0x7c)){
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

void Iveco_Radio::_updateSpeed(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[1] < 0x0a){
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

void Iveco_Radio::transfer(void)
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


void Iveco_Radio::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Iveco_Radio::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Iveco_Radio::filt(CanRMsg *pmsg)
{
    bool ret = false;
	if((pmsg->msg.IDT == CAN_ID_EXT) && (pmsg->msg.ExtId == SHIFT_POS_MSG_ID)) {  
        pmsg->msg.Data[0] = pmsg->msg.Data[4] =0x7c;
        pmsg->msg.Data[4] = pmsg->msg.Data[6] =0x52;
        ret = true;
	}
	
	return (ret);
}


