#include "serialline.h"

#include <QDebug>
#include "imessage.h"

SerialLine::SerialLine(const SerialConnectionParams &connection, IMessage *notify)
    : notify_ (notify)
{
    port_.setPortName(connection.com);
    port_.setBaudRate(connection.baudrate);
    port_.setDataBits(QSerialPort::Data8);
    port_.setStopBits(QSerialPort::OneStop);
    port_.setParity(QSerialPort::NoParity);
    port_.setFlowControl(QSerialPort::NoFlowControl);
    open_ = port_.open(QIODevice::ReadWrite);

    QObject::connect(&port_, &QSerialPort::readyRead, this, &SerialLine::slot_readyRead);
    QObject::connect(&port_, &QSerialPort::errorOccurred, this, &SerialLine::slot_error);
}

void SerialLine::slot_readyRead()
{
    auto all = port_.readAll();
    notify_->notify(all.data(), all.size());
}

void SerialLine::slot_error(QSerialPort::SerialPortError error)
{
    qDebug() << error;
    // toho auto reconect a sig_con failed
}

void SerialLine::send(const char *data, int size)
{
    port_.write(data, size);
}
