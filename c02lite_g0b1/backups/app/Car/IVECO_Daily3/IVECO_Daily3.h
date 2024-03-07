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

#ifndef IVECO_DAILY3_H_
#define IVECO_DAILY3_H_

#include <Car/Car.h>


class IVECO_Daily3 : public Car {
public:
    IVECO_Daily3();
    ~IVECO_Daily3();
    bool init(void);
	void run(CanRMsg *pmsg);
	bool filt(CanRMsg *pmsg);
	void updateSpeedStatus(CanRMsg *pmsg);
	void updateTVFKeyStatus(CanRMsg *pmsg);
	void outPutController(void);
private:
    void _updateLights(CanRMsg *pmsg);
    void _updateSeatBelt(CanRMsg *pmsg);
    void _updateSpeed(CanRMsg *pmsg);
    void _updateHandbrake(CanRMsg *pmsg);
    void _updateHandbrakeDoorCTL(CanRMsg *pmsg);
    void _updateIgnition(CanRMsg *pmsg);
    bool _isDoorOpen(void){ return (doors_status&0x01);};
    
    enum {
    	ON=true,
    	OFF=false,
    };

	enum {
		IGNITION_OFF_DOOR_CLOSE = 0x00,
		IGNITION_OFF_DOOR_OPEN,
		
		IGNITION_ON_DOOR_CLOSE = 0x80,
		IGNITION_ON_DOOR_OPEN,
	};

    bool handbrake_status;
    bool seatbelt_status;
    bool ignition_status1;
    bool lights_status;
    bool ignition_status;

    u8 doors_status;


};

#endif

