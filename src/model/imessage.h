#ifndef IMESSAGE_H
#define IMESSAGE_H

class IMessage
{
public:
    IMessage() = default;

    /**
    * @brief process this data immediately
    * @param data
    * @param size
    */
    virtual void notify (const char *data, int size) = 0;
};

#endif // IMESSAGE_H
