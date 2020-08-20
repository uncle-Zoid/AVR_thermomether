#ifndef RINGBUFFER_H
#define RINGBUFFER_H

using byte_t = unsigned char;

class RingBuffer
{
    static constexpr byte_t BUFFER_MAX  = 128;
    static constexpr byte_t BUFFER_MASK = BUFFER_MAX - 1;

public:
    RingBuffer();

    void seek(byte_t value);
    byte_t at(byte_t pos);

    bool push(const byte_t *data, int size);
    bool pop (byte_t *data, int size);
    int count() const;
    int free() const;
    bool empty() const;

private:
    byte_t head_; ///< cteci konec
    byte_t tail_; ///< zapisovaci konec
    byte_t buffer_[BUFFER_MAX];

    byte_t inc(byte_t &val);
};

#endif // RINGBUFFER_H
