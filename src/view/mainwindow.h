#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    Controler &controler_;
};
#endif // MAINWINDOW_H
