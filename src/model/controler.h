#ifndef CONTROLER_H
#define CONTROLER_H

#include "message.h"
#include <list>

struct TemperatureEntry
{
    double temperature;
    int64_t timestamp;
};

class Controler
{
    static constexpr const char *POWER_MODE[] =
    {
        "No sensor"
        ,"Parasitic"
        ,"External supply"
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

    friend class Message;
};

#endif // CONTROLER_H
