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
    double temperature;
    byte_t Th;
    byte_t Tl;
    byte_t resolution;
    byte_t crc;
};



class Controler : public QObject
{
    Q_OBJECT
    static constexpr const char *POWER_MODE[] =
    {
        "No sensor"
        ,"Parasitic"
        ,"External supply"
    };

public:
    explicit Controler();

    void notify(Commands command);
    void readInfo();

    const std::list<TemperatureEntry> &temperatures() const;
    const char *powerMode() const;
    bool serialStatus() const;
    void setPeriod(uint16_t period);
    bool connect(const SerialConnectionParams &params);

signals:
    void sig_mcuUpdate(const Commands&);
    void sig_connectionStatus(bool);


private:
    Message messageProceser_;
    std::list <TemperatureEntry> temperatures_;
    Scratchpad sensorScratchpad_;
    uint16_t measurePeriod_;
    byte_t powerMode_;
    byte_t sensorRom_[8];
    bool connected_;

    void setConnectionStatus(bool connected);

    friend class Message;
    friend class MainWindow;
};

#endif // CONTROLER_H
