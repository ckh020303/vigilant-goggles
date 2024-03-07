/*
* RoadPassion CONFIDENTIAL
*
* Copyright 2018 RoadPassion Electronics Technology Co., Ltd.
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
#ifndef CANDETECTOR_H_
#define CANDETECTOR_H_

#include "includes.h"

class CanDetector {
public:
	static void delay_ms(u32 nms){
		u32 i=0;  
		while(nms--){
			i=10100;
			while(i--);    
		}
	}
	static CanDetector *getInstance(void);
	void start(bool needSleep=true);
	void stop(void);


protected:
	virtual void _chipsSleep(void);
	CanDetector(void);
	~CanDetector(void);
private:
	static void _CanRxIndicatorHandler(void *param);
	static void _CanRxIRQMonitorCB(void *param);
	static void _CanRxMonitorTimerHandler(void *param);

	void _gotoSleep(void);

	DrvGPTimer *_pSleepTimer;
	DrvGPTimer *_pRedLEDTimer;

	bool _needSleep;
};


#endif /* CANDETECTOR_H_ */
