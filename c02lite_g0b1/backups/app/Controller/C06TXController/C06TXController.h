/*
 * Controller.h
 *
 *  Created on: 2013-11-4
 *      Author: Administrator
 */

#ifndef C06TXCONTROLLER_H_
#define C06TXCONTROLLER_H_

#include <includes.h>
#include <Car/Car.h>

#define MAX_LINE_LEN	128

class C06TXController {
public:


	enum {
		CAN_DATA_FORMAT_ASC = 0,
		CAN_DATA_FORMAT_TRC,
		CAN_DATA_FORMAT_CAN,
	};

	enum {
		SEND_CTRL_LOOP = 0,
		SEND_CTRL_ONESHOT,
	};

	struct dataSendContrl_t {
		char dataFileName[64];
		int dataFormat;
		int sendControl;
		int baudRate;
	};

	C06TXController(Car *pCar);
	~C06TXController(void);
	void run(void);

protected:
	int checkConfigFile(void);
	int parseCfgOneLine(char *pBuf, int len);

	int parseAscDataOneLine(char *pBuf, int len);
	int parseTrcDataOneLine(char *pBuf, int len);
	int parseCanDataOneLine(char *pBuf, int len);
	int sendDataFile(void);

private:
	Car *_pCar;
	bool isTJA1043;		// True: TJA1043; False: TJA1044

	int loopCnt;

	/*
	 * Data File Control
	 */
	struct dataSendContrl_t dsc;
	CanTMsg canTxMsg;

};

#endif /* C06TXCONTROLLER_H_ */
