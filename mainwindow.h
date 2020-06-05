#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "comportreader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    comPortReader reader;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void updatePortsComboBox(QList<QSerialPortInfo>); // обновляет список доступных портов

    void appendText(QString); // добавляет текст в лог

private slots:
    void on_m_connectButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
