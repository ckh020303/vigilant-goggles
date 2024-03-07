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

#include "IvecoS.h"
#include "IvecoS_common.h"

using namespace ivecoS;

IvecoS::IvecoS(){
    name = "IvecoS";
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

    new_cnt=0;
    old_cnt=0;
}

IvecoS::~IvecoS() {
}

bool IvecoS::init(void)
{
	return 0;
}

void IvecoS::run(CanRMsg *pmsg)
{
	    
	if(pmsg->msg.IDT == CAN_ID_EXT) {
		switch(pmsg->msg.ExtId) {
		case HANDBRAKE_MAG_ID:
			_updateHandbrake(pmsg);
			break;
        case DOOR_MAG_ID:
       		_updateHandbrakeDoorCTL(pmsg);
       		break;
		default:
			break;
		}
	}
}


void IvecoS::_updateHandbrake(CanRMsg *pmsg)
{   
    new_cnt+=1;

    switch(pmsg->msg.Data[0]){
        case 0xF7:
            handbrake_status=ON;
            break;
        case 0xF3:
            handbrake_status=OFF;
            break;
        default:
			break;
    }

}

void IvecoS::_updateHandbrakeDoorCTL(CanRMsg *pmsg)
{
    printf("old:%d    new:%d\n",old_cnt,new_cnt);
    if(old_cnt==new_cnt){
        gpDrvLeftIndicator->off();
        return;
    }

    if(handbrake_status==OFF){
        if(pmsg->msg.Data[0]==DOOR_OPEN){
            gpDrvLeftIndicator->on();
        }
//        else
//            gpDrvLeftIndicator->off();
    }else    
        gpDrvLeftIndicator->off();

    old_cnt = new_cnt;
    
}

void IvecoS::transfer(void)
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



void IvecoS::updateSpeedStatus(CanRMsg *pmsg)
{
}

void IvecoS::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool IvecoS::filt(CanRMsg *pmsg)
{
}


