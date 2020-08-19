#ifndef SERIALLINE_H
#define SERIALLINE_H

class IMessage;

#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>

struct SerialConnectionParams
{
    QString com;
    int baudrate;
};

class SerialLine : public QObject
{
    Q_OBJECT

public:
    SerialLine(const SerialConnectionParams &connection, IMessage *notify);
    void send(const char *data, int size);

private slots:
    void slot_readyRead();
    void slot_error(QSerialPort::SerialPortError error);


private:
    IMessage *notify_;
    QSerialPort port_;
    bool open_;
};

#endif // SERIALLINE_H
