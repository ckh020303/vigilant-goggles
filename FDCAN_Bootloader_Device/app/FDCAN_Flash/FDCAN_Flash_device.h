#ifndef __FDCAN_FLASH_DEVICE_H_
#define __FDCAN_FLASH_DEVICE_H_

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
#define DEVICE_ID_MSB                     0x04              /* MSB byte of device ID */
#define DEVICE_ID_LSB                     0x67              /* LSB byte of device ID */
#define OPENBL_FDCAN_COMMANDS_NB             11U    /* Number of supported commands */

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
#define NACK_BYTE		0x1F

extern uint8_t isCommandID;
extern uint8_t isSelectID;
extern FDCAN_TxHeaderTypeDef TxHeader;
extern FDCanRxHeader header;

void FDCAN_Config(void);
void FDCAN_Enable(void);
void FDCAN_TxConfig(FDCAN_TxHeaderTypeDef *header);

void FDCAN_SendByte(uint8_t byte);
void FDCAN_SendBytes(uint8_t *Buffer, uint32_t BufferSize);
uint8_t FDCAN_ReadByte(void);
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

void FDCAN_GetCommand_d(void);
void FDCAN_GetID_d(void);
void FDCAN_ReadMemory_d(void);
void FDCAN_WriteMemory_d(void);
void FDCAN_EraseMemory_d(void);

uint8_t FDCAN_GetAddress(uint32_t *Address);

//void FDCAN_FLASH_SectorErase(uint32_t SectorAddr);
//void FDCAN_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
//void FDCAN_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
//void FDCAN_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
//
//uint8_t FDCAN_FLASH_ReadByte(void);
//uint8_t FDCAN_FLASH_SendByte(uint8_t byte);
//
//void FDCAN_FLASH_WriteEnable(void);
//void FDCAN_FLASH_WaitForWriteEnd(void);

#endif
