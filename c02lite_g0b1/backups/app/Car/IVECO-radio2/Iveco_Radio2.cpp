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

#include "Iveco_Radio2.h"
#include "Iveco_Radio2_common.h"

using namespace Iveco_radio2;

Iveco_Radio2::Iveco_Radio2(){
    name = "Iveco_Radio2";
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

}

Iveco_Radio2::~Iveco_Radio2() {
}

bool Iveco_Radio2::init(void)
{
	return 0;
}

void Iveco_Radio2::run(CanRMsg *pmsg)
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

void Iveco_Radio2::_updateIndicatorstatus(CanRMsg *pmsg)
{
	static u8 output1ReleaseCnt = 0;
	static u8 output4ReleaseCnt = 0;
	
	if(pmsg->msg.Data[1]==0x40){
		output1ReleaseCnt = 0;
		gpDrvSpeedLimit->on();                   //left Indicatorstatus output1 ON
	}else{
		if(gpDrvSpeedLimit->getValue() && ++output1ReleaseCnt==30){
			output1ReleaseCnt = 0;
			gpDrvSpeedLimit->off();              
		}
	}
        
	if(pmsg->msg.Data[1]==0x10){
		output4ReleaseCnt = 0;
		gpDrvRightIndicator->on();               //right Indicatorstatus output1 ON
	}else{
		if(gpDrvRightIndicator->getValue() && ++output4ReleaseCnt==30){
			output4ReleaseCnt = 0;
			gpDrvRightIndicator->off();
		}
	}
	
}


void Iveco_Radio2::_updateIgnition(CanRMsg *pmsg) 
{
	if(pmsg->msg.Data[5] == 0x04){
		   Ignition_status = true;
           //gpDrvRightIndicator->on();                  //AV4
           }
        else if(pmsg->msg.Data[5] == 0x01){
		   Ignition_status = false;	
                //gpDrvRightIndicator->off();            //AV4
                }

}

void Iveco_Radio2::_updateHandbrake(CanRMsg *pmsg) 
{
	if(pmsg->msg.Data[6] == 0x0){
           gpDrvParkingBrake->on();                    //AV3
           handbrake_status = false;
           }
        else if(pmsg->msg.Data[6] == 0x20){
                gpDrvParkingBrake->off();              //AV3
                handbrake_status = true;
                }
}

void Iveco_Radio2::_updateShiftPos(CanRMsg *pmsg)    
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
               gpDrvReverseGear->on();                 //AV2
               }else{
                     gpDrvReverseGear->off();          //AV2
                     }
    }

}

void Iveco_Radio2::_updateSpeed(CanRMsg *pmsg)     
{
//	if(pmsg->msg.Data[1] < 0x0a){
//           //gpDrvSpeedLimit->on();
//           if(handbrake_status==true){
//              gpDrvLeftIndicator->off();                 //AV5
//           }else{
//              gpDrvLeftIndicator->on();                  //AV5
//                 } 
//        }else{
//              //gpDrvSpeedLimit->off();                     
//              gpDrvLeftIndicator->off();                 //AV5
//              }
		
	if(Ignition_status){
	       if(pmsg->msg.Data[1] < 0x0a){
			  if(handbrake_status==true){
				  gpDrvLeftIndicator->off();		     //AV5
			  }else{
				  gpDrvLeftIndicator->on(); 		     //AV5
					} 
		   }else{			
				 gpDrvLeftIndicator->off();				 //AV5
				 }
	}else{
		  gpDrvLeftIndicator->off();
          }

}

void Iveco_Radio2::transfer(void)
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


void Iveco_Radio2::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Iveco_Radio2::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Iveco_Radio2::filt(CanRMsg *pmsg)
{
    bool ret = false;
	if((pmsg->msg.IDT == CAN_ID_EXT) && (pmsg->msg.ExtId == SHIFT_POS_MSG_ID)) {  
        pmsg->msg.Data[0] = pmsg->msg.Data[4] =0x7c;
        pmsg->msg.Data[4] = pmsg->msg.Data[6] =0x52;
        ret = true;
	}
	
	return (ret);
}


