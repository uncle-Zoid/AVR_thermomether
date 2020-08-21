#include "view\mainwindow.h"

#include <QApplication>
#include <iostream>

#include "model/controler.h"
#include "model/DalasCrc.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    SerialConnectionParams params = {"COM3", 9600};
    Controler c(params);

    return a.exec();

//    DalasCrc crc;
//    unsigned char arr[] = {0xf7, 0x10, 0x0b, 0xff, 0x7f, 0x46, 0x4b, 0x01, 0xa5};

//    cout << hex << +crc.compute8(arr, 8) << endl;

    return 0;
}
