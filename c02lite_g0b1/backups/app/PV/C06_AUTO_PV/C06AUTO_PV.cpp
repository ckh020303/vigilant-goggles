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

#include <Controller/C06Controller/C06Controller.h>
#include <CanDetector/CanDetector.h>
#include "../../../Zox/drivers/DrvFs/SPI_SD_driver.h"


#include "C06AUTO_PV.h"
#include <includes.h>
#include <CanDetector/CanDetector.h>

C06AUTO_PV::C06AUTO_PV() {
        pdevice.RCC_APB2Periph_MISO	= RCC_APB2PERIPH_GPIOA;
		pdevice.GPIO_Port_MISO 		= GPIOA;
		pdevice.GPIO_Pin_MISO		= GPIO_Pins_6;

		pdevice.RCC_APB2Periph_MOSI	= RCC_APB2PERIPH_GPIOA;
		pdevice.GPIO_Port_MOSI 		= GPIOA;
		pdevice.GPIO_Pin_MOSI		= GPIO_Pins_7;

		pdevice.RCC_APB2Periph_SCK	= RCC_APB2PERIPH_GPIOA;
		pdevice.GPIO_Port_SCK 		= GPIOA;
		pdevice.GPIO_Pin_SCK		= GPIO_Pins_5;

		pdevice.RCC_APB2Periph_nCS	= RCC_APB2PERIPH_GPIOA;
		pdevice.GPIO_Port_nCS 		= GPIOA;
		pdevice.GPIO_Pin_nCS		= GPIO_Pins_4;

		pdevice.RCC_APB2Periph_nCD	= RCC_APB2PERIPH_GPIOC;
		pdevice.GPIO_Port_nCD 		= GPIOC;
		pdevice.GPIO_Pin_nCD		= GPIO_Pins_4;
	// TODO Auto-generated constructor stub
}

C06AUTO_PV::~C06AUTO_PV() {
	// TODO Auto-generated destructor stub
}

void C06AUTO_PV::run(void)
{
    DrvFs *pfatfs = new DrvFs(&pdevice);

    int ret;
    u32 len=0;
    ret = pfatfs->init();
    if(ret != 0)
        printf("FatFs init fail");

    while(!SPI_SD_GetSDCard());

    ret = pfatfs->open(MSG_FILE_NAME);
    if(ret == FR_OK) {
		ret = pfatfs->read(buf,BUF_MAX_LEN,&len);
		pfatfs->close();
	}
	printf("len:%d,ret:%d\n",len,ret);
    int i = 0;
	if(len){
        for(len;len==0;len--){
            printf("%x",buf[i++]);
        }
	}
}

