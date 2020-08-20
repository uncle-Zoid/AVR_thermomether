#include "ringbuffer.h"


RingBuffer::RingBuffer()
    : head_ (0)
    , tail_ (0)
{}

void RingBuffer::seek(byte_t value)
{
    while ((buffer_[head_] != value) && (head_ != tail_)) inc(head_);
}

byte_t RingBuffer::at(byte_t pos)
{
    byte_t index = (head_ + pos) & BUFFER_MASK;
    return buffer_[index];
}

bool RingBuffer::push(const byte_t *data, int size)
{
    if (free() < size)
    {
        return false;
    }
    const byte_t *pdata = data;
    while (size)
    {
        buffer_[tail_] = *pdata++;
        inc(tail_);
        --size;
    }
    return true;
}

bool RingBuffer::pop(byte_t *data, int size)
{
    if (count() < size)
    {
        return false;
    }
    byte_t *pdata = data;
    while (size && !empty())
    {
        *pdata++ = buffer_[head_];
        inc(head_);
        -- size;
    }
    return true;
}

int RingBuffer::count() const
{
    return (BUFFER_MAX + tail_ - head_) & BUFFER_MASK;
}

int RingBuffer::free() const
{
    //        return BUFFER_MAX - count() - 1;
    return BUFFER_MASK - count();
}

bool RingBuffer::empty() const
{
    return tail_ == head_;
}

byte_t RingBuffer::inc(byte_t &val)
{
    val = (val + 1) & BUFFER_MASK;
    return val;
}
