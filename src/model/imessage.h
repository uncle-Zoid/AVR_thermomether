#ifndef IMESSAGE_H
#define IMESSAGE_H

using byte_t = unsigned char;

class IMessage
{
public:
    IMessage() = default;

    /**
    * @brief process this data immediately
    * @param data
    * @param size
    */
    virtual void notify (const byte_t *data, int size) = 0;
};

#endif // IMESSAGE_H
