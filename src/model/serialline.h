#ifndef SERIALLINE_H
#define SERIALLINE_H

class IMessage;

#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

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
    bool open(const SerialConnectionParams &connection);
    bool isOpen () const;

private slots:
    void slot_readyRead();
    void slot_error(QSerialPort::SerialPortError error);
    void slot_connect();


private:
    SerialConnectionParams connection_;
    IMessage *notify_;
    QSerialPort port_;
    QTimer timer_;
    bool open_;
    bool open();
};

#endif // SERIALLINE_H
