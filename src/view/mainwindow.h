#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Controler;
enum class Commands;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Controler &c, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_mcuUpdate(const Commands &c);
    void slot_updateValues();
    void slot_connectionStatus(bool connected);

    void on_pbSet_clicked();

private:
    Ui::MainWindow *ui;

    Controler &controler_;
    QTimer timer_;
    QSettings setti_;
    bool initGui_;
};
#endif // MAINWINDOW_H
