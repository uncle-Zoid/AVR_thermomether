#include "view\mainwindow.h"

#include "model/controler.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    SerialConnectionParams params = {"COM3", 9600};
    Controler c(params);

    return a.exec();
}
