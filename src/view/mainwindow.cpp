#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "model/controler.h"

MainWindow::MainWindow(Controler &c, QWidget *parent)
    : QMainWindow(parent)
    , ui            (new Ui::MainWindow)
    , controler_    (c)
    , initGui_      (true)
{
    ui->setupUi(this);

    QObject::connect(&controler_, &Controler::sig_mcuUpdate, this, &MainWindow::slot_mcuUpdate);
    QObject::connect(&timer_, &QTimer::timeout, this, &MainWindow::slot_updateValues);


    ui->leThSet->setValidator(new QIntValidator());

//    SerialConnectionParams params = {"COM3", 9600};
    controler_.connect({"COM3", 9600});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_connectionStatus(bool connected)
{
    if (connected)
    {
        int period = setti_.value("period", 1000).toInt();
        controler_.setPeriod(period & 0xFFFF);
        controler_.readInfo();

        initGui_ = true;
    }
}

void MainWindow::slot_mcuUpdate(const Commands &c)
{
    slot_updateValues();
}

void MainWindow::slot_updateValues()
{
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

    ui->leResolution->setText(QString("%1 b").arg(scr.resolution));
    ui->leThConfirm->setText(QString::number(scr.Th));
    ui->leTlConfirm->setText(QString::number(scr.Tl));
    ui->lePeriodConfirm->setText(QString::number(controler_.measurePeriod_));

    if (initGui_)
    {
        ui->cbResolution->setCurrentIndex(scr.resolution - 9);
        ui->leThSet->setText(QString::number(scr.Th));
        ui->leTlSet->setText(QString::number(scr.Tl));
        ui->lePeriodSet->setText(QString::number(controler_.measurePeriod_));
        initGui_ = false;
    }
}

void MainWindow::on_pbSet_clicked()
{
    controler_.setPeriod(10000);
}
