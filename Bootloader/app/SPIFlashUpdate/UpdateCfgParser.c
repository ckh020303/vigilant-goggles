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

#include "UpdateCfgParser.h"
#include <stdio.h>
/**
 * 从输入的字符串中获取待升级的文件的描述结构体数组
 * @param update_buf 升级描述字符串
 * @param len 升级描述字符串的长度
 * @param pupdateCfg 待升级的文件的描述结构体数组
 * @return 待升级的文件数量
 */
u8 UpdateCfgParser(char *update_buf,u32 len,struct UpdateCfgEntry* pupdateCfg)
{
	unsigned mark = 0;
	unsigned state = 0;  
        unsigned i = 0;
	unsigned j = 0;
	unsigned k=0;

	u8 image_flashaddr[MAX_FLASHADDR_LEN];
	u8 image_flashkind[MAX_FLASHKIND_LEN];
	u8  cfg_num = 0;
	for(k=0;k<len+1;k++) {
		if(k==len) update_buf[k] = 0x0d;
		if( (update_buf[k] == '@') && (state ==0)) {
			unsigned i=0;
			for(j=0;j<(k-mark);j++) {
				if( (update_buf[mark+j]!= 0x20) && (update_buf[mark+j] != 0x0a) &&(update_buf[mark+j] != 0x0d))
					pupdateCfg[cfg_num].fileName[i++] = update_buf[mark+j];
			}
			pupdateCfg[cfg_num].fileName[i] = 0x00;
			mark = k+1;
			state = 1;
		} else if( (update_buf[k] == '.') && (state ==1)) {
			for(j=0;j<(k-mark);j++) {
				if( (update_buf[mark+j]!= 0x20) && (update_buf[mark+j] != 0x0a))
					image_flashkind[i++] = update_buf[mark+j];
			}
			image_flashkind[i] = 0x00;
			mark = k+1;
			state = 2;
		} else if((update_buf[k] == 0x0d) && (state==2) ) {
			unsigned i=0;
			for(j=0;j<(k-mark);j++) {
				if( (update_buf[mark+j]!= 0x20) && (update_buf[mark+j] != 0x0a))
					image_flashaddr[i++] = update_buf[mark+j];
			}
			image_flashaddr[i] = 0x00;
			sscanf((const char*)image_flashaddr,"0x%x",&pupdateCfg[cfg_num].addr);

			if(strncmp("INTERNAL",image_flashkind,8) ==0)
				pupdateCfg[cfg_num].type = INTERNAL;
			else if(strncmp("SPI",image_flashkind,3) ==0 )
				pupdateCfg[cfg_num].type =  SPI;
			else if(strncmp("USART",image_flashkind,5) ==0 )
				pupdateCfg[cfg_num].type =  USART;
			else if(strncmp("PASSWORD",image_flashkind,8) ==0)
				pupdateCfg[cfg_num].type =  PASSWORD;
			else if(strncmp("CAN",image_flashkind,3) ==0 )
				pupdateCfg[cfg_num].type =  CAN;
                        else if(strncmp("TP6806",image_flashkind,6) ==0 )
				 pupdateCfg[cfg_num].type = TP6806;
			else pupdateCfg[cfg_num].type = UNKNOW;

			if(strncmp("//",pupdateCfg[cfg_num].fileName,2)==0) {
				printf("Pass OneLine...\r\n");
			} else {
				printf("Name: %s \r\n", pupdateCfg[cfg_num].fileName);
				printf("Kind: %s \r\n", image_flashkind);
				printf("Addr: %s = %d \r\n",image_flashaddr,pupdateCfg[cfg_num].addr);

				cfg_num++;
			}
			mark = k+1;
			state = 0;
		}
	}
	printf("%d files got !!.\r\n",cfg_num);
	return cfg_num;
}

