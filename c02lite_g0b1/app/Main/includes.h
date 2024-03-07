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


#ifndef  __INCLUDES_H
#define  __INCLUDES_H

#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>

#include "usart.h"

#include "version.h"

#define FLASH_MAX_SIZE 0x1FFFF

static char *banner =
		"\n********************************************************\n"
		"Guangzhou RoadPassion Electronics Technology Co., Ltd.\n"
		"Model: C02Lite Artery.\n"
		"Version: %s\n"
		"Date: %s\n"
		"********************************************************\n\n";

#ifdef __cplusplus
extern "C" {
#endif

void appMain(void);

#ifdef __cplusplus
}
#endif

#endif
