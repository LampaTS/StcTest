#ifndef COMPORTREADER_H
#define COMPORTREADER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDataStream>

#define MSG_AMOUNT 4

class comPortReader : public QObject
{
    Q_OBJECT

    QSerialPort m_port;
    QByteArray m_data;

public:
    comPortReader();

signals:

    void portListChanged(QList<QSerialPortInfo>); // обновляет список портов

    void sendMessage(QString); // отправка сообщения в лог

public slots:

    void checkAvaliblePorts(); // проверить список портов

    void startListening(QString); // начать слушать порт

private slots:

    void readData(); // прочитать данные из порта

};

#endif // COMPORTREADER_H
