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

#ifndef SPRINTER_PSM_H_
#define SPRINTER_PSM_H_

#include <Car/Car.h>

class Sprinter_Psm : public Car {
public:
    Sprinter_Psm();
    ~Sprinter_Psm();
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
    void _updateIndicator(CanRMsg *pmsg);

    u8 preIgnition;
	u8 curIgnition;

	u8 preReverse;
    u8 curReverse;

	bool handbrake_status;
	bool Indicator_status;
};

#endif

