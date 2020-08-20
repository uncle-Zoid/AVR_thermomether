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
    QTimer::singleShot(11000, [&]()
    {
        messageProceser_.send(Commands::SEND_SCRATCHPAD);
//        uint16_t time = 0x0FFF;
//        messageProceser_.send(Commands::SET_MEASURE_PERIOD, (byte_t*)&time, 2);

        readInfo();
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
    switch (command)
    {
    case Commands::SEND_TEMPERATURE:
        printf("%s: Temperature is %.02f"
               , qPrintable(QDateTime::fromMSecsSinceEpoch(temperatures_.back().timestamp).toString("hh:mm:ss"))
               , temperatures_.back().temperature
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
        std::cout << "\tTemperature: "  << (sensorScratchpad_.temperature / 100.0) << std::endl;
        std::cout << "\tAlarm hight: "  << +sensorScratchpad_.Th << std::endl;
        std::cout << "\tAlarm low  : "  << +sensorScratchpad_.Tl << std::endl;
        std::cout << "\tResolution : "  << +sensorScratchpad_.resolution << "b" << std::endl;
        std::cout << "\tCRC        : "  << +sensorScratchpad_.crc << std::endl;


        break;
    }
}

void Controler::readInfo()
{
    messageProceser_.send(Commands::SEND_POWER_MODE);
    messageProceser_.send(Commands::SEND_ROM);
}
