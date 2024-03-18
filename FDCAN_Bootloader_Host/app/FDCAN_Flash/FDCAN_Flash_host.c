#include <FDCAN_Flash_host.h>
#include <string.h>

uint8_t rx_flag;
FDCAN_TxHeaderTypeDef TxHeader;
FDCanRxHeader _Buffer[400];
int size = 400;
int validCount;
int head,tail;

void FDCAN_Config(void)
{
	hfdcan_filter.IdType = FDCAN_STANDARD_ID;
	hfdcan_filter.FilterIndex = 0;
	hfdcan_filter.FilterType = FDCAN_FILTER_MASK;
	hfdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	hfdcan_filter.FilterID1 = 0x111;
	hfdcan_filter.FilterID2 = 0x7FF;

	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &hfdcan_filter) != HAL_OK)
	{
		Error_Handler();
	}

	/* Configure and enable Tx Delay Compensation, required for BRS mode.
		 TdcOffset default recommended value: DataTimeSeg1 * DataPrescaler
		 TdcFilter default recommended value: 0 */
	if (HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan1, hfdcan1.Init.DataPrescaler * hfdcan1.Init.DataTimeSeg1, 0) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_FDCAN_EnableTxDelayCompensation(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}
}

void FDCAN_Enable(void)
{
	FDCAN_Config();
	enterNormalMode();											//CAN收发器正常模�?????
	HAL_Delay(5);

	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0) != HAL_OK)		//使能FIFO0新消息中�?????
	{
		Error_Handler();
	}

	if (HAL_FDCAN_Start(&hfdcan1)!= HAL_OK)														//使能FDCAN
	{
		Error_Handler();
	}

	TxHeader.Identifier = 0x111;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_64;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader.FDFormat = FDCAN_FD_CAN;
	TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker       = 0;

	head = tail = 0;
	validCount = 0;
}

void FDCAN_TxConfig(void)
{
	TxHeader.Identifier = 0x111;
}

void FDCAN_SendByte(uint8_t byte)
{
	TxHeader.DataLength = FDCAN_DLC_BYTES_1;
	while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) == 0);

	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1,&TxHeader,&byte);

	/* Wait that the data is completely sent (sent FIFO empty) */
	while (((&hfdcan1)->Instance->IR & FDCAN_IR_TFE) != FDCAN_IR_TFE );

	/* Clear the complete flag */
	(&hfdcan1)->Instance->IR &= FDCAN_IR_TFE;

	HAL_Delay(1);
}

void FDCAN_SendBytes(uint8_t *Buffer, uint32_t BufferSize)
{
	TxHeader.DataLength = BufferSize;

	while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) == 0);

	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, Buffer);

	/* Wait that the data is completely sent (sent FIFO empty) */
	while (((&hfdcan1)->Instance->IR & FDCAN_IR_TFE) != FDCAN_IR_TFE);

	/* Clear the complete flag */
	(&hfdcan1)->Instance->IR &= FDCAN_IR_TFE;

	HAL_Delay(1);
}

uint8_t FDCAN_ReadByte(void)
{
	FDCanRxHeader header;
	int err;

	err = read(&header);
	if(err){
		HAL_Delay(1);
		return header.data[0];
	}
	else
	{
		printf("Buffer is Empty\n");
	}
}

void FDCAN_ReadBytes(uint8_t *Buffer, uint32_t BufferSize)
{
	FDCanRxHeader header;
	int err;

	err = read(&header);
	if(err){
		memcpy(Buffer,header.data,sizeof(header.data));
		HAL_Delay(1);
	}
	else
	{
		printf("Buffer is Empty\n");
	}
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)			//接收中断回调函数重写
{
	FDCanRxHeader header;
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
//		rx_flag = 1;
		/* Retrieve Rx messages from RX FIFO0 */
		HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &(header.RxHeader), header.data);
		/* write buffer */
		write(&header);
	}
}

int getNextPos(int cur)
{
	return (cur+1) == size ? 0 : cur+1;
}

int write(FDCanRxHeader *content)
{
	int ret = 1;
	if (validCount < size) {
		_Buffer[tail] = *content;

		tail = getNextPos(tail);
		validCount++;
	} else {
		/* Buffer is FULL */
		ret = 0;
	}
	return ret;
}

int read(FDCanRxHeader* buf)
{
	int ret = 1;
	if (validCount > 0) {
		*buf = _Buffer[head];

		head = getNextPos(head);
		validCount--;
	} else {
		/* Buffer is Empty */
		ret = 0;
	}
	return ret;
}

void FDCAN_GetCommand(void)
{
	uint8_t num,  version, commandlist[12];
	TxHeader.Identifier = GET;

	FDCAN_SendByte(0x00);
	HAL_Delay(10);
	if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
	{
		num = FDCAN_ReadByte();
		version = FDCAN_ReadByte();
		printf("num:%d,version:%d\n",num,version);
		printf("GetCommand:\n");

		for(int i=0;i<num;i++)
		{
			commandlist[i] = FDCAN_ReadByte();
			printf("0x%x\n",commandlist[i]);
		}

		if((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
		{
			printf("GetCommand Success!\n");
		}
	}
	else if((FDCAN_ReadByte() & NACK_BYTE) == NACK_BYTE)
	{
		printf("GetCommand fail!\n");
	}
	else
	{
		printf("error\n");
	}
}

void FDCAN_GetID(void)
{
	uint8_t data[64];
	TxHeader.Identifier = GETID;

	FDCAN_SendByte(0x00);
	HAL_Delay(10);
	if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
	{
		FDCAN_ReadBytes(data,FDCAN_DLC_BYTES_2);
		printf("GetID:0x%x%x\n",*data,*(data+1));
		if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
		{
			printf("GetID Success!\n");
		}
	}
	else if((FDCAN_ReadByte() & NACK_BYTE) == NACK_BYTE)
	{
		printf("GetID fail!\n");
	}
	else
	{
		printf("error\n");
	}
}

void FDCAN_ReadMemory(void)
{
	uint8_t data[64];
	uint8_t txdata[5] = {0x08,0x01,0x00,0x00,0xFF};
	TxHeader.Identifier = READ;

	FDCAN_SendBytes(txdata,FDCAN_DLC_BYTES_5);
	HAL_Delay(10);
	if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
	{
		printf("ReadMemoryData:\n");
		for(int i=0;i<4;i++)
		{
			FDCAN_ReadBytes(data,FDCAN_DLC_BYTES_64);
			for (int j=0;j<64;j++)
			{
				printf("%x ",data[j]);
			}
			printf("\n");
		}

		if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
		{
			printf("ReadMemory Success!\n");
		}
	}
	else if((FDCAN_ReadByte() & NACK_BYTE) == NACK_BYTE)
	{
		printf("ReadMemory fail!\n");
	}
	else
	{
		printf("error\n");
	}
}

void FDCAN_WriteMemory(uint8_t *address, uint8_t *data)
{
//	uint32_t address;
//	uint8_t txdata[5] = {0x08,0x01,0x00,0x00,0xFF};
	TxHeader.Identifier = WRITE;

//	address = 0x08000000;

	FDCAN_SendBytes(address,FDCAN_DLC_BYTES_5);
	FDCAN_TxConfig();
	HAL_Delay(10);
	if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
	{
		for (int i=0;i<4;i++)
		{
//			printf("WriteMemoryData:\n");
//			for (int count = 0 ; count > 64U; count++)
//			{
//				data[count] = FLASH_Read(address++);
//				printf("%x ",data[count]);
//			}
//			printf("\n");
			FDCAN_SendBytes((data+i*64),FDCAN_DLC_BYTES_64);
		}

		HAL_Delay(10);
		if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
		{
			printf("WriteMemory Success!\n");
		}
	}
	else if ((FDCAN_ReadByte() & NACK_BYTE) == NACK_BYTE)
	{
		printf("WriteMemory fail!\n");
	}
	else
	{
		printf("error\n");
	}
}

void FDCAN_EraseMemory(uint8_t *txdata, uint8_t *data)
{
	TxHeader.Identifier = ERASE;

	FDCAN_SendBytes(txdata,FDCAN_DLC_BYTES_2);
	FDCAN_TxConfig();
	HAL_Delay(10);
	if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
	{
		FDCAN_SendBytes(data,FDCAN_DLC_BYTES_64);

		HAL_Delay(100);
		if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
		{
			printf("EraseMemory Success!\n");
		}
	}
	else if ((FDCAN_ReadByte() & NACK_BYTE) == NACK_BYTE)
	{
		printf("WriteMemory fail!\n");
	}
	else
	{
		printf("error\n");
	}
}

void FDCAN_Go(void)
{
	uint8_t txdata[4] = {0x08,0x01,0x00,0x00};
	TxHeader.Identifier = GO;

	FDCAN_SendBytes(txdata,FDCAN_DLC_BYTES_4);
	HAL_Delay(10);
	if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
	{
		HAL_Delay(10);
		if ((FDCAN_ReadByte() & ACK_BYTE) == ACK_BYTE)
		{
			printf("Go Success!\n");
		}
	}
	else if ((FDCAN_ReadByte() & NACK_BYTE) == NACK_BYTE)
	{
		printf("Go fail!\n");
	}
	else
	{
		printf("error\n");
	}
}


