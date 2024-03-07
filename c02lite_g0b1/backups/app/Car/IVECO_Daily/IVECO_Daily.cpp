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

#include "IVECO_Daily.h"
#include "IVECO_Daily_common.h"

using namespace iveco_daily;

IVECO_Daily::IVECO_Daily(){
    name = "IVECO_Daily";
    const static uint32_t idList[] = {

			0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;

    handbrake_status=false;

    new_cnt=0;
    old_cnt=0;
}

IVECO_Daily::~IVECO_Daily() {
}

bool IVECO_Daily::init(void)
{
	return 0;
}

void IVECO_Daily::run(CanRMsg *pmsg)
{
	    
	if(pmsg->msg.IDT == CAN_ID_EXT) {
		switch(pmsg->msg.ExtId) {
		case HANDBRAKE_MAG_ID:
			_updateHandbrake(pmsg);
            _updateHandbrakeDoorCTL(pmsg);
			break;
//        case DOOR_MAG_ID:
//       		_updateHandbrakeDoorCTL(pmsg);
//       		break;
		default:
			break;
		}
	}
}


void IVECO_Daily::_updateHandbrake(CanRMsg *pmsg)
{   
//    new_cnt+=1;

    switch(pmsg->msg.Data[6]){
        case 0x20:
            handbrake_status=ON;
            break;
        case 0x0:
            handbrake_status=OFF;
            break;
        default:
			break;
    }

}

void IVECO_Daily::_updateHandbrakeDoorCTL(CanRMsg *pmsg)
{
    if(handbrake_status==OFF){
		if((pmsg->msg.Data[2]==0x48) || (pmsg->msg.Data[2]==0x80)){
			ENABLE_AV5_OUTPUT(1);
		}else{
			ENABLE_AV5_OUTPUT(0);
		}
    }else{    
        ENABLE_AV5_OUTPUT(0);
    }
}


void IVECO_Daily::updateSpeedStatus(CanRMsg *pmsg)
{
}

void IVECO_Daily::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool IVECO_Daily::filt(CanRMsg *pmsg)
{
}


