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


#include "SPIFLashUpdater.h"

#define BUF_SIZE 2048

/**
 * SPIFlash升级
 * @param pupdatecfg 升级描述结构体指针
 * @return 成功返回非0值
 */
int SPIFlashUpdate(struct UpdateCfgEntry *pupdatecfg)
{
	FRESULT res;
	FIL file;
//	FATFS fs;
	static u8 buffer[BUF_SIZE];
	u32 len;
	u32 addr_read = pupdatecfg->addr;

//	res = f_mount(0, &fs);
	res = f_open(&file,(char const *)pupdatecfg->fileName, FA_OPEN_EXISTING | FA_READ);
	printf("fileName:%s\n",pupdatecfg->fileName);
	if(res != FR_OK) {
		printf("Open failed: %s\r\n", pupdatecfg->fileName);
		return -1;
	}
	u32 CRC_READ=0;
	u32 CRC_WRITE=0;
	while(1) {
		res = f_read(&file,(char *)&buffer, BUF_SIZE, &len);
		printf("res:%d\n",res);
		if (len == 0)
			break;
		for (int i = 0; i<len; ) {//计算待升级文件内容的 CRC 校验值（CRC_WRITE），读取文件内容，并分块写入 SPI Flash 中，每次写入 BUF_SIZE 大小的数据块，并在写入后擦除下一块的扇区。
			if((len-i)/4 !=0) {
				CRC_WRITE += *(u32*)(buffer+i);
				i +=4;
			} else {
				CRC_WRITE += buffer[i];
				i +=1;
			}
		}
		SPI_FLASH_SectorErase(pupdatecfg->addr);
		printf("len:%d\n",len);
		SPI_FLASH_BufferWrite(buffer,pupdatecfg->addr, len);
		pupdatecfg->addr += len;
		printf("111\n");
	}
	f_close(&file);
	printf("%s updated.\r\n", pupdatecfg->fileName);
	len = file.obj.objsize / BUF_SIZE;
	if(len !=0) {
		for (int i = 0; i < len; i++) {
			SPI_FLASH_BufferRead( buffer, addr_read , BUF_SIZE);
			addr_read += BUF_SIZE;
                        
			for (int j=0;j<BUF_SIZE;) {
				CRC_READ += *(u32*)(buffer+j);
				j +=4;
			}
		}
	}
	printf("22\n");
	len = file.obj.objsize % BUF_SIZE;

	SPI_FLASH_BufferRead( buffer, addr_read , len);
	for (int j=0;j< len;) {
		if((len-j)/4 !=0) {//再次读取 SPI Flash 中已写入的数据，计算读取数据的 CRC 校验值（CRC_READ）并与之前写入前的校验值进行对比
			CRC_READ += *(u32*)(buffer+j);
			j +=4;
		} else {
			CRC_READ += buffer[j];
			j +=1;
		}
	}
	printf("222\n");
//	if(CRC_READ == CRC_WRITE) {//若 CRC_READ 与 CRC_WRITE 相等，则说明升级过程中数据未发生改变，更新成功
//		printf("the same \r\n");
//	} else {
//		printf("not same \r\n");
//		return -2;
//	}

	return 0;
}


