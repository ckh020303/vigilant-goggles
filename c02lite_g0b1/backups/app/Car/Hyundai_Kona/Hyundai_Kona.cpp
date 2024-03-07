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

#include "Hyundai_Kona.h"
#include "Hyundai_Kona_common.h"

using namespace Hyundai_kona;

Hyundai_Kona::Hyundai_Kona(){
	name = "Hyundai_Kona";
	const static uint32_t idList[] = {
			HANDBRAKE_MSG_ID,
			FOOTBRAKE_MSG_ID,
            IGNITION_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;
	ignition_status  = false;
	footbrake_status = false;
    handbrake_status = false;
}

Hyundai_Kona::~Hyundai_Kona() {
}

bool Hyundai_Kona::init(void)
{
	return 0;
}

void Hyundai_Kona::run(CanRMsg *pmsg)
{    
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case HANDBRAKE_MSG_ID:
			_updateHandbrake(pmsg);
			break;
		case FOOTBRAKE_MSG_ID:
			_updateFootbrake(pmsg);
			break;
		case IGNITION_MSG_ID:
			_updateIgnition(pmsg);
			break;
		default:
			break;
		}
	}

	/*
	 * Set the outputs
	 */
	  outPutController();                 

}

void Hyundai_Kona::_updateIgnition(CanRMsg *pmsg)
{
	ignition_status = (pmsg->msg.Data[1] == 0x5e)?true:false;
}

void Hyundai_Kona::_updateHandbrake(CanRMsg *pmsg)
{   
    handbrake_status = (pmsg->msg.Data[0] == 0x01)?true:false;
}

void Hyundai_Kona::_updateFootbrake(CanRMsg *pmsg)
{
    footbrake_status = (pmsg->msg.Data[2] == 0x17)?true:false;
	speedValue = pmsg->msg.Data[0];
}

void Hyundai_Kona::outPutController(void)
{

	/* OUTPUT1 */
	ENABLE_AV1_OUTPUT(!handbrake_status);        

	/* OUTPUT2 */
	ENABLE_AV2_OUTPUT(footbrake_status);

	/* OUTPUT3 */
	if((speedValue <= 0x0a) && !handbrake_status){
		ENABLE_AV3_OUTPUT(1);		
	}else{
		ENABLE_AV3_OUTPUT(0);
	}

	/* OUTPUT4 */
	if(speedValue >= 0x01){
		ENABLE_AV4_OUTPUT(1);		
	}else{
		ENABLE_AV4_OUTPUT(0);
	}

	/* OUTPUT5 */
	ENABLE_AV5_OUTPUT(ignition_status);		
     
}

void Hyundai_Kona::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Hyundai_Kona::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Hyundai_Kona::filt(CanRMsg *pmsg)
{
	return false;
}


