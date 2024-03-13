#include <FDCAN_Flash_device.h>
#include "Bsp.h"

uint8_t isCommandID;
uint8_t isSelectID;
uint8_t TxData[64];
FDCAN_TxHeaderTypeDef TxHeader;
FDCanRxHeader header;

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

	isCommandID = 1;

	TxHeader.Identifier = 0x111;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_64;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader.FDFormat = FDCAN_FD_CAN;
	TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker       = 0;
}

void FDCAN_TxConfig(FDCAN_TxHeaderTypeDef *header)
{
	header->Identifier = 0x111;
	header->IdType = FDCAN_STANDARD_ID;
	header->TxFrameType = FDCAN_DATA_FRAME;
	header->DataLength = FDCAN_DLC_BYTES_64;
	header->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	header->BitRateSwitch = FDCAN_BRS_ON;
	header->FDFormat = FDCAN_FD_CAN;
	header->TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
	header->MessageMarker       = 0;
}

uint8_t FDCAN_ReadByte(void)
{
	/* check if FIFO 0 receive at least one message */
	while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) < 1);

	/* Retrieve Rx messages from RX FIFO0 */
	HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &(header.RxHeader), header.data);

	return *(header.data);
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

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)			//接收中断回调函数重写
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
	  /* Retrieve Rx messages from RX FIFO0 */
		if(isCommandID == 1)
		{
			isSelectID = 1;
		}
		else
		{
			HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &(header.RxHeader), header.data);
		}
	}
}

void FDCAN_GetCommand_d(void)
{
	uint32_t counter;
	const uint8_t a_FDCAN_CommandsList[OPENBL_FDCAN_COMMANDS_NB] =
	{
		GET,
		GETVE,
		GETID,
		READ,
		GO,
		WRITE,
		ERASE,
		WREN,
		WRDS,
		RDEN,
		RDDS
	};

	FDCAN_SendByte(ACK_BYTE);
	/*Send 1 message: number of bytes*/
	FDCAN_SendByte(OPENBL_FDCAN_COMMANDS_NB);
	/*Send 1 message: protocol version*/
	FDCAN_SendByte(0x01);
	/*Send 12 messages (message by message): command opcodes */
	for(int i=0;i<OPENBL_FDCAN_COMMANDS_NB;i++)
	{
		FDCAN_SendByte(a_FDCAN_CommandsList[i]);
	}

	FDCAN_SendByte(ACK_BYTE);
}

void FDCAN_GetID_d(void)
{
	FDCAN_SendByte(ACK_BYTE);

	TxData[0] = DEVICE_ID_MSB;
	TxData[1] = DEVICE_ID_LSB;
	//STM32 product ID
	FDCAN_SendBytes(TxData,FDCAN_DLC_BYTES_2);

	FDCAN_SendByte(ACK_BYTE);
}

void FDCAN_ReadMemory_d(void)
{
	uint32_t address;
	uint32_t counter;
	uint32_t number_of_bytes;
	uint32_t count;
	uint32_t single;
	uint8_t  data_length;

	/* Check memory protection then send adequate response */
	if (GetReadOutProtectionStatus() != RESET)
	{
		FDCAN_SendByte(NACK_BYTE);
	}
	else
	{
		if (FDCAN_GetAddress(&address) == NACK_BYTE)
		{
			FDCAN_SendByte(NACK_BYTE);
		}
		else
		{
			FDCAN_SendByte(ACK_BYTE);

			/* Get the number of bytes to be read from memory (Max: data + 1 = 256) */
			number_of_bytes = (uint32_t)(header.data[4]) + 1U;
			count  = number_of_bytes / 64U;
			single = (uint32_t)(number_of_bytes % 64U);

			while (count != 0U)
			{
				data_length = 0;
				for (counter = 64U ; counter > 0U; counter--)
				{
					TxData[data_length] = FLASH_Read(address);

					data_length++;
					address++;
				}
				FDCAN_SendBytes(TxData, FDCAN_DLC_BYTES_64);
				count--;
			}

			if (single != 0U)
			{
				data_length = 0U;
				for (counter = single ; counter > 0U; counter--)
				{
					TxData[data_length] = FLASH_Read(address);

					data_length++;
					address++;
				}
				/* Fill the rest of the buffer with 0xFF */
				for (counter = (64U - single) ; counter > 0U; counter--)
				{
					TxData[data_length] = 0xFF;
					data_length++;
				}
				FDCAN_SendBytes(TxData, FDCAN_DLC_BYTES_64);
			}
			FDCAN_SendByte(ACK_BYTE);
		}
	}
}

uint8_t FDCAN_GetAddress(uint32_t *Address)
{
	uint8_t status;

	*Address = (((((uint32_t) header.data[0]) << 24)  |
			   (((uint32_t) header.data[1]) << 16)  |
			   (((uint32_t) header.data[2]) << 8)   |
			   (((uint32_t) header.data[3]))));

	/* Check if received address is valid or not */
	if ((*Address >= 0x08000000) && (*Address <= 0x08020000))
	{
		status = ACK_BYTE;
	}
	else
	{
		status = NACK_BYTE;
	}

	return status;
}

