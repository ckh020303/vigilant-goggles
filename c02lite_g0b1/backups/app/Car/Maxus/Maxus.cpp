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

#include "Maxus.h"
#include "Maxus_common.h"

using namespace maxus;

Maxus::Maxus(){
    name = "Maxus";
    const static uint32_t idList[] = {
        SWITCH_SYSTEM_MAG_ID,
		0x0,
	};
	pCanFilterIDList = idList;
    baudRate = 500;

    system_status=false;
    preStatus = 0;
    curStatus = 0;
}

Maxus::~Maxus() {
}

bool Maxus::init(void)
{
	return 0;
}

void Maxus::run(CanRMsg *pmsg)
{
    if(gpDrvDel1->getValue()){
        _handleoutput(pmsg);
    }else{
        preStatus = 0;
        curStatus = 0;
    }
        
	    
}


void Maxus::_handleoutput(CanRMsg *pmsg)
{   
    if(pmsg->msg.StdId == SWITCH_SYSTEM_MAG_ID){
        if((pmsg->msg.Data[0]==0x40)/*||(pmsg->msg.Data[1]==0x2)*/){
            curStatus=0;
        }else if((pmsg->msg.Data[0]==0x0)/*||(pmsg->msg.Data[1]==0x0)*/){
            curStatus = 1;
        }
    }

    if(curStatus != preStatus){
        preStatus = curStatus;
        if(curStatus){
            _Output1sec();
        }else{
            
        }
    }

}

void Maxus::_Output1sec(void)
{
    delay_1ms(1000);
    gpDrvSpeedLimit->on();
    delay_1ms(1000);
    gpDrvSpeedLimit->off();
}



void Maxus::updateSpeedStatus(CanRMsg *pmsg)
{
}

void Maxus::updateTVFKeyStatus(CanRMsg *pmsg)
{
}

bool Maxus::filt(CanRMsg *pmsg)
{
}


