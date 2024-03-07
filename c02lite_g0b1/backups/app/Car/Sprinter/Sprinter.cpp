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

#include "Sprinter.h"
#include "Sprinter_common.h"

using namespace sprinter;

Sprinter::Sprinter(){
    name = "Sprinter";
    const static uint32_t idList[] = {
			SHIFT_POS_MSG_ID,
			HANDBRAKE_MSG_ID,
			IGNITION_MSG_ID,
            SPEED_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 250;
//    shiftPos = SHIFT_POS_OTHER; // 由于目前没有档位信息，让其一直处于倒档

    preReverse=0;
    curReverse=0;
    preIgnition=0;
    curIgnition=0;
    handbrake_status=false;
}

Sprinter::~Sprinter() {
}

bool Sprinter::init(void)
{
	return 0;
}

void Sprinter::run()
{
    DrvWatchDog::getInstance()->initTheDog(2500);		/* 2.5S */
	DrvWatchDog::getInstance()->enableTheDog(true);
	CanRMsg *msg;
	while(1) {
	    DrvWatchDog::getInstance()->feedTheDog();
	    
//		printf("CAN1    IDT:%x ID:%x   ",gpDrvCAN->_Can1RMsg.msg.IDT,gpDrvCAN->_Can1RMsg.msg.StdId);
//        for(int i = 0;i<8;i++)
//            printf("Data:%x ",gpDrvCAN->_Can1RMsg.msg.Data[i]);
//        printf("\n");
		msg=&(gpDrvCAN->_Can1RMsg);
		
		if(msg->msg.IDT == CAN_ID_STD) {
			switch(msg->msg.StdId) {
			case IGNITION_MSG_ID:
				_updateIgnition(msg);
				break;
			case HANDBRAKE_MSG_ID:
				_updateHandbrake(msg);
				break;
			case SHIFT_POS_MSG_ID:
				_updateShiftPos(msg);
                 break;
            case SPEED_MSG_ID:
           		_updateSpeed(msg);
           		break;
			default:
				break;
			}
		}
	}
}

void Sprinter::_updateShiftPos(CanRMsg *pmsg)
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

void Sprinter::_updateHandbrake(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[2] == 0x23){
        gpDrvParkingBrake->off();
        handbrake_status = false;
    }else if(pmsg->msg.Data[2] == 0x24){
        gpDrvParkingBrake->on();
        handbrake_status = true;
    }
}

void Sprinter::_updateIgnition(CanRMsg *pmsg)
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

void Sprinter::_updateSpeed(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[4] < 0x0a){
        gpDrvSpeedLimit->on();
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





