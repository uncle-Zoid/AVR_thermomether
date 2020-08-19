#include "message.h"

#include <algorithm>
#include <QDebug>
#include <QTime>

Message::Message(const SerialConnectionParams &params)
    : serial_(params, this)
{ }

const char* Message::findHead(const char *pdata, const char *pend)
{
    while ((*pdata != HEAD) && (pdata != pend)) ++pdata;
    return pdata;
}

void Message::process(const QByteArray &message)
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << message.toHex();

}

void Message::notify(const char *data, int size)
{
    if(!ringBuffer_.push(data, size))
    {
        qDebug("No space in buffer");
    }
    while (ringBuffer_.count() > 3)
    {
        ringBuffer_.seek(HEAD);
        byte_t messageSize = ringBuffer_.at(OFFSET_SIZE);

        QByteArray message (messageSize, 0x00);

        if(!ringBuffer_.pop(message.data(), message.size()))
        {
            break;
        }

        process(message);
    }
}

void Message::send(byte_t *data, byte_t size, Commands command)
{
    bufferOut_[0] = HEAD;
    bufferOut_[1] = (size + 3) & 0xFF;
    bufferOut_[2] = byte_t(command);

    std::copy(data, data + size, bufferOut_ + 3);

    serial_.send(bufferOut_, bufferOut_[1]);
}
