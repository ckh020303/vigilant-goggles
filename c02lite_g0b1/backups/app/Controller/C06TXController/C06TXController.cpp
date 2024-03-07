/*
* RoadPassion CONFIDENTIAL
*
* Copyright 2018 RoadPassion Electronics Technology Co., Ltd.
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

/*
 * ASC file format:
 *   Doc\C06TX\CAN_LOG_TRIGGER_ASC_Format.pdf
 *
 */
#include "C06TXController.h"
#include "tff.h"


C06TXController::C06TXController(Car *pCar)
{
    _pCar=pCar;
    _pCar->init();

    gpDrvTJA1041->init();
    
#ifdef C06_V1_2
    // Auto Detect
    isTJA1043 = gpDrvTJAdetect->getValue();

    printf("CAN transceiver type: %s\n",isTJA1043?"TJA1043":"TJA1044");

    if(isTJA1043){
        gpDrvTJA1041->enterPownMode();
    }else{
        gpDrvTJA1041->TJA1044enterNormalMode();
    }
#else
	isTJA1043 = true;
    gpDrvTJA1041->enterPownMode();
#endif

    gpDrvCAN->setBaudrate(CAN1, _pCar->baudRate, CAN_Mode_Normal);

//	gpDrvCAN->addFilters(pCar->pCanFilterIDList);
    gpDrvCAN->start();

    memset(&canTxMsg, 0, sizeof(CanTMsg));
    canTxMsg.CANx 		= CAN1;
    canTxMsg.msg.IDT 	= CAN_Id_Standard;
    canTxMsg.msg.RTR 	= CAN_RTR_Data;

}

C06TXController::~C06TXController(void) {

}

void C06TXController::run(void)
{
	int ret;
	CanRMsg rxMsg;
	CanTMsg txMsg;

	/*
	 * V1.2 Uses StandBy Mode, so we can NOT use IWDG otherwise
	 * MCU will be wake up
	 */
//	if (isTJA1043) { //打开看门狗会导致V1.1硬件不休眠
//		DrvWatchDog::getInstance()->initTheDog(2500);		/* 2.5S */
//		DrvWatchDog::getInstance()->enableTheDog(true);
//	}
	zprintf("Controller: Start\n");

	SPI_SD_Init(&gSpiSDCardDevice);

	txMsg.CANx = CAN1;
	txMsg.msg.StdId = 0x123;
	txMsg.msg.ExtId = 0;
	txMsg.msg.DLC   = 8;
	txMsg.msg.IDT   = CAN_Id_Standard;
	txMsg.msg.RTR   = CAN_RTR_Data;
	txMsg.msg.Data[0] = 0x01;
	txMsg.msg.Data[1] = 0x02;
	txMsg.msg.Data[2] = 0x03;
	txMsg.msg.Data[3] = 0x04;
	txMsg.msg.Data[4] = 0x05;
	txMsg.msg.Data[5] = 0x06;
	txMsg.msg.Data[6] = 0x07;
	txMsg.msg.Data[7] = 0x08;

	loopCnt = 0;

	memset(&dsc, 0, sizeof(struct dataSendContrl_t));

	while(1) {
	    delay_1ms(100);

	    loopCnt++;

	    ret = checkConfigFile();
	    if (ret == 0) {
	    	if (dsc.sendControl == SEND_CTRL_LOOP) {
	    		while (1) {
	    			if (sendDataFile() == -1) {
	    				break;
	    			}
	    			delay_1ms(10);
	    		}
	    	} else {
	    		sendDataFile();
	    	}
	    }

#if 0
	    ret = gpDrvCAN->pCAN1RxBuffer->read(&msg);
	    if (ret) {
	    	printf("RX: %x - %x\n", msg.msg.StdId, msg.msg.DLC);
	    }
#endif

#if 1
		txMsg.msg.Data[0] = loopCnt % 255;
	    gpDrvCAN->transmit(CAN1, &txMsg.msg, true);
#endif
	}

}

int C06TXController::parseCfgOneLine(char *pBuf, int len)
{
	int ret = 0;
	int i = 0;
	int j = 0;

	printf("===> %s\n", pBuf);

	char tempBuf[128];

	if (len <= 1) {
		printf("Skip\n");

		return -1;
	}

	/*
	 * trim the string
	 *   - Remove start Space 0x20
	 *   - Remove start Tab 0x09
	 *   - Remove end '\n'
	 */
	char *pTrimBuf;

	// End of Line \n to 0x0
	pBuf[len-1] = 0x0;

	while ((pBuf[i] == ' ') || (pBuf[i] == '\t')) {
		i++;
	}

	pTrimBuf = &pBuf[i];

	printf("New len = %d -> %d\n", strlen(pBuf), strlen(pTrimBuf));

	if (pTrimBuf[0] == '#') {
		printf("Comment line. Skip\n");
		return 0;
	}

	if (strncmp(pTrimBuf, "baudrate=", 9) == 0) {
		memset(tempBuf, 0, 128);
		while (pTrimBuf[j+9] != 0x0) {
			tempBuf[j] = pTrimBuf[j+9];
			j++;
		}

		dsc.baudRate = atoi(tempBuf);
		printf("Baud Rate = %d\n", dsc.baudRate);

	} else if (strncmp(pTrimBuf, "datafile=", 9) == 0) {
		memset(tempBuf, 0, 128);
		while (pTrimBuf[j+9] != 0x0) {
			tempBuf[j] = pTrimBuf[j+9];
			j++;
		}

		strncpy(dsc.dataFileName, tempBuf, strlen(tempBuf));

		printf("Date File = %s\n", dsc.dataFileName);
	} else if (strncmp(pTrimBuf, "datatype=asc", 12) == 0) {
			dsc.dataFormat = CAN_DATA_FORMAT_ASC;
	} else if (strncmp(pTrimBuf, "datatype=trc", 12) == 0) {
		dsc.dataFormat = CAN_DATA_FORMAT_TRC;
	} else if (strncmp(pTrimBuf, "datatype=can", 12) == 0) {
		dsc.dataFormat = CAN_DATA_FORMAT_CAN;
	} else if (strncmp(pTrimBuf, "sendmode=loop", 13) == 0) {
		dsc.sendControl = SEND_CTRL_LOOP;
	} else if (strncmp(pTrimBuf, "sendmode=oneshot", 16) == 0) {
		dsc.sendControl = SEND_CTRL_ONESHOT;
	}


	ret = strlen(tempBuf);

	return ret;
}

/*
 * Parse ASC Data Line
 * Format:
 *   <Time> <Channel> <ID> <Dir> d <DLC> <D0> <D1>...<D8> <MessageFlags>
 *
 *
 */
int C06TXController::parseAscDataOneLine(char *pBuf, int len)
{
	printf("\nASC: %s\n", pBuf);

	int lineLen = 0;
	char lineBuf[128];

	unsigned char channel, dlc;
	unsigned int id;
	unsigned char data[8];

	memset(lineBuf, 0, 128);

	int i = 0, j = 0;

	// Remove Start ' ' or '\t'
	while ((pBuf[i] == ' ') || (pBuf[i] == '\t')) {
		i++;
	}

	while (i < len) {
		if (pBuf[i] == '\t') {
			lineBuf[j] = ' ';
		} else if (pBuf[i] == '\n') {
			lineBuf[j] = 0x0;
		} else {
			lineBuf[j] = pBuf[i];
		}

		i++;
		j++;
	}

	lineLen = j;

	if (lineLen < 10) {
		return 0;
	}

	if ((lineBuf[0] >= '0') && (lineBuf[0] <= '9')) {
		/*
		 * FIXME
		 *
		 * something wrong the sscanf() to get the all line
		 */
#if 0
		int cnt = sscanf(lineBuf, "%f %d %x %s %c %d %x %x %x %x %x %x %x %x", &timestamp, &channel, &id, strDir, &dFlag, &dlc,
				&data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]);

#endif
		/* 1. Get ID */
		char *pNoTimeStr;
		char *pDlcStr;

		int i = 0;
		while (lineBuf[i] != ' ') {
			i++;
			if ((i+1) >= lineLen) {
				break;
			}
		}

		pNoTimeStr = &lineBuf[i+1];

//		printf("NoTimeStr: %s\n", pNoTimeStr);

		int cnt = sscanf(pNoTimeStr, "%d%x", &channel, &id);
//		printf("cnt = %d ch=%d id=%x \n", cnt, channel, id);


		/* 2. Get DLC and Data */
		int spaceNum = 0;
		int j = 0;
		for (j = 0; j < lineLen; j++) {
			if (lineBuf[j] == ' ') {
				spaceNum++;
			}

			if (spaceNum == 5) {
				break;
			}
		}

		pDlcStr = &lineBuf[j+1];

//		printf("DLC String: %s", pDlcStr);

		int cnt2 = sscanf(pDlcStr, "%d%x%x%x%x%x%x%x%x", &dlc, &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]);

//		printf("cnt2 = %d dlc=%d data: ", cnt2, dlc);

		if (id <= 0x7FF) {
			canTxMsg.msg.StdId = id;
			canTxMsg.msg.ExtId = 0;
			canTxMsg.msg.IDT = CAN_Id_Standard;
		} else {
			canTxMsg.msg.StdId = 0;
			canTxMsg.msg.ExtId = id;
			canTxMsg.msg.IDT = CAN_Id_Extended;
		}

		canTxMsg.msg.DLC = dlc;
		for (int k = 0; k < 8; k++) {
			canTxMsg.msg.Data[k] = data[k];
		}

#if 1
		printf("==> ID: %x DLC=%d Data:", id, dlc);
		for (int k = 0; k < 8; k++) {
			printf("%02x ", data[k]);
		}
		printf("\n");

#endif

	} else if (strncmp(lineBuf, "date", 4)) {
		return 0;
	} else if (strncmp(lineBuf, "base", 4)) {
		return 0;
	} else if (strncmp(lineBuf, "//", 2)) {
		return 0;
	} else {
		return 0;
	}


	return len;
}


/*
 * Parse CAN Data Line
 * Format:
 *   <Time>,<Channel>,<Dir>,<ID>,<DLC> <D0> <D1>...<D8>
 *
 *   - Data is split by SPACE
 *   - DLC is count by data number
 *
 *   000.004.191,0,0,45E,80 10 00 00 00 00 00 08    --> ID: 45E, DLC=8
 *   000.000.107,0,0,222,00 00                      --> ID: 222, DLC=2
 *
 */
int C06TXController::parseCanDataOneLine(char *pBuf, int len)
{
	printf("CAN: %s\n", pBuf);

	int lineLen = 0;
	char lineBuf[128];

	unsigned char dlc;
	unsigned int id;
	unsigned char data[8];

	memset(lineBuf, 0, 128);

	int in = 0, out = 0;

	// Remove Start ' ' or '\t'
	while ((pBuf[in] == ' ') || (pBuf[in] == '\t')) {
		in++;
	}

	while (in < len) {
		if (pBuf[in] == ',') {
			lineBuf[out] = ' ';
		} else if (pBuf[in] == '\n') {
			lineBuf[out] = 0x0;
		} else {
			lineBuf[out] = pBuf[in];
		}

		in++;
		out++;
	}

	lineLen = out;

	if (lineLen < 10) {
		return 0;
	}

	if ((lineBuf[0] >= '0') && (lineBuf[0] <= '9')) {
		char *pFrameStr;
		char *pDataStr;

		int i = 0;
		int spaceNum = 0;
		for (i = 0; i < lineLen; i++) {
			if (lineBuf[i] == ' ') {
				spaceNum++;
			}

			if (spaceNum == 3) {
				break;
			}
		}

		pFrameStr = &lineBuf[i+1];

		int cnt = sscanf(pFrameStr, "%x%x%x%x%x%x%x%x%x",&id,&data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]);
		dlc = cnt -1;

		if (id <= 0x7FF) {
			canTxMsg.msg.StdId = id;
			canTxMsg.msg.ExtId = 0;
			canTxMsg.msg.IDT = CAN_Id_Standard;
		} else {
			canTxMsg.msg.StdId = 0;
			canTxMsg.msg.ExtId = id;
			canTxMsg.msg.IDT = CAN_Id_Extended;
		}

		canTxMsg.msg.DLC = dlc;
		for (int k = 0; k < 8; k++) {
			canTxMsg.msg.Data[k] = data[k];
		}

#if 1
		printf("==> ID: %x DLC=%d Data:", id, dlc);
		for (int k = 0; k < 8; k++) {
			printf("%02x ", data[k]);
		}
		printf("\n");
#endif
	}
	return 0;
}

int C06TXController::parseTrcDataOneLine(char *pBuf, int len)
{
	return 0;
}

/*
 * 0 - OK
 *
 */
int C06TXController::checkConfigFile(void)
{
	int ret = 0;

	static bool tfStatusOld = false;
	bool tfStatus = SPI_SD_GetSDCard();

	if (tfStatusOld != tfStatus) {
		tfStatusOld = tfStatus;
		if (!tfStatus) {
			// Removed
			return -2;
		}
	} else {
		return -1;
	}

	/* SD Inserted */
	FATFS fs;
	char buf[128];

	FRESULT res;
	FIL file;
	u32 len=0;

	res = f_mount(0, &fs);
	if(res == FR_OK) {

	} else {
		printf("Mount Failed\n");
		return -1;
	}

	res = f_open(&file, "C06TX.cfg", FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK) {
		//			f_read(&file,buf,128,&len);
		char *outbuf;
		int i = 0;
		while (1) {
			memset(buf, 0, 128);
			i++;

			outbuf = fgets(buf, 128, &file);
			if (outbuf != 0) {
#if 0
				printf("Line %2d - len %3d: %s\n", i, strlen(buf), buf);
#endif
				parseCfgOneLine(buf, strlen(buf));
			} else {
				break;
			}
		}


		f_close(&file);
	}

	printf("Send Config - BaudRate = %d\n", dsc.baudRate);
	printf("Send Config - Data File = %s\n", dsc.dataFileName);
	printf("Send Config - Format = %d\n", dsc.dataFormat);
	printf("Send Config - Send = %d\n", dsc.sendControl);

	if (dsc.baudRate) {
		printf("Set BaudRate to %d\n", dsc.baudRate);
	    gpDrvCAN->setBaudrate(CAN1, dsc.baudRate, CAN_Mode_Normal);
	}

	return ret;
}

int C06TXController::sendDataFile(void)
{
	int sendCnt = 0;

	FATFS fs;
	char buf[128];

	FRESULT res;
	FIL file;
	u32 len=0;

	gpDrvLEDGreen->off();

	res = f_mount(0, &fs);
	if(res == FR_OK) {

	} else {
		printf("Mount Failed\n");
		return -1;
	}

	res = f_open(&file, dsc.dataFileName, FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK) {
		//			f_read(&file,buf,128,&len);
		char *outbuf;
		int i = 0;
		while (1) {
			memset(buf, 0, 128);
			i++;

			outbuf = fgets(buf, 128, &file);
			if (outbuf != 0) {

				canTxMsg.msg.DLC = 0;

				if (dsc.dataFormat == CAN_DATA_FORMAT_ASC) {
					parseAscDataOneLine(buf, strlen(buf));
				} else if (dsc.dataFormat == CAN_DATA_FORMAT_TRC) {
					parseTrcDataOneLine(buf, strlen(buf));

				} else if (dsc.dataFormat == CAN_DATA_FORMAT_CAN) {
					parseCanDataOneLine(buf, strlen(buf));

				}

				// Send
				if (canTxMsg.msg.DLC != 0) {
					gpDrvCAN->transmit(CAN1, &canTxMsg.msg, true);

				    sendCnt++;
				    if (sendCnt%20 == 0) {
				    	gpDrvLEDGreen->toggle();		// Blinking LED
				    }
				}

				// Delay
			    delay_1ms(1);

			} else {
				break;
			}
		}


		f_close(&file);
	} else {
		printf("CAN not open data file: %s\n", dsc.dataFileName);
		return -1;
	}



	return 0;
}

