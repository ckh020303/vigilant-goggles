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

#include "Fiat_talento_2017.h"
#include "Fiat_talento_2017_common.h"

using namespace FIAT_TALENTO_2017_NS;

Fiat_talento_2017::Fiat_talento_2017(){
	name = "Fiat_talento_2017";
	const static uint32_t idList[] = {
			SHIFT_POS_MSG_ID,
			HANDBRAKE_MSG_ID,
			IGNITION_MSG_ID,
			SPEED_MSG_ID,
			0x0,
	};
	pCanFilterIDList = idList;
	baudRate = 500;

	ignition_status  = false;
	reverse_status   = false;
	handbrake_status = false;
}

Fiat_talento_2017::~Fiat_talento_2017() {
}

bool Fiat_talento_2017::init(void)
{
	return 0;
}

void Fiat_talento_2017::run(CanRMsg *pmsg)
{
	if(pmsg->msg.IDT == CAN_ID_STD) {
		switch(pmsg->msg.StdId) {
		case IGNITION_MSG_ID:
			_updateIgnition(pmsg);
			break;
		case HANDBRAKE_MSG_ID:
			_updateHandbrake(pmsg);
			break;
		case SHIFT_POS_MSG_ID:
			_updateShiftPos(pmsg);
			break;
		case SPEED_MSG_ID:
			_updateSpeed(pmsg);
			break;
		default:
			break;
		}
	}
	outPutController();
}

void Fiat_talento_2017::_updateShiftPos(CanRMsg *pmsg)
{
	if(pmsg->msg.Data[5] == 0xB8){
		reverse_status = true;
	}else{ //0x18
		reverse_status = false;
	}
}

void Fiat_talento_2017::_updateHandbrake(CanRMsg *pmsg)
{
	if((pmsg->msg.Data[0]==0x58) || (pmsg->msg.Data[0]==0x98)){
		handbrake_status = true;
	}else{ //0x54
		handbrake_status = false;
	}
}

void Fiat_talento_2017::_updateIgnition(CanRMsg *pmsg)
{
	ignition_status = (pmsg->msg.Data[0]&0x4)?true:false;
}

void Fiat_talento_2017::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[3];
}

void Fiat_talento_2017::outPutController(void)
{
	/* OUTPUT1 */
	if((speedValue<=0xa) && ignition_status){
		ENABLE_AV1_OUTPUT(1);
	}else{
		ENABLE_AV1_OUTPUT(0);
	}

	/* OUTPUT2 */
	ENABLE_AV2_OUTPUT(reverse_status);

	/* OUTPUT3 */
	ENABLE_AV3_OUTPUT(!handbrake_status);

	/* OUTPUT4 */
	ENABLE_AV4_OUTPUT(ignition_status);

	/* OUTPUT5 */
	if((speedValue<=0xa) && ignition_status && !handbrake_status){
		ENABLE_AV5_OUTPUT(1);		// Speed <= 10 and HandBrake Off then Output High
	}else{
		ENABLE_AV5_OUTPUT(0);
	}	

}

void Fiat_talento_2017::updateSpeedStatus(CanRMsg *pmsg){}
void Fiat_talento_2017::updateTVFKeyStatus(CanRMsg *pmsg){}
bool Fiat_talento_2017::filt(CanRMsg *pmsg){}



