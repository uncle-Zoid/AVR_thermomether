#include "message.h"

#include <vector>
#include <algorithm>
#include <QDebug>
#include <QTime>
#include <iostream>

#include "controler.h"

Message::Message(const SerialConnectionParams &params, Controler *controler)
    : serial_    (params, this)
    , controler_ (controler)
{ }

const char* Message::findHead(const char *pdata, const char *pend)
{
    while ((*pdata != HEAD) && (pdata != pend)) ++pdata;
    return pdata;
}

void Message::process(const message_t &message)
{
    auto ts = QDateTime::currentMSecsSinceEpoch();
    switch (Commands(message[OFFSET_COMMAND]))
    {
    case Commands::SEND_POWER_MODE:
        controler_->powerMode_ = message[OFFSET_DATA];
        break;

    case Commands::SEND_TEMPERATURE:
    {
        int t = ((message[OFFSET_DATA] << 8) | message[OFFSET_DATA + 1]);
        controler_->temperatures_.push_back({t / 100.0, ts});
        break;
    }
    default:
        std::cout << qPrintable(QTime::currentTime().toString("hh:mm:ss")) << " " << std::hex;
        std::copy(message.begin(), message.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;

    }

    controler_->notify(Commands(message[OFFSET_COMMAND]));
}

void Message::notify(const byte_t *data, int size)
{
    if(!ringBuffer_.push(data, size))
    {
        qDebug("No space in buffer");
    }
    while (ringBuffer_.count() > 3)
    {
        ringBuffer_.seek(HEAD);
        byte_t messageSize = ringBuffer_.at(OFFSET_SIZE);

        message_t message (messageSize, 0x00);

        if(!ringBuffer_.pop(message.data(), message.size()))
        {
            break;
        }

        process(message);
    }
}

void Message::send(Commands command, byte_t *data, byte_t size)
{
    bufferOut_[0] = HEAD;
    bufferOut_[1] = (size + 3) & 0xFF;
    bufferOut_[2] = byte_t(command);

    if (data)
    {
        std::copy(data, data + size, bufferOut_ + 3);
    }
    serial_.send(bufferOut_, bufferOut_[1]);
}
