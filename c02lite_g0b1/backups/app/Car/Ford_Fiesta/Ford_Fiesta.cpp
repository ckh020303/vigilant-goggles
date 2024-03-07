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

#include "Ford_Fiesta.h"
#include "Ford_Fiesta_common.h"

using namespace ford_fiesta;

Ford_Fiesta::Ford_Fiesta(){
    name = "Ford_Fiesta";
    const static uint32_t idList[] = {
			REAR_DOORS_MSG_ID,
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

Ford_Fiesta::~Ford_Fiesta() {
}

bool Ford_Fiesta::init(void)
{
	return 0;
}

void Ford_Fiesta::run(CanRMsg *pmsg)
{
	    
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case REAR_DOORS_MSG_ID:
			_updateRearDoorStatus(pmsg);
			break;
		default:
			break;
		}
	}
}

/*void Ford_Fiesta::_updateRearDoorStatus(CanRMsg *pmsg)     //Car for Ford_Fiesta
{
    if((pmsg->msg.Data[6]==0x0) || (pmsg->msg.Data[6]==0x40)){
		if((pmsg->msg.Data[7]==0x20) || (pmsg->msg.Data[7]==0x22) || (pmsg->msg.Data[7]==0x30) || (pmsg->msg.Data[7]==0x32) || (pmsg->msg.Data[7]==0xB0) || (pmsg->msg.Data[7]==0x70) || (pmsg->msg.Data[7]==0x72) || (pmsg->msg.Data[7]==0xB2) || (pmsg->msg.Data[7]==0xA0) || (pmsg->msg.Data[7]==0x60) || (pmsg->msg.Data[7]==0xA2) || (pmsg->msg.Data[7]==0x62)){
           //gpDrvRightIndicator->on();  //4
		   //gpDrvLeftIndicator->on();   //5
	       gpDrvSpeedLimit->on();      //1
		   gpDrvReverseGear->on();     //2
		   //gpDrvParkingBrake->on();    //3  
		}else if ((pmsg->msg.Data[6]==0x80) || (pmsg->msg.Data[6]==0xC0)){
	       gpDrvSpeedLimit->off();
		   gpDrvReverseGear->off();
		}

	}else if ((pmsg->msg.Data[6]==0x80) || (pmsg->msg.Data[6]==0xC0)){
	       gpDrvSpeedLimit->off();
		   gpDrvReverseGear->off();
	}
}*/

/*void Ford_Fiesta::_updateRearDoorStatus(CanRMsg *pmsg)     //Car for Citroen Dispatch
{
    if((pmsg->msg.Data[6]==0xd8) || (pmsg->msg.Data[6]==0xc0) || (pmsg->msg.Data[6]==0xe0) || (pmsg->msg.Data[6]==0xe8) || (pmsg->msg.Data[6]==0xf0) || (pmsg->msg.Data[6]==0xf8) || (pmsg->msg.Data[6]==0xa0) || (pmsg->msg.Data[6]==0x80) || (pmsg->msg.Data[6]==0x88) || (pmsg->msg.Data[6]==0x90) || (pmsg->msg.Data[6]==0x98)){
	       gpDrvRightIndicator->on(); 	
	}else{
	       gpDrvRightIndicator->off();
	}
}*/

void Ford_Fiesta::_updateRearDoorStatus(CanRMsg *pmsg)     //Car for Vauxhall Movano
{
    if((pmsg->msg.Data[0]==0x80) || (pmsg->msg.Data[0]==0xA0) || (pmsg->msg.Data[0]==0xB0) || (pmsg->msg.Data[0]==0xA8) || (pmsg->msg.Data[0]==0xB8) || (pmsg->msg.Data[0]==0x90) || (pmsg->msg.Data[0]==0x88) || (pmsg->msg.Data[0]==0x98)){
	       gpDrvSpeedLimit->on(); 	
	}else{
	       gpDrvSpeedLimit->off();
	}
}


void Ford_Fiesta::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Ford_Fiesta::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Ford_Fiesta::filt(CanRMsg *pmsg)
{
}



