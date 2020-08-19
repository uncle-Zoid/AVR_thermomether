#ifndef MESSAGE_H
#define MESSAGE_H

#include <cinttypes>

#include "packet.h"
#include "imessage.h"
#include "serialline.h"
#include "ringbuffer.h"

enum class Commands{
      SEND_POWER_MODE = 1
    , SEND_ROM
    , SEND_SCRATCHPAD
    , SEND_TEMPERATURE
    , SET_AUTOMATIC_MEASURE
    , SET_SENSOR_PARAMS
    , SET_MEASURE_PERIOD
};




class Message : public IMessage
{
public:
    static constexpr byte_t BUFFER_MAX = 32;
    static constexpr byte_t HEAD = 0x1A;
    static constexpr byte_t OFFSET_SIZE    = 1;
    static constexpr byte_t OFFSET_COMMAND = 2;
    static constexpr byte_t OFFSET_DATA    = 3;

public:
    Message(const SerialConnectionParams &params);
    virtual ~Message() = default;

    Message( const Message &c ) = delete;
    Message& operator=( const Message &c ) = delete;


    template <class T>
    void send(T data, Commands command)
    {
        byte_t *pdataBegin = reinterpret_cast<byte_t*>(&data);
        send(pdataBegin, sizeof (T), command);
    }

    void send(byte_t *data, byte_t size, Commands command);
    void send(const Packet &pck);

    void receive(Packet &p);

private:
    SerialLine serial_;
    RingBuffer ringBuffer_;

    char bufferOut_[BUFFER_MAX];

    static const char *findHead(const char *pdata, const char *pend);
    void process(const QByteArray &message);

    // IMessage interface
public:
    void notify(const char *data, int size) override;
}; //Communicator

#endif // MESSAGE_H
