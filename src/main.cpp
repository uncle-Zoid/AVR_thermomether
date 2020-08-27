#include "view\mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <iostream>

#include "model/controler.h"
#include "model/DalasCrc.h"
#include "ansicolors.h"

using namespace std;

static const char*      APP_VERSION             = "1.1.2";
static const char*      APP_DATE                = "21. 8. 2020";
static const char*      APP_NAME                = "avr-thermomether";

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(QString("%1 - %2").arg(APP_VERSION).arg(APP_DATE));


    QApplication a(argc, argv);   
    Controler c;

    MainWindow w(c);
    w.show();

    return a.exec();

//    DalasCrc crc;
//    unsigned char arr[] = {0xf7, 0x10, 0x0b, 0xff, 0x7f, 0x46, 0x4b, 0x01, 0xa5};

//    cout << hex << +crc.compute8(arr, 8) << endl;

}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    fprintf(stdout, ANSI_COLOR_GREEN "%s " ANSI_COLOR_RESET, qPrintable(QDateTime::currentDateTimeUtc().toString("HH:mm:ss.zzz")));
    fprintf(stdout, ANSI_COLOR_MAGENTA "[%4d]", context.line);
    fprintf(stdout, ANSI_COLOR_CYAN "%-50.50s", context.function);
    fprintf(stdout, ANSI_COLOR_RESET " : ");

    switch (type)
    {
    case QtDebugMsg:
        fprintf(stdout, COLOR_DEBUG "%s", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stdout,  COLOR_INFO "%s", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stdout, COLOR_WARN "%s", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stdout, COLOR_CRITICAL "%s", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stdout, ANSI_COLOR_INVERSE COLOR_CRITICAL "%s", localMsg.constData());
        break;
    }
    fprintf(stdout, ANSI_COLOR_RESET "\n");

    fflush(stdout);
}
