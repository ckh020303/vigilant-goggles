#ifndef __FDCAN_FLASH_HOST_H_
#define __FDCAN_FLASH_HOST_H_

#include "gpio.h"
#include "usart.h"
#include "fdcan.h"

#include "main.h"

typedef struct
{
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t data[64];
} FDCanRxHeader;

#define FDCAN_FLASH_PageSize 256

extern uint8_t rx_flag;

#define GET			0xFFFF00
#define GETVE		0xFFFF01
#define GETID		0xFFFF02

#define READ 		0xFFFF11
#define GO			0xFFFF21
#define WRITE		0xFFFF31
#define ERASE		0xFFFF44

#define WREN		0xFFFF63
#define WRDS		0xFFFF73
#define RDEN		0xFFFF82
#define RDDS		0xFFFF92

#define ACK_BYTE			0x79
#define NACK_BYTE			0x1F

void FDCAN_Config(void);
void FDCAN_Enable(void);
void FDCAN_TxConfig(void);

void FDCAN_SendByte(uint8_t byte);
void FDCAN_SendBytes(uint8_t *Buffer, uint32_t BufferSize);
uint8_t FDCAN_ReadByte(void);
void FDCAN_ReadBytes(uint8_t *Buffer, uint32_t BufferSize);
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

void FDCAN_GetCommand(void);
void FDCAN_GetID(void);
void FDCAN_ReadMemory(void);
void FDCAN_WriteMemory(uint8_t *address, uint8_t *data);
void FDCAN_EraseMemory(uint8_t *txdata, uint8_t *data);
void FDCAN_Go(void);

void FDCAN_FLASH_SectorErase(uint32_t SectorAddr);
void FDCAN_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void FDCAN_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void FDCAN_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

uint8_t FDCAN_FLASH_ReadByte(void);
uint8_t FDCAN_FLASH_SendByte(uint8_t byte);

void FDCAN_FLASH_WriteEnable(void);
void FDCAN_FLASH_WaitForWriteEnd(void);

#endif
