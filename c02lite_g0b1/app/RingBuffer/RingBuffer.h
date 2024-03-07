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


#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <list>

#include "../Main/includes.h"
/*
 * 内部文档
 *
 *  - 头指针(head)表示当前读的位置，尾指针(tail)表示当前写的位置。
 *  - 初始化:
 *      head = tail = 0;
 *      isFull = false;
 *  - 满靠isFull判断，空靠读的时候是否阻塞判断
 *  - 读写之前要确认满标志
 *  - 读写完后要更新满标志
 */
template <typename T, size_t size>
class RingBuffer {

private:
	T _buffer[size];
	int head, tail;		/* head for read, tail for write */
	int validCount;			/* total data count in buffer */
#ifdef ZDEBUG
	int overflow_count;
#endif
public:
	RingBuffer()
	{
		head = tail = 0;
		validCount = 0;
#ifdef ZDEBUG
		overflow_count = 0;
#endif
	}
	~RingBuffer() {}
	/*
	 * Next Position for Read or Write
	 */
	int getNextPos(int cur)
	{
		return (cur+1) == size ? 0 : cur+1;
	}

	/*
	 * Write Data to RingBuffer
	 * Return:
	 *  1  - success
	 *  0 - Full
	 */
	int write(T *content)
	{
		int ret = 1;
		if (validCount < size) {
			_buffer[tail] = *content;

			tail = getNextPos(tail);
			validCount++;
		} else {
			/* Buffer is FULL */
			ret = 0;
#ifdef ZDEBUG
			overflow_count++;
//			printf("CAN Buffer Overflow: %d\n",overflow_count);
#endif
		}
		return ret;
	}

	/*
	 * Read Data from RingBuffer
	 * Return:
	 *   1 - success
	 *   0 - Empty
	 */
	int read(T* buf)
	{
		int ret = 1;
		if (validCount > 0) {
			*buf = _buffer[head];

			head = getNextPos(head);
			validCount--;
		} else {
			/* Buffer is Empty */
			ret = 0;
		}
		return ret;
	}

	void clear(void)
	{
		head = tail = 0;
		validCount = 0;
	}
#ifdef ZDEBUG
	void showRingBufferInfo(void)
	{
		printf("RingBuffer: Head=%d Tail=%d Valid=%d Overflow=%d\n", head, tail, validCount, overflow_count);
	}
#endif
};

#endif /* RINGBUFFER_H_ */
