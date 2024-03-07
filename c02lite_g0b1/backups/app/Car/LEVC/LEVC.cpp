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

#include "LEVC.h"
#include "LEVC_common.h"

using namespace LEVc;

LEVC::LEVC(){
    name = "LEVC";
    const static uint32_t idList[] = {
    	SPEED_MAG_ID,
		HANDBRAKE_MAG_ID,
		SEAT_BELT_MAG_ID,
		IGNITION_MAG_ID,
		REVERSE_MAG_ID,
		0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;
	SEAT_BELT = false;
	handbrake_status = false;
	reverse_status = false;
	ignition_status = false;
	Det1_status = false;
}

LEVC::~LEVC() {
}

bool LEVC::init(void)
{
	return 0;
}

void LEVC::run(CanRMsg *pmsg)
{
	if(pmsg->msg.IDT == CAN_ID_STD) {
			switch(pmsg->msg.StdId) {
				case SPEED_MAG_ID:
					_updateSpeed(pmsg);
					break;	
				case HANDBRAKE_MAG_ID:
					_updateHandbrake(pmsg);
					break;
				case SEAT_BELT_MAG_ID:
					_updateSEAT_BELT(pmsg);
					break;	
				case IGNITION_MAG_ID:
					_updateIgnitionstatus(pmsg);
					break;
				case REVERSE_MAG_ID:
					_updateREVERSEstatus(pmsg);
					break;
			}
    } 
	outPutController();   
}

void LEVC::_updateSEAT_BELT(CanRMsg *pmsg)
{

	SEAT_BELT = pmsg->msg.Data[6] == 0x03?ON:OFF;
	
}

void LEVC::_updateREVERSEstatus(CanRMsg *pmsg)
{

	reverse_status = pmsg->msg.Data[6] == 0x07?ON:OFF;
	
}

void LEVC::_updateIgnitionstatus(CanRMsg *pmsg)
{   

	ignition_status = pmsg->msg.Data[0]&0x08?ON:OFF;

}

void LEVC::_updateSpeed(CanRMsg *pmsg)
{
	speedValue = pmsg->msg.Data[4];
}

void LEVC::_updateHandbrake(CanRMsg *pmsg)
{
	handbrake_status = pmsg->msg.Data[0]&0x80?ON:OFF;
}

void LEVC::outPutController(void)
{

  	Det1_status = gpDrvDel1->getValue()?ON:OFF; 

	/* OUTPUT1 */
	if(Det1_status)
		ENABLE_AV1_OUTPUT(ignition_status && !handbrake_status);
	else
		ENABLE_AV1_OUTPUT(0);

	/* OUTPUT2 */
	ENABLE_AV2_OUTPUT(reverse_status);
	
	/* OUTPUT3 */
	if(speedValue <= 0xa)
		ENABLE_AV3_OUTPUT(!handbrake_status);
	else
		ENABLE_AV3_OUTPUT(0);
		
	/* OUTPUT4 */
	if(speedValue >= 0x5)
		ENABLE_AV4_OUTPUT(SEAT_BELT);
	else
		ENABLE_AV4_OUTPUT(0);


	/* OUTPUT5 */
	ENABLE_AV5_OUTPUT(ignition_status);

}


void LEVC::updateSpeedStatus(CanRMsg *pmsg)
{
}

void LEVC::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool LEVC::filt(CanRMsg *pmsg)
{
}



