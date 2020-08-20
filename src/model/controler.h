#ifndef CONTROLER_H
#define CONTROLER_H

#include "message.h"
#include <list>

struct TemperatureEntry
{
    double temperature;
    int64_t timestamp;
};


struct Scratchpad
{
    byte_t raw[9];
    uint16_t temperature;
    byte_t Th;
    byte_t Tl;
    byte_t resolution;
    byte_t crc;
};



class Controler
{
    static constexpr const char *POWER_MODE[] =
    {
        "No sensor"
        ,"Parasitic"
        ,"External supply"
    };

    static constexpr const char *MODE[] =
    {

    };

public:
    Controler(const SerialConnectionParams &params);

    void notify(Commands command);
    void readInfo();

private:
    Message messageProceser_;
    std::list <TemperatureEntry> temperatures_;
    byte_t powerMode_;
    byte_t sensorRom_[8];
    Scratchpad sensorScratchpad_;

    friend class Message;
};

#endif // CONTROLER_H
