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

#include "InternalFlashUpdater.h"

#define FLASH_FLAG_ALL_ERRORS           (FLASH_FLAG_OPERR   | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | \
                                         FLASH_FLAG_PGAERR  | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR | \
                                         FLASH_FLAG_MISERR  | FLASH_FLAG_FASTERR | \
                                         FLASH_FLAG_OPTVERR | FLASH_FLAG_ECCC    | FLASH_FLAG_ECCD)

#define PAGE_SIZE   FLASH_PAGE_SIZE          /* 2KB Page */
#define	MAX_SIZE	0x08000000 + 0x020000 - 0x0800F000

/**
 * internalFlash升级滤波器
 * @param pupdatecfg
 * @return 成功返回0
 */
static int InternalFlashFilter(struct UpdateCfgEntry *pupdatecfg)
{
	//FATFS fs;
	FRESULT res;
	FIL file;
	u8 buffer[16];
	u32 len = 0;
	int ret = 0;
	/* Check Image Now */
	//res = f_mount(0, &fs);
	res = f_open(&file, (char const *)pupdatecfg->fileName, FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK) {
		printf("Open failed: %s\r\n", pupdatecfg->fileName);
		return -1;
	}
	res = f_read(&file,(char *)&buffer, PAGE_SIZE, &len);
	if(res != FR_OK) {
		printf("Read failed\r\n");
		return -1;
	}
	f_close(&file);

	return ret;
}

/**
 * internalFlash升级
 * @param pupdatecfg struct UpdateCfgEntr结构体指针
 * @return 成功返回0 不成功返回非0值
 */
int InternalFlashUpdate(struct UpdateCfgEntry *pupdatecfg)
{
	int ret = 0;
	ret = InternalFlashFilter(pupdatecfg);
	if(ret) {
		return ret;
	}
	//FATFS fs;
	FRESULT res;
	FIL file;
	u32 len = 0;
	u32 index = 0, offset = 0;
	u32 a[2] = {0,2};

	uint32_t status;
	FLASH_EraseInitTypeDef pEraseInit;

	uint32_t PageError = 0;
	pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	pEraseInit.Banks = FLASH_BANK_1;
	pEraseInit.NbPages = 1;

	u32 page_address = pupdatecfg->addr;
	uint8_t image_buffer[PAGE_SIZE];

	/* Unlock Flash */
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

	/* Mount File System */
	//res = f_mount(0, &fs);
	res = f_open(&file, (char const *)pupdatecfg->fileName, FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK) {
		index = 0;
		while(1)
		{
			page_address = pupdatecfg->addr + index;
			/* read one page */
			res = f_read(&file,(char *)&image_buffer, PAGE_SIZE, &len);
//			printf("res:%d\n",res);

			if (len == 0)
				break;

			/* Erase one page */
			pEraseInit.Page = ((page_address - 0x08000000) / PAGE_SIZE);
			printf("Page:%d\n",pEraseInit.Page);
			status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);

			if (status != HAL_OK) {
				printf("Erase int %d falled.\n",page_address);
				ret = -1;
				goto exit;
			}
			/* Program one page */
			for (offset = 0; offset < PAGE_SIZE; offset+=8) {
				status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, page_address+offset, *(uint64_t *)(&image_buffer[offset]));
				if (status != HAL_OK) {
					printf("Program int falled.\n");
					ret = -2;
					goto exit;
				}
			}
			/* update address offset */
			index += FLASH_PAGE_SIZE;
		}
		printf("SUCCESS.\r\n");
		ret =0;
	}
	exit:
	f_close(&file);
	HAL_FLASH_Lock();
	return ret;
}


