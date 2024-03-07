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

#include "Fiat_Ducato.h"
#include "Fiat_Ducato_common.h"

using namespace fiat_ducato;

Fiat_Ducato::Fiat_Ducato(){
    name = "Fiat_Ducato";
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
    isACCon = false;
}

Fiat_Ducato::~Fiat_Ducato() {
}

bool Fiat_Ducato::init(void)
{
	return 0;
}

void Fiat_Ducato::run(CanRMsg *pmsg)
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

void Fiat_Ducato::_updateIgnition(CanRMsg *pmsg)
{
    static bool isACCOn_old = false;

//	if(pmsg->msg.Data[5] == 0x10){
//        isACCon=true;
//    }else if(pmsg->msg.Data[6] == 0x12){
//        isACCon=false;
//    }

    if(pmsg->msg.Data[5]&0x02){
        isACCon=false;
    }else{
        isACCon=true;
    }

    if(isACCon != isACCOn_old){
        if(isACCon){
            gpDrvRightIndicator->on();
        }
        else{
            gpDrvRightIndicator->off();
            gpDrvParkingBrake->off();
            gpDrvReverseGear->off();
            gpDrvSpeedLimit->off();
            gpDrvLeftIndicator->off();

            handbrake_status = false;
        }

        isACCOn_old=isACCon;
    }
}

void Fiat_Ducato::_updateHandbrake(CanRMsg *pmsg)
{
    if(isACCon){
        if(pmsg->msg.Data[6] == 0x20){
            gpDrvParkingBrake->off();
            handbrake_status = true;
        }else{
            gpDrvParkingBrake->on();
            handbrake_status = false;
        }
    }else
        gpDrvParkingBrake->off();
}

void Fiat_Ducato::_updateShiftPos(CanRMsg *pmsg)
{
	curReverse = (pmsg->msg.Data[7] & 0x4)?1:0;

    if(isACCon){
        if(curReverse != preReverse){
            preReverse = curReverse;
            if(curReverse){
                gpDrvReverseGear->on();
            }else{
                gpDrvReverseGear->off();
            }
        }
    }else
        gpDrvReverseGear->off();
}

void Fiat_Ducato::_updateSpeed(CanRMsg *pmsg)
{
    if(isACCon){
    	if(pmsg->msg.Data[4] < 0x07){
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
    }else{
        gpDrvSpeedLimit->off();
        gpDrvLeftIndicator->off();
    }
}

void Fiat_Ducato::transfer(void)
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


void Fiat_Ducato::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Fiat_Ducato::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Fiat_Ducato::filt(CanRMsg *pmsg)
{
    bool ret = false;
	if((pmsg->msg.IDT == CAN_ID_EXT) && (pmsg->msg.ExtId == SHIFT_POS_MSG_ID)) {  
        pmsg->msg.Data[0] = pmsg->msg.Data[4] =0x7c;
        pmsg->msg.Data[4] = pmsg->msg.Data[6] =0x52;
        ret = true;
	}
	
	return (ret);
}


