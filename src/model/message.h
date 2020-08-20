#ifndef MESSAGE_H
#define MESSAGE_H

#include <cinttypes>

#include "packet.h"
#include "imessage.h"
#include "serialline.h"
#include "ringbuffer.h"

class Controler;

enum class Commands{
      SEND_POWER_MODE = 1
    , SEND_ROM
    , SEND_SCRATCHPAD
    , SEND_TEMPERATURE
    , SET_AUTOMATIC_MEASURE
    , SET_SENSOR_PARAMS
    , SET_MEASURE_PERIOD
};


using message_t = std::vector<byte_t>;

class Message : public IMessage
{
public:
    static constexpr byte_t BUFFER_MAX = 32;
    static constexpr byte_t HEAD = 0x1A;
    static constexpr byte_t OFFSET_SIZE    = 1;
    static constexpr byte_t OFFSET_COMMAND = 2;
    static constexpr byte_t OFFSET_DATA    = 3;

public:
    Message(const SerialConnectionParams &params, Controler *controler);
    virtual ~Message() = default;

    Message( const Message &c ) = delete;
    Message& operator=( const Message &c ) = delete;


    template <class T>
    void send(Commands command, T data)
    {
        byte_t *pdataBegin = reinterpret_cast<byte_t*>(&data);
        send(command, pdataBegin, sizeof (T));
    }

    void send(Commands command, byte_t *data = nullptr, byte_t size = 0);
    void send(const Packet &pck);

    void receive(Packet &p);

private:
    SerialLine serial_;
    RingBuffer ringBuffer_;
    Controler *controler_;

    char bufferOut_[BUFFER_MAX];

    static const char *findHead(const char *pdata, const char *pend);
    void process(const message_t &message);

    // IMessage interface
public:
    void notify(const byte_t *data, int size) override;
}; //Communicator

#endif // MESSAGE_H
