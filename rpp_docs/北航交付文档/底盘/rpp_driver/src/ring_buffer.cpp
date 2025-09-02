#include "rpp_driver/ring_buffer.h"

namespace rpp
{

void RingBuffer::initSerialRingBuffer(uint32_t size)
{
    std::lock_guard<std::mutex> lock(mutex_);
    head = 0;
    tail = 0;
    uart_length = 0;
    buffer_size = size;
    if (recv_buf != NULL)
    {
        delete[] recv_buf;
    }
    recv_buf = new uint8_t[size];
    memset(recv_buf, 0, sizeof(recv_buf));
}

/* 读队列 */
bool RingBuffer::readSerialRingBuffer(uint8_t *data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (uart_length == 0)
    {
        return false;
    }
    *data = recv_buf[head];
    recv_buf[head] = 0;
    head++;
    head %= buffer_size; /* 防止数组溢出 */
    uart_length--;
    return true;
}

/* 往队列里面写 */
bool RingBuffer::writeSerialRingBuffer(uint8_t data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (uart_length >= buffer_size)
    {
        return false;
    }
    recv_buf[tail] = data;
    tail++;
    tail %= buffer_size; /* 防止数组溢出 */
    uart_length++;
    return true;
}

/* 获取当前队列有效数据（未处理数据）长度 */
uint32_t RingBuffer::getSerialRingBuffer()
{
    uint32_t length;
    std::lock_guard<std::mutex> lock(mutex_);
    length = uart_length;
    return length;
}

} // namespace rpp