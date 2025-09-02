#ifndef __H_RING_BUFFER_H__
#define __H_RING_BUFFER_H__

#include <stdlib.h>
#include <cstring>
#include <mutex>

namespace rpp
{

class RingBuffer
{
  public:
    RingBuffer() : recv_buf(NULL), tail(0), head(0), uart_length(0), buffer_size(0)
    {
    }
    ~RingBuffer()
    {
        if (recv_buf != NULL)
        {
            delete[] recv_buf;
        }
    }
    // init serial ring handler_buf
    void initSerialRingBuffer(uint32_t size);
    /* 读队列 */
    bool readSerialRingBuffer(uint8_t *data);
    /* 写队列 */
    bool writeSerialRingBuffer(uint8_t data);
    /* 获取当前队列有效数据（未处理数据）长度 */
    uint32_t getSerialRingBuffer();

  private:
    uint32_t tail;        /* 队列尾，入队时需要自加 */
    uint32_t head;        /* 队列头，出队时需要自减 */
    uint32_t uart_length; /* 保存的是当前队列有效数据的长度 */
    uint32_t buffer_size; /* 保存的是当前队列有效数据的长度 */
    uint8_t *recv_buf;    /* 数据缓冲区 */
    std::mutex mutex_;
};

} // namespace rpp

#endif // __H_RING_BUFFER_H__