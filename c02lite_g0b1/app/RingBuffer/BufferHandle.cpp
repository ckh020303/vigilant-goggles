/*
* RoadPassion CONFIDENTIAL
*
* Copyright 2023 RoadPassion Electronics Technology Co., Ltd.
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

#include "BufferHandle.h"
#include "CanHub.h"

FDCanRxHeader _FDCan1RxHeader,_FDCan2RxHeader;

BaudRateTypeDef baudRate[] = {
	{ 4000, 3, 12, 3, 1 },
	{ 2000, 3, 12, 3, 2 },
	{ 1000, 3, 12, 3, 4 },	//baudRate 1000k
	{  800, 4, 15, 4, 4 },	//800k
	{  666, 5, 17, 6, 4 },	//666k
	{  500, 3, 12, 3, 8 },	//500k				采样点：12+1/12+1+3=81.25%
	{  400, 4, 15, 4, 8 },	//400k				一般在70%~85%之间
	{  250, 3, 12, 3, 16 },	//250k
	{  200, 4, 15, 4, 16 },	//200k
	{  125, 3, 12, 3, 32 },	//125k
	{  100, 4, 15, 4, 32 }	//100k
};

BufferHandle::BufferHandle(){
	_fdcanRxMonitorCB = NULL;
}

BufferHandle::~BufferHandle(){
}

int BufferHandle::init(void)
{
	static RingBuffer<FDCanRxHeader, RING_BUFFER_SIZE>	_fdcan1RxBuffer;
	static RingBuffer<FDCanRxHeader, RING_BUFFER_SIZE>	_fdcan2RxBuffer;
	pFDCAN1RxBuffer = &_fdcan1RxBuffer;
	pFDCAN2RxBuffer = &_fdcan2RxBuffer;

	baudRateNr = sizeof(baudRate)/sizeof(BaudRateTypeDef);

	return 0;
}

void BufferHandle::clearBuffer(void)
{
	pFDCAN1RxBuffer->clear();
	pFDCAN2RxBuffer->clear();
}

int BufferHandle::setBaudrate(FDCAN_HandleTypeDef hfdcan,int br)
{
	int br_index = baudRateIndex(br);

	if (br_index < 0 || br_index >= baudRateNr)
	{
		printf("error baudRate!\n");
		return -1;
	}
	else
	{
		printf("baudRateNr:%d\n",br_index);
	}

	hfdcan.Init.NominalPrescaler = baudRate[br_index].FDCAN_Prescaler;
	hfdcan.Init.NominalTimeSeg1 = baudRate[br_index].FDCAN_BS1;
	hfdcan.Init.NominalTimeSeg2 = baudRate[br_index].FDCAN_BS2;
	hfdcan.Init.NominalSyncJumpWidth = baudRate[br_index].FDCAN_SJW;

	if (HAL_FDCAN_Init(&hfdcan) != HAL_OK)
	{
		Error_Handler();
	}

	hfdcan_filter.IdType = FDCAN_STANDARD_ID;
	hfdcan_filter.FilterIndex = 0;
	hfdcan_filter.FilterType = FDCAN_FILTER_MASK;
	hfdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	hfdcan_filter.FilterID1 = 0x000;
	hfdcan_filter.FilterID2 = 0x000;

	if (HAL_FDCAN_ConfigFilter(&hfdcan, &hfdcan_filter) != HAL_OK)
	{
		Error_Handler();
	}

//	Configure the FDCAN global filter
//	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
//	{
//		Error_Handler();
//	}

	/* Configure and enable Tx Delay Compensation, required for BRS mode.
	     TdcOffset default recommended value: DataTimeSeg1 * DataPrescaler
	     TdcFilter default recommended value: 0 */
	if (HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan, hfdcan.Init.DataPrescaler * hfdcan.Init.DataTimeSeg1, 0) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_EnableTxDelayCompensation(&hfdcan) != HAL_OK)
	{
		Error_Handler();
	}
}

int BufferHandle::setBaudratefd(FDCAN_HandleTypeDef hfdcan,int br, int br_d)
{
	int br_index = baudRateIndex(br);
	int br_data = baudRateIndex(br_d);

	if ((br_index < 0 || br_index >= baudRateNr) && (br_data < 0 || br_data >= baudRateNr))
	{
		printf("error baudRate!\n");
		return -1;
	}
	else
	{
		printf("baudRateNr1:%d\n",br_index);
		printf("baudRateNr2:%d\n",br_data);
	}

	hfdcan.Init.NominalPrescaler = baudRate[br_index].FDCAN_Prescaler;
	hfdcan.Init.NominalTimeSeg1 = baudRate[br_index].FDCAN_BS1;
	hfdcan.Init.NominalTimeSeg2 = baudRate[br_index].FDCAN_BS2;
	hfdcan.Init.NominalSyncJumpWidth = baudRate[br_index].FDCAN_SJW;

	hfdcan.Init.DataPrescaler = baudRate[br_data].FDCAN_Prescaler;
	hfdcan.Init.DataTimeSeg1 = baudRate[br_data].FDCAN_BS1;
	hfdcan.Init.DataTimeSeg2 = baudRate[br_data].FDCAN_BS2;
	hfdcan.Init.DataSyncJumpWidth = baudRate[br_data].FDCAN_SJW;

	if (HAL_FDCAN_Init(&hfdcan) != HAL_OK)
	{
		Error_Handler();
	}

	hfdcan_filter.IdType = FDCAN_STANDARD_ID;
	hfdcan_filter.FilterIndex = 0;
	hfdcan_filter.FilterType = FDCAN_FILTER_MASK;
	hfdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	hfdcan_filter.FilterID1 = 0x000;
	hfdcan_filter.FilterID2 = 0x000;

	if (HAL_FDCAN_ConfigFilter(&hfdcan, &hfdcan_filter) != HAL_OK)
	{
		Error_Handler();
	}

//	Configure the FDCAN global filter
//	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
//	{
//		Error_Handler();
//	}

	/* Configure and enable Tx Delay Compensation, required for BRS mode.
	     TdcOffset default recommended value: DataTimeSeg1 * DataPrescaler
	     TdcFilter default recommended value: 0 */
	if (HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan, hfdcan.Init.DataPrescaler * hfdcan.Init.DataTimeSeg1, 0) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_FDCAN_EnableTxDelayCompensation(&hfdcan) != HAL_OK)
	{
		Error_Handler();
	}
}

int BufferHandle::baudRateIndex(const uint16_t br)
{
	int i;
	for (i = 0; i < baudRateNr; i++) {
		if (baudRate[i].BaudRateValue == br)
			return i;
	}
	return -1;
}

uint8_t BufferHandle::getData(FDCAN_HandleTypeDef hfdcan, FDCanRxHeader *pheader)
{
	if(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan,FDCAN_RX_FIFO0) > 0)
	{
		if (HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &(pheader->RxHeader), pheader->data) == HAL_OK)
		{
			return (pheader->RxHeader).DataLength;
		}
	}
	return 0;
}

void BufferHandle::FDCAN_RxData(FDCAN_HandleTypeDef *hfdcan)
{
	FDCanRxHeader header;
	HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &(header.RxHeader), header.data);
//	printf("receive!\n");
//	for (int i = 0; i < 64; i++){
//		printf("%d ",header.data[i]);
//	}
//	printf("\n");

	if (hfdcan->Instance == hfdcan1.Instance)
	{
		header.fdcanx = hfdcan1;
		_FDCan1RxHeader = header;
		bufferHandle->pFDCAN1RxBuffer->write(&header);
	}
	else
	{
		header.fdcanx = hfdcan2;
		_FDCan2RxHeader = header;
		bufferHandle->pFDCAN2RxBuffer->write(&header);
	}

	if (_fdcanRxMonitorCB) {
		_fdcanRxMonitorCB(_fdcanRxMonitorCBParam);
	}
}
