#ifndef PACKET_H
#define PACKET_H

using byte_t = unsigned char;

// | HEAD | LEN | COMMAND | data
struct Packet
{
    static constexpr byte_t MaxSize = 32;
    Packet ()
        : size  (-1)
        , error (0)
    {}

    bool isValid () const
    {
        return (size > 0) && (error == 0);
    }

    byte_t maxSize()
    {
        return MaxSize;
    }

    byte_t data[MaxSize];
    int size;
    int error;
};

#endif // PACKET_H
