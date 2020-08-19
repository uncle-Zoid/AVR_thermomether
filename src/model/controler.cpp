#include "controler.h"

#include <QTimer>

Controler::Controler(const SerialConnectionParams &params)
    : messageProceser_(params)
{
    QTimer::singleShot(1000, [&]()
    {
        uint16_t time = 0x0FFF;
        messageProceser_.send((byte_t*)&time, 2, Commands::SET_MEASURE_PERIOD);
    });

}
