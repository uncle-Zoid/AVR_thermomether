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
    switch (Commands(message[OFFSET_COMMAND]))
    {
    case Commands::SEND_POWER_MODE:
        controler_->powerMode_ = message[OFFSET_DATA];
        break;

    case Commands::SEND_ROM:
        std::copy(message.begin() + OFFSET_DATA, message.end(), controler_->sensorRom_);
        break;

    case Commands::SEND_SCRATCHPAD:
    {
        auto &ref = controler_->sensorScratchpad_;

        const byte_t *databegin = message.data() + OFFSET_DATA;
        const byte_t *dataend   = message.data() + OFFSET_DATA + 9;

        std::copy(databegin, dataend, ref.raw);

        ref.Th = databegin[ThRegister];
        ref.Tl = databegin[TlRegister];
        ref.temperature = temperature(ref.raw);
        ref.resolution = toResolution(databegin[ConfigRegister]);

        ref.crc = dalasCrc_.compute8(databegin, 9);

        break;
    }
    case Commands::SEND_TEMPERATURE:
    {
        int t = ((message[OFFSET_DATA] << 8) | message[OFFSET_DATA + 1]);
        controler_->sensorScratchpad_.temperature = t / 100.0;
        break;
    }

    case Commands::SET_MEASURE_PERIOD:
        controler_->measurePeriod_ = uint16_t((message[OFFSET_DATA] << 8) | message[OFFSET_DATA + 1]);
        break;

    default:
        std::cout << qPrintable(QTime::currentTime().toString("hh:mm:ss")) << " " << std::hex;
        std::copy(message.begin(), message.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }

    controler_->notify(Commands(message[OFFSET_COMMAND]));
}

byte_t Message::toResolution(byte_t configRegister)
{
    switch (configRegister)
    {
    case RESOLUTION_12B: return 12;
    case RESOLUTION_11B: return 11;
    case RESOLUTION_10B: return 10;
    case RESOLUTION_09B: return 9;
    }
    return 0;
}

double Message::temperature(byte_t *scratchpad)
{

    int sign = 1; // kladne znamenko
    int t = (scratchpad[TEMPERATURE_MSB] << 8) | scratchpad[TEMPERATURE_LSB];
    // je teplota zaporna?
    if (t & 0x8000)
    {
        t = (t ^ 0xFFFF) + 1; // dvojkovy doplnek
        sign = -1;
    }

    switch (scratchpad[ConfigRegister])
    {
        // vsechny bity platne ... rozliseni 0,0625
        case RESOLUTION_12B:
            return sign * double(t) * 0.0625;

        // bit 0 neni platny ... rozliseni 0,125
        case RESOLUTION_11B:
            t >>= 1;
            return sign * double(t) * 0.125;

        // bity 0 a 1 nejsou platne ... rozliseni 0,25
        case RESOLUTION_10B:
            t >>= 2;
            return sign * double(t) * 0.25;

        // bity 0, 1 a 2 nejsou platne ... rozliseni 0,5
        case RESOLUTION_09B:
            t >>= 3;
            return sign * double(t) * 0.5;
    }
    return 0;
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

bool Message::serialStatus() const
{
    return serial_.isOpen();
}
