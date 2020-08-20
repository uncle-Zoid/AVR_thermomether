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
    QTimer::singleShot(1000, [&]()
    {
        uint16_t time = 0x0FFF;
        messageProceser_.send(Commands::SET_MEASURE_PERIOD, (byte_t*)&time, 2);

        readInfo();
    });

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
    }
}

void Controler::readInfo()
{
    messageProceser_.send(Commands::SEND_POWER_MODE);
    messageProceser_.send(Commands::SEND_ROM);
}
