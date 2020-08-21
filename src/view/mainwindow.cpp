#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "model/controler.h"

MainWindow::MainWindow(Controler &c, QWidget *parent)
    : QMainWindow(parent)
    , ui            (new Ui::MainWindow)
    , controler_    (c)
{
    ui->setupUi(this);

    QObject::connect(&controler_, &Controler::sig_mcuUpdate, this, &MainWindow::slot_mcuUpdate);

    controler_.readInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_mcuUpdate(const Commands &c)
{
    qInfo() << int(c);
    switch (c)
    {
    case Commands::SEND_SCRATCHPAD:
        auto &scr = controler_.sensorScratchpad_;
        ui->lbl_temperature->setText(QString::asprintf("%.02f °C", scr.temperature));
        ui->lbl_crc->setText(scr.crc == 0 ? "OK":"fail");

        QString rom = "| ";
        QTextStream stream(&rom);
        stream.setFieldWidth(2);
        stream.setIntegerBase(16);
        stream.setPadChar('0');
        stream.setNumberFlags(QTextStream::UppercaseDigits);
        for (int i = 0; i < 8; ++i)
        {
            stream  << controler_.sensorRom_[i] << " | ";
        }
        ui->lbl_rom->setText(rom);
        ui->lblpowerMode->setText(controler_.powerMode());
        ui->lbl_serialStatus->setText(controler_.serialStatus() ? "Connected" : "Disconnected");
        break;
    }
}

