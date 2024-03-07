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

#ifndef IVECO_YELLOW_H_
#define IVECO_YELLOW_H_

#include <Car/Car.h>

class Iveco_Yellow : public Car {
public:
    Iveco_Yellow();
    ~Iveco_Yellow();
    bool init(void);
	void run(CanRMsg *pmsg);
	bool filt(CanRMsg *pmsg);
	void updateSpeedStatus(CanRMsg *pmsg);
	void updateTVFKeyStatus(CanRMsg *pmsg);
private:
    void _updateShiftPos(CanRMsg *pmsg);
    void _updateIgnition(CanRMsg *pmsg);
    void _updateSpeed(CanRMsg *pmsg);
    void _updateHandbrake(CanRMsg *pmsg);

//    void _doForward(CanRMsg *pcanRMsg);
    
    void transfer(void);

    u8 preIgnition;
	u8 curIgnition;

	u8 preReverse;
    u8 curReverse;

	bool handbrake_status;

	
    CanRMsg Rmsg;
};

#endif

