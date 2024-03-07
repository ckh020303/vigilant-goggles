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

#ifndef Landrover_Defender_H_
#define Landrover_Defender_H_

#include <Car/Car.h>


class Landrover_defender : public Car {
public:
    Landrover_defender();
    ~Landrover_defender();
    bool init(void);
	void run(CanRMsg *pmsg);
	bool filt(CanRMsg *pmsg);
	void updateSpeedStatus(CanRMsg *pmsg);
	void updateTVFKeyStatus(CanRMsg *pmsg);
	void outPutController(void);
private:
    void _updateINDICATOR(CanRMsg *pmsg);
    void _updateREVERSE(CanRMsg *pmsg);
    void _updateSPEED(CanRMsg *pmsg);
    void _updateHANDBRAKE(CanRMsg *pmsg);
    void _updateIGNITION(CanRMsg *pmsg);

    enum {
    	ON=true,
    	OFF=false,
    };


    bool LEFT_INDICATOR_status;
    bool RIGHT_INDICATOR_status;
    bool REVERSE_status;
    bool HANDBRAKE_status;
    bool IGNITION_status;

};

#endif

