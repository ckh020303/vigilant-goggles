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

#define GET			0x00
#define GETVE		0x01
#define GETID		0x02

#define READ 		0x11
#define GO			0x21
#define WRITE		0x31
#define ERASE		0x44

#define WREN		0x63
#define WRDS		0x73
#define RDEN		0x82
#define RDDS		0x92

#define ACK_BYTE			0x79
#define NACK_BYTE			0x1F

void FDCAN_Config(void);
void FDCAN_Enable(void);
void FDCAN_TxConfig(FDCAN_TxHeaderTypeDef *header);

void FDCAN_SendByte(uint8_t byte);
void FDCAN_SendBytes(uint8_t *Buffer, uint32_t BufferSize);
uint8_t FDCAN_ReadByte(void);
void FDCAN_ReadBytes(uint8_t *Buffer, uint32_t BufferSize);
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

void FDCAN_GetCommand(void);
void FDCAN_GetID(void);

void FDCAN_FLASH_SectorErase(uint32_t SectorAddr);
void FDCAN_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void FDCAN_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void FDCAN_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

uint8_t FDCAN_FLASH_ReadByte(void);
uint8_t FDCAN_FLASH_SendByte(uint8_t byte);

void FDCAN_FLASH_WriteEnable(void);
void FDCAN_FLASH_WaitForWriteEnd(void);

#endif
