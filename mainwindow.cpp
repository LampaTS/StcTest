#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updatePortsComboBox(QSerialPortInfo::availablePorts()); // заполняем список доступных портов в первый раз

    // автоматическое обновление доступных портов
    connect(&reader, SIGNAL(portListChanged(QList<QSerialPortInfo>)), this, SLOT(updatePortsComboBox(QList<QSerialPortInfo>)));

    // чтение сообщения
    connect(&reader, SIGNAL(sendMessage(QString)), this, SLOT(appendText(QString)));

    appendText("Simple COM-port listener, choose port and click 'connect'");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_m_connectButton_clicked()
{
    appendText("Starting listening, path = " + ui->m_portsComboBox->currentText());
    reader.startListening(ui->m_portsComboBox->currentText()); // начинаем слушать выбранный порт
}

void MainWindow::updatePortsComboBox(QList<QSerialPortInfo> ports)
{
    // сохраняем исходную опцию
    QString currentText = ui->m_portsComboBox->currentText();

    // обновляем список
    ui->m_portsComboBox->clear();
    for (QSerialPortInfo p : ports)
        ui->m_portsComboBox->addItem(p.systemLocation());

    // возвращаем исходную опцию обратно
    ui->m_portsComboBox->setCurrentText(currentText);
}

void MainWindow::appendText(QString text)
{
    ui->m_logTextEdit->appendPlainText(text);
}

