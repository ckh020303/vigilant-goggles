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

#include "receiveStr.h"
#include <utils/Time/Time.h>
/****************************************************************************
 * 简介   :	在指定的时间范围内，从串口接收一个指定长度的字符串。
 * 			指定长度的字符串接收成功，或者超时，该函数返回。
 * 			该函数原样拷贝从串口获取的字符，不会在字符串后面添加包括'\0'在内的任何字符。
 * 参数   :	buf		保存字符串的缓冲区，长度必须不小于len
 * 			len		字符串的长度
 * 			timeout	超时值。单位：ms
 * 			pDrvUSART	所使用的串口驱动
 * 返回值 :	指定长度的字符串已经接收到	0
 * 			超时						1
 * 描述   :
 ****************************************************************************/
uint8_t	receiveStr(char *buf, uint8_t len, uint32_t timeout, DrvUSART *pDrvUSART)
{
	uint32_t start = Time::getTimems();
	uint32_t end;
	int i = 0;
        int ret;

	while (1) {
		char c;
		ret = pDrvUSART->readCharNonBlock(&c);
		if (ret == 0) {
			// 成功接收字符
			buf[i++] = c;
			if (i == len) {
				return 0;
			}
		} else {
			end = Time::getTimems();
			if (end - start > timeout) {
				return 1;
			}
		}
	}
}
