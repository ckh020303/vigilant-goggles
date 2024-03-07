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

#ifndef NISSAN_H_
#define NISSAN_H_

#include <Car/Car.h>

class Nissan  : public Car {
public:
    Nissan();
    ~Nissan();
    bool init(void);
	void run(CanRMsg *pmsg);
	
	bool filt(CanRMsg *pmsg);
	void updateSpeedStatus(CanRMsg *pmsg);
	void updateTVFKeyStatus(CanRMsg *pmsg);
private:
    void _updateIgnition(CanRMsg *pmsg);
    void _updateSpeed(CanRMsg *pmsg);
    void _updateHandbrake(CanRMsg *pmsg);

    void HandleOutput(u8 outputNum,bool status);

    enum {
    	ON=true,
    	OFF=false,
    };

    enum{
        SPEED_0_5MPH = 1,
        SPEED_5_10MPH,
        SPEED_10_20MPH,
        SPEED_20_30MPH,

        SPEED_OTHER,
    };
        

	bool handbrake_status;
    bool ignition_status;
};

#endif

