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

#include "IvecoS_Way2.h"
#include "IvecoS_Way2_common.h"

using namespace ivecoS_Way2;
IvecoS_Way2::IvecoS_Way2(){
    name = "IvecoS_Way2";
    const static uint32_t idList[] = {
		        INDICATOR_MAG_ID,
			IGNITION_MAG_ID,
			HANDBRAKE_MAG_ID,
			SHIFT_POS_MSG_ID,
			SPEED_MAG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;

    preReverse=0;
    curReverse=0;
    preIgnition=0;
    curIgnition=0;
    handbrake_status=false;
    Ignition_status=false;
    Speed_status=false;

}
IvecoS_Way2::~IvecoS_Way2() {
}

bool IvecoS_Way2::init(void)
{
	return 0;
}

void IvecoS_Way2::run(CanRMsg *pmsg)
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
		case INDICATOR_MAG_ID:
			_updateIndicatorstatus(pmsg);
       		        break;
		default:
			break;
		}
	}
}

//DrvGPIO *gpDrvSpeedLimit;		//AV1
//DrvGPIO *gpDrvParkingBrake;		//AV3
//DrvGPIO *gpDrvReverseGear;		//AV2
//DrvGPIO *gpDrvRightIndicator;	        //AV4
//DrvGPIO *gpDrvLeftIndicator;	        //AV5

void IvecoS_Way2::_updateIndicatorstatus(CanRMsg *pmsg)
{
	static u8 output1OnCnt = 0;
	static u8 output4OnCnt = 0;
	u32 curTicks = 0;
	static u32 lastOut1Ticks = 0;
	static u32 lastOut4Ticks = 0;
	
	if(Ignition_status){
		if(pmsg->msg.Data[1] == 0x01){
			curTicks = systick_gettime();
			if(lastOut1Ticks == 0) lastOut1Ticks = curTicks;
			if((curTicks-lastOut1Ticks) > 100){
				lastOut1Ticks = 0;
				gpDrvSpeedLimit->on();                   //left Indicatorstatus output1 ON
			}
		}else{
			lastOut1Ticks = 0;
			gpDrvSpeedLimit->off();              
		}
    }else{
    	lastOut1Ticks = 0;
		gpDrvSpeedLimit->off();
	}
        
        
	if(Ignition_status){
		if(gpDrvSpeedLimit->getValue()){
			if(Speed_status){
				gpDrvParkingBrake->on();                   //left Indicatorstatus output3 ON
			}else{
				gpDrvParkingBrake->off();
			}        
		}else{
			gpDrvParkingBrake->off();              
		}
	}else{
		gpDrvParkingBrake->off();
	}
 
        
	if(Ignition_status){
		if(pmsg->msg.Data[1] == 0x02){
			curTicks = systick_gettime();
			if(lastOut4Ticks == 0) lastOut4Ticks = curTicks;
			if((curTicks-lastOut4Ticks) > 50){
				lastOut4Ticks = 0;
				gpDrvRightIndicator->on();                //right Indicatorstatus output1 ON
			}
		}else{
			lastOut4Ticks = 0;
			gpDrvRightIndicator->off();
		}
	}else{
		lastOut4Ticks = 0;
		gpDrvRightIndicator->off();
	}

}


void IvecoS_Way2::_updateIgnition(CanRMsg *pmsg) 
{
	    if(pmsg->msg.Data[0] == 0xC0){
		   Ignition_status = true;
           }
        else if(pmsg->msg.Data[0] == 0xC2){
		   Ignition_status = false;	
                }
        
	if(Ignition_status){
		if(Speed_status){   //小于10
			if(handbrake_status){
				gpDrvLeftIndicator->on();                   //Output5
			}else{
				gpDrvLeftIndicator->off();              
			}
		}else{
			gpDrvLeftIndicator->off();
		}
	}else{
		gpDrvLeftIndicator->off();
	}
}

void IvecoS_Way2::_updateHandbrake(CanRMsg *pmsg) 
{
	if(pmsg->msg.Data[5] == 0x1f){                   
                handbrake_status = true;
           }
        else if(pmsg->msg.Data[5] == 0x0f){            

                handbrake_status = false;
                }
}

void IvecoS_Way2::_updateShiftPos(CanRMsg *pmsg)    
{

                   if(Ignition_status){
                     if((pmsg->msg.Data[0] == 0x7c)){
               gpDrvReverseGear->on();                 //Output2
               }else{
                     gpDrvReverseGear->off();         
                     }
                    	}else{
		            gpDrvReverseGear->off();
          }

}

void IvecoS_Way2::_updateSpeed(CanRMsg *pmsg)     
{
	if(pmsg->msg.Data[2] < 0x0A){
           Speed_status = true;             
           }
        else{
           Speed_status = false;              
                 } 


}

void IvecoS_Way2::transfer(void)
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


void IvecoS_Way2::updateSpeedStatus(CanRMsg *pmsg)
{
}

void IvecoS_Way2::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool IvecoS_Way2::filt(CanRMsg *pmsg)
{
    bool ret = false;
	if((pmsg->msg.IDT == CAN_ID_EXT) && (pmsg->msg.ExtId == SHIFT_POS_MSG_ID)) {  
        pmsg->msg.Data[0] = pmsg->msg.Data[4] =0x7c;
        pmsg->msg.Data[4] = pmsg->msg.Data[6] =0x52;
        ret = true;
	}
	
	return (ret);
}


