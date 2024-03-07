
/*
 * RoadPassion CONFIDENTIAL
 *
 * Copyright 2023 RoadPassion Electronics Co., Ltd.
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

#ifndef BUFFERHANDLE_H_
#define BUFFERHANDLE_H_

#include "RingBuffer.h"
#include "fdcan.h"

// 带时间戳的CAN消息
typedef struct
{
	FDCAN_HandleTypeDef fdcanx;
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t data[64];
//	uint8_t data[8];
} FDCanRxHeader;

typedef struct
{
	uint16_t BaudRateValue;
	uint8_t  FDCAN_SJW;
	uint8_t  FDCAN_BS1;
	uint8_t  FDCAN_BS2;
	uint16_t FDCAN_Prescaler;
} BaudRateTypeDef;

#define RING_BUFFER_SIZE	200

class BufferHandle {
public:
	typedef void (*FDCanRxMonitorCB_T)(void *param);
	typedef RingBuffer<FDCanRxHeader, RING_BUFFER_SIZE> RingReceiveBufferT;

	BufferHandle();
	virtual ~BufferHandle();
	int init(void);

	int setBaudrate(FDCAN_HandleTypeDef hfdcan,int br);
	int setBaudratefd(FDCAN_HandleTypeDef hfdcan,int br, int br_d);
	int baudRateIndex(const uint16_t br);

	void setFDCanRxMonitorCB(FDCanRxMonitorCB_T cb, void *param)
	{
		_fdcanRxMonitorCB = cb;
		_fdcanRxMonitorCBParam = param;
	}

	void clearBuffer(void);
	uint8_t getData(FDCAN_HandleTypeDef hfdcan, FDCanRxHeader *pheader);
	void FDCAN_RxData(FDCAN_HandleTypeDef *hfdcan);

    RingReceiveBufferT	 *pFDCAN1RxBuffer;		/* FDCAN1 RX Buffer */
    RingReceiveBufferT	 *pFDCAN2RxBuffer;

#ifdef ZDEBUG
	void showBufferInfo(void)
	{
		pCAN1RxBuffer->showRingBufferInfo();
		pCAN2RxBuffer->showRingBufferInfo();
	}
#endif
private:
	int baudRateNr;
	FDCanRxMonitorCB_T _fdcanRxMonitorCB;
	void *_fdcanRxMonitorCBParam;
};

#endif /* BUFFERHANDLE_H_ */
