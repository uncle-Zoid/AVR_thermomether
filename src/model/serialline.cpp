#include "serialline.h"

#include <QDebug>
#include "imessage.h"

SerialLine::SerialLine(const SerialConnectionParams &connection, IMessage *notify)
    : connection_   (connection)
    , notify_       (notify)
{

    QObject::connect(&port_, &QSerialPort::readyRead, this, &SerialLine::slot_readyRead);
    QObject::connect(&port_, &QSerialPort::errorOccurred, this, &SerialLine::slot_error);

    QObject::connect(&timer_, &QTimer::timeout, this, &SerialLine::slot_connect);

    open();
}

bool SerialLine::open()
{
    port_.setDataBits(QSerialPort::Data8);
    port_.setStopBits(QSerialPort::OneStop);
    port_.setParity(QSerialPort::NoParity);
    port_.setFlowControl(QSerialPort::NoFlowControl);
    port_.setPortName(connection_.com);
    port_.setBaudRate(connection_.baudrate);
    open_ = port_.open(QIODevice::ReadWrite);

    return open_;
}
bool SerialLine::open(const SerialConnectionParams &connection)
{
    connection_ = connection;
    return open();
}

bool SerialLine::isOpen() const
{
    return open_;
}

void SerialLine::slot_readyRead()
{
    auto all = port_.readAll();
    notify_->notify(reinterpret_cast<const uint8_t*>(all.data()), all.size());
}

void SerialLine::slot_error(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError)
    {
        open_ = false;
        if (port_.isOpen())
        {
            port_.close();
        }
        if (!timer_.isActive())
        {
            timer_.start();
        }
        qDebug() << "Serial error: " << error;
    }
}

void SerialLine::slot_connect()
{
    if (timer_.interval() < 1000)
    {
        timer_.setInterval(1000);
    }
    if (open())
    {
        timer_.stop();
    }
}

void SerialLine::send(const char *data, int size)
{
    port_.write(data, size);
}
