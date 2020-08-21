#include "controler.h"

#include <QTimer>
#include <QDateTime>
#include <iostream>

using namespace std;

constexpr const char *Controler::POWER_MODE[];

Controler::Controler(const SerialConnectionParams &params)
    : messageProceser_(params, this)
    , powerMode_      (0)
{
    static QTimer t;
    QObject::connect(&t, &QTimer::timeout, [&](){messageProceser_.send(Commands::SEND_SCRATCHPAD); t.setInterval(1000);});
//    t.start(10000);

    QTimer::singleShot(10, [&]()
    {
//        messageProceser_.send(Commands::SEND_TEMPERATURE);
//        messageProceser_.send(Commands::SEND_SCRATCHPAD);
//        uint16_t time = 0x0FFF;
//        messageProceser_.send(Commands::SET_MEASURE_PERIOD, (byte_t*)&time, 2);

//        readInfo();
    });
    //QTimer::singleShot(2000, [&]()
    //{
    //    byte_t d[4] = {0x10, 0x05, 0x1F, true};
    //    messageProceser_.send(Commands::SET_SENSOR_PARAMS, d, 4);
    //
//  //      _sleep(1);
    //    messageProceser_.send(Commands::SEND_SCRATCHPAD);
    //});

}

void Controler::notify(Commands command)
{
    auto ts = QDateTime::currentMSecsSinceEpoch();

    switch (command)
    {
    case Commands::SEND_TEMPERATURE:
        temperatures_.push_back({sensorScratchpad_.temperature, ts});
        printf("%s: Temperature is %.02f"
               , qPrintable(QDateTime::fromMSecsSinceEpoch(ts).toString("hh:mm:ss"))
               , sensorScratchpad_.temperature
               );
        cout << " °C" << endl;
        break;

    case Commands::SEND_POWER_MODE:
        cout << "Power mode is: " << POWER_MODE[powerMode_] << std::endl;
        break;

    case Commands::SEND_ROM:
        cout << "Sensor ROM is: " << hex;
        std::copy(sensorRom_, sensorRom_ + 8, std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
        break;

    case Commands::SEND_SCRATCHPAD:
        cout << "Sensor scratchpad is: " << hex;
        std::copy(sensorScratchpad_.raw, sensorScratchpad_.raw + 9, std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl << dec;
        std::cout << "\tTemperature: "  << sensorScratchpad_.temperature << " °C" << std::endl;
        std::cout << "\tAlarm hight: "  << +sensorScratchpad_.Th << std::endl;
        std::cout << "\tAlarm low  : "  << +sensorScratchpad_.Tl << std::endl;
        std::cout << "\tResolution : "  << +sensorScratchpad_.resolution << "b" << std::endl;
        std::cout << "\tCRC        : "  << +sensorScratchpad_.crc << std::endl;

        temperatures_.push_back({sensorScratchpad_.temperature, ts});
        break;

    default:;
    }

    if(temperatures_.size() > 1e4)
    {
        temperatures_.pop_front();
    }

    emit sig_mcuUpdate(command);
}

void Controler::readInfo()
{
    messageProceser_.send(Commands::SEND_POWER_MODE);
    messageProceser_.send(Commands::SEND_ROM);
}

const std::list<TemperatureEntry> &Controler::temperatures() const
{
    return temperatures_;
}

const char *Controler::powerMode() const
{
    return POWER_MODE[powerMode_];
}

bool Controler::serialStatus() const
{
    return messageProceser_.serialStatus();
}
