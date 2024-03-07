

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

#ifndef LDV_MAXUS_2021_6_H_
#define LDV_MAXUS_2021_6_H_

#include <Car/Car.h>



class LDV_Maxus_2021_6 : public Car {
public:
    LDV_Maxus_2021_6();
    ~LDV_Maxus_2021_6();
    bool init(void);
	void run(CanRMsg *pmsg);
	bool filt(CanRMsg *pmsg);
	void updateSpeedStatus(CanRMsg *pmsg);
	void updateTVFKeyStatus(CanRMsg *pmsg);
	void outPutController(void);
private:
	void _updateSpeed(CanRMsg *pmsg);
	void _updateHandbrake(CanRMsg *pmsg);
   void _updateIndicatorstatus(CanRMsg *pmsg);
   void _updateIgnitionstatus(CanRMsg *pmsg);
   void _A_status(CanRMsg *pmsg);   
    

    
    enum {
		ON=true,
		OFF=false,
    };
	bool ignition_status;
	bool output1;
	bool output2;
	bool output4;
	bool _status;
	bool handbrake_status;
    CanRMsg Rmsg;
};

#endif

